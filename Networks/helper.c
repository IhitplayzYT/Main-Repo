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

void print_hex(void *str, i32 n) {
    i32 i, j;
    i8 *data = (i8*)str;
    for (i = 0; i < n; i += 16) {
        for (j = 0; j < 16; j++) {
            if (i + j < n) printf("0x%02hhX ", data[i + j]);
            else printf("     "); 
        }
        printf(" ");
        for (j = 0; j < 16 && i + j < n; j++) {
            i8 c = data[i + j];
            printf("%c", (c >= 32 && c <= 126) ? c : '.');
        }

        printf("\n");
    }
}