#include <WiFi.h>
#include <WebServer.h>
#include "device.h"
#include "DeviceConstants.h"
#include "loops/SetupLoop.h"
#include "handlers/LCDHandler.h"

const char* settingsForm = R"rawliteral(
	<!DOCTYPE html>
	<html lang="en">
	<head>
	  <meta charset="UTF-8">
	  <title>CIMA IoT Settings Portal</title>
	  <style>
		body {
		  font-family: Arial, sans-serif;
		  background-color: #f3f3f3;
		  margin: 0;
		  padding: 0;
		  text-align: center;
		}
		h1 {
		  background-color: #007BFF;
		  color: white;
		  margin: 0;
		  padding: 20px 0;
		}
		h2 {
		  margin-top: 10px;
		  color: #333;
		}
		form {
		  background-color: white;
		  padding: 30px;
		  margin: 30px auto;
		  max-width: 400px;
		  border-radius: 10px;
		  box-shadow: 0 0 10px rgba(0,0,0,0.1);
		  text-align: left;
		}
		label {
		  display: block;
		  margin-top: 15px;
		  font-weight: bold;
		}
		input[type="text"],
		input[type="password"],
		input[type="number"] {
		  width: 100%;
		  padding: 8px;
		  margin-top: 5px;
		  border: 1px solid #ccc;
		  border-radius: 5px;
		}
		input[type="checkbox"] {
		  margin-right: 8px;
		}
		.button-container {
		  text-align: center;
		  margin-top: 20px;
		}
		input[type="submit"],
		input[type="reset"] {
		  background-color: #007BFF;
		  color: white;
		  border: none;
		  padding: 10px 20px;
		  margin: 0 10px;
		  border-radius: 5px;
		  cursor: pointer;
		  font-size: 16px;
		}
		input[type="submit"]:hover,
		input[type="reset"]:hover {
		  background-color: #0056b3;
		}
		footer {
		  margin-top: 40px;
		  font-size: 14px;
		  color: #777;
		}
	  </style>
	</head>
	<body>
	  <h1>Settings Portal</h1>
	  <h2>CIMA IoT</h2>
	  <form action="/save" method="GET">
		<label>WiFi SSID:
		  <input type="text" name="ssid">
		</label>
		<label>WiFi Password:
		  <input type="password" name="password">
		</label>
		<label><input type="checkbox" name="enable-ac"> Enable AC</label>
		<label>AC brand:
		  <input type="text" name="ac-brand">
		</label>
		<label><input type="checkbox" name="auto-off"> Auto Turn Off AC</label>
		<label>Auto Turn Off AC Time (seconds):
		  <input type="number" name="time-off">
		</label>
		<label><input type="checkbox" name="enable-rain"> Enable Rain Sensor</label>
		<label><input type="checkbox" name="enable-ht"> Enable Humidity and Temperature Sensor</label>
		<label><input type="checkbox" name="enable-soil-humidity"> Enable Soil Humidity Sensor</label>
		<label><input type="checkbox" name="enable-light"> Enable Light Sensor</label>
		<label><input type="checkbox" name="enable-co2"> Enable CO2 Sensor</label>
	
		<div class="button-container">
		  <input type="submit" value="Save">
		  <input type="reset" value="Reset">
		</div>
	  </form>
	
	  <footer>
		&copy; All rights reserved
	  </footer>
	</body>
	</html>
	)rawliteral";

