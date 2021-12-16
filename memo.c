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

#define MENU 0
#define LEVEL1 1
#define LEVEL2 2
#define LEVEL3 3
#define FAIL 4
#define STAGE1 5
#define STAGE2 6
#define STAGE3 7
#define STAGE4 8
#define STAGE5 9
#define STAGE6 10
#define STAGE7 11
#define STAGE8 12
#define START 13
#define EXIT 14

#define HURDLE_WIDTH 51
#define HURDLE_HIGH 30
#define DES_SENSTIVE 60

const int DESTINATION_S1 [3][2]  = {{100,100},
                                    {200,100},
                                    {300,200}};
const int DESTINATION_S2 [3][2]  = {{100,100},
                                    {200,100},
                                    {300,200}};
const int DESTINATION_S3 [3][2]  = {{100,100},
                                    {200,100},
                                    {300,200}};

const int HURDLE_0[2] = {500,300}; //왼쪽 위 꼭지점 좌표 hurdle_0[0] : x, hurdle_0[1] : y
const int HURDLE_1[2] = {400,500};

void HW_init();
void HW_close();

void Level1();
void Level2();
void Level3();

void Level1_Playgame(int destinationX, int destinationY, int nextStage);
void Level2_Playgame(int destinationX, int destinationY, int nextStage);
void Level3_Playgame(int destinationX, int destinationY, int nextStage);

void Ball_display1(int ballLocationX, int ballLocationY);
void Ball_display2(int ballLocationX, int ballLocationY);
void Ball_display3(int ballLocationX, int ballLocationY);

int set_Mobility();
int angle_x(void);
int angle_y(void);

int ballLocation[2] = {0,0};
int status = MENU;
int stage = STAGE1;
int key_value = 0;
int mobility = 0;

void main(){
    HW_init();
    while (1){
        switch (status) {
            case MENU:
                textLCD_off();
                writeLCD(1, "MENU");
                status = LEVEL1;
                break;
                
            case LEVEL1:
                textLCD_off();
                writeLCD(1, "LEVEL1");
                Level1();
                break;

            case LEVEL2:
                textLCD_off();
                writeLCD(1, "LEVEL2");
                Level2();
                break;

            case LEVEL3:
                textLCD_off();
                writeLCD(1, "LEVEL3");
                Level3();
                break;

            case FAIL:
                textLCD_off();
                writeLCD(1, "FAIL");
                break;

            default:
                fndOff();
                status = MENU;
                stage = STAGE1;
                break;
        }
    }
}

/*
1. 화면을 받고
2. 버튼 : 쓰레드
3. 자이로 : 쓰레도
4. 게임 : 쓰레드
5. 화면 출력 : 쓰레드
*/

void Level1(){
    int destination[2];
    switch (stage) {
        case STAGE1:
            fndDisp(1, 0);
            destination[0] = DESTINATION_S1[0][0];
            destination[1] = DESTINATION_S1[0][1];
            Level1_Playgame(destination[0],destination[1],STAGE2);
            break;
        case STAGE2:
            fndDisp(2, 0);
            destination[0] = DESTINATION_S1[1][0];
            destination[1] = DESTINATION_S1[1][1];
            Level1_Playgame(destination[0],destination[1],STAGE3);
            break;
        case STAGE3:
            fndDisp(3, 0);
            destination[0] = DESTINATION_S1[2][0];
            destination[1] = DESTINATION_S1[2][1];
            Level1_Playgame(destination[0],destination[1],STAGE4);
            break;
        default:
            break;
        }
}

void Level2(){
    int destination[2];
    switch (stage) {
        case STAGE1:
            fndDisp(1, 0);
            destination[0] = DESTINATION_S2[0][0];
            destination[1] = DESTINATION_S2[0][0];
            Level2_Playgame(destination[0],destination[1],STAGE2);
            break;
        case STAGE2:
            fndDisp(2, 0);
            destination[0] = DESTINATION_S2[1][0];
            destination[1] = DESTINATION_S2[1][1];
            Level2_Playgame(destination[0],destination[1],STAGE3);
            break;
        case STAGE3:
            fndDisp(3, 0);
            destination[0] = DESTINATION_S2[2][0];
            destination[1] = DESTINATION_S2[2][1];
            Level2_Playgame(destination[0],destination[1],STAGE4);
            break;
        default:
            break;
    }
}

