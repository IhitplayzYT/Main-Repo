#pragma once
#ifndef STOPWORDS
#define STOPWORDS
#include <string>
#include <unordered_set>
#include <vector>
#include <sstream>
#include "utility.h"
#endif

std::vector<std::string> filter_stopwords(std::string &,bool);