Device* SetupLoop::device = nullptr;
WebServer* SetupLoop::server = nullptr;
String SetupLoop::ssid = "";
String SetupLoop::password = "";
String SetupLoop::acBrand = "";
bool SetupLoop::enableAC = false;
bool SetupLoop::enableRain = false;
bool SetupLoop::enableHT = false;
bool SetupLoop::enableSoilHumidity = false;
bool SetupLoop::enableLight = false;
bool SetupLoop::enableCO2 = false;
bool SetupLoop::autoOff = true;
int SetupLoop::autoOffTime = NO_MOTION_TIME; 
int SetupLoop::setupOption = WIFI_SSID_OPTION;
int SetupLoop::setupSelectedOption = WIFI_SSID_OPTION;
char SetupLoop::currentCharacter = '\0';
int SetupLoop::currentWordIndex = 0;
char SetupLoop::currentWord[EEPROM_MAX_WORD_SIZE];
int SetupLoop::buttonUpState = PUSHED_BUTTON;
int SetupLoop::buttonDownState = PUSHED_BUTTON;
int SetupLoop::buttonRightState = PUSHED_BUTTON;
int SetupLoop::buttonLeftState = PUSHED_BUTTON;
int SetupLoop::buttonOkState = PUSHED_BUTTON;

SetupLoop::SetupLoop(Device* device) {
	device->getSerial()->println("Initializing setup main task configuration...");
	SetupLoop::device = device;
	server = device->webServerFactory(WEB_SERVER_PORT);
	device->getWIFI()->mode(WIFI_AP);
	device->getWIFI()->softAP(WIFI_AP_SSID, WIFI_PASSWORD);
	IPAddress ip = device->getWIFI()->softAPIP();
	device->getSerial()->print("IP Address: ");
	device->getSerial()->println(ip);
	printLCD(ip.toString(), LCD_FIRST_CHAR, LCD_SECOND_ROW);
	device->delayFunc(DISPLAY_IP_DELAY);
	server->on("/", handleRoot);
	server->on("/save", handleSave);
	server->begin();
	resetMenuState();
}

void SetupLoop::setupLoop() {
	printLCD("                ", LCD_FIRST_CHAR, LCD_SECOND_ROW);
	readKeyboard();
	printSettingsMenu();
	processSelectedOption();
	server->handleClient();
}

void SetupLoop::handleRoot() {
    server->send(200, "text/html", settingsForm);
}

