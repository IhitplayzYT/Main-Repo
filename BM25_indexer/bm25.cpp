#include "bm25.h"
#include <cmath>
#include <memory_resource>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

double avg_doc_len(Corpus& c){
if (!c.size()){
    return 0.0;
}
double ret = 0.0;
for (Doc x : c) {
ret += x.second.size();
}
return (ret / c.size());
}


std::pmr::unordered_map<std::string,int> DF(Corpus &c) {
std::pmr::unordered_map<std::string,int> ret(std::pmr::get_default_resource());
for (Doc& doc: c ) {
    std::unordered_set<std::string> seen;
    for (std::string s : doc.second) 
        seen.insert(s);
    for (std::string y : seen) 
        ret[y]+=1;
}
return ret;
}

std::pmr::unordered_map<std::string,int> TF(Doc &c) {
std::pmr::unordered_map<std::string,int> ret(std::pmr::get_default_resource());
for (auto &tok : c.second) {
ret[tok] += 1;
}
return ret;
}

double IDF(int N,int df){
    return std::log(1.0 + (N - df + 0.5)/(df + 0.5));
}


double BM25_score(Doc& doc,std::pmr::unordered_map<std::string,int> df,double avg_doc,int N,std::vector<std::string> query) {
if (!doc.second.empty() || !avg_doc || !query.empty()) {return 0.0;}
double ret(0.0),l(doc.second.size());
auto tf = TF(doc);
std::unordered_set<std::string> uniq_query(query.begin(), query.end());
for (auto &word : uniq_query){
auto tf_it = tf.find(word);
auto df_it = df.find(word);
if (tf_it == tf.end() || df_it == df.end()) {continue;}
auto freq = tf_it->second;
auto dft = df_it->second;
ret += ((IDF(N,dft)) * (freq * (K1 + 1.0))) / (freq + K1 * (1.0 - B + (B * l/avg_doc) ));
}
return ret;
}








