CC=arm-linux-gnueabi-gcc
AR=arm-linux-gnueabi-ar

all: libMyPeri.a testButton.out testLED.out testBuzzer.out

testButton.out: buttontest.c libMyPeri.a
	$(CC) buttontest.c -o testButton.out -lMyPeri -L. -lpthread
testLED.out: ledtest.c libMyPeri.a
	$(CC) ledtest.c -o testLED.out -lMyPeri -L.
testBuzzer.out: buzzertest.c libMyPeri.a
	$(CC) buzzertest.c -o testBuzzer.out -lMyPeri -L.

libMyPeri.a: button.o led.o buzzer.o
	$(AR) rc libMyPeri.a led.o button.o buzzer.o

button.o: button.h button.c
	arm-linux-gnueabi-gcc -c button.c -lpthread -o button.o
led.o: led.h led.c
	arm-linux-gnueabi-gcc -c led.c -o led.o
buzzer.o: buzzer.h buzzer.c
	arm-linux-gnueabi-gcc -c buzzer.c -o buzzer.o

clean: 
	rm -f *.o *.a
	rm -f $(TARGET)