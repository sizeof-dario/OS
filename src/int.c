#include "int.h"
#include "vga_driver.h"

struct interrupt_descriptor idt[IDT_LENGTH];

void set_interrupt_descriptor(
	uint8_t  idt_vector_index,	    		
	uint32_t isr_address_offset,
	uint16_t selector,  		
	uint8_t  gate_type, 
	uint8_t  dpl)
{
    idt[idt_vector_index].offset_low = (uint16_t)(isr_address_offset & 0xFFFF);
    idt[idt_vector_index].selector = selector;
    idt[idt_vector_index].reserved = 0;
    idt[idt_vector_index].gate_type = gate_type;
    idt[idt_vector_index].zero = 0;
    idt[idt_vector_index].dpl = dpl;
    idt[idt_vector_index].present = 1;
    idt[idt_vector_index].offset_high = (uint16_t)(isr_address_offset >> 0x10);
}

void isr_generic(struct interrupt_frame *frame)
{
    write("Interrupt!\n", VGA_COLOR_RED);
}

void isr_generic_error_code(struct interrupt_frame *frame, uint32_t error_code)
{
    write("Interrupt with error code!\n", VGA_COLOR_RED);
}

void idt_init()
{
    for (uint16_t i = 0; i < IDT_LENGTH; i++) {
        if(i == 8 || (i > 9 && i < 15) || i == 17 || i == 21) {
            set_interrupt_descriptor(
                i, 
                (uint32_t)&isr_generic_error_code, 
                SELECTOR_CODE, 
                GATE_TYPE_INTERRUPT, 
                DPL_0);
        } else {
            set_interrupt_descriptor(
                i, 
                (uint32_t)&isr_generic, 
                SELECTOR_CODE, 
                GATE_TYPE_INTERRUPT, 
                DPL_0);
        }
    }
}

void enable_protected_interrupts()
{
    idt_init();

    struct idtr_value idtr_v;
    idtr_v.base = (uintptr_t)idt;
    idtr_v.limit = IDT_LENGTH - 1;
    lidt(&idtr_v);

    pic_remap();

    asm volatile("sti" : : : "memory");
}