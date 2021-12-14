CC=arm-linux-gnueabi-gcc



all:
	$(CC) bmpoverlay.c libfbdev.c libbmp.c -o bmpoverlay -O3
	scp -P30201 bmpoverlay ecube@cheese.mju.ac.kr:/home/ecube/
