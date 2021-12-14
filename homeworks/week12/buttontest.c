#include "button.h"

static int msgID = 0;

int main(void)
{
    msgID = buttonInit();
    int returnValue = 0;
    BUTTON_MSG_T messageRxData;
    returnValue = msgrcv(msgID, &messageRxData, sizeof(int), 0, 0);
    if (returnValue < 0)
        printf("Receive Key Failed\r\n");
    switch (messageRxData.keyInput)
    {
    case KEY_VOLUMEUP:
        printf("Volume up key");
        break;
    case KEY_HOME:
        printf("Home key");
        break;
    case KEY_SEARCH:
        printf("Search key");
        break;
    case KEY_BACK:
        printf("Back key");
        break;
    case KEY_MENU:
        printf("Menu key");
        break;
    case KEY_VOLUMEDOWN:
        printf("Volume down key");
        break;
    }
    
    printf("\r\n");
    buttonExit();
    return 0;
}