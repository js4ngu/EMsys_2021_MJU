CC=arm-linux-gnueabi-gcc
AR=arm-linux-gnueabi-ar

all: ttaheuheuk.out
	scp *.out ecube@192.168.0.6:/home/ecube/EMsys
	
ttaheuheuk.out: Device_test.c libMyPeri.a
	$(CC) Device_test.c -o ttaheuheuk.out -lMyPeri -L. -lpthread -lm

clean: 
	rm -f *.o *.a *.out
	rm -f $(TARGET)