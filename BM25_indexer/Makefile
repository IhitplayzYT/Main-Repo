COMPILER := g++
flags:= -Wall -Wextra -Wpedantic -std=c++20 -g -O0 -I/includes
objs:=main.o Stemmer.o stopwords.o utility.o bm25.o fzf_tui.o

all : clean main sudo_cl

.PHONY:all clean sudo_cl

main: $(objs)
	$(COMPILER) $(flags) $^ -o $@

main.o:includes/main.h includes/errors.h includes/Stemmer.h includes/stopwords.h includes/utility.h includes/bm25.h
	$(COMPILER) $(flags) -c main.cpp

Stemmer.o:includes/Stemmer.h includes/utility.h
	$(COMPILER) $(flags) -c Stemmer.cpp

stopwords.o:includes/stopwords.h
	$(COMPILER) $(flags) -c stopwords.cpp

utility.o:includes/utility.h
	$(COMPILER) $(flags) -c utility.cpp

bm25.o : includes/bm25.h
	$(COMPILER) $(flags) -c bm25.cpp

fzf_tui.o: includes/fzf_tui.h
	$(COMPILER) $(flags) -c fzf_tui.cpp

sudo_cl:
	rm -rf *.o

clean:
	rm -rf *.o main
