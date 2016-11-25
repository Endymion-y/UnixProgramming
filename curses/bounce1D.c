// Purpose: animation with user controlled speed and direction
// Note: the handler does the animation, and the main program reads keyboard input
// Compile: cc bounce1D.c -l curses -o bounce1D
#include <stdio.h>
#include <curses.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>

#define MESSAGE "Hello"
#define BLANK "     "

int row;        // Current row
int col;        // Current column
int dir;        // Current direction

void move_msg(int signum);      // Handler for timer
int set_ticker(int n_msecs);

int main(){
    int delay;
    int ndelay;

    int c;         // User input
    
    initscr();
    crmode();
    noecho();
    clear();

    row = 10;
    col = 0;
    dir = 1;
    delay = 200;

    move(row, col);
    addstr(MESSAGE);
    signal(SIGALRM, move_msg);
    set_ticker(delay);

    while (1){
        ndelay = 0;
        c = getch();
        if (c == 'Q') break;
        if (c == ' ') dir = -dir;
        if (c == 'f' && delay > 2) ndelay = delay / 2;
        if (c == 's') ndelay = delay * 2;
        if (ndelay > 0) set_ticker(delay = ndelay);
    }
    endwin();
    return 0;
}

void move_msg(int signum){
    signal(SIGALRM, move_msg);   // Reset, just in case
    move(row, col);
    addstr(BLANK);
    col += dir;                 // Move to new column
    move(row, col);             // Then set cursor
    addstr(MESSAGE);
    refresh();

    // Handle borders
    if (dir == -1 && col <= 0) dir = 1;
    else if (dir == 1 && (col + strlen(MESSAGE) >= COLS)) dir = -1;
}

// Arranges for interval timer to issue SIGALRMs at regular interval
// Returns -1 on error, 0 for OK
// Arg in milliseconds, converted into whole seconds and microseconds
// Note: set_ticker(0) turns off ticker
int set_ticker(int n_msecs){
	struct itimerval new_timeset;
	long n_sec, n_usecs;

	n_sec = n_msecs / 1000;
	n_usecs = (n_msecs % 1000) * 1000L;

	new_timeset.it_interval.tv_sec = n_sec;
	new_timeset.it_interval.tv_usec = n_usecs;

	new_timeset.it_value.tv_sec = n_sec;
	new_timeset.it_value.tv_usec = n_usecs;

	return setitimer(ITIMER_REAL, &new_timeset, NULL);
}