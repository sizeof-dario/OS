.PHONY: run clean disasm
CROSS = $(HOME)/opt/cross/bin

kernel.bin: boot.o kmain.o VGAdriver.o int.o link.ld
	$(CROSS)/i686-elf-ld -T link.ld boot.o VGAdriver.o int.o kmain.o -o kernel.bin -Map=kernel.map

kmain.o: kmain.c
	$(CROSS)/i686-elf-gcc -ffreestanding -O0 -c kmain.c -o kmain.o

VGAdriver.o: VGAdriver.c
	$(CROSS)/i686-elf-gcc -ffreestanding -O0 -c VGAdriver.c -o VGAdriver.o

boot.o: boot.asm
	nasm -f elf32 boot.asm -o boot.o

int.o: int.c
	$(CROSS)/i686-elf-gcc -ffreestanding -O0 -mgeneral-regs-only -c int.c -o int.o

run: kernel.bin
	qemu-system-i386 -drive file=kernel.bin,format=raw,index=0,media=disk -monitor stdio

clean:
	rm -f boot.o VGAdriver.o kmain.o int.o kernel.bin kernel.map

disasm: kernel.bin
	$(CROSS)/i686-elf-objdump -D -b binary -m i386 kernel.bin