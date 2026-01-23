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
#include "bm25.h"
#define N_BEST 10
#endif

using std::cout, std::cin, std::endl, std::string;

int main(int, char **);
Doc process_file(std::filesystem::path &,char);
void process_dirs(string &,char);
void eval_file(string &,char );
bool is_valid_file(const std::filesystem::directory_entry &);
std::pair<string,char> get_input(int,char **);