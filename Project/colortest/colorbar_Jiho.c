// #include "framebuffer.h"

#include <stdio.h>
#include <stdlib.h>     // for exit
#include <unistd.h>     // for open/close
#include <fcntl.h>      // for O_RDWR
#include <sys/ioctl.h>  // for ioctl
#include <sys/mman.h>
#include <linux/fb.h>   // for fb_var_screeninfo, FBIOGET_VSCREENINFO
#include <stdlib.h> // for exit

#define FBDEV_FILE_0 "/dev/fb0"
#define FBDEV_FILE_1 "/dev/fb1"

#define DES_SIZE 500
#define BALL_SIZE 100
#define WIDTH 51
#define HIGH 30

int main (int argc, char **argv)
{
    int screen_width;
    int screen_height;
    int bits_per_pixel;
    int line_length;

    int fb_fd_0;
    int fb_fd_1;
    struct  fb_var_screeninfo fbvar;
    struct  fb_fix_screeninfo fbfix;
    unsigned char   *fb_mapped0;
    unsigned char   *fb_mapped1;
    int mem_size;
    unsigned long   *ptr0;
    unsigned long   *ptr1;
    int coor_y;
    int coor_x;

    printf("===================================\n");
    printf("Frame buffer Application - ColorBar\n");
    printf("===================================\n");

    if( access(FBDEV_FILE_0, F_OK) )
    {
        printf("%s: access error\n", FBDEV_FILE_0);
        exit(1);
    }

    if( (fb_fd_0 = open(FBDEV_FILE_0, O_RDWR)) < 0)
    {
        printf("%s: open error\n", FBDEV_FILE_0);
        exit(1);
    }

    if( ioctl(fb_fd_0, FBIOGET_VSCREENINFO, &fbvar) )
    {
        printf("%s: ioctl error - FBIOGET_VSCREENINFO \n", FBDEV_FILE_0);
        exit(1);
    }

    if( ioctl(fb_fd_0, FBIOGET_FSCREENINFO, &fbfix) )
    {
        printf("%s: ioctl error - FBIOGET_FSCREENINFO \n", FBDEV_FILE_0);
        exit(1);
    }

    if( access(FBDEV_FILE_1, F_OK) )
    {
        printf("%s: access error\n", FBDEV_FILE_1);
        exit(1);
    }

    if( (fb_fd_1 = open(FBDEV_FILE_1, O_RDWR)) < 0)
    {
        printf("%s: open error\n", FBDEV_FILE_1);
        exit(1);
    }

    if( ioctl(fb_fd_1, FBIOGET_VSCREENINFO, &fbvar) )
    {
        printf("%s: ioctl error - FBIOGET_VSCREENINFO \n", FBDEV_FILE_1);
        exit(1);
    }

    if( ioctl(fb_fd_1, FBIOGET_FSCREENINFO, &fbfix) )
    {
        printf("%s: ioctl error - FBIOGET_FSCREENINFO \n", FBDEV_FILE_1);
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
    printf("frame_buffer : %s\n", FBDEV_FILE_0);
    printf("frame_buffer : %s\n", FBDEV_FILE_1);
    printf("____________________\n");

    mem_size    =   screen_width * screen_height * 4;
    fb_mapped0   =   (unsigned char *)mmap(0, mem_size, PROT_READ|PROT_WRITE, MAP_SHARED, fb_fd_0, 0);
    fb_mapped1   =   (unsigned char *)mmap(0, mem_size, PROT_READ|PROT_WRITE, MAP_SHARED, fb_fd_1, 0);
    if (fb_mapped0 < 0 || fb_mapped1 < 0)
    {
        printf("mmap error!\n");
        exit(1);
    }
    if (fb_mapped1 < 0)
    {
        printf("mmap error!\n");
        exit(1);
    }
    int coor_Des[2] = {500,300};
    int coor_Ball[2] = {800,500};

    for(coor_y = 0; coor_y < screen_height; coor_y++) {
        ptr0 =   (unsigned long *)fb_mapped0 + screen_width * coor_y;
        for(coor_x = 0; coor_x < screen_width; coor_x++) {
            *ptr0++  =   0x000000;
        }
    }
    munmap( fb_mapped0, mem_size);
    munmap( fb_mapped1, mem_size);
    close( fb_fd_0);
    close( fb_fd_1);
    return 0;
}
