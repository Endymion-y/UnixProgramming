// Purpose: animation with user control, using aio_read() etc
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
#include <aio.h>
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
void setup_aio_buffer();

struct aiocb kbcbuf;          // An aio control buffer

int main(){
    initscr();
    crmode();
    noecho();
    clear();

    signal(SIGIO, on_input);
    setup_aio_buffer();       // Initialize aio control buffer
    aio_read(&kbcbuf);        // Place a read request

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

// Handler called when aio_read() has stuff to read
// First check for any error codes, and if ok, then get the return code
void on_input(int signum){
    int c;
    char* cp = (char*)kbcbuf.aio_buf;

    // Check for errors
    if (aio_error(&kbcbuf) != 0) perror("Reading failed");
    else {
        // Get number of chars read
        if (aio_return(&kbcbuf) == 1){
            c = *cp;
            if (c == 'Q' || c == EOF) done = 1;
            else if (c == ' ') dir = -dir;
        }
    }
    // Place a new request
    aio_read(&kbcbuf);
}

void on_alarm(int signum){
    mvaddstr(row, col, BLANK);
    col += dir;
    mvaddstr(row, col, MESSAGE);
    refresh();

    if (dir == -1 && col <= 0) dir = 1;
    else if (dir == 1 && (col + strlen(MESSAGE)) >= COLS) dir = -1;
}

// Set numbers of struct
// First specify args like those for read (fd, buf, num) and offset
// Then specify what to do (send signal) and what signal (SIGIO)
void setup_aio_buffer(){
    static char input[1];      // 1 char of input
    // Describe what to read
    kbcbuf.aio_fildes = STDIN_FILENO;    // Standard input
    kbcbuf.aio_buf = input;              // Buffer
    kbcbuf.aio_nbytes = 1;               // Number to read
    kbcbuf.aio_offset = 0;               // Offset in file

    // Describe what to do when read is ready
    kbcbuf.aio_sigevent.sigev_notify = SIGEV_SIGNAL;
    kbcbuf.aio_sigevent.sigev_signo = SIGIO;   // Send SIGIO
}