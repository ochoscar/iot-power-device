#ifndef LCD_HANDLER_H
#define LCD_HANDLER_H

#include <LiquidCrystal_I2C.h>

void clearLCD();
void printLCD(String text, int col, int row);

#endif