/* more03.c - version 0.3 of more
 * Read and print 24 lines then pause for a few special commands
 * Feature of version 0.2: reads from /dev/tty(device) for commands
 * rather than standard input
 */
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#define PAGELEN 24
#define LINELEN 512

struct termios original_mode;
// int original_flags;

void do_more(FILE *);
int see_more(FILE *);
void term_save();
void term_restore();
void set_crmode();

int main(int argc, char* argv[]){
	term_save();
	set_crmode();

	FILE* fp;
	if (argc == 1){
		do_more(stdin);
	}
	else {
		while (--argc){
			if ((fp = fopen(*++argv, "r")) != NULL){
				do_more(fp);
				fclose(fp);
			}
			else {
				term_restore();
				exit(1);
			}
		}
	}
	term_restore();
	return 0;
}

void term_save(){
	tcgetattr(STDIN_FILENO, &original_mode);
	// original_flags = fcntl(STDIN_FILENO, F_GETFL);
}

void term_restore(){
	tcsetattr(STDIN_FILENO, TCSANOW, &original_mode);
}

void set_crmode(){
	struct termios ttystate;
	tcgetattr(STDIN_FILENO, &ttystate);
	ttystate.c_lflag &= ~ICANON;         // No buffering
	ttystate.c_lflag &= ~ECHO;           // No echo either
	ttystate.c_cc[VMIN] = 1;              // Get a char at a time
	tcsetattr(STDIN_FILENO, TCSANOW, &ttystate);
}

/* Read PAGELEN lines, then call see_more() for further instructions */
void do_more(FILE* fp){
	char line[LINELEN];
	int num_of_lines = 0;
	int reply;
	FILE* fp_tty;
	fp_tty = fopen("/dev/tty", "r");
	if (fp_tty == NULL) exit(1);

	while (fgets(line, LINELEN, fp)){
		if (num_of_lines == PAGELEN){
			reply = see_more(fp_tty);
			if (reply == 0) break;
			num_of_lines -= reply;
		}
		if (fputs(line, stdout) == EOF) exit(1);
		num_of_lines++;
	}
}

/* Print message, wait for response, return # of lines to advance
 * q means no, space means yes, CR means one line
 */
int see_more(FILE* cmd){
	int c;
	printf("\033[7m more?\033[m\n");
	while ((c = getc(cmd)) != EOF) {
		if (c == 'q') return 0;
		if (c == ' ') return PAGELEN;
		if (c == '\n') return 1;
	}
	return 0;
}
