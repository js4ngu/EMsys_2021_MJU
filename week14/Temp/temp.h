#ifndef _TEMP_H_
#define _TEMP_H_

#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>

#include "temp.h"

int spi_init(char filename[40]);
char * spi_read_lm74(int file);
int readTEMP(void);
#endif
