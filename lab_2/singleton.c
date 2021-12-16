//
// Created by Mateusz Pysera on 12/12/2021.
//
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/file.h>
#include <errno.h>
#include <stdlib.h>
#include <signal.h>

#define LOCK_PATH "/var/run/lock/my_singleton.pid"

int main(int argc, char *argv[], char *envp[]) {

    int pid_file = open(LOCK_PATH, O_CREAT | O_RDWR, S_IRWXU);
    int lock = flock(pid_file, LOCK_EX | LOCK_NB);
    if (lock == -1) {
        if (errno == EWOULDBLOCK) {
            printf("Another process is running\n");

            // check if env flag is set
            int env_flag = 0;
            for (char **env = envp; *env != NULL; env++) {
                if (strcmp(*env, "SO2=NEW") == 0) {
                    env_flag = 1;
                }
            }

            if (env_flag) {
                printf("kill previous process SO2=NEW\n");
                FILE *file = fopen(LOCK_PATH, "r");
                if (file == NULL) {
                    printf("Failed to open lock file");
                    exit(EXIT_FAILURE);
                } else {
                    int pid;
                    fscanf(file, "%d", &pid);
                    printf("Killing process with pid = %d\n", pid);
                    kill(pid, SIGKILL);
                    fclose(file);
                }
            } else {
                printf("Exiting\n");
                exit(EXIT_FAILURE);
            }
        }
    }
    FILE *file = fopen(LOCK_PATH, "w");
    if (file == NULL) {
        printf("Failed to open lock file");
        exit(EXIT_FAILURE);
    }
    fprintf(file, "%d", getpid());
    fclose(file);

    printf("Process running with pid: %d", getpid());
    getchar();

    return 0;
}
