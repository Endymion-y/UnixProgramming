/* who3.c - who with buffered reads
 * - suppresses empty records
 * - formats time nicely
 * - buffers input (using utmplib)
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <utmp.h>
#include <fcntl.h>
#include <time.h>
#include "utmplib.h"

#define SHOWHOST

void show_info(struct utmp* rec);

int main(){
	struct utmp* utbufp;

	if (utmp_open(UTMP_FILE) == -1){
		perror(UTMP_FILE);
		exit(1);
	}
	while (utbufp = utmp_next()){
		show_info(utbufp);
	}
	utmp_close();
	return 0;
}

/* Display contents of the utmp struct in human readable format
 * NOTE: these sizes should not be hardwired
 */
void show_info(struct utmp* rec){
	if (rec->ut_type != USER_PROCESS) return;

	printf("%-15.15s", rec->ut_user);    // Username
	printf(" ");
	printf("%-15.15s", rec->ut_line);    // Device name
	printf(" ");
	printf("%20.20s", ctime(&rec->ut_time) + 4);     // Login time
	printf(" ");
#ifdef SHOWHOST
	printf("(%s)", rec->ut_host);     // Hostname
#endif
	printf("\n");
}
