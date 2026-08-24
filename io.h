#ifndef IO_H
#define IO_H 1

#include <stdint.h>

/* Reads current byte from a port. */
static inline __attribute__((always_inline))
uint8_t inb(uint16_t port)
{
    uint8_t value;
    asm volatile("inb %w1, %b0" : "=a"(value) : "Nd"(port));
    return value;
}

/* Writes a byte to a port. */
static inline __attribute__((always_inline))
void outb(uint16_t port, uint8_t value)
{
    asm volatile("outb %b0, %w1" : : "a"(value), "Nd"(port));
}

#endif /* ifndef IO_H. */

