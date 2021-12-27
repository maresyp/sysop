#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char * argv[], char * envp[]) {
    if (argc == 1) {
        printf("Program wykonany bez argumentow");
        exit(EXIT_SUCCESS);
    }
    if (argc == 2) { 
        printf("Process with pid=%d arg=%s\n", GetCurrentProcessId(), argv[1]);
        exit(EXIT_SUCCESS);
    }
    for(int i = 1; i < argc; i++) {

        STARTUPINFO si;
        PROCESS_INFORMATION pi;

        ZeroMemory( &si, sizeof(si) );
        si.cb = sizeof(si);
        ZeroMemory( &pi, sizeof(pi) );

        char * command = malloc(strlen(argv[0]) + 1 + strlen(argv[i]) + 1);
        if (command == NULL) {
            printf("Bad alloc");
            exit(EXIT_FAILURE);
        }
        
        command[0] = '\0';
        strncat(command, argv[0], strlen(argv[0]));
        strncat(command, " ", 1);
        strncat(command, argv[i], strlen(argv[i]));

        if( !CreateProcess( 
            NULL,           // No module name (use command line)
            command,        // Command line
            NULL,           // Process handle not inheritable
            NULL,           // Thread handle not inheritable
            FALSE,          // Set handle inheritance to FALSE
            0,              // No creation flags
            NULL,           // Use parent's environment block
            NULL,           // Use parent's starting directory 
            &si,            // Pointer to STARTUPINFO structure
            &pi )           // Pointer to PROCESS_INFORMATION structure
        ) {
            printf("Failed to create process with error=%d", GetLastError() );
            free(command);
            exit(EXIT_FAILURE);
        }
    
        free(command);

        WaitForSingleObject( pi.hProcess, INFINITE );

        // Close process and thread handles. 
        CloseHandle( pi.hProcess );
        CloseHandle( pi.hThread );
    }
    
    exit(EXIT_SUCCESS);
}