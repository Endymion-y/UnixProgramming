#ifndef SMSH1_H
#define SMSH1_H

#define YES 1
#define NO 0

void setup();
char* next_cmd();
char** splitline(char* line);
void freelist(char** list);
void *emalloc(size_t n);
void *erealloc(void* p, size_t n);
int execute(char* argv[]);
void fatal(char* s1, char* s2, int n);
char* newstr(char* s, int l);

#endif