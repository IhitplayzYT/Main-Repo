#include "fzf_tui.h"
#include <termios.h>
#include <unistd.h>

extern termios orig_termios;

void disable_mode(){
    tcsetattr(STDIN_FILENO,TCSAFLUSH, &orig_termios);
}

void enable_mode(){
    tcgetattr(STDIN_FILENO,&orig_termios);
    atexit(disable_mode);
    termios cur = orig_termios;
    cur.c_lflag &= ~(ECHO | ICANON /*| ISIG*/);
    cur.c_iflag &= ~(IXON | ICRNL);
    //cur.c_oflag &= ~(OPOST);
    tcsetattr(STDIN_FILENO,TCSAFLUSH,&cur);
}

void clear() {
    cout << "\x1b[2J\x1b[H";
}


