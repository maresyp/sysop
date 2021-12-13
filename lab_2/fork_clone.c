#include <unistd.h> // for fork
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sched.h>
#include <sys/wait.h>

void duplicate_me(void *arg) {
    printf("clone arg -> %s", (char *) arg);
}

int main(int argc, char *argv[], char *envp[]) {
    if (argc == 1) {
        printf("Program wykonany bez argumentow");
    } else {
        if (strcmp(argv[1], "c") == 0) {
            // use clone
            const int STACK_SIZE = 65536;
            char *stack = malloc(STACK_SIZE);
            if (stack == NULL) {
                printf("bad alloc");
                exit(-1);
            }
            for (int i = 2; i < argc; ++i) {
                pid_t pid = clone(duplicate_me, stack, SIGCHLD, (void *) argv[i]);
            }
        } else {
            // use fork
            for (int i = 2; i < argc; ++i) {
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

