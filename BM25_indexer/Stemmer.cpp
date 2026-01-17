#include "Stemmer.h"
#include "utility.h"
#include <utility>
#include <vector>




// TODO:
std::vector<std::string> Lanchaster::stem_input() {
  std::vector<std::string> ret;
  /*
  Lanchaster Code
  */
  return ret;
}

Stemmer::~Stemmer() {} 

std::vector<std::string> Stemmer::stem_input() {std::cout << "hello" << std::endl; return vec; }



std::vector<std::string> Snowball::stem_input() {
  std::vector<std::string> ret;
  auto vec = this->vec;
  for (auto &s : vec) {
    if (ends_with(s, "sses")) {
      s.replace(s.length() - 4, 4, "ss");
    } else if (ends_with(s, "ies")) {
      s.replace(s.length() - 3, 3, "i");
    } else if (ends_with(s, "ss")) {
    } else if (ends_with(s, "s")) {
      s.pop_back();
    }

    bool removed = false;
    if (ends_with(s, "ed")) {
      std::string stem = s.substr(0, s.size() - 2);
      if (contains_vowel(stem)) {
        s = stem;
        removed = true;
      }
    } else if (ends_with(s, "ing")) {
      std::string stem = s.substr(0, s.size() - 3);
      if (contains_vowel(stem)) {
        s = stem;
        removed = true;
      }
    }

    if (removed) {
      if (ends_with(s, "at") || ends_with(s, "bl") || ends_with(s, "iz")) {
        s += "e";
      } else if (double_consonant(s)) {
        char c = s[s.length() - 1];
        if (!(c == 'l' || c == 's' || c == 'z'))
          s.pop_back();
      } else if (measure(s) == 1 && cvc(s))
        s += "e";
    }

    int r1_ = r1(s);
    static const std::vector<std::pair<std::string, std::string>> rul = {{"ational","ate"},{"tional","tion"},{"enci","ence"},{"anci","ance"},{"izer","ize"},{"bli","ble"},{"alli","al"},{"entli","ent"},{"eli","e"},{"ousli","ous"},{"ization","ize"}, {"ation","ate"},{"ator","ate"},{"alism","al"},{"iveness","ive"},{"fulness","ful"},{"ousness","ous"},{"aliti","al"},{"iviti","ive"},{"biliti","ble"}};

for (auto &[a, b] : rul) {
    if (ends_with(s, a) && (int)s.size() - (int)a.size() >= r1_) {
      s.replace(s.size() - a.size(), a.size(), b);
      break;
    }
  }

  r1_ = r1(s);
  int r2_ = r2(s, r1_);
  std::vector<std::pair<std::string, std::string>> rules = {{"icate","ic"},{"alize","al"},{"iciti","ic"},{"ical","ic"},{"ful",""},{"ness",""}};
  
bool applied = false;
  for (auto &[a, b] : rules) {
    if (ends_with(s, a) && (int)s.size() - (int)a.size() >= r1_) {
      s.replace(s.size() - a.size(), a.size(), b);
      applied = true;
      break;
    }
  }

  if (!applied && ends_with(s, "ative") && (int)s.size() - 5 >= r2_) {
    s.erase(s.size() - 5);
  }

  r1_ = r1(s);
  r2_ = r2(s, r1_);

  std::vector<std::string> suffixes = {
      "al",   "ance", "ence", "er",  "ic",  "able", "ible", "ant", "ement",
      "ment", "ent",  "ism",  "ate", "iti", "ous",  "ive",  "ize"};

bool f = 0;
  for (std::string &suf : suffixes) {
    if (ends_with(s,suf) && (int)s.size()-(int)suf.size()>=r2_ && s.size()>suf.size()) {
      s.erase(s.size()-suf.size());
      f = true;
      break;
      }
    }

  if (!f && ends_with(s,"ion") && (int)s.size()-3>=r2_) { 
    char c = s[s.size()-4];
    if (c=='s' || c=='t')
      s.erase(s.size()-3);    
    }


  r1_ = r1(s);
  r2_ = r2(s, r1_);

  if (ends_with(s, "e")) {
    std::string stem = s.substr(0,s.size()-1);
    int pos = stem.size();
    if (pos >= r2_ || (pos >= r1_ && !cvc(stem))) {
      s.pop_back();
    }
  }

  if (ends_with(s, "ll") && ((int)s.length() - 1 >= r2_)) {
    s.pop_back();
  }

  ret.push_back(s);
}
return ret;
}

