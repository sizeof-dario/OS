ORG 0x7C00
BITS 16
; vvv SET SEGMENT REGISTERS vvv ------------------------------------------------

        ; set cs:ip to 0x0000:0x7C00
        jmp     0x0000:start
start:

        ; set segment registers to 0x0000 and set the stack
        ; the stack is set below 0x0000:0x7C00
        ; the stack is set along with segment registers to ensure proper ss:sp
        ;       setting via mov ss interrupt shadow feature
        xor     ax,     ax
        mov     ss,     ax
        mov     sp,     0x7C00
        mov     ds,     ax
        mov     es,     ax

; ^^^ SET SEGMENT REGISTERS ^^^ ------------------------------------------------

; vvv READ SECTOR 2 vvv --------------------------------------------------------

        ; sector 2 will be placed after MBR at 0x7E00

        ; set up registers to query int 0x13
        ; dl (drive index) is assumed to be set by the BIOS
        ; es (destination segment) is already set to 0x0000
        mov     ah,     0x02    ; read subfunction
        mov     al,     0x01    ; number of sectors
        mov     ch,     0x00    ; 0-7 cylinder bits
        mov     cl,     0x02    ; 8-9 cylinder bits -> [--][------] <- sector
        mov     dh,     0x00    ; head
        mov     bx,     0x7E00  ; destination offset

        int     0x13            
        jc      safe_hlt        ; halt on failure

; ^^^ READ SECTOR 2 ^^^ --------------------------------------------------------

safe_hlt:
        hlt
        jmp     safe_hlt

; vvv GLOBAL DESCRIPTOR TABLE vvv ----------------------------------------------
GDT:
; null segment
DQ 0
; code segment
;       base            = 0x00000000
;       limit           = 0xFFFFF
;
;                         P DPL S E D/C R/A A
;       access byte     = 1 00  1 1 0   0   0 = 0x98
;
;                         G DB L R
;       flags           = 1 1  0 0 = 0xC
DW 0xFFFF
TIMES 3 DB 0x0000
DB 0x98
DB 0xCF ; 0xC0 + 0x0F
DB 0x00
; data segment
;       base            = 0x00000000
;       limit           = 0xFFFFF
;
;                         P DPL S E D/C R/A A
;       access byte     = 1 00  1 0 0   1   0 = 0x92
;
;                         G DB L R
;       flags           = 1 1  0 0 = 0xC
DW 0xFFFF
TIMES 3 DB 0x0000
DB 0x92
DB 0xCF ; 0xC0 + 0x0F
DB 0x00
GDT_end:

GDTR_value:
DW GDT_end - GDT - 1
DD GDT
; ^^^ GLOBAL DESCRIPTOR TABLE ^^^ ----------------------------------------------

TIMES 510 - ($ - $$) DB 0
DW 0xAA55

; SECTOR 2
TIMES 512 db 0xDC   ; test value
