#include "includes/bm25.h"
#include <algorithm>
#include <cmath>
#include <memory_resource>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

extern Corpus CORPUS;

double avg_doc_len(Corpus& c){
if (!c.size()){
    return 0.0;
}
double ret = 0.0;
for (Doc &x : c) {
ret += x.second.size();
}
return (ret / c.size());
}


std::pmr::unordered_map<std::string,int> DF(Corpus &c) {
std::pmr::unordered_map<std::string,int> ret(std::pmr::get_default_resource());
for (Doc& doc: c ) {
std::pmr::unordered_set<std::string> seen(std::pmr::get_default_resource());
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

std::unordered_map<std::string,int> TF_query(const std::vector<std::string>& q) {
    std::unordered_map<std::string,int> ret;
    for (const auto& w : q)
        ret[w]++;
    return ret;
}

double IDF(int N,int df){
    return std::log(1.0 + (N - df + 0.5)/(df + 0.5));
}

double BM25_score(Doc& doc,std::pmr::unordered_map<std::string,int>& df,double avg_doc,int N,std::vector<std::string> query) {
if (doc.second.empty() || !avg_doc || query.empty()) {return 0.0;}
double ret(0.0),l(doc.second.size());
auto tf = TF(doc);
auto qtf = TF_query(query);
for (auto &[word,qfreq] : qtf){
auto tf_it = tf.find(word);
auto df_it = df.find(word);
if (tf_it == tf.end() || df_it == df.end()) {continue;}
auto freq = tf_it->second;
auto dft = df_it->second;
double termScore = (IDF(N,dft) * (freq * (K1 + 1.0))) / (freq + K1 * (1.0 - B + (B * l/avg_doc)));
    ret += qfreq * termScore;   
}
return ret;
}

std::vector<int> rank_corpus(Corpus &corp,std::vector<std::string> &query){
std::vector<int> ret;
if (query.size() == 0) return ret;
int N = corp.size();
double avgdl = avg_doc_len(corp);
// No issue spotted till here

auto map = DF(corp);
std::vector<std::pair<double,int>> scored;
for (int i = 0 ; i < N ; i++) {
Doc &doc = corp[i];
double score = BM25_score(doc,map,avgdl,N,query);
std::cout << score << std::endl;
scored.emplace_back(score,i);
}
std::sort(scored.begin(),scored.end(),[](auto &a,auto &b){return a.first > b.first;});
for (int i = 0 ; i < (int)scored.size();i++){
ret.push_back(scored[i].second);
}
return ret;
}

void show_corpus(){
for (Doc & doc : CORPUS) {
std::cout << doc.first << ": " << std::endl;
for (std::string &word: doc.second) std::cout <<  word << " ";
std::cout << std::endl;
}
}


