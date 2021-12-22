#include <stdio.h>
#include <windows.h>


int main(int argc, char * argv[], char * envp[]) {

    // try to open file in write mode
    // this should fail if other process is running
    FILE * fptr = fopen("file_lock.txt", "w");
    if (fptr == NULL) {
        printf("Failed to open file_lock");
        exit(EXIT_FAILURE);
    }
    fclose(fptr);

    HANDLE file_handle = CreateFile(
        TEXT("file_lock.txt"),
        GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ, // other process can read file     
        NULL,       // no security attributes
        CREATE_NEW, // creating a new temp file
        0,          // not overlapped index/O
        NULL);
    
    printf("%d", GetLastError());
    getchar();

    if (!CloseHandle(file_handle)) {
        printf("Something went wrong while closing file handle");
        exit(EXIT_FAILURE);
    }
    return 0;
}