void SetupLoop::handleSave() {
    if (server->hasArg("ssid")) ssid = server->arg("ssid");
    if (server->hasArg("password")) password = server->arg("password");
	if (server->hasArg("ac-brand")) acBrand = server->arg("ac-brand");
    autoOff = server->hasArg("auto-off");
    if (server->hasArg("time-off")) autoOffTime = server->arg("time-off").toInt();
	enableAC = server->hasArg("enable-ac");
	enableRain = server->hasArg("enable-rain");
	enableHT = server->hasArg("enable-ht");
	enableSoilHumidity = server->hasArg("enable-soil-humidity");
	enableLight = server->hasArg("enable-light");
	enableCO2 = server->hasArg("enable-co2");

	saveToEEPROM(EEPROM_SSID_POSITION, ssid.c_str());
	saveToEEPROM(EEPROM_PASSWORD_POSITION, password.c_str());
	saveToEEPROM(EEPROM_AC_BRAND_POSITION, acBrand.c_str());
	saveToEEPROM(EEPROM_AUTOOFF_POSITION, autoOff ? EEPROM_ON_VALUE : EEPROM_OFF_VALUE);
	saveToEEPROM(EEPROM_TURNOFF_TIME_POSITION, String(autoOffTime).c_str());
	saveToEEPROM(EEPROM_ENABLE_AC_POSITION, enableAC ? EEPROM_ON_VALUE : EEPROM_OFF_VALUE);
	saveToEEPROM(EEPROM_ENABLE_RAIN_POSITION, enableRain ? EEPROM_ON_VALUE : EEPROM_OFF_VALUE);
	saveToEEPROM(EEPROM_ENABLE_HT_POSITION, enableHT ? EEPROM_ON_VALUE : EEPROM_OFF_VALUE);
	saveToEEPROM(EEPROM_ENABLE_SOIL_POSITION, enableSoilHumidity ? EEPROM_ON_VALUE : EEPROM_OFF_VALUE);
	saveToEEPROM(EEPROM_ENABLE_LIGHT_POSITION, enableLight ? EEPROM_ON_VALUE : EEPROM_OFF_VALUE);
	saveToEEPROM(EEPROM_ENABLE_CO2_POSITION, enableCO2 ? EEPROM_ON_VALUE : EEPROM_OFF_VALUE);

    String response = R"rawliteral(
		<!DOCTYPE html>
		<html>
		<head>
		  <meta charset="UTF-8">
		  <title>Configuration Saved</title>
		  <style>
			body {
			  font-family: Arial, sans-serif;
			  background-color: #f3f3f3;
			  text-align: center;
			  padding: 40px;
			}
			.container {
			  background-color: #fff;
			  padding: 30px;
			  margin: auto;
			  max-width: 500px;
			  border-radius: 10px;
			  box-shadow: 0 0 10px rgba(0,0,0,0.1);
			}
			h2 {
			  color: #007BFF;
			  margin-bottom: 20px;
			}
			.item {
			  text-align: left;
			  margin-bottom: 10px;
			}
			footer {
			  margin-top: 40px;
			  font-size: 14px;
			  color: #777;
			}
		  </style>
		</head>
		<body>
		  <div class="container">
			<h2>Configuration Saved</h2>
			<div class="item"><strong>WiFi SSID:</strong> )rawliteral" + ssid + R"rawliteral(</div>
			<div class="item"><strong>WiFi Password:</strong> )rawliteral" + password + R"rawliteral(</div>
			<div class="item"><strong>AC Brand:</strong> )rawliteral" + acBrand + R"rawliteral(</div>
			<div class="item"><strong>Enable AC:</strong> )rawliteral" + String(enableAC ? "Yes" : "No") + R"rawliteral(</div>
			<div class="item"><strong>Auto Turn Off AC:</strong> )rawliteral" + String(autoOff ? "Yes" : "No") + R"rawliteral(</div>
			<div class="item"><strong>Auto Turn Off AC Time:</strong> )rawliteral" + String(autoOffTime) + R"rawliteral( seconds</div>
			<div class="item"><strong>Enable Rain Sensor:</strong> )rawliteral" + String(enableRain ? "Yes" : "No") + R"rawliteral(</div>
			<div class="item"><strong>Enable Humidity and Temperature Sensor:</strong> )rawliteral" + String(enableHT ? "Yes" : "No") + R"rawliteral(</div>
			<div class="item"><strong>Enable Soil Humidity Sensor:</strong> )rawliteral" + String(enableSoilHumidity ? "Yes" : "No") + R"rawliteral(</div>
			<div class="item"><strong>Enable Light Sensor:</strong> )rawliteral" + String(enableLight ? "Yes" : "No") + R"rawliteral(</div>
			<div class="item"><strong>Enable CO2 Sensor:</strong> )rawliteral" + String(enableCO2 ? "Yes" : "No") + R"rawliteral(</div>
		  </div>
		  <footer>&copy; All rights reserved</footer>
		</body>
		</html>
		)rawliteral";
    server->send(200, "text/html", response);
}

void SetupLoop::readKeyboard() {
	buttonUpState = device->digitalReadFunc(KEYBOARD_UP_PIN);
	buttonDownState = device->digitalReadFunc(KEYBOARD_DOWN_PIN);
	buttonRightState = device->digitalReadFunc(KEYBOARD_RIGHT_PIN);
	buttonLeftState = device->digitalReadFunc(KEYBOARD_LEFT_PIN);
	buttonOkState = device->digitalReadFunc(KEYBOARD_OK_PIN);
}

