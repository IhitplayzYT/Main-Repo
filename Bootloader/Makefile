exec := bootloader
obj :=
ldflags := -T bootloader.ld -m elf_i386
flags := -f elf 
.PHONY: clean 

all:clean bootloader

bootloader: bootloader.elf
	objcopy -I elf32-i386 -O binary $^ bootloader.bin
	truncate -s %512 bootloader.bin
	mv bootloader.bin $@

bootloader.elf: bootloader.o
	ld $(ldflags) $^ -o $@
	chmod 755 bootloader.elf


bootloader.o: bootloader.asm
	nasm $(flags) $^ -o $@


clean:
	rm -rf *.o *.elf *.bin bootloader


