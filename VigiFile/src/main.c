#include "../include/VigiFile.h"
#include <signal.h>
#include <stdio.h>
#include <sys/inotify.h>
#include <unistd.h>

extern volatile sig_atomic_t exit_f;

int main(int argc, char *argv[]) { 
    CLI * clargs = arg_parse(argc,argv);
    int wfd;
    char fds[MAX_LEN][MAX_WORD_LEN];
    printf("herllo\n");
    if (init(&wfd,fds,clargs) < 0){
        return EINIT;
    }
    

    while(exit_f){
    char buff[BUFSIZ];
    int l = read(wfd,buff,BUFSIZ);
    if (l <= 0 ) continue;
        int offset = 0;
        while(offset < l){
            struct  inotify_event * events = (struct inotify_event *)(buff + offset);
            parse_event(events);
            offset = sizeof(struct inotify_event) + events->len;
        }
    }

    if (deinit(&wfd,fds,clargs)){
        return EDEINIT;
    }


    return ESUCC; 

}