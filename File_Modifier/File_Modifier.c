/* File_Modifier.c */
#include "File_Modifier.h"

void rand_perm(mode_t *perm) {
  char buff[50];
  snprintf(buff, 50, "07%d%d", rand() % 8, rand() % 8);
  *perm = strtol(buff, NULL, 8);
}

int main(int argc, char *argv[]) {

  srand(time(NULL));
  if (argc != 2) {
    return -1;
  }
  mode_t perm;
  rand_perm(&perm);
  chmod(argv[1], perm);

  time_t t_a, t_m;
  t_a = (time(NULL) % (rand() + 1) - rand()) * (time(NULL) % (rand() + rand()));
  t_m =
      (time(NULL) % (rand() + 1) - rand()) * -1 * (time(NULL) % (829 + rand()));
  struct utimbuf s_utimebuf;
  s_utimebuf.actime = t_a;
  s_utimebuf.modtime = t_m;
  utime(argv[1], &s_utimebuf);
  char file_name[256];

  strcpy(file_name, argv[1]);

  char *ptr = file_name;
  while (ptr != NULL) {
    ptr = strchr(ptr + 1, '.');
    if (strchr(ptr + 1, '.') == NULL) {
      break;
    }
  }

  char ext[4];
  memset(ext, 0, 4);
  snprintf(ext, 4, "%c%c%c", (rand() % 26) + 97, (rand() % 26) + 97,
           (rand() % 26) + 97);
  memcpy(++ptr, ext, 3);
  rename(argv[1], file_name);
  int pid = fork();
  if (pid == 0) {
    int fd = open("/dev/null", O_WRONLY);
    dup2(fd, STDOUT_FILENO);
    close(fd);
    execlp("exiftool", "exiftool", "-ALL", file_name, NULL);
    exit(0);
  }

  wait(NULL);
  printf("\x1b[92mSUCCESS!\x1b[0m\n");
  return 0;
}
