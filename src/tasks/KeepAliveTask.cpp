#include <freertos/projdefs.h>
#include "DeviceConstants.h"
#include "handlers/LCDHandler.h"
#include "Dependencies.h"
#include "Device.h"
#include "tasks/KeepAliveTask.h"

void keepAliveTask(void* param) {
    Device* device = static_cast<Device*>(param);
    
    while (true) {
        if (device->getAWSPubSubclient()) {
			bool response = device->getAWSPubSubclient()->loop();
			if (!response) {
			  printLCD("AWS RESTART", LCD_FIRST_CHAR, LCD_SECOND_ROW);
			  Serial.println("\nRestart by AWS");
			  device->delayFunc(RESTART_DELAY_MS);
			  device->getESP()->restart();
			}

			if(device->getWIFI()->status() != WL_CONNECTED) {
				printLCD("WIFI RESTART", LCD_FIRST_CHAR, LCD_SECOND_ROW);
				device->getSerial()->println("\nRestart by WiFi");
				device->getESP()->restart();
				return;
			}
        }
        device->taskDelay(pdMS_TO_TICKS(TASK_TICKS_200));
    }
}