/* Ethernet.c */
#include "Ethernet.h"

int main(int argc,char * argv[]){
Mac * mac = mkmac((i8*)"12AF:461C:DDEF");
show_mac(mac,1);

free(mac);
return 0;
}

public void show_mac(Mac * m,i8 x){
if (x == 0){
for (int i = 0;i < 6;i++){
if (i && !(i & 1)){
printf(".");
}
printf("%.02X",m->addr[i]);
}
printf("\n");}
else if (x == 1){
for (int i = 0;i < 6;i++){
if (i) printf(":");
printf("%.02X",m->addr[i]);
}
printf("\n");   
}
}

public i8* mac2strf(Mac * mac){
if (!mac) return (i8*)0;
i8 * buff = (i8*)malloc(16);
if (!buff) return (i8*)0;
i8 k = 0;
for (int i = 0 ; i < 6;i++){
i8* t = ascii2hex(mac->addr[i]);
buff[k++] = t[0];
buff[k++] = t[1];
if (k > 16) continue;
if (k % 3) buff[k++] = ':';
}
return buff;
}

public i8* mac2str(Mac * mac){
if (!mac) return (i8*)0;
i8 * ret = (i8*)malloc(20);
if (!ret) return (i8*)0;
zero(ret,20);
snprintf((char *)ret,19,"%.03hhu%.03hhu%.03hhu%.03hhu%.03hhu%.03hhu",mac->addr[0],mac->addr[1],mac->addr[2],mac->addr[3],mac->addr[4],mac->addr[5]);
return ret;
}

public Mac * to_mac(i64 arg){
Mac * mac = (Mac *)malloc(sizeof(Mac));
if (!mac) return (Mac*)0;
mac->addr[0] = arg & 0x00000000000000ff;
mac->addr[1] = arg & 0x000000000000ff00;
mac->addr[2] = arg & 0x0000000000ff0000;
mac->addr[3] = arg & 0x00000000ff000000;
mac->addr[4] = arg & 0x000000ff00000000;
mac->addr[5] = arg & 0x0000ff0000000000;
return mac;
}

public Mac* to_macs(i8* str){
if (!*str) return (Mac*)0;
Mac * mac = (Mac *)malloc(sizeof(Mac));
i8 k = 0,j = 0;
i8 buff[3] = {0};
if (!mac) return (Mac*)0;
for (int i = 0 ;str[i];i++){
if (str[i] == ':' || str[i] == '.') continue;
buff[j++] = str[i];
if (j == 2){
mac->addr[k++] = hex2ascii(buff);
j = buff[0] = buff[1] = buff[2] = 0;}
}
if (k != 6) {return (Mac*)0;}
return mac;
}

public i8 hex2ascii(i8* s){
i8 t = len(s) - 1;
i8 ret=0,k=1;
for (int i = t; i > -1 ;i--){
if (s[i] >= 'A' && s[i] <= 'F'){ret += ((s[i] -'A'+10) * k);}
else if(s[i] >= 'a' && s[i] <= 'f'){ret += ((s[i] -'a'+10) * k);}
else{ ret += ((s[i] - '0') * k);}
k *= 16;
}
return ret;
}

public i8* ascii2hex(i8 x){
if (!x) return (i8*)0;
i8* ret = (i8*)malloc(3);
*ret = *(ret+1) = *(ret+2) = 0;
if (!ret) return (i8*)0;
while (x > 16) {
i8 rem = x % 16;
if (rem > 9) ret[1] = 'A' + (rem-10);
else ret[1] = '0' + rem;
x /= 16;
}
if (x > 9) ret[0] = 'A' + (x-10);
else ret[0] = x + '0';
return ret;
}

public i8* eval_ether(Ethernet * ether){
if (!ether) return (i8*)0;
Raw_Ethernet *raw;
i8* ret = (i8*)malloc(sizeof(Raw_Ethernet));
if (!ret) return (i8*)0;
raw = (Raw_Ethernet*)ret;
raw->dst = ether->dst;
raw->src = ether->src;
raw->ethtype = ether->protocol;
return ret;
}