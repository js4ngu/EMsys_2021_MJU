all: libMyPeri.a testButton.out testLED.out testBuzzer.out
libMyPeri.a: button.o led.o buzzer.o
		arm-linux-gnueabi-ar rc libMyPeri.a led.o button.o buzzer.o

button.o: button.h button.c
	arm-linux-gnueabi-gcc -c button.c -o button.o
testButton.out: button.h buttontest.c
	arm-linux-gnueabi-gcc -o testButton.out -L. -l myPeri buttontest.c -L. -l myPeri

led.o: led.h led.c
	arm-linux-gnueabi-gcc -c led.c -o led.o
testLED.out: led.h ledtest.c
	arm-linux-gnueabi-gcc -o testLED.out -L. -l myPeri ledtest.c -L. -l myPeri

buzzer.c: buzzer.h buzzer.c
	arm-linux-gnueabi-gcc -c buzzer.c -o buzzer.o
testBuzzer.out: buzzer.h buzzertest.c
	arm-linux-gnueabi-gcc -o testBuzzer.out -L. -l myPeri buzzertest.c -L. -l myPeri

clear: rm -f *.o *.a *.out