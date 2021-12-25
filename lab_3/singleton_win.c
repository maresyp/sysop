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

        // check for env variable 
        char * env = getenv("SO2");
        if (env != NULL) {
            printf("SO2=%s\n", env);
            if (strcmp(env, "NEW") == 0) {
                printf("Terminating previous process ...\n");

                // TODO : Read pid from fille and kill process

                // wait for previous process to terminate and grab mutex
                WaitForSingleObject(mutex, INFINITE);   
            }
        } else { 
            exit(EXIT_FAILURE);
        }
    }

    printf("Created new process with pid=%d", GetCurrentProcessId());
    
    // TODO: Write current pid to file

    getchar();
    CloseHandle(mutex);
    
    exit(EXIT_SUCCESS);
}