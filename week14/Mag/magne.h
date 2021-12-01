#ifndef _MAGNE_H_
#define _MAGNE_H_

#define MAGNEPATH "/sys/class/misc/FreescaleMagnetometer/"

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int readMagne_x(void);
int readMagne_y(void);
int readMagne_z(void);

#endif
