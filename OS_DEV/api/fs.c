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
i16 bl = floor_div(idx,Inodesperblock);
i16 jump = idx % Inodesperblock;
if (!dread(fs->dd,(i8*)&buff.data,bl+2)) {seterr(DISK_IO_ERR);return ret;}
memcopy((i8*)ret,(i8*)&(buff.inodes[jump]),sizeof(Inode));
return ret;
}


internal void fsshow(Filesystem*fs){
if (!fs) {seterr(BAD_ARG);return;}
printf("-----FileSystem-----\nDrive- \\%c\nPath: %s\nBlocks= %d\nMagicString (0x%.04x) (0x%.04x)\nNo of Blocks: [%d]\nNo of Inodes-> %d\nNo of Inode Blocks->[%d]\n--------------------\n",(fs->drive == 1) ? 'C':(fs->drive == 2)?'D':'?',numppend(Basepath,fs->drive),fs->dd->blocks,fs->metadata.magic[0],fs->metadata.magic[1],fs->metadata.blocks,fs->metadata.inodes,fs->metadata.inodeblocks);
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
zero((i8 *)inode.direct,sizeof(ptr) * DirectPtrsperInode);
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
if (!dwrite(dd,(i8 *)&fsb,(i+3))) {dealloc(fs);seterr(DISK_IO_ERR);    
return (Filesystem*)0;}
}
memcopy((i8*)&fs->metadata,(i8*)&super,BLOCK_SIZE);
Bitmap *bm = mkbitmap(fs,0);
if (!bm){return (Filesystem*)0;}
i16 size = 1+1+fs->metadata.inodeblocks;
for (int i = 0 ;i < size;i++) setbit((i8*)bm,i);
fs->bitmap = bm;
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

// Fix SOME ERROR
internal Filename * parse_name(i8* str){
if (!str) {seterr(BAD_ARG);return (Filename*)0;}
Filename * fname = (Filename*)alloc(sizeof(Filename));
if (!fname){seterr(MEM_ERR);return (Filename*)0;}
zero(fname,sizeof(Filename));
if (strcomp(str,"..") == 0) {
memcopy(fname->name,"..",2);
memcopy(fname->ext,NULL,3);
return fname;
}
if (strcomp(str,".") == 0) {
memcopy(fname->name,".",1);
memcopy(fname->ext,NULL,3);
return fname;
}


i8 flag = 0;
i8 m,n;
m = n = 0;
for (int i = 0 ; str[i] != '\0';i++){
if (str[i] == '.' && i != 0){flag = 1;continue;}
if (flag){
if (n == 3) break;
fname->ext[n++] = str[i];
}
else{
if (m == 8) break;
fname->name[m++] = str[i];  
}
}
if (!n) memcopy(fname->ext,NULL,3);

return fname;}

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
if (!name || !t || (t==DirType && *name->ext)) {seterr(BAD_ARG);return 0;}
for (int i = 0; name->name[i] != '\0';i++){
if (!validchar(name->name[i])) {seterr(BAD_FILE_NAME);return 0;}
}

if (t == FileType){
for (int i = 0; name->ext[i] != '\0';i++){
if (!validchar(name->ext[i])) {seterr(BAD_FILE_NAME);return 0;}
}
}
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

// TODO: DECIDE WHETHER TO APPEND HOME/IHITPLAYZYT TO 
internal i8* eval_path(i8* path){
if (!*path) {seterr(BAD_ARG);return "~";}
i32 l = freq(path,(i8)'/');
i8** stack = (i8**)alloc(sizeof(i8*) * (l+1));
stack[l] = (i8*)0;
if (!stack) {seterr(MEM_ERR);return "~";}
i16 top = 0;
for (int i = 0 ; i <= l;i++) stack[i] = alloc(sizeof(i8)*MAX_FILE_NAME);
struct s_Tok_ret *ret = tokenise(path,'/');
for (int i = 0 ; i < ret->n;i++){
if (!*ret->ret[i] || strcomp(ret->ret[i],".") == 0) continue;
else if (strcomp(ret->ret[i],"..") == 0) (top == 0) ? top:--top;
else stack[top++] = ret->ret[i];
}


stack = (i8**)realloc(stack,top * sizeof(i8*));
i8 * simplified_path = (i8*)alloc(sizeof(i8) * MAX_PATH_LEN);
i8 k = 0;
if (!simplified_path) {seterr(MEM_ERR);return "~";}
for (int i = 0 ; i < top;i++){
// if (strcomp(stack[i],"c:") == 0 || strcomp(stack[i],"C:") == 0 || strcomp(stack[i],"d:") == 0 || strcomp(stack[i],"D:") == 0){k += (i16)strcopy(simplified_path + k,stack[i])-1;simplified_path[++k]='/';k++;}
// else if (i == 1 && strcomp(stack[i],"~") != 0) {
//     strcopy(simplified_path+k,"/home/IhitplayzYT/");
//     k += 18;
//     k += strcopy(simplified_path+k,stack[i])-1;
//     simplified_path[++k] = '/';
//     k++;
// }
// else if (i == 1)  k = strcopy(simplified_path+k,"/");
k += (i16)strcopy(simplified_path + k,stack[i])-1;simplified_path[++k]='/';k++;
}
simplified_path[k-1] = '\0';
dealloc(ret);
dealloc(stack);
return simplified_path;
}




