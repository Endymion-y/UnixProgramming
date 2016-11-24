// Signal demo
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void f(int signum);

int main(){
	int i;
	signal(SIGINT, f);
	for (i = 0; i < 100; i++){
		printf("hello\n");
		sleep(1);
	}
	return 0;
}

void f(int signum){
	printf("OUCH!\n");
}
