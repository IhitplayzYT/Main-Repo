#ifndef FZF
#define FZF
#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <vector>
#include <string>
#include <csignal>

using std::string,std::cout,std::cin,std::endl,std::vector;
static termios orig_termios;
#endif

void disable_mode();
void enable_mode();
void clear();
