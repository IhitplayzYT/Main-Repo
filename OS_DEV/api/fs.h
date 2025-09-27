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
#define PtrsperInode (8)
#define PtrsperBlock (256)

/* Definations */
typedef i16 ptr;
typedef i8 Bootsector[500];
internal packed enum {};

internal packed struct s_sb{
    Bootsector boot;
    i16 _;
    i16 blocks;
    i16 inodeblocks;
    i16 inodes;
    i16 magic[2];
};

typedef struct s_sb Suprblk;

internal packed struct s_fs{
    i8 drive;
    Disk * dd;
    i8 * bitmap;
    Suprblk metadata;
};

typedef struct s_fs Filesystem;

internal packed struct fname{
i8 name[8];
i8 ext[3];
};
typedef struct fname Filename;

internal packed struct s_inode{
i8 validtype;
i16 size;
Filename name;
ptr indirect;
ptr direct[PtrsperInode];
};
typedef struct s_inode Inode;

internal packed union u_block{
    Suprblk super;
    i8 data[BLOCK_SIZE];
    ptr ptrs[PtrsperBlock];
    ptr inodes[Inodesperblock];
};
typedef union u_block FSblock;



/* Function Signatures */
int main(int, char **);
internal Filesystem *fsformat(Disk *,Bootsector*);

/* Function Signatures */
