#include <math.h>
#include <stdlib.h> // for exit
#include "./lib/accelMagGyro.h"
#include "./lib/button.h"
#include "./lib/buzzer.h"
#include "./lib/colorLED.h"
#include "./lib/fnd.h"
#include "./lib/led.h"
#include "./lib/temp.h"
#include "./lib/textLCD.h"
#include "./lib/display.h"

#define MENU 7
#define WIN 8
#define PWROFF 9
#define FAIL 10
#define EXIT 11
#define LEVEL1 12
#define LEVEL2 13
#define LEVEL3 14

void HW_init();
void HW_close();
int set_Mobility();

double angle_x(double accel_x, double accel_y, double accel_z);
double angle_y(double accel_x, double accel_y, double accel_z);
double round(double num);
static int msgID = 0;
int status = MENU;

int main(void)
{
    HW_init();
    display_init();
    int mobility = set_Mobility();
    msgID = buttonInit();
    int returnValue = 0;
    BUTTON_MSG_T messageRxData;

    while (1)
    {
        returnValue = msgrcv(msgID, &messageRxData, sizeof(int), 0, 0);

        if (returnValue < 0)
            printf("Receive Key Failed\r\n");

        status = MENU;
        writeLCD(1, "MENU");
        switch (messageRxData.keyInput)
        {
        case KEY_HOME:
            printf("Home key\r\n");
            printf("PLAY key\r\n");
            status = LEVEL1;
            break;
        case KEY_BACK:
            printf("Back key\r\n");
            printf("PLAY key\r\n");
            mobility = 1;
            status = LEVEL1;
            break;
        case KEY_VOLUMEDOWN:
            printf("Volume down key\r\n");
            printf("PWR OFF\r\n");
            status = PWROFF;
            break;
        default:
            printf("KEY IMPUT ERROT \r\n");
            break;
        }

        switch (status)
        {
        case LEVEL1:
            writeLCD(1, "LEVEL1");
            printf("LEVEL1\r\n");
            int Ax = 300;
            int Ay = 512;

            level_1_background();
            while (1)
            {
                init_accel();
                double ax = read_accel(X) / 163;
                double ay = read_accel(Y) / 163;
                Ax = Ax + (ax * mobility);
                Ay = Ay + (ay * mobility);

                int Cx = (Ax + 40) / 2;
                int Cy = (Ay + 40) / 2;

                if (Cx > 50 && Cx < 1024 - 500 && Cy > 50 && Cy < 600 - 50)
                {
                }
                else
                {
                    status = FAIL;
                    break;
                }

                draw_square(Ay, Ax, 40, 40, 0x000000, 0);
                usleep(100000);
                draw_square(Ay, Ax, 40, 40, 0xFFFFFF, 0);

                printf("Deg Data: %d, %d\r\n", (int)ax, (int)ay);
            }
            break;
        case LEVEL2:
            writeLCD(1, "LEVEL2");
            printf("LEVEL2\r\n");
            usleep(1000);

            break;
        case LEVEL3:
            writeLCD(1, "LEVEL3");
            printf("LEVEL3\r\n");
            usleep(1000);

            break;
        case FAIL:
            writeLCD(1, "FAIL");
            printf("FAIL\r\n");
            usleep(1000);
            break;
        case PWROFF:
            writeLCD(1, "PWROFF");
            writeLCD(2, "Thanks for Play");
            printf("PWROFF\r\n");
            buttonExit();
            return 0;
            break;
        default:
            break;
        }
    }

    return 0;

    while (1)
    {
        switch (status)
        {
        case MENU: // 메인 메뉴
            // 프레임버퍼에 메뉴 사진 밀어넣고 띄우기
            returnValue = msgrcv(msgID, &messageRxData, sizeof(int), 0, 0); // 버튼 입력 받기
            if (returnValue < 0)
            {
                printf("Receive Key Failed\r\n");
                status = MENU;
                break;
            }
            switch (messageRxData.keyInput)
            {
            case KEY_HOME: // 정상 게임 모드
                printf("Home key\r\n");
                printf("PLAY key\r\n");
                status = LEVEL1;
                break;
            case KEY_BACK: // 디버그 게임 모드
                printf("Back key\r\n");
                printf("PLAY key\r\n");
                mobility = 1;
                status = LEVEL1;
                break;
            case KEY_VOLUMEDOWN: // 게임 종료
                printf("Volume down key\r\n");
                printf("PWR OFF\r\n");
                status = EXIT;
                break;
            default: // 잘못된 키 입력
                printf("KEY INPUT ERROR \r\n");
                break;
            }
            break;
        
        case WIN: // 게임 승리
            // 프레임버퍼에 승리 사진 밀어넣고 띄우기
            writeLCD(1, "You Won This Game");
            writeLCD(2, "You Are Winner");
            returnValue = msgrcv(msgID, &messageRxData, sizeof(int), 0, 0); // 버튼 입력 받기
            if (returnValue < 0)
            {
                printf("Receive Key Failed\r\n");
                status = WIN;
                break;
            }
            if (messageRxData.keyInput == KEY_HOME)
            {
                status = MENU;
            }
            else if(messageRxData.keyInput == KEY_VOLUMEDOWN)
            {
                status = EXIT;
            }
            break;

        case EXIT: // 게임 종료
            writeLCD(1, "Good Bye");
            buttonExit();
            return 0;

        case LEVEL1:
            writeLCD(1, "LEVEL1");
            printf("LEVEL1\r\n");
            int Ax = 300;
            int Ay = 512;
            int flag_safe = 1;

            level_1_background();
            while (flag_safe)
            {
                init_accel();
                double ax = read_accel(X) / 163;
                double ay = read_accel(Y) / 163;
                Ax = Ax + (ax * mobility);
                Ay = Ay + (ay * mobility);

                int Cx = (Ax + 40) / 2;
                int Cy = (Ay + 40) / 2;

                if (Cx > 50 && Cx < 1024 - 50 && Cy > 50 && Cy < 600 - 50)
                {}
                else
                {
                    status = FAIL;
                    flag_safe = 0;
                }

                draw_square(Ay, Ax, 40, 40, 0x000000, 0);
                usleep(100000);
                draw_square(Ay, Ax, 40, 40, 0xFFFFFF, 0);

                printf("Deg Data: %d, %d\r\n", (int)ax, (int)ay);
            }
            break;
        }
    }
}

void HW_init()
{
    //init_accel();
    //init_gyro();
    textLCD_Init();
    ledInit();
    printf("HW INIT\n");
}

void HW_close()
{
    close_accel();
    textLCD_off();
    ledExit();
}

int set_Mobility()
{
    int temp = readTEMP() / 5;
    if (temp > 12)
        temp = 12;
    return temp;
}

void level_1_background(void)
{
    draw_background(0x8B4513, 0);
    draw_square(50, 50, 1024 - 100, 600 - 100, 0xFFFFFF, 0);
}

void level_2_background(void)
{
    draw_background(0x8B4513, 0);
    draw_square(50, 50, 1024 - 100, 600 - 100, 0xFFFFFF, 0);
    draw_square(358, 250, 308, 100, 0x8B4513, 0);
}
void level_3_background(void)
{
    draw_background(0x8B4513, 0);
    draw_square(50, 50, 1024 - 100, 600 - 100, 0xFFFFFF, 0);
}