#include "utility.h"

bool is_vowel(char &c){
    return c =='a' || c == 'e' || c == 'i' || c == 'o' || c == 'u';
}

bool ends_with(const std::string &word,const std::string &suffix){
return word.length() > suffix.length() && !word.compare(word.length() - suffix.length(),suffix.length(),suffix);
}