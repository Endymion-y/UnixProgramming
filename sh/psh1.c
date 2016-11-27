// Prompting shell version 1
// Prompts for the command and its arguments
// Builds the argument vector for the call to execvp
// Uses execvp(), and never returns
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define MAXARGS 20
#define ARGLEN 100

char* makestring(char* buf);
int execute(char* arglist[]);

int main(){
    char* arglist[MAXARGS + 1];
    int numargs;
    char argbuf[ARGLEN];
    
    numargs = 0;
    while (numargs < MAXARGS){
        printf("Arg[%d]?", numargs);
        if (fgets(argbuf, ARGLEN, stdin) && *argbuf != '\n')
            arglist[numargs++] = makestring(argbuf);
        else {
            if (numargs > 0){
                arglist[numargs] = NULL;
                execute(arglist);
                numargs = 0;
            }
        }
    }
    return 0;
}

// Use execvp to do it
int execute(char* arglist[]){
    execvp(arglist[0], arglist);
    perror("execvp failed");
    exit(-1);
}

// Trim off newline and create storage for the string
char* makestring(char* buf){
    char* cp;
    buf[strlen(buf) - 1] = '\0';    // Trim newline
    cp = malloc(strlen(buf) + 1);
    if (cp == NULL){
        fprintf(stderr, "No memory\n");
        exit(-1);
    }
    strcpy(cp, buf);
    return cp;
}