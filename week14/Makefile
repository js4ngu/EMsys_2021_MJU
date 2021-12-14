CC=arm-linux-gnueabi-gcc
AR=arm-linux-gnueabi-ar

all: libMyPeri.a accelMagGyroTest.out

accelMagGyroTest.out: accelMagGyroTest.c libMyPeri.a
	$(CC) accelMagGyroTest.c -o accelMagGyroTest.out -lMyPeri -L.

libMyPeri.a: accelMagGyro.o
	$(AR) rc libMyPeri.a accelMagGyro.o

accelMagGyro.o: accelMagGyro.h accelMagGyro.c
	$(CC) -c accelMagGyro.c -o accelMagGyro.o

clean: 
	rm -f *.o *.a *.out
	rm -f $(TARGET)