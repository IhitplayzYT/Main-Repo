flags=-O2 -Wall -g -Wextra -pedantic -std=c2x

.PHONY: all clean cl run

all:cl clean shared Hotreload.o  main_rl 

cl:
	clear

run:main
	./$<


shared: shared_varibs.c
	gcc -fPIC -shared $^ -o shared.so

Hotreload.o: Hotreload.c Hotreload.h
	gcc $(flags) -c $<

main_rl: main.c Hotreload.o 
	gcc $(flags) $< -ldl -o main 

clean:
	rm -rf *.o main *.so

