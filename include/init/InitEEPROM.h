#ifndef INIT_EEPROM_H
#define INIT_EEPROM_H

void initEEPROM(Device* device);
void initDeviceParams(Device *device);
String readEEMPROM(Device* device, int initPosition);

#endif