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
    unsigned long   *ptr;
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

    for(coor_y = 0; coor_y < screen_height; coor_y++) {
        ptr =   (unsigned long *)fb_mapped + screen_width * coor_y;
        for(coor_x = 0; coor_x < screen_width; coor_x++) {
            *ptr++  =   0x000000;
        }
    }
    // color bar
    int offsety1 = screen_height * 1 / 5;
    int offsetx1 = screen_width  * 1 / 5;
    int size_x = 50;
    int size_y = 3;
    for (int i = 1; i < 5; i++) {
        for (int j = 1; j < 5; j++) {
            for(coor_y = (offsety1)*(i-1); coor_y < (offsety1)*(i); coor_y++) {
                ptr =   (unsigned long*)fb_mapped + screen_width * coor_y;
                for (coor_x = 0; coor_x < offsetx1*(size_x+j); coor_x++) {
                    *ptr++  =   0x000000;
                }
                for (coor_x = offsetx1*(size_x+j); coor_x < offsetx1*(size_x+(j+1)); coor_x++) {
                    *ptr++  =   0x000000;
                }
                for (coor_x = offsetx1*(size_x+(j+1)); coor_x < screen_width; coor_x++) {
                    *ptr++  =   0x000000;
                }
            }
            for(coor_y = (offsety1)*i; coor_y < (offsety1)*(i+1); coor_y++) {
                ptr =   (unsigned long*)fb_mapped + screen_width * coor_y;
                for (coor_x = 0; coor_x < offsetx1*(size_y+j); coor_x++) {
                    *ptr++  =   0x000000;
                }
                for (coor_x = offsetx1*(size_y+j); coor_x < offsetx1*(size_y+(j+1)); coor_x++) {
                    *ptr++  =   0xFFFFFF;
                }
                for (coor_x = offsetx1*(size_y+(j+1)); coor_x < screen_width; coor_x++) {
                    *ptr++  =   0x000000;
                }
            }
            usleep(300000);
        }
    }

    munmap( fb_mapped, mem_size_x);
    close( fb_fd);
    return 0;
}
