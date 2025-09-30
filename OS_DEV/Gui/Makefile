flags = -fno-stack-protector -Wall -O0 -std=gnu23 -m16 -ffreestanding -I include -fno-pic -fno-PIE
ldflags = --nmagic --script=gui.ld --oformat binary
objects = main.o shapes.o
xobjects = xgfx.o
NASMENV = -i include/asm/
export NASMENV

.PHONY: clean

all: gui.com

gui.com: $(objects) $(xobjects)
	ld -m elf_i386 $(ldflags) $^ -o $@

main.o: main.c
	gcc $(flags) -c $^

shapes.o: shapes.c
	gcc $(flags) -c $^

xgfx.o: xgfx.asm
	nasm -f elf $^ -o $@
	
clean:
	rm -f *.o *.com