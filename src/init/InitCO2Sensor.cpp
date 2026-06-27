#include <SensirionI2CScd4x.h>
#include <Wire.h>
#include "init/InitCO2Sensor.h"
#include "Device.h"
#include "DeviceConstants.h"
#include "handlers/LCDHandler.h"

void initCO2(Device* device) {
    if(!device->getEnableCO2()) {
        device->getSerial()->println("No CO2 sensor enabled. Skipping CO2 initialization.");
        printLCD("SKIPPING CO2", LCD_FIRST_CHAR, LCD_SECOND_ROW);
        device->delayFunc(DELAY_BETWEEN_INIT_STAGES_MS);
        return;
    }
    device->getSerial()->println("Initializing CO2 Sensor...");
    SensirionI2cScd4x* scd4x = device->co2Factory();
    scd4x->begin(*device->getWire(), CO2_ADDRESS);
    device->delayFunc(ONESECOND_MS);
    scd4x->stopPeriodicMeasurement();
    uint16_t error = scd4x->startPeriodicMeasurement();
    if (error) {
        printLCD("CO2 ERROR", LCD_FIRST_CHAR, LCD_SECOND_ROW);
        device->getSerial()->println("CO2 Sensor Error. Restarting.\n");
        device->delayFunc(RESTART_DELAY_MS);
        device->getESP()->restart();
    }
    device->setCO2Meter(scd4x);
	printLCD("CO2 Sensor OK", LCD_FIRST_CHAR, LCD_SECOND_ROW);
	device->getSerial()->println("Init CO2 Sensor done. OK.\n");
    device->delayFunc(DELAY_BETWEEN_INIT_STAGES_MS);
}