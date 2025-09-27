/*  fs.h  */
#include "fs.h"

// TODO: Fix the bitmap and th ebitmap related fxns


internal Bitmap* mkbitmap(Filesystem*fs,i8 scan){
FSblock block;
i16 idx = 0;
if (!fs) return(Bitmap*)0;
Bitmap *map = (Bitmap *)alloc(ceil_div(fs->dd->blocks,8));
if (!map) return(Bitmap*)0;
if (!scan) {zero(map,ceil_div(fs->dd->blocks,8));return map;}

for (int i = 2;i <= fs->metadata.inodeblocks+1;i++){ 
    if (!dread(fs->dd,(i8 *)&block,i)) {
        dealloc(map);
        return (Bitmap*)0;}

    for (int j = 0 ;j < Inodesperblock;j++,idx++){
        if (block.inodes[j].validtype & 0x01){
            setbit((i8*)map,idx);
        }else{
            unsetbit((i8*)map,idx);
        }
    }
}

return map;
}



internal i16 allocbitmap(Filesystem*fs,Bitmap * bmp){
int i;
if (!bmp || !fs) return 0;
for (i = 0 ;i < fs->dd->blocks;i++) if (!getbit((i8*)bmp,i)) {setbit((i8*)bmp,i);return i+1;}
return 0;
}

internal void  freebitmap(Filesystem*fs,Bitmap*bmp,i16 x){
int i;
if (!bmp || !fs) return;
unsetbit((i8*)bmp,x-1);
}

public Filesystem *fsformat(Disk *dd,Bootsector* mbr,i8 force){
if (!dd){seterr(NOT_MOUNT_ERR);return (Filesystem*)0;}
if (openfiles(dd)){if (!force) {seterr(BUSY_ERR);return (Filesystem*)0;}
else closeallfiles(dd);}
Suprblk super;
Inode inode;
FSblock fsb;

Filesystem *fs = (Filesystem *)alloc(sizeof(Filesystem));

if (!fs){seterr(MEM_ERR);return (Filesystem*)0;}
fs->drive = dd->drive_no;
fs->dd = dd;
inode.validtype = DirType;
inode.size = 0;
zero((i8 *)&inode.name,11);
inode.indirect = 0;
zero((i8 *)inode.direct,sizeof(ptr) * IndirPtrsperInode);
super.magic[0] = magic_str1;
super.magic[1] = magic_str2;
super.inodes = 1;
super.blocks = dd->blocks;
super.inodeblocks = ceil_div(super.blocks,10);
super._ = 0;
if (mbr) copyn((i8 *)&super.boot,(i8*)mbr,500);
else zero((i8 *)&super.boot,500);


if (!dwrite(dd,&super,1)){dealloc(fs); seterr(DISK_IO_ERR);return (Filesystem*)0;}
Block x;
zero(x,BLOCK_SIZE);
copyn((i8 *)x,(i8 *)&inode,sizeof(inode));
if (!dwrite(dd,x,2)){dealloc(fs); seterr(DISK_IO_ERR);return (Filesystem*)0;}
for (int i = 0 ;i < super.inodeblocks;i++)
{zero((i8*)&fsb,BLOCK_SIZE);
if (!dwrite(dd,(i8 *)&fsb,(i+3))) {dealloc(fs);seterr(DISK_IO_ERR);return (Filesystem*)0;}
}
copyn((i8*)&fs->metadata,(i8*)&super,BLOCK_SIZE);
Bitmap *bm = mkbitmap(fs,0);
i16 size = 1+1+fs->metadata.inodeblocks;
for (int i = 0 ;i < size;i++){setbit((i8*)bm,i);}
fs->bitmap = bm;
return fs;
}



internal i16 openfiles(Disk * dd){
return 0;
}
internal void closeallfiles(Disk * dd){
return;
}
