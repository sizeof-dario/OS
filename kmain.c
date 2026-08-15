#include <stdint.h>

static uint32_t sentinel = 0xDEADC0DE;

void kmain(void)
{
    uint32_t stackvalue = 0xDEADBEEF;
    while(1)
    {
        asm volatile ("hlt");
    }
}
