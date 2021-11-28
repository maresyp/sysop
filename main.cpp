#include <iostream>
#include <unistd.h> // for fork

int main(int argc, char * argv[]) {
    if (argc == 1) {
        std::cout << "Program wykonany bez argumentow" << std::endl;
    } else {
        for (int i = 1; i < argc; ++i) {
            // create process
            auto pid = fork();
            std::cout << argv[i] << " process pid -> " << pid << std::endl;
        }
    }
    getchar();
    return 0;
}