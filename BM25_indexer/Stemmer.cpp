#include "includes/Stemmer.h"
#include "includes/utility.h"
#include <cctype>
#include <unordered_map>
#include <vector>

std::unordered_map<char,std::vector<Lanch_Rules>> lanch_map;

std::string rules[] ={
"ai*2.",
"a*1.",
"bb1.",
"city3s.",
"ci2>",
"cn1t>",
"dd1.",
"dei3y>",
"deec2ss.",
"dee1.",
"de2>",
"dooh4>",
"e1>",
"feil1v.",
"fi2>",
"gni3>",
"gai3y.",
"ga2>",
"gg1.",
"ht*2.",
"hsiug5ct.",
"hsi3>",
"i*1.",
"i1y>",
"ji1d.",
"juf1s.",
"ju1d.",
"jo1d.",
"jeh1r.",
"jrev1t.",
"jsim2t.",
"jn1d.",
"j1s.",
"lbaifi6.",
"lbai4y.",
"lba3>",
"lbi3>",
"lib2l>",
"lc1>",
"lufi4y.",
"luf3>",
"lu2.",
"lai3>",
"lau3>",
"la2>",
"ll1.",
"mui3>",
"mu*2.",
"msi3>",
"mm1.",
"nois4j>",
"noix4ct.",
"noi3>",
"nai3>",
"na2>",
"nee0.",
"pihs4>",
"pp1.",
"re2>",
"rae0.",
"ra2.",
"ro2>",
"ru2>",
"rr1.",
"suo3>",
"sis2.",
"si2>",
"sis0.",
"ssen4>",
"ss0.",
"suo4>",
"s1>",
"tacilp4y.",
"ta2>",
"tnem4>",
"tne3>",
"tna3>",
"tpir2b.",
"tpro2b.",
"tcud1.",
"tpmus2.",
"tpec2iv.",
"tulo2v.",
"tsis0.",
"tsi3>",
"tt1.",
"uqi3.",
"ugo1.",
"vis3j>",
"vie0.",
"vi2>",
"ylb1>",
"yli3y>",
"ylp0.",
"yl2>",
"ygo1.",
"yhp1.",
"ymo1.",
"ypo1.",
"yti3>",
"yte3>",
"ytl2.",
"zi2>",
"zy1s."
};



void show_rules() {
for (auto &[chr,rules]:lanch_map) {
  std::cout << chr << ": " << std::endl;
for (auto &rule:rules)
  std::cout << "  ["  << "Suffix: " << rule.suffix << "Pop Count: " << rule.pop_cnt << "To Append: " <<  rule.to_append << "Cflag: " << rule.continue_flag << "Iflag: " << rule.intact_flag << "]\n" ;
}
}


void init_lmap(){
for (auto &str : rules) {
int i = 0;
int l = str.size();
Lanch_Rules ret{};

while (i < l && is_alphabetic(str[i])){
  ret.suffix += std::tolower(str[i]);
  i += 1;
}

if (i < l && str[i] == '*') {
  ret.intact_flag = true;
  i += 1;
}

if (i < l && is_numeric(str[i])){
ret.pop_cnt = str[i] - '0';
i += 1;
}

while (i < l && str[i] != '>' && str[i] != '.' ) {
ret.to_append += str[i];
  i += 1;
}

if (i < l) {
    if (str[i] == '>') ret.continue_flag = true;
    else if (str[i] == '.') ret.continue_flag = false;
    else continue;

}
lanch_map[ret.suffix.back()].push_back(ret);
}
}




std::string apply_lrule(std::string & str,Lanch_Rules & rule) {
std::string ret("");
int l = (int)str.size();
ret = str.substr(0, l- rule.pop_cnt);
ret += rule.to_append;
return ret;  
}



std::vector<std::string> Lanchaster::stem_input() {
  init_lmap();
  std::vector<std::string> ret;
  auto vec = this->vec;
  for (auto &word: vec) {
    if (word.size() <= 3){
      ret.push_back(word);
      continue;
    }
   bool intact = true;
      while (true) {
          char key = word.back();
          if (lanch_map.find(key) == lanch_map.end())
              break;
          auto &rules = lanch_map[key];
          bool applied = false;
          for (auto &rule : rules) {
              if (rule.intact_flag && !intact)
                  continue;
              if (!ends_with(word, rule.suffix))
                  continue;
              int new_len = word.size() - rule.pop_cnt + rule.to_append.size();
              if (new_len < 2)
                  continue;
              word = apply_lrule(word, rule);
              intact = false;
              applied = true;
              if (!rule.continue_flag)
                  goto done_stemming;
              break;
          }
          if (!applied) break;
      }
      done_stemming:
      ret.push_back(word);

  }
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

