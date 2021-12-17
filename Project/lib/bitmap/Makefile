CC=arm-linux-gnueabi-gcc
AR=arm-linux-gnueabi-ar

all: bitmap.out
	scp *.out *.bmp ecube@192.168.0.6:/home/ecube/EMsys
bitmap.out: bitmap.c
	$(CC) bitmap.c -o bitmap.out

clean: 
	rm -f *.o *.a *.out
	rm -f $(TARGET)

