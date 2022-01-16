//
// Created by Mateusz Pysera on 01/01/2022.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdatomic.h>
#include <assert.h>
#include <unistd.h>

#define MIN_THREADS (int)3
#define MAX_THREADS (int)100

enum direction {
    INC = 0, DEC = 1
};

struct thread_info {
    pthread_t thread_id;
    uint8_t queue_slot;
    _Atomic int *next_to_close;
};

// Globals
enum direction close_order;
pthread_mutex_t mutex;
int increment_me = 0;

void *thread_run(void *arg) {
    struct thread_info *t_info = arg;

    // wait for random period of time
    sleep(rand() % 10);

    pthread_mutex_lock(&mutex);
    increment_me++;
    pthread_mutex_unlock(&mutex);

    while (t_info->queue_slot != *t_info->next_to_close) {
        //printf("Thread %d waiting in spinlock\n", t_info->queue_slot);
    }
    printf("Watek z queue=%d zakonczyl prace\n", t_info->queue_slot);
    if (close_order == INC)
        *t_info->next_to_close = *t_info->next_to_close + 1;
    else
        *t_info->next_to_close = *t_info->next_to_close - 1;
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Program wykonany z bledna iloscia argumentow\n");
        exit(EXIT_FAILURE);
    }

    long threads_amount = strtol(argv[1], NULL, 10);
    if (threads_amount < MIN_THREADS || threads_amount > MAX_THREADS) {
        printf("Liczba watkow jest nieprawidlowa\n");
        exit(EXIT_FAILURE);
    }

    if (strcmp(argv[2], "inc") == 0) {
        close_order = INC;
    } else if (strcmp(argv[2], "dec") == 0) {
        close_order = DEC;
    } else {
        printf("Prosze wybrac inc / dec. %s nie jest poprawne\n", argv[2]);
        exit(EXIT_FAILURE);
    }

    if (pthread_mutex_init(&mutex, NULL) != 0) {
        printf("Nie udalo sie utworzyc mutexa\n");
        exit(EXIT_FAILURE);
    }

    struct thread_info *t_info = malloc(threads_amount * sizeof(*t_info));
    if (t_info == NULL) {
        printf("Bad alloc");
        exit(EXIT_FAILURE);
    }

    srand(time(NULL));

    int ret;
    _Atomic int ntc = (close_order == INC) ? 0 : threads_amount - 1;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    for (int i = 0; i < threads_amount; i++) {
        t_info[i].queue_slot = (uint8_t) i;
        t_info[i].next_to_close = &ntc;
        ret = pthread_create(&t_info[i].thread_id, &attr, thread_run, &t_info[i]);
        printf("Uruchomiono watek nr %d\n", i);
        if (ret != 0) {
            printf("Nie udalo sie stworzyc watku nr %d\n", i);
            free(t_info);
            pthread_attr_destroy(&attr);
            exit(EXIT_FAILURE);
        }
    }
    printf("Zakonczono tworzenie watkow\n");
    for (int i = 0; i < threads_amount; ++i) {
        pthread_join(t_info[i].thread_id, NULL);
    }
    pthread_attr_destroy(&attr);
    free(t_info);

    printf("increment_me = %d\n", increment_me);
    assert(increment_me == threads_amount);

    return 0;
}