void SetupLoop::printSettingsMenu() {
	if(setupOption == 0) 					printLCD("-Select option- ", LCD_FIRST_CHAR, LCD_FIRST_ROW);
	if(setupOption == WIFI_SSID_OPTION) 	printLCD("1. WiFi/SSID    ", LCD_FIRST_CHAR, LCD_FIRST_ROW);
	if(setupOption == WIFI_PASSWORD_OPTION) printLCD("2. WiFi/Password", LCD_FIRST_CHAR, LCD_FIRST_ROW);
	if(setupOption == ENABLE_AC_OPTION) 	printLCD("3. Enable AC", LCD_FIRST_CHAR, LCD_FIRST_ROW);
	if(setupOption == AC_BRAND_OPTION) 	    printLCD("4. AC Brand", LCD_FIRST_CHAR, LCD_FIRST_ROW);
	if(setupOption == AUTOFF_OPTION) 		printLCD("5. Enable AutOff", LCD_FIRST_CHAR, LCD_FIRST_ROW);
	if(setupOption == TURNOFF_TIME_OPTION) 	printLCD("6. Turn off Time", LCD_FIRST_CHAR, LCD_FIRST_ROW);
	if(setupOption == ENABLE_RAIN_OPTION) 	printLCD("7. Enable Rain", LCD_FIRST_CHAR, LCD_FIRST_ROW);
	if(setupOption == ENABLE_HT_OPTION) 	printLCD("8. Enable HT", LCD_FIRST_CHAR, LCD_FIRST_ROW);
	if(setupOption == ENABLE_SOIL_OPTION) 	printLCD("9. Enable Soil", LCD_FIRST_CHAR, LCD_FIRST_ROW);
	if(setupOption == ENABLE_LIGHT_OPTION)  printLCD("10. Enable Light", LCD_FIRST_CHAR, LCD_FIRST_ROW);
	if(setupOption == ENABLE_CO2_OPTION) 	printLCD("11. Enable CO2", LCD_FIRST_CHAR, LCD_FIRST_ROW);
}

void SetupLoop::processSelectedOption() {
	if(setupSelectedOption == 0) 
        if(buttonRightState == PUSHED_BUTTON)
			setupOption = setupOption < MAX_SETUP_MENU_OPTIONS ? setupOption + 1 : setupOption;
        if(buttonLeftState == PUSHED_BUTTON)
			setupOption = setupOption > 1 ? setupOption - 1 : setupOption;
        if(buttonDownState == PUSHED_BUTTON)
            setupSelectedOption = setupOption;
    if(setupSelectedOption == WIFI_SSID_OPTION || setupSelectedOption == WIFI_PASSWORD_OPTION || 
		setupSelectedOption == AUTOFF_OPTION || setupSelectedOption == TURNOFF_TIME_OPTION) 
		getWordFromUserAndSaveIt();
}

void SetupLoop::getWordFromUserAndSaveIt() {
	if(buttonOkState == PUSHED_BUTTON) {
		saveWordAs();
		resetMenuState();
		return;
	} 
	if(buttonUpState == PUSHED_BUTTON)	currentCharacter++;
	if(buttonDownState == PUSHED_BUTTON)  currentCharacter--;
	if(buttonLeftState == PUSHED_BUTTON) {
		currentWord[currentWordIndex] = '_';
		currentWordIndex = currentWordIndex > 0 ? currentWordIndex - 1 : currentWordIndex;
		currentCharacter = currentWord[currentWordIndex];
	}
	if(buttonRightState == PUSHED_BUTTON) {
		currentWordIndex = currentWordIndex < EEPROM_MAX_WORD_SIZE ? currentWordIndex + 1 : currentWordIndex;
		currentCharacter = WORD_DELIMITER_CHAR;
	}
	currentWord[currentWordIndex] = currentCharacter;
	printLCD(currentWord, LCD_FIRST_CHAR, LCD_SECOND_ROW);
}

