/*  fs.h  */
#include "fs.h"

internal Filesystem *fsformat(Disk *dd,Bootsector* mbr,i8 force){
if (!dd) reterr(NOT_MOUNT_ERR);
if (openfiles(dd)){
if (!force)reterr(BAD_FD)
else closefiles(dd);
}
i16 size,inodeblocks,blocks;
Suprblk super;
Inode inode;
blocks = dd->blocks;
inodeblocks = (i16)blocks/10;
inode.validtype = TypeDir;
inode.size = 0;
zero((i8 *)&inode.name,11);
inode.indirect = 0;
size = sizeof(ptr) * PtrsperInode;
zero((i8 *)inode.direct,size);
super.magic[0] = magic_str1;
super.magic[1] = magic_str2;
super.inodes = 1;
super.inodeblocks = inodeblocks;
super.blocks = blocks;
super._ = 0;
if (mbr) copyn((i8 *)&super.boot,mbr,500);
else zero((i8 *)&super.boot,500);

Filesystem *fs;

if (!dwrite(dd,&super,1)) reterr(DISK_IO_ERR);
if (!dwrite(dd,&inode,2)) reterr(DISK_IO_ERR);





}
