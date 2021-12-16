// // #include "framebuffer.h"

// #include <stdio.h>
// #include <stdlib.h>     // for exit
// #include <unistd.h>     // for open/close
// #include <fcntl.h>      // for O_RDWR
// #include <sys/ioctl.h>  // for ioctl
// #include <sys/mman.h>
// #include <linux/fb.h>   // for fb_var_screeninfo, FBIOGET_VSCREENINFO
// #include <stdlib.h> // for exit

// #define FBDEV_FILE_0 "/dev/fb0"
// #define FBDEV_FILE_1 "/dev/fb1"

// #define DES_SIZE 500
// #define BALL_SIZE 100
// #define WIDTH 51
// #define HIGH 30

// int main (int argc, char **argv)
// {
//     int screen_width;
//     int screen_height;
//     int bits_per_pixel;
//     int line_length;

//     int fb_fd_0;
//     int fb_fd_1;
//     struct  fb_var_screeninfo fbvar;
//     struct  fb_fix_screeninfo fbfix;
//     unsigned char   *fb_mapped0;
//     unsigned char   *fb_mapped1;
//     int mem_size;
//     unsigned long   *ptr0;
//     unsigned long   *ptr1;
//     int coor_y;
//     int coor_x;

//     printf("===================================\n");
//     printf("Frame buffer Application - ColorBar\n");
//     printf("===================================\n");

//     if( access(FBDEV_FILE_0, F_OK) )
//     {
//         printf("%s: access error\n", FBDEV_FILE_0);
//         exit(1);
//     }

//     if( (fb_fd_0 = open(FBDEV_FILE_0, O_RDWR)) < 0)
//     {
//         printf("%s: open error\n", FBDEV_FILE_0);
//         exit(1);
//     }

//     if( ioctl(fb_fd_0, FBIOGET_VSCREENINFO, &fbvar) )
//     {
//         printf("%s: ioctl error - FBIOGET_VSCREENINFO \n", FBDEV_FILE_0);
//         exit(1);
//     }

//     if( ioctl(fb_fd_0, FBIOGET_FSCREENINFO, &fbfix) )
//     {
//         printf("%s: ioctl error - FBIOGET_FSCREENINFO \n", FBDEV_FILE_0);
//         exit(1);
//     }

//     if( access(FBDEV_FILE_1, F_OK) )
//     {
//         printf("%s: access error\n", FBDEV_FILE_1);
//         exit(1);
//     }

//     if( (fb_fd_1 = open(FBDEV_FILE_1, O_RDWR)) < 0)
//     {
//         printf("%s: open error\n", FBDEV_FILE_1);
//         exit(1);
//     }

//     if( ioctl(fb_fd_1, FBIOGET_VSCREENINFO, &fbvar) )
//     {
//         printf("%s: ioctl error - FBIOGET_VSCREENINFO \n", FBDEV_FILE_1);
//         exit(1);
//     }

//     if( ioctl(fb_fd_1, FBIOGET_FSCREENINFO, &fbfix) )
//     {
//         printf("%s: ioctl error - FBIOGET_FSCREENINFO \n", FBDEV_FILE_1);
//         exit(1);
//     }

//     screen_width    =   fbvar.xres;
//     screen_height   =   fbvar.yres;
//     bits_per_pixel  =   fbvar.bits_per_pixel;
//     line_length     =   fbfix.line_length;

//     printf("screen_width : %d\n", screen_width);
//     printf("screen_height : %d\n", screen_height);
//     printf("bits_per_pixel : %d\n", bits_per_pixel);
//     printf("line_length : %d\n", line_length);
//     printf("frame_buffer : %s\n", FBDEV_FILE_0);
//     printf("frame_buffer : %s\n", FBDEV_FILE_1);
//     printf("____________________\n");

