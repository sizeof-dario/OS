#include "VGAdriver.h"

void putc(char ch, uint8_t x, uint8_t y)
{
    VGAMEM[x + SCREEN_WIDTH * y].ch = ch;
    VGAMEM[x + SCREEN_WIDTH * y].attr = 0x7;
}

uint16_t getcursorloc(void)
{
    uint16_t pos = 0;
    outb(CRTCR_ADDR, CRTCR_CL);
    pos |= inb(CRTCR_DATA);
    outb(CRTCR_ADDR, CRTCR_CH);
    pos |= ((uint16_t)inb(CRTCR_DATA)) << 8;
    return pos;
}

void setcursorloc(uint16_t loc)
{
    outb(CRTCR_ADDR, CRTCR_CL);
	outb(CRTCR_DATA, (uint8_t)(loc & 0xFF));
	outb(CRTCR_ADDR, CRTCR_CH);
	outb(CRTCR_DATA, (uint8_t)((loc >> 8) & 0xFF));
}

void write(const char *string)
{
    uint16_t pos = getcursorloc();
    uint8_t x = pos % SCREEN_WIDTH;
    uint8_t y = pos / SCREEN_WIDTH;

    for (int32_t i = 0; string[i]; i++)
    {
        // Handle character
        switch (string[i])
        {
        case '\r':
            x = 0;
            break;
        case '\n':
            x = 0;
            y++;
            break;
        default:
            putc(string[i], x++, y);
            if(x == SCREEN_WIDTH)
            {
                x = 0;
                y++;
            }
            break;
        }

        // Handle scroll
        if(y == SCREEN_HEIGHT)
        {
            for (uint16_t j = 0; j < (SCREEN_HEIGHT - 1) * SCREEN_WIDTH; j++)
            {
                VGAMEM[j].ch = VGAMEM[j + SCREEN_WIDTH].ch;
                VGAMEM[j].attr = VGAMEM[j + SCREEN_WIDTH].attr;
            }
            for (uint16_t k = (SCREEN_HEIGHT - 1) * SCREEN_WIDTH; k < SCREEN_HEIGHT * SCREEN_WIDTH; k++)
            {
                VGAMEM[k].ch = ' ';
                VGAMEM[k].attr = 0x7;
            }
            y--;
        }
    }
    setcursorloc(x + SCREEN_WIDTH * y);
}
