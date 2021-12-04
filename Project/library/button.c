#include "button.h"

int fd;
int msgID;
pthread_t buttonTh_id;

char inputDevPath[200] = {
    0,
};

static void *buttonThFunc(void)
{
    BUTTON_MSG_T msgTx;
    msgTx.messageNum = 1;
    struct input_event stEvent;
    while (1)
    {
        read(fd, &stEvent, sizeof(stEvent));
        if ((stEvent.type == EV_KEY) && (stEvent.value == 0))
        {
            msgTx.keyInput = stEvent.code;
            msgsnd(msgID, &msgTx, sizeof(int), 0);
        }
    }
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
            (returnValue == 1) &&                                              //찾은 상태에서
            (strncasecmp(tmpStr, HAVE_TO_FIND_2, strlen(HAVE_TO_FIND_2)) == 0) //Event??을 찾았으면
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
    {
        printf("ERROR! File Not Found!\r\n");
        printf("Did you insmod?\r\n");
        return 0;
    }
    printf("inputDevPath: %s\r\n", inputDevPath);
    fd = open(inputDevPath, O_RDONLY);
    msgID = msgget(MESSAGE_ID, IPC_CREAT | 0666);
    pthread_create(&buttonTh_id, NULL, &buttonThFunc, NULL);
    return msgID;
}

int buttonStatus(void)
{
    struct BUTTON_MSG_T messageRxData;
    if (msgID == -1)
    {
        printf("Message Initialize Fail!\r\n");
        return -1;
    }
    while (1)
    {
        int returnValue = 0;
        returnValue = msgrcv(msgID, &messageRxData, sizeof(messageRxData))
    }
}

int buttonExit(void)
{
    pthread_cancel(buttonTh_id);
    close(fd);
    printf("button_test finish!\n");
}
