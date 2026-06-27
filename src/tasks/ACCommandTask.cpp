#include <freertos/projdefs.h>
#include "DeviceConstants.h"
#include "handlers/LCDHandler.h"
#include "Dependencies.h"
#include "Device.h"
#include "tasks/ACCommandTask.h"
#include "ac/ACCommand.h"

void acCommandTask(void* param) {
    Device* device = static_cast<Device*>(param);
    static portMUX_TYPE acCommandMux = portMUX_INITIALIZER_UNLOCKED;
    ACCommand cmd;
    while (true) {
        if (device->queueReceive(device->getACCommandQueue(), &cmd, portMAX_DELAY)) {
            device->getAC()->setMode(device->getACMode());
            device->getAC()->setFanSpeed(device->getACFan());
            device->getAC()->setTemperature(device->getACTemp());
            device->getAC()->setPower(device->getACPower());
            device->getSerial()->println(device->getAC()->toString());
			device->taskDelay(pdMS_TO_TICKS(TASK_TICKS_500));
            device->getAC()->sendCommand();
        }
    }
}