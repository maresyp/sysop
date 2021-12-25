// compile with -lpsapi

#include <stdio.h>
#include <windows.h>
#include <psapi.h>

int find_and_kill(long pid) {
    DWORD processes[1024], needed, process;
    if (!EnumProcesses(processes, sizeof(processes), &needed)) { return -1; }
    process = needed / sizeof(DWORD); // calc how many found
    for(int i=0; i < process; i++) {
        if (processes[i] == pid) {
            HANDLE process_handle = OpenProcess(
                PROCESS_TERMINATE,
                FALSE,
                pid
            );
            if (process_handle == NULL) { return -2; }
            if (TerminateProcess(process_handle, 0) == 0) { return -3; }
        }
    }
    return 0;
}

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

                // Read pid from fille and kill process
                HANDLE pid_file = CreateFile(
                                    TEXT("pid_file.txt"),
                                    GENERIC_READ,
                                    0,             // open with exclusive access
                                    NULL,          // no security attributes
                                    OPEN_EXISTING, // opens a file or device, only if it exists. 
                                    0,             // not overlapped index/O
                                    NULL);

                if (pid_file == INVALID_HANDLE_VALUE) {
                    printf("Failed to open pid file, error=%d", GetLastError());
                    exit(EXIT_FAILURE);
                }
                char txt[10];
                DWORD bytes_read;
                BOOL ret = ReadFile(
                    pid_file,
                    &txt,
                    9,
                    &bytes_read,
                    NULL
                );
                if (ret == FALSE) {
                    printf("Failed reading from file with error=%d", GetLastError());
                    exit(EXIT_FAILURE);
                }

                // clean input
                long pid = strtol(txt, NULL, 10);
                if (pid == 0) {
                    printf("Failed to parse input");
                    exit(EXIT_FAILURE);
                }
                CloseHandle(pid_file);   

                printf("Killing process with pid=%d\n", pid);
                int fak = find_and_kill(pid);
                if (fak != 0) {
                    printf("Failed to kill process with error=%d", GetLastError());
                    exit(EXIT_FAILURE);
                }
                printf("Waiting...\n");
                // wait for previous process to terminate and grab mutex
                WaitForSingleObject(mutex, INFINITE);   
            } else {
                exit(EXIT_FAILURE);
            }
        } else { 
            exit(EXIT_FAILURE);
        }
    }

    printf("Created new process with pid=%d\n", GetCurrentProcessId());
    
    // Write current pid to file
    HANDLE pid_file = CreateFile(
                        TEXT("pid_file.txt"),
                        GENERIC_WRITE,
                        0,             // open with exclusive access
                        NULL,          // no security attributes
                        CREATE_ALWAYS, // creates a new file, always
                        0,             // not overlapped index/O
                        NULL);

    if (pid_file == INVALID_HANDLE_VALUE) {
        printf("Failed to create file error=%d", GetLastError());
        exit(EXIT_FAILURE);
    } 
    
    DWORD data = GetCurrentProcessId();
    DWORD bytes_written;
    char txt[10]; sprintf(txt, "%d", data);

    BOOL ret = WriteFile(
        pid_file,
        txt,
        strlen(txt),
        &bytes_written,
        NULL
    );
    if (ret == FALSE) {
        printf("Failed writting to file with error=%d", GetLastError());
        exit(EXIT_FAILURE);
    }

    CloseHandle(pid_file);
    getchar();
    CloseHandle(mutex);
    
    exit(EXIT_SUCCESS);
}