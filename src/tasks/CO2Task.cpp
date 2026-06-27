#include "tasks/CO2Task.h"
#include "DeviceConstants.h"
#include "Device.h"
#include "handlers/LCDHandler.h"
#include "handlers/AWSHandler.h"
#include "handlers/PublishVariables.h"

void co2Task(void* param) {
    Device* device = static_cast<Device*>(param);
    uint16_t error;
    PublishVariables publish = {
        .txAC = false,
        .txRain = false,
        .txHT = false,
        .txSoil = false,
        .txLight = false,
        .txCo2 = true,
        .init = false
    };

    while (true) {
        device->taskDelay(pdMS_TO_TICKS(FIVEMINUTES_MS));
        uint16_t co2;
        float temperature, humidity;
        bool dataReady = false;
        error = device->getCO2Meter()->readMeasurement(co2, temperature, humidity);
        if (error) {
            device->getSerial()->println("Failed to read measurement");
            continue;
        }
        device->getSerial()->printf("SCD4X Sensor variables CO₂: %d ppm, Temp: %.2f °C, Hum: %.2f %%\n", co2, temperature, humidity);
        device->setCO2(co2);
        device->setEnvironmentTempSCD4X(temperature);
        device->setEnvironmentHumiditySCD4X(humidity);
        publishMessage(device, publish);
        printLCD("Cima IoT-Tx CO2", LCD_FIRST_CHAR, LCD_FIRST_ROW);
        printLCD(String(co2) + " ppm", LCD_FIRST_CHAR, LCD_SECOND_ROW);
    }
}