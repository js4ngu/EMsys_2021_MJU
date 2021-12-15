#include <stdio.h>
#include <stdlib.h>     // for exit
#include <unistd.h>     // for open/close
#include <fcntl.h>      // for O_RDWR
#include <sys/ioctl.h>  // for ioctl
#include <sys/mman.h>
#include <linux/fb.h>   // for fb_var_screeninfo, FBIOGET_VSCREENINFO

#define FBDEV_FILE "/dev/fb0"
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

    int fb_fd;
    struct  fb_var_screeninfo fbvar;
    struct  fb_fix_screeninfo fbfix;
    unsigned char   *fb_mapped;
    int mem_size;
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

    mem_size    =   screen_width * screen_height * 4;
    fb_mapped   =   (unsigned char *)mmap(0, mem_size,
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


    int coor_Des[2] = {500,300};
    int coor_Ball[2] = {500,300};

    //DES DP
	for(int coor_y = coor_Des[1]; coor_y < coor_Des[1] + DES_SIZE; coor_y++) {
		for (int coor_x = coor_Des[0]; coor_x < coor_Des[0] + DES_SIZE; coor_x++) {
            *ptr++ = 0xFFFFFF;
		}
    }

    //BALL DP
	for(int coor_y = coor_Ball[1]; coor_y < coor_Ball[1] + BALL_SIZE; coor_y++) {
		for (int coor_x = coor_Ball[0]; coor_x < coor_Ball[0] + BALL_SIZE; coor_x++) {
            *ptr++ = 0xFF00FF;
		}
    }


    munmap( fb_mapped, mem_size);
    close( fb_fd);
    return 0;
}