void Level3(){
    int destination[2];
    switch (stage) {
        case STAGE1:
            fndDisp(1, 0);
            destination[0] = DESTINATION_S3[0][0];
            destination[1] = DESTINATION_S3[0][1];
            Level3_Playgame(destination[0],destination[1],STAGE2);
            break;
        case STAGE2:
            fndDisp(2, 0);
            destination[0] = DESTINATION_S3[1][0];
            destination[1] = DESTINATION_S3[1][1];
            Level3_Playgame(destination[0],destination[1],STAGE3);
            break;
        case STAGE3:
            fndDisp(3, 0);
            destination[0] = DESTINATION_S3[2][0];
            destination[1] = DESTINATION_S3[2][1];
            Level3_Playgame(destination[0],destination[1],STAGE4);
            break;
        default:
            break;
        }
        
}

void Level1_Playgame(int destination_x, int destination_y,int nextStage){
    int destination[2] = {destination_x,destination_y};
    int ballSpeed[2];

    int des_Flag[2] = {0,0};
    int result = 2;

    while (1) {
        printf("%d, ", angle_x());
        printf("%d\n", angle_y());
        Ball_display1(ballLocation[0], ballLocation[1]);
        ballSpeed[0] = mobility * angle_x();
        ballSpeed[1] = mobility * angle_y();
        ballLocation[0] = ballLocation[0] + ballSpeed[0];
        ballLocation[1] = ballLocation[1] + ballSpeed[1];
        
        if (( destination[0] - DES_SENSTIVE < ballLocation[1] ) && ( ballLocation[1] < destination[1] + DES_SENSTIVE )) des_Flag[0] = 1;
        if (( destination[0] - DES_SENSTIVE < ballLocation[1] ) && ( ballLocation[1] < destination[1] + DES_SENSTIVE )) des_Flag[1] = 1;

        if (des_Flag[0] && des_Flag[1]) {
            result = 1;
            break;
        }
        Ball_display1(ballLocation[0], ballLocation[1]);
    }

    if(nextStage == STAGE1) {
        if (result == 1) {
            stage = nextStage;
            status = LEVEL2;
        }
        else if (result == 0) status = FAIL;
    }
    else {
        if (result == 1) stage = nextStage;
    }
}

void Level2_Playgame(int destination_x, int destination_y, int nextStage){
    int destination[2] = {destination_x,destination_y};
    int ballSpeed[2];

    int des_Flag[2] = {0,0};
    int deadLine_Flag0[2] = {0,0};
    int result = 2;

    while (1) {
        printf("%d, ", angle_x());
        printf("%d\n", angle_y());
        Ball_display2(ballLocation[0], ballLocation[1]);
        ballSpeed[0] = mobility * angle_x();
        ballSpeed[1] = mobility * angle_y();
        ballLocation[0] = ballLocation[0] + ballSpeed[0];
        ballLocation[1] = ballLocation[1] + ballSpeed[1];
        
        //장애물 검출 알고리즘 입력
        if (( HURDLE_0[0] - HURDLE_WIDTH < ballLocation[0] ) && ( ballLocation[0] < HURDLE_0[0] + HURDLE_WIDTH )) deadLine_Flag0[0] = 1;
        if (( HURDLE_0[1] - HURDLE_HIGH  < ballLocation[1] ) && ( ballLocation[1] < HURDLE_0[1] + HURDLE_HIGH )) deadLine_Flag0[1] = 1;

        if (( destination[0] - DES_SENSTIVE < ballLocation[1] ) && ( ballLocation[1] < destination[1] + DES_SENSTIVE )) des_Flag[0] = 1;
        if (( destination[0] - DES_SENSTIVE < ballLocation[1] ) && ( ballLocation[1] < destination[1] + DES_SENSTIVE )) des_Flag[1] = 1;

        if (des_Flag[0] && des_Flag[1]) {
            result = 1;
            break;
        }
        if (deadLine_Flag0[0] && deadLine_Flag0[1]) {
            result = 0;
            break;
        }
        Ball_display2(ballLocation[0], ballLocation[1]);
    }
    if(nextStage == STAGE1) {
        if (result == 1) {
            stage = nextStage;
            status = LEVEL3;
        }
        else if (result == 0) status = FAIL;
    }
    else {
        if (result == 1) stage = nextStage;
        else if (result == 0) status = FAIL;
    }
}

