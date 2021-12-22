#include <stdio.h>
#include <windows.h>


int main(int argc, char * argv[], char * envp[]) {
    HANDLE file_handle = CreateFile(
        TEXT("datafile.txt"),
        GENERIC_READ | GENERIC_WRITE,
        0,          // open with exclusive access
        NULL,       // no security attributes
        CREATE_NEW, // creating a new temp file
        0,          // not overlapped index/O
        NULL);
    return 0;
}