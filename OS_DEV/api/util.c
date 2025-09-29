#include <stdio.h>
#include <string.h>
#include "disk.h"
#include "fs.h"

typedef unsigned char i8;
typedef unsigned short i16;

void usage_format(i8 * str){
fprintf(stderr,"Usage: %s format [-s] <drive>\n",str,str);
exit(-1);
}

void usage(i8 * str){
fprintf(stderr,"Usage: %s <command> [arguments]\n",str);
exit(-1);
}

void cmd_format(i8 * s1,i8 * s2){
i8 drive,*drivestr,bootable;
if (!s1) usage_format("diskutil");
else if (!s2){bootable = 0;drivestr = s1;}
else{ if(s1[0] == '-' && s1[1]=='s'){
    bootable = 1;
    drivestr = s2;
}else{
    usage_format("diskutil");
}
}

drive = (drivestr[0] == 'c' || drivestr[0] == 'C')? 1 :(drivestr[0] == 'd' || drivestr[0] == 'D') ? 2 : -1;
if (drive == -1 ) usage_format("diskutil");
if (bootable) {fprintf(stderr,"Boot not supported\n");exit(-1);}
i8 force;
printf("This will format the Disk %s erase all data.Proceed?[y/N]\n",drivestr);
scanf("%c",&force);
if (force != 'y' && force != 'Y') return;
i8 iforce =  (force == 'y' || force == 'Y' )? 1 : 0;


printf("Formatting Disk %s...\n",drivestr);
dinit();
Disk *d = attach(drive);
if (!d) {printf("Bad Disk\n");exit(-1);}
Filesystem * fs = fsformat(d,0,iforce);
if (!fs) {printf("Formatting Error\n");exit(-1);}
printf("Disk Formatted!\n");
fsshow(fs);
print_inodes(fs);
print_bitmap(fs);
detach(d);
free(fs);
return;
}



int main(int argc,char * argv[]){
    char * s1,*s2;
if (argc < 2) usage_format(argv[0]);
else if (argc == 2) s1 = s2 = 0;
else if (argc == 3){
s1 = argv[2];
s2 = 0;
}else{
    s1 = argv[2];
    s2 = argv[3];
}
i8* cmd = argv[1];
if (!strcmp(cmd,"format")) cmd_format(s1,s2);
else usage(argv[1]);


return 0;
}