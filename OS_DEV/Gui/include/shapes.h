/* shapes.h */
#pragma once
#include "gui.h"
#include "assembly.h"

struct s_point{
i16 x;
i16 y;
i8 color:5;
} packed;

typedef struct s_point Point;
#define MAX_X 40
#define MAX_Y 25
#define x40_25_T 0

void videomode(i8);