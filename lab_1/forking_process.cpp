//
// Created by Mateusz Pysera on 28/11/2021.
//

#include <iostream>
#include <unistd.h> // for fork
// use clone

int main(int argc, char * argv[], char * envp[]) {
    if (argc == 1) {
        std::cout << "Program wykonany bez argumentow" << std::endl;
    } else {
        for (int i = 0; i < argc; ++i) {
            // create process
            pid_t pid = fork();
            if (pid == 0) {
                // do child stuff
                std::cout << argv[i] << " process pid -> " << pid << std::endl;
                return 0;
            }
        }
    }
    getchar();
    return 0;
}

