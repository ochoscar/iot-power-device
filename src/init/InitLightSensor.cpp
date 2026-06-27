#include <BH1750.h>
#include <Wire.h>
#include "init/InitLightSensor.h"
#include "Device.h"
#include "DeviceConstants.h"
#include "handlers/LCDHandler.h"

void initLight(Device* device) {
    if(!device->getEnableLight()) {
        device->getSerial()->println("No Light Sensor enabled. Skipping CO2 initialization.");
        printLCD("SKIPPING LIGHT", LCD_FIRST_CHAR, LCD_SECOND_ROW);
        device->delayFunc(DELAY_BETWEEN_INIT_STAGES_MS);
        return;
    }
    device->getSerial()->println("Initializing Light Sensor...");
    BH1750* lightMeter = device->lightMeterFactory();
    bool init = lightMeter->begin(BH1750::CONTINUOUS_HIGH_RES_MODE, BH1750_ADDRESS);
    if (!init) {
        printLCD("LIGHT ERROR", LCD_FIRST_CHAR, LCD_SECOND_ROW);
        device->getSerial()->println("Light Sensor Error. Restarting.\n");
        device->delayFunc(RESTART_DELAY_MS);
        device->getESP()->restart();
    }
    device->setLightMeter(lightMeter);
    printLCD("Light Sensor OK", LCD_FIRST_CHAR, LCD_SECOND_ROW);
	device->getSerial()->println("Init Light Sensor done. OK.\n");
    device->delayFunc(DELAY_BETWEEN_INIT_STAGES_MS);
}