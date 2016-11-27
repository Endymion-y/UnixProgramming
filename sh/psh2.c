// Prompting shell version 2.0
// Solve the one-shot problem of version 1
// Uses execvp(), but fork() first so that the shell waits
// around to perform another command
// New problem: shell catches signals. Run vi, press ^C
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define MAXARGS 20
#define ARGLEN 100

char* makestring(char* buf);
void execute(char* arglist[])

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

// Use fork and execvp and wait to do it
void execute(char* arglist[]){
    int pid, exitstatus;

    pid = fork();
    if (pid == -1){
        perror("Fork failed");
        exit(-1);
    }
    else if (pid == 0){
        execvp(arglist[0], arglist);
        perror("execvp failed");
        exit(-1);
    }
    else {
        while (wait(&exitstatus) != pid);
        printf("Child exited with status %d, %d\n", exitstatus >> 8, 
            exitstatus & 0377);
    }
}

// Trim off newline and create storage for the string
char* makestring(char* buf){
    char* cp;

    if (buf[strlen(buf) - 1] == '\n')
        buf[strlen(buf) - 1] = '\0';
    cp = malloc(strlen(buf) + 1);
    if (cp == NULL){
        fprintf(stderr, "No memory\n");
        exit(-1);
    }
    strcpy(cp, buf);
    return cp;
}