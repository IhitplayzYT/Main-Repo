/* bmp.h */
#pragma once
#include "gui.h"
#include "shapes.h"


typedef unsigned char i8;
typedef unsigned short i16;
typedef unsigned int i32;
typedef unsigned long i64;

#define public __attribute__((visibility("default")))
#define internal __attribute__((visibility("hidden")))
#define private static
#define packed __attribute__((packed))


struct s_bmp {
i16 sign;
i16 _;
i16 file_size;
i16 __,___;
i16 ____;
i16 offset;
}packed;

typedef struct s_bmp Bmp_header;

struct s_bmp_info{
i16 _;
i16 header_size;
i16 __;
i16 width;
i16 ___;
i16 height;
i16 ____;
i16 _____;
i16 ______,_______;
i16 ________,_________;  
i16 __________,___________;
i16 ____________,_____________; 
i16 ______________,_______________;
} packed;

typedef struct s_bmp_info Bmp_Info_header;

struct s_rgb_color{
    i8 red;
    i8 blue;
    i8 green;
    i8 _;
}packed;

typedef struct s_rgb_color RGB;
typedef RGB Color_table[16];
typedef Point Bmpline[MAX_X];

struct s_bitmap{
i16 x,y;
Bmp_header * header;
Bmp_Info_header * info;
Color_table * color;
Bmpline lines[];
}packed;

typedef struct s_bitmap Bitmap;

Bitmap * parse_bmp(i8*);
void draw_bmp(Bitmap*);