// Hello1.c
// Purpose: show the minimal calls needed to use curses
// Outline: initialize, draw stuff, wait for input, quit
// Compile: cc hello1.c -l curses

#include <stdio.h>
#include <stdlib.h>
#include <curses.h>

int main(){
	initscr();      // Turn on curses
	clear();        // Clear screen
	move(10, 20);   // Row 10, col 20
	addstr("Hello, world!"); // Add a string
	move(LINES - 1, 0);       // Move to LL
	refresh();               // Update the screen
	getchar();               // Wait for user input
	endwin();

	return 0;
}
