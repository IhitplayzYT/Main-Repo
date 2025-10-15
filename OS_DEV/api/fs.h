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
    Bootsector boot;   // 500 bytes bootsector code         = 500 bytes
    i16 _;             // Reserved 2 bytes 0x00             = 2 bytes
    i16 blocks;        // No of Blocks                      = 2 bytes
    i16 inodes;        // No of Inodes                      = 2 bytes
    i16 inodeblocks;   // No of Inode Blocks                = 2 bytes
    i16 magic[2];      // Magic string hex {0xfade,0xaa55}  = 4 bytes
}packed;                                                  /*512 Bytes*/

typedef struct s_sb Suprblk;

/* FileSystem */
internal struct s_fs{   
    i8 drive;           // Drive no  (Option TODO: DD can provide driveno)
    Disk * dd;      // Disk descriptor onto which fs mounted
    i8 * bitmap;    // Bitmap to check active andf available inodes
    Suprblk metadata;  // Metadata of superblock 
}packed;

typedef struct s_fs Filesystem;

internal struct fname{   // Support for extentions of max 3 chars 
i8 name[8];     
i8 ext[3];
}packed;
typedef struct fname Filename;

/* Inode 32B */
internal struct s_inode{
i8 validtype;               // Inode Type (Valid/File/Dir)  = 1 byte
i16 size;                   // Size of file pointed by inode = 2 bytes
Filename name;              // 11 byte filename
ptr indirect;               // 2 byte indirect ptr
ptr direct[IndirPtrsperInode];      //  2 * 8 = 16 bytes
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
internal Filesystem *fsformat(Disk *,Bootsector*,i8);   // To format and mount fs onto a disk
internal Bitmap* mkbitmap(Filesystem*,i8);   // To generate bitmap
internal  i16 allocbitmap(Filesystem*,Bitmap*);  // Alloc bitmap
internal void  freebitmap(Filesystem*,Bitmap*,i16);     // Free bitmap
internal i16 openfiles(Disk *);             // YET to IMPLEM TODO:
internal void closeallfiles(Disk*);         // YET to IMPLEM TODO:
internal void fsshow(Filesystem*);          // Details of Filesystem
internal Inode* fetchinode(Filesystem*,ptr);  // Retrieve the ith inode
internal i8* filestr(Filename*);            // Convert s_Filename to filename string
internal void print_inodes(Filesystem *);    // Print all available active inodes
internal void print_bitmap(Filesystem*);       // Prints entire bitmap
internal Filesystem* fsmount(i8);              // Mount the Fs
internal void fsunmount(Filesystem*);           // Unmount the fs;
internal ptr create_inode(Filesystem*,Filename*,Type);  //  Creates an inode
#define destroy_inode(f,p) inode_dealloc((Filesystem*)fs,(ptr)p);   // Destroys a inode
internal ptr inode_alloc(Filesystem*);  // Helper fxn to create_inode
internal i8 inode_dealloc(Filesystem*,ptr);     // Helper fxn to destroy_inode 
internal ptr save_inode(Filesystem*,Inode*,ptr);  // Write thew inode to disk, hekper to create_inode
internal i8 validfname(Filename*,Type);  // Checks validity of Filename struct of Type type
internal File_stat* fsstat(Filesystem*,ptr);     // File_stat struct of a file
internal void fstatshow(File_stat*);      // Displays fstat struct
internal Filename * parse_name(i8*);       // Pareses the filename string to a s_Filename struct 
/* Function Signatures */
