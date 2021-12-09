//
// Created by Mateusz Pysera on 29/11/2021.
//

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[], char *envp[]) {

    // TODO: handle envp

    if (argc == 1) {
        printf("No arguments specified");
    } else {

        pthread_mutex_t mutex;
        pthread_mutexattr_t attr_mutex;
        // TODO: Add checking
        pthread_mutexattr_init(&attr_mutex);
        pthread_mutexattr_setpshared(&attr_mutex, PTHREAD_PROCESS_SHARED);
        pthread_mutex_init(&mutex, &attr_mutex);

        for (int i = 0; i < argc; ++i) {
            pid_t pid = fork();
            if (pid == 0) {
                // do child stuff
                pthread_mutex_lock(&mutex);
                printf("%d %s\n", i, argv[i]);
                pthread_mutex_unlock(&mutex);
                exit(0);
            }
        }

        pthread_mutex_destroy(&mutex);
        pthread_mutexattr_destroy(&attr_mutex);
    }
    getchar(); // stop here
    return 0;
}
