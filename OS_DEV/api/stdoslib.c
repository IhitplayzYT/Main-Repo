/* stdoslib.c */
#include "stdoslib.h"

extern void** DEALLOCATOR;
extern i32 ALLOCED_OBJ;

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

public Tuple * Tuple_init(void* _ ,...){
va_list args;
va_start(args,_);
Tuple * t = (Tuple*)alloc(sizeof(Tuple));
if (!t) return(Tuple*)0;
t->sz = 0;
t->cap = 0;
t->add = t_add;
void * x;
while ((x = va_arg(args,void*))){
t->add(t,x);
}
return t;
}

public void t_add(Tuple* t,void* data){

if (((Tuple *)t)->sz == ((Tuple*)t)->cap){
((Tuple*)t)->data = realloc(((Tuple*)t)->data,((Tuple*)t)->cap*2);
((Tuple*)t)->cap = sizeof(Tuple) * 2;
}
((Tuple*)t)->data[((Tuple*)t)->sz++] = data;
}

public void FINALIZE(){
for (int i = ((Tuple*)this)->sz - 1;i >= 0;i--){
    dealloc(((Tuple*)this)->data[i]);
}
dealloc(((Tuple*)this)->data);
}


public i64 ticks_elapsed(){
i32 l,h;
__asm__ volatile(
"rdtsc":"=a"(l),"=d"(h)
);
return ((((i64)h) << 32) | l);
}

public i64 tick_freq(){
i64 b = ticks_elapsed();
sleep(1);
i64 a = ticks_elapsed();
return a - b;
}

public i64 seconds_elapsed(){
return ticks_elapsed() / tick_freq();
}

public Time * curr_time(){
i64 t = seconds_elapsed();
Time * ts = (Time *)alloc(sizeof(Time));
if (!t) return (Time *)0;
ts->minutes = (i16)(t / 60);
ts->hours = (i16)(t / 3600);
ts->seconds = t % 60;
ts->minutes %= 60;
ts->days = (i16)(ts->hours / 24);
ts->hours %= 60;
ts->weeks = (i8)ts->days / 7;
ts->month = (ts->days <= 31)?1:(ts->days <= 59)?2:(ts->days <= 90)?3:(ts->days <= 120)?4:(ts->days <= 151)?5:(ts->days <= 181)?6:(ts->days <= 212)?7:(ts->days <= 243)?8:(ts->days <= 273)?9:(ts->days <= 304)?10:(ts->days <= 334)?11:(ts->days <= 365)?12:0;
ts->weekday = (i8)ts->days % 7;
ts->hours %= 24;
ts->years = (i16)ts->days / 365;
ts->days %= 365;
ts->date_of_month = (ts->days <= 31)?ts->days:(ts->days <= 59)?ts->days - 31:(ts->days <= 90)?ts->days - 59:(ts->days <= 120)?ts->days - 90:(ts->days <= 151)?ts->days - 120:(ts->days <= 181)?6:(ts->days <= 212)?ts->days - 181:(ts->days <= 243)?ts->days - 212:(ts->days <= 273)?ts->days - 243:(ts->days <= 304)?ts->days - 273:(ts->days <= 334)?ts->days - 304:(ts->days <= 365)?ts->days - 334:0;
return ts;
}

public i8* fmttime(Time * ts){
if (!ts) ts = curr_time();
i8* buff = (i8*)alloc(50);
if (!buff) return (i8*)0;
snprintf(buff,49,"%s, %.02hu:%.02hu:%.02hu , %.02hu %s,%.04hu",(ts->weekday == 0)?"Monday":(ts->weekday ==1)?"Tuesday":(ts->weekday == 2)?"Wednesday":(ts->weekday == 3)?"Thursday":(ts->weekday == 4)?"Friday":(ts->weekday == 5)?"Saturday":(ts->weekday == 6)?"Sunday":"INVALID",ts->hours,ts->minutes,ts->seconds,ts->date_of_month,(ts->month == 1)?"January":(ts->month == 2)?"February":(ts->month == 3)?"March":(ts->month == 4)?"April":(ts->month == 5)?"May":(ts->month == 6)?"June":(ts->month == 7)?"July":(ts->month == 8)?"August":(ts->month == 9)?"September":(ts->month == 10)?"October":(ts->month == 11)?"November":(ts->month == 12)?"December":"INVALID",ts->years);
return buff;
}

public i8* strchar(i8* s,i8 ch){
if (!*s) return (i8*)0;
i8* p = s;
while(*p){
if (*p == ch) return p;
p++;
}
return (i8*)0;
}

public s16 strcharidx(i8* s,i8 ch){
if (!*s) return (s16)-1;
i8* p = s;
while(*p){
if (*p == ch) return p - s;
p++;
}
return (s16)-1;
}


public i8* strstrs(i8* s,i8* str){
if (!*s || !*str) return (i8*)0;
i8 * p = s,*q;
while (*p){
q = str;
if (*s == *p){
i8 * m = p,*n = q;
i8 f = 1; 
while (f && *m && *n) {
    if (*m != *n) {f = 0;}
    m++;
    n++;
}
if (f){return p;}
}
p++;
}
return (i8*)0;
}

public s16 strstrsidx(i8* s,i8* str){
if (!*s || !*str) return (s16)-1;
i8 * p = s,*q;
while (*p){
q = str;
if (*s == *p){
i8 * m = p,*n = q;
i8 f = 1; 
while (f && *m && *n) {
    if (*m != *n) {f = 0;}
    m++;
    n++;
}
if (f){return p - s;}
}
p++;
}
return (s16)-1;
}

public i8** tokenise(i8* str,i8 ch){
i8 ** ret = (i8**)malloc(sizeof(i8*));
if (!ret) return (i8**)0;
i8* p = str,*start = p;
i16 k = 0;
while (*p){
if (*p == ch){
ret[k] = (i8*)malloc(p - start+1);
memcopy(ret[k],start,p - start);
ret[k][p-start] = '\0';
k++;
ret = realloc(ret,(k+1)  * sizeof(i8*));

start = ++p;
}
else ++p;
}

if (p != start){
    ret[k] = (i8*)malloc(p - start + 1);
    memcopy(ret[k], start, p - start);
    ret[k][p - start] = '\0';
    k++;
    ret = (i8**)realloc(ret, (k + 1) * sizeof(i8*));
}
ret[k] = (i8*)0;
return ret;
}


public void * _alloc(i32 x){
void * ret = malloc((i32)x);
DEALLOCATOR[ALLOCED_OBJ++] = ret;
return ret;
}

public void FINALISE(){
    for (i32 i = ALLOCED_OBJ-1;i > -1;i--){
        if (DEALLOCATOR[i]) dealloc(DEALLOCATOR[i]);
        else ++i;
    }
}

public void dealloc(void * p){
    for (i32 i = ALLOCED_OBJ-1;i > -1;i--){
    if (DEALLOCATOR[i] == p){
        DEALLOCATOR[i] == NULL;
        break;}
    }
    free(p);
}