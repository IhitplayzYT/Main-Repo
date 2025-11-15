flags=-O2 -Wall -g -Wextra -pedantic -std=c2x 
ldflags=-lm -lncurses 

.PHONY: all clean cl run

all:cl clean Embedder 

cl:
	clear

run:Embedder
	./$<


Embedder: Embedder.o
	gcc $(flags) $^ -o $@ $(ldflags)

Embedder.o: Embedder.c Embedder.h
	gcc $(flags) -c $<

clean:
	rm -rf *.o Embedder

