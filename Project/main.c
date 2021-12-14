#include <math.h>

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

#define WIDTH 51
#define HIGH 30
#define DES_SENSTIVE 60

const int DESTINATION_S1 [8][2]  = {{0,0},
                                    {0,0}};
const int DESTINATION_S2 [8][2]  = {{0,0},
                                    {0,0}};
const int DESTINATION_S3 [8][2]  = {{0,0},
                                    {0,0}};

const int HURDLE_0[2] = {0,}; //왼쪽 위 꼭지점 좌표 hurdle_0[0] : x, hurdle_0[1] : y
const int HURDLE_1[2] = {0,};
const int HURDLE_2[2] = {0,};

void HW_init();
void HW_close();

void Level1(int stage);
void Level2(int stage);
void Level3(int stage);

void Level1_Playgame(int destinationX, int destinationY, int nextStage);
void Level2_Playgame(int destinationX, int destinationY, int nextStage);
void Level3_Playgame(int destinationX, int destinationY, int nextStage);

void Ball_display1(int ballLocationX, int ballLocationY);
void Ball_display2(int ballLocationX, int ballLocationY);
void Ball_display3(int ballLocationX, int ballLocationY);

int set_Mobility();
int angle_x(void);
int angle_y(void);
int Ball_speed(char axis);

int ballLocation[2] = {0,0};
int status = MENU;
int stage = STAGE1;
int key_value = 0;
int mobility = 0;

void main(){
    HW_init();
    switch (status) {
    case MENU:
        TFT_Display();
        if (key_value == START) {
            status = LEVEL1;
        }
        else if(key_value == EXIT) {
            //status;
        }
        break;
        
    case LEVEL1:
        Level1(stage);
        break;

    case LEVEL2:
        Level2(stage);
        break;

    case LEVEL3:
        Level3(stage);
        break;

    case FAIL:
        break;

    default:
        status = MENU;
        stage = STAGE1;
        break;
    }
}

/*
1. 화면을 받고
2. 버튼 : 쓰레드
3. 자이로 : 쓰레도
4. 게임 : 쓰레드
5. 화면 출력 : 쓰레드
*/

void Level1(int stage){
    int destination[2];
    switch (stage) {
        case STAGE1:
            destination[0] = DESTINATION_S1[0][0];
            destination[1] = DESTINATION_S1[0][1];
            Level1_Playgame(destination[0],destination[1],STAGE2);
            break;
        case STAGE2:
            destination[0] = DESTINATION_S1[1][0];
            destination[1] = DESTINATION_S1[1][1];
            Level1_Playgame(destination[0],destination[1],STAGE3);
            break;
        case STAGE3:
            destination[0] = DESTINATION_S1[2][0];
            destination[1] = DESTINATION_S1[2][1];
            Level1_Playgame(destination[0],destination[1],STAGE4);
            break;
        case STAGE4:
            destination[0] = DESTINATION_S1[3][0];
            destination[1] = DESTINATION_S1[3][1];
            Level1_Playgame(destination[0],destination[1],STAGE5);
            break;
        case STAGE5:
            destination[0] = DESTINATION_S1[4][0];
            destination[1] = DESTINATION_S1[4][1];
            Level1_Playgame(destination[0],destination[1],STAGE6);
            break;
        case STAGE6:
            destination[0] = DESTINATION_S1[5][0];
            destination[1] = DESTINATION_S1[5][1];
            Level1_Playgame(destination[0],destination[1],STAGE7);
            break;
        case STAGE7:
            destination[0] = DESTINATION_S1[6][0];
            destination[1] = DESTINATION_S1[6][1];
            Level1_Playgame(destination[0],destination[1],STAGE8);
            break;
        case STAGE8:
            destination[0] = DESTINATION_S1[7][1];
            destination[1] = DESTINATION_S1[7][1];
            Level1_Playgame(destination[0],destination[1],STAGE1);
            break;
        default:
            break;
        }

}

