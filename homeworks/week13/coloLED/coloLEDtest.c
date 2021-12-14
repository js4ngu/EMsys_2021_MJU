#include "colorLED.h"

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
	
    writeCOLORLED(red, green, blue);
}

//arm-linux-gnueabi-gcc coloLEDtest.c -lMyPeri -L.