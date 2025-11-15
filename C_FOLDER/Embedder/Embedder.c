/* Embedder.c */
#include "Embedder.h"

void usage_(i8 * arg){
fprintf(stderr,"Usage : %s [-h | -a] <file>\n",arg);
exit(-1);
}

int main(int argc, char *argv[]) { 
i8 * identifier = 0;
Lang lang = c;
if (argc > 2)    {
if (compare(argv[1],"-h") ||  compare(argv[1],"--help")) usage(argv[0]);
else if (argc == 3){
    if (compare(argv[1],"-a")){
        lang = assm;
        identifier = (i8*)argv[2];
    }
}else usage(argv[0]);
}
else if (argc == 2){
identifier = (i8*)argv[1];
}
else usage(argv[0]);

printheader(identifier,lang);
printbody(lang);
write(0,"\n",1);
fflush(stdout);
return 0; }


i8 compare_(i8*a,i8*b){
i8 * p,*q;
for (p = a,q = b; *p!='\0' && *q !='\0';p++,q++){
    if (*p != *q) return 0;
}
if (*p == '\0' && *q == '\0') return 1;
return 0;
}

void printheader(i8* iden,Lang lan){
if (!*iden) return;
switch (lan){
case c:{
printf("unsigned char %s[] =\n\t\"",iden);
break;    }
case assm:{
printf("%s:\n\tdb  ",iden);
break;
}
}
fflush(stdout);  
}

void printbody(Lang x){
i32 n = 0;
int ret;
i8 buff[2],*ch;
buff[0] = buff[1] = 0;

while ((ret = read(0,buff,1)) == 1){
    if (x == assm && !n) write(0,",",1);
    n++;
    ch = convert(*buff,x);
    switch (x){
        case assm:
            if (n > 1)write(1,",",1);
                write(1,ch,len(ch));
                if (!(n % 16)) write (1,"\n\tdb ",5);
            break;
        default:
        case c: 
            write(1,ch,len(ch));
            if (!(n % 16)) write (1,"\"\n\t\"",4);
            break;
    }
buff[0] = buff[1] = 0;
}

switch(x){
    case c:
    {write(1,"\";\n",3);
    break;}
    case assm:
    default:
        write(1,"\n",1);
}

}


i8* convert(i8 ch,Lang lang){
static char ret[8];
for (int i = 0 ;i < 8;i++) ret[i] = '0';
switch(lang){
case assm:{
snprintf(ret,7,"0x%.02hhx",ch);
break;
}
default:
case c:{
snprintf(ret,7,"\\x%.02hhx",ch);
break;}
}
return (i8*)ret;
}

i32 len(i8*a)
{i8 * p;
    for (p=a;*p != '\0';p++);
return p - a;
}