void Level2(int stage){
    int destination[2];
    switch (stage) {
        case STAGE1:
            destination[0] = DESTINATION_S2[0][0];
            destination[1] = DESTINATION_S2[0][0];
            Level2_Playgame(destination[0],destination[1],STAGE2);
            break;
        case STAGE2:
            destination[0] = DESTINATION_S2[1][0];
            destination[1] = DESTINATION_S2[1][1];
            Level2_Playgame(destination[0],destination[1],STAGE3);
            break;
        case STAGE3:
            destination[0] = DESTINATION_S2[2][0];
            destination[1] = DESTINATION_S2[2][1];
            Level2_Playgame(destination[0],destination[1],STAGE4);
            break;
        case STAGE4:
            destination[0] = DESTINATION_S2[3][0];
            destination[1] = DESTINATION_S2[3][1];
            Level2_Playgame(destination[0],destination[1],STAGE5);
            break;
        case STAGE5:
            destination[0] = DESTINATION_S2[4][0];
            destination[1] = DESTINATION_S2[4][1];
            Level2_Playgame(destination[0],destination[1],STAGE6);
            break;
        case STAGE6:
            destination[0] = DESTINATION_S2[5][0];
            destination[1] = DESTINATION_S2[5][1];
            Level2_Playgame(destination[0],destination[1],STAGE7);
            break;
        case STAGE7:
            destination[0] = DESTINATION_S2[6][0];
            destination[1] = DESTINATION_S2[6][1];
            Level2_Playgame(destination[0],destination[1],STAGE8);
            break;
        case STAGE8:
            destination[0] = DESTINATION_S2[7][0];
            destination[1] = DESTINATION_S2[7][1];
            Level2_Playgame(destination[0],destination[1],STAGE1);
            break;
        default:
            break;
    }
}

void Level3(int stage){
    int destination[2];
    switch (stage) {
        case STAGE1:
            destination[0] = DESTINATION_S3[0][0];
            destination[1] = DESTINATION_S3[0][1];
            Level3_Playgame(destination[0],destination[1],STAGE2);
            break;
        case STAGE2:
            destination[0] = DESTINATION_S3[1][0];
            destination[1] = DESTINATION_S3[1][1];
            Level3_Playgame(destination[0],destination[1],STAGE3);
            break;
        case STAGE3:
            destination[0] = DESTINATION_S3[2][0];
            destination[1] = DESTINATION_S3[2][1];
            Level3_Playgame(destination[0],destination[1],STAGE4);
            break;
        case STAGE4:
            destination[0] = DESTINATION_S3[3][0];
            destination[1] = DESTINATION_S3[3][1];
            Level3_Playgame(destination[0],destination[1],STAGE5);
            break;
        case STAGE5:
            destination[0] = DESTINATION_S3[4][0];
            destination[1] = DESTINATION_S3[4][1];
            Level3_Playgame(destination[0],destination[1],STAGE6);
            break;
        case STAGE6:
            destination[0] = DESTINATION_S3[5][0];
            destination[1] = DESTINATION_S3[5][1];
            Level3_Playgame(destination[0],destination[1],STAGE7);
            break;
        case STAGE7:
            destination[0] = DESTINATION_S3[6][0];
            destination[1] = DESTINATION_S3[6][1];
            Level3_Playgame(destination[0],destination[1],STAGE8);
            break;
        case STAGE8:
            destination[0] = DESTINATION_S3[7][0];
            destination[1] = DESTINATION_S3[7][1];
            Level3_Playgame(destination[0],destination[1],STAGE1);
            break;
        default:
            break;
        }
        
}

void Level1_Playgame(int destination_x, int destination_y,int nextStage){
    int destination[2] = {destination_x,destination_y};
    int ballSpeed[2];

    int des_Flag[2] = {0,0};
    int deadLine_Flag0[2] = 0;
    int result = 2;

    while (1) {
        Ball_display1(ballLocation[0], ballLocation[1]);
        ballSpeed[0] = mobility * Ball_speed(X);
        ballSpeed[1] = mobility * Ball_speed(Y);
        ballLocation[0] = ballLocation[0] + ballSpeed[0];
        ballLocation[1] = ballLocation[1] + ballSpeed[1];
        
        //장애물 검출 알고리즘 입력
        if (( HURDLE_0[0] - WIDTH < ballLocation[0] ) && ( ballLocation[0] < HURDLE_0[0] + WIDTH )) deadLine_Flag0[0] = 1;
        if (( HURDLE_0[1] - HIGH  < ballLocation[1] ) && ( ballLocation[1] < HURDLE_0[1] + HIGH )) deadLine_Flag0[1] = 1;

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
        else if (result == 0) status = FAIL;
    }
}

