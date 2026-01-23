#include "main.h"
#include "Stemmer.h"
#include "errors.h"
#include "utility.h"
#include <filesystem>
#include <fstream>


extern const std::unordered_set<std::string> allowed_extensions;
// HOLDS INFO OF THE MOST RECENT STATUS OF REGIME
Debug GLOBAL_DEBUG_STRUCT;

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
GLOBAL_DEBUG_STRUCT.t_stopword = (optimise >> 1) ?"Aggresive":"Non-Aggresive";
auto filtered_wordlist = filter_stopwords(text,optimise >> 1);
GLOBAL_DEBUG_STRUCT.stop_prog = 1;
std::vector<string> ret;
if (!(optimise & 1))
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
return std::pair(x,ret);
}

void traverse(std::filesystem::path &path,Corpus &corp,char optimise){
for (auto &file: std::filesystem::recursive_directory_iterator(path)){
if (!is_valid_file(file))  continue;
auto &p = file.path();
Doc doc = process_file(p,optimise);
if (!doc.second.empty()) corp.push_back(doc);
}
}

void process_dirs(string &path,char optimise){
Corpus corp;
std::filesystem::path p(path);
traverse(p,corp,optimise);
int avgdl = avg_doc_len(corp);
int N = corp.size();



std::vector<std::pair<string, double>> ret;
std::sort(ret.begin(),ret.end(),[](auto &a,auto &b){return a.second > b.second;});
}



void eval_file(string &input_path,char optimise) {
     if (std::filesystem::is_directory(input_path)){
      process_dirs(input_path,optimise);
     }else{
      throw InvalidIO("Invalid file type\n");
     }

}

std::pair<string,char> get_input(int argc,char ** argv){
  if (argc < 2) {usage(argv[0]);exit(-1);}
  if (argc > 3) {usage(argv[0]);exit(-1);}
  string input_path;
  char optimise = 0;
  if (argc == 2) {
    input_path = argv[1];
    optimise = 0;
    std::error_code ec;
    if (!std::filesystem::exists(input_path, ec))
        throw PathError("Path does not exist: " + input_path);
     }
  else{
    string s(argv[1]);
    string opt[2] = {"-OPTIMISE=","-O"};
    if (s.find(opt[0]) == string::npos && s.find(opt[1]) == string::npos){
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

int main(int argc, char *argv[]) {
  cout << endl;
auto [input_path,optimise] = get_input(argc,argv);
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

  show_n(N_BEST);
  return 0;
}