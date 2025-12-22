section .data
    list:
       dd 1,3,12,34,5,21,5,3,9,62,5,3,78,4,23,0
section .text
global _start
; edi -> index
; eax -> curr
; ebx -> largest
_start:
   xor edi,edi
   mov eax,[list + edi * 4]
   mov ebx,eax
start_loop:
    cmp eax,0
    je loop_exit 
    inc edi 
    mov eax,[list + edi * 4]
    cmp eax,ebx
    jle start_loop
    mov ebx,eax 
    jmp start_loop

loop_exit:
    mov eax,1 
    int 80h