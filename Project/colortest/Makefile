CC=arm-linux-gnueabi-gcc
AR=arm-linux-gnueabi-ar

all: colorbar.out colorbar_Jiho.out
	scp *.out ecube@192.168.0.6:/home/ecube/EMsys
colorbar.out: colorbar.c
	$(CC) colorbar.c -o colorbar.out

colorbar_Jiho.out: colorbar_Jiho.c libMyPeri.a
	$(CC) colorbar_Jiho.c -o colorbar_Jiho.out -lMyPeri -L. -lpthread -lm

clean: 
	rm -f *.o *.a *.out
	rm -f $(TARGET)

