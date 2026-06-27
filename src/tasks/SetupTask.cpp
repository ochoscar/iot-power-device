#include <freertos/projdefs.h>
#include "DeviceConstants.h"
#include "handlers/LCDHandler.h"
#include "Dependencies.h"
#include "Device.h"
#include "tasks/SetupTask.h"
#include <loops/SetupLoop.h>

void setupTask(void* param) {
    Device* device = static_cast<Device*>(param);
    SetupLoop* setupObject = new SetupLoop(device);

    while (true) {
        if (device->getDeviceMode() == SETTINGS_MODE) {
            setupObject->setupLoop();
        }
        device->taskDelay(pdMS_TO_TICKS(TASK_TICKS_100));
    }
}