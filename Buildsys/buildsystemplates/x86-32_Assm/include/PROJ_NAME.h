;  PROJ_NAME.h 


section .text
global clr_all
clr_all:
  cli
  xor eax,eax
  xor ebx,ebx
  xor ecx,ecx
  xor edx,edx
  sti
