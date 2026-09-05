#include "vga_driver.h"
#include "int.h"

#define EXAMPLETEXT "\nBifrost (version 0.0.0)\n"

extern struct interrupt_descriptor IDT[IDT_LENGTH];

__attribute__((noreturn)) void kmain(void)
{
	enable_protected_interrupts();

	write(EXAMPLETEXT, VGA_COLOR_WHITE);

	while(1) {
        asm volatile ("hlt");
	}
}
