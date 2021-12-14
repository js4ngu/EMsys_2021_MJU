all: acc.h acc.c
	arm-linux-gnueabi-gcc -c acc.c -o acc.o
	arm-linux-gnueabi-ar rc libMyPeri.a acc.o

clear: rm -f *.o *.a