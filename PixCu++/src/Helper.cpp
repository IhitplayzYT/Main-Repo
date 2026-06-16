#include "../include/PixCu++.hpp"
#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <iostream>
#include <ostream>
#include <string>
#include <sys/types.h>
#include <unordered_set>
#include <vector>

#define DBG_STR                                                                \
  "PixCu++ - GPU-accelerated image processing utility\n\nUsage:\n    pixcu++ " \
  "[OPTIONS] <fpath>\n\nArguments:\n    <fpath>    Path to the input "         \
  "image.\n\nOptions:\n    -a, --accelerate\n            Execute image "       \
  "processing kernels on the GPU.\n            If omitted, processing is "     \
  "performed on the CPU.\n\n    -h, --help\n            Display this help "    \
  "message and exit.\n\nSupported Image Formats:\n    .bmp\n    .jpeg\n    "   \
  ".jpg\n    .png\n    .webp\n\nExamples:\n    pixcu++ image.png\n           " \
  " Process image.png using the CPU.\n\n    pixcu++ --accelerate image.png\n " \
  "           Process image.png using GPU acceleration.\n\n    pixcu++ -a "    \
  "~/Pictures/photo.webp\n            Process photo.webp on the GPU.\n"
std::pmr::unordered_set<std::string> SUPPORTED_EXT{".bmp", ".png", ".jpg",
                                                   ".jpeg", ".webp"};

void HELP() {
  std::cout << DBG_STR << std::endl;
  exit(0);
}

CLI::CLI() : dbg(false), accelerate(false), fpath("") {}

void CLI::parse_args(std::vector<std::string> plist) {
  for (std::string &text : plist) {
    std::transform(text.begin(), text.end(), text.begin(),
                   [](unsigned char c) { return std::tolower(c); });
  }

  for (ssize_t i = 0; i < plist.size(); i++) {
    auto pos = plist[i].rfind('.');
    if (plist[i].compare("-h") || plist[i].compare("--help")) {
      HELP();
    } else if (plist[i].compare("-d") || plist[i].compare("--debug")) {
      this->dbg = true;
    } else if (plist[i].compare("-a") || plist[i].compare("--accel")) {
      this->accelerate = true;
    } else if (pos != std::string::npos && SUPPORTED_EXT.find(plist[i].substr(
                                               pos)) != SUPPORTED_EXT.end()) {
      this->fpath = plist[i];
    } else {
      this->print();
      HELP();
    }
  }
  this->print();
  if (this->fpath.empty()) {
    std::cerr << "Please provide file to process and work on!" << std::endl;
  }
}

void CLI::print() const {
  std::cout << "CLI: {" << std::endl
            << "Debug: " << (this->dbg ? "True" : "False") << std::endl
            << "Accelerate: " << (this->accelerate ? "True" : "False")
            << std::endl
            << "Fpath: " << this->fpath << std::endl;
}
