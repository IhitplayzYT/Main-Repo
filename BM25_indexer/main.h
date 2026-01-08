#ifndef MAIN
#define MAIN
#include <dirent.h>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include "stopwords.cpp"
#include "Stemmer.h"
#include "errors.h"
#endif
using std::cout, std::cin, std::endl, std::string;

int main(int, char **);
void usage(string);