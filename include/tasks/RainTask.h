#ifndef RAIN_TASK_H
#define RAIN_TASK_H

#include <esp_attr.h>

void IRAM_ATTR rainISRHandler();
void rainTask(void* param);

#endif