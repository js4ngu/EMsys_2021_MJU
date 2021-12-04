#include "buzzer.h"

int main(void)
{
    if (buzzerInit() == 0)
	{
		printf ("Buzzer Initialized Failed\r\n");
		return 1;
    }

    buzzerPlaySong(0);
    usleep(200000);
    buzzerPlaySong(4);
    usleep(200000);
    buzzerPlaySong(7);
    usleep(200000);
    buzzerStopSong();
    usleep(400000);

    buzzerPlaySong(1);
    usleep(200000);
    buzzerPlaySong(5);
    usleep(200000);
    buzzerPlaySong(8);
    usleep(200000);
    buzzerStopSong();
    usleep(400000);

    buzzerPlaySong(2);
    usleep(200000);
    buzzerPlaySong(6);
    usleep(200000);
    buzzerPlaySong(9);
    usleep(200000);
    buzzerStopSong();
    usleep(400000);

    buzzerPlaySong(3);
    usleep(200000);
    buzzerPlaySong(7);
    usleep(200000);
    buzzerPlaySong(10);
    usleep(200000);
    buzzerStopSong();
    usleep(400000);
    
    buzzerPlaySong(4);
    usleep(200000);
    buzzerPlaySong(8);
    usleep(200000);
    buzzerPlaySong(11);
    usleep(200000);
    buzzerStopSong();
    usleep(400000);

    buzzerExit();
    return 0;
}
