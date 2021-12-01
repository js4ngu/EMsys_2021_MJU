#include "acc.h"

int main(int argc, char **argv){
    int acc[3];
	acc[0] = readAcc_x();
    acc[1] = readAcc_y();
    acc[2] = readAcc_z();

	printf("%d %d %d\n", acc[0], acc[1], acc[2]);

	return 0;
}

////arm-linux-gnueabi-gcc testAcc.c -lMyPeri -L.
