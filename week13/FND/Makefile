all: fnd.h fnd.c
	arm-linux-gnueabi-gcc -c fnd.c -o fnd.o
	arm-linux-gnueabi-ar rc libMyPeri.a fnd.o

clear: rm -f *.o *.a