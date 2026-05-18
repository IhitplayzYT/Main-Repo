#include "../include/VigiFile.h"
#include <sys/inotify.h>



int main(int argc, char *argv[]) { 
    CLI * clargs = arg_parse(argc,argv);
    if (!clargs) return EARGS;
    int* fds;
    int wfd;
    int succ = init(&wfd,fds,clargs);
    if (succ < 0 ){
        return EINIT;
    }
    if (deinit()){
        return EDEINIT;
    }
    FREE(clargs);
    return ESUCC; 

}