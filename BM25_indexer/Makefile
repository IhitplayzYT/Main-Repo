flags:= -Wall -Wextra -Wpedantic -std=c++20
objs:=main.o Stemmer.o stopwords.o utility.o
all : main

.PHONY:all clean

main: $(objs)
	g++ $^ -o $@

main.o:main.h errors.h Stemmer.h stopwords.h utility.h
	g++ $(flags) -c main.cpp

Stemmer.o:Stemmer.h utility.h
	g++ $(flags) -c Stemmer.cpp

stopwords.o:stopwords.h
	g++ $(flags) -c stopwords.cpp

utility.o:utility.h
	g++ $(flags) -c utility.cpp

clean:
	rm -rf *.o main
