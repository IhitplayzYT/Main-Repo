/* bmp.c */
#include "bmp.h"

Bitmap * parse_bmp(i8* image,i16 x,i16 y){
if (!image) return (Bitmap*)0;
save();
Bmp_header *hdr = (Bmp_header*)image;
i16 size = sizeof(Bmp_header);
Bmp_Info_header * info = (Bmp_Info_header*)(image+size);
size += sizeof(Bmp_Info_header);
Color_table *clrs = (Color_table*)alloc(sizeof(Color_table));
if (!clrs){ load();return (Bitmap *)0;}
copy(clrs,image+size,sizeof(clrs));
Bitmap *bm = (Bitmap *)alloc(sizeof(Bitmap));
if (!bm){ load();return (Bitmap *)0;}
bm->x = x;
bm->y = y;
bm->header = hdr;
bm->info = info;
bm->color = clrs; 
bm->filename = addbmp(image);
i16 fd = open(bm->filename,0);
i8 *buff;
if (!fd) { load();return (Bitmap *)0;}
buff = (i8 *)alloc(128);
i8* p = buff;
for (i8 i = 118;i;i--,p++)  *p = read(fd);
close(fd);
return bm;
}




i8* addbmp(i8* a){
static i8 buff[15];
i8* p =a,i;

for (i = 0;*p != '\0';p++,i++) buff[i] = *p;
buff[i++] = '.';
buff[i++] = 'b';
buff[i++] = 'm';
buff[i++] = 'p';
buff[i++] = '$';
buff[i] = '\0';
return buff;
}



i8 draw_bmp(Bitmap* bmp){
if (!bmp || !videoflag) return 0;
save();
i8 byte,bl,bh;
i16 size,width,height,l,col,fd,n;
Point *pnt;

width = bmp->info->width;
height = bmp->info->height;
size = ((height * width) % 2)?((height * width) >> 1)+1:(height * width) >> 1;
l = col = 0;
fd = open(bmp->filename,bmp->header->offset);
if (!fd) {load();return 0;}

for (n=size;n;n--){
if (!(n % width)) {l++;col = 0;}
byte = read(fd);
if (!byte){close(fd);load();return 1;}
bl = (byte & 0x0f);
bh = ((byte & 0xf0) >> 4);

pnt = init_point(col+bmp->x,l+bmp->y,getcolor(bmp,bh));
col++;
if (!pnt){close(fd);load();return 0;}
draw_point(pnt);

if (!(n % width)) {l++;col = 0;}

pnt = init_point(col+bmp->x,l+bmp->y,getcolor(bmp,bl));
col++;
if (!pnt){close(fd);load();return 0;}
draw_point(pnt);
load(); 
}
close(fd);

return 1;
}
 