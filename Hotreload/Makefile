flags=-O2 -Wall -g -Wextra -pedantic -std=c2x
ldflags=

.PHONY: all clean cl run

all:cl clean shared.so Hotreload 

cl:
	clear

run:Hotreload
	./$<

shared.so : shared.c
	gcc -fPIC -shared -ldl shared.c -o shared.so

Hotreload: Hotreload.o shared.so
	gcc $(flags) $^ -o $@ $(ldflags)

Hotreload.o: Hotreload.c Hotreload.h
	gcc $(flags) -c $<

clean:
	rm -rf *.o Hotreload

