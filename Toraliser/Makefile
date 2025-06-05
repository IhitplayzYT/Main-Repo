flags=-O2 -Wall -g -Wextra -pedantic -std=c2x
ldflags=-lm -lncurses

.PHONY: all clean run

all: clean Toraliser run

Toraliser: Toraliser.o
	gcc $(flags) $^ -o $@ $(ldflags)

Toraliser.o: Toraliser.c Toraliser.h
	gcc $(flags) -c $<

run:Toraliser
	./Toraliser

clean:
	rm -rf *.o Toraliser