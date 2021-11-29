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

void doHelp(void)
{
	printf("Usage:Color LED\n");
	printf("colorledtest  red green blue\n");
	printf("color range 0 ~ 100\n");
	printf("ex) colorledtest  100 100 100 =>white color, all full on\n");
}

void pwmActivate(int bActivate, int pwmIndex)
{
	char	strshellcmd[150];
	if ( bActivate)
	{
		sprintf(strshellcmd,"echo '0' > %s%d/export\n",PWM_BASE_SYS_PATH,pwmIndex);
		printf(strshellcmd);
		system(strshellcmd);
	}
	else
	{
		sprintf(strshellcmd,"echo '0' > %s%d/unexport\n",PWM_BASE_SYS_PATH,pwmIndex);
		printf(strshellcmd);
		system(strshellcmd);
	}
}

void pwmEnable(int bEnable , int pwmIndex)
{
	char	strshellcmd[150];	
	if ( bEnable)
	{
		sprintf(strshellcmd,"echo '1' > %s%d/pwm0/%s\n",PWM_BASE_SYS_PATH,pwmIndex,PWM_ENABLE_NAME);
		printf(strshellcmd);
		system(strshellcmd);
	}
	else
	{
		sprintf(strshellcmd,"echo '0' > %s%d/pwm0/%s\n",PWM_BASE_SYS_PATH,pwmIndex,PWM_ENABLE_NAME);
		printf(strshellcmd);
		system(strshellcmd);
	}
}

// 65537 ~ 1000000000  Hz
void writePWMPeriod(int frequency, int pwmIndex) 
{
	char	strshellcmd[150];	
	sprintf(strshellcmd,"echo '%d' > %s%d/pwm0/%s\n",frequency,PWM_BASE_SYS_PATH, pwmIndex,PWM_FREQUENCY_NAME);
	printf(strshellcmd);
	system(strshellcmd);	
}

void writePWMDuty(int  DutyCycle , int pwmIndex)
{
	char	strshellcmd[150];	
	sprintf(strshellcmd,"echo '%d' > %s%d/pwm0/%s\n",DutyCycle,PWM_BASE_SYS_PATH,pwmIndex,PWM_DUTYCYCLE_NAME);
	printf(strshellcmd);
	system(strshellcmd);	
}

int main(int argc , char **argv)
{
	int red,green,blue;
	int redduty,greenduty,blueduty;
	
	if (argc < 4 )
	{
		perror(" Args number is less than 4\n");
		doHelp();
		return 1;
	}

	red = atoi(argv[1]);
	green = atoi(argv[2]);
	blue = atoi(argv[3]);
	
	if ( (red > MAX_INPUT_VALUE) || (red < 0))
	{
		perror("red over range.\n");
		doHelp();
		return 1;
	}

	if ( (green > MAX_INPUT_VALUE) || (green < 0))
	{
		perror("green over range.\n");
		doHelp();
		return 1;
	}

	if ( (blue > MAX_INPUT_VALUE) || (blue < 0))
	{
		perror("blue over range.\n");
		doHelp();
		return 1;
	}
	
	// inverse
	red = MAX_INPUT_VALUE - red;
	green = MAX_INPUT_VALUE - green;
	blue = MAX_INPUT_VALUE - blue;
	
	// percentage
	redduty = PWM_FREQUENCY * red / MAX_INPUT_VALUE;
	greenduty = PWM_FREQUENCY * green / MAX_INPUT_VALUE;
	blueduty = PWM_FREQUENCY * blue / MAX_INPUT_VALUE;
	
	pwmActivate(TRUE,RED_INDEX );
	writePWMPeriod(PWM_FREQUENCY, RED_INDEX);
	writePWMDuty(redduty, RED_INDEX);
	pwmEnable(TRUE, RED_INDEX);
	
	pwmActivate(TRUE,GREEN_INDEX );
	writePWMPeriod(PWM_FREQUENCY, GREEN_INDEX);
	writePWMDuty(greenduty, GREEN_INDEX);
	pwmEnable(TRUE, GREEN_INDEX);
	
	pwmActivate(TRUE,BLUE_INDEX );
	writePWMPeriod(PWM_FREQUENCY, BLUE_INDEX);
	writePWMDuty(blueduty, BLUE_INDEX);
	pwmEnable(TRUE, BLUE_INDEX);
	
	return 0;
	
}
