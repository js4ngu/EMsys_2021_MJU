#include "magne.h"

int main(int argc, char **argv){
    int mange[3];
	magne[0] = readMagne_x();
    magne[1] = readMagne_y();
    magne[2] = readMagne_z();

	printf("%d %d %d\n", magne[0], magne[1], magne[2]);

	return 0;
}

////arm-linux-gnueabi-gcc tempTest.c -lMyPeri -L.
