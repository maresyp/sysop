#include <stdio.h>
#include <windows.h>


int main(int argc, char * argv[], char * envp[]) {

    HANDLE mutex = CreateMutex(
        NULL,
        TRUE,
        "my_mutex"
    );
    if (mutex == NULL) { 
        printf("Failed creating mutex : %d\n", GetLastError()); 
        exit(EXIT_FAILURE); 
    }  
    if (GetLastError() == ERROR_ALREADY_EXISTS) {
        printf("Another process is running\n");
        exit(EXIT_FAILURE);
    }

    // TODO: Write current pid to file

    getchar();
    CloseHandle(mutex);
    
    return 0;
}