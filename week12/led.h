#ifndef _LED_H_
#define _LED_H_

#include <fcntl.h>
#include <unistd.h> 

#define LED_DRIVER_NAME "/dev/periled"
#define ON 1
#define OFF 0

void doHelp(void);
int ledLibInit(void);
int ledOnOff(int ledNum, int onOff);
int ledStatus(void);
int ledLibExit(void);

#endif _LED_H
