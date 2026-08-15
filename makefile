.PHONY: run clean disasm
CROSS = $(HOME)/opt/cross/bin

kernel.bin: boot.o kmain.o link.ld
	$(CROSS)/i686-elf-ld -T link.ld boot.o kmain.o -o kernel.bin -Map=kernel.map

kmain.o: kmain.c
	$(CROSS)/i686-elf-gcc -ffreestanding -O0 -c kmain.c -o kmain.o

boot.o: boot.asm
	nasm -f elf32 boot.asm -o boot.o

run: kernel.bin
	qemu-system-i386 -fda kernel.bin -monitor stdio

clean:
	rm -f boot.o kmain.o kernel.bin kernel.map

disasm: kernel.bin
	$(CROSS)/i686-elf-objdump -D -b binary -m i386 kernel.bin