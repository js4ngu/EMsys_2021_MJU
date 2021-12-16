#include "../lib/display.h"

void level_1_background(void)
{
    draw_background(0x8B4513, 0);
    draw_square(50,50,1024-100,600-100,0xFFFFFF, 0);
}

void level_1_obj_1(int OnOff)
{
    if(OnOff == 1) draw_square(342,220,50,50,0xFF0000, 0);
    else if(OnOff == 0) draw_square(342,220,50,50,0xFFFFFF, 0);
    else;
}

void level_1_obj_2(int OnOff)
{
    if(OnOff == 1) draw_square(729,403,50,50,0xFF0000, 0);
    else if(OnOff == 0) draw_square(729,403,50,50,0xFFFFFF, 0);
    else;
}

int main(int argc, char **argv)
{
    display_init();
     
    printf("===================================\n");
    printf("Frame buffer Application - ColorBar\n");
    printf("===================================\n");

    level_1_background();  
    level_1_obj_1(1);
    level_1_obj_2(1);
    for (int i = 25; i < 230; i++)
    {
        if(i == 98) level_1_obj_1(0);
        if(i == 189) level_1_obj_2(0);
        draw_square(4*i, 2*i, 40, 40, 0x000000, 0);
        
        usleep(100000);
        draw_square(4*i, 2*i, 40, 40, 0xFFFFFF, 0);
    }
    level_1_obj_1(0);
    level_1_obj_2(0);
    display_close();
    
    return 0;
}
