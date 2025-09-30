; xgfx.asm
bits 16
%include "xgfx.h"
global exit
global xputchar
global xvideomode
global xgetchar
exit:
        mov ah,0x4c
        xor al,al
        int 0x21
xputchar:
    push bp
    mov bp,sp

    arg ax,0
    mov ah,0x0e
    xor bx,bx
    int 0x10

    mov sp,bp
    pop bp
    ret
xgetchar:
    push bp
    mov bp,sp
    xor ax,ax
    int 0x16
    mov sp,bp
    pop bp
    ret
xvideomode:
    push bp
    mov bp,sp
    arg ax,0
    xor ah,ah
    int 0x10
    mov sp,bp
    pop bp
    ret