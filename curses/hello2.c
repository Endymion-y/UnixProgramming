// Show how to use curses functions with a loop
#include <stdio.h>
#include <curses.h>
#include <stdlib.h>

int main(){
	int i;

	initscr();
	clear();
	for (i = 0; i < LINES; i++){
		move(i, i + 1);
		if (i % 2) standout();
		addstr("Hello, world!");
		if (i % 2) standend();
	}
	refresh();
	getch();
	endwin();

	return 0;
}
