// Show how a program run another program
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
	char* arglist[3];
	arglist[0] = "ls";
	arglist[1] = "-l";
	arglist[2] = NULL;
	printf("*** About to exec ls -l\n");
	execvp("ls", arglist);
	printf("*** ls is done. Bye\n");   // NOTE: will not be printed

	return 0;
}
