/* bmp.c */
#include "bmp.h"
#include "omnistd.h"

Bitmap * parse_bmp(i8* image,i16 x,i16 y){
if (!image) return (Bitmap*)0;
save();

i8 *filename = addbmp(image);
i16 fd = open(filename, 0);
if (!fd) { load(); return 0; }

Bmp_header *hdr = (Bmp_header*)alloc(sizeof(Bmp_header));
if (!hdr){close(fd);load();return (Bitmap*)0;}
readn(fd, hdr, sizeof(Bmp_header));

Bmp_Info_header * info = (Bmp_Info_header*)alloc(sizeof(Bmp_Info_header));
if (!info){close(fd);load();return (Bitmap*)0;}
readn(fd, info, sizeof(Bmp_Info_header));


Color_table *clrs = (Color_table*)alloc(sizeof(Color_table));
if (!clrs){close(fd);load();return (Bitmap *)0;}
readn(fd, clrs, sizeof(Color_table));

Bitmap *bm = (Bitmap *)alloc(sizeof(Bitmap));
if (!bm){close(fd);load();return (Bitmap *)0;}


bm->y = y;
bm->x = x;
bm->header = hdr;
bm->info = info;
bm->color = clrs; 
bm->filename = filename;


if (hdr->sign != 0x4D42) { 
    close(fd); load(); return 0;
}

close(fd);
bmp_show(bm);
return bm;
}

void bmp_show(Bitmap*bmp){
printf("Name: %s\nX: %s\nY: %s\nSignature: %s\nSize: %s\nOffset: %s\nHeight: %s\nWidth: %s\n",(i8*)bmp->filename,tostr(bmp->x),tostr(bmp->y),tostr(bmp->header->sign),tostr(bmp->header->file_size),tostr(bmp->header->offset),tostr(bmp->info->height),tostr(bmp->info->width));
return;
}



i8* addbmp(i8* a){
    if (!a) return (i8*)0;
    i16 len = 0;
    while(a[len] != '\0') len++;

    i8* buff = (i8*)alloc(len + 5); 
    if (!buff) return (i8*)0;

    for(i16 i = 0; i < len; i++) buff[i] = a[i];
    buff[len++] = '.';
    buff[len++] = 'b';
    buff[len++] = 'm';
    buff[len++] = 'p';
    buff[len] = '\0';
    return buff;
}


i8 draw_bmp(Bitmap* bmp){
if (!bmp || !videoflag) return 0;
save();
i8 byte,bl,bh;
i16 size,width,height,l,col,fd,n;
Point *pnt;
fd = open(bmp->filename,bmp->header->offset);
if (!fd) {load();return 0;}
size = ((bmp->info->height * bmp->info->width) % 2)?((bmp->info->height * bmp->info->width) >> 1)+1:(bmp->info->height * bmp->info->width) >> 1;
col = 0;
l = bmp->info->height;

width = bmp->info->width/2;

for (n=size;n;n--){
if (!(n % width)) {l--;col = 0;}
byte = read(fd);
bl = (byte & 0x0f);
bh = ((byte & 0xf0) >> 4);

pnt = init_point(col+bmp->x,l+bmp->y,3);
if (pnt) draw_point(pnt);
col++;
pnt = init_point(col+bmp->x,l+bmp->y,3);

if (pnt) draw_point(pnt);
col++;
load(); 
}
close(fd);

return 1;
}
 