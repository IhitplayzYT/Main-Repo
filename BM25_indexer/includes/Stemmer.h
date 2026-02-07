#pragma once
#ifndef PORTER
#define PORTER 
#include <vector>
#include <string>
#include <algorithm>
#include "utility.h"
#endif

class Stemmer {
public:
    std::vector<std::string> vec;
    Stemmer(const std::vector<std::string> &v) : vec(v) {}
    virtual ~Stemmer();
    virtual std::vector<std::string> stem_input();
};

class Lanchaster : public Stemmer{
public:
    Lanchaster(const std::vector<std::string> &v) : Stemmer(v) {};

    std::vector<std::string> stem_input() override;
};

class Snowball : public Stemmer{
public:
    Snowball(const std::vector<std::string> &v) : Stemmer(v) {}
    std::vector<std::string> stem_input() override;

};



typedef struct s_Lanchaster{
std::string suffix; 
bool intact_flag = false;
int pop_cnt = 0;
std::string to_append;
bool continue_flag = false;
} Lanch_Rules;

void show_rules();
