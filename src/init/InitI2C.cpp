#include <Wire.h>
#include "init/InitI2C.h"
#include "Device.h"
#include "DeviceConstants.h"

void initI2C(Device* device) {
    device->getSerial()->println("Initializing I2C...");
    for (int i = 0; i < 10; i++) {
        device->digitalWriteFunc(SCL, HIGH);
        device->delayMicrosecondsFunc(10);
        device->digitalWriteFunc(SCL, LOW);
        device->delayMicrosecondsFunc(10);
    }
    device->getWire()->begin(I2C_SDA, I2C_SCL);
    device->getSerial()->println("I2C Scanner...");
	for (byte address = 1; address < 127; address++) {
        device->getWire()->beginTransmission(address);
	    if (device->getWire()->endTransmission() == 0) {
		    device->getSerial()->print("I2C device found at 0x");
    		device->getSerial()->println(address, HEX);
	    }
	}
    device->getSerial()->println("I2C initialized. OK.\n");
    device->delayFunc(DELAY_BETWEEN_INIT_STAGES_MS);
}