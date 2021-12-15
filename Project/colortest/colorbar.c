#include <stdio.h>
#include <stdlib.h>     // for exit
#include <unistd.h>     // for open/close
#include <fcntl.h>      // for O_RDWR
#include <sys/ioctl.h>  // for ioctl
#include <sys/mman.h>
#include <linux/fb.h>   // for fb_var_screeninfo, FBIOGET_VSCREENINFO

#define FBDEV_FILE "/dev/fb0"
#define DES_size_x 500
#define BALL_size_x 100
#define WIDTH 51
#define HIGH 30

void Ball_DP(int x1, int y1, unsigned long *ptr, unsigned char *fb_mapped, int screen_width, int screen_height);
void Back_ground(int COLOR, unsigned long *ptr, unsigned char *fb_mapped, int screen_width, int screen_height);
void hurdle_0(int X1, int Y1, unsigned long *ptr, unsigned char *fb_mapped, int screen_width, int screen_height);
void hurdle_1(int X1, int Y1, unsigned long *ptr, unsigned char *fb_mapped, int screen_width, int screen_height);
void DES(int X1, int Y1, unsigned long *ptr, unsigned char *fb_mapped, int screen_width, int screen_height);

int main (int argc, char **argv)
{
    int screen_width;
    int screen_height;
    int bits_per_pixel;
    int line_length;

    int fb_fd;
    struct  fb_var_screeninfo fbvar;
    struct  fb_fix_screeninfo fbfix;
    unsigned char   *fb_mapped;
    int mem_size_x;
    unsigned long *ptr;
    int coor_y;
    int coor_x;

    printf("===================================\n");
    printf("Frame buffer Application - ColorBar\n");
    printf("===================================\n");

    if( access(FBDEV_FILE, F_OK) ) {
        printf("%s: access error\n", FBDEV_FILE);
        exit(1);
    }

    if( (fb_fd = open(FBDEV_FILE, O_RDWR)) < 0) {
        printf("%s: open error\n", FBDEV_FILE);
        exit(1);
    }

    if( ioctl(fb_fd, FBIOGET_VSCREENINFO, &fbvar) ) {
        printf("%s: ioctl error - FBIOGET_VSCREENINFO \n", FBDEV_FILE);
        exit(1);
    }

    if( ioctl(fb_fd, FBIOGET_FSCREENINFO, &fbfix) ) {
        printf("%s: ioctl error - FBIOGET_FSCREENINFO \n", FBDEV_FILE);
        exit(1);
    }

    screen_width    =   fbvar.xres;
    screen_height   =   fbvar.yres;
    bits_per_pixel  =   fbvar.bits_per_pixel;
    line_length     =   fbfix.line_length;

    printf("screen_width : %d\n", screen_width);
    printf("screen_height : %d\n", screen_height);
    printf("bits_per_pixel : %d\n", bits_per_pixel);
    printf("line_length : %d\n", line_length);

    mem_size_x    =   screen_width * screen_height * 4;
    fb_mapped   =   (unsigned char *)mmap(0, mem_size_x,
                     PROT_READ|PROT_WRITE, MAP_SHARED, fb_fd, 0);

    if (fb_mapped < 0) {
        printf("mmap error!\n");
        exit(1);
    }
    while (1){
        int i = 0;
        Back_ground(0x000000, ptr, fb_mapped, screen_width, screen_height);
        for (i = 100; i < 300; i++) {
            int y1 = i;
            int x1 = i + 100;
            hurdle_0(300,400, ptr, fb_mapped, screen_width, screen_height); //R
            hurdle_1(400,100, ptr, fb_mapped, screen_width, screen_height); //G
            DES(200,200, ptr, fb_mapped, screen_width, screen_height);      //B
            Ball_DP(x1,y1, ptr, fb_mapped, screen_width, screen_height);    //W
        }
        for (i = 400; i < 800; i++) {
            int x1 = i;
            hurdle_0(300,400, ptr, fb_mapped, screen_width, screen_height); //R
            hurdle_1(400,100, ptr, fb_mapped, screen_width, screen_height); //G
            DES(200,200, ptr, fb_mapped, screen_width, screen_height);      //B
            Ball_DP(x1,300, ptr, fb_mapped, screen_width, screen_height);   //W
        }
        for (i = 300; i < 600; i++) {
            int y1 = i;
            hurdle_0(300,400, ptr, fb_mapped, screen_width, screen_height); //R
            hurdle_1(400,100, ptr, fb_mapped, screen_width, screen_height); //G
            DES(200,200, ptr, fb_mapped, screen_width, screen_height);      //B
            Ball_DP(800,y1, ptr, fb_mapped, screen_width, screen_height);   //W
        }
        usleep(1);
    }
    

    

    //카메라 상에서는 혜성처럼 꼬리가 생김. 실제로는 어떻게 작동하는지 검증이 필요함.
    munmap( fb_mapped, mem_size_x);
    close( fb_fd);
    return 0;
}
void Back_ground(int COLOR, unsigned long *ptr, unsigned char *fb_mapped, int screen_width, int screen_height){
    int coor_y, coor_x;
    for(coor_y = 0; coor_y < screen_height; coor_y++) {
        ptr =   (unsigned long *)fb_mapped + screen_width * coor_y;
        for(coor_x = 0; coor_x < screen_width; coor_x++) {
            *ptr++  =  COLOR;
        }
    }
}

