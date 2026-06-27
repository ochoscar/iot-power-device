#include "tasks/RainTask.h"
#include "DeviceConstants.h"
#include "Device.h"
#include "handlers/LCDHandler.h"
#include "handlers/AWSHandler.h"
#include "handlers/PublishVariables.h"

volatile unsigned int rainPulseCount = 0;

void IRAM_ATTR rainISRHandler() {
    rainPulseCount++;
}

void rainTask(void* param) {
    Device* device = static_cast<Device*>(param);
    unsigned int lastCount = 0;
    PublishVariables publish = {
        .txAC = false,
        .txRain = true,
        .txHT = false,
        .txSoil = false,
        .txLight = false,
        .txCo2 = false,
        .init = false
    };

    while (true) {
        device->taskDelay(pdMS_TO_TICKS(ONEMINUTE_MS)); 

        unsigned int count = rainPulseCount;
        unsigned int pulsesInInterval = count - lastCount;
        lastCount = count;

        float mmRain = pulsesInInterval * RAIN_SENSOR_MM_PER_PULSE;
        device->getSerial()->printf("Last minute rain: %.2f mm\n", mmRain);
        if(mmRain > 0) {
            device->getSerial()->println("Rain detected! Sending message to AWS...");
            device->setRain(mmRain);
            publishMessage(device, publish);
            printLCD("Cima IoT-Tx RAIN", LCD_FIRST_CHAR, LCD_FIRST_ROW);
		    printLCD(String(mmRain), LCD_FIRST_CHAR, LCD_SECOND_ROW);
        } else {
            device->setRain(0);
            device->getSerial()->println("No rain detected.");
        }
    }
}