// Show how sleep works
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

#define SHHHH

void wakeup(int signum);

int main(){
	printf("About to sleep for 4 seconds\n");
	signal(SIGALRM, wakeup);
	alarm(4);
	pause();        // freeze here
	printf("Morning so soon?\n");

	return 0;
}

void wakeup(int signum){
#ifdef SHHHH
	printf("Alarm received from kernel\n");
#endif
}
