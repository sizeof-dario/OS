; ORG 0x7C00
;       not needed anymore since the file gets assembled into a object file
EXTERN kmain

BITS 16
; vvv SET CODE AND STACK REGISTERS vvv -----------------------------------------

        ; set cs:ip to 0x0000:0x7C00
        jmp     0x0000:start
start:

        ; set the stack below 0x0000:0x7C00
        xor     ax,     ax
        mov     ss,     ax
        mov     sp,     0x7C00

; ^^^ SET CODE AND STACK REGISTERS ^^^ -----------------------------------------

; CURRENT REGISTERS STATE
;       ax      bx      cx      dx      cs      ds      es      ss
;       0x0000  0x????  0x????  0x????  0x0000  0x????  0x????  0x0000

; vvv ENABLE A20 LINE vvv ------------------------------------------------------
        
        ; check if A20 line is already enabled
        mov     es,                     ax
        mov     ax,                     0xFFFF
        mov     ds,                     ax
        mov     WORD[0x7E0E],           0x0000
        cmp     WORD[es:0x7DFE],        0x55AA

        ; do nothing if already enabled
        je      A20_enabled

        ; query A20 gate support
        mov     ax,     0x2403                    
        int     0x15
        jc      safe_hlt        ; halt on failure
        test    ah,     ah
        jnz     safe_hlt        ; halt on failure

        ; activate A20 gate
        mov     ax,     0x2401          
        int     0x15
        jc      safe_hlt        ; halt on failure
        test    ah,     ah
        jnz     safe_hlt        ; halt on failure

A20_enabled:
; ^^^ ENABLE A20 LINE ^^^ ------------------------------------------------------

; CURRENT REGISTERS STATE
;       ax      bx      cx      dx      cs      ds      es      ss
;       0x????  0x????  0x????  0x????  0x0000  0xFFFF  0x0000  0x0000

; vvv READ SECTOR 2 vvv --------------------------------------------------------

        ; sector 2 will be placed after MBR at 0x7E00

        ; set up registers to query int 0x13
        ; dl (drive index) is assumed to be set by the BIOS
        ; es (destination segment) is already set to 0x0000
        mov     ah,     0x02    ; read subfunction
        mov     al,     0x09    ; number of sectors (10 - 1)
        mov     ch,     0x00    ; 0-7 cylinder bits
        mov     cl,     0x02    ; 8-9 cylinder bits -> [--][------] <- sector
        mov     dh,     0x00    ; head
        mov     bx,     0x7E00  ; destination offset

        int     0x13            
        jnc     switch_mode     ; halt on failure

; ^^^ READ SECTOR 2 ^^^ --------------------------------------------------------

; CURRENT REGISTERS STATE
;       ax      bx      cx      dx      cs      ds      es      ss
;       0x0001  0x7E00  0x0002  0x00??  0x0000  0xFFFF  0x0000  0x0000


; 16 BIT SAFE HALT LOOP
safe_hlt:
        hlt
        jmp     safe_hlt

; vvv SWITCH TO PROTECTED MODE vvv ---------------------------------------------
switch_mode:

        lgdt    [es:GDTR_value]         ; load gdt
        cli                             ; disable interrupts
        ; enter protected mode
        mov     eax,    cr0             
        or      eax,    0x00000001
        mov     cr0,    eax
        ; far jump at table index 1 in the GDT, RPL = 0, TI = 0
        jmp     0x0008:start_protected

start_protected:
; ^^^ SWITCH TO PROTECTED MODE ^^^ ---------------------------------------------
BITS 32

; CURRENT REGISTERS STATE
;       eax             ebx             ecx             edx
;       0x????????      0x????7E00      0x????0002      0x????00??
;       cs      ds      es      ss
;       0x0008  0xFFFF  0x0000  0x0000

; vvv PREPARE FOR C CODE vvv ---------------------------------------------------

        ; reset stack
        mov     ax,     0x0010
        mov     ss,     ax
        mov     esp,    0x000A0000      ; below VGA video memory

        ; set other segment registers
        mov     ds,     ax
        mov     es,     ax

; ^^^ PREPARE FOR C CODE ^^^ ---------------------------------------------------

; CURRENT REGISTERS STATE
;       eax             ebx             ecx             edx
;       0x????0010      0x????7E00      0x????0002      0x????00??
;       cs      ds      es      ss
;       0x0008  0x0010  0x0010  0x0010

; JUMP TO C
        call    kmain

; 32 BIT SAFE HALT LOOP
safe_hlt_protected:
        hlt
        jmp safe_hlt_protected

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