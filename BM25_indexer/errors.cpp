#include "errors.h"

extern Debug GLOBAL_DEBUG_STRUCT;

void debug_show() {
std::cout << "====DEBUG====" << std::endl <<
"Stemmer : " << GLOBAL_DEBUG_STRUCT.t_stemmer << std::endl <<
"Stemmer Progress : " << ((GLOBAL_DEBUG_STRUCT.stem_prog) ? "COMPLETED":"NOT STARTED") << std::endl <<
"Stopword : " << GLOBAL_DEBUG_STRUCT.t_stopword << std::endl <<
"Stopword Progress : " << ((GLOBAL_DEBUG_STRUCT.stop_prog) ? "COMPLETED":"NOT STARTED") << std::endl;
}

