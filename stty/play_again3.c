/* play_again1.c
 * Purpose: ask if user wants another transaction
 * Method: set tty into char-by-char mode, read char, return result
 * Return value: 0=>yes, 1=>no
 * Defect: Echo inappropriate input
 */
#include <stdio.h>
#include <termios.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define QUESTION "Do you want another transaction"
#define TRIES 1000
#define SLEEPTIME 2
#define BEEP putchar('\a');

struct termios original_mode;
int original_flags;
int get_response(char* question, int tries);
void set_crmode();
int get_ok_char();
void set_nodelay_mode();

int main(){
	int response;
	// Save tty mode
	tcgetattr(0, &original_mode);
	original_flags = fcntl(0, F_GETFL);
	// Set char-by-char mode
	set_crmode();
	set_nodelay_mode();
	response = get_response(QUESTION, TRIES);

	// Restore tty mode
	tcsetattr(0, TCSANOW, &original_mode);
	fcntl(0, F_SETFL, original_flags);
	return response;
}

int get_response(char* question, int tries){
	int input;
	printf("%s (y/n)?", question);
	fflush(stdout);
	while (1){
		sleep(SLEEPTIME);
		input = get_ok_char();
		if (input == 'y' || input == 'Y') return 0;
		if (input == 'n' || input == 'N') return 1;
		// printf("%d\n", tries);
		// fflush(stdout);
		if (tries-- == 0) return 2;
		BEEP;
	}
}

int get_ok_char(){
	int c;
	while ((c = getchar()) != EOF && strchr("yYnN", c) == NULL);
	return c;
}

void set_crmode(){
	struct termios ttystate;
	tcgetattr(0, &ttystate);
	ttystate.c_lflag &= ~ICANON;      // No buffering
	ttystate.c_lflag &= ~ECHO;        // No echo either
	ttystate.c_cc[VMIN] = 1;          // Get 1 char at a time
	tcsetattr(0, TCSANOW, &ttystate);
}

void set_nodelay_mode(){
	int termflags;
	termflags = fcntl(0, F_GETFL);
	termflags |= O_NDELAY;
	fcntl(0, F_SETFL, termflags);
}