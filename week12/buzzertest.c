#include "buzzer.h"

int main(int argc, char **argv)
{
    int freIndex;
    is_exit = 0;

    if (argc < 2 || findBuzzerSysPath())
    {
        printf("Error!\n");
        doHelp();
        return 1;
    }
    freIndex = atoi(argv[1]);

    printf("freIndex :%d \n", freIndex);
    if (freIndex > MAX_SCALE_STEP)
    {
        buzzerPlaySong(freIndex);
    }
    if (freIndex < 0)
    {
        printf(" <buzzerNo> under range \n");
        doHelp();
        return 1;
    }
    if (freIndex == 0) // disable
    {
        buzzerStopSong();
    }
    if (freIndex == -1)
    {
        buzzerExit();
    }
    else
    {
        setFrequency(musicScale[freIndex - 1]);
        buzzerEnable(1);
    }
    return 0;
}
