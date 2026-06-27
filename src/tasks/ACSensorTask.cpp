#include <freertos/projdefs.h>
#include "DeviceConstants.h"
#include "Device.h"
#include "tasks/ACSensorTask.h"
#include "loops/ACSensorLoop.h"

void acSensorTask(void* param) {
    Device* device = static_cast<Device*>(param);

    while (true) {
        if (device->getDeviceInitialized()) {
            acSensorLoop(device);
        }
        device->taskDelay(pdMS_TO_TICKS(TASK_TICKS_100));
    }
}