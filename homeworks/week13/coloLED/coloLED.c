#include "colorLED.h"

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

void pwmEnable(int bEnable , int pwmIndex) {
	char	strshellcmd[150];	
	if ( bEnable) {
		sprintf(strshellcmd,"echo '1' > %s%d/pwm0/%s\n",PWM_BASE_SYS_PATH,pwmIndex,PWM_ENABLE_NAME);
		printf(strshellcmd);
		system(strshellcmd);
	}
	else {
		sprintf(strshellcmd,"echo '0' > %s%d/pwm0/%s\n",PWM_BASE_SYS_PATH,pwmIndex,PWM_ENABLE_NAME);
		printf(strshellcmd);
		system(strshellcmd);
	}
}

// 65537 ~ 1000000000  Hz
void writePWMPeriod(int frequency, int pwmIndex)  {
	char	strshellcmd[150];	
	sprintf(strshellcmd,"echo '%d' > %s%d/pwm0/%s\n",frequency,PWM_BASE_SYS_PATH, pwmIndex,PWM_FREQUENCY_NAME);
	printf(strshellcmd);
	system(strshellcmd);	
}

void writePWMDuty(int  DutyCycle , int pwmIndex) {
	char	strshellcmd[150];	
	sprintf(strshellcmd,"echo '%d' > %s%d/pwm0/%s\n",DutyCycle,PWM_BASE_SYS_PATH,pwmIndex,PWM_DUTYCYCLE_NAME);
	printf(strshellcmd);
	system(strshellcmd);	
}

	
void writeCOLORLED(int red, int green, int blue){
    // inverse
	red = MAX_INPUT_VALUE - red;
	green = MAX_INPUT_VALUE - green;
	blue = MAX_INPUT_VALUE - blue;
	
	// percentage
	int redduty = PWM_FREQUENCY * red / MAX_INPUT_VALUE;
	int greenduty = PWM_FREQUENCY * green / MAX_INPUT_VALUE;
	int blueduty = PWM_FREQUENCY * blue / MAX_INPUT_VALUE;
	
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
}