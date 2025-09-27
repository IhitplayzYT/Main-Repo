/* os.h */
#pragma once
/* Defines */
#define public __attribute__((visibility("default")))
#define internal __attribute__((visibility("hidden")))
#define private static
/* Defines */

/* Typedefs */
typedef unsigned char i8;
typedef unsigned short i16;
typedef unsigned int i32;
typedef unsigned long i64;
/* Typedefs */

/* Error Number Implem */
#ifndef ERR_GLOBAL
#define ERR_GLOBAL
extern public i8 err_number;
extern public i8 INIT_FLAG;
#endif


#define reterr(x)     \
    do                \
    {                 \
    err_number = (x); \
    return (x);       \
    } while (0);      \

#define seterr(x) err_number = (x)
/* Error Number Implem */

/* Error Number defs */
public enum{NO_ERR,BAD_FD,IO_ERR,CLOSED_FD,NO_INIT_ERR,BUFF_OVRFLW,NOT_MOUNT_ERR,DISK_IO_ERR,BUSY_ERR,MEM_ERR};
/* Error Number defs */

/*Function Signatures */
public i8 write_byte(i8, i8); /* Writes a singular byte to a fd */
public i8 read_byte(i8); /* Reads a singular byte from a fd */
public int getposixfd(i8); /* Return the posixFD for our FD */
public void init(); /* Initialises some functionalities */
public void dinit(); /* Initialises the disk/drive*/
public i8 get_errno();  /* Returns the Err_Number */
public void print_err();  /* Prints Err_Number with Description */
/*Function Signatures */