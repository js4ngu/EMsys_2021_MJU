CC=arm-linux-gnueabi-gcc
AR=arm-linux-gnueabi-ar

all: ttaheuheuk.out
	scp *.out ecube@210.99.114.80:/home/ecube/EMsys
	
ttaheuheuk.out: main.c libMyPeri.a
	$(CC) main.c -o ttaheuheuk.out -lMyPeri -L. -lpthread -lm

clean: 
	rm -f *.o *.a *.out
	rm -f $(TARGET)