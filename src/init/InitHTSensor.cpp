#include "init/InitHTSensor.h"
#include "Device.h"
#include "DeviceConstants.h"
#include "handlers/LCDHandler.h"

void initHT(Device* device) {
    if(!device->getEnableHT()) {
        device->getSerial()->println("No HT Sensor enabled. Skipping CO2 initialization.");
        printLCD("SKIPPING HT", LCD_FIRST_CHAR, LCD_SECOND_ROW);
        device->delayFunc(DELAY_BETWEEN_INIT_STAGES_MS);
        return;
    }
    device->getSerial()->println("Initializing HT Sensor...");
    Adafruit_SHT31* sht30 = device->sHTFactory();
    bool init = sht30->begin(SHT_ADDRESS);
    if (!init) {
        printLCD("HT ERROR", LCD_FIRST_CHAR, LCD_SECOND_ROW);
        device->getSerial()->println("HT Sensor Error. Restarting.\n");
        device->delayFunc(RESTART_DELAY_MS);
        device->getESP()->restart();
    }
    device->setSHT(sht30);
	printLCD("HT Sensor OK", LCD_FIRST_CHAR, LCD_SECOND_ROW);
	device->getSerial()->println("Init HT Sensor done. OK.\n");
    device->delayFunc(DELAY_BETWEEN_INIT_STAGES_MS);
}