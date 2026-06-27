#include <freertos/projdefs.h>
#include "DeviceConstants.h"
#include "Dependencies.h"
#include "loops/ACIRLoop.h"
#include "tasks/ACIRTask.h"

void acIRTask(void* param) {
    Device* device = static_cast<Device*>(param);

    while (true) {
        ACIRLoop(device);
        device->taskDelay(pdMS_TO_TICKS(TASK_TICKS_50));
    }
}