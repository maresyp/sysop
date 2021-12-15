#define _GNU_SOURCE

#include <unistd.h> // for fork
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sched.h>
#include <sys/wait.h>

int duplicate_me(void *arg) {
    printf("Duplicate me is running");
    printf("clone arg -> %s", (char *) arg);
    return 0;
}

int main(int argc, char *argv[], char *envp[]) {
    if (argc == 1) {
        printf("Program wykonany bez argumentow");
    } else {
        if (strcmp(argv[1], "c") == 0) {
            // use clone
            const int STACK_SIZE = 65536;
            void *stack = malloc(STACK_SIZE);
            if (stack == NULL) {
                printf("bad alloc");
                exit(-1);
            }
            for (int i = 2; i < argc; ++i) {
                pid_t pid = clone(duplicate_me, stack + STACK_SIZE, SIGCHLD, argv[i]);
                if (pid == -1) {
                    printf("Failed to clone");
                    exit(EXIT_FAILURE);
                }
                printf("New process using clone created with pid = %d\n", pid);
            }
            free(stack);
        } else {
            // use fork
            for (int i = 1; i < argc; ++i) {
                pid_t pid = fork();
                if (pid == -1) { printf("Forking failed"); }
                if (pid == 0) {
                    // do child stuff
                    printf("%s process pid %d\n", argv[i], pid);
                    exit(0);
                }
            }
        }

    }
    getchar();
    return 0;
}

