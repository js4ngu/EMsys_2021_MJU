CC = arm-linux-gnueabi-gcc
AR = arm-linux-gnueabi-ar

all: libMyPeri.a jukebox
	
jukebox: jukebox.c libMyPeri.a
	$(CC) jukebox.c -o jukebox -l MyPeri -L. -lpthread
	scp  jukebox ecube@210.99.114.80:/home/ecube

libMyPeri.a:	led.o buzzer.o button.o
	$(AR) rc libMyPeri.a led.o buzzer.o button.o

led.o: led.h led.c
	$(CC) led.c -o led.o -c
	
button.o: button.h button.c
	$(CC) button.c -o button.o -c
	
buzzer.o: buzzer.h buzzer.c
	$(CC) buzzer.c -o buzzer.o -c
	
clean:
	rm *.o -rf
	rm *.a -rf
	rm jukebox -rf
