#include "tasks/HTTask.h"
#include "DeviceConstants.h"
#include "Device.h"
#include "handlers/LCDHandler.h"
#include "handlers/AWSHandler.h"
#include "handlers/PublishVariables.h"

void htTask(void* param) {
    Device* device = static_cast<Device*>(param);
    PublishVariables publish = {
        .txAC = false,
        .txRain = false,
        .txHT = true,
        .txSoil = false,
        .txLight = false,
        .txCo2 = false,
        .init = false
    };

    while (true) {
        device->taskDelay(pdMS_TO_TICKS(FIVEMINUTES_MS)); 
        float temp = device->getSHT()->readTemperature();
        float hum = device->getSHT()->readHumidity();
        if (!isnan(temp) && !isnan(hum)) {
            device->getSerial()->printf("Temp: %.2f °C\tHum: %.2f %%\n", temp, hum);
            device->setEnvironmentTemp(temp);
            device->setEnvironmentHumidity(hum);
            publishMessage(device, publish);
            printLCD("Cima IoT-Tx HT", LCD_FIRST_CHAR, LCD_FIRST_ROW);
		    printLCD(String(temp, 2) + " " + String(hum, 2), LCD_FIRST_CHAR, LCD_SECOND_ROW);
        } else {
            device->getSerial()->println("Error reading temperature or humidity.");
        }
    }
}