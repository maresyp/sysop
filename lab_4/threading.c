//
// Created by Mateusz Pysera on 01/01/2022.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MIN_THREADS (int)2
#define MAX_THREADS (int)100

enum direction {
    INC = 0, DEC = 1
};
enum direction close_order;

void run() {
    // critical section with mutex

    /*
     * for(...) {
     *     // thread.join in dec or inc order
     * }
     */
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

    if (strcmp(argv[2], "inc") == 0) {
        close_order = INC;
    } else if (strcmp(argv[2], "dec") == 0) {
        close_order = DEC;
    } else {
        printf("Prosze wybrac inc / dec. %s nie jest poprawne", argv[2]);
        exit(EXIT_FAILURE);
    }

    return 0;
}

