// Demonstrate use of interval timer to generate regular
// signals, which are in turn caught and used to count down
#include <stdio.h>
#include <sys/time.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

void countdown(int signum);
int set_ticker(int n_msecs);

int main(){
	signal(SIGALRM, countdown);
	if (set_ticker(500) == -1)
		perror("set_ticker");
	else 
		while (1){
			pause();
		}
	return 0;
}

void countdown(int signum){
	static int num = 10;
	printf("%d..", num--);
	fflush(stdout);
	if (num < 0){
		printf("DONE!\n");
		exit(0);
	}
}

// Arranges for interval timer to issue SIGALRMs at regular interval
// Returns -1 on error, 0 for OK
// Arg in milliseconds, converted into whole seconds and microseconds
// Note: set_ticker(0) turns off ticker
int set_ticker(int n_msecs){
	struct itimerval new_timeset;
	long n_sec, n_usecs;

	n_sec = n_msecs / 1000;
	n_usecs = (n_msecs % 1000) * 1000L;

	new_timeset.it_interval.tv_sec = n_sec;
	new_timeset.it_interval.tv_usec = n_usecs;

	new_timeset.it_value.tv_sec = n_sec;
	new_timeset.it_value.tv_usec = n_usecs;

	return setitimer(ITIMER_REAL, &new_timeset, NULL);
}
