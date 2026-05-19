/* VigiFile.c */
#include "../include/VigiFile.h"
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <sys/inotify.h>
#include <unistd.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <fcntl.h>
volatile sig_atomic_t exit_f = 0;
void _zero(i8 *p ,int k){
  int z = 0;
  for (;k>=0;k--,p++) *p = z;
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

int init(int* wfd,int fds[MAX_LEN][MAX_WORD_LEN],CLI * cli){
    struct sigaction sa = {0};
    sa.sa_handler = sigint_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;

    if (sigaction(SIGINT, &sa, NULL) == -1)
    {
        perror("sigaction");
        return 1;
    }

*wfd = inotify_init1(IN_NONBLOCK);
if (*wfd < 0) return *wfd;
int cnt = 0;

if (cli){
for (char ** p = (char **)cli->includes;*p;p+=1){
  *fds[cnt++] = inotify_add_watch(*wfd, *p,BITMASK);  
  if (*fds[cnt] < 0) perror("Failed to add a file to wfd");
}
}

recurse_add(wfd,fds,&cnt);
return 1; 
}

int deinit(int * wfd,int ** fds, CLI * clargs){
    close(*wfd);
    for(int i = 0 ; i < MAX_FDS && *fds[i] ; i++) close(*fds[i]);
    FREE(clargs,*fds,*wfd);
  return 1;
}

//  ./VigiFile -I=x -E=y jhibcuhb hgbisdvhiu
CLI* arg_parse(int argc, char **argv){
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

int find(char * needle,char haystack[MAX_LEN][MAX_WORD_LEN]){
  for (int i = 0 ;i < MAX_LEN;i++) if (strcmp(needle, haystack[i]) == 0) return 1;
return 0;
}

void recurse_add(int* wfd, int fds[MAX_LEN][MAX_WORD_LEN], int *cur)
{
    Queue q;
    queue_init(&q);
    enqueue(&q, strdup("."));
    while (!queue_empty(&q))
    {
        char *current = dequeue(&q);

      printf("xxx\n");
        int wd = inotify_add_watch(
            *wfd,
            current,
            BITMASK
        );


        if (wd >= 0)
        {
            (*fds)[(*cur)++] = wd;
        }

        DIR *dir = opendir(current);

        if (!dir)
        {
            free(current);
            continue;
        }

        struct dirent *entry;

        while ((entry = readdir(dir)))
        {

        printf("yyy\n");
            if (!strcmp(entry->d_name, ".") ||
                !strcmp(entry->d_name, ".."))
            {
                continue;
            }

            struct stat st;

            if (fstatat(
                    dirfd(dir),
                    entry->d_name,
                    &st,
                    AT_SYMLINK_NOFOLLOW
                ) == -1)
            {
                continue;
            }

            if (S_ISDIR(st.st_mode))
            {
                char fullpath[MAX_PATH_LEN];

                snprintf(
                    fullpath,
                    sizeof(fullpath),
                    "%s/%s",
                    current,
                    entry->d_name
                );

                enqueue(
                    &q,
                    strdup(fullpath)
                );
            }
        }

        closedir(dir);
        free(current);
    }

    queue_destroy(&q);
}

void sigint_handler(int signo)
{
    (void)signo;
    exit_f = 1;
}


void parse_event(struct inotify_event *event)
{

    if (!event) return;

    printf("[MASK=0x%x COOKIE=%u] ",event->mask,event->cookie);

    if (event->len > 0) printf("NAME=%s ", event->name);
    printf("\n");

    if (event->mask & IN_ACCESS)
    {
        printf("IN_ACCESS: file accessed\n");
    }

    if (event->mask & IN_ATTRIB)
    {
        printf("IN_ATTRIB: metadata changed\n");
    }

    if (event->mask & IN_CLOSE_WRITE)
    {
        printf("IN_CLOSE_WRITE: writable file closed\n");
    }

    if (event->mask & IN_CLOSE_NOWRITE)
    {
        printf("IN_CLOSE_NOWRITE: non-writable file closed\n");
    }

    if (event->mask & IN_OPEN)
    {
        printf("IN_OPEN: file opened\n");
    }

    if (event->mask & IN_CREATE)
    {
        printf("IN_CREATE: file/directory created\n");
    }

    if (event->mask & IN_DELETE)
    {
        printf("IN_DELETE: file/directory deleted\n");
    }

    if (event->mask & IN_DELETE_SELF)
    {
        printf("IN_DELETE_SELF: watched object deleted\n");
    }

    if (event->mask & IN_MODIFY)
    {
        printf("IN_MODIFY: file modified\n");
    }

    if (event->mask & IN_MOVE_SELF)
    {
        printf("IN_MOVE_SELF: watched object moved\n");
    }

    if (event->mask & IN_MOVED_FROM)
    {
        printf("IN_MOVED_FROM: moved out (cookie=%u)\n",
               event->cookie);
    }

    if (event->mask & IN_MOVED_TO)
    {
        printf("IN_MOVED_TO: moved in (cookie=%u)\n",
               event->cookie);
    }

    if (event->mask & IN_IGNORED)
    {
        printf("IN_IGNORED: watch removed\n");
    }

    if (event->mask & IN_Q_OVERFLOW)
    {
        printf("IN_Q_OVERFLOW: event queue overflow\n");
    }

    if (event->mask & IN_UNMOUNT)
    {
        printf("IN_UNMOUNT: filesystem unmounted\n");
    }

    if (event->mask & IN_ISDIR)
    {
        printf("TARGET_TYPE: directory\n");
    }
    else
    {
        printf("TARGET_TYPE: file\n");
    }
}