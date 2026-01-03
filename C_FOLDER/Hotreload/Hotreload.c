/* Hotreload.c */
#include "Hotreload.h"
#include <dlfcn.h>
#include "shared.h"

void * handle;
char * so_file;
i8 update_handle(){
    dlclose(handle);
    handle = dlopen(so_file, RTLD_NOW);
    return 1;
}

i8 init_handle(char * file) {
    so_file = (!file  || !*file) ? __FILE__ : file;
    handle = dlopen(so_file,RTLD_NOW);
    return 1;
}
void clean_handle() {
    dlclose(handle);
    handle = NULL;
}
void usage() {
    fprintf(stderr,"Usage : [update/init]_handle -> RELOAD");
    exit(-1);
}
void reload_float(char * f,f64 v){
    if (!handle) {usage();return;}
    void * addr = dlsym(handle,f);
    if (!addr) return;
    *(f64 *)addr = v; 
}
void reload_int(char * f,i64 v){
    if (!handle) {usage();return;}
    void * addr = dlsym(handle,f);
    if (!addr) return;
    *(i64 *)addr = v;
}
void reload_string(char * f ,char * v){
    if (!handle) {usage();return;}
    void * addr = dlsym(handle,f);
    if (!addr) return;
    *(char ** )addr = v;
}
void reload_byte(char * f,byte v){
    if (!handle) {usage();return;}
    void * addr = dlsym(handle,f);
    if (!addr) return;
    *(byte *)addr = v; 
}
void reload_struct(char * f, void * v,size_t l){
    if (!handle) {usage();return;}
    void * addr = dlsym(handle,f); 
    if (!addr) return;
    memcpy(addr,v,l);
}

void reload_sym(char * s) {
    if (!handle) {usage();return;}
    void * addr = dlsym(handle,s); 
}
int main(int argc, char *argv[]) {
    if (argc != 2) return -1;
    init_handle(argv[1]);
    void (*print)(void);
    void * fptr = dlsym(handle,"print_val");
    *(void **)(&print) = fptr;
    while (1) {
    print();
    sleep(1);
    update_handle();
    reload_sym("val");
    }
    dlclose(handle);
return 0;
}
