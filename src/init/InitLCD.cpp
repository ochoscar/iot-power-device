#include <LiquidCrystal_I2C.h>
#include "device.h"
#include "DeviceConstants.h"
#include "init/InitLCD.h"
#include "handlers/LCDHandler.h"

void initLCD(Device* device) {
	device->getSerial()->println("Initializing LCD...");
	LiquidCrystal_I2C* lcd = device->lcdFactory(LCD_ADDRESS, LCD_COLUMNS, LCD_ROWS);
	device->setLCD(lcd);
	lcd->init();
	lcd->backlight();
	clearLCD();
	printLCD("Cima IoT", LCD_FIRST_CHAR, LCD_FIRST_ROW);
	printLCD("LCD OK", LCD_FIRST_CHAR, LCD_SECOND_ROW);
	device->getSerial()->println("Init LCD done. OK.\n");
	device->delayFunc(DELAY_BETWEEN_INIT_STAGES_MS);
}