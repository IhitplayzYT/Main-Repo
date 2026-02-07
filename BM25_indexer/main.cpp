#include "includes/main.h"
#include "includes/Stemmer.h"
#include "includes/errors.h"
#include "includes/fzf_tui.h"
#include "includes/stopwords.h"
#include "includes/utility.h"
#include <filesystem>
#include <fstream>
#include <ostream>
#include <string>
#include <vector>

extern const std::unordered_set<std::string> allowed_extensions;
Debug GLOBAL_DEBUG_STRUCT;
Corpus CORPUS;


bool is_valid_file(const std::filesystem::directory_entry& e) {
    if (!e.is_regular_file()) return false;
    auto ext = e.path().extension().string();
    return allowed_extensions.find(ext) != allowed_extensions.end();
}

Doc process_file(const std::filesystem::path &x,char optimise){
std::ifstream file(x);
if (!file) {exit(-1);}
std::ostringstream buff;
buff << file.rdbuf();
string text = buff.str();
text = preprocess(text);
vector<string> filtered_wordlist;
if (optimise == 'f' || optimise == 'x') {
GLOBAL_DEBUG_STRUCT.t_stopword = "No filter";
filtered_wordlist = tokenize_ws(text);
}else{
GLOBAL_DEBUG_STRUCT.t_stopword = (!(optimise == 's') || optimise >> 1) ?"Aggresive":"Non-Aggresive";
filtered_wordlist = filter_stopwords(text,(optimise == 's') ? 0: optimise >> 1);
}
GLOBAL_DEBUG_STRUCT.stop_prog = 1;
std::vector<string> ret;
if (optimise == 'x' || optimise == 's'){
GLOBAL_DEBUG_STRUCT.t_stemmer = "No Stemmer";
ret = filtered_wordlist; 
GLOBAL_DEBUG_STRUCT.stem_prog = 1;
}
else{

if (optimise == 'f' || ! (optimise & 1))
{Snowball stemmer(filtered_wordlist);
GLOBAL_DEBUG_STRUCT.t_stemmer = "Snowball";
ret = stemmer.stem_input();
GLOBAL_DEBUG_STRUCT.stem_prog = 1;
}
else{
Lanchaster stemmer(filtered_wordlist);
GLOBAL_DEBUG_STRUCT.t_stemmer = "Lanchaster";
ret = stemmer.stem_input();
GLOBAL_DEBUG_STRUCT.stem_prog = 1;
}
}
return std::pair(x,ret);
}

void traverse(std::filesystem::path &path,Corpus &corp,char optimise){
for (auto &file: std::filesystem::recursive_directory_iterator(path)){
if (!is_valid_file(file))  continue;
auto &p = file.path();
Doc doc = process_file(p,optimise);
if (!doc.second.empty()) corp.push_back(std::move(doc));
}
}

void process_dirs(string &path,char optimise){
std::filesystem::path p(path);
CORPUS.clear();
traverse(p,CORPUS,optimise);
}


void eval_file(string &input_path,char optimise) {
     if (std::filesystem::is_directory(input_path)){
      process_dirs(input_path,optimise);
     }else{
      throw InvalidIO("Invalid file type\n");
     }

}

int main(int argc, char *argv[]) {
cout << endl;
auto [input_path,optimise] = get_input(argc,argv);
enable_mode();

try{
  eval_file(input_path,optimise);
} 
catch (const InvalidIO &e){
  cout << e.what();
  return -1;
} catch (const PathError &e){
  cout << e.what();
  return -1;
}

string input;
vector<string> ret;
vector<int> matches;
int selected = 0;

while (true){
char c(0);
Keys k = read_key(c);
if (k == KEY_ESC) {disable_mode();cout << '\n';return 0;}
if (k == KEY_BACKSPACE && !input.empty())input.pop_back();
else if (k == KEY_ENTER && !matches.empty()){
  clear();
  std::cout << matches[selected] << "\n";
  break;
} else if (k == KEY_UP && selected > 0){
  selected --;
}
else if (k == KEY_DOWN && selected < (int)matches.size() - 1){
  selected ++;
}else if (c >= 32 && c <= 126){input.push_back(c);}
input = preprocess(input);

vector<string> filtered_wordlist;
if (optimise == 'f' || optimise == 'x') {
GLOBAL_DEBUG_STRUCT.t_stopword = "No filter";
filtered_wordlist = tokenize_ws(input);
}else{
GLOBAL_DEBUG_STRUCT.t_stopword = (!(optimise == 's') || optimise >> 1) ?"Aggresive":"Non-Aggresive";
filtered_wordlist = filter_stopwords(input,(optimise == 's') ? 0: optimise >> 1);
}

if (optimise == 'f' || !(optimise & 1))
{Snowball stemmer(filtered_wordlist);
ret = stemmer.stem_input();
}
else{
Lanchaster stemmer(filtered_wordlist);
ret = stemmer.stem_input();
}

matches = rank_corpus(CORPUS,ret);
draw(input, CORPUS,matches,selected);
}
  return 0;
}