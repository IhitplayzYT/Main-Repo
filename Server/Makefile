flags=-O2 -Wall -g -Wextra -pedantic -std=c2x
ldflags=-lm -lncurses

.PHONY: all clean run 

all: clean Server 

cl:
	clear

run:Server
	./$<

Server: Server.o
	gcc $(flags) $^ -o $@ $(ldflags)

Server.o: Server.c Server.h
	gcc $(flags) -c $<

clean:
	rm -rf *.o Server

