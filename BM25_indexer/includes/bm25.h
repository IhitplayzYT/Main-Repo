#ifndef bm25
#define bm25
#include <algorithm>
#include <iostream>
#include <string>
#include <cstring>
#include <utility>
#include <vector>
#include <unordered_map>
#include <cmath>
#include <unordered_set>
#endif

#define K1 1.2
#define B 0.7

typedef std::pair<std::string ,std::vector<std::string>> Doc;
typedef std::vector<Doc> Corpus;

//  Corpus
//     |_ Doc1
//     |_ Doc2 
//     |_ Doc3

double avg_doc_len(Corpus &);
std::pmr::unordered_map<std::string,int> TF(Doc &);
std::pmr::unordered_map<std::string,int> DF(Corpus &);
double IDF(int,int);
double BM25_score(Doc &,std::pmr::unordered_map<std::string,int>&,double,int,std::vector<std::string>);
void show_n(int);
std::vector<int> rank_corpus(Corpus &corp,std::vector<std::string> &query);
void show_corpus();