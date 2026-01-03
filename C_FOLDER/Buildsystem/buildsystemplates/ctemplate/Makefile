flags=-O2 -Wall -g -Wextra -pedantic -std=c2x
ldflags=-lm -lncurses

.PHONY: all clean cl run

all:cl clean PROJ_NAME 

cl:
	clear

run:PROJ_NAME
	./$<


PROJ_NAME: PROJ_NAME.o
	gcc $(flags) $^ -o $@ $(ldflags)

PROJ_NAME.o: PROJ_NAME.c PROJ_NAME.h
	gcc $(flags) -c $<

clean:
	rm -rf *.o PROJ_NAME

