/*  fs.h  */
#pragma once
/*Includes*/
#include "stdoslib.h"
#include "api.h"
#include "disk.h"
/*Includes*/

/* Typedefinations */
typedef unsigned char i8;
typedef unsigned short i16;
typedef unsigned int i32;
typedef unsigned long i64;
/* Typedefinations */

/* MACROS */
#define public __attribute__((visibility("default")))
#define internal __attribute__((visibility("hidden")))
#define private static
/* MACROS */

/* Definations */
#define IP "127.0.0.1"
#define PORT 80
#define ERR_STR "Error %d : %s\n"
#define magic_str1 (0xfade)
#define magic_str2 (0xaa55)
#define Inodesperblock (16)
#define IndirPtrsperInode (8)
#define PtrsperBlock (256)
#define MAX_FS MAX_DD
/* Definations */
typedef i16 ptr;
typedef i8 Bootsector[500];
internal packed enum e_type{InvalidType=0x00,FileType=0x01,DirType=0x03};
typedef enum e_type Type;
typedef i8 Path;

typedef i8 Bitmap;

/* Superblock 512B */
internal struct s_sb{
    Bootsector boot;
    i16 _;
    i16 blocks; 
    i16 inodes;
    i16 inodeblocks;
    i16 magic[2];
}packed;

typedef struct s_sb Suprblk;

/* FileSystem */
internal struct s_fs{
    i8 drive;
    Disk * dd;
    i8 * bitmap;
    Suprblk metadata;
}packed;

typedef struct s_fs Filesystem;

internal struct fname{
i8 name[8];
i8 ext[3];
}packed;
typedef struct fname Filename;

/* Inode 32B */
internal struct s_inode{
i8 validtype;
i16 size;
Filename name;
ptr indirect;
ptr direct[IndirPtrsperInode];
}packed;
typedef struct s_inode Inode;

/* FSBLOCK(Block pointed to by indirect ptr) */
internal union u_block{
    Suprblk super;
    i8 data[BLOCK_SIZE];
    ptr ptrs[PtrsperBlock];
    Inode inodes[Inodesperblock];
}packed;
typedef union u_block FSblock;

internal struct s_file_info{
i16 size;
ptr idx;
}packed;
typedef struct s_file_info File_stat;

#ifndef omsdhwr
#define omsdhwr
extern public Filesystem* FileDescriptors[MAX_FS];
#endif

/* Function Signatures */
int main(int, char **);
internal Filesystem *fsformat(Disk *,Bootsector*,i8);
internal Bitmap* mkbitmap(Filesystem*,i8);
internal  i16 allocbitmap(Filesystem*,Bitmap*);
internal void  freebitmap(Filesystem*,Bitmap*,i16);
internal i16 openfiles(Disk *);
internal void closeallfiles(Disk*);
internal void fsshow(Filesystem*);
internal Inode* fetchinode(Filesystem*,ptr);
internal i8* filestr(Filename*);
internal void print_inodes(Filesystem *);
internal void print_bitmap(Filesystem*);
internal Filesystem* fsmount(i8);
internal void fsunmount(Filesystem*);
internal ptr create_inode(Filesystem*,Filename*,Type); 
#define destroy_inode(f,p) inode_dealloc((Filesystem*)fs,(ptr)p);
internal ptr inode_alloc(Filesystem*);
internal i8 inode_dealloc(Filesystem*,ptr);
internal ptr save_inode(Filesystem*,Inode*,ptr);
internal ptr save_inode(Filesystem*,Inode*,ptr);
internal i8 validfname(Filename*,Type);
internal File_stat* fsstat(Filesystem*,ptr);
internal void fstatshow(File_stat*);
internal Filename * parse_name(i8*);
/* Function Signatures */
