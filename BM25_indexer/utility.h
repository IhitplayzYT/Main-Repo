#ifndef UTILITY
#define UTILITY
#include <iostream>
#include <string>
#include <algorithm>
#include <cstring>
#endif
typedef signed char s8;
typedef signed short s16;
typedef signed int s32;
typedef signed long long s64;
typedef unsigned char i8;
typedef unsigned short i16;
typedef unsigned int i32;
typedef unsigned long long i64;

void usage(std::string);
bool is_vowel(char &c);
bool ends_with(const std::string &,const std::string &);
int measure(const std::string &);
int r1(std::string &);
int r2(std::string &,int);
bool double_consonant(const std::string &);
bool cvc(const std::string &);
bool contains_vowel(const std::string &);
std::string preprocess(std::string&);