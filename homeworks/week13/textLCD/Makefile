all: textlcd.h textlcd.c
	arm-linux-gnueabi-gcc -c textlcd.c -o textlcd.o
	arm-linux-gnueabi-ar rc libMyPeri.a textlcd.o

clear: rm -f *.o *.a