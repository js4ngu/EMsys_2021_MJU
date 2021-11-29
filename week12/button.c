#include "button.h"

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

