//
// Created by Mateusz Pysera on 12/12/2021.
//
#include "stdio.h"
#include "string.h"

int main(int argc, char *argv[], char **envp) {


    int flag = 0;
    for (char **env = envp; *env != 0; env++) {
        char *thisEnv = *env;
        if (strcmp(thisEnv, "SO2=NEW") == 0) {
            flag = 1;
        }
    }

    if (flag) {
        printf("create new process and kill previous SO2=NEW");
    }

    getchar();
    return 0;
}
