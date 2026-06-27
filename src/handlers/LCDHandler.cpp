#include <LiquidCrystal_I2C.h>
#include "DeviceConstants.h"
#include "Device.h"
#include "Dependencies.h"
#include "handlers/LCDHandler.h"

void completeLCDText(String* text);

void clearLCD() {
    Device* device = Dependencies::getInstance()->cimaDevice;

	device->getLCD()->setCursor(0, 0);
	device->getLCD()->print("                ");
	device->getLCD()->setCursor(0, 1);
	device->getLCD()->print("                ");
}

void printLCD(String text, int col, int row) {
    Device* device = Dependencies::getInstance()->cimaDevice;

	completeLCDText(&text);
	device->getLCD()->setCursor(col, row);
	device->getLCD()->print(text);
}

void completeLCDText(String* text) {
    if (text->length() > 16) {
        *text = text->substring(0, 16);
    } else {
        while (text->length() < 16) {
            *text += " ";
        }
    }
}