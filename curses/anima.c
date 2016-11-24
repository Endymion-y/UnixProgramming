// Simple animation
// cmd: cc anima.c -l curses
#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <unistd.h>
#include <string.h>

#define HIGH 23 
#define WIDTH 79

int main(){
	initscr();      // Turn on curses
	clear();        // Clear screen
	int x, y, xdir, ydir;
	x = 0;
	y = 0;
	xdir = 1;
	ydir = 1;
	char str[20];

	int time = 10;
	while (time--){
		clear();
		move(x, y);
		sprintf(str, "x=%d, y=%d", x, y);
		//addstr(str);
		addstr("あ");
		move(LINES - 1, 0);
		refresh();
		if (x == 0 && xdir == -1) xdir = 1;
		if (x == HIGH && xdir == 1) xdir = -1;
		if (y == 0 && ydir == -1) ydir = 1;
		if (y == WIDTH && ydir == 1) ydir = -1;
		x += xdir;
		y += ydir;
		sleep(1);
	}
	endwin();

	return 0;
}
