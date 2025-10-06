/* omnistd.c */
#include "omnistd.h"
#include "gui.h"

void _copy(i8* a,i8* b,i16 n,i8 flag){
i8*p,*q;
if (flag){
for (p=a,q=b;n && *a && *b;n--,p++,q++) *p = *q;
}else{
for (p=a,q=b;n;n--,p++,q++) *p = *q;
}
}

void _fill(i8*a,i8 c,i16 n,i8 flag){
i8*p;
if (flag) for (p=a;n && *a;n--,p++) *p = c;
else for (p=a;n;n--,p++) *p = c;
}

i8* snprintf(i8*a,i16 n,i8* frmt,...){
if (!a || !frmt) return 0;
static i8 buff[BUFF_SZ];
i16 bytes = 0;
i8* dptr = a,*sptr = frmt,c,contin=1; 
while (*sptr && contin){
switch (*sptr){
case '%':
    




    break;
case '\n':
    if (bytes > BUFF_SZ - 2){contin = 0;break;}
    *dptr ++ = '\r';
    *dptr ++ = '\n';
    sptr++;
    bytes++;

    break; 
default:
    if (bytes > BUFF_SZ-1){contin=0;break;}
    *dptr ++ = *sptr ++;
    bytes++;
    break;
}

}
*dptr = '\0';
return a;
}