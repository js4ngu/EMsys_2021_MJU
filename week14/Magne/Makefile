all: magne.h magne.c
	arm-linux-gnueabi-gcc -c magne.c -o magne.o
	arm-linux-gnueabi-ar rc libMyPeri.a magne.o

clear: rm -f *.o *.a