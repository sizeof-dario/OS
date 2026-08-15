OUTPUT_FORMAT("binary")

SECTIONS
{
    . = 0x7C00;

    .text :
    {
        boot.o(.text)
        *(.text)

    }

    .data :
    {
        *(.data)
    }

    .bss :
    {
        *(.bss)
    }

    . = 0x9000 - 1;
    .padding :
    {
        BYTE(0x00)
    }
}
