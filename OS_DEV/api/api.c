/* api.c */

#define ERR_GLOBAL
#include "api.h"

public i8 err_number = 0;
public i8 INIT_FLAG = 0;
public char err[][25] = {"No Errors","Bad/Invalid FD","I/O Error","Closed FD","Init not called","Buffer Overflow","Disk not mounted","Disk IO Error","Unclosed/Busy File","Memory Error","Invalid Argument(s)","Invalid File/Dir Name","Invalid Inode","Limit Reached","Network Error","Invalid Directory","Mismatched Types","Path Not Found","Unable to Open Dir/File"};

public i8 latest_Err_idx = 0;
public i8 ERR_BUFFER[ERR_BUF_SIZE];


private i8 fds[MAX_FD];
/* 
fd[0] -> stderr
fd[1] -> stdin
fd[2] -> stdout
fd[...] -> file stream FDs
*/

/* Return : 1 -> Open FD, 0 -> Closed FD, NO_INIT_ERR */
private i8 isopen(i8 fd)
{struct stat _;
   if (!INIT_FLAG) reterr(NO_INIT_ERR);
    if (fd < 3) return 0;
    i8 psxfd = getposixfd(fd);
    if (!psxfd)return 0;
    if  (fstat(psxfd,&_) == -1) return 0;
    return 1;
}

/* Return : 0 -> Sucessful write to fd , else Err_number*/
public i8 write_byte(i8 fd, i8 chr)
{ if (!INIT_FLAG) reterr(NO_INIT_ERR);
    if (fd < 0) reterr(BAD_FD);
    if (fd > 1  && !isopen(fd)) reterr(CLOSED_FD);
    i8 buff[2] = {0};
    buff[0] = chr;
    i8 psxfd = getposixfd(fd);
    if (!psxfd) reterr(CLOSED_FD);
    psxfd = (psxfd == 1) ? 0 : (psxfd == 2) ? 1: (psxfd);
if (!write(psxfd,buff,1)) reterr(IO_ERR);
return 0;
}


/* Return : 0 -> Sucessful read from fd , else Err_number*/
public i8 read_byte(i8 fd) {
if (!INIT_FLAG) reterr(NO_INIT_ERR);
if (fd < 0) reterr(BAD_FD);
if (fd > 2 && !isopen(fd)) reterr(CLOSED_FD);
char buff[2] = {0};
    i8 psxfd = getposixfd(fd);
    if (!psxfd) reterr(CLOSED_FD);
    psxfd = (psxfd == 1) ? 0 : (psxfd == 2) ? 1: (psxfd);
if (!read(psxfd,buff,1)) reterr(IO_ERR);
return (i8)*buff;
}


private void init_fds(){
fds[0] = 1;
fds[1] = 2;
for (int i = 2;i < MAX_FD ;i ++)fds[i] = 0;
}

public void print_err(){
printf(ERR_STR,err_number,err[err_number]);
}

public void init(){
init_fds();
err_number = 0;
INIT_FLAG = 1;
dinit();
}

public void print_err_buff(){
printf("-----Errors-----\n");
int c;
for (int i = latest_Err_idx,c = 0;c<ERR_BUF_SIZE;i++,c++){
if (!ERR_BUFFER[i]) printf("Err %d : [%d] : %s\n",i+1,ERR_BUFFER[i % ERR_BUF_SIZE],err[ERR_BUFFER[i]]);
}
} 


public i8 get_errno(){return err_number;}

public Dir *open_dir(i8* str){
if (!str){seterr(BAD_ARG);return (Dir*)0;}
Dir * dir = (Dir*)alloc(sizeof(Dir));
if (!dir) {seterr(MEM_ERR);return (Dir*)0;}
Path * path = init_path(str,(Filesystem*)0);
if (!path){seterr(PATH_ERR);return (Dir*)0;}
if (!*path->inter->ret[0]) return (Dir*)0;
Inode * ino = fetchinode(path->fs,1);
if (!ino) {dealloc(path);seterr(INODE_ERR);return (Dir*)0;}  
ptr x;
ptr dirptr = 1;
for (i16 i = 0 ; i < path->inter->n;i++){
x = read_dir(path->fs,dirptr,parse_name(path->inter->ret[i]));
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
dir->name = path->target;
dir->idx = x;
File_entry * filelist = init_filelist(dir->fs,ino);


// dir->entrys
return dir;
}


// Fix this Complete this..
public File_entry* init_filelist(Filesystem* fs,Inode* ino){
i16 c = 0;
File_entry * entry,*filelist,arr[MAX_FILE_PER_DIR];
if (!fs || !ino){seterr(BAD_ARG);return(File_entry*)0;} 
Inode *inode;
for (int i = 0 ; i < DirectPtrsperInode;i++){
inode = fetchinode(fs,ino->direct[i]);
if (!inode)continue;
c++;
entry = &arr[c-1];
entry->idx = inode;
entry->name = &inode->name;
entry->size = inode->size;
entry->filetype = inode->validtype;

}
if (!ino->direct) return 
inode = 
for (int i = 0 ; i < PtrsperBlock)


}

