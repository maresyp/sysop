#define _GNU_SOURCE

#include <unistd.h> // for fork
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sched.h>
#include <sys/wait.h>

int duplicate_me(void *arg) {
    printf("clone arg -> %s\n", (char *) arg);
    exit(0);
}

int main(int argc, char *argv[], char *envp[]) {
    if (argc == 1) {
        printf("Program wykonany bez argumentow");
    } else {
        if (strcmp(argv[1], "c") == 0) {
            // use clone
            const int STACK_SIZE = 1024 * 1024;
            void *stack = malloc(STACK_SIZE);
            if (stack == NULL) {
                printf("bad alloc");
                exit(-1);
            }
            pid_t pid;
            for (int i = 1; i < argc; ++i) {
                // CLONE_VFORK will wait here for child_process to finish its job
                pid = clone(duplicate_me, stack + STACK_SIZE, CLONE_VM | CLONE_VFORK, argv[i]);
                if (pid == -1) {
                    printf("Failed to clone");
                    exit(EXIT_FAILURE);
                }
                printf("New process using clone created with pid = %d\n", pid);
            }
            while (wait(NULL) > 0);
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

