/* play_again1.c
 * Purpose: ask if user wants another transaction
 * Method: set tty into char-by-char mode, read char, return result
 * Return value: 0=>yes, 1=>no
 * Defect: Echo inappropriate input
 */
#include <stdio.h>
#include <termios.h>

#define QUESTION "Do you want another transaction"

struct termios original_mode;
int get_response(char* question);
void set_crmode();

int main(){
	int response;
	// Save tty mode
	tcgetattr(0, &original_mode);
	// Set char-by-char mode
	set_crmode();
	response = get_response(QUESTION);

	// Restore tty mode
	tcsetattr(0, TCSANOW, &original_mode);
	return response;
}

int get_response(char* question){
	int input;
	printf("%s (y/n)?", question);
	while (1){
		switch (input = getchar()){
			case 'y':
			case 'Y': return 0;
			case 'n':
			case 'N':
			case EOF: return 1;
			default:
			printf("\nCannot understand %c, ", input);
			printf("Please type y or n\n");
		}
	}
}

void set_crmode(){
	struct termios ttystate;
	tcgetattr(0, &ttystate);
	ttystate.c_lflag &= ~ICANON;      // No buffering
	ttystate.c_lflag &= ~ECHO;        // No echo either
	ttystate.c_cc[VMIN] = 1;          // Get 1 char at a time
	tcsetattr(0, TCSANOW, &ttystate);
}