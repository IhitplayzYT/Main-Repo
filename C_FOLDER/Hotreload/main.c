#include "Hotreload.h"

int main(int argc, char *argv[]) {
  void *handle = dlopen("./shared.so", RTLD_NOW);
  void (*print_fxn)() = dlsym(handle, "print_global_varib");
  print_fxn();
  _load_i("global_varib", 723682, "./shared.so");
  _load_f("flt_glob_var", 673283.32, "./shared.so");
  print_fxn();
  return 0;
}
