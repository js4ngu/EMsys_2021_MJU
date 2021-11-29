#include "led.h"

int main(int argc, char **argv)
{
    unsigned int data = 0;
    enum LedState led_state;
    int fd;
    int led_num;
    if (argc < 2)
    {
        perror(" Args number is less than 2\n");
        doHelp();
        return 1;
    }
    data = strtol(argv[1], NULL, 16); //String을 16진수로 가정하고 integer형으로 변환
    printf("wrote data :0x%X\n", data);
    ledLibInit();

    led_state = ON;
    led_num = 2;

    ledOnOff(led_num, led_state);
    ledStatus();
    ledLibExit();
    return 0;
}
