all: gyro.h gyro.c
	arm-linux-gnueabi-gcc -c gyro.c -o gyro.o
	arm-linux-gnueabi-ar rc libMyPeri.a gyro.o

clear: rm -f *.o *.a