#include "int.h"
#include "VGAdriver.h"

struct interrupt_descriptor IDT[IDT_LENGTH];

void set_interrupt_descriptor(uint8_t index, uint32_t offset, uint16_t selector, uint8_t gate_type, uint8_t dpl)
{
    IDT[index].offset_low = (uint16_t)(offset & 0x0000FFFF);
    IDT[index].selector = selector;
    IDT[index].reserved = 0;
    IDT[index].gate_type = gate_type;
    IDT[index].zero = 0;
    IDT[index].dpl = dpl;
    IDT[index].present = 1;
    IDT[index].offset_high = (uint16_t)(offset >> 0x10);
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
    for (uint16_t i = 0; i < IDT_LENGTH; i++)
    {
        if(i == 8 || (i > 9 && i < 15) || i == 17 || i == 21) {
            set_interrupt_descriptor(i, 
                (uint32_t)&isr_generic_error_code, 
                SELECTOR_CODE, 
                GATE_TYPE_INTERRUPT, 
                DPL_0);
        } else {
            set_interrupt_descriptor(i, 
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

    struct IDTR_value idtr_v;
    idtr_v.base = (uintptr_t)IDT;
    idtr_v.limit = IDT_LENGTH - 1;
    lidt(&idtr_v);

    // Remap PIC

    // Adding memory to clobbered prevents GCC to reorder instructions.
    asm volatile("sti" : : : "memory");
}