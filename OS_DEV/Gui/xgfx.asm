; xgfx.asm
bits 16
%include "xgfx.h"
global exit
global xputchar
global xvideomode
global heap1
global xgetchar
global xdrawpoint_txt
global xdrawpixel
global heapsize
heap1 dw _heap
_heap:  times 0x1000 db 0x00
        times 0x1000 db 0x00
        times 0x1000 db 0x00
        times 0x1000 db 0x00
        times 0x1000 db 0x00
        times 0x1000 db 0x00
        times 0x1000 db 0x00
        times 0x1000 db 0x00
        times 0x1000 db 0x00
        times 0x1000 db 0x00
        times 0x1000 db 0x00
        times 0x1000 db 0x00

heapsize dw ($$ - _heap)
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

xdrawpoint_txt:
    push bp
    mov bp,sp

    arg ax,0
    arg dx,1
    and dx,0xf
    and ax,0xf
    mov dh,al

    xor ax,ax
    mov ah,0x02
    xor bx,bx
    int 0x10

    mov ah,0x0a
    mov al,'+'
    xor bx,bx
    mov cx,0x01
    int 0x10

    mov ax,0x01
    mov sp,bp
    pop bp
    ret

xdrawpixel:
    push bp
    mov bp,sp
    arg cx,0
    arg dx,1
    arg ax,2
    xor bx,bx
    mov ah,0x0c
    int 0x10

    mov sp,bp
    pop bp
    ret
