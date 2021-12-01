#ifndef _COLORLED_H_
#define _COLORLED_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <ctype.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "colorLED.h"


#define MAX_SCALE_STEP		7

#define PWM_BASE_SYS_PATH       "/sys/class/pwm/pwmchip"

#define PWM_RED_BASE_SYS_PATH	"/sys/class/pwm/pwmchip0/pwm0/"
#define PWM_GREEN_BASE_SYS_PATH	"/sys/class/pwm/pwmchip1/pwm0/"
#define PWM_BLUE_BASE_SYS_PATH	"/sys/class/pwm/pwmchip2/pwm0/"

#define PWM_SHOW	"1"
#define PWM_HIDE	"0"

#define PWM_ENABLE_NAME		"enable"
#define PWM_FREQUENCY_NAME	"period"
#define PWM_DUTYCYCLE_NAME	"duty_cycle"

#define RED_INDEX	0
#define GREEN_INDEX	1
#define BLUE_INDEX	2

#define TRUE	1
#define FALSE	0

#define  PWM_FREQUENCY		100000

#define  MAX_INPUT_VALUE	100

void doHelp(void);
void pwmActivate(int bActivate, int pwmIndex);
void pwmEnable(int bEnable , int pwmIndex);
void writePWMPeriod(int frequency, int pwmIndex) ;
void writePWMDuty(int  DutyCycle , int pwmIndex);
void writeCOLORLED(int red, int green, int blue);

#endif
