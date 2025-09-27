/* stdoslib.c */
#include "stdoslib.h"

public void zero(i8 *p,i8 n){
if (!p || n <= 0) return;
for (;n;p++,n--) *p = (i8)0;
}

public void copyn(i8 *a,i8 *b,i8 n){
for (;*b!='\0' && n;n--) *a = *b;
*a = '\0';
}

public void fill (i8*a,i32 l,i8 hx){
if (!a) return;
for (;l;a++,l--) *a = hx;
}

public int len(i8 * str){
int i;
for (i = 0; str[i] != '\0';i++);
return i;
}

public void copy(i8 *a,i8 *b){
for (;*b !='\0';a++,b++) *a = *b;
*a = '\0';
}

public int min(int a,int b){return (a >= b) ? b : a;}
public int max(int a,int b){return (a >= b) ? a : b;}
