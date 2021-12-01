#include "magne.h"


int readMagne_x(void) {
    int magne[3];
    int magne_x;
    int fd = 0;
    FILE *fp = NULL;

    //Magnetometer
    fd = open(MAGNEPATH "enable", O_WRONLY);
    dprintf(fd, "1");
    close(fd);
    fp = fopen(MAGNEPATH "data", "rt");

    //fscanf(fp, "%d", &magne_x);
    fscanf(fp, "%d, %d, %d", &magne[0], &magne[1], &magne[2]);
    //printf("I read Magneto %d, %d, %d\r\n", magne[0], magne[1], magne[2]);
    magne_x = magne[0];
    fclose(fp);

    return magne_x;
}

int readMagne_y(void) {
    int magne[3];
    int magne_y;
    int fd = 0;
    FILE *fp = NULL;

    //Magnetometer
    fd = open(MAGNEPATH "enable", O_WRONLY);
    dprintf(fd, "1");
    close(fd);
    fp = fopen(MAGNEPATH "data", "rt");

    fscanf(fp, "%d, %d, %d", &gyro[0], &gyro[1], &gyro[2]);
    //printf("I read Magneto %d, %d, %d\r\n", magne[0], magne[1], magne[2]);
    magne_y = magne[1];
    fclose(fp);
    return magne_y;
}

int readMagne_z(void) {
    int magne[3];
    int magne_z;
    int fd = 0;
    FILE *fp = NULL;
    //Magnetometer
    fd = open(MAGNEPATH "enable", O_WRONLY);
    dprintf(fd, "1");
    close(fd);
    fp = fopen(MAGNEPATH "data", "rt");

    //fscanf(fp, "%d", &magne_z);
    fscanf(fp, "%d, %d, %d", &magne[0], &magne[1], &magne[2]);
    //printf("I read Magneto %d, %d, %d\r\n", magne[0], magne[1], magne[2]);
    magne_z = magne[2];
    fclose(fp);
    return magne_z;
}