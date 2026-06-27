#include "device.h"
#include "DeviceConstants.h"
#include "init/InitSerial.h"

void initSerial(Device* device) {
    device->getSerial()->begin(SERIAL_BAUD_RATE);
    device->getSerial()->println("\n\n");
    device->getSerial()->println("***************************************");
    device->getSerial()->println("CimaIoT - PowerDevice1 - Copyright@2025");
    device->getSerial()->println("***************************************");
    device->getSerial()->println("Init communications done. OK.\n");
    device->delayFunc(DELAY_BETWEEN_INIT_STAGES_MS);
}