#include "led.h"

int main(void)
{
    ledLibInit();

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

    ledLibExit();
    return 0;
}
