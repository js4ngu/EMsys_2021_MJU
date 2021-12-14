#ifndef _GYRO_H_
#define _GYRO_H_

#define GYROPATH "/sys/class/misc/FreescaleGyroscope/"

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int readGyro_x(void);
int readGyro_y(void);
int readGyro_z(void);

#endif