internal ptr read_dir(Filesystem* fs,ptr p,Filename* name){
if (!fs || !p || !name) return 0;
if (!validfname(name,DirType)){
seterr(BAD_FILE_NAME);
return 0;
}
Inode * inode = fetchinode(fs,p);
if (!inode) {seterr(INODE_ERR);return 0;}
if (inode->validtype != DirType) {dealloc(inode);seterr(TYPE_ERR);return 0;}
ptr idx;
name = toggle_fname_case(name,0);
for (ptr i = 0 ; i < DirectPtrsperInode;i++) {
    idx = inode->direct[i];
    if (!idx) continue;
    Inode * pointed = fetchinode(fs,idx);
    if (!pointed) continue;
    if (memcomp(name,(i8*)&pointed->name,11) == 0) {
        dealloc(inode);
        dealloc(pointed);
        return idx; 
    }
dealloc(pointed);
}

if (!inode->indirect){dealloc(inode);seterr(INODE_ERR);return 0;}
ptr bno = inode->indirect;
FSblock block;
if (!dread(fs->dd,&block,bno)) {dealloc(inode);seterr(DISK_IO_ERR);return 0;}
for (ptr i = 0 ; i < Inodesperblock;i++) {
    idx = block.ptrs[i];
    if (!idx) continue;
    Inode * pointed = fetchinode(fs,idx);
    if (!pointed) continue;
    if (memcomp(name,(i8*)&pointed->name,11) == 0) {
        dealloc(inode);
        dealloc(pointed);
        return idx; 
    }
dealloc(pointed);
}
dealloc(inode);
return 0;
}

internal Filename* toggle_fname_case(Filename* fn,i8 flag){
if (!fn) {seterr(BAD_ARG);return (Filename*)0;}
Filename * name = (Filename*)alloc(sizeof(Filename));
if (!name) {seterr(MEM_ERR);return (Filename*)0;}
if (flag == 0){
memcopy(name->name,tolwrn(fn->name,8),8);
memcopy(name->ext,tolwrn(fn->ext,3),3);
}
else{
memcopy(name->name,toupprn(fn->name,8),8);
memcopy(name->ext,tolwrn(fn->ext,3),3);
}
return name;
}

internal void show(void * arg,i8* s){
if (!arg) {seterr(BAD_ARG);return;}
if (strcomp(s,"inode") == 0) print_inodes((Filesystem*)arg);
else if (strcomp(s,"bitmap") == 0) print_bitmap((Filesystem*)arg);
else if (strcomp(s,"fstat") == 0) fstatshow((File_stat*)arg);
else if (strcomp(s,"filesystem") == 0) fsshow((Filesystem*)arg);
else if (strcomp(s,"filename") == 0) filename_show((Filename*)arg);
else if (strcomp(s,"path") == 0) show_path((Path*)arg);
else if (strcomp(s,"ls") == 0) show_ls((Ls*)arg);
else {seterr(BAD_ARG);return;}
}

internal void filename_show(Filename* fn){
if (!fn) {seterr(BAD_ARG);return;}
if (*fn->ext)
    printf("Filename: %s.%.3s\n",fn->name,fn->ext);
else
    printf("Dirname: %s\n",fn->name);
}

internal i8 validchar(i8 c){
i8 * p = VALID_VOCAB;
for (;*p;p++){if (*p == c) return 1;}
return 0;
}

///  ///  massive seg fault here

internal Path * init_path(i8* path,Filesystem* fs){
if (!path || !*path){seterr(BAD_ARG);return (Path *)0;}
path = eval_path(path);
Path * ans = (Path*)alloc(sizeof(Path));
if (!ans) {seterr(MEM_ERR);return (Path*)0;}
struct s_Tok_ret * ret = tokenise(path,'/');
if (!ret) {return (Path*)0;}
i8 drive = ret->ret[0][0] - 0x62;
if (fs) ans->fs = fs; 
else
ans->fs = FileDescriptors[drive -1];
ans->target = parse_name(ret->ret[ret->n-1]);
if (strcomp(ans->target,"c:")==0||strcomp(ans->target,"C:")==0||strcomp(ans->target,"d:")==0||strcomp(ans->target,"D:")==0 ) ans->target = parse_name("/");
if (!ans->target){seterr(BAD_FILE_NAME);return(Path*)0;}
ret->ret[--ret->n] = NULL; 
ans->inter = ret;
return ans;
}

