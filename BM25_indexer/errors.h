#pragma once
#ifndef ERRORS
#define ERRORS
#include <exception>
#include <stdexcept>
#include "utility.h"
#endif

class PathError : public std::runtime_error {
public:
    PathError(const std::string &s) : std::runtime_error(s) {}
};

class InvalidIO : public std::runtime_error {
public:
    InvalidIO(const std::string &x) : std::runtime_error(x) {}
};


class InvalidArgs : public std::runtime_error{
public:
    InvalidArgs(const std::string &s) : std::runtime_error(s) {}
};


typedef struct debug {
std::string t_stemmer,t_stopword;
bool stem_prog,stop_prog;
}Debug;

void debug_show();
