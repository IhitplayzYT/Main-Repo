;   PROJ_NAME.h 


section .text
global clr_all
clr_all:
  cli
  xor ax,ax
  xor bx,bx
  xor cx,cx
  xor dx,dx
  xor ds,ds
  xor si,si
  xor di,di
  xor bp,bp
  xor es,es
  xor ss,ss
  mov sp,0x7c00
  sti
