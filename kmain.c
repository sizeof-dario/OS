#include "VGAdriver.h"
#include "int.h"

#define EXAMPLETEXT "\nUnnamedOS (version 0.0.0)\n"

extern struct interrupt_descriptor IDT[IDT_LENGTH];

void kmain(void)
{
	write(EXAMPLETEXT, VGA_COLOR_WHITE);

	while(1) {
        asm volatile ("hlt");
	}
}
