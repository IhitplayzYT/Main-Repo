section .data
    list:
        dd 5,4,3,2,1,0
section .text

global _start
; eax -> temp
; edi -> idx
; ebx -> cur
; ecx -> next
; esi -> loop_counter
_start:
    mov ebx,[list]
    mov ecx,[list + 4]
    xor edi,edi
    xor eax,eax
    xor esi,esi
en_loop:
    cmp esi,4
    jle incre
    cmp ecx,0
    je exit
    cmp ebx,ecx
    jge swap
    jmp en_loop

swap:
    mov eax,ebx
    mov ebx,ecx
    mov ecx,eax
    jmp en_loop    
incre:
    inc esi
exit:
    mov eax,1
    int 0x80