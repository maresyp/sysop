//
// Created by Mateusz Pysera on 28/11/2021.
//
#include <iostream>
#include <unistd.h>
#include <sched.h>

void prt() {
    fork();
    clone();
    std::cout << "fork" << std::endl;
}

int main() {
    std::cout << "Hello before fork" << std::endl;
    prt();
    std::cout << "Hello after fork" << std::endl;
    return 0;
}
