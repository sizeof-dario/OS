ORG 0x7C00
BITS 16

    ; set cs:ip to 0x0000:0x7C00
    jmp 0x0000:start
start:

    ; set ss and ds to 0x0000
    xor ax, ax
    mov ss, ax ; (sp yet to be set)
    mov ds, ax

    ; finish to set the stack (below the master boot record)
    mov sp, 0x7C00

safe_hlt:
    hlt
    jmp safe_hlt

TIMES 510 - ($ - $$) DB 0
DW 0xAA55
