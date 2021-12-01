#include "acc.h"

int readAcc_x(void){
    int fd = 0;
    FILE *fp = NULL;
    int accel[3];
    int acc_x;

    //Accelerometer
    fd = open(ACCELPATH "enable", O_WRONLY);
    dprintf(fd, "1");
    close(fd);
    fp = fopen(ACCELPATH "data", "rt");
    
    fscanf(fp, "%d, %d, %d", &accel[0], &accel[1], &accel[2]);
    acc_x = accel[0];
    //printf("I read Accel %d, %d, %d\r\n", accel[0], accel[1], accel[2]);
    fclose(fp);

    return acc_x;
}

int readAcc_y(void){
    int fd = 0;
    FILE *fp = NULL;
    int accel[3];
    int acc_y;

    //Accelerometer
    fd = open(ACCELPATH "enable", O_WRONLY);
    dprintf(fd, "1");
    close(fd);
    fp = fopen(ACCELPATH "data", "rt");
    
    fscanf(fp, "%d, %d, %d", &accel[0], &accel[1], &accel[2]);
    acc_y = accel[1];
    //printf("I read Accel %d, %d, %d\r\n", accel[0], accel[1], accel[2]);
    fclose(fp);

    return acc_y;
}

int readAcc_z(void){
    int fd = 0;
    FILE *fp = NULL;
    int accel[3];
    int acc_z;

    //Accelerometer
    fd = open(ACCELPATH "enable", O_WRONLY);
    dprintf(fd, "1");
    close(fd);
    fp = fopen(ACCELPATH "data", "rt");
    
    fscanf(fp, "%d, %d, %d", &accel[0], &accel[1], &accel[2]);
    acc_z = accel[2];
    //printf("I read Accel %d, %d, %d\r\n", accel[0], accel[1], accel[2]);
    fclose(fp);

    return acc_z;
}