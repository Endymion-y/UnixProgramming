// Small shell version 1
// Parses the command line into strings
// Uses fork, exec, wait, and ignores signals
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include "smsh1.h"

#define DFL_PROMPT ">"

int main(){
    char* cmdline;
    char* prompt;
    char** arglist;
    int result;

    prompt = DFL_PROMPT;
    setup();

    while ((cmdline = next_cmd(prompt, stdin)) != NULL){
        if ((arglist = splitline(cmdline)) != NULL){
            result = execute(arglist);
            freelist(arglist);
        }
        free(cmdline);
    }
    return 0;
}

// Initialize the shell
// Call fatal() if trouble
void setup(){
    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
}

void fatal(char* s1, char* s2, int n){
    fprintf(stderr, "Error: %s, %s\n", s1, s2);
    exit(n);
}

// Run a program with given arguments
// Return status returned via wait, or -1 on error
int execute(char* argv[]){
    int pid;
    int child_info = -1;

    if (argv[0] == NULL) return 0;

    if ((pid = fork()) == -1) perror("fork");
    else if (pid == 0){
        signal(SIGINT, SIG_DFL);
        signal(SIGQUIT, SIG_DFL);
        execvp(argv[0], argv);
        perror("Cannot execute command");
        exit(-1);
    }
    else {
        if (wait(&child_info) == -1) perror("wait");
        return child_info;
    }
}

// Read next command line from fp
// Return a dynamically allocated string holding the command line
// NULL at EOF
// Call fatal from emalloc()
// Note: allocates space in BUFSIZ chunks
char* next_cmd(char* prompt, FILE* fp){
    char* buf;
    int bufspace = 0;
    int pos;
    int c;

    printf("%s", prompt);
    while ((c = getc(fp)) != EOF){
        // Need space?
        if (pos + 1 >= bufspace){
            if (bufspace == 0)
                buf = emalloc(BUFSIZ);
            else {
                buf = erealloc(buf, bufspace + BUFSIZ);
            }
            bufspace += BUFSIZ;
        }

        // End of command?
        if (c == '\n')
            break;
        
        // Else, add to buffer
        buf[pos++] = c;
    }
    if (c == EOF && pos == 0)      // EOF and no input
        return NULL;
    buf[pos] = '\0';
    return buf;
}

#define is_delim(x) ((x)==' '||(x)=='\t')

// Split a line into array of white-space separated tokens
// Return a NULL-terminated array of pointers to copies of 
// the tokens or NULL if no tokens on the line
// Traverse the array, locate strings, make copies
// Note: strtok() could work, but we may want to add quotes later
char** splitline(char* line){
    char** args;
    int spots = 0;        // Spots in table
    int bufspace = 0;     // Bytes in table
    int argnum = 0;       // Slots used
    char* cp = line;
    char* start;
    int len;

    if (line == NULL) return NULL;

    args = emalloc(BUFSIZ);
    bufspace = BUFSIZ;
    spots = BUFSIZ / sizeof(char*);

    while (*cp != '\0'){
        while (is_delim(*cp)) cp++;
        if (*cp == '\0') break;

        // Make sure the array has room
        if (argnum + 1 >= spots){
            args = erealloc(args, bufspace + BUFSIZ);
            bufspace += BUFSIZ;
            spots += (BUFSIZ / sizeof(char*));
        }

        // Mark start, then find end of word
        start = cp;
        len = 1;
        while (1){
            cp++;
            if (*cp == '\0' || is_delim(*cp)) break;
            else len++;
        }
        args[argnum++] = newstr(start, len);
    }
    args[argnum] = NULL;
    return args;
}

// Contructor for strings
char* newstr(char* s, int l){
    char* rv = emalloc(l + 1);

    rv[l] = '\0';
    strncpy(rv, s, l);
    return rv;
}

// Free the list returned by splitline
void freelist(char** list){
    char** cp = list;
    while (*cp){
        free(*cp++);
    }
    free(list);
}

void *emalloc(size_t n){
    void* rv;
    if ((rv = malloc(n)) == NULL)
        fatal("Out of memory", "", -1);
    return rv;
}

void *erealloc(void* p, size_t n){
    void* rv;
    if ((rv = realloc(p, n)) == NULL)
        fatal("realloc() failed", "", -1);
    return rv;
}