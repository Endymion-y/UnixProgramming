/* more01.c - version 0.1 of more
 * read and print 24 lines then pause for a few special commands
 */
#include <stdio.h>
#define PAGELEN 24
#define LINELEN 512

void do_more(FILE *);
int see_more();

int main(int argc, char* argv[]){
	FILE* fp;
	if (ac == 1){
		do_more(stdin);
	}
	else {
		while (--ac){
			if ((fp = fopen(*++av, "r")) != NULL){
				do_more(fp);
				fclose(fp);
			}
			else exit(1);
		}
	}
	return 0;
}

/* Read PAGELEN lines, then call see_more() for further instructions */
void do_more(FILE* fp){
	char line[LINELEN];
	int num_of_lines = 0;
	int reply;

	while (fgets(line, LINELEN, fp)){
		if (num_of_lines == PAGELEN){
			reply = see_more();
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
int see_more(){
	int c;
	printf("\033[7m more?\033[m\n");
	while ((c = getchar()) != EOF) {
		if (c == 'q') return 0;
		if (c == ' ') return PAGELEN;
		if (c == '\n') return 1;
	}
	return 0;
}