// Purpose: show answers to signal questions
// Question 1: Does the handler stay in effect after a signal arrives?
// Question 2: What if a signalX arrives while handling signalX?
// Question 3: What if a signalX arrives while handling signalY?
// Question 4: What happens to read() when a signal arrives
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

#define INPUTLEN 100

void inthandler(int signum);
void quithandler(int signum);

int main(int argc, char* argv[]){
	char input[INPUTLEN];
	int nchars;

	signal(SIGINT, inthandler);
	signal(SIGQUIT, quithandler);

	do {
		printf("\nType a message:\n");
		nchars = read(STDIN_FILENO, input, (INPUTLEN - 1));
		if (nchars == -1)
			perror("Read returned an error");
		else {
			input[nchars] = '\0';
			printf("You typed: %s", input);
		}
	}
	while (strncmp(input, "quit", 4));

	return 0;
}

void inthandler(int signum){
	printf("Received signal %d .. waiting\n", signum);
	sleep(2);
	printf("Leaving inthandler\n");
}

void quithandler(int signum){
	printf("Received signal %d .. waiting\n", signum);
	sleep(3);
	printf("Leaving quithandler\n");
}
