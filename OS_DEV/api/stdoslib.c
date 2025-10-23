/* stdoslib.c */
#include "stdoslib.h"

public void _copyn(i8 *a,i8 *b,i16 n,i8 z){
if (!a || !b) return;
if (z==1) for (;n;n--,a++,b++) *a = *b;
else{
for (;n && *a != '\0' && *b != '\0';n--,a++,b++)*a = *b;
*a = '\0';
}
}

void print_hex(void *str, i32 n) {
i32 i, j;
i8 *data = (i8*)str;
for (i = 0; i < n; i += 16) {
for (j = 0; j < 16; j++) {
    if (i + j < n) printf("0x%02hhX ", data[i + j]);
    else printf("     "); }
printf(" ");
for (j = 0; j < 16 && i + j < n; j++) {
    i8 c = data[i + j];
    printf("%c", (c >= 32 && c <= 126) ? c : '.');}
printf("\n");
}}


public void _copy(i8 *a,i8 *b){
if (!a || !b) return;
for (;*b != '\0';a++,b++) *a = *b;
*a = '\0';
}

public void _fill (i8*a,i16 l,i8 hx){
if (!a) return;
for (;l;a++,l--) *a = hx;
}

public void print_bytes(void *ptr, i32 size) {
    if (!ptr)return;
    i8 *p = ptr;
    for (i32 i = 0; i < size; i++) {
        printf("%02X ", p[i]);   
        if ((i + 1) % 16 == 0)  
            printf("\n");
    }
    printf("\n");
}

public i16 ceil_div(i16 a,i16 b){
return (!a % b) ? (i16)a/b : ((i16)a/b) + 1;
}
public i16 floor_div(i16 a,i16 b){
return (i16)a/b;
}

public i8 _getbit(i8 * a,i16 n){
return (((1 << n) & *a) >> n );
}
public void _setbit(i8 * a,i16 n){
*a |= (1 << n);
}
public void _unsetbit(i8 * a,i16 n){
*a &= (~(1 << n)) ;
}
public void _flipbit(i8 *a,i16 n){
*a ^= (1 << n) ;
}



public i8* concat(i8*a,i8*b){
static i8 buff[512];
strcopy(buff,a);
strcopy(&buff[len(a)],b);
buff[len(buff)] = '\0';
return buff;
}

public i8 getbit(i8 *str,i16 idx){
i16 bytes_move;
i8 bits_move;
bytes_move = (i16)idx/8;
bits_move = idx % 8;
void * mem = (void *)(str+bytes_move);
i8 bit = _getbit((i8*)mem,bits_move);
return bit;
}

public void setbit(i8 *str,i16 idx){
i16 bytes_move;
i8 bits_move;
bytes_move = (i16)idx/8;
bits_move = idx % 8;
void * mem = (void *)(str+bytes_move);
_setbit((i8*)mem,bits_move);
}

public void unsetbit(i8 *str,i16 idx){
i16 bytes_move;
i8 bits_move;
bytes_move = (i16)idx/8;
bits_move = idx % 8;
void * mem = (void *)(str+bytes_move);
_unsetbit((i8*)mem,bits_move);
}

public void flipbit(i8 *str,i16 idx){
i16 bytes_move;
i8 bits_move;
bytes_move = (i16)idx/8;
bits_move = idx % 8;
void * mem = (void *)(str+bytes_move);
_flipbit((i8*)mem,bits_move);
}

public double precision(double x,i8 n){
    i64 t = 1;
    for (int i = 0;i< n;i++) t*= 10;
    i64 l = x * t;
    x = (double)l / t;
return x;
}

public i32 ipaddr(i8* s){
i8 a[4] = {0},*p,c = 0;
i32 ret;
for (p = s;*p;p++){
if (*p == '.' || *p == '-' || *p == ':') c++;
else{
a[c] *= 10;
a[c] += *p - '0';
}}
ret = (a[3] << 24) | (a[2] << 16) | (a[1] << 8) | a[0];
return ret;
}

public i8* ipstr(i32 addr){
i8 *buff = (i8*)malloc(16);
zero(buff,16);
i8 a[4];
a[0] = (addr & 0xff000000) >> 24;
a[1] = (addr & 0x00ff0000) >> 16;
a[2] = (addr & 0x0000ff00) >> 8;
a[3] = (addr & 0x000000ff);
snprintf((char*)buff, 16, "%u.%u.%u.%u", a[3], a[2], a[1], a[0]);
return buff;
}

public i16 endian16(i16 x){
i16 a,b,c;
a = x & 0x00ff;
b = (x & 0xff00) >> 8;
c = (a << 8) | b;
return c;
}

