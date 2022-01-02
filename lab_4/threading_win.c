//
// Created by Mateusz Pysera on 02/01/2022.
//
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MIN_THREADS (int)2
#define MAX_THREADS (int)100

enum direction {
    INC = 0, DEC = 1
};

// Globals
enum direction close_order;
int increment_me = 0;

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
    return 0;
}
