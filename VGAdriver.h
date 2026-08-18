#ifndef VGA_DRIVER_H
#define VGA_DRIVER_H 1

#include <stdint.h>
#include "io.h"

#define SCREEN_WIDTH    80
#define SCREEN_HEIGHT   25

// CRT Controller Registers

#define CRTCR_ADDR  0x3D4
#define CRTCR_DATA  0x3D5
// Cursor location High byte
#define CRTCR_CH    0x0E 
// Cursor location Low byte   
#define CRTCR_CL    0x0F   

typedef struct __attribute__((packed))
{
    char    ch;
    uint8_t attr;
} VGAchar_t;

// Starting address of VGA memory in RAM
static VGAchar_t *const VGAMEM = (VGAchar_t *)0xB8000;

// Put a character on screen
void putc(char ch, uint8_t x, uint8_t y);

// Get cursor location
uint16_t getcursorloc(void);

// Set cursor location
void setcursorloc(uint16_t loc);

// Write a sstring on screen
void write(const char* string);

#endif // VGA_DRIVER_H