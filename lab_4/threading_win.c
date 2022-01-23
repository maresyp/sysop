//
// Created by Mateusz Pysera on 02/01/2022.
//
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>

#define MIN_THREADS (int)3
#define MAX_THREADS (int)100

struct thread_info {

    _Atomic int next_to_close;
};
enum direction {
    INC = 0, DEC = 1
};

// Globals
enum direction close_order;
int increment_me = 0;

DWORD WINAPI thread_run(LPVOID lpParam) {
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

    DWORD *thread_ids = malloc(sizeof(*thread_ids) * threads_amount);
    HANDLE *thread_handles = malloc(sizeof(*thread_handles) * threads_amount);

    for (int i = 0; i < threads_amount; ++i) {
        // create threads
        thread_handles[i] = CreateThread(
                NULL,
                0,
                thread_run,
                0,
                0,
                &thread_ids[i]);

        if (thread_handles[i] == NULL) {
            // TODO: handle error
        }
    }

    printf("Zakonczono tworzenie watkow\n");
    WaitForMultipleObjects(threads_amount, thread_handles, TRUE, INFINITE);

    for (int i = 0; i < threads_amount; ++i) {
        CloseHandle(thread_handles[i]);
    }
    free(thread_ids);
    free(thread_handles);

    return 0;
}
