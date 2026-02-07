#include "includes/fzf_tui.h"
#include <ostream>
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
    cur.c_lflag &= ~(ECHO | ICANON);
    cur.c_iflag &= ~(IXON | ICRNL);
    tcsetattr(STDIN_FILENO,TCSANOW,&cur);
}

void clear() {
    cout << "\x1b[2J\x1b[H";
}

Keys read_key(char &out) {
    char c;
    if (read(STDIN_FILENO,&c,1) <= 0) return KEY_NONE;
    switch (c) {
        case 27:{
            char buf[2];
            if ((read(STDIN_FILENO,&buf,2) == 2) && (buf[0] == '[')){
                return (buf[1] == 'A') ? KEY_UP :(buf[1] == 'B') ? KEY_DOWN : KEY_ESC;
            }
            return KEY_ESC;
        }
        case '\n':{
            return KEY_ENTER;
        }
        case 127:{
            return KEY_BACKSPACE;
        }
    }
out = c;
return KEY_NONE;
}

string get_preview(Doc &doc,string & query) {
for (auto &word : doc.second){
    if (word.find(query) != string::npos){
        return "..... " + word + " .....";
    }
}
return "[No Match]";
}


void draw(std::string & query,Corpus& corp,vector<int> matches,int select){
clear();
cout << "Query: " << query << "\n\n";
for (int i  = 0 ; i < (int)matches.size(); i++){
if (i == select) {
    cout <<  "\x1b[7m";;
    cout << (i+1) << ". " << corp[matches[i]].first;
    cout << "\x1b[0m\n";
}else{
cout << (i+1) << "." << corp[matches[i]].first << endl;
}
}

if (!matches.empty()){
    cout  << "\n---------------- Preview ----------------\n";
    cout << get_preview(corp[matches[select]],query) ;
}
cout << "\n\n↑↓ navigate  Enter select  ESC quit";
cout.flush();
}