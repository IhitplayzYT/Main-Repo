/* VigiFile.c */
#include "../include/VigiFile.h"
#include <stdio.h>
#include <string.h>
#include <sys/inotify.h>
#include <unistd.h>


void _zero(void *p ,int k){
  int z = 0;
  for (;k>=0;k--,p++) *(i8 *)p = z;
}


void freeall(void * x,...){
va_list args;
va_start(args,x);
void * z = x;
while (z){
  dealloc(z);
  z = va_arg(args,void*);
} 
va_end(args); 
return;
}

int init(int* wfd,int * fds,CLI * cli){
*wfd = inotify_init1(IN_NONBLOCK);
if (*wfd < 0) return *wfd;
int cnt = 0;
for (char ** p = cli->includes;*p;p+=1){
  fds[cnt++] = inotify_add_watch(*wfd, *p,IN_ACCESS |IN_ATTRIB |IN_CLOSE_WRITE |IN_CREATE |IN_DELETE |IN_DELETE_SELF |IN_MODIFY);  
  if (fds[cnt] < 0) perror("Failed to add a file to wfd");
}



  
return 1; 
}

int deinit(){
  return 1;
}

//  ./VigiFile -I=x -E=y jhibcuhb hgbisdvhiu
CLI* arg_parse(int argc, char **argv){
if (argc < 2) return 0;
CLI * ret = (CLI *)alloc(sizeof(CLI));
if (!ret) return 0;
zero(ret->includes, MAX_LEN * MAX_WORD_LEN);
zero(ret->excludes, MAX_LEN * MAX_WORD_LEN);
int m = 0,n = 0;
for (int i = 1; i < argc;i++){
  char * p = 0,*q = 0;
  int flag = 0;
  p = strstr(argv[i],"-I=");
  if (p && p - argv[i] == 0){
    flag = 1;
  }
  q = strstr(argv[i],"-E=");
  if (q && q - argv[i] == 0){
    flag = 2;
  }

  if (flag){
      if (flag == 1) {p += 3;
      strncpy(ret->includes[m++], p,MAX_WORD_LEN);
      }
      else {q += 3;
      strncpy(ret->excludes[n++], q,MAX_WORD_LEN);
      }
  }
}

return ret;
}