#ifndef PIC_H
#define PIC_H 1

#include <stdint.h>

#include "io.h"

void pic_mask(uint16_t port, uint8_t mask);

void pic_init(
	uint16_t command_port, 
	uint16_t data_port, 
	uint8_t  icw_1, 
	uint8_t  icw_2, 
	uint8_t  icw_3, 
	uint8_t  icw_4,
	uint8_t  init_mask);

void pic_remap();

#endif /* ifndef PIC_H. */
