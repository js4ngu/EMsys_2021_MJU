CC=arm-linux-gnueabi-gcc
AR=arm-linux-gnueabi-ar

all: libMyPeri.a
	mv *.a ..

libMyPeri.a: accelMagGyro.o bitmap.o button.o buzzer.o colorLED.o display.o fnd.o led.o temp.o textLCD.o
	$(AR) rc libMyPeri.a accelMagGyro.o bitmap.o button.o buzzer.o colorLED.o display.o fnd.o led.o temp.o textLCD.o

accelMagGyro.o: accelMagGyro.h accelMagGyro.c
	$(CC) -c accelMagGyro.c -o accelMagGyro.o
bitmap.o: bitmap.c bitmap.h
	$(CC) -c bitmap.c -o bitmap.o
button.o: button.h button.c
	$(CC) -c button.c -lpthread -o button.o
buzzer.o: buzzer.h buzzer.c
	$(CC) -c buzzer.c -o buzzer.o
colorLED.o: colorLED.h colorLED.c
	$(CC) -c colorLED.c -o colorLED.o
display.o: display.h display.c
	$(CC) -c display.c -o display.o
fnd.o: fnd.h fnd.c
	$(CC) -c fnd.c -o fnd.o
led.o: led.h led.c
	$(CC) -c led.c -o led.o
temp.o: temp.h temp.c
	$(CC) -c temp.c -o temp.o
textLCD.o: textLCD.h textLCD.c
	$(CC) -c textLCD.c -o textLCD.o

clean: 
	rm -f *.o *.a *.out
	rm -f $(TARGET)