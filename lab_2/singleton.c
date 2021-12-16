//
// Created by Mateusz Pysera on 12/12/2021.
//
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/file.h>
#include <errno.h>
#include <stdlib.h>

int main(int argc, char *argv[], char *envp[]) {

    int pid_file = open("/var/run/my_singleton.pid", O_CREAT | O_RDWR, S_IRWXU);
    int lock = flock(pid_file, LOCK_EX | LOCK_NB);
    printf("lock -> %d", lock);
    if (lock) {
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
                printf("kill previous process SO2=NEW");
            } else {
                printf("Exiting");
                exit(EXIT_FAILURE);
            }
        }
    }

    printf("Process running with pid: %d", getpid());
    getchar();
    return 0;
}
