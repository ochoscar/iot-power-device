#ifndef SETUP_LOOP_H
#define SETUP_LOOP_H

#include <WiFi.h>
#include <WebServer.h>
#include <EEPROM.h>
#include <LiquidCrystal_I2C.h>
#include <HardwareSerial.h>

class SetupLoop {
	private:
		static Device* device;
		static WebServer* server;
		static String ssid;
		static String password;
		static String acBrand;
		static bool enableAC;
		static bool enableRain;
		static bool enableHT;
		static bool enableSoilHumidity;
		static bool enableLight;
		static bool enableCO2;
		static bool autoOff;
		static int autoOffTime;
		static int setupOption;
		static int setupSelectedOption;
		static char currentCharacter;
		static int currentWordIndex;
		static char currentWord[EEPROM_MAX_WORD_SIZE];
		static int buttonUpState;
		static int buttonDownState;
		static int buttonRightState;
		static int buttonLeftState;
		static int buttonOkState;

		static void readKeyboard();
		static void printSettingsMenu();
		static void processSelectedOption();
		static void getWordFromUserAndSaveIt();
		static void saveWordAs();
		static void saveToEEPROM(int wordInitPosition, const char* currentWord);
		static void resetMenuState();
		static void handleRoot();
		static void handleSave();
	public:
		SetupLoop(Device* device);
		void setupLoop();
};

#endif