/* cmnd.c */
#include "cmnd.h"

int main(int argc, char *argv[]) { 
    init();
    char c = read_byte(0);
    printf("%c\n",c);
    print_err();
    }
