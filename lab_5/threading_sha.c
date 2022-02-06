//
// Created by Mateusz Pysera on 16/01/2022.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "sha256.h"
#include <signal.h>

#define MIN_THREADS (int)3
#define MAX_THREADS (int)100

struct thread_info {
    pthread_t thread_id;
    uint8_t queue_slot;
};

// Globals
pthread_mutex_t mutex;
_Atomic int amount_of_confirmations = -1;
BYTE result[SHA256_BLOCK_SIZE];

void *thread_run(void *arg) {
    struct thread_info const *t_info = arg;

    // calculate sha256
    BYTE index[] = {"236439"};
    SHA256_CTX ctx;
    BYTE buffer[SHA256_BLOCK_SIZE];

    sha256_init(&ctx);
    sha256_update(&ctx, index, 6);
    sha256_final(&ctx, buffer);

    pthread_mutex_lock(&mutex);

    if (amount_of_confirmations < 3) {
        // store calculated value to global variable if not set already
        if (amount_of_confirmations == -1) {
            memcpy(result, buffer, SHA256_BLOCK_SIZE);
            amount_of_confirmations++;
        } else {
            if (memcmp(result, buffer, SHA256_BLOCK_SIZE) == 0) {
                amount_of_confirmations++;

                if (amount_of_confirmations == 3) {
                    for (int i = 0; i < SHA256_BLOCK_SIZE; ++i) {
                        printf("%d", result[i]);
                    }
                    printf("\n");
                }
            }
        }
        printf("Watek z queue=%d zakonczyl prace\n", t_info->queue_slot);
    }

    pthread_mutex_unlock(&mutex);

    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Program wykonany z bledna iloscia argumentow\n");
        exit(EXIT_FAILURE);
    }

    long threads_amount = strtol(argv[1], NULL, 10);
    if (threads_amount < MIN_THREADS || threads_amount > MAX_THREADS) {
        printf("Liczba watkow jest nieprawidlowa\n");
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
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    for (int i = 0; i < threads_amount; i++) {
        t_info[i].queue_slot = (uint8_t) i;
        ret = pthread_create(&t_info[i].thread_id, &attr, thread_run, &t_info[i]);
        if (ret != 0) {
            printf("Nie udalo sie stworzyc watku nr %d\n", i);
            free(t_info);
            pthread_attr_destroy(&attr);
            exit(EXIT_FAILURE);
        }
    }

    pthread_attr_destroy(&attr);
    printf("Zakonczono tworzenie %ld watkow\n", threads_amount);

    for (int i = 0; i < threads_amount; ++i) {
        pthread_join(t_info[i].thread_id, NULL);
    }

    free(t_info);
    printf("confirmations: %d", amount_of_confirmations);
    return 0;
}


