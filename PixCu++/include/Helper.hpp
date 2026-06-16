#include <string>
#include <vector>
class CLI {
public:
  bool dbg, accelerate;
  std::string fpath;

  CLI();
  void parse_args(std::vector<std::string>);
  void print() const;
};

void HELP();
