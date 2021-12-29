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

    // print environment variables
    for (char **env = envp; *env != NULL; env++) {
        printf("%s \n", *env);
    }

    if (argc == 1) {
        printf("Program wykonany bez argumentow");
    } else {
        if (strcmp(argv[1], "c") == 0) {

            if (argc == 2) {
                printf("Program wykonany bez argumentow");
                exit(EXIT_SUCCESS);
            }

            // use clone
            const int STACK_SIZE = 1024 * 1024;
            void **stack_array = malloc(sizeof(void *) * (argc - 1));
            if (stack_array == NULL) {
                printf("bad alloc");
                exit(EXIT_FAILURE);
            }
            for (int i = 0; i < argc - 1; ++i) {
                stack_array[i] = malloc(STACK_SIZE);
                if (stack_array[i] == NULL) {
                    for (int j = i - 1; j >= 0; j--) {
                        free(stack_array[j]);
                    }
                    free(stack_array);
                    printf("bad alloc");
                    exit(EXIT_FAILURE);
                }
            }

            for (int i = 2; i < argc; ++i) {
                // CLONE_VFORK will wait here for child_process to finish its job
                pid_t pid = clone(duplicate_me, stack_array[i - 2] + STACK_SIZE, CLONE_VM | CLONE_VFORK, argv[i]);
                if (pid == -1) {
                    printf("Failed to clone");
                } else {
                    printf("New process using clone created with pid = %d\n", pid);
                }
            }

            for (int i = 0; i < argc - 1; ++i) {
                free(stack_array[i]);
            }
            free(stack_array);
        } else {
            // use fork
            for (int i = 1; i < argc; ++i) {
                pid_t pid = fork();
                if (pid == -1) { printf("Forking failed"); }
                if (pid == 0) {
                    // do child stuff
                    printf("arg=%s process pid=%d\n", argv[i], getpid());
                    exit(0);
                }
            }
        }

    }
    getchar();
    return 0;
}

