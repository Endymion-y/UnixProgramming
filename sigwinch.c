// Test with SIGWINCH signal
#include <stdio.h>
#include <termios.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/ioctl.h>

void change(int signum);

int main(){
	signal(SIGWINCH, change);

	int c;
	while ((c = getchar()) != EOF);

	return 0;
}

void change(int signum){
	static char fillch = 'A';
	struct winsize ws;
	if (ioctl(0, TIOCGWINSZ, &ws) == -1)
		exit(-1);
	int times = ws.ws_row * ws.ws_col;
	int i;
	for (i = 1; i <= times; i++) printf("%c", fillch);
	printf("\n");
	fillch = (fillch - 'A' + 1) % 26 + 'A';
}
