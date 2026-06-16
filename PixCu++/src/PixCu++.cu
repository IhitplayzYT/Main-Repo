/* PixCu++.c */
#include "../include/PixCu++.hpp"
#include <string>
#include <vector>

int main(int argc, char *argv[]) {
  std::vector<std::string> params;
  for (ssize_t i = 0; i < argc; i++) {
    params.push_back(std::string{argv[i]});
  }
  CLI clargs{};
  clargs.parse_args(params);
  if (clargs.dbg) {
    clargs.print();
  }

  if (clargs.accelerate) {
    gpu_main(clargs);
  } else {
    cpu_main(clargs);
  }

  return 0;
}
