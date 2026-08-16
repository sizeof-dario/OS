#include <stdint.h>

typedef struct __attribute__((packed))
{
    uint8_t c;  // character
    uint8_t a;  // attributes
} VGAchar_t;

// Starting address of VGA video memory in RAM
static VGAchar_t *const VGAMEM = (VGAchar_t *)0xB8000;

// Puts a character on screen
void putc(uint8_t row, uint8_t col, uint8_t c)
{
    VGAMEM[80*row + col].c = c;
    VGAMEM[80*row + col].a = 0x7;
}

// Puts a string on screen
void puts(uint8_t row, uint8_t col, const uint8_t* str)
{
    for (uint32_t i = 0; str[i]; i++)
    {
        putc(row, col + i, str[i]);
    }    
}

void kmain(void)
{
    puts(13, 3, "Hello, World!");

    while(1)
    {
        asm volatile ("hlt");
    }
}
