#ifndef bm25
#define bm25
#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <unordered_map>
#include <cmath>
#include <unordered_set>
#endif

#define K1 1.2
#define B 0.7

typedef std::vector<std::string> Doc;
typedef std::vector<Doc> Corpus;

//      Corpus
//         |_ Doc1
//         |_ Doc2 
//         |_ Doc3

double avg_doc_len(Corpus);
