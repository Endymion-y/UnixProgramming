// Shows how child processes pick up at the return
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
	printf("My pid is %d\n", getpid());
	fork();
	fork();
	fork();
	printf("My pid is %d\n", getpid());

	return 0;
}
