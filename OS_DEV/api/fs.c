/*  fs.h  */
#include "fs.h"

public Filesystem *FileDescriptors[MAX_FS];


internal Bitmap* mkbitmap(Filesystem*fs,i8 scan){
FSblock block;
i16 idx = 0;
if (!fs){seterr(BAD_ARG); return(Bitmap*)0;}
i16 x = ceil_div(fs->dd->blocks,8);
Bitmap *map = (Bitmap *)alloc(x);
if (!map){seterr(MEM_ERR);return(Bitmap*)0;}
if (!scan) {zero(map,x);return map;}

for (int i = 2;i <= fs->metadata.inodeblocks+1;i++){ 
    if (!dread(fs->dd,(i8 *)&block,i)) {
        dealloc(map);
        seterr(DISK_IO_ERR);
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
if (!fs){seterr(BAD_ARG);return;}
i16 m = fs->metadata.inodeblocks * Inodesperblock;
for (int i = 0 ; i < m ;i++){
    Inode * inode = fetchinode(fs,i);
    if (inode && inode->validtype & 0x01){
        printf("----Inode---- %d:\nPath: %s\nSize: %d\nInode Type: %s\n",(i+1),concat("/",filestr(&inode->name)),inode->size,(inode->validtype == FileType)?"File":(inode->validtype == DirType)?"Directory":"????");
        printf("--------\n");
    }
}
return;
}

internal Inode* fetchinode(Filesystem*fs,ptr idx){
if (!fs) {seterr(BAD_ARG);return (Inode *)0;}
FSblock buff;
Inode * ret = (Inode*)alloc(sizeof(Inode));
if (!ret) {seterr(MEM_ERR);return (Inode*)0;}
i16 bl = (i16)idx/Inodesperblock;
i16 jump = idx % Inodesperblock;
if (!dread(fs->dd,(i8*)&buff.data,bl+2)) {seterr(DISK_IO_ERR);return ret;}
memcopy((i8*)ret,(i8*)&(buff.inodes[jump]),sizeof(Inode));
return ret;
}


internal void fsshow(Filesystem*fs){
if (!fs) {seterr(BAD_ARG);return;}
printf("-----FileSystem-----\nDrive- \\%c\nPath: %s\nBlockSize= %d\nMagicString (0x%.04x) (0x%.04x)\nNo of Blocks: [%d]\nNo of Inodes-> %d\nNo of Inode Blocks->[%d]\n--------------------\n",(fs->drive == 1) ? 'C':(fs->drive == 2)?'D':'?',numppend(Basepath,fs->drive),fs->dd->blocks,fs->metadata.magic[0],fs->metadata.magic[1],fs->metadata.blocks,fs->metadata.inodes,fs->metadata.inodeblocks);
}

internal void print_bitmap(Filesystem*fs){
if (!fs) {seterr(BAD_ARG);return;}
print_bytes((void *)fs->bitmap,ceil_div(fs->dd->blocks,8));
}

internal i16 allocbitmap(Filesystem*fs,Bitmap * bmp){
int i;
if (!bmp || !fs) {seterr(BAD_ARG);return 0;}
for (i = 0 ;i < fs->dd->blocks;i++) if (!getbit((i8*)bmp,i)) {setbit((i8*)bmp,i);return i+1;}
return 1;
}

internal void  freebitmap(Filesystem*fs,Bitmap*bmp,i16 x){
if (!bmp || !fs) {seterr(BAD_ARG);return;}
unsetbit((i8*)bmp,x-1);
return;}

internal Filesystem *fsformat(Disk *dd,Bootsector* mbr,i8 force){
if (!dd){seterr(NOT_MOUNT_ERR);return (Filesystem*)0;}
if (openfiles(dd)){
    if (!force) {seterr(BUSY_ERR);return (Filesystem*)0;}
    else closeallfiles(dd);
}
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
if (!bm){return (Filesystem*)0;}
i16 size = 1+1+fs->metadata.inodeblocks;
for (int i = 0 ;i < size;i++) setbit((i8*)bm,i);
fs->bitmap = bm;
ptr idx1 = create_inode(fs,parse_name("hello.txt"),FileType);
ptr idx2 = create_inode(fs,parse_name("Dir"),DirType);

print_inodes(fs);

inode_dealloc(fs,idx1);
inode_dealloc(fs,idx2);
return fs;
}

internal i8* filestr(Filename*n){
if (!n || !*n->name){seterr(BAD_FILE_NAME);return (i8*)0;};
if (!*n->ext){return n->name;}
static i8 buff[13];
strcopy(buff,n->name);
buff[len(n->name)] = '.';
strcopy(buff + len(n->name)+1 ,n->ext);
return buff;
}

internal Filename * parse_name(i8* str){
if (!str) {seterr(BAD_ARG);return (Filename*)0;}
Filename * fname = (Filename*)alloc(sizeof(Filename));
if (!fname){seterr(MEM_ERR);return (Filename*)0;}
i8 flag = 0;
i8 m,n;
m = n = 0;
for (int i = 0 ; str[i] != '\0';i++){
if (str[i] == '.'){flag = 1;continue;}
if (flag){
if (n == 3){break;}
fname->ext[n++] = str[i];
}
else{
if (m == 8){break;}
fname->name[m++] = str[i];  
}
}
return fname;
}

internal Filesystem* fsmount(i8 drive){
if (drive > MAX_FS){seterr(LIMIT_ERR);return (Filesystem*)0;}
Disk * dd = DiskDescriptor[drive-1];
if (!dd) {seterr(MEM_ERR);return (Filesystem*)0;}
Filesystem*fs = (Filesystem*)alloc(sizeof(Filesystem));
if (!fs) {seterr(MEM_ERR);return (Filesystem*)0;}
fs->drive = drive;
fs->dd = dd;
fs->bitmap = mkbitmap(fs,1);
if (!fs->bitmap){
    dealloc(fs);
    return (Filesystem*)0;
}

if (!dread(dd,(i8*)&fs->metadata,1)){
seterr(DISK_IO_ERR);
dealloc(fs);
return (Filesystem*)0;
}
kprintf("Mounted disk 0x%02x on drive %c:",fs->drive,(drive == 1)?'C':(drive == 2) ? 'D':'?');
FileDescriptors[drive-1] = fs;
return fs;
}

internal void fsunmount(Filesystem*fs){
if (!fs) {seterr(BAD_ARG);return;}
FileDescriptors[fs->drive-1] = (Filesystem*)0;
kprintf("Unmounted disk 0x%x on drive %c:",fs->drive,(fs->drive == 1)?'C':(fs->drive == 2) ? 'D':'?');
dealloc(fs);
}

internal i8 validfname(Filename* name,Type t){
if (!name || !t) return 0;
return 1;
}


// TODO:  MAKE SURE BITMAP ALSO GETS UPDAGTED HERE 
internal ptr create_inode(Filesystem* fs,Filename* name,Type t){
if (!fs) {seterr(BAD_ARG);return 0;};
if (!validfname(name,t)) reterr(BAD_FILE_NAME);
ptr idx = inode_alloc(fs);
if (!idx) {seterr(INODE_ERR);return 0;}
Inode * ino = (Inode*)alloc(sizeof(Inode));
if (!ino){seterr(MEM_ERR);return 0;}
if (!ino) {seterr(INODE_ERR);return 0;}
zero(ino,sizeof(Inode));

ino->validtype = t;
ino->size = 0;
memcopy(&(ino->name),name,sizeof(Filename));
if (!save_inode(fs,ino,idx)){seterr(INODE_ERR);dealloc(ino);return 0;}

return idx;
} 



internal ptr inode_alloc(Filesystem* fs){
if (!fs){seterr(BAD_ARG);return 0;}
ptr n,idx = 0;
Inode *p = (Inode*)alloc(sizeof(Inode));
if (!p){seterr(MEM_ERR);return 0;}
zero(p,sizeof(p));
i16 t = Inodesperblock * fs->metadata.inodeblocks;
for (n = 0;n<t;n++){
p = fetchinode(fs,n);
if (!p){seterr(INODE_ERR);return 0;}
if (!(p->validtype & 0x01)) 
{   idx = n;
    p->validtype=0x01;
    if (!save_inode(fs,p,idx)) {idx = 0;seterr(INODE_ERR);break;}
    fs->metadata.inodes++;
    dwrite(fs->dd,&fs->metadata,1);
    break;
}
}
if (!p) dealloc(p);
return idx;
}


internal ptr save_inode(Filesystem* fs,Inode* inode,ptr idx){
if (!fs || !inode) {seterr(BAD_ARG);return 0;}
ptr bno = ((ptr)idx/16)+2;
ptr mod = idx % 16;
i8 buff[512];
if (!dread(fs->dd,buff,bno)) {seterr(DISK_IO_ERR);return 0;}
memcopy(&buff[sizeof(Inode)*mod],inode,sizeof(Inode));
if (!dwrite(fs->dd,buff,bno)) {seterr(DISK_IO_ERR);return 0;}
return bno;
}

internal i8 inode_dealloc(Filesystem*fs,ptr p){
if (!fs) {seterr(BAD_ARG);return 0;}
Inode * inode = fetchinode(fs,p);
if (!inode) {seterr(INODE_ERR);return 0;}
inode->validtype = InvalidType;
ptr b = save_inode(fs,inode,p);
if (!b){dealloc(inode);seterr(INODE_ERR);return 0;}
return 1;
}

internal File_stat* fsstat(Filesystem* fs,ptr p){
if (!fs) {seterr(BAD_ARG);return (File_stat*)0;}
Inode * ino = fetchinode(fs,p);
if (!ino){seterr(MEM_ERR);return 0;}
File_stat * fstat = (File_stat*)alloc(sizeof(File_stat));
if (!fstat){seterr(MEM_ERR);dealloc(ino);return 0;}
fstat->idx = p;
fstat->size = ino->size;
dealloc(ino);
return fstat;
}

// TODO: TO MAKE IT COMPATIBLE WITH PATH
internal void fstatshow(File_stat* fst){
if (!fst){seterr(BAD_ARG);return;}
printf("-----FILE-----\nSize: %d\nInode num: %d\n-----------\n",fst->size,fst->idx);
}

internal i8* eval_path(i8* path){
if (!path) return '~';
i32 l = len(path);
i8** stack = (i8**)alloc(l+1);
if (!stack) return '~';
for (int i = 0 ; i <= l;i++) stack[i] = alloc(sizeof(i8)*MAX_FILE_NAME);

/*USE TOKENISE*/

for (int i = 0 ; i <= l ;i ++){
dealloc(stack[i]);
}
dealloc(stack);
}

//TODO: Implement these two
internal i16 openfiles(Disk * dd){
return 0;
}
internal void closeallfiles(Disk * dd){
return;
}

