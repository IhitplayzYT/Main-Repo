#include "includes/stopwords.h"
using std::unordered_set,std::string;

const unordered_set<string> stopword_set_non_aggr = {
    "a","an","the",
    "i","me","my","we","our","you","your","he","him","his",
    "she","her","it","its","they","them","their",
    "this","that","these","those",
    "is","are","was","were","be","been","being",
    "have","has","had","do","does","did",
    "of","to","in","for","with","on","at","by","from",
    "and","or","but","if","because","as","until","while",
    "not","no","nor",
    "all","any","both","each","few","more","most","some",
    "such","only","own","same","so","than","too","very",
    "can","will","just","should","now"
};

const unordered_set<string> stopword_set_aggr = {
    // Articles
    "a", "an", "the",
    // Pronouns
    "i", "me", "my", "myself", "we", "our", "ours", "ourselves", "you", "your",
    "yours", "yourself", "yourselves", "he", "him", "his", "himself", "she",
    "her", "hers", "herself", "it", "its", "itself", "they", "them", "their",
    "theirs", "themselves", "what", "which", "who", "whom", "this", "that",
    "these", "those",
    // Auxiliary verbs
    "am", "is", "are", "was", "were", "be", "been", "being", "have", "has",
    "had", "having", "do", "does", "did", "doing", "would", "should", "could",
    "ought", "can", "may", "might", "must", "shall", "will",
    // Prepositions
    "of", "at", "by", "for", "with", "about", "against", "between", "into",
    "through", "during", "before", "after", "above", "below", "to", "from",
    "up", "down", "in", "out", "on", "off", "over", "under", "again", "further",
    "then", "once", "here", "there", "when", "where", "why", "how", "all",
    "both", "each", "few", "more", "most", "other", "some", "such", "than",
    "too", "very",
    // Conjunctions
    "and", "but", "if", "or", "because", "as", "until", "while", "nor", "so",
    "yet", "though", "although", "since", "unless", "whereas", "whether",
    // Negations
    "no", "not", "nor", "none", "never", "neither", "nobody", "nothing",
    "nowhere", "hardly", "scarcely",
    // Common verbs
    "get", "got", "gets", "getting", "give", "gave", "given", "giving", "go",
    "goes", "going", "gone", "went", "make", "makes", "made", "making", "put",
    "puts", "putting", "take", "takes", "took", "taken", "taking", "come",
    "comes", "came", "coming", "see", "sees", "saw", "seen", "seeing", "know",
    "knows", "knew", "known", "knowing", "think", "thinks", "thought",
    "thinking", "say", "says", "said", "saying", "tell", "tells", "told",
    "telling", "become", "becomes", "became", "becoming", "leave", "leaves",
    "left", "leaving", "find", "finds", "found", "finding", "seem", "seems",
    "seemed", "seeming", "ask", "asks", "asked", "asking", "work", "works",
    "worked", "working", "feel", "feels", "felt", "feeling", "try", "tries",
    "tried", "trying", "use", "uses", "used", "using", "keep", "keeps", "kept",
    "keeping", "let", "lets", "letting", "begin", "begins", "began", "begun",
    "beginning", "show", "shows", "showed", "shown", "showing", "hear", "hears",
    "heard", "hearing", "play", "plays", "played", "playing", "run", "runs",
    "ran", "running", "move", "moves", "moved", "moving", "like", "likes",
    "liked", "liking", "live", "lives", "lived", "living", "believe",
    "believes", "believed", "believing", "hold", "holds", "held", "holding",
    "bring", "brings", "brought", "bringing", "happen", "happens", "happened",
    "happening", "write", "writes", "wrote", "written", "writing", "sit",
    "sits", "sat", "sitting", "stand", "stands", "stood", "standing", "lose",
    "loses", "lost", "losing", "pay", "pays", "paid", "paying", "meet", "meets",
    "met", "meeting", "include", "includes", "included", "including",
    "continue", "continues", "continued", "continuing", "set", "sets",
    "setting", "learn", "learns", "learned", "learning", "change", "changes",
    "changed", "changing", "lead", "leads", "led", "leading", "understand",
    "understands", "understood", "understanding", "watch", "watches", "watched",
    "watching", "follow", "follows", "followed", "following", "stop", "stops",
    "stopped", "stopping", "create", "creates", "created", "creating", "speak",
    "speaks", "spoke", "spoken", "speaking", "read", "reads", "reading",
    "allow", "allows", "allowed", "allowing", "add", "adds", "added", "adding",
    "spend", "spends", "spent", "spending", "grow", "grows", "grew", "grown",
    "growing", "open", "opens", "opened", "opening", "walk", "walks", "walked",
    "walking", "win", "wins", "won", "winning", "offer", "offers", "offered",
    "offering", "remember", "remembers", "remembered", "remembering", "love",
    "loves", "loved", "loving", "consider", "considers", "considered",
    "considering", "appear", "appears", "appeared", "appearing", "buy", "buys",
    "bought", "buying", "wait", "waits", "waited", "waiting", "serve", "serves",
    "served", "serving", "die", "dies", "died", "dying", "send", "sends",
    "sent", "sending", "expect", "expects", "expected", "expecting", "build",
    "builds", "built", "building", "stay", "stays", "stayed", "staying", "fall",
    "falls", "fell", "fallen", "falling", "cut", "cuts", "cutting", "reach",
    "reaches", "reached", "reaching", "kill", "kills", "killed", "killing",
    "remain", "remains", "remained", "remaining", "suggest", "suggests",
    "suggested", "suggesting", "raise", "raises", "raised", "raising", "pass",
    "passes", "passed", "passing", "sell", "sells", "sold", "selling",
    "require", "requires", "required", "requiring", "report", "reports",
    "reported", "reporting", "decide", "decides", "decided", "deciding", "pull",
    "pulls", "pulled", "pulling",
    // Quantifiers and determiners
    "any", "many", "much", "several", "enough", "little", "less", "least",
    "own", "same", "only", "just", "even", "ever", "also", "either", "neither",
    // Time-related
    "now", "today", "yesterday", "tomorrow", "tonight", "always", "often",
    "sometimes", "usually", "seldom", "rarely", "ever", "never", "already",
    "still", "yet",
    // Contractions (expanded)
    "ain", "aren", "couldn", "didn", "doesn", "hadn", "hasn", "haven", "isn",
    "mightn", "mustn", "needn", "shan", "shouldn", "wasn", "weren", "won",
    "wouldn", "don", "won't", "can't", "couldn't", "didn't", "doesn't",
    "hadn't", "hasn't", "haven't", "isn't", "mightn't", "mustn't", "needn't",
    "shan't", "shouldn't", "wasn't", "weren't", "wouldn't", "don't", "aren't",
    "i'm", "i've", "i'll", "i'd", "you're", "you've", "you'll", "you'd", "he's",
    "he'll", "he'd", "she's", "she'll", "she'd", "it's", "it'll", "we're",
    "we've", "we'll", "we'd", "they're", "they've", "they'll", "they'd",
    "that's", "that'll", "who's", "who'll", "what's", "what'll", "where's",
    "where'll", "when's", "when'll", "why's", "why'll", "how's", "how'll",
    // Miscellaneous common words
    "yes", "okay", "ok", "um", "hmm", "ah", "oh", "well", "please", "thanks",
    "thank", "hello", "hi", "hey", "bye", "goodbye", "sorry",
    // Additional common words
    "became", "become", "becomes", "becoming", "via", "re", "per", "onto",
    "towards", "upon", "within", "without", "throughout", "hereby", "therein",
    "thereof", "thereon", "thereto", "therefore", "thus", "hence", "however",
    "moreover", "furthermore", "nevertheless", "nonetheless", "meanwhile",
    "otherwise", "instead", "besides", "aside", "apart", "across", "along",
    "among", "amongst", "around", "beyond", "behind", "beside", "besides",
    "concerning", "considering", "despite", "except", "following", "inside",
    "near", "outside", "regarding", "toward", "underneath", "unlike", "onto"};


std::vector<std::string> filter_stopwords(std::string &str,bool b){
std::istringstream s(str);
std::vector<std::string> ret;
if (b){
for (std::string x ; s >> x;) {
if (stopword_set_aggr.find(x) == stopword_set_aggr.end())
    ret.push_back(x);
}
}else{
for (std::string x ; s >> x;) {
if (stopword_set_non_aggr.find(x) == stopword_set_non_aggr.end())
    ret.push_back(x);
}
}
return ret;
}

std::vector<std::string> tokenize_ws(const std::string& s)
{
    std::istringstream iss(s);
    std::vector<std::string> tokens;

    for (std::string word; iss >> word; )
        tokens.push_back(word);

    return tokens;
}
