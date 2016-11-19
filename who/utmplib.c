/* utmplib.c - functions to buffer reads from utmp file
 * Read NRECS per read and doles them out from the buffer
 */
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <utmp.h>
#include <sys/types.h>
#include <unistd.h>
#include "utmplib.h"

static char utmpbuf[NRECS * UTSIZE];      // Storage
static int num_recs;                      // Num stored
static int cur_rec;                       // Next to go
static int fd_utmp = -1;                  // Read from

// Open file
int utmp_open(char* filename){
	fd_utmp = open(filename, O_RDONLY);
	cur_rec = num_recs = 0;
	return fd_utmp;
}

// Return pointer to next struct
struct utmp* utmp_next(){
	struct utmp* recp;
	if (fd_utmp == -1)
		return NULLUT;
	if (cur_rec == num_recs && utmp_reload() == 0)
		return NULLUT;

	recp = (struct utmp*)&utmpbuf[cur_rec * UTSIZE];
	cur_rec++;
	return recp;
}

// Read next bunch of records into buffer
int utmp_reload(){
	int amt_read;
	amt_read = read(fd_utmp, utmpbuf, NRECS * UTSIZE);
	num_recs = amt_read / UTSIZE;
	cur_rec = 0;
	return num_recs;
}

// Close file
void utmp_close(){
	if (fd_utmp != -1)
		close(fd_utmp);
}
