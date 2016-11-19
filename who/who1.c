/* who1.c - a first version of the who program
 * Open, read UTMP file, and show results
 */
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <utmp.h>
#include <unistd.h>

#define SHOWHOST

void show_info(struct utmp* rec);

int main(){
	struct utmp current_record;
	int utmpfd;
	int reclen = sizeof(struct utmp);

	if ((utmpfd = open(UTMP_FILE, O_RDONLY)) == -1){
		perror(UTMP_FILE);
		exit(1);
	}
	while (read(utmpfd, &current_record, reclen) == reclen)
		show_info(&current_record);
	close(utmpfd);
	return 0;
}

/* Display contents of the utmp struct in human readable format
 * NOTE: these sizes should not be hardwired
 */
void show_info(struct utmp* rec){
	printf("%-8.8s", rec->ut_user);    // Username
	printf(" ");
	printf("%-8.8s", rec->ut_line);    // Device name
	printf(" ");
	printf("%10ld", rec->ut_time);     // Login time
	printf(" ");
#ifdef SHOWHOST
	printf("(%s)", rec->ut_host);     // Hostname
#endif
	printf("\n");
}
