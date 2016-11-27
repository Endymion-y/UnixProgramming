// Shows how parent pauses until child finishes
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define DELAY 2

void child_code(int delay);
void parent_code(int childpid);

int main(){
	int newpid;
	printf("Before: mypid is %d\n", getpid());

	if ((newpid = fork()) == -1)
		perror("fork");
	else if (newpid == 0)
		child_code(DELAY);
	else parent_code(newpid);

	return 0;
}

// New process takes a nap and then exits
void child_code(int delay){
	printf("Child %d here wll sleep for %d seconds\n", getpid(), delay);
	sleep(delay);
	printf("Child done. About to exit\n");
	exit(17);
}

// Parent waits for child then prints a message
void parent_code(int childpid){
	int wait_rv;
	wait_rv = wait(NULL);
	printf("Done waiting for %d. Wait returned: %d\n", childpid, wait_rv);
}
