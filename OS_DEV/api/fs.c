/*  fs.h  */
#include "fs.h"

internal Bitmap* mkbitmap(Filesystem*fs,i8 scan){
FSblock block;
i16 idx = 0;
if (!fs) return(Bitmap*)0;
i16 x = ceil_div(fs->dd->blocks,8);
Bitmap *map = (Bitmap *)alloc(x);
if (!map) return(Bitmap*)0;
if (!scan) {zero(map,x);return map;}

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

internal void print_inodes(Filesystem *fs){
if (!fs) return;
i16 m = fs->metadata.inodeblocks * Inodesperblock;
for (int i = 0 ; i < m ;i++){
    Inode * inode = fetchinode(fs,i);
    if (inode && inode->validtype & 0x01){
        printf("----Inode---- %d:\nPath: %s\nSize: %d\nInode Type: %s\n",(i+1),concat("/",filestr(&inode->name)),inode->size,(inode->validtype == FileType)?"File":(inode->validtype == DirType)?"Directory":"????");
        printf("--------\n");
    }
}
}

internal Inode* fetchinode(Filesystem*fs,ptr idx){
if (!fs) return (Inode *)0;
FSblock buff;
Inode * ret = (Inode*)alloc(sizeof(Inode));
if (!ret) return (Inode*)0;
i16 bl = (i16)idx/Inodesperblock;
i16 jump = idx % Inodesperblock;
if (!dread(fs->dd,(i8*)&buff.data,bl+2)) return ret;
memcopy((i8*)ret,(i8*)&(buff.inodes[jump]),sizeof(Inode));
return ret;
}


internal void fsshow(Filesystem*fs){
if (!fs) return;
printf("-----FileSystem-----\nDrive- \\%c\nPath: %s\nBlockSize= %d\nMagicString (0x%.04x) (0x%.04x)\nNo of Blocks: [%d]\nNo of Inodes-> %d\nNo of Inode Blocks->[%d]\n--------------------\n",(fs->drive == 1) ? 'C':(fs->drive == 2)?'D':'?',numppend(Basepath,fs->drive),fs->dd->blocks,fs->metadata.magic[0],fs->metadata.magic[1],fs->metadata.blocks,fs->metadata.inodes,fs->metadata.inodeblocks);
}

internal void print_bitmap(Filesystem*fs){
if (!fs) return;
print_bytes((void *)fs->bitmap,ceil_div(fs->dd->blocks,8));
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

internal Filesystem *fsformat(Disk *dd,Bootsector* mbr,i8 force){
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
if (mbr) memcopy((i8 *)&super.boot,(i8*)mbr,500);
else zero((i8 *)&super.boot,500);
if (!dwrite(dd,&super,1)){dealloc(fs); seterr(DISK_IO_ERR);return (Filesystem*)0;}
Block x;
zero(x,BLOCK_SIZE);
memcopy((i8 *)x,(i8 *)&inode,sizeof(inode));
if (!dwrite(dd,x,2)){dealloc(fs); seterr(DISK_IO_ERR);return (Filesystem*)0;}
for (int i = 0 ;i < super.inodeblocks;i++)
{zero((i8*)&fsb,BLOCK_SIZE);
if (!dwrite(dd,(i8 *)&fsb,(i+3))) {dealloc(fs);seterr(DISK_IO_ERR);return (Filesystem*)0;}
}
memcopy((i8*)&fs->metadata,(i8*)&super,BLOCK_SIZE);
Bitmap *bm = mkbitmap(fs,0);
i16 size = 1+1+fs->metadata.inodeblocks;
for (int i = 0 ;i < size;i++){setbit((i8*)bm,i);}
fs->bitmap = bm;
return fs;
}

internal i8* filestr(Filename*n){
if (!n || !*n->name) return (i8*)0;
if (!*n->ext){
return n->name;
}

static i8 buff[13];
strcopy(n->name,buff);
buff[len(n->name)] = '.';
strcopy(&(buff[len(n->name)+1]),n->ext);
return buff;
}

public Filesystem *FileDescriptors[MAX_FS];


internal Filesystem* fsmount(i8 drive){
if (drive > MAX_FS)return (Filesystem*)0;
Disk * dd = DiskDescriptor[drive-1];
if (!dd) return (Filesystem*)0;
Filesystem*fs = (Filesystem*)alloc(sizeof(Filesystem));
if (!fs) return (Filesystem*)0;
fs->drive = drive;
fs->dd = dd;
fs->bitmap = mkbitmap(fs,1);
if (!fs->bitmap){
    dealloc(fs);
    return (Filesystem*)0;
}

if (!dread(dd,(i8*)&fs->metadata,1)){
dealloc(fs);
return (Filesystem*)0;
}
kprintf("Mounted disk 0x%02x on drive %c:",fs->drive,(drive == 1)?'C':(drive == 2) ? 'D':'?');
FileDescriptors[drive-1] = fs;
i16 p = inode_alloc(fs);
p = inode_alloc(fs);
fsshow(fs);
return fs;
}

internal void fsunmount(Filesystem*fs){
if (!fs) return;
FileDescriptors[fs->drive-1] = (Filesystem*)0;
kprintf("Unmounted disk 0x%x on drive %c:",fs->drive,(fs->drive == 1)?'C':(fs->drive == 2) ? 'D':'?');
dealloc(fs);
}

// internal ptr create_inode(Filesystem* fs,Filename* name,Type t){
// if (!fs || !name || t & InvalidType) return 0;
// Inode *inode;
// } 
// internal i8 destroy_inode(Filesystem* fs,ptr p){}


internal ptr inode_alloc(Filesystem* fs){
if (!fs) return 0;
ptr idx = 0,n;
Inode *p = (Inode*)alloc(sizeof(Inode));
zero(p,sizeof(p));
i16 t = Inodesperblock * fs->metadata.inodeblocks;
for (n = 0;n<t;n++){
p = fetchinode(fs,n);
if (!(p->validtype & 0x01)) 
{   
    idx = n;
    p->validtype=1;
    if (!save_inode(fs,p,idx)) {idx = 0;break;}
    fs->metadata.inodes++;
    dwrite(fs->dd,&fs->metadata,1);

    break;
}
}
if (!p) dealloc(p);
return idx;
}

internal i8 inode_dealloc(Filesystem*fs,ptr p){
if (!fs) return 0;
Inode * inode = fetchinode(fs,p);
if (!inode) return 0; 
inode->validtype = InvalidType;
ptr b = save_inode(fs,inode,p);
if (!b){dealloc(inode);return 0;}
return 1;
}

internal ptr save_inode(Filesystem* fs,Inode* inode,ptr idx){
if (!fs || !inode) return 0;
ptr bno = ((ptr)idx/16)+2;
ptr mod = idx % 16;
FSblock bl;
if (!dread(fs->dd,&bl,bno)) {return 0;}
memcopy(&bl.inodes[mod],inode,sizeof(Inode));
print_bytes(&bl.inodes,512);
if (!dwrite(fs->dd,&bl.inodes,bno)) return 0;

return bno;
}

//TODO: Implement these two
internal i16 openfiles(Disk * dd){
return 0;
}
internal void closeallfiles(Disk * dd){
return;
}
