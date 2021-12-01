#include "gyro.h"

int main(int argc, char **argv){
    int gyro[3];
	gyro[0] = readGyro_x();
    gyro[1] = readGyro_y();
    gyro[2] = readGyro_z();

	printf("%d %d %d\n", gyro[0], gyro[1], gyro[2]);

	return 0;
}

////arm-linux-gnueabi-gcc tempTest.c -lMyPeri -L.
