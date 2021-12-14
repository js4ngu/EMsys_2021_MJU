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

void HW_init();
void Level1(int stage);
void Level2(int stage);
void Level3(int stage);

void Level1_Playgame(int destination[2], int nextStage);
void Level2_Playgame(int destination[2], int nextStage);
void Level3_Playgame(int destination[2], int nextStage);

void Ball_display1(int ballLocation[2], int hurdle_0[4]);
void Ball_display2(int ballLocation[2], int hurdle_0[4], int hurdle_1[4]);
void Ball_display3(int ballLocation[2], int hurdle_0[4], int hurdle_1[4], int hurdle_2[4]);

int set_Mobility();

int status = MENU;
int stage = STAGE1;
int key_value = 0;
int mobility;

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
            destination[0] = ;
            destination[1] = ;
            Level1_Playgame(destination[0],destination[1],STAGE2);
            break;
        case STAGE2:
            destination[0] = ;
            destination[1] = ;
            Level1_Playgame(destination[0],destination[1],STAGE3);
            break;
        case STAGE3:
            destination[0] = ;
            destination[1] = ;
            Level1_Playgame(destination[0],destination[1],STAGE4);
            break;
        case STAGE4:
            destination[0] = ;
            destination[1] = ;
            Level1_Playgame(destination[0],destination[1],STAGE5);
            break;
        case STAGE5:
            destination[0] = ;
            destination[1] = ;
            Level1_Playgame(destination[0],destination[1],STAGE6);
            break;
        case STAGE6:
            destination[0] = ;
            destination[1] = ;
            Level1_Playgame(destination[0],destination[1],STAGE7);
            break;
        case STAGE7:
            destination[0] = ;
            destination[1] = ;
            Level1_Playgame(destination[0],destination[1],STAGE8);
            break;
        case STAGE8:
            destination[0] = ;
            destination[1] = ;
            Level1_Playgame(destination[0],destination[1],STAGE1);
            break;
        default:
            break;
        }
        break;
}

void Level2(int stage){
    int destination[2];
    switch (stage) {
        case STAGE1:
            destination[0] = ;
            destination[1] = ;
            Level2_Playgame(destination[0],destination[1],STAGE2);
            break;
        case STAGE2:
            destination[0] = ;
            destination[1] = ;
            Level2_Playgame(destination[0],destination[1],STAGE3);
            break;
        case STAGE3:
            destination[0] = ;
            destination[1] = ;
            Level2_Playgame(destination[0],destination[1],STAGE4);
            break;
        case STAGE4:
            destination[0] = ;
            destination[1] = ;
            Level2_Playgame(destination[0],destination[1],STAGE5);
            break;
        case STAGE5:
            destination[0] = ;
            destination[1] = ;
            Level2_Playgame(destination[0],destination[1],STAGE6);
            break;
        case STAGE6:
            destination[0] = ;
            destination[1] = ;
            Level2_Playgame(destination[0],destination[1],STAGE7);
            break;
        case STAGE7:
            destination[0] = ;
            destination[1] = ;
            Level2_Playgame(destination[0],destination[1],STAGE8);
            break;
        case STAGE8:
            destination[0] = ;
            destination[1] = ;
            Level2_Playgame(destination[0],destination[1],STAGE1);
            break;
        default:
            break;
        }
        break;
}

void Level3(int stage){
    int destination[2];

    switch (stage) {
        case STAGE1:
            destination[0] = ;
            destination[1] = ;
            Level3_Playgame(destination[0],destination[1],STAGE2);
            break;
        case STAGE2:
            destination[0] = ;
            destination[1] = ;
            Level3_Playgame(destination[0],destination[1],STAGE3);
            break;
        case STAGE3:
            destination[0] = ;
            destination[1] = ;
            Level3_Playgame(destination[0],destination[1],STAGE4);
            break;
        case STAGE4:
            destination[0] = ;
            destination[1] = ;
            Level3_Playgame(destination[0],destination[1],STAGE5);
            break;
        case STAGE5:
            destination[0] = ;
            destination[1] = ;
            Level3_Playgame(destination[0],destination[1],STAGE6);
            break;
        case STAGE6:
            destination[0] = ;
            destination[1] = ;
            Level3_Playgame(destination[0],destination[1],STAGE7);
            break;
        case STAGE7:
            destination[0] = ;
            destination[1] = ;
            Level3_Playgame(destination[0],destination[1],STAGE8);
            break;
        case STAGE8:
            destination[0] = ;
            destination[1] = ;
            Level3_Playgame(destination[0],destination[1],STAGE1);
            break;
        default:
            break;
        }
        break;
}

