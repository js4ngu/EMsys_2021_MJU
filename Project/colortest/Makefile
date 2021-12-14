CC=arm-linux-gnueabi-gcc
AR=arm-linux-gnueabi-ar

all: colorbar.out
	scp *.out ecube@210.99.114.80:/home/ecube/EMsys
	
colorbar.out: colorbar.c
	$(CC) colorbar.c -o colorbar.out

clean: 
	rm -f *.o *.a *.out
	rm -f $(TARGET)