internal void show_path(Path* p){
if (!p) return;
if (!*p->target->ext)
printf("----Path----\nDisk: %s\nTarget Dir: %s\n",(p->fs->drive == 1)?"C":(p->fs->drive == 2)?"D":"None",p->target->name);
else
printf("----Path----\nDisk: %s\nTarget File: %s.%.3s\n",(p->fs->drive == 1)?"C":(p->fs->drive == 2)?"D":"None",p->target->name,p->target->ext);
printf("Dirs: [");
for (int i = 0 ; i < p->inter->n;i++){
printf("%s",p->inter->ret[i]);
if (i != p->inter->n-1) printf(", ");
}
printf("]\n-----------\n");
}

internal void show_ls(Ls* ls){
if (!ls) return;
for (i16 i = 0 ;i < ls->count;i++ ){
if(!ls) continue;
if (!ls->arr[i]->name.ext) printf("%d. %s | %s | %d bytes ",i+1,(ls->arr[i]->filetype == DirType)?"Dir":(ls->arr[i]->filetype == FileType)?"File":"None",ls->arr[i]->name.name,ls->arr[i]->size);
else printf("%d. %s | %s.%s | %d bytes ",i+1,(ls->arr[i]->filetype == DirType)?"Dir":(ls->arr[i]->filetype == FileType)?"File":"None",ls->arr[i]->name.name,ls->arr[i]->name.ext,ls->arr[i]->size);
}

}


internal Dir *open_dir(i8* str){
if (!str){seterr(BAD_ARG);return (Dir*)0;}
Dir * dir = (Dir*)alloc(sizeof(Dir));
if (!dir) {seterr(MEM_ERR);return (Dir*)0;}
Path * path = init_path(str,(Filesystem*)0);
if (!path){seterr(PATH_ERR);return (Dir*)0;}
Inode * ino = fetchinode(path->fs,0);
if (!ino) {dealloc(path);seterr(INODE_ERR);return (Dir*)0;}  
ptr x;
ptr dirptr = 1;
for (i16 i = 0 ; i < path->inter->n;i++){
Filename * name = parse_name(path->inter->ret[i]);
if (name) {seterr(BAD_FILE_NAME);return (Dir*)0;}
x = read_dir(path->fs,dirptr,name);
if (x){dealloc(path);dealloc(ino);seterr(PATH_ERR);return (Dir*)0;}
dirptr = x;
}
x = read_dir(path->fs,dirptr,path->target);
if (!x) {dealloc(path);dealloc(ino);seterr(PATH_ERR);return (Dir*)0;}
dirptr = x;
ino = fetchinode(path->fs,dirptr);
if (!ino) {dealloc(path);seterr(FILE_NOT_FOUND);return (Dir*)0;}
if (ino->validtype != DirType){dealloc(ino);dealloc(path);seterr(FILE_NOT_FOUND);return (Dir*)0;}
dir->fs = path->fs;
memcopy(&dir->name,path->target,sizeof(Filename));
dir->idx = x;
Ls * filelist = listfiles(dir->fs,ino);
if (!filelist){seterr(PATH_ERR);dealloc(ino);dealloc(path);return (Dir*)0;}
dir->entrys = filelist;
dealloc(ino);
return dir;
}


internal Ls* listfiles(Filesystem* fs,Inode* dir){
if (!fs || !dir){seterr(BAD_ARG);return(Ls*)0;} 
i16 c = 0;
File_entry arr[MAX_FILE_PER_DIR];
Inode *inode;
FSblock block;
ptr iptr;
for (int i = 0 ; i < DirectPtrsperInode;i++){
iptr = dir->direct[i];
inode = fetchinode(fs,iptr);
if (!inode || !inode->validtype)continue;
c++;
arr[c-1].idx = iptr;
memcopy(&arr[c-1].name,&inode->name,sizeof(Filename));
arr[c-1].size = inode->size;
arr[c-1].filetype = inode->validtype;
free(inode);
}

if (dir->indirect){
zero(&block,BLOCK_SIZE);
iptr = dir->indirect;
if (!dread(fs->dd,&block,iptr)){seterr(DISK_IO_ERR);return (Ls*)0;}
for (int i = 0 ; i < PtrsperBlock;i++){
iptr = block.ptrs[i];
if (!iptr) continue;
inode = fetchinode(fs,iptr);
if (!inode) continue;
c++;
arr[c-1].idx = iptr;
memcopy(&arr[c-1].name,&inode->name,sizeof(Filename));
arr[c-1].size = inode->size;
arr[c-1].filetype = inode->validtype;
free(inode);
}
}
Ls * ret = (Ls*)alloc(sizeof(Ls));
for (int i = 0 ; i < c;i++)ret->arr[i] = (File_entry*)alloc(sizeof(File_entry*));
ret->count = c;
copy(ret->arr,arr,sizeof(File_entry)*(c-1));
return ret;
}


//TODO: Implement these two
internal i16 openfiles(Disk * dd){
return 0;
}
internal void closeallfiles(Disk * dd){
return;
}