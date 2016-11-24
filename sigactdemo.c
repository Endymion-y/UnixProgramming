// Purpose: shows use of sigaction()
// Feature: blocks ^\ while handling ^C
// Does not reset ^C handler, so two kill
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

#define INPUTLEN 100

void inthandler(int signum);

int main(){
    struct sigaction newhandler;    // New settings
    sigset_t blocked;               // Set of blocked signals
    char x[INPUTLEN];

    // Load the two members first
    newhandler.sa_handler = inthandler;
    newhandler.sa_flags = SA_RESETHAND | SA_RESTART;

    // Then build the list of blocked signals
    sigemptyset(&blocked);           // Clear all bits
    sigaddset(&blocked, SIGQUIT);    // Add SIGQUIT to list
    newhandler.sa_mask = blocked;

    if (sigaction(SIGINT, &newhandler, NULL) == -1)
        perror("sigaction");
    else {
        while (1){
            fgets(x, INPUTLEN, stdin);
            printf("input: %s", x);
        }
    }
}

void inthandler(int signum){
    printf("Called with signal %d\n", signum);
    sleep(signum);
    printf("Done handling signal %d\n", signum);
}