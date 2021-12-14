#include "led.h"

int main(void)
{
    ledInit();

    for (int i = 0; i < 8; i++)
    {
        ledOnOff(i, ON);
        ledStatus();
        sleep(1);
    }
    for (int i = 0; i < 8; i++)
    {
        ledOnOff(i, OFF);
        ledStatus();
        sleep(1);
    }

    ledExit();
    return 0;
}
