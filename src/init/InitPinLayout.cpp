#include "device.h"
#include "DeviceConstants.h"
#include "init/InitPinLayout.h"

void initPinLayout(Device* device) {
    device->getSerial()->println("Initializing pin layout...");
    device->pinModeFunc(KEYBOARD_LEFT_PIN, INPUT_PULLUP);
    device->pinModeFunc(KEYBOARD_RIGHT_PIN, INPUT_PULLUP);
    device->pinModeFunc(KEYBOARD_UP_PIN, INPUT_PULLUP);
    device->pinModeFunc(KEYBOARD_DOWN_PIN, INPUT_PULLUP);
    device->pinModeFunc(KEYBOARD_OK_PIN, INPUT_PULLUP);
    device->pinModeFunc(PIR_PIN, INPUT);
    device->pinModeFunc(STATUS_LED_PIN, OUTPUT);
    device->pinModeFunc(AC_CONTROL_PIN, OUTPUT);
    device->pinModeFunc(IR_RECEIVE_PIN, INPUT);
    device->pinModeFunc(RAIN_SENSOR_PIN, INPUT_PULLUP);
    device->pinModeFunc(SOIL_HUMIDITY_CONTROL_PIN, OUTPUT);
    device->pinModeFunc(I2C_SCL, OUTPUT);
    device->getSerial()->println("Init pin layout done. OK.\n");
    device->delayFunc(DELAY_BETWEEN_INIT_STAGES_MS);
}