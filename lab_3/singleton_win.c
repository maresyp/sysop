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

        // TODO : check for env variable 
        char * env = getenv("SO2");
        if (env != NULL && strcmp(env, "NEW") == 0) {
            printf("Env -> %s", env);

            // wait for previous process to terminate and grab mutex
            WaitForSingleObject(mutex, INFINITE);    
        } else { 
            exit(EXIT_FAILURE);
        }
    }

    // TODO: Write current pid to file

    getchar();
    CloseHandle(mutex);
    
    return 0;
}