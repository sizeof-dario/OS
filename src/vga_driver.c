#include "vga_driver.h"

/* Still needs refactioring. */

void VGA_setmem(uint16_t offset, char ascii, uint8_t color)
{
    	VGA_MEM[offset].ascii = ascii;
    	VGA_MEM[offset].attr  = color;
}

uint16_t VGA_getcursorlocation(void)
{
    	uint16_t pos = 0;
    	outb(CRT_ADDR_REG, CRT_CURSOR_LOW);
    	pos |= inb(CRT_DATA_REG);
    	outb(CRT_ADDR_REG, CRT_CURSOR_HIGH);
    	pos |= ((uint16_t)inb(CRT_DATA_REG)) << 8;
    	return pos;
}

void VGA_setcursorlocation(uint16_t loc)
{
    	outb(CRT_ADDR_REG, CRT_CURSOR_LOW);
	outb(CRT_DATA_REG, (uint8_t)(loc & 0xFF));
	outb(CRT_ADDR_REG, CRT_CURSOR_HIGH);
	outb(CRT_DATA_REG, (uint8_t)((loc >> 8) & 0xFF));
}

void write(const char *ntstring, uint8_t color)
{
    	uint16_t pos = VGA_getcursorlocation();
    	uint8_t x = pos % VGA_WIDTH;
    	uint8_t y = pos / VGA_WIDTH;

	for (int32_t i = 0; ntstring[i]; i++) {
		/* Handle character. */
		switch (ntstring[i])
		{
		case '\r':
			x = 0;
			break;
		case '\n':
			x = 0;
			y++;
			break;
		default:
			VGA_setmem(x + y * VGA_WIDTH, ntstring[i], color);
			x++;
			if(x == VGA_WIDTH) {
				x = 0;
				y++;
			}
			break;
		}

		/* Handle scroll. */
		if(y == VGA_HEIGHT) {
			for (uint16_t j = 0; j < (VGA_HEIGHT - 1) * VGA_WIDTH; j++) {
				VGA_MEM[j].ascii = VGA_MEM[j + VGA_WIDTH].ascii;
				VGA_MEM[j].attr = VGA_MEM[j + VGA_WIDTH].attr;
			}
			for (uint16_t k = (VGA_HEIGHT - 1) * VGA_WIDTH; k < VGA_HEIGHT * VGA_WIDTH; k++) {
				VGA_MEM[k].ascii = ' ';
				VGA_MEM[k].attr = VGA_COLOR_WHITE;
			}
			y--;
		}
	}
	VGA_setcursorlocation(x + VGA_WIDTH * y);
}
