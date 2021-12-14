all: colorLED.h coloLED.c
	arm-linux-gnueabi-gcc -c coloLED.c -o colorLED.o
	arm-linux-gnueabi-ar rc libMyPeri.a colorLED.o

clear: rm -f *.o *.a