#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define MAX 50

FILE *fileUtils(char *);
char *allocate();

int main(int argc, char *argv[]) {
    FILE *fp;
    char *command;
    
    // allocate
    command = allocate();

    // check file parameter
    if (argc<2) {
        fprintf(stdout, "Insufficient number of parameters.\n");
        exit(EXIT_FAILURE);
    }
    
    //file management
    fp = fileUtils(argv[1]);
    int i=0;
    
    while(fgets(command, MAX, fp) != NULL) {
        // check correctness and execute -> system / exec
        // assume no 'end' for this purpose
        if (command != NULL) {
            i++;
            fprintf(stdout, "Command n. %d\n", i);
            system(command);
        }
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
