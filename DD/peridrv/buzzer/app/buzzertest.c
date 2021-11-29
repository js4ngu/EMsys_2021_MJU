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

#define MAX_SCALE_STEP		8

#define BUZZER_BASE_SYS_PATH	"/sys/bus/platform/devices/peribuzzer."

#define BUZZER_ENABLE_NAME		"enable"
#define BUZZER_FREQUENCY_NAME	"frequency"
char gBuzzerBaseSysDir[128];

const int musicScale[MAX_SCALE_STEP] =
{
	262, // do
	294,
	330,
	349,
	392,
	440,
	494,  // si
	523
};


void doHelp(void)
{
	printf("Usage:\n");
	printf("buzzertest <buzzerNo> \n");
	printf("buzzerNo: \n");
	printf("do(1),re(2),mi(3),fa(4),sol(5),ra(6),si(7),do(8) \n");
	printf("off(0)\n");
}

int findBuzzerSysPath()
{
	char str[128];
	char result; 
	char *pdel; 
	char * s; 
	sprintf(str,"ls -d %s*", BUZZER_BASE_SYS_PATH); 
	FILE* p = popen(str, "r");
	if ( p == NULL)
	{
		printf("popen fail \n"); 
		return -1; 
	}
	// str ==> ex) /sys/bus/platform/devices/peribuzzer.30
	s = fgets(str,128,p);
	if ( s == NULL )
	{
		printf("fgets fail\n"); 
		return -1; 
	}
	pdel = strrchr(s,(int)'.');
	if ( pdel == NULL ) 
	{
		printf(" . is not found\n"); 
		return -1; 
	}
	int i=0;
	while(1)
	{
		if ( pdel[i] == 0x0A )
		{
			pdel[i] = 0; 
			break; 
		}
		i++;
	}
	sprintf(gBuzzerBaseSysDir,"%s%s/",BUZZER_BASE_SYS_PATH,pdel+1);
	printf("find %s\n",gBuzzerBaseSysDir);
	pclose(p);

	return 0; 
}

void buzzerEnable(int bEnable)
{
	char	strshellcmd[150];	
	if ( bEnable)
	{
		sprintf(strshellcmd,"echo '1' > %s%s\n",gBuzzerBaseSysDir,BUZZER_ENABLE_NAME);
//		printf(strshellcmd);
		system(strshellcmd);
	}
	else
	{
		sprintf(strshellcmd,"echo '0' > %s%s\n",gBuzzerBaseSysDir,BUZZER_ENABLE_NAME);
//		printf(strshellcmd);
		system(strshellcmd);
	}
}
// 1 ~ 1000  Hz
void setFrequency(int frequency) 
{
	char	strshellcmd[150];	
	sprintf(strshellcmd,"echo '%d' > %s%s\n",frequency,gBuzzerBaseSysDir,BUZZER_FREQUENCY_NAME);
//	printf(strshellcmd);
	system(strshellcmd);	
}

int main(int argc , char **argv)
{
	int freIndex;
	
	if (argc < 2 )
	{
		perror(" Args number is less than 2\n");
		doHelp();
		return 1;
	}
	if (findBuzzerSysPath() == -1)
	{
		printf("There is no buzzer sys directory\n");
		return -1; 
	}

	freIndex = atoi(argv[1]);
	

	printf("freIndex  :%d \n",freIndex);

	if ( freIndex > MAX_SCALE_STEP )
	{
		perror(" <buzzerNo> over range \n");
		doHelp();
		return 1;
	}
	
	if ( freIndex == 0)// disable
	{
		buzzerEnable(0);
		return 0;
	}
	
	setFrequency(musicScale[freIndex-1]);
	
	buzzerEnable(1);
	
	return 0;
}
