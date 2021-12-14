CC=arm-linux-gnueabi-gcc
AR=arm-linux-gnueabi-ar

all: memo.out
	scp *.out ecube@210.99.114.80:/home/ecube/EMsys
	
memo.out: memo.c
	$(CC) memo.c -o memo.out

clean: 
	rm -f *.o *.a *.out
	rm -f $(TARGET)