//     mem_size    =   screen_width * screen_height * 4;
//     fb_mapped0   =   (unsigned char *)mmap(0, mem_size, PROT_READ|PROT_WRITE, MAP_SHARED, fb_fd_0, 0);
//     fb_mapped1   =   (unsigned char *)mmap(0, mem_size, PROT_READ|PROT_WRITE, MAP_SHARED, fb_fd_1, 0);
//     if (fb_mapped0 < 0 || fb_mapped1 < 0)
//     {
//         printf("mmap error!\n");
//         exit(1);
//     }
//     if (fb_mapped1 < 0)
//     {
//         printf("mmap error!\n");
//         exit(1);
//     }
//     int coor_Des[2] = {500,300};
//     int coor_Ball[2] = {800,500};

//     for(coor_y = 0; coor_y < screen_height; coor_y++) {
//         ptr0 =   (unsigned long *)fb_mapped0 + screen_width * coor_y;
//         for(coor_x = 0; coor_x < screen_width; coor_x++) {
//             *ptr0++  =   0x000000;
//         }
//     }
//     munmap( fb_mapped0, mem_size);
//     munmap( fb_mapped1, mem_size);
//     close( fb_fd_0);
//     close( fb_fd_1);
//     return 0;
// }

#include <stdio.h>
#include <stdlib.h>    // for exit
#include <unistd.h>    // for open/close
#include <fcntl.h>     // for O_RDWR
#include <sys/ioctl.h> // for ioctl
#include <sys/kd.h>    // for remove cursor
#include <sys/mman.h>
#include <linux/fb.h> // for fb_var_screeninfo, FBIOGET_VSCREENINFO

#define FBDEV_FILE0 "/dev/fb0"
#define FBDEV_FILE1 "/dev/fb1"

#define DES_size_x 500
#define BALL_size_x 100
#define WIDTH 51
#define HIGH 30

void Ball_DP(int x1, int y1, unsigned long *ptr, unsigned char *fb_mapped, int screen_width, int screen_height);
void Ball_RM(int x1, int y1, unsigned long *ptr, unsigned char *fb_mapped, int screen_width, int screen_height);
void Back_ground(int COLOR, unsigned long *ptr, unsigned char *fb_mapped, int screen_width, int screen_height);
void hurdle_0(int X1, int Y1, unsigned long *ptr, unsigned char *fb_mapped, int screen_width, int screen_height);
void hurdle_1(int X1, int Y1, unsigned long *ptr, unsigned char *fb_mapped, int screen_width, int screen_height);
void DES(int X1, int Y1, unsigned long *ptr, unsigned char *fb_mapped, int screen_width, int screen_height);

int fb_fd0;
int fb_fd1;
struct fb_var_screeninfo fbvar;
struct fb_fix_screeninfo fbfix;
unsigned char *fb_mapped0;
unsigned char *fb_mapped1;
unsigned long *ptr0;
unsigned long *ptr1;

int screen_width;
int screen_height;
int bits_per_pixel;
int line_length;

int mem_size;

int display_init(void)
{
    // 커서 제거
    int conFD = open("/dev/tty0", O_RDWR);
    ioctl(conFD, KDSETMODE, KD_GRAPHICS);
    close(conFD);

    if (access(FBDEV_FILE0, F_OK))
    {
        printf("%s: access error\n", FBDEV_FILE0);
        exit(1);
    }

    if ((fb_fd0 = open(FBDEV_FILE0, O_RDWR)) < 0)
    {
        printf("%s: open error\n", FBDEV_FILE0);
        exit(1);
    }

    if (ioctl(fb_fd0, FBIOGET_VSCREENINFO, &fbvar))
    {
        printf("%s: ioctl error - FBIOGET_VSCREENINFO \n", FBDEV_FILE0);
        exit(1);
    }

    if (ioctl(fb_fd0, FBIOGET_FSCREENINFO, &fbfix))
    {
        printf("%s: ioctl error - FBIOGET_FSCREENINFO \n", FBDEV_FILE0);
        exit(1);
    }

    if (access(FBDEV_FILE1, F_OK))
    {
        printf("%s: access error\n", FBDEV_FILE1);
        exit(1);
    }

    if ((fb_fd1 = open(FBDEV_FILE1, O_RDWR)) < 0)
    {
        printf("%s: open error\n", FBDEV_FILE1);
        exit(1);
    }

    if (ioctl(fb_fd1, FBIOGET_VSCREENINFO, &fbvar))
    {
        printf("%s: ioctl error - FBIOGET_VSCREENINFO \n", FBDEV_FILE1);
        exit(1);
    }

    if (ioctl(fb_fd1, FBIOGET_FSCREENINFO, &fbfix))
    {
        printf("%s: ioctl error - FBIOGET_FSCREENINFO \n", FBDEV_FILE1);
        exit(1);
    }

    screen_width = fbvar.xres;
    screen_height = fbvar.yres;
    bits_per_pixel = fbvar.bits_per_pixel;
    line_length = fbfix.line_length;

    printf("screen_width : %d\n", screen_width);
    printf("screen_height : %d\n", screen_height);
    printf("bits_per_pixel : %d\n", bits_per_pixel);
    printf("line_length : %d\n", line_length);

    mem_size = screen_width * screen_height * 4;
    fb_mapped0 = (unsigned char *)mmap(0, mem_size, PROT_READ | PROT_WRITE, MAP_SHARED, fb_fd0, 0);
    fb_mapped1 = (unsigned char *)mmap(0, mem_size, PROT_READ | PROT_WRITE, MAP_SHARED, fb_fd1, 0);

    if (fb_mapped0 < 0 || fb_mapped1 < 0)
    {
        printf("mmap error!\n");
        exit(1);
    }

    return 0;
}

