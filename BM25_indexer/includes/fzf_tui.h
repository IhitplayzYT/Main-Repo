#ifndef FZF
#define FZF
#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <vector>
#include <string>
#include <csignal>
#include "bm25.h"

using std::string,std::cout,std::cin,std::endl,std::vector;
static termios orig_termios;
enum Keys {KEY_NONE,KEY_UP,KEY_DOWN,KEY_ENTER,KEY_ESC,KEY_BACKSPACE};
#endif

void disable_mode();
void enable_mode();
void clear();
Keys read_key(char &out);
void draw(std::string & query,Corpus& corp,vector<int> matches,int select);