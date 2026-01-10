#include "main.h"
#include <algorithm>

void usage(string x) {
  cout << "Usage " << x << " [-OPTIMISE=0/1] <FILEPATH> " << endl;
}
void process_file(string &x,char optimise){
std::ifstream file(x);
if (!file) {exit(-1);}
std::stringstream buff;
buff << file.rdbuf();
string text = buff.str();
std::copy_if(text.begin(),text.end(),text.begin(),[] (char c) {return c != '"' && c != '\'' && c != ',' && c != '.';});
std::transform(text.begin(),text.end(),text.begin(),[] (char c) {return std::tolower(c);});
auto filtered_wordlist = filter_stopwords(text);
for (auto &s:filtered_wordlist) {
  cout << s << " ";
}
cout << endl;
//std::unique_ptr<Stemmer> stemmer_model = (optimise) ? std::make_unique<Snowball>(filtered_wordlist):std::make_unique<Lanchaster>(filtered_wordlist);

}


void process_dirs(string &x,char optimise){
}

void  valid_file(string &input_path,char optimise) {
     if (std::filesystem::is_regular_file(input_path)){
      process_file(input_path,optimise);
     }else if (std::filesystem::is_directory(input_path)){
      process_dirs(input_path,optimise);
     }else{
      throw InvalidIO("Invalid file type\n");
     }

}


// MAKE IT CHECK FOR TOKEN OPTIMISE= :TODO:
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
    optimise = (s[s.length() - 1] == '1' || s[s.length() - 1] == '0') ? s[s.length() - 1] : 3;
    if (optimise == 3) {usage(argv[0]);exit(-1);}
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
  valid_file(input_path,optimise);
  } catch (const InvalidIO &e){
    cout << e.what();
    return -1;
  } catch (const PathError &e){
    cout << e.what();
    return -1;
  }



  
  return 0;
}
