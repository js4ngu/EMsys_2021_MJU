all: mag.h mag.c
	arm-linux-gnueabi-gcc -c mag.c -o mag.o
	arm-linux-gnueabi-ar rc libMyPeri.a mag.o

clear: rm -f *.o *.a