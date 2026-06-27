#include "tasks/SoilHumidityTask.h"
#include "DeviceConstants.h"
#include "Device.h"
#include "handlers/LCDHandler.h"
#include "handlers/AWSHandler.h"
#include "handlers/PublishVariables.h"

void soilHumidityTask(void* param) {
    Device* device = static_cast<Device*>(param);
    PublishVariables publish = {
        .txAC = false,
        .txRain = false,
        .txHT = false,
        .txSoil = true,
        .txLight = false,
        .txCo2 = false,
        .init = false
    };

    while (true) {
        device->taskDelay(pdMS_TO_TICKS(FIVEMINUTES_MS)); 
        device->digitalWriteFunc(SOIL_HUMIDITY_CONTROL_PIN, HIGH); 
        device->taskDelay(pdMS_TO_TICKS(ONESECOND_MS));
        int soilHumidity = MAX_ADC_RESOLUTION - device->analogReadFunc(ANALOG_INPUT_SOIL_HUMIDITY_PIN);
        device->getSerial()->printf("Soil humidity: %d\n", soilHumidity);
        device->setSoilHumidity(soilHumidity);
        publishMessage(device, publish);
        printLCD("Cima IoT-Tx SOIL", LCD_FIRST_CHAR, LCD_FIRST_ROW);
        printLCD(String(soilHumidity), LCD_FIRST_CHAR, LCD_SECOND_ROW);
        device->digitalWriteFunc(SOIL_HUMIDITY_CONTROL_PIN, LOW); 
    }
}