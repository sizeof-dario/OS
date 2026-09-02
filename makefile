.PHONY: run clean clean_obj disasm

CC = $(HOME)/opt/cross/bin/i686-elf-gcc
LD = $(HOME)/opt/cross/bin/i686-elf-ld
AS = nasm
DA = $(HOME)/opt/cross/bin/i686-elf-objdump
QM = qemu-system-i386

C_SOURCES = $(wildcard src/*.c)
OBJECTS = build/boot.o $(patsubst src/%.c,build/%.o,$(C_SOURCES))

build/int.o: EXTRA_FLAGS = -mgeneral-regs-only



build/kernel.bin: $(OBJECTS) link.ld
	$(LD) -T link.ld $(OBJECTS) -o $@ -Map=build/kernel.map && $(MAKE) clean_obj

build/boot.o: src/boot.asm
	$(AS) -f elf32 $< -o $@

build/%.o:src/%.c
	$(CC) -ffreestanding -O0 -Iinclude $(EXTRA_FLAGS) -c $< -o $@



run: build/kernel.bin
	$(QM) -drive file=$<,format=raw,index=0,media=disk -monitor stdio

clean_obj:
	rm -f $(OBJECTS)

clean:
	rm -f build/kernel.bin build/kernel.map

disasm: build/kernel.bin
	$(DA) -D -b binary -m i386 $<