#include "bm25.h"
#include <cmath>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

double avg_doc_len(Corpus c){
if (!c.size()){
    return 0.0;
}
double ret = 0.0;
for (Doc x : c) {
ret += x.size();
}
return (ret / c.size());
}


std::pmr::unordered_map<std::string,int> DF(Corpus c) {
std::pmr::unordered_map<std::string,int> ret;
for (Doc doc: c ) {
    std::unordered_set<std::string> seen;
    for (std::string s : doc) 
        seen.insert(s);
    for (std::string y : seen) 
        ret[y]+=1;

}
return ret;
}

std::pmr::unordered_map<std::string,int> TF(Doc c) {
std::pmr::unordered_map<std::string,int> ret;
for (auto &tok : c) {
ret[tok] += 1;
}
return ret;
}

double IDF(int N,int df){
    return std::log(1.0 + (N - df + 0.5)/(df + 0.5));
}


double BM25_score(Doc doc,std::pmr::unordered_map<std::string,int> df,double avg_doc,int N,std::vector<std::string> query) {
double ret(0.0),l(doc.size());
auto tf = TF(doc);

for (auto &word : query){
auto iter = tf.find(word);
if (iter == tf.end()) continue;
int dft = df[word],freq = iter->second;
ret += ((IDF(N,dft)) * (freq * (K1 + 1))) / (freq + K1 *(1 - B - (B * l/avg_doc) ));
}
return ret;
}





