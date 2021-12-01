all: temp.h temp.c
	arm-linux-gnueabi-gcc -c temp.c -o temp.o
	arm-linux-gnueabi-ar rc libMyPeri.a temp.o

clear: rm -f *.o *.a