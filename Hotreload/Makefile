flags=-O2 -Wall -g -Wextra -pedantic -std=c2x
ldflags=-lm -lncurses

.PHONY: all clean cl run

all:cl clean Hotreload 

cl:
	clear

run:Hotreload
	./$<


Hotreload: Hotreload.o
	gcc $(flags) $^ -o $@ $(ldflags)

Hotreload.o: Hotreload.c Hotreload.h
	gcc $(flags) -c $<

clean:
	rm -rf *.o Hotreload

