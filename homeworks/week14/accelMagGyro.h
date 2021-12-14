#pragma once
#ifndef _ACCELMAGGYRO_H_
#define _ACCELMAGGYRO_H_

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define ACCELPATH "/sys/class/misc/FreescaleAccelerometer/"
#define MAGNEPATH "/sys/class/misc/FreescaleMagnetometer/"
#define GYROPATH "/sys/class/misc/FreescaleGyroscope/"

#define X (0)
#define Y (1)
#define Z (2)

// Accelerometer

int init_accel(void);
int read_accel(int axis);
int close_accel(void);

// Magnetometer

int init_mag(void);
int read_mag(int axis);
int close_mag(void);

// Gyroscope
int init_gyro(void);
int read_gyro(int axis);
int close_gyro(void);

#endif
