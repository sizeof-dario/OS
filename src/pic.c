#include "pic.h"

void pic_mask(uint16_t port, uint8_t mask)
{
    outb(port, mask);
}

void pic_init(
	uint16_t command_port, 
	uint16_t data_port, 
	uint8_t  icw_1, 
	uint8_t  icw_2, 
	uint8_t  icw_3, 
	uint8_t  icw_4,
	uint8_t  init_mask)
{
    outb(command_port, icw_1);
    outb(data_port, icw_2);
    outb(data_port, icw_3);
    outb(data_port, icw_4);

    outb(data_port, init_mask);
}

/* Still needs refactioring. */

void pic_remap()
{
    pic_init(0x20, 0x21, 0x11, 0x20, 0x04, 0x01, 0xFF);
    pic_init(0xA0, 0XA1, 0X11, 0x28, 0x02, 0x01, 0xFF);
}
