#include "gyro.h"

int readGyro_x(void){
    int fd = 0;
    FILE *fp = NULL;

    int gyro[3];
    int gyro_x;
    //Gyroscope
    fd = open(GYROPATH "enable", O_WRONLY);
    dprintf(fd, "1");
    close(fd);
    fp = fopen(GYROPATH "data", "rt");

    
    fscanf(fp, "%d, %d, %d", &gyro[0], &gyro[1], &gyro[2]);
    gyro_x = gyro[0];
    //printf("I read Gyroscope %d, %d, %d\r\n", gyro[0], gyro[1], gyro[2]);
    fclose(fp);
    return gyro_x;
}

int readGyro_y(void){
    int fd = 0;
    FILE *fp = NULL;

    int gyro[3];
    int gyro_y;
    //Gyroscope
    fd = open(GYROPATH "enable", O_WRONLY);
    dprintf(fd, "1");
    close(fd);
    fp = fopen(GYROPATH "data", "rt");

    
    fscanf(fp, "%d, %d, %d", &gyro[0], &gyro[1], &gyro[2]);
    gyro_y = gyro[1];
    //printf("I read Gyroscope %d, %d, %d\r\n", gyro[0], gyro[1], gyro[2]);
    fclose(fp);
    return gyro_y;
}

int readGyro_z(void){
    int fd = 0;
    FILE *fp = NULL;

    int gyro[3];
    int gyro_z;
    //Gyroscope
    fd = open(GYROPATH "enable", O_WRONLY);
    dprintf(fd, "1");
    close(fd);
    fp = fopen(GYROPATH "data", "rt");

    
    fscanf(fp, "%d, %d, %d", &gyro[0], &gyro[1], &gyro[2]);
    gyro_z = gyro[2];
    //printf("I read Gyroscope %d, %d, %d\r\n", gyro[0], gyro[1], gyro[2]);
    fclose(fp);
    return gyro_z;
}