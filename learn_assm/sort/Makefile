.PHONY : all clean run

all : sort

run : sort
	./sort

clean:
	rm -rf *.o sort

sort: sort.o 
	ld -m elf_i386 sort.o -o sort

sort.o : sort.asm
	nasm -f elf32 sort.asm -o sort.o




