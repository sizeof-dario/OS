OUTPUT_FORMAT("binary")

SECTIONS
{
    . = 0x7C00;

    .text :
    {
        build/boot.o(.text)
        *(.text)

    }

    .data :
    {
        *(.data)
    }

    .rodata :
    {
        *(.rodata)
    }

    .bss :
    {
        *(.bss)
    }

    /* 10 sectors in total */
    . = 0x9000 - 1;
    .padding :
    {
        BYTE(0x00)
    }
}
