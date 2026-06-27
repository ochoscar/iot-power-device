#include "tasks/LightTask.h"
#include "DeviceConstants.h"
#include "Device.h"
#include "handlers/LCDHandler.h"
#include "handlers/AWSHandler.h"
#include "handlers/PublishVariables.h"

void lightTask(void* param) {
    Device* device = static_cast<Device*>(param);
    PublishVariables publish = {
        .txAC = false,
        .txRain = false,
        .txHT = false,
        .txSoil = false,
        .txLight = true,
        .txCo2 = false,
        .init = false
    };

    while (true) {
        device->taskDelay(pdMS_TO_TICKS(FIVEMINUTES_MS));
        float lux = device->getLightMeter()->readLightLevel();
        device->getSerial()->printf("Light: %.2f lux\n", lux);
        device->setEnvironmentLight(lux);
        publishMessage(device, publish);
        printLCD("Cima IoTTx LIGHT", LCD_FIRST_CHAR, LCD_FIRST_ROW);
        printLCD(String(lux, 2) + " lux", LCD_FIRST_CHAR, LCD_SECOND_ROW);
    }
}