int end_display(void)
{
    munmap(fb_mapped0, mem_size);
    close(fb_fd0);
    munmap(fb_mapped1, mem_size);
    close(fb_fd1);
    return 0;
}

int main(int argc, char **argv)
{
    display_init();
     
    printf("===================================\n");
    printf("Frame buffer Application - ColorBar\n");
    printf("===================================\n");

    Back_ground(0x000000, ptr0, fb_mapped0, screen_width, screen_height);
    for (int i = 0; i < 1024; i++)
    {
        Draw_Square(i, i * 600 / 1024, 40, 40, ptr0, fb_mapped0, screen_width, screen_height);
        usleep(10000);
        Back_ground(0x000000, ptr0, fb_mapped0, screen_width, screen_height);
        usleep(1);
    }
    Back_ground(0x000000, ptr0, fb_mapped0, screen_width, screen_height);
    
    end_display();
    
    return 0;
}

void Back_ground(int COLOR, unsigned long *ptr, unsigned char *fb_mapped, int screen_width, int screen_height)
{
    int coor_y, coor_x;
    for (coor_y = 0; coor_y < screen_height; coor_y++)
    {
        for (coor_x = 0; coor_x < screen_width; coor_x++)
        {
            ptr = (unsigned long *)fb_mapped + screen_width * coor_y + coor_x;
            *ptr = COLOR;
        }
    }
}

void Draw_Square(int X, int Y, int dX, int dY, unsigned long *ptr, unsigned char *fb_mapped, int screen_width, int screen_height)
{
    for (int x = X; x < X + dX; x++)
    {
        for (int y = Y; y < Y + dY; y++)
        {
            ptr = (unsigned long *)fb_mapped + screen_width * y + x;
            *ptr = 0xFFFFFF;
        }
    }
}

void Ball_DP(int X1, int Y1, unsigned long *ptr, unsigned char *fb_mapped, int screen_width, int screen_height)
{
    int size_x = 100;
    int size_y = 100;

    for (int x = X1; x < X1 + size_x; x++)
    {
        for (int y = Y1; y < Y1 + size_y; y++)
        {
            ptr = (unsigned long *)fb_mapped + screen_width * y + x;
            *ptr = 0xFFFFFF;
        }
    }
}

void Ball_RM(int X1, int Y1, unsigned long *ptr, unsigned char *fb_mapped, int screen_width, int screen_height)
{
    int size_x = 100;
    int size_y = 100;

    for (int x = X1; x < X1 + size_x; x++)
    {
        for (int y = Y1; y < Y1 + size_y; y++)
        {
            ptr = (unsigned long *)fb_mapped + screen_width * y + x;
            *ptr = 0x000000;
        }
    }
}

/*
 아이디어
 움직이는 방향의 정 반대에 배경색과 동일한 색을 한 줄 입력하고 그려주기 시작하는 위치를 변경

*/

