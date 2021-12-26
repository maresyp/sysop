#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char * argv[], char * envp[]) {
    if (argc == 1) {
        printf("Program wykonany bez argumentow");
        exit(EXIT_SUCCESS);
    }
    for(int i = 1; i < argc; i++) {

        STARTUPINFO si;
        PROCESS_INFORMATION pi;

        ZeroMemory( &si, sizeof(si) );
        si.cb = sizeof(si);
        ZeroMemory( &pi, sizeof(pi) );
        char command = "echo siema";
        if( !CreateProcess( 
            NULL,           // No module name (use command line)
            &command,        // Command line
            NULL,           // Process handle not inheritable
            NULL,           // Thread handle not inheritable
            FALSE,          // Set handle inheritance to FALSE
            0,              // No creation flags
            NULL,           // Use parent's environment block
            NULL,           // Use parent's starting directory 
            &si,            // Pointer to STARTUPINFO structure
            &pi )           // Pointer to PROCESS_INFORMATION structure
    ) {
        printf( "CreateProcess failed (%d).\n", GetLastError() );
        // return;
    }
    }

    exit(EXIT_SUCCESS);
}