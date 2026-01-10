#include "Stemmer.h"
#include "utility.h"
#include <vector>

int measure(const std::string &s){
int c = 0;
bool vow = false;
for (auto c: s){
if (is_vowel(c)){
    if (vow) c += 1;
    else vow = true;
}
else vow = false;
}
return c;    
}




std::vector<std::string> Lanchaster::stem_input(){
std::vector<std::string> ret;
/*
Lanchaster Code
*/
return ret;
}

Stemmer::~Stemmer() = default;
std::vector<std::string> Stemmer::stem_input() {
    return vec; 
}

bool double_consonant(const std::string& w) {
    int n = w.size();
    if (n < 2) return false;
    char c = w[n-1];
    return w[n-1] == w[n-2] && !is_vowel(c);
}

bool cvc(const std::string &s){
auto l = s.length();
if (l < 3) return false;
char a = s[l-1],b = s[l-2],c = s[l-3];
return !is_vowel(a) && is_vowel(b) && !is_vowel(c);
}

bool contains_vowel(const std::string &s) {
for (auto c: s) {
if (is_vowel(c)){
    return true;
}
}
return false;
}


std::vector<std::string> Snowball::stem_input(){
std::vector<std::string> ret;
auto vec = this->vec;
for (auto &s : vec) {
    
if (ends_with(s,"sses")) {s.replace(s.length()-4,4,"ss");}
else if (ends_with(s,"ies")) {s.replace(s.length()-3,3,"i");}
else if (ends_with(s,"ss")) {continue;}
else if (ends_with(s,"s")) {s.pop_back();}


bool removed = false;
 if (ends_with(s,"ed")) {
        std::string stem = s.substr(0, s.size()-2);
        if (contains_vowel(stem)) {
            s = stem;
            removed = true;
        }
    }
else if (ends_with(s,"ing")) {
    std::string stem = s.substr(0, s.size()-3);
    if (contains_vowel(stem)) {
        s = stem;
        removed = true;
    }
}

if (!removed) {continue;};


if (ends_with(s, "at") || ends_with(s, "bl") || ends_with(s, "iz")){s += "e";}
else if (double_consonant(s)){
char c = s[s.length() - 1];
if (!(c == 'l' || c == 's' || c == 'z')) s.pop_back();
}
else if (measure(s) == 1 && cvc(s)) s += "e";









}
return ret;
}


std::vector<std::string> common_stages(std::vector<std::string> vec){




return vec;
}