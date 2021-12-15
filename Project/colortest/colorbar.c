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
    }/*
    //DES DP
    for (size_t i = 0; i < coor_Des[2]; i++) {
        for(int FLASHING_y = 0; FLASHING_y < screen_height; FLASHING_y++) {
            ptr0 =   (unsigned long *)fb_mapped0 + screen_width * coor_y;
            for(int FLASHING_x = 0; FLASHING_x < screen_width; FLASHING_x++) {
                *ptr0++  =   0x000000;
            }
        }

        for(int coor_y = i - DES_SIZE; coor_y < i; coor_y++) {
            ptr0 =   (unsigned long *)fb_mapped0 + screen_width * coor_y;
            for (int coor_x = coor_Des[0] - DES_SIZE; coor_x < coor_Des[0]; coor_x++) {
                *ptr0++ = 0xFFFFFF;
            }
        }
        usleep(10000);
    }*/
    /*
    for(coor_y = 0; coor_y < screen_height/2; coor_y++)
    {
        ptr0 =   (unsigned long *)fb_mapped0 + screen_width * coor_y;
        for(coor_x = 0; coor_x < screen_width; coor_x++)
        {
            *ptr0++  =   0xFF0000;
        }
    }

    for(coor_y = screen_height/2; coor_y < screen_height; coor_y++)
    {
        ptr1 =   (unsigned long *)fb_mapped1 + screen_width * coor_y;
        for(coor_x = 0; coor_x < screen_width; coor_x++)
        {
            *ptr1++  =   0x0000FF;
        }
    }
*/
    /*
    int offsety1 = screen_height * 3 / 5;
    int offsety2 = screen_height * 4 / 5;
    // color bar
    int offsetx1 = screen_width /7 ;
    for(coor_y = 0; coor_y < offsety1; coor_y++)
    {
        ptr =   (unsigned long*)fb_mapped + screen_width * coor_y;
        for (coor_x = 0; coor_x < offsetx1; coor_x++)
        {
            *ptr++  =   0xFFFFFF;
        }
        for (coor_x = offsetx1; coor_x < offsetx1*2; coor_x++)
        {
            *ptr++  =   0xFFE146;
        }
        for (coor_x = (offsetx1*2); coor_x < offsetx1*3; coor_x++)
        {
            *ptr++  =   0x00D7FF;
        }
        for (coor_x = (offsetx1*3); coor_x < offsetx1*4; coor_x++)
        {
            *ptr++  =   0x006400;
        }
        for (coor_x = (offsetx1*4); coor_x < offsetx1*5; coor_x++)
        {
            *ptr++  =   0xE65AE6;
        }
        for (coor_x = (offsetx1*5); coor_x < offsetx1*6; coor_x++)
        {
            *ptr++  =   0xFF0000;
        }
        for (coor_x = (offsetx1*6); coor_x < screen_width; coor_x++)
        {
            *ptr++  =   0x0000FF;
        }
    }
    for(coor_y = offsety1; coor_y < offsety2; coor_y++)
    {
        ptr =   (unsigned long*)fb_mapped + screen_width * coor_y;
        for (coor_x = 0; coor_x < offsetx1; coor_x++)
        {
            *ptr++  =   0x0000FF;
        }
        for (coor_x = offsetx1; coor_x < offsetx1*2; coor_x++)
        {
            *ptr++  =   0x000000;
        }
        for (coor_x = (offsetx1*2); coor_x < offsetx1*3; coor_x++)
        {
            *ptr++  =   0xE65AE6;
        }
        for (coor_x = (offsetx1*3); coor_x < offsetx1*4; coor_x++)
        {
            *ptr++  =   0x000000;
        }
        for (coor_x = (offsetx1*4); coor_x < offsetx1*5; coor_x++)
        {
            *ptr++  =   0x00D7FF;
        }
        for (coor_x = (offsetx1*5); coor_x < offsetx1*6; coor_x++)
        {
            *ptr++  =   0x000000;
        }
        for (coor_x = (offsetx1*6); coor_x < screen_width; coor_x++)
        {
            *ptr++  =   0xFFFFFF;
        }
    }
    offsetx1 = screen_width/4; 
    for(coor_y = offsety2; coor_y < screen_height ; coor_y++)
    {
        ptr =   (unsigned long*)fb_mapped + screen_width * coor_y;
        for (coor_x = 0; coor_x < offsetx1; coor_x++)
        {
            *ptr++  =   0xA0A0A0;
        }
        for (coor_x = offsetx1; coor_x < offsetx1*2; coor_x++)
        {
            *ptr++  =   0xFFFFFF;
        }
        for (coor_x = offsetx1*2; coor_x < offsetx1*3; coor_x++)
        {
            *ptr++  =   0x14148C;
        }
        for (coor_x = offsetx1*3; coor_x < screen_width; coor_x++)
        {
            *ptr++  =   0x000000;
        }
    }
    */

    munmap( fb_mapped0, mem_size);
    munmap( fb_mapped1, mem_size);
    close( fb_fd_0);
    close( fb_fd_1);
    return 0;
}
