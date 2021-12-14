#ifndef _ACC_H_
#define _ACC_H_

#define ACCELPATH "/sys/class/misc/FreescaleAccelerometer/"

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int readAcc_x(void);
int readAcc_y(void);
int readAcc_z(void);

#endif
