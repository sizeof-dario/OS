#ifndef INT_H
#define INT_H 1

#include <stdint.h>
#include "io.h"

#define IDT_LENGTH 		(uint16_t)256

#define SELECTOR_CODE		(uint16_t)0x0008
#define SELECTOR_DATA		(uint16_t)0x0010
#define GATE_TYPE_INTERRUPT 	(uint8_t)0xE
#define GATE_TYPE_TRAP		(uint8_t)0xF
#define DPL_0			(uint8_t)0x0
#define DPL_3			(uint8_t)0x3

struct IDTR_value {
	uint16_t  limit;
	uintptr_t base;
} __attribute__((packed));

static inline __attribute__((always_inline))
void lidt(struct IDTR_value *idtrv_p)
{
    asm volatile("lidt %0" : : "m"(*idtrv_p));
}

struct interrupt_descriptor {
	uint16_t offset_low;
	uint16_t selector;
	uint8_t	 reserved;
	uint8_t	 gate_type : 4;
	uint8_t	 zero	   : 1;
	uint8_t	 dpl	   : 2;
	uint8_t	 present   : 1;
	uint16_t offset_high;
} __attribute__((packed));

struct interrupt_frame {
	uint32_t eip;
	uint16_t cs;
	uint16_t padding;
	uint32_t eflags;
} __attribute__((packed));

void set_interrupt_descriptor(
	uint8_t  index,	    /* Vector index in the IDT. 		     */
	uint32_t offset,    /* Address of the ISR.  			     */
	uint16_t selector,  /* Selector of the ISR address. 		     */
	uint8_t  gate_type, /* Either GATE_TYPE_INTERRUPT or GATE_TYPE_TRAP. */
	uint8_t  dpl	    /* Either DPL_0 or DPL_3. 			     */
);

void isr_generic(struct interrupt_frame *frame) 
	__attribute__((interrupt));

void isr_generic_error_code(struct interrupt_frame *frame, uint32_t error_code)
	__attribute__((interrupt));

void idt_init();

void pic_mask(uint16_t port, uint8_t mask);

void pic_init(
	uint16_t command_port, 
	uint16_t data_port, 
	uint8_t icw1, 
	uint8_t icw2, 
	uint8_t icw3, 
	uint8_t icw4,
	uint8_t init_mask
);

void pic_remap();

void enable_protected_interrupts();

#endif /* ifndef INT_H. */
