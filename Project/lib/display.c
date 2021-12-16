#include "display.h"

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

int draw_background(int color, int idx_buffer)
{
    if (idx_buffer == 0)
    {
        for (int i = 0; i < screen_height; i++)
        {
            for (int j = 0; j < screen_width; j++)
            {
                ptr0 = (unsigned long *)fb_mapped0 + screen_width * i + j;
                *ptr0 = color;
            }
        }
        return 0;
    }
    else if (idx_buffer == 1)
    {
        for (int i = 0; i < screen_height; i++)
        {
            for (int j = 0; j < screen_width; j++)
            {
                ptr1 = (unsigned long *)fb_mapped1 + screen_width * i + j;
                *ptr1 = color;
            }
        }
        return 0;
    }
    else
        return -1;
}

int draw_square(int X, int Y, int dX, int dY, int color, int idx_buffer)
{
    if (idx_buffer == 0)
    {
        for (int x = X; x < X + dX; x++)
        {
            for (int y = Y; y < Y + dY; y++)
            {
                ptr0 = (unsigned long *)fb_mapped0 + screen_width * y + x;
                *ptr0 = color;
            }
        }
        return 0;
    }
    else if (idx_buffer == 1)
    {
        for (int x = X; x < X + dX; x++)
        {
            for (int y = Y; y < Y + dY; y++)
            {
                ptr1 = (unsigned long *)fb_mapped1 + screen_width * y + x;
                *ptr1 = color;
            }
        }
        return 0;
    }
    else
        return -1;
}

int display_close(void)
{
    munmap(fb_mapped0, mem_size);
    close(fb_fd0);
    munmap(fb_mapped1, mem_size);
    close(fb_fd1);
    return 0;
}