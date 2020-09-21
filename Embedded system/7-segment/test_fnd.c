#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h> 

#define FND0    0x00
#define FND1    0x01
#define FND2    0x02
#define FND3    0x03
#define FND4    0x04 
#define FND5    0x05
#define FND6    0x06
#define FND7    0x07
#define FND8    0x08
#define FND9    0x09
#define FNDP    0x0A // DP
#define FNDA    0x0B // ALL 
int main(int argc, char **argv)
{
    int fnd_fd;
    char get_fndnumber;
    char get_number;
    char set_fndvalue;
    char set_value;

    unsigned short temp;
    unsigned char temp1;
    unsigned char temp2;

    if(argc != 2) {
        printf("Usage : %s [Value]\n",argv[0]);
        return -1;
    }

    fnd_fd = open("/dev/fnd_driver", O_WRONLY);
    if (fnd_fd<0){
        printf("FND Driver Open Failed!\n");
        return -1;
    }
 
 int my_num = atoi(argv[1]);
 char arr[4];
 int arr_len = 0;
 while(my_num != 0)
 {
     arr[arr_len] =  (char)(my_num % 10);
     my_num /= 10;
     arr_len += 1;
 }

 int k = 0;
 int idx = arr_len;
 while(k < arr_len)
 {

     set_fndvalue = 0x96;

    switch(arr[idx - 1]) {
        case FND0 :
            set_value = 0x02;
            break;
        case FND1 :
            set_value = 0x9F;
            break;
        case FND2 :
            set_value = 0x25;
            break;
        case FND3 :
            set_value = 0x0D;
            break;
        case FND4 :
            set_value = 0x99;
            break;
        case FND5 :
            set_value = 0x49;
            break;
        case FND6 :
            set_value = 0xC1;
            break;
        case FND7 :
            set_value = 0x1F;
            break;
        case FND8 :
            set_value = 0x01;
            break;
        case FND9 :
            set_value = 0x09;
            break;
    }
    temp1 = set_fndvalue;
    temp2 = set_value;
    temp = temp + temp1;

    printf("%d", temp);

    temp = (temp<<8)|temp2;
    write(fnd_fd,&temp,sizeof(short));

  usleep(500000);
  k++;
  idx--;

 }

   close(fnd_fd);

    return 0;
}

