#include "device.h"
#include "DeviceConstants.h"
#include "init/InitDeviceMode.h"
#include "handlers/LCDHandler.h"

void initDeviceMode(Device* device) {
    device->getSerial()->println("Initializing device mode...");
    unsigned long initialTime = device->millisFunc();
    unsigned long currentTime = device->millisFunc();
    int keyboardOkStatus = 0;
    
    do {
        keyboardOkStatus = device->digitalReadFunc(KEYBOARD_OK_PIN);
        currentTime = device->millisFunc() - initialTime;
    } while(keyboardOkStatus == PUSHED_BUTTON && currentTime < SETTINGS_MODE_TIMEOUT_MS);
    int deviceMode = keyboardOkStatus == PUSHED_BUTTON ? SETTINGS_MODE : NORMAL_MODE;
    device->setDeviceMode(deviceMode);
    printLCD(deviceMode == SETTINGS_MODE ? "SETTINGS MODE" : "NORMAL MODE", LCD_FIRST_CHAR, LCD_SECOND_ROW);
    device->getSerial()->println(
        deviceMode == SETTINGS_MODE
        ? "init mode done. Settings mode OK.\n" 
        : "init mode done. Normal mode OK.\n" 
    );
    device->delayFunc(DELAY_BETWEEN_INIT_STAGES_MS);
}