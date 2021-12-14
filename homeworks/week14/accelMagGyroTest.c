#include "accelMagGyro.h"

int main(void)
{
    init_accel();
    init_mag();
    init_gyro();

    printf("Accel Data: %d, %d, %d\r\n", read_accel(X), read_accel(Y), read_accel(Z));
    printf("Magnet Data: %d, %d, %d\r\n", read_mag(X), read_mag(Y), read_mag(Z));
    printf("Gyro Data: %d, %d, %d\r\n", read_gyro(X), read_gyro(Y), read_gyro(Z));
    
    int close_accel();
    int close_mag();
    int close_gyro();

    return 0;
}
