flags=-O2 -Wall -g -Wextra -pedantic -std=c2x
ldflags=-lm -lncurses -lexif

.PHONY: all clean cl run

all:cl clean File_Modifier 

cl:
	clear

run:File_Modifier
	./$<


File_Modifier: File_Modifier.o
	gcc $(flags) $^ -o $@ $(ldflags)

File_Modifier.o: File_Modifier.c File_Modifier.h
	gcc $(flags) -c $<

clean:
	rm -rf *.o File_Modifier

