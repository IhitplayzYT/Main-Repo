#include <stdio.h>
struct Point {
    int x,y;
};



async struct Point * a1(struct Point * y){
    printf("In 1");
    
}
async struct Point * a2(struct Point * y){
    printf("In 2");

}
async struct Point * a3(struct Point * y){
    printf("In 3");

} 

async void hello_async(char ** z) {
    struct Point * r1 = a1();
    struct Point * r2 = a2(r1);
    a3(r2);
}