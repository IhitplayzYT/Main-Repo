.PHONY : all clean run   

all:  first 

run: first
	./first

clean:
	rm -rf *.o first

first.o: first.asm
	nasm -f elf32 first.asm -o first.o

first: first.o
	ld -m elf_i386 first.o -o first