CC=arm-linux-gnueabi-gcc
AR=arm-linux-gnueabi-ar

all: ttaheuheuk.out
	scp 
	
ttaheuheuk.out: main.c libMyPeri.a
	$(CC) main.c -o ttaheuheuk.out -lMyPeri -L. -lpthread -lmath

clean: 
	rm -f *.o *.a *.out
	rm -f $(TARGET)