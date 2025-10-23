#include "stdoslib.h"

i8 is_even(i32 k){return (k & 1)?0:1;}
init_filter(i32,is_even);
int main() {
i32 arr[] = {1231,4,5,342,3,6,7,3,21,6,78,76};
int x;
i32 * res = i32_is_even_filter(arr,12,&x);
printarr(res,x);
}





