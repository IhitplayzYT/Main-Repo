#pragma once
#ifndef MAIN
#define MAIN
#include <dirent.h>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <cstring>
#include <thread>
#include <vector>
#include <utility>
#include <cctype>
#include <algorithm>
#include "stopwords.h"
#include "Stemmer.h"
#include "utility.h"
#include "errors.h"
#endif
using std::cout, std::cin, std::endl, std::string;

int main(int, char **);
void process_file(string &,char);
void process_dirs(string &,char);
void  valid_file(string &,char );
void usage(string);
std::pair<string,char> get_input(int,char **);