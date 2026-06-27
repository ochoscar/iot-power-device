#include <freertos/projdefs.h>
#include "DeviceConstants.h"
#include "handlers/LCDHandler.h"
#include "Dependencies.h"
#include "Device.h"
#include "tasks/InitTask.h"
#include <loops/InitLoop.h>

void initTask(void* param) {
    Device* device = static_cast<Device*>(param);

    while (!device->getDeviceInitialized()) {
        initLoop(device);
        device->taskDelay(pdMS_TO_TICKS(TASK_TICKS_100));
    }
    device->taskDelete(NULL);
}