// void hurdle_0(int X1, int Y1, unsigned long *ptr, unsigned char *fb_mapped, int screen_width, int screen_height){
//     int size_x = 200;
//     int size_y = 100;
//     int X2 = X1 + size_x;
//     int Y2 = Y1 - size_y;
//     int coor_y, coor_x;

//     for(coor_y = 0; coor_y < Y1; coor_y++) {
//         ptr =   (unsigned long*)fb_mapped + screen_width * coor_y;
//         for (coor_x = 0; coor_x < X1; coor_x++) {
//             *ptr++  =   0x000000;
//         }
//         for (coor_x = X1; coor_x < X2; coor_x++) {
//             *ptr++  =   0xFF0000;
//         }
//         for (coor_x = X2; coor_x < screen_width; coor_x++) {
//             *ptr++  =   0x000000;
//         }
//     }
//     for(coor_y = 0; coor_y < Y2; coor_y++) {
//         ptr =   (unsigned long*)fb_mapped + screen_width * coor_y;
//         for (coor_x = 0; coor_x < X1; coor_x++) {
//             *ptr++  =   0x000000;
//         }
//         for (coor_x = X1; coor_x < X2; coor_x++) {
//             *ptr++  =   0x000000;
//         }
//         for (coor_x = X2; coor_x < screen_width; coor_x++) {
//             *ptr++  =   0x000000;
//         }
//     }
// }

// void hurdle_1(int X1, int Y1, unsigned long *ptr, unsigned char *fb_mapped, int screen_width, int screen_height){
//     int size_x = 300;
//     int size_y = 100;
//     int X2 = X1 + size_x;
//     int Y2 = Y1 - size_y;
//     int coor_y, coor_x;

//     for(coor_y = 0; coor_y < Y1; coor_y++) {
//         ptr =   (unsigned long*)fb_mapped + screen_width * coor_y;
//         for (coor_x = 0; coor_x < X1; coor_x++) {
//             *ptr++  =   0x000000;
//         }
//         for (coor_x = X1; coor_x < X2; coor_x++) {
//             *ptr++  =   0x00FF00;
//         }
//         for (coor_x = X2; coor_x < screen_width; coor_x++) {
//             *ptr++  =   0x000000;
//         }
//     }
//     for(coor_y = 0; coor_y < Y2; coor_y++) {
//         ptr =   (unsigned long*)fb_mapped + screen_width * coor_y;
//         for (coor_x = 0; coor_x < X1; coor_x++) {
//             *ptr++  =   0x000000;
//         }
//         for (coor_x = X1; coor_x < X2; coor_x++) {
//             *ptr++  =   0x000000;
//         }
//         for (coor_x = X2; coor_x < screen_width; coor_x++) {
//             *ptr++  =   0x000000;
//         }
//     }
// }

// void DES(int X1, int Y1, unsigned long *ptr, unsigned char *fb_mapped, int screen_width, int screen_height){
//     int size_x = 100;
//     int size_y = 100;
//     int X2 = X1 + size_x;
//     int Y2 = Y1 - size_y;
//     int coor_y, coor_x;

//     for(coor_y = 0; coor_y < Y1; coor_y++) {
//         ptr =   (unsigned long*)fb_mapped + screen_width * coor_y;
//         for (coor_x = 0; coor_x < X1; coor_x++) {
//             *ptr++  =   0x000000;
//         }
//         for (coor_x = X1; coor_x < X2; coor_x++) {
//             *ptr++  =   0x0000FF;
//         }
//         for (coor_x = X2; coor_x < screen_width; coor_x++) {
//             *ptr++  =   0x000000;
//         }
//     }
//     for(coor_y = 0; coor_y < Y2; coor_y++) {
//         ptr =   (unsigned long*)fb_mapped + screen_width * coor_y;
//         for (coor_x = 0; coor_x < X1; coor_x++) {
//             *ptr++  =   0x000000;
//         }
//         for (coor_x = X1; coor_x < X2; coor_x++) {
//             *ptr++  =   0x000000;
//         }
//         for (coor_x = X2; coor_x < screen_width; coor_x++) {
//             *ptr++  =   0x000000;
//         }
//     }
// }