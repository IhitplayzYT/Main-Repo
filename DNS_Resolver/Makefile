flags=-O2 -Wall -g -Wextra -pedantic -std=c2x
ldflags=-lstdoslib

.PHONY: all clean cl run

all:cl clean DNS_Resolver 

cl:
	clear

run:DNS_Resolver
	./$<


DNS_Resolver: DNS_Resolver.o
	gcc $(flags) $^ -o $@ $(ldflags)

DNS_Resolver.o: DNS_Resolver.c DNS_Resolver.h
	gcc $(flags)  -c $<

clean:
	rm -rf *.o DNS_Resolver

