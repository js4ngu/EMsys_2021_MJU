#include "led.h"

static unsigned int ledValue = 0;
static int fd = 0;

enum LedState
{
    OFF = 0,
    ON
};

void doHelp(void)
{
    printf("ledtest <hex byte> :data bit0 operation 1=>on 0=>off\n");
    printf(" ledtest 0x05 ;4th and 1th led on\n");
    printf(" ledtest 0xff ;all led on\n");
    printf(" ledtest 0x00 ;all led off\n");
}

int ledLibInit(void)
{
    fd = open(LED_DRIVER_NAME, O_WRONLY);
    if (fd < 0)
    {
        perror("driver (//dev//cnled) open error.\n");
        return 1;
    }
    ledValue = 0;
}

int ledOnOff(int ledNum, int onOff) // ON or OFF
{
    int i = 1;
    i = i << ledNum;
    ledValue = ledValue & (~i);
    if (onOff != 0)
        ledValue |= i;
    write(fd, &ledValue, 4);
}

int ledStatus(void)
{
    bool is_led_on = false;
    read(fd, &ledValue, 8);
    printf("LED ON : ");
    for(int i = 0;i<8;i++)
    {
        if(ledValue % 2 == 1)
        printf("%d ", i);
        ledValue = ledValue >> 1;
    }
}

int ledLibExit(void)
{
    ledValue = 0;
    ledOnOff(0, 0);
    close(fd);
}

