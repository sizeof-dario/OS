#include "VGAdriver.h"

#define EXAMPLETEXT "\nUnnamedOS (version 0.0.0)\n"

void kmain(void)
{
    write(EXAMPLETEXT, VGA_COLOR_WHITE);

    while(1)
    {
        asm volatile ("hlt");
    }
}
