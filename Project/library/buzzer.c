#include "buzzer.h"

char gBuzzerBaseSysDir[128]; ///sys/bus/platform/devices/peribuzzer.XX 가 결정됨
static int fdEnable;
static int fdFrequency;
const int musicScale[MAX_SCALE_STEP] =
    {
        262, /*do*/ 294, 330, 349, 392, 440, 494, /* si */ 523};

int findBuzzerSysPath(void)
{
    DIR *dir_info = opendir(BUZZER_BASE_SYS_PATH);
    int ifNotFound = 1;
    if (dir_info != NULL)
    {
        while (1)
        {
            struct dirent *dir_entry;
            dir_entry = readdir(dir_info);
            if (dir_entry == NULL)
                break;
            if (strncasecmp(BUZZER_FILENAME, dir_entry->d_name, strlen(BUZZER_FILENAME)) == 0)
            {
                ifNotFound = 0;
                sprintf(gBuzzerBaseSysDir, "%s%s/", BUZZER_BASE_SYS_PATH, dir_entry->d_name);
            }
        }
    }
    printf("find %s\n", gBuzzerBaseSysDir);
    return ifNotFound;
}

//버저 경로 찾기: /sys/bus/platform/devices/peribuzzer.XX 의 XX를 결정하는 것
void doHelp(void)
{
    printf("Usage:\n");
    printf("buzzertest <buzzerNo> \n");
    printf("buzzerNo: \n");
    printf("do(1),re(2),mi(3),fa(4),sol(5),ra(6),si(7),do(8) \n");
    printf("if buzzerNo > 8 --> frequency = buzzerNo \n");
    printf("off(0),exit(-1) \n");
}

int buzzerInit(void)
{
    if (findBuzzerSysPath() == 0)
        return 0;

    char path[256];
    sprintf(path, "%s%s", gBuzzerBaseSysDir, BUZZER_ENABLE_NAME);
    fdEnable = open(path, O_WRONLY);

    sprintf(path, "%s%s", gBuzzerBaseSysDir, BUZZER_FREQUENCY_NAME);
    fdFrequency = open(path, O_WRONLY);

    return 1;
}

void buzzerEnable(int bEnable)
{
    if (bEnable)
        write(fdEnable, &"1", 1);
    else
        write(fdEnable, &"0", 1);
}

void setFrequency(int frequency)
{
    dprintf(fdFrequency, "%d", frequency);
}

void buzzerPlaySong(int scale)
{
    setFrequency(musicScale[scale]);
    buzzerEnable(1);
}

void buzzerStopSong(void)
{
    buzzerEnable(0);
}

int buzzerExit(void)
{
    buzzerEnable(0);
    close(fdEnable);
    close(fdFrequency);
    return 0;
}