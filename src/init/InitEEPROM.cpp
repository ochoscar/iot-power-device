#include "device.h"
#include "DeviceConstants.h"
#include "init/InitEEPROM.h"
#include "handlers/LCDHandler.h"

void initEEPROM(Device* device) {
    device->getEEPROM()->begin(EEPROM_SIZE);
}

void initDeviceParams(Device* device) {
    device->getSerial()->println("Initializing EEMPROM Parameters...");
    String wifiSSID = DEAFULT_WIFI_SSID;
    String wifiPassword = DEFAULT_WIFI_PASSWORD;
    String enableAC = EEPROM_ON_VALUE;
    String acBrand = DEFAULT_AC_BRAND;
    String autoOff = EEPROM_ON_VALUE;
    String turnOffTime = String(NO_MOTION_TIME);
    String enableRain = EEPROM_ON_VALUE;
    String enableHT = EEPROM_ON_VALUE;
    String enableSoilHumidity = EEPROM_ON_VALUE;
    String enableLight = EEPROM_ON_VALUE;
    String enableCO2 = EEPROM_ON_VALUE;

    if(USE_EEPROM_SETTINGS) {
        wifiSSID = readEEMPROM(device, EEPROM_SSID_POSITION);
        wifiPassword = readEEMPROM(device, EEPROM_PASSWORD_POSITION);
        acBrand = readEEMPROM(device, EEPROM_AC_BRAND_POSITION);
        enableAC = readEEMPROM(device, EEPROM_ENABLE_AC_POSITION);
        autoOff = readEEMPROM(device, EEPROM_AUTOOFF_POSITION);
        turnOffTime = readEEMPROM(device, EEPROM_TURNOFF_TIME_POSITION);
        enableRain = readEEMPROM(device, EEPROM_ENABLE_RAIN_POSITION);
        enableHT = readEEMPROM(device, EEPROM_ENABLE_HT_POSITION);
        enableSoilHumidity = readEEMPROM(device, EEPROM_ENABLE_SOIL_POSITION);
        enableLight = readEEMPROM(device, EEPROM_ENABLE_LIGHT_POSITION);
        enableCO2 = readEEMPROM(device, EEPROM_ENABLE_CO2_POSITION);
    }
    device->getSerial()->println("EEMPROM parameters");
    device->getSerial()->printf("Saved WiFi SSID: %s\n", wifiSSID);
    device->getSerial()->printf("Saved WiFi Password: %s\n", wifiPassword);
    device->getSerial()->printf("Saved Enable AC: %s\n", enableAC);
    device->getSerial()->printf("Saved AC Brand: %s\n", acBrand);
    device->getSerial()->printf("Saved AutoOff: %s\n", autoOff);
    device->getSerial()->printf("Saved Turn Off Time: %s\n", turnOffTime);
    device->getSerial()->printf("Saved Enable Rain: %s\n", enableRain);
    device->getSerial()->printf("Saved Enable HT: %s\n", enableHT);
    device->getSerial()->printf("Saved Enable Soil: %s\n", enableSoilHumidity);
    device->getSerial()->printf("Saved Enable Light: %s\n", enableLight);
    device->getSerial()->printf("Saved Enable CO2: %s\n", enableCO2);
    device->getSerial()->println("EEMPROM parameters read done.\n");
    
    device->setWIFISSID(wifiSSID);
    device->setWIFIPassword(wifiPassword);
    device->setEnableAC(enableAC.equals(EEPROM_ON_VALUE));
    device->setACBrand(acBrand);
    device->setACAutoOff(autoOff.equals(EEPROM_ON_VALUE));
    device->setACTurnOffTime(turnOffTime.toInt());
    device->setEnableRain(enableRain.equals(EEPROM_ON_VALUE));
    device->setEnableHT(enableHT.equals(EEPROM_ON_VALUE));
    device->setEnableSoilHumidity(enableSoilHumidity.equals(EEPROM_ON_VALUE));
    device->setEnableLight(enableLight.equals(EEPROM_ON_VALUE));
    device->setEnableCO2(enableCO2.equals(EEPROM_ON_VALUE));
    printLCD("EEMPROM OK", LCD_FIRST_CHAR, LCD_SECOND_ROW);
    device->getSerial()->println("Init read EEPROM parameters done. OK.\n");
    device->delayFunc(DELAY_BETWEEN_INIT_STAGES_MS);
}

String readEEMPROM(Device* device, int initPosition) {
    String data = "";
    for(int i = initPosition; i < (initPosition + EEPROM_MAX_WORD_SIZE); i++) {
        char c = (char)device->getEEPROM()->read(i);
        if (c == '\0') break;
        data.concat(c);
    }
    return data;
}