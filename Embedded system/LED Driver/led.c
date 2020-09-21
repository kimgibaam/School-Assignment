#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char **argv)
{
    int led_fd;
    int get_number = 0;
    unsigned char val[] = {0x70, 0xB0, 0xD0, 0xE0, 0x00, 0xF0};

    // 숫자 순서대로 LED가 동작할수 있게 나열하였음
    unsigned char my_val[] = {0xF0,0x70,0xB0,0x30,0xD0,0x50,0x90,0x10,0xE0,0x60,0xA0,0x20,0xC0,0x40,0x80,0x00};

//   반복실행을 위해 어규먼트는 받지 않음
 //   if(argc != 2) {  // 실행 어규먼트를 받았는지 체크한다.
 //       printf("Usage : %s [Number]\n",argv[0]);
 //       return -1;
 //   }


    led_fd = open("/dev/led_driver", O_RDWR); // 디바이스를 오픈한다.
    if (led_fd<0){  // 만약 디바이스가 정상적으로 오픈되지 않으면 오류 처리후 종
료한다.
        printf("LED Driver Open Failured!\n");
        return -1;
    }

   while(1)
   {
        scanf("%x",&get_number);   // 16진수 입력

        if(get_number == -1)      // -1일 경우 종료
           break;

        if(get_number>=0&&get_number<=15)  // 숫자가 0~16 사이
        {
            write(led_fd,&my_val[get_number],sizeof(unsigned char));
        }
        else     
           printf("Invalid Value : 0 thru F\n");  // 포함되지 않으면, 메시지를 >출력한다.
    }
    close(led_fd);  // 장치를 닫아준다.

    return 0; // 프로그램을 종료한다.
}
