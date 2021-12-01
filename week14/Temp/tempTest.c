#include "temp.h"

int main(int argc, char **argv){
	int value = readTEMP();
	printf("%d \n", value);

	return 0;
}

////arm-linux-gnueabi-gcc tempTest.c -lMyPeri -L.