public i32 endian32(i32 x){
i32 a,b,c;
a = endian16(x & 0x0000ffff);
b = endian16((x & 0xffff0000) >> 16);
c = (a << 16) | b;
return c;
}

public i64 endian64(i64 x){
i64 a,b,c;
a = endian32(x & 0x00000000ffffffffULL);
b = endian32((x & 0xffffffff00000000ULL) >> 32);
c = (a << 32) | b;
return c;
}

public i16 net_port(i16 x){
return endian16(x);
}


public Vector * Vector_init(void* _,...){
va_list args;
va_start(args,_);
void * data = va_arg(args,void*);
i32 sz = va_arg(args,i32);

Vector * v = (Vector*)alloc(sizeof(Vector));
if (!v) return (Vector *)0;
v->l = 0;
v->cap = 2;
v->type = sz * 8;
v->append = v_append;
v->pop = v_pop;
v->iterator = iterator;
v->data = (void **)alloc(sizeof(void *) * v->cap);
if (!v->data) {dealloc(v);return (Vector*)0;}
v->data[v->l++] = data;
return v;
}

public void v_append(struct s_vector * v,void * data){
if (v->l < v->cap){
v->data[v->l++] = data;
}else{
v->cap *= 2;
void ** temp = realloc(v->data,v->cap * v->type);
v->data = temp;
v->data[v->l++] = data;
}
}

public void v_pop(struct s_vector * v){
v->l = (v->l)?--v->l:v->l;
v->data[v->l]= NULL;
}

Iterator * iterator(struct s_vector* v){
Iterator * iter = (Iterator *)alloc(sizeof(Iterator));
if (!iter) return (Iterator*)0;
iter->i = 0;
iter->l = v->l;
iter->type = v->type;
iter->next = next;
iter->data = v->data;
return iter;
}

void* next(Iterator* iter){
return (iter->i < iter->l)?iter->data[iter->i++]:(void*)0;
}

public double _pow(double x,int y){
if (x == 0 || x == 1) return x;
if (y == 0) return 1;
double ret = 1;
if (y < 0){
y *= -1;
for (int i = 0 ; i < y; i++) ret *= (double)(1/x);
}
else{
for (int i = 0 ; i < y ; i++) ret *= (double)x;
}
return ret;
}


public i8 _stoi8(i8*s){
i8 l = len(s);
i8 ret = 0;
for (i8 i = 0 ;i < l;i++){
if (s[i] >= '0' && s[i] <= '9') ret += (s[i] - '0') * _pow(10,l-i-1);
else if (s[i] == '+' || s[i] == ',' || s[i] == '_') continue;
else return 0;
}
return ret;
}

public i16 _stoi16(i8*s){
i8 l = len(s);
i16 ret = 0;
for (i8 i = 0 ;i < l;i++){
if (s[i] >= '0' && s[i] <= '9') ret += (s[i] - '0') * _pow(10,l-i-1);
else if (s[i] == '+' || s[i] == ',' || s[i] == '_') continue;
else return 0;
}
return ret;
}

public i32 _stoi32(i8*s){
i8 l = len(s);
i32 ret = 0;
for (i8 i = 0 ;i < l;i++){
if (s[i] >= '0' && s[i] <= '9') ret += (s[i] - '0') * _pow(10,l-i-1);
else if (s[i] == '+' || s[i] == ',' || s[i] == '_') continue;
else return 0;
}
return ret;
}

public i64 _stoi64(i8*s){
i8 l = len(s);
i64 ret = 0;
for (i8 i = 0 ;i < l;i++){
if (s[i] >= '0' && s[i] <= '9') ret += (s[i] - '0') * _pow(10,l-i-1);
else if (s[i] == '+' || s[i] == ',' || s[i] == '_') continue;
else return 0;
}
return ret;
}


public int _stoi(i8*s){
i8 l = len(s);
i8 flag = 0;
int ret = 0;
for (i8 i = 0 ;i < l;i++){
if (s[i]==' ' && !flag){continue;}
else if (s[i] >= '0' && s[i] <= '9') {ret += (s[i] - '0') * _pow(10,l-i-1);flag = 1;}
else if (s[i] == ',' || s[i] == '_'){continue;}
else if (s[i] == '+' && !flag){continue;}
else if (s[i] == '+' && flag){return __INT32_MAX__;}
else if (s[i] == '-' && !flag){ret *= -1;}
else if (s[i] == '-' && flag){return __INT32_MAX__;}
else return 0;
}
return ret;
}