void Level1_Playgame(int destination_x, int destination_y,int nextStage){
    int destination[2] = {destination_x,destination_y};
    int ballSpeed[2];
    int hurdle_0[4]  = {x1,x2,y1,y2};

    int des_Flag[2] = {0,0};
    int deadLine_Flag0[2];
    int result = 2;

    while (1) {
        Ball_display1(ballLocation[2], hurdle_0[4]);
        ballSpeed[0] = mobility * readGyro_x();
        ballSpeed[1] = mobility * readGyro_y();
        ballLocation[0] = ballLocation[0] + ballSpeed[0];
        ballLocation[1] = ballLocation[1] + ballSpeed[1];
        

        //장애물 검출 알고리즘 입력
        if (( hurdle_0[0] -10 < ballLocation[0] ) && ( ballLocation[0] < hurdle_0[1] +10 )) deadLine_Flag0[0] = 1;
        if (( hurdle_0[2] -10 < ballLocation[1] ) && ( ballLocation[1] < hurdle_0[3] +10 )) deadLine_Flag0[1] = 1;

        if (( destination[0] -10 < ballLocation[0] ) && ( ballLocation[0] < destination[0] +10 )) des_Flag[0] = 1;
        if (( destination[1] -10 < ballLocation[1] ) && ( ballLocation[1] < destination[1] +10 )) des_Flag[1] = 1;

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

    int hurdle_0[4]  = {x1,x2,y1,y2};
    int hurdle_1[4]  = {x1,x2,y1,y2};

    int des_Flag[2] = {0,0};
    int deadLine_Flag0[2];
    int deadLine_Flag1[2];
    int result = 2;

    while (1) {
        Ball_display2(ballLocation[2], hurdle_0[4], hurdle_1[4]);
        ballSpeed[0] = mobility * readGyro_x();
        ballSpeed[1] = mobility * readGyro_y();
        ballLocation[0] = ballLocation[0] + ballSpeed[0];
        ballLocation[1] = ballLocation[1] + ballSpeed[1];
        
        //장애물 검출 알고리즘 입력
        if (( hurdle_0[0] -10 < ballLocation[0] ) && ( ballLocation[0] < hurdle_0[1] +10 )) deadLine_Flag0[0] = 1;
        if (( hurdle_0[2] -10 < ballLocation[1] ) && ( ballLocation[1] < hurdle_0[3] +10 )) deadLine_Flag0[1] = 1;
        if (( hurdle_1[0] -10 < ballLocation[0] ) && ( ballLocation[0] < hurdle_1[1] +10 )) deadLine_Flag1[0] = 1;
        if (( hurdle_1[2] -10 < ballLocation[1] ) && ( ballLocation[1] < hurdle_1[3] +10 )) deadLine_Flag1[1] = 1;

        if (( destination[0] -10 < ballLocation[0] ) && ( ballLocation[0] < destination[0] +10 )) des_Flag[0] = 1;
        if (( destination[1] -10 < ballLocation[1] ) && ( ballLocation[1] < destination[1] +10 )) des_Flag[1] = 1;

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

    int hurdle_0[4]  = {x1,x2,y1,y2};
    int hurdle_1[4]  = {x1,x2,y1,y2};
    int hurdle_2[4]  = {x1,x2,y1,y2};

    int des_Flag[2] = {0,0};
    int deadLine_Flag0[2];
    int deadLine_Flag1[2];
    int deadLine_Flag2[2];
    int result = 2;

    while (1) {
        Ball_display3(ballLocation[2], hurdle_0[4], hurdle_1[4], hurdle_2[4]);
        ballSpeed[0] = mobility * readGyro_x();
        ballSpeed[1] = mobility * readGyro_y();
        ballLocation[0] = ballLocation[0] + ballSpeed[0];
        ballLocation[1] = ballLocation[1] + ballSpeed[1];
        

        //장애물 검출 알고리즘 입력
        if (( hurdle_0[0] -10 < ballLocation[0] ) && ( ballLocation[0] < hurdle_0[1] +10 )) deadLine_Flag0[0] = 1;
        if (( hurdle_0[2] -10 < ballLocation[1] ) && ( ballLocation[1] < hurdle_0[3] +10 )) deadLine_Flag0[1] = 1;
        if (( hurdle_1[0] -10 < ballLocation[0] ) && ( ballLocation[0] < hurdle_1[1] +10 )) deadLine_Flag1[0] = 1;
        if (( hurdle_1[2] -10 < ballLocation[1] ) && ( ballLocation[1] < hurdle_1[3] +10 )) deadLine_Flag1[1] = 1;
        if (( hurdle_2[0] -10 < ballLocation[0] ) && ( ballLocation[0] < hurdle_2[1] +10 )) deadLine_Flag2[0] = 1;
        if (( hurdle_2[2] -10 < ballLocation[1] ) && ( ballLocation[1] < hurdle_2[3] +10 )) deadLine_Flag2[1] = 1;

        if (( destination[0] -10 < ballLocation[0] ) && ( ballLocation[0] < destination[1] +10 )) des_Flag[0] = 1;
        if (( destination[0] -10 < ballLocation[1] ) && ( ballLocation[1] < destination[1] +10 )) des_Flag[1] = 1;

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


void Ball_display1(int ballLocation[2], int hurdle_0[4], int hurdle_1[4]){
    //BMP파일 로드
    //boll Location이랑 or 연산
    //TFT에 뿌려주기
}
void Ball_display2(int ballLocation[2], int hurdle_0[4], int hurdle_1[4]){
    //BMP파일 로드
    //boll Location or 연산
    //TFT에 뿌려주기
}
void Ball_display3(int ballLocation[2], int hurdle_0[2], int hurdle_1[4], int hurdle_2[4]){
    //BMP파일 로드
    //boll Location or 연산
    //TFT에 뿌려주기
}

int set_Mobility(){
    int temp = int(readTemp());
    if ( (-10<temp) && (temp<60) ){
        mobility = temp + 10;
    }
    else if(temp < -10){
        mobility = 1;
    }
    else if(60 < temp){
        mobility = 60;
    }
    else;
    return mobility;
}