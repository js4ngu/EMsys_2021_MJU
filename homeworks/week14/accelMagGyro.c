#include "accelMagGyro.h"

int fd_accel = 0;
int fd_mag = 0;
int fd_gyro = 0;

FILE *fp_accel = NULL;
FILE *fp_mag = NULL;
FILE *fp_gyro = NULL;

// Accelerometer

int init_accel(void)
{
    fd_accel = open(ACCELPATH "enable", O_WRONLY);
    dprintf(fd_accel, "1");
    close(fd_accel);
    fp_accel = fopen(ACCELPATH "data", "rt");
    return 0;
}

int read_accel(int axis)
{
    int accel[3];
    fscanf(fp_accel, "%d, %d, %d", &accel[0], &accel[1], &accel[2]);
    // printf("I read Accel %d, %d, %d\r\n", accel[0], accel[1], accel[2]);
    switch (axis)
    {
    case X:
        return accel[0];
        break;
    case Y:
        return accel[1];
        break;
    case Z:
        return accel[2];
        break;
    default:
        return -1;
        break;
    }
}

int close_accel(void)
{
    fd_accel = open(ACCELPATH "enable", O_WRONLY);
    dprintf(fd_accel, "0");
    close(fd_accel);
    fclose(fp_accel);
    return 0;
}

// Magnetometer

int init_mag(void)
{
    fd_mag = open(MAGNEPATH "enable", O_WRONLY);
    dprintf(fd_mag, "1");
    close(fd_mag);
    fp_mag = fopen(MAGNEPATH "data", "rt");
    return 0;
}

int read_mag(int axis)
{
    int mag[3];
    fscanf(fp_mag, "%d, %d, %d", &mag[0], &mag[1], &mag[2]);
    // printf("I read Mag %d, %d, %d\r\n", mag[0], mag[1], mag[2]);
    switch (axis)
    {
    case X:
        return mag[0];
        break;
    case Y:
        return mag[1];
        break;
    case Z:
        return mag[2];
        break;
    default:
        return -1;
        break;
    }
}

int close_mag(void)
{
    fd_mag = open(MAGNEPATH "enable", O_WRONLY);
    dprintf(fd_mag, "0");
    close(fd_mag);
    fclose(fp_mag);
    return 0;
}

// Gyroscope

int init_gyro(void)
{
    fd_gyro = open(GYROPATH "enable", O_WRONLY);
    dprintf(fd_gyro, "1");
    close(fd_gyro);
    fp_gyro = fopen(GYROPATH "data", "rt");
    return 0;
}

int read_gyro(int axis)
{
    int gyro[3];
    fscanf(fp_gyro, "%d, %d, %d", &gyro[0], &gyro[1], &gyro[2]);
    // printf("I read Gyro %d, %d, %d\r\n", gyro[0], gyro[1], gyro[2]);
    switch (axis)
    {
    case X:
        return gyro[0];
        break;
    case Y:
        return gyro[1];
        break;
    case Z:
        return gyro[2];
        break;
    default:
        return -1;
        break;
    }
}

int close_gyro(void)
{
    fd_gyro = open(GYROPATH "enable", O_WRONLY);
    dprintf(fd_gyro, "0");
    close(fd_gyro);
    fclose(fp_gyro);
    return 0;
}
