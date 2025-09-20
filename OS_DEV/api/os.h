/* os.h */

/* Defines */
#define public __attribute__((visibility("default")))
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
public i8 err_number;

#else
extern public i8 err_number;
#endif


#define reterr(x)     \
    do                \
    {                 \
    err_number = (x); \
    return (x);       \
    } while (0);      \
/* Error Number Implem */

/* Error Number defs */
#define NO_ERR 0
#define BAD_FD 1
#define IO_ERR 2
#define CLOSED_FD 3
#define NO_INIT_ERR 4
/* Error Number defs */

/*Function Signatures */
public i8 write_byte(i8, i8); /* Writes a singular byte to a fd */
public i8 read_byte(i8); /* Reads a singular byte from a fd */
public int getposixfd(i8); /* Return the posixFD for our FD */
public void init(); /* Initialises some functionalities */
public void print_err();
/*Function Signatures */