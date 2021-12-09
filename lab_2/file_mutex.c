//
// Created by Mateusz Pysera on 03/12/2021.
//

#include <stdio.h>
#include <unistd.h>

#define MUTEX "mutex"

int f_mutex_create(const char *filename) {
    FILE *fptr = fopen(filename, "r");
    if (fptr == NULL) return -1;
    fclose(fptr);
    return 0;
}

int f_mutex_init() {
    return 0;
}

int f_mutex_lock() {
    return 0;
}

int f_mutex_unlock() {
    return 0;
}

int main(int argc, char *argv[], char *envp[]) {

    if (argc == 1) {
        printf("Process ran without parameters");
        return 0;
    }

    if (f_mutex_create(MUTEX) != 0) {
        printf("failed to create mutex file");
        return -1;
    }

    f_mutex_init();

    for (int i = 1; i < argc; ++i) {
        pid_t pid = fork();
        if (pid == 0) {
            // child process
            f_mutex_lock();
            printf("arg %d : %s", i, argv[i]);
            f_mutex_unlock();
            return 0;
        }
    }

    return 0;
}

