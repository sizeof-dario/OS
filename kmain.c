#include <stdint.h>

typedef struct __attribute__((packed))
{
    char c;  // character
    uint8_t a;  // attributes
} VGAchar_t;

// Starting address of VGA video memory in RAM
static VGAchar_t *const VGAMEM = (VGAchar_t *)0xB8000;

// Puts a character on screen
void putc(uint8_t row, uint8_t col, char c)
{
    VGAMEM[80*row + col].c = c;
    VGAMEM[80*row + col].a = 0x7;
}

// Puts a string on screen
void puts(uint8_t row, uint8_t col, const char *str)
{
    for (uint32_t i = 0; str[i]; i++)
    {
        putc(row, col + i, str[i]);
    }    
}

static uint8_t cursor_r = 0;
static uint8_t cursor_c = 0;

void write(const char *str)
{
    uint8_t new_cursor_r = cursor_r;
    uint8_t new_cursor_c = cursor_c;

    for (int32_t i = 0; str[i]; i++)
    {
        if(str[i] == '\r')
        {
            new_cursor_c = 0;
        }
        else if (str[i] == '\n')
        {
            new_cursor_c = 0;
            new_cursor_r++;
        }
        else
        {
            putc(cursor_r, cursor_c, str[i]);
            new_cursor_c++;
            if(new_cursor_c == 80)
            {
                new_cursor_c = 0;
                new_cursor_r++;
            }
        }
        
        if(new_cursor_c < 80 && new_cursor_r < 25)
        {
            cursor_c = new_cursor_c;
            cursor_r = new_cursor_r;
            continue;
        }

        if(new_cursor_r < 25)
        {
            cursor_c = 0;
            cursor_r = new_cursor_r;
            continue;
        }

        cursor_c = 0;

        for (uint16_t j = 0; j < 24*80; j++)
        {
            VGAMEM[j].c = VGAMEM[j + 80].c;
            VGAMEM[j].a = VGAMEM[j + 80].a;
        }
        for (uint16_t k = 24*80; k < 25*80; k++)
        {
            VGAMEM[k].c = ' ';
            VGAMEM[k].a = 0x7;
        }
    }
}

void kmain(void)
{
    write("Hello, World!");

    while(1)
    {
        asm volatile ("hlt");
    }
}
