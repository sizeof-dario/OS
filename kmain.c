#include "VGAdriver.h"

void kmain(void)
{
    write("Hello, World!");

    while(1)
    {
        asm volatile ("hlt");
    }
}
