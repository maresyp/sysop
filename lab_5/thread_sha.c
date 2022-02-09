//
// Created by Mateusz Pysera on 08/02/2022.
//

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#include "sha256.h"

#define MAX_THREADS (int)100
#define MIN_THREADS (int)3

typedef struct thread_info {
    pthread_t thread_id;
    uint8_t queue_slot;
} thread_info;

typedef struct worker_info {
    long threads_count;
    thread_info *t_info_array;
} worker_info;

/* GLOBALS */

pthread_mutex_t mutex;
int amount_of_confirmations = -1;

/* GLOBALS */

void *calculate_sha256(void *arg) {

    thread_info const *t_info = arg;

    // calculate sha256
    BYTE index[] = {"236439"};
    SHA256_CTX ctx;
    BYTE buffer[SHA256_BLOCK_SIZE];

    sha256_init(&ctx);
    sha256_update(&ctx, index, 6);
    sha256_final(&ctx, buffer);

    printf("Thread with queue = %d finished its job", t_info->queue_slot);
    return NULL;
}

void *create_workers(void *arg) {

    worker_info *w_info = arg;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    /* Wait here for mutex from main before creating new threads */
    pthread_mutex_lock(&mutex);
    pthread_mutex_unlock(&mutex);

    /* Create threads */
    for (int i = 0; i < w_info->threads_count; i++) {
        w_info->t_info_array[i].queue_slot = (uint8_t) i;
        int error = pthread_create(&w_info->t_info_array[i].thread_id, &attr, calculate_sha256,
                                   &w_info->t_info_array[i]);
        if (error != 0) {
            printf("Nie udalo sie stworzyc watku nr %d\n", i);
            free(w_info->t_info_array);
            pthread_attr_destroy(&attr);
            exit(EXIT_FAILURE);
        }
    }

    /* Wait for all threads to finish their job */
    for (int i = 0; i < w_info->threads_count; ++i) {
        pthread_join(w_info->t_info_array[i].thread_id, NULL);
    }

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

    thread_info *t_info = malloc(threads_amount * sizeof(*t_info));
    if (t_info == NULL) {
        printf("Bad alloc");
        exit(EXIT_FAILURE);
    }

    worker_info w_info = {.t_info_array = t_info, .threads_count = threads_amount};
    pthread_t create_threads_id;

    /* Take mutex here to prevent create_workers function to run before main gets to wait for result */
    pthread_mutex_lock(&mutex);
    if (pthread_create(&create_threads_id, NULL, create_workers, &w_info) != 0) {
        printf("Failed to create thread, aborting");
        free(t_info);
        exit(EXIT_FAILURE);
    }
    /* Release mutex after creation of thread */
    pthread_mutex_unlock(&mutex);

    /* Wait here for amount_of_confirmations to reach desired value */


    free(t_info);
    return 0;
}

