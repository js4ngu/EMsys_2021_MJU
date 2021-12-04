#ifndef _BUZZER_H_
#define _BUZZER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>

#define MAX_SCALE_STEP 8
#define BUZZER_BASE_SYS_PATH "/sys/bus/platform/devices/”
#define BUZZER_FILENAME "peribuzzer"
#define BUZZER_ENABLE_NAME "enable"
#define BUZZER_FREQUENCY_NAME "frequency"

void doHelp(void);
int findBuzzerSysPath();
void buzzerEnable(int bEnable);
void setFrequency(int frequency);
int buzzerInit(void);
void buzzerPlaySong(int scale);
void buzzerStopSong(void);
void buzzerExit(void);

#endif