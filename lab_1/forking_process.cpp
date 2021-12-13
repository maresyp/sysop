//
// Created by Mateusz Pysera on 28/11/2021.
//

#include <iostream>
#include <cstring>
#include <unistd.h> // for fork

int main(int argc, char *argv[], char *envp[]) {
    if (argc == 1) {
        std::cout << "Program wykonany bez argumentow" << std::endl;
    } else {
        if (strcmp(argv[1], "c")) {
            for (int i = 0; i < argc; ++i) {
                // create process
                pid_t pid = fork();
                if (pid == 0) {
                    // do child stuff
                    std::cout << argv[i] << " process pid -> " << pid << std::endl;
                    exit(0);
                } else if (pid == -1) {
                    std::cout << "forking failed ;(" << std::endl;
                }
            }
        }

    }
    getchar();
    return 0;
}

