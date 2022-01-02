//
// Created by Mateusz Pysera on 01/01/2022.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdatomic.h>

#define MIN_THREADS (int)2
#define MAX_THREADS (int)100

enum direction {
    INC = 0, DEC = 1
};

struct thread_info {
    pthread_t thread_id;
    uint8_t queue_slot;
};

// Globals
enum direction close_order;
pthread_mutex_t mutex;

void *thread_run(void *arg) {
    struct thread_info const *t_info = arg;
    // critical section with mutex

    /*
     * for(...) {
     *     // thread.join in dec or inc order
     * }
     */
    printf("Watek z queue=%d zakonczyl prace\n", t_info->queue_slot);
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

    int ret;
    // TODO : Add thread attributes
    for (int i = 0; i < threads_amount; i++) {
        t_info[i].queue_slot = (uint8_t) i;
        ret = pthread_create(&t_info[i].thread_id, NULL, thread_run, &t_info[i]);
        if (ret != 0) {
            printf("Nie udalo sie stworzyc watku nr %d\n", i);
            free(t_info);
            exit(EXIT_FAILURE);
        }
    }
    free(t_info);
    return 0;
}