void Level3_Playgame(int destination_x, int destination_y, int nextStage){
    int destination[2] = {destination_x,destination_y};
    int ballSpeed[2];

    int des_Flag[2] = {0,0};
    int deadLine_Flag0[2] = {0,0};
    int deadLine_Flag1[2] = {0,0};
    int result = 2;

    while (1) {
        printf("%d, ", angle_x());
        printf("%d\n", angle_y());
        Ball_display3(ballLocation[0], ballLocation[1]);
        ballSpeed[0] = mobility * angle_x();
        ballSpeed[1] = mobility * angle_y();
        ballLocation[0] = ballLocation[0] + ballSpeed[0];
        ballLocation[1] = ballLocation[1] + ballSpeed[1];
        
        //장애물 검출 알고리즘 입력
        if (( HURDLE_0[0] - HURDLE_WIDTH < ballLocation[0] ) && ( ballLocation[0] < HURDLE_0[0] + HURDLE_WIDTH )) deadLine_Flag0[0] = 1;
        if (( HURDLE_0[1] - HURDLE_HIGH  < ballLocation[1] ) && ( ballLocation[1] < HURDLE_0[1] + HURDLE_HIGH )) deadLine_Flag0[1] = 1;
        if (( HURDLE_1[0] - HURDLE_WIDTH < ballLocation[0] ) && ( ballLocation[0] < HURDLE_1[0] + HURDLE_WIDTH )) deadLine_Flag1[0] = 1;
        if (( HURDLE_1[1] - HURDLE_HIGH  < ballLocation[1] ) && ( ballLocation[1] < HURDLE_1[1] + HURDLE_HIGH )) deadLine_Flag1[1] = 1;

        if (( destination[0] - DES_SENSTIVE < ballLocation[1] ) && ( ballLocation[1] < destination[1] + DES_SENSTIVE )) des_Flag[0] = 1;
        if (( destination[0] - DES_SENSTIVE < ballLocation[1] ) && ( ballLocation[1] < destination[1] + DES_SENSTIVE )) des_Flag[1] = 1;

        if (des_Flag[0] && des_Flag[1]) {
            result = 1;
            break;
        }
        if (deadLine_Flag0[0] && deadLine_Flag0[1]) {
            result = 0;
            break;
        }
        if (deadLine_Flag1[0] && deadLine_Flag1[1]) {
            result = 0;
            break;
        }
        Ball_display3(ballLocation[0], ballLocation[1]);
    }
    if(nextStage == STAGE1) {
        if (result == 1) {
            stage = nextStage;
            status = MENU;
        }
        else if (result == 0) status = FAIL;
    }
    else {
        if (result == 1) stage = nextStage;
        else if (result == 0) status = FAIL;
    }
}


void Ball_display1(int ballLocationX, int ballLocationY){

}
void Ball_display2(int ballLocationX, int ballLocationY){

}
void Ball_display3(int ballLocationX, int ballLocationY){

}

int set_Mobility(){
    int temp = readTEMP();
    mobility = 1;
    /*
    if ( (-10<temp) && (temp<60) ){
        mobility = temp + 10;
    }
    else if(temp < -10){
        mobility = 1;
    }
    else if(60 < temp){
        mobility = 60;
    }
    else;*/
    return mobility;
}

void HW_init(){
    init_accel();
    textLCD_Init();
    textLCD_off();
    //int mobility = set_Mobility();
    printf("HW INIT\n");
}

void HW_close(){
    close_accel();
}

int angle_x(){
    int ANGLE_X;
    int accel_y = read_accel(Y);
    int accel_z = read_accel(Z);
    if(accel_z == 0){
        ANGLE_X = 0;
    }
    else ANGLE_X = (int)atan(accel_y / accel_z) * (180 / 3.14);
    return ANGLE_X;
}

int angle_y(){
    int ANGLE_Y;
    int accel_x = read_accel(X);
    int accel_y = read_accel(Y);
    int accel_z = read_accel(Z);
    if(sqrt(accel_y*accel_y + accel_z*accel_z) == 0){
        ANGLE_Y = 0;
    }
    else ANGLE_Y = (int)atan(accel_x/ sqrt(accel_y*accel_y + accel_z*accel_z)) * (180 / 3.14);
    return ANGLE_Y;
}