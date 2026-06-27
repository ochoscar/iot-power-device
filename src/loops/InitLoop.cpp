#include "Device.h"
#include "DeviceConstants.h"
#include "loops/InitLoop.h"
#include "handlers/LCDHandler.h"
#include "handlers/AWSHandler.h"
#include "handlers/PublishVariables.h"


void initLoop(Device* device) {
    static bool initializationStarted = false;
    static unsigned long initStartTime = 0;
    static unsigned long lastTickTime = 0;
    PublishVariables publish = {
        .txAC = false,
        .txRain = false,
        .txHT = false,
        .txSoil = false,
        .txLight = false,
        .txCo2 = false,
        .init = true
    };

    unsigned long now = device->millisFunc();

    if (!initializationStarted) {
        device->getSerial()->println("Initializing...");
        clearLCD();
        printLCD("Initializing...", LCD_FIRST_CHAR, LCD_FIRST_ROW);
        publishMessage(device, publish);

        initStartTime = now;
        lastTickTime = now;
        initializationStarted = true;
        return;
    }

    if (now - lastTickTime >= ONESECOND_MS) {
        unsigned long elapsedSeconds = (now - initStartTime) / ONESECOND_MS;
        String message = "Elapsed: " + String(elapsedSeconds) + "s";
		printLCD(message, LCD_FIRST_CHAR, LCD_SECOND_ROW);
        device->getSerial()->printf(".");
        lastTickTime = now;
    }

    if ((now - initStartTime) >= (INIT_AWS_MAX_TIME_SEC)) {
        printLCD("AWS INIT RESTART", LCD_FIRST_CHAR, LCD_SECOND_ROW);
        device->getSerial()->println("\nRestart by Init AWS");
        device->getESP()->restart();
    }
}
