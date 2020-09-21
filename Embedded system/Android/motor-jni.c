#include <android/log.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <jni.h>


void motor_main (int x)
{
    int i;
    int dev;
    int str_size;

    char motor_state[3];

    memset(motor_state,0,sizeof(motor_state));

    motor_state[0]= 1;
    motor_state[1]= x;
    motor_state[2]= 10;
                             // O_WRONLY  O_RDWR
    dev = open("/dev/fpga_step_motor_driver",O_WRONLY);

    write(dev,motor_state,3);
   // ioctl(dev,MY_IOCTL_WRITE,&motor_state);

    close(dev);
}


jstring Java_com_example_stepmotor_MainActivity_ReceiveLedValue( JNIEnv* env,jobject thiz, jint val)
{
	__android_log_print(ANDROID_LOG_INFO, "motorJniExample", "motor value = %d", val);
	motor_main(val);
	return NULL;
}

