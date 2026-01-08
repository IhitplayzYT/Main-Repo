#include "main.h"
#include <filesystem>
#include <system_error>

void usage(string x) {
  cout << "Usage " << x << " [-OPTIMISE=0/1] <FILEPATH> " << endl;
}


int main(int argc, char *argv[]) {
  if (argc < 2) {usage(argv[0]);return -1;}
  if (argc > 3) {usage(argv[0]);return -1;}
  string input_path;
  char optimise = 0;
  if (argc == 2) {
    input_path = argv[1];
    optimise = 0;
    std::error_code ec;
    if (!std::filesystem::exists(input_path, ec))
        throw PathError("Path does not exist: " + input_path);
    if (!std::filesystem::is_directory(input_path, ec))
        throw PathError("Path is not a directory: " + input_path);
     }
  else{
    string s(argv[1]);
    optimise = (s[s.length() - 1] == '1' || s[s.length() - 1] == '0') ? s[s.length() - 1] : 3;
    if (optimise == 3) {usage(argv[0]);return -1;}
    input_path = argv[2];
    std::error_code ec;
    if (!std::filesystem::exists(input_path, ec))
        throw PathError("Path does not exist: " + input_path);
    if (!std::filesystem::is_directory(input_path, ec))
        throw PathError("Path is not a directory: " + input_path);
     }

  input_path = (input_path.empty()) ? input_path : "/";  




  
  return 0;
}
