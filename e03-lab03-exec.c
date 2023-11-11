#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#define MAX 50

FILE *fileUtils(char *);
char *allocate();

int main(int argc, char *argv[]) {
    FILE *fp;
    char *command;
    
    // allocate
    command = allocate();

    // check file parameter: file + timer
    if (argc<3) {
        fprintf(stdout, "Insufficient number of parameters.\n");
        exit(EXIT_FAILURE);
    }
    
    //file management
    fp = fileUtils(argv[1]);
    int i=0;
    
    while(fgets(command, MAX, fp) != NULL) {
    
        // Introduce a n-second delay
        // except for the first element
        if (i!=0) sleep(atoi(argv[2]));
        
        
        int command_len = strlen(command);
        command[command_len - 1] = '\0';
        // check correctness and execute -> system / exec
        // assume no 'end' for this purpose, otherwise strip it
        
        int retFork = fork(); 
        if (retFork == 0) {
            // Child process
            char *args[32];
            int arg_count = 0;
            char *saveptr; // For strtok_r

            // Tokenize the command and arguments using strtok_r
            // the function is re-entrant, thread-safe
            // the latter additional pointer is used to maintain the 
            // coherence between each execution
            char *token = strtok_r(command, " ", &saveptr);
            while (token != NULL) {
                args[arg_count] = token;
                arg_count++;
                token = strtok_r(NULL, " ", &saveptr);
            }
            args[arg_count] = NULL;

            // Execute the command using execvp
            // no need to kill child process
            // the op is done autmatically
            // fork -> exec -> exit(0) -> merge
            // while parent is waiting
            execvp(args[0], args);

            // If execvp returns, it means there was an error
            perror("execvp");
            exit(EXIT_FAILURE);
        } 
        else if (retFork < 0) {
            perror("Error while executing fork.\n");
        } 
        else {
            // Parent process -> waits execution of child process
            int status;
            waitpid(retFork, &status, 0);
            if (WIFEXITED(status) && WEXITSTATUS(status) != 0) {
                printf("Error executing command: %s\n", command);
            }
        }
        fprintf(stdout, "Command %d executed.\n", ++i);   
    }
    
    free(command);
    fclose(fp);
    return(0);
}

FILE *fileUtils(char *filename) {
    FILE *fp;
    fp = fopen(filename, "r");
    if (fp == NULL) {
        fprintf(stdout, "Error while opening the file.\n");
        exit(EXIT_FAILURE);
    }
    
    return fp;
}

char *allocate() {
    char *var;
    var = (char *)malloc(MAX*sizeof(char));
    
    if (var == NULL) {
        fprintf(stdout, "Allocation blah blah.\n");
        exit(EXIT_FAILURE);
    }
    
    return var;
}