void SetupLoop::saveWordAs() {
	int wordInitPosition = 0;
	String saveMessage = "";
	if (setupSelectedOption == WIFI_SSID_OPTION) {
		wordInitPosition = EEPROM_SSID_POSITION;
		saveMessage = "Saving SSID";
	} else if(setupSelectedOption == WIFI_PASSWORD_OPTION) {
		wordInitPosition = EEPROM_PASSWORD_POSITION;
		saveMessage = "Saving Password";
	} else if(setupSelectedOption == ENABLE_AC_OPTION) {
		wordInitPosition = EEPROM_ENABLE_AC_POSITION;
		saveMessage = "Saving Enable AC";
	} else if(setupSelectedOption == AC_BRAND_OPTION) {
		wordInitPosition = EEPROM_AC_BRAND_POSITION;
		saveMessage = "Saving AC Brand";
	} else if(setupSelectedOption == AUTOFF_OPTION) {
		wordInitPosition = EEPROM_AUTOOFF_POSITION;
		saveMessage = "Saving AutoOff";
		strcpy(currentWord, String(currentWord).equals("0") ? EEPROM_ON_VALUE : EEPROM_OFF_VALUE);
	} else if(setupSelectedOption == TURNOFF_TIME_OPTION) {
		wordInitPosition = EEPROM_TURNOFF_TIME_POSITION;
		saveMessage = "Saving TurnOff";
	} else if(setupSelectedOption == ENABLE_RAIN_OPTION) {
		wordInitPosition = EEPROM_ENABLE_RAIN_POSITION;
		saveMessage = "Saving Enable Rain";
		strcpy(currentWord, String(currentWord).equals("0") ? EEPROM_ON_VALUE : EEPROM_OFF_VALUE);
	} else if(setupSelectedOption == ENABLE_HT_OPTION) {
		wordInitPosition = EEPROM_ENABLE_HT_POSITION;
		saveMessage = "Saving Enable HT";
		strcpy(currentWord, String(currentWord).equals("0") ? EEPROM_ON_VALUE : EEPROM_OFF_VALUE);
	} else if(setupSelectedOption == ENABLE_SOIL_OPTION) {
		wordInitPosition = EEPROM_ENABLE_SOIL_POSITION;
		saveMessage = "Saving Enable Soil";
		strcpy(currentWord, String(currentWord).equals("0") ? EEPROM_ON_VALUE : EEPROM_OFF_VALUE);
	} else if(setupSelectedOption == ENABLE_LIGHT_OPTION) {
		wordInitPosition = EEPROM_ENABLE_LIGHT_POSITION;
		saveMessage = "Saving Enable Light";
		strcpy(currentWord, String(currentWord).equals("0") ? EEPROM_ON_VALUE : EEPROM_OFF_VALUE);
	} else if(setupSelectedOption == ENABLE_CO2_OPTION) {
		wordInitPosition = EEPROM_ENABLE_CO2_POSITION;
		saveMessage = "Saving Enable CO2";
		strcpy(currentWord, String(currentWord).equals("0") ? EEPROM_ON_VALUE : EEPROM_OFF_VALUE);
	}
	device->getSerial()->printf("Saving information to EEMPROM\n%s\nInformation saved: %s\n", saveMessage.c_str(), currentWord);
	printLCD(saveMessage, LCD_FIRST_CHAR, LCD_FIRST_ROW);
	saveToEEPROM(wordInitPosition, currentWord);
	device->delayFunc(AFTER_SAVE_EEMPORM_DELAY);
}

void SetupLoop::resetMenuState() {
	setupOption = 0;
	setupSelectedOption = 0;
	currentCharacter = 'A';
	currentWordIndex = 0;
	for(int i = 0; i < EEPROM_MAX_WORD_SIZE; i++) currentWord[i] = '\0';
}

void SetupLoop::saveToEEPROM(int wordInitPosition, const char* currentWord) {
	for(int i = wordInitPosition; i < (wordInitPosition + EEPROM_MAX_WORD_SIZE); i++) {
		device->getSerial()->print((char)currentWord[i - wordInitPosition]);
		device->getEEPROM()->write(i, (char)currentWord[i - wordInitPosition]);
	}
	bool success = device->getEEPROM()->commit();

	if (success) {
		device->getSerial()->println("Data saved to EEPROM successfully");
	} else {
		device->getSerial()->println("Error saving data to EEPROM.");
	}
}