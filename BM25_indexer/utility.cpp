#include "utility.h"
#include "errors.h"

bool is_vowel(char &c){
    return c =='a' || c == 'e' || c == 'i' || c == 'o' || c == 'u';
}

bool ends_with(const std::string &word,const std::string &suffix){
return word.length() > suffix.length() && !word.compare(word.length() - suffix.length(),suffix.length(),suffix);
}

int measure(const std::string &s) {
  int c = 0;
  bool vow = false;
  for (auto c : s) {
    if (is_vowel(c)) {
      if (vow)
        c += 1;
      else
        vow = true;
    } else
      vow = false;
  }
  return c;
}

int r1(std::string &arg) {
  int l = arg.size();
  for (int i = 1; i < l; i++) {
    if (!is_vowel(arg[i - 1]) && !is_vowel(arg[i])) {
      return i + 1;
    }
  }
  return arg.length();
}

int r2(std::string &arg, int idx) {
  int l = arg.length();
  for (int i = idx + 1; i < l; i++) {
    if (!is_vowel(arg[i - 1]) && is_vowel(arg[i])) {
      return i + 1;
    }
  }
  return arg.length();
}

bool double_consonant(const std::string &w) {
  int n = w.size();
  if (n < 2)
    return false;
  char c = w[n - 1];
  return w[n - 1] == w[n - 2] && !is_vowel(c);
}

bool cvc(const std::string &s) {
  auto l = s.length();
  if (l < 3)
    return false;
  char a = s[l - 1], b = s[l - 2], c = s[l - 3];
  if (!is_vowel(a) && is_vowel(b) && !is_vowel(c)) {
    return !(a == 'w' || a == 'x' || a == 'y');
  }
  return false;
}

bool contains_vowel(const std::string &s) {
  for (auto c : s) {
    if (is_vowel(c)) {
      return true;
    }
  }
  return false;
}

std::pair<std::string,char> get_input(int argc,char ** argv){
  if (argc < 2) {usage(argv[0]);exit(-1);}
  if (argc > 3) {usage(argv[0]);exit(-1);}
  std::string input_path;
  char optimise = 0;
  if (argc == 2) {
    input_path = argv[1];
    optimise = 0;
    std::error_code ec;
    if (!std::filesystem::exists(input_path, ec))
        throw PathError("Path does not exist: " + input_path);
     }
  else{
    std::string s(argv[1]);
    std::string opt[2] = {"-OPTIMISE=","-O"};
    if (s.find(opt[0]) == std::string::npos && s.find(opt[1]) == std::string::npos){
      usage(argv[0]);
      throw InvalidArgs("Invalid Flag [" + s +  "] found instead of OPTIMISE");
    }
    int l(s.length());
    optimise = (s[l - 1] == '0' || s[l-1] == '1' || s[l-1]== '2' || s[l-1] == '3' ) ? s[l-1] - '0': 4;
    if (optimise == 4) {usage(argv[0]);exit(-1);}
    input_path = argv[2];
    std::error_code ec;
    if (!std::filesystem::exists(input_path, ec))
        throw PathError("Path does not exist: " + input_path+"\n");
      }
  input_path = (!input_path.empty()) ? input_path : "/"; 
  return {input_path,optimise};
}

std::string preprocess(std::string &text){
std::transform(text.begin(),text.end(),text.begin(),[] (char c) {return std::tolower(c);});
std::string ret("");
for (char c:text){
if ( c != '"' && c != '\'' && c != ',' && c != '.' && c != ';' && c != ':' && c != '`' && c !='\\' && c != '/' && c != '\t' && c != '\n')
ret += c;
} 
return ret;
}


void usage(std::string x) {
  std::cout << "Usage " << x << " [-OPTIMISE=0/1/2 | -O[0/1/2]] <FILEPATH> " << std::endl << "\t -OPTIMIZE VALUES:" << std::endl <<
  "\t\t 0: Non Aggressive stopword removal && Snowball Stemmer(Reliable Search)" << std::endl <<
  "\t\t 1: Non Aggressive stopword removal && Lanchaster Stemmer(Quick Search)" << std::endl <<
  "\t\t 2: Aggressive stopword removal && Snowball Stemmer(Reliable Search)" << std::endl << 
  "\t\t 3: Aggressive stopword removal && Lanchaster Stemmer(Quick Search)" << std::endl;
}

