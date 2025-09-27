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

/* Definations */
typedef i16 ptr;
typedef i8 Bootsector[500];
internal packed enum {InvalidType=0x00,FileType=0x01,DirType=0x03};
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



/* Function Signatures */
int main(int, char **);
public Filesystem *fsformat(Disk *,Bootsector*,i8);
internal Bitmap* mkbitmap(Filesystem*,i8);
internal  i16 allocbitmap(Filesystem*,Bitmap*);
internal void  freebitmap(Filesystem*,Bitmap*,i16);
internal i16 openfiles(Disk *);
internal void closeallfiles(Disk*);

/* Function Signatures */
