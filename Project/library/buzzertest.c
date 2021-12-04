#include "buzzer.h"

int main()
{
    if (buzzerInit() == 0)
    {
        printf("Buzzer Initialize Failed!\r\n");
        return 1;
    }

    buzzerPlaySong(0);
    usleep(500000);
    buzzerPlaySong(1);
    usleep(500000);
    buzzerPlaySong(2);
    usleep(500000);
    buzzerPlaySong(3);
    usleep(500000);
    buzzerPlaySong(4);
    usleep(500000);
    buzzerPlaySong(5);
    usleep(500000);
    buzzerPlaySong(6);
    usleep(500000);
    buzzerPlaySong(7);
    usleep(500000);
    buzzerStopSong();

    buzzerExit();

    return 0;
}
