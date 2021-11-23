#include <linux/init.h> // module_init, module_exit 매크로 정의
#include <linux/kernel.h> //  printk 함수 정의
#include <linux/module.h>// 모듈 관련 자료 구조 및 매크로 정의
#include <linux/fs.h> // register_chrdev 함수
#include <asm/uaccess.h> // copy_to_use

#define HELLO_MAJOR_NUM 290
#define HELLO_NAME "hello"

static char array[2000] = {0,};
int how_many_element_in_arr = 0;

static int hello_open(struct inode *node, struct file *pfile) {
    printk("hello_open enter\n");
    return 0;
}

static ssize_t hello_read(struct file *pfile, char __user *pBuff, size_t size, loff_t *filepos) {
    //char arrData[4] = {'1','2','3','4'};
    printk("hello_read enter\n");
    if ( size >= 4) {
        copy_to_user( pBuff, array, 2000 );
        return 4;
    }
    return 0;
}

static ssize_t hello_write(struct file *pfile, const char __user *pBuff, size_t size, loff_t *filepos) {
    //char arrData[4];
    printk("hello_write enter\n");
    if( size >= 4 ) {
        how_many_element_in_arr = size;
        copy_from_user ( array , pBuff , 2000 );
        return 4;
    }
    return 0;
}

static long hello_ioctl(struct file *filp, unsigned int cmd, unsigned long arg) {
    int cnt = 0;
    int k = 0;
    int i = 0;
    
    if (_IOC_TYPE(cmd) != 0x55) {
        printk("Wrong Magic #!\n");
        return -1;
    }
    switch (_IOC_NR(cmd)) {
    case 99://should be 20
        printk("IOCTL Size: %d\n",_IOC_SIZE ( cmd ));
        printk("RX CMD: ");
        
        for(i = 0; i < how_many_element_in_arr; i++) {
            int n = array[i] - 48;
            printk("%d",n);
        }
        printk("\n");
        break;

    case 98://should be 4
        printk("IOCTL Size: %d\n",_IOC_SIZE ( cmd ));
        printk("RX CMD: ");;
        for(i = 0; i < how_many_element_in_arr; i++) {
            int n = array[i] - 48;
            printk("%d",n);
        }
        printk("\n");
        break;

    default:
        printk("RX CMD: ");
        for(i = 0; i < how_many_element_in_arr; i++) {
            int n = array[i] - 48;
            printk("%d",n);
        }
        printk("\n");
        break;
    }
}

int flagRegisterSuccess = 0;

static const struct file_operations hello_fops = {
    .owner = THIS_MODULE,
    .open = hello_open,
    .read = hello_read,
    .write = hello_write,
    .unlocked_ioctl = hello_ioctl
    //.release = hello_release,
};

int __init init_hello(void) {
    int ret;
    printk("Hello world.\n");
    ret = register_chrdev(HELLO_MAJOR_NUM, HELLO_NAME, &hello_fops);

    if ( ret < 0) { 
        printk("Hello registr_chrdev fail\n");
        return ret;
    }

    flagRegisterSuccess = 1;
    return 0;
}

void __exit exit_hello(void){ 
    if ( flagRegisterSuccess) { 
        flagRegisterSuccess = 0;
        unregister_chrdev(HELLO_MAJOR_NUM, HELLO_NAME);
    }

    printk("Good-bye~ \n");
}

module_init(init_hello); //  모듈 로딩시(insmod) 해당 함수 호출
module_exit(exit_hello); //  모듈 제거시(rmmod) 해당 함수 호출

MODULE_LICENSE("GPL");


//git push test
//git push test2
//git push test3
//git push test4