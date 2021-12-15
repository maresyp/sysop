//
// Created by Mateusz Pysera on 12/12/2021.
//
#include <stdio.h>
#include <string.h>
#include <semaphore.h>

int main(int argc, char *argv[], char *envp[]) {

    // create named semaphore
    sem_open("/my_semaphore", 1);

    int flag = 0;
    for (char **env = envp; *env != 0; env++) {
        const char *thisEnv = *env;
        if (strcmp(thisEnv, "SO2=NEW") == 0) {
            flag = 1;
        }
    }

    if (flag) {
        printf("create new process and kill previous SO2=NEW");
    }
    // /var/run/lock -> stworzyc plik
    // use semaphore dla kilku procesów
    getchar();
    return 0;
}
