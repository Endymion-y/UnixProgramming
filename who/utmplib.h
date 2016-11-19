/* utmplib.h - functions to buffer reads from utmp file */
#ifndef UTMPLIB_H
#define UTMPLIB_H

#define NRECS 16
#define NULLUT ((struct utmp*)NULL)
#define UTSIZE (sizeof(struct utmp))

int utmp_open(char* filename);
struct utmp* utmp_next();
int utmp_reload();
void utmp_close();

#endif
