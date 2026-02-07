#ifndef UTILITY
#define UTILITY
#include <iostream>
#include <string>
#include <algorithm>
#include <cstring>
#include <unordered_set>
#include <string>
#include <filesystem>

const std::unordered_set<std::string> allowed_extensions = {
    // Core plain text
    ".txt", ".text", ".log", ".out", ".err", ".lst", ".msg",
    ".note", ".notes", ".todo", ".readme", ".license", ".authors",
    ".copying", ".changelog", ".manifest",
    // C / C++ / systems
    ".c", ".h", ".i", ".ii", ".cpp", ".cc", ".cxx",
    ".hxx", ".hpp", ".inl", ".def", ".inc",
    // Rust
    ".rs", ".toml", ".lock",
    // Go
    ".go", ".mod", ".sum", ".work",
    // Python
    ".py", ".pyi", ".pyw", ".pxd", ".pxi",
    // JVM languages
    ".java", ".kt", ".kts", ".scala", ".groovy", ".clj", ".edn",
    // JavaScript / Web
    ".js", ".mjs", ".cjs", ".ts", ".tsx", ".jsx",
    ".json", ".jsonc",
    // Shell / scripting
    ".sh", ".bash", ".zsh", ".fish", ".ps1", ".cmd", ".bat",
    // Markup / docs
    ".md", ".markdown", ".rst", ".adoc", ".asciidoc",
    ".tex", ".latex",
    ".html", ".htm", ".xhtml",
    ".xml", ".xsd", ".dtd", ".svg",
    // Config / data
    ".ini", ".cfg", ".conf", ".cnf", ".env",
    ".properties",
    ".yaml", ".yml",
    ".hjson",
    // Build / tooling
    ".make", ".mk", ".makefile",
    ".cmake", ".cmakelists.txt",
    ".gradle", ".bzl", ".ninja", ".buck",
    // Version control / patches
    ".gitignore", ".gitattributes", ".gitmodules",
    ".hgignore", ".svnignore",
    ".patch", ".diff",
    // System / OS
    ".service", ".socket", ".mount", ".timer",
    ".target", ".desktop", ".rules", ".udev", ".pam",
    // Data / datasets
    ".csv", ".tsv", ".psv", ".ssv",
    ".dat", ".data",
    ".arff", ".libsvm", ".vw",
    // Documentation formats
    ".man", ".pod", ".help", ".info", ".nfo",
    // Security / crypto (text-encoded)
    ".pem", ".crt", ".csr", ".key", ".pub", ".asc", ".sig",
    // Email / messaging
    ".eml", ".mail", ".mbox",
    // Media-related text
    ".srt", ".vtt", ".sub",
    // Localization
    ".po", ".pot",
    // Graphics / shaders (text)
    ".shader", ".vert", ".frag", ".glsl", ".hlsl",
    // Misc
    ".sql", ".rtf"
};



#endif
typedef signed char s8;
typedef signed short s16;
typedef signed int s32;
typedef signed long long s64;
typedef unsigned char i8;
typedef unsigned short i16;
typedef unsigned int i32;
typedef unsigned long long i64;

void usage(std::string);
bool is_vowel(char &c);
bool ends_with(const std::string &,const std::string &);
int measure(const std::string &);
int r1(std::string &);
int r2(std::string &,int);
bool double_consonant(const std::string &);
bool cvc(const std::string &);
bool contains_vowel(const std::string &);
std::pair<std::string,char> get_input(int argc,char ** argv);
std::string preprocess(std::string&);
bool is_alphabetic(char c);
bool is_alphanumeric(char c);
bool is_numeric(char c); 