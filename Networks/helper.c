#include "Networks.h"

public void _copyn(i8 *a,i8 *b,i16 n,i8 z){
if (!a || !b) return;
if (z==1) for (;n;n--,a++,b++) *a = *b;
else{
for (;n && *a != '\0' && *b != '\0';n--,a++,b++)*a = *b;
*a = '\0';
}
}

public void zero(i8* str,i32 n){
i8* p;
for (p =str;n;n--,p++) *p = 0;
}

public void print_hex(void * str,i32 n){
i32 i = 0;
for (;i < n ;i ++){
printf("0x%02hhX ",*((i8*)str+i));
if (!((i+1) % 16)){
for (i16 m = i - 15;m <= i;m++) printf("%c", (*((i8*)str+m) >=32 && *((i8*)str+m) <= 126)? *((i8*)str+m) : '.' );
printf("\n");}}

int start = n - (n % 16);
if (n % 16) {
    i32 j;
    printf("\t");
    for (int k = 0 ; k < start/8;k++) printf("    ");
    for (j = start; j < n; j++) {
        printf("%c", ((*((i8*)str+j) >= 32 && *((i8*)str+j) <= 126) ? *((i8*)str+j) : '.'));
    }
    

} 

printf("\n");
}