/* Hotreload.c */
#include "Hotreload.h"

public int _load_f(const char *varib_name, float new_val, const char *fname) {
  if (!fname || !varib_name)
    return 0;
  void *new_handle = dlopen(fname, RTLD_NOW);
  if (!new_handle) {
    fprintf(stderr, "FILE ERR");
    return 0;
  }
  dlerror();
  void *sym = dlsym(new_handle, varib_name);
  if (!sym) {
    fprintf(stderr, "SYMBOL ERR");
    dlclose(new_handle);
    return 0;
  }
  dlerror();
  *(float *)sym = new_val;
  return 1;
}
public int _load_i(const char *varib_name, int new_val, const char *fname) {
  if (!fname || !varib_name)
    return 0;
  void *new_handle = dlopen(fname, RTLD_NOW);
  if (!new_handle) {
    fprintf(stderr, "FILE ERR");
    return 0;
  }
  dlerror();
  void *sym = dlsym(new_handle, varib_name);
  if (!sym) {
    fprintf(stderr, "SYMBOL ERR");
    dlclose(new_handle);
    return 0;
  }
  dlerror();
  *(int *)sym = new_val;
  return 1;
}

public int _load_s(const char *varib_name, char *new_val, const char *fname) {
  if (!fname || !varib_name)
    return 0;
  void *new_handle = dlopen(fname, RTLD_NOW);
  if (!new_handle) {
    fprintf(stderr, "FILE ERR");
    return 0;
  }
  dlerror();
  void *sym = dlsym(new_handle, varib_name);
  if (!sym) {
    fprintf(stderr, "SYMBOL ERR");
    dlclose(new_handle);
    return 0;
  }
  dlerror();
  *(char **)sym = strdup(new_val);
  return 1;
}