void Ball_DP(int X1, int Y1, unsigned long *ptr, unsigned char *fb_mapped, int screen_width, int screen_height) {
    int size_x = 100;
    int size_y = 100;
    int X2 = X1 + size_x;
    int Y2 = Y1 - size_y;
    int coor_y, coor_x;
    for (coor_y = 0; coor_y < Y1; coor_y++)
    {
        ptr = (unsigned long *)fb_mapped + screen_width * coor_y;
        for (coor_x = 0; coor_x < X1; coor_x++)
        {
            *ptr++ = 0x000000;
        }
        for (coor_x = X1; coor_x < X2; coor_x++)
        {
            *ptr++ = 0xFFFFFF;
        }
        for (coor_x = X2; coor_x < screen_width; coor_x++)
        {
            *ptr++ = 0x000000;
        }
    }
    for (coor_y = 0; coor_y < Y2 + 1; coor_y++)
    {
        ptr = (unsigned long *)fb_mapped + screen_width * coor_y;
        for (coor_x = 0; coor_x < X1; coor_x++)
        {
            *ptr++ = 0x000000;
        }
        for (coor_x = X1; coor_x < X2; coor_x++)
        {
            *ptr++ = 0x000000;
        }
        for (coor_x = X2; coor_x < screen_width; coor_x++)
        {
            *ptr++ = 0x000000;
        }
    }
    usleep(3000);
}

void hurdle_0(int X1, int Y1, unsigned long *ptr, unsigned char *fb_mapped, int screen_width, int screen_height){
    int size_x = 200;
    int size_y = 100;
    int X2 = X1 + size_x;
    int Y2 = Y1 - size_y;
    int coor_y, coor_x;

    for(coor_y = 0; coor_y < Y1; coor_y++) {
        ptr =   (unsigned long*)fb_mapped + screen_width * coor_y;
        for (coor_x = 0; coor_x < X1; coor_x++) {
            *ptr++  =   0x000000;
        }
        for (coor_x = X1; coor_x < X2; coor_x++) {
            *ptr++  =   0xFF0000;
        }
        for (coor_x = X2; coor_x < screen_width; coor_x++) {
            *ptr++  =   0x000000;
        }
    }
    for(coor_y = 0; coor_y < Y2; coor_y++) {
        ptr =   (unsigned long*)fb_mapped + screen_width * coor_y;
        for (coor_x = 0; coor_x < X1; coor_x++) {
            *ptr++  =   0x000000;
        }
        for (coor_x = X1; coor_x < X2; coor_x++) {
            *ptr++  =   0x000000;
        }
        for (coor_x = X2; coor_x < screen_width; coor_x++) {
            *ptr++  =   0x000000;
        }
    }
}

void hurdle_1(int X1, int Y1, unsigned long *ptr, unsigned char *fb_mapped, int screen_width, int screen_height){
    int size_x = 300;
    int size_y = 100;
    int X2 = X1 + size_x;
    int Y2 = Y1 - size_y;
    int coor_y, coor_x;

    for(coor_y = 0; coor_y < Y1; coor_y++) {
        ptr =   (unsigned long*)fb_mapped + screen_width * coor_y;
        for (coor_x = 0; coor_x < X1; coor_x++) {
            *ptr++  =   0x000000;
        }
        for (coor_x = X1; coor_x < X2; coor_x++) {
            *ptr++  =   0x00FF00;
        }
        for (coor_x = X2; coor_x < screen_width; coor_x++) {
            *ptr++  =   0x000000;
        }
    }
    for(coor_y = 0; coor_y < Y2; coor_y++) {
        ptr =   (unsigned long*)fb_mapped + screen_width * coor_y;
        for (coor_x = 0; coor_x < X1; coor_x++) {
            *ptr++  =   0x000000;
        }
        for (coor_x = X1; coor_x < X2; coor_x++) {
            *ptr++  =   0x000000;
        }
        for (coor_x = X2; coor_x < screen_width; coor_x++) {
            *ptr++  =   0x000000;
        }
    }
}

void DES(int X1, int Y1, unsigned long *ptr, unsigned char *fb_mapped, int screen_width, int screen_height){
    int size_x = 100;
    int size_y = 100;
    int X2 = X1 + size_x;
    int Y2 = Y1 - size_y;
    int coor_y, coor_x;

    for(coor_y = 0; coor_y < Y1; coor_y++) {
        ptr =   (unsigned long*)fb_mapped + screen_width * coor_y;
        for (coor_x = 0; coor_x < X1; coor_x++) {
            *ptr++  =   0x000000;
        }
        for (coor_x = X1; coor_x < X2; coor_x++) {
            *ptr++  =   0x0000FF;
        }
        for (coor_x = X2; coor_x < screen_width; coor_x++) {
            *ptr++  =   0x000000;
        }
    }
    for(coor_y = 0; coor_y < Y2; coor_y++) {
        ptr =   (unsigned long*)fb_mapped + screen_width * coor_y;
        for (coor_x = 0; coor_x < X1; coor_x++) {
            *ptr++  =   0x000000;
        }
        for (coor_x = X1; coor_x < X2; coor_x++) {
            *ptr++  =   0x000000;
        }
        for (coor_x = X2; coor_x < screen_width; coor_x++) {
            *ptr++  =   0x000000;
        }
    }
}