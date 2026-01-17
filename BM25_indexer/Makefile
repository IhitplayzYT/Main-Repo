COMPILER := g++
flags:= -Wall -Wextra -Wpedantic -std=c++20 -g -O0
objs:=main.o Stemmer.o stopwords.o utility.o bm25.o
all : clean main

.PHONY:all clean

main: $(objs)
	$(COMPILER) $^ -o $@

main.o:main.h errors.h Stemmer.h stopwords.h utility.h bm25.h
	$(COMPILER) $(flags) -c main.cpp

Stemmer.o:Stemmer.h utility.h
	$(COMPILER) $(flags) -c Stemmer.cpp

stopwords.o:stopwords.h
	$(COMPILER) $(flags) -c stopwords.cpp

utility.o:utility.h
	$(COMPILER) $(flags) -c utility.cpp

bm25.o : bm25.h
	$(COMPILER) $(flags) -c bm25.cpp

clean:
	rm -rf *.o main
