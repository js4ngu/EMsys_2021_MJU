#include "button.h"

int fp
int fd;
int msgID;
pthread_t buttonTh_id;

char inputDevPath[200] = {
        0,
    };

void* buttonThFunc(void* arg)
{
    struct BUTTON_MSG_T messageTxData;
    int msgQueue = msgget(MESSAGE_ID, IPC_CREAT | 0666);
    FILE *fp = fopen(PROBE_FILE, "rt");
    while (1)
    {
        readSize = read(fp, &stEvent, sizeof(stEvent));
        if (readSize != sizeof(stEvent))
        {
            continue;
        }
        if (stEvent.type == EV_KEY)
        {
            printf("EV_KEY(");
            switch (stEvent.code)
            {
            case KEY_VOLUMEUP:
                printf("Volume up key):");
                messageTxData.keyInput = 0;
                break;
            case KEY_HOME:
                printf("Home key):");
                messageTxData.keyInput = 1;
                break;
            case KEY_SEARCH:
                printf("Search key):");
                messageTxData.keyInput = 2;
                break;
            case KEY_BACK:
                printf("Back key):");
                messageTxData.keyInput = 3;
                break;
            case KEY_MENU:
                printf("Menu key):");
                messageTxData.keyInput = 4;
                break;
            case KEY_VOLUMEDOWN:
                printf("Volume down key):");
                messageTxData.keyInput = 5;
                break;
            }
            if (stEvent.value)
                printf("pressed\n");
                messageTxData.pressed = 1;
            else
                printf("released\n");
                messageTxData.pressed = 0;
        }     //End of if
        else  // EV_SYN
            ; // do notthing
    }         // End of While
}

int probeButtonPath(char *newPath)
{
    int returnValue = 0;                //button에 해당하는 event#을 찾았나?
    int number = 0;                     //찾았다면 여기에 집어넣자
    FILE *fp = fopen(PROBE_FILE, "rt"); //파일을 열고

    while (!feof(fp)) //끝까지 읽어들인다.
    {
        char tmpStr[200];       //200자를 읽을 수 있게 버퍼
        fgets(tmpStr, 200, fp); //최대 200자를 읽어봄
        //printf ("%s",tmpStr);
        if (strcmp(tmpStr, HAVE_TO_FIND_1) == 0)
        {
            printf("YES! I found!: %s\r\n", tmpStr);
            returnValue = 1; //찾음
        }
        if (
            (returnValue == 1) && //찾은 상태에서
            (strncasecmp(tmpStr, HAVE_TO_FIND_2,
                         strlen(HAVE_TO_FIND_2)) == 0) //Event??을 찾았으면
        )
        {
            printf("-->%s", tmpStr);
            printf("\t%c\r\n", tmpStr[strlen(tmpStr) - 3]);
            number = tmpStr[strlen(tmpStr) - 3] - '0';
            //Ascii character '0'-'9' (0x30-0x39)
            //to interger(0)
            break;
        }
    }
    fclose(fp);
    if (returnValue == 1)
        sprintf(newPath, "%s%d", INPUT_DEVICE_LIST, number);
    return returnValue;
}

int buttonInit(void)
{
    if (probeButtonPath(inputDevPath) == 0)
        return 0;
    fd = open(buttonPath, O_RDONLY);
    msgID = msgget(MESSAGE_ID, IPC_CREAT | 0666);
    pthread_create(&buttonTh_id, NULL, &buttonThFunc, NULL);
    return 1;
}

int buttonStatus(void)
{
    struct BUTTON_MSG_T messageRxData;
    if(msgID == -1)
    {
        printf("Message Not Correctly Initialized!\r\n");
        return -1;
    }
    while(1)
    {
        int returnValue = 0;
        returnValue = msgrcv(msgID, &messageRxData, sizeof(messageRxData))
    }
}