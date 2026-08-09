ORG 0x7C00
BITS 16

    ; set cs:ip to 0x0000:0x7C00
    jmp 0x0000:start
start:

    ; set ss, ds, and es to 0x0000
    xor ax, ax
    mov ss, ax ; (sp yet to be set)
    mov ds, ax
    mov es, ax

    ; finish to set the stack (below the master boot record)
    mov sp, 0x7C00

    ; set up registers to query int 0x13
    mov ah, 0x02    ; read
    mov al, 0x01    ; number ofsectors
    mov ch, 0x00    ; 0-7 cylinder bits
    mov cl, 0x02    ; 8-9 cylinder bits - sector
    mov dh, 0x00    ; head
                    ; dl is assumed to be set by the BIOS
                    ; es was already set to 0x0000
    mov bx, 0x7E00  ; destination offset

    int 0x13
    jc  safe_hlt    ; halt on failure

    ; ...

safe_hlt:
    hlt
    jmp safe_hlt

TIMES 510 - ($ - $$) DB 0
DW 0xAA55

TIMES 512 db 0xDC   ; test value