void Level2_Playgame(int destination_x, int destination_y, int nextStage){
    int destination[2] = {destination_x,destination_y};
    int ballSpeed[2];

    int des_Flag[2] = {0,0};
    int deadLine_Flag0[2] = 0;
    int deadLine_Flag1[2] = 0;
    int result = 2;

    while (1) {
        Ball_display2(ballLocation[0], ballLocation[1]);
        ballSpeed[0] = mobility * Ball_speed(X);
        ballSpeed[1] = mobility * Ball_speed(Y);
        ballLocation[0] = ballLocation[0] + ballSpeed[0];
        ballLocation[1] = ballLocation[1] + ballSpeed[1];
        
        //장애물 검출 알고리즘 입력
        if (( HURDLE_0[0] - WIDTH < ballLocation[0] ) && ( ballLocation[0] < HURDLE_0[0] + WIDTH )) deadLine_Flag0[0] = 1;
        if (( HURDLE_0[1] - HIGH  < ballLocation[1] ) && ( ballLocation[1] < HURDLE_0[1] + HIGH )) deadLine_Flag0[1] = 1;
        if (( HURDLE_1[0] - WIDTH < ballLocation[0] ) && ( ballLocation[0] < HURDLE_1[0] + WIDTH )) deadLine_Flag1[0] = 1;
        if (( HURDLE_1[1] - HIGH  < ballLocation[1] ) && ( ballLocation[1] < HURDLE_1[1] + HIGH )) deadLine_Flag1[1] = 1;

        if (( destination[0] - DES_SENSTIVE < ballLocation[1] ) && ( ballLocation[1] < destination[1] + DES_SENSTIVE )) des_Flag[0] = 1;
        if (( destination[0] - DES_SENSTIVE < ballLocation[1] ) && ( ballLocation[1] < destination[1] + DES_SENSTIVE )) des_Flag[1] = 1;

        if (des_Flag[0] && des_Flag[1]) {
            result = 1;
            break;
        }
        if (deadLine_Flag0[0] && deadLine_Flag0[1]) {
            result = 1;
            break;
        }
        if (deadLine_Flag1[0] && deadLine_Flag1[1]) {
            result = 1;
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
    int deadLine_Flag0[2] = 0;
    int deadLine_Flag1[2] = 0;
    int deadLine_Flag2[2] = 0;
    int result = 2;

    while (1) {
        Ball_display3(ballLocation[0], ballLocation[1]);
        ballSpeed[0] = mobility * Ball_speed(X);
        ballSpeed[1] = mobility * Ball_speed(Y);
        ballLocation[0] = ballLocation[0] + ballSpeed[0];
        ballLocation[1] = ballLocation[1] + ballSpeed[1];
        
        //장애물 검출 알고리즘 입력
        if (( HURDLE_0[0] - WIDTH < ballLocation[0] ) && ( ballLocation[0] < HURDLE_0[0] + WIDTH )) deadLine_Flag0[0] = 1;
        if (( HURDLE_0[1] - HIGH  < ballLocation[1] ) && ( ballLocation[1] < HURDLE_0[1] + HIGH )) deadLine_Flag0[1] = 1;
        if (( HURDLE_1[0] - WIDTH < ballLocation[0] ) && ( ballLocation[0] < HURDLE_1[0] + WIDTH )) deadLine_Flag1[0] = 1;
        if (( HURDLE_1[1] - HIGH  < ballLocation[1] ) && ( ballLocation[1] < HURDLE_1[1] + HIGH )) deadLine_Flag1[1] = 1;
        if (( HURDLE_2[0] - WIDTH < ballLocation[0] ) && ( ballLocation[1] < HURDLE_2[0] + WIDTH )) deadLine_Flag2[1] = 1;
        if (( HURDLE_2[1] - HIGH  < ballLocation[1] ) && ( ballLocation[1] < HURDLE_2[1] + HIGH )) deadLine_Flag2[1] = 1;

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
        if (deadLine_Flag2[0] && deadLine_Flag2[1]) {
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
    //BMP파일 로드
    //HURDLE_0 이랑 or 연산
    //boll Location이랑 or 연산
    //TFT에 뿌려주기
}
void Ball_display2(int ballLocationX, int ballLocationY){
    //BMP파일 로드
    //HURDLE_0 이랑 or 연산
    //HURDLE_1 이랑 or 연산
    //boll Location이랑 or 연산
    //TFT에 뿌려주기
}
void Ball_display3(int ballLocationX, int ballLocationY){
    //BMP파일 로드
    //HURDLE_0 이랑 or 연산
    //HURDLE_1 이랑 or 연산
    //HURDLE_2 이랑 or 연산
    //boll Location이랑 or 연산
    //TFT에 뿌려주기
}

int set_Mobility(){
    int temp = readTemp();
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
    mobility = set_Mobility();
}

void HW_close(){
    close_accel();
}

int angle_x(){
    int accel_y = read_accel(Y);
    int accel_z = read_accel(Z);
    return (int)atan(accel_y / accel_z) * (180 / 3.14);
}
int angle_y(){
    int accel_x = read_accel(X);
    int accel_y = read_accel(Y);
    int accel_z = read_accel(Z);
    return (int)atan(accel_x/ sqrt(accel_y*accel_y + accel_z*accel_z)) * (180 / 3.14);
}

int Ball_speed(char axis){
    int AXIS, speed;
    if (axis == 'X') AXIS = 0;
    else if (axis == 'Y') AXIS = 1;
    else AXIS = 2;
    switch (AXIS) {
    case 0:
        speed = angle_x() / 180;
        break;
    case 1:
        speed = angle_x() / 180;
        break;
    default:
        break;
    }
    return speed;
}

