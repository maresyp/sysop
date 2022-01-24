//
// Created by Mateusz Pysera on 02/01/2022.
//
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>

#define MIN_THREADS (int)3
#define MAX_THREADS (int)100

struct thread_info {
    int queue_slot;
    _Atomic int *next_to_close;
};
enum direction {
    INC = 0, DEC = 1
};

// Globals
enum direction close_order;
int increment_me = 0;
HANDLE mutex;

DWORD WINAPI thread_run(LPVOID lpParam) {
    struct thread_info * t_info = lpParam;

    sleep(rand() % 10);

    WaitForSingleObject(mutex, INFINITE);
    increment_me++;
    ReleaseMutex(mutex);

    while(t_info->queue_slot != *t_info->next_to_close) {
        // printf("spinlock");
    }
    printf("Watek nr %d zakonczyl prace.\n", t_info->queue_slot);
    if (close_order == INC)
        *t_info->next_to_close = *t_info->next_to_close + 1;
    else
        *t_info->next_to_close = *t_info->next_to_close - 1;
    return 0;
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

    mutex = CreateMutex(
        NULL,
        FALSE,
        "mutex"
    );
    if (mutex == NULL) { 
        printf("Failed creating mutex : %d\n", GetLastError()); 
        exit(EXIT_FAILURE); 
    } 

    HANDLE *thread_handles = malloc(sizeof(*thread_handles) * threads_amount);
    if (thread_handles == NULL) {
        CloseHandle(mutex);
        exit(EXIT_FAILURE);
    }
    struct thread_info *t_info = malloc(sizeof(*t_info) * threads_amount);
    if (t_info == NULL) {
        CloseHandle(mutex);
        free(thread_handles);
        exit(EXIT_FAILURE);
    }
    srand(time(NULL));

    _Atomic int ntc = (close_order == INC) ? 0 : threads_amount - 1;

    for (int i = 0; i < threads_amount; ++i) {
        // create threads
        t_info[i].queue_slot = i;
        t_info[i].next_to_close = &ntc;
        thread_handles[i] = CreateThread(
                NULL,
                sizeof(struct thread_info),
                thread_run,
                &t_info[i],
                0,
                NULL);

        if (thread_handles[i] == NULL) {
            printf("Failed to create thread with error=%d", GetLastError());
            for (int j = i - 1; j >= 0; j--) {
                CloseHandle(thread_handles[j]);
            }
            free(t_info);
            free(thread_handles);
            exit(EXIT_FAILURE);
        }
    }

    printf("Zakonczono tworzenie watkow\n");
    
    //WaitForMultipleObjects(threads_amount, thread_handles, TRUE, INFINITE);

    if (close_order == INC) {
        for(int i = 0; i< threads_amount; i++) {
            WaitForSingleObject(thread_handles[i], INFINITE);
            CloseHandle(thread_handles[i]);
        }
    } else {
        for(int i = threads_amount; i >= 0; i--) {
            WaitForSingleObject(thread_handles[i], INFINITE);
            CloseHandle(thread_handles[i]);
        }
    }


    free(t_info);
    free(thread_handles);

    printf("inc_me = %d", increment_me);
    assert(increment_me == threads_amount);

    return 0;
}
