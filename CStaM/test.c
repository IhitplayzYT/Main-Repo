#include <stdio.h>
struct Point {
    int x,y;
};


async struct Point * a1(){
    printf("In 1");
    
}
async struct Point * a2(){
    printf("In 2");

}
async struct Point * a3(){
    printf("In 3");

}

async void hello_async(char ** z) {
    a1();
    a2();
    a3();
}

