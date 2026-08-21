#ifndef VGA_DRIVER_H
#define VGA_DRIVER_H 1

#include <stdint.h>
#include "io.h"

#define VGA_WIDTH	(uint8_t)80
#define VGA_HEIGHT	(uint8_t)25

#define CRT_ADDR_REG	(uint16_t)0x03D4
#define CRT_DATA_REG  	(uint16_t)0x03D5
/* Cursor location high byte. */
#define CRT_CURSOR_HIGH	(uint8_t)0x0E 
/* Cursor location low byte. */  
#define CRT_CURSOR_LOW	(uint8_t)0x0F

#define VGA_COLOR_BLACK (uint8_t)0x00
#define VGA_COLOR_BLUE	(uint8_t)0x01
#define VGA_COLOR_GREEN	(uint8_t)0x02
#define VGA_COLOR_CYAN	(uint8_t)0x03
#define VGA_COLOR_RED	(uint8_t)0x04
#define VGA_COLOR_MAG	(uint8_t)0x05
#define VGA_COLOR_WHITE	(uint8_t)0x07

struct VGA_char {
	char 	ascii;
	uint8_t attr;
} __attribute__((packed));

/* Starting address of VGA memory in RAM. */
static struct VGA_char *const VGA_MEM = (struct VGA_char *)0xB8000;

void VGA_setmem(uint16_t offset, char ascii, uint8_t color);

uint16_t VGA_getcursorlocation(void);

void VGA_setcursorlocation(uint16_t location);

/* Write a null-terminated string to the terminal. */
void write(const char* ntstring, uint8_t color);

#endif /* ifdef VGA_DRIVER_H. */