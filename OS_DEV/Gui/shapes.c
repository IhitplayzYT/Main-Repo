/* shapes.c */
#include "shapes.h"

void videomode(i8 mode){
    if (mode > 0x9f) return;
    else xvideomode(mode);
    return;
    
}
