#include <linux/module.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <mach/gpio.h>
#include <mach/regs-gpio.h>
#include <plat/gpio-cfg.h>
#include <linux/platform_device.h>
#include <linux/delay.h>

#include <asm/io.h>
#include <asm/uaccess.h>
#include <linux/kernel.h>
#include <linux/ioport.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/version.h>

#include <linux/ioctl.h>

#define MY_IOCTL_NUMBER 100
#define MY_IOCTL_READ   _IOR(MY_IOCTL_NUMBER,0,int)
#define MY_IOCTL_WRITE  _IOW(MY_IOCTL_NUMBER,1,int)
#define MY_IOCTL_STATUS _IO(MY_IOCTL_NUMBER,2)
#define MY_IOCTL_READ_WRITE     _IOWR(MY_IOCTL_NUMBER,3,int)
//

#define IOM_FPGA_STEP_MOTOR_MAJOR 267           // ioboard led device major number
#define IOM_FPGA_STEP_MOTOR_NAME "fpga_step_motor"              // ioboard led device name

#define IOM_FPGA_STEP_MOTOR_ADDRESS 0x0400000C // pysical address
#define IOM_FPGA_DEMO_ADDRESS 0x04000300 

#define UON 0x00 // IOM
#define UOFF 0x01 // IOM
//Global variable
static int fpga_step_motor_port_usage = 0;
static unsigned char *iom_fpga_step_motor_addr;
static unsigned char *iom_demo_addr;

// define functions...
ssize_t iom_fpga_step_motor_write(struct file *inode, const char *gdata, size_t length, loff_t *off_what);
int iom_fpga_step_motor_open(struct inode *minode, struct file *mfile);
int iom_fpga_step_motor_release(struct inode *minode, struct file *mfile);
int iom_fpga_step_motor_ioctl(struct inode *inode, struct file *filp, unsigned int cmd, unsigned char *arg);


// define file_operations structure 
struct file_operations iom_fpga_step_motor_fops =
{
        owner:          THIS_MODULE,
        open:           iom_fpga_step_motor_open,
        write:          iom_fpga_step_motor_write,
        release:        iom_fpga_step_motor_release,
};
// when fpga_step_motor device open ,call this function
int iom_fpga_step_motor_open(struct inode *minode, struct file *mfile)
{
        if(fpga_step_motor_port_usage != 0) return -EBUSY;

        fpga_step_motor_port_usage = 1;


        return 0;
}

// when fpga_step_motor device close ,call this function
int iom_fpga_step_motor_release(struct inode *minode, struct file *mfile)
{
        fpga_step_motor_port_usage = 0;

        return 0;
}
// when write to fpga_step_motor device  ,call this function
ssize_t iom_fpga_step_motor_write(struct file *inode, const char *gdata, size_t length, loff_t *off_what)
{
        int i;

        unsigned char value[3];
        const char *tmp = gdata;

        if (copy_from_user(&value, tmp, length))
                return -EFAULT;

        for(i=0;i<length;i++)
                outb(value[i],(unsigned int)iom_fpga_step_motor_addr+i);

        return length;
}

//
int iom_fpga_step_motor_ioctl(struct inode *inode, struct file *filp,  unsigned int cmd, unsigned char *arg)
{

   unsigned char my_arr[3];

   switch(cmd) {
        case MY_IOCTL_WRITE:
           copy_from_user(&my_arr, arg, 3);
           break;
        }

   int i;
   for(i=0;i<3;i++)
   {
      outb(my_arr[i], (unsigned int)iom_fpga_step_motor_addr+i);
   }

   return 0;
}
//
int __init iom_fpga_step_motor_init(void)
{
        int result;

        result = register_chrdev(IOM_FPGA_STEP_MOTOR_MAJOR, IOM_FPGA_STEP_MOTOR_NAME, &iom_fpga_step_motor_fops);
        if(result < 0) {
                printk(KERN_WARNING"Can't get any major\n");
                return result;
        }

        iom_fpga_step_motor_addr = ioremap(IOM_FPGA_STEP_MOTOR_ADDRESS, 0x4);
        iom_demo_addr = ioremap(IOM_FPGA_DEMO_ADDRESS, 0x1);

        outb(UON,(unsigned int)iom_demo_addr);

        printk("init module, %s major number : %d\n", IOM_FPGA_STEP_MOTOR_NAME, IOM_FPGA_STEP_MOTOR_MAJOR);

        return 0;
}
void __exit iom_fpga_step_motor_exit(void)
{
        iounmap(iom_fpga_step_motor_addr);
        iounmap(iom_demo_addr);

        unregister_chrdev(IOM_FPGA_STEP_MOTOR_MAJOR, IOM_FPGA_STEP_MOTOR_NAME);
}

module_init(iom_fpga_step_motor_init);
module_exit(iom_fpga_step_motor_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Huins");
