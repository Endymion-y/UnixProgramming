// Purpose: animation with user control, using O_ASYNC on fd
// Note: set_ticker() sends SIGALRM, handler does animation
//       keyboard sends SIGIO, main only calls pause()
// BUG: Type 'Q' cannot exit
#include <stdio.h>
#include <curses.h>
#include <signal.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "ticker.h"

#define MESSAGE "Hello"
#define BLANK "     "

int row = 10;
int col = 0;
int dir = 1;
int delay = 200;
int done = 0;

void on_alarm(int signum);
void on_input(int signum);
void enable_kbd_signals();

int main(){
    initscr();
    crmode();
    noecho();
    clear();

    signal(SIGIO, on_input);
    enable_kbd_signals();          // Turn on keyboard signals
    signal(SIGALRM, on_alarm);
    set_ticker(delay);

    move(row, col);
    addstr(MESSAGE);
    while (!done){
        pause();
    }
    endwin();
    return 0;
}

void on_input(int signum){
    int c = getch();

    if (c == 'Q' || c == EOF) done = 1;
    else if (c == ' ') dir = -dir;
}

void on_alarm(int signum){
    mvaddstr(row, col, BLANK);
    col += dir;
    mvaddstr(row, col, MESSAGE);
    refresh();

    if (dir == -1 && col <= 0) dir = 1;
    else if (dir == 1 && (col + strlen(MESSAGE)) >= COLS) dir = -1;
}

// Install a handler, tell kernel who to notify on input,
// enable signal
void enable_kbd_signals(){
    int fd_flags;

    fcntl(STDIN_FILENO, F_SETOWN, getpid());
    fd_flags = fcntl(0, F_GETFL);
    fcntl(STDIN_FILENO, F_SETFL, (fd_flags | O_ASYNC));
}