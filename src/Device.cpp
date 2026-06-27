#include <Arduino.h> 
#include <freertos/queue.h>
#include <LiquidCrystal_I2C.h>
#include <HardwareSerial.h>
#include <EEPROM.h>
#include <ArduinoJson.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <freertos/queue.h>
#include <SensirionI2CScd4x.h>
#include <Adafruit_SHT31.h>
#include <BH1750.h>
#include <Wire.h>
#include "Dependencies.h"
#include "Device.h"
#include "DeviceConstants.h"
#include "init/InitSerial.h"
#include "init/InitPinLayout.h"
#include "init/InitLCD.h"
#include "init/InitEEPROM.h"
#include "init/InitDeviceMode.h"
#include "init/InitWiFi.h"
#include "init/InitAWS.h"
#include "init/InitQueues.h"
#include "init/InitAC.h"
#include "init/InitHTSensor.h"
#include "init/InitLightSensor.h"
#include "init/InitCO2Sensor.h"
#include "init/InitI2C.h"
#include "ac/ACCommand.h"
#include "tasks/ACCommandTask.h"
#include "tasks/KeepAliveTask.h"
#include "tasks/InitTask.h"
#include "tasks/ACIRTask.h"
#include "tasks/ACSensorTask.h"
#include "tasks/SetupTask.h"
#include "tasks/RainTask.h"
#include "tasks/HTTask.h"
#include "tasks/SoilHumidityTask.h"
#include "tasks/LightTask.h"
#include "tasks/CO2Task.h"
#include "ac/ACStates.h"

Device::Device(
	TwoWire* wire,
	EEPROMClass* eemprom,
	WiFiClass* wifi,
	EspClass* esp,
	HardwareSerial* serial,
	void (*delayFunc)(unsigned int),
	void (*delayMicrosecondsFunc)(uint32_t us),
	unsigned long (*millisFunc)(void),
	unsigned long (*microsFunc)(void),
	void (*pinModeFunc)(uint8_t pin, uint8_t mode),
    void (*digitalWriteFunc)(uint8_t pin, uint8_t val),
	int (*digitalReadFunc)(uint8_t pin),
	uint16_t (*analogReadFunc)(uint8_t pin),
	float (*temperatureReadFunc)(),
	DeserializationError (*deserializeJsonFunc)(JsonDocument &doc, unsigned char *input),
	size_t (*serializeJsonFunc)(JsonVariantConst source, char (&destination)[1024]),
	std::function<SensirionI2cScd4x*()> co2Factory,
	std::function<BH1750*()> lightMeterFactory,
	std::function<Adafruit_SHT31*()> sHTFactory,
	std::function<LiquidCrystal_I2C*(
		uint8_t lcd_Addr, uint8_t lcd_cols, uint8_t lcd_rows
	)> lcdFactory,
	std::function<WebServer*(int port)> webServerFactory,
	std::function<PubSubClient*(WiFiClientSecure& net)> pubSubClientFactory,
	std::function<WiFiClientSecure*()> wifiClientSecureFactory,
	std::function<void (TaskHandle_t xTaskToDelete)> taskDelete,
	std::function<void (TickType_t xTicksToDelay)> taskDelay,
	std::function<BaseType_t (
		TaskFunction_t pvTaskCode, const char *pcName, uint32_t usStackDepth, void *pvParameters, 
		UBaseType_t uxPriority, TaskHandle_t *pvCreatedTask, BaseType_t xCoreID
	)> taskCreator,
	std::function<BaseType_t (
		QueueHandle_t xQueue,
		void * const pvBuffer,
		TickType_t xTicksToWait
	)> queueReceive,
	std::function<QueueHandle_t (
		const UBaseType_t uxQueueLength,
		const UBaseType_t uxItemSize
	)> queueCreator,
	std::function<void (uint8_t pin, void (*)(), int mode)> attachInterruptFunc,
	std::function<IRsend*(uint16_t irSendPin)> irSendFactory,
	std::function<IRrecv*(uint16_t irPin)> irRecvFactory,
	std::function<IRCoolixAC*(uint16_t irSendPin)> acFactoryCoolix,
	std::function<IRSamsungAc*(uint16_t irSendPin)> acFactorySamsung,
	std::function<IRDaikinESP*(uint16_t irSendPin)> acFactoryDaikin280,
	std::function<IRLgAc*(uint16_t irSendPin)> acFactoryLG
) {
	this->acPower = AC_INIT_POWER;
	this->acTemp = AC_INIT_TEMP;
	this->acMode = ACMode::AC_MODE_AUTO;
	this->acFan = ACFan::AC_FAN_AUTO;
	this->deviceMode = NORMAL_MODE;
	this->deviceInitialized = false;
	this->pirMessage = false;
	this->eemprom = eemprom;
	this->wifi = wifi;
	this->esp = esp;
	this->serial = serial;
	this->delayFunc = delayFunc;
	this->delayMicrosecondsFunc = delayMicrosecondsFunc;
	this->millisFunc = millisFunc;
	this->microsFunc = microsFunc;
	this->pinModeFunc = pinModeFunc;
	this->digitalWriteFunc = digitalWriteFunc;
	this->digitalReadFunc = digitalReadFunc;
	this->analogReadFunc = analogReadFunc;
	this->temperatureReadFunc = temperatureReadFunc;
	this->deserializeJsonFunc = deserializeJsonFunc;
	this->serializeJsonFunc = serializeJsonFunc;
	this->co2Factory = co2Factory;
	this->lightMeterFactory = lightMeterFactory;
	this->sHTFactory = sHTFactory;
	this->wire = wire;
	this->lcdFactory = lcdFactory;
	this->webServerFactory = webServerFactory;
	this->pubSubClientFactory = pubSubClientFactory;
	this->wifiClientSecureFactory = wifiClientSecureFactory;
	this->taskDelete = taskDelete;
	this->taskDelay = taskDelay;
	this->taskCreator = taskCreator;
	this->queueReceive = queueReceive;
	this->queueCreator = queueCreator;
	this->attachInterruptFunc = attachInterruptFunc;
	this->irSendFactory = irSendFactory;
	this->irRecvFactory = irRecvFactory;
	this->acFactoryCoolix = acFactoryCoolix;
	this->acFactorySamsung = acFactorySamsung;
	this->acFactoryDaikin280 = acFactoryDaikin280;
	this->acFactoryLG = acFactoryLG;
}

void Device::setup() {
    initSerial(this);
    initPinLayout(this);
	initI2C(this);
    initLCD(this);
	initQueues(this);
	initDeviceMode(this);
	initEEPROM(this);
	if(deviceMode != SETTINGS_MODE) {
		initDeviceParams(this); 
		initAC(this);
		initHT(this);
		initLight(this);
		initCO2(this);
		initWiFi(this);
		initAWS(this);
	}
	if(deviceMode == SETTINGS_MODE) {
		taskCreator(setupTask, SETUP_TASK, MEDIUM_TASK_STACK_SIZE, this, TASK_PRIORITY_MIN, NULL, CORE_0);	
	} else if(!deviceInitialized) {
		taskCreator(initTask, INIT_TASK, MEDIUM_TASK_STACK_SIZE, this, TASK_PRIORITY_MIN, NULL, CORE_0);
		taskCreator(keepAliveTask, AWS_TASK, MEDIUM_TASK_STACK_SIZE, this, TASK_PRIORITY_MIN, NULL, CORE_0);
		if(enableRain) {
			attachInterruptFunc(digitalPinToInterrupt(RAIN_SENSOR_PIN), rainISRHandler, FALLING);
			taskCreator(rainTask, RAIN_TASK, MEDIUM_TASK_STACK_SIZE, this, TASK_PRIORITY_MIN, NULL, CORE_0);
		}
		if(enableHT) {
			taskCreator(htTask, HT_TASK, MEDIUM_TASK_STACK_SIZE, this, TASK_PRIORITY_MIN, NULL, CORE_0);
		}
		if(enableSoilHumidity) {
			taskCreator(soilHumidityTask, SOIL_HUMIDITY_TASK, MEDIUM_TASK_STACK_SIZE, this, TASK_PRIORITY_MIN, NULL, CORE_0);
		}
		if(enableLight) {
			taskCreator(lightTask, LIGHT_TASK, MEDIUM_TASK_STACK_SIZE, this, TASK_PRIORITY_MIN, NULL, CORE_0);
		}
		if(enableCO2) {
			taskCreator(co2Task, CO2_TASK, MEDIUM_TASK_STACK_SIZE, this, TASK_PRIORITY_MIN, NULL, CORE_0);
		}
		if(enableAC) {
			taskCreator(acSensorTask, SENSOR_TASK, MEDIUM_TASK_STACK_SIZE, this, TASK_PRIORITY_MIN, NULL, CORE_0);	
			taskCreator(acIRTask, IR_TASK, MEDIUM_TASK_STACK_SIZE, this, TASK_PRIORITY_HIGH2, NULL, CORE_1);
			taskCreator(acCommandTask, AC_COMMANDS, MEDIUM_TASK_STACK_SIZE, this, TASK_PRIORITY_HIGH1, NULL, CORE_1);
		}
	}
}

void Device::loop() {
}

void Device::setDeviceState(String strMessage) {
    ACCommand cmd = {
        .send = true
    };
	if (strMessage.startsWith(INIT_FROM_AWS)) {
		String jsonPart = strMessage.substring(5);
		StaticJsonDocument<256> doc;
		DeserializationError error = deserializeJson(doc, jsonPart);
		
		if (error) {
			this->getSerial()->print("JSON parse failed: ");
			this->getSerial()->println(error.c_str());
			this->getSerial()->println("\nRestart by Init");
			this->delayFunc(RESTART_DELAY_MS);
       		this->getESP()->restart();
		}

		if (doc.containsKey(ACTION_ON_OFF)) {
			const char* onOff = doc[ACTION_ON_OFF];
			this->getSerial()->printf("\nInit power on: %s", onOff);
			bool onOffState = String(onOff).equals(DEVICE_ON);
			this->digitalWriteFunc(AC_CONTROL_PIN, onOffState ? HIGH : LOW);
			this->setACPower(onOffState);
		}
		if (doc.containsKey(ACTION_SET_TEMP)) {
			int temp = doc[ACTION_SET_TEMP];
			this->getSerial()->printf("\nInit temp: %i", temp);
			this->setACTemp(temp);
		}
		if (doc.containsKey(ACTION_SET_FAN)) {
			const char* fan = doc[ACTION_SET_FAN];
			this->getSerial()->printf("\nInit fan: %s", fan);
			this->setACMode(string2ACMode(fan));
		}
		if (doc.containsKey(ACTION_AC_MODE)) {
			const char* acMode = doc[ACTION_AC_MODE];  
			this->getSerial()->printf("\nInit mode: %s", acMode);
			this->setACFan(string2ACFan(acMode));
		}
		this->getSerial()->printf("\nSending init command\n");
		this->setDeviceInitialized(true);
    } else if(strMessage.equals(String(ACTION_ON_OFF) + ":" + DEVICE_OFF)) {
        this->setACPower(false);
		this->digitalWriteFunc(AC_CONTROL_PIN, LOW);
    } else if(strMessage.equals(String(ACTION_ON_OFF) + ":" + DEVICE_ON)) {
		this->digitalWriteFunc(AC_CONTROL_PIN, HIGH);
        this->setACPower(true);
    } else if(strMessage.equals(String(ACTION_SET_TEMP_UP_DOWN) + ":" + TEMP_UP)) {
        int newTemp = this->getACTemp() == AC_MAX_TEMP ? this->getACTemp() : this->getACTemp() + 1;
        this->setACTemp(newTemp);
    } else if(strMessage.equals(String(ACTION_SET_TEMP_UP_DOWN) + ":" + TEMP_DOWN)) {
        int newTemp = this->getACTemp() == AC_MIN_TEMP ? this->getACTemp() : this->getACTemp() - 1;
        this->setACTemp(newTemp);
    } else if(strMessage.startsWith(String(ACTION_AC_MODE))) {
		String acMode = strMessage.substring(8);
        this->setACMode(string2ACMode(acMode));
	} else if(strMessage.startsWith(String(ACTION_SET_FAN))) {
		String acFan = strMessage.substring(8);
        this->setACFan(string2ACFan(acFan));
    } else if(strMessage.startsWith(ACTION_SET_TEMP)) {
		int separatorIndex = strMessage.indexOf(':');
		String tempStr = strMessage.substring(separatorIndex + 1);
		int temp = tempStr.toInt();
        if (strMessage.length() > 0 && strMessage != "0") {
            this->setACTemp(temp);
        } else {
			return;
		}
	}
	xQueueSend(this->getACCommandQueue(), &cmd, 0);
}

String Device::getACBrand() {
	return acBrand;
}

void Device::setACBrand(String acBrand) {
	this->acBrand = acBrand;
}

bool Device::getEnableAC() {
	return enableAC;
}

void Device::setEnableAC(bool enableAC)	{
	this->enableAC = enableAC;
}

bool Device::getEnableRain() {
	return enableRain;
}

void Device::setEnableRain(bool enableRain) {
	this->enableRain = enableRain;
}

bool Device::getEnableHT() {
	return enableHT;
}

void Device::setEnableHT(bool enableHT) {
	this->enableHT = enableHT;
}

bool Device::getEnableSoilHumidity() {
	return enableSoilHumidity;
}

void Device::setEnableSoilHumidity(bool enableSoilHumidity) {
	this->enableSoilHumidity = enableSoilHumidity;
}
bool Device::getEnableLight() {
	return enableLight;
}

void Device::setEnableLight(bool enableLight) {
	this->enableLight = enableLight;
}

bool Device::getEnableCO2() {
	return enableCO2;
}

void Device::setEnableCO2(bool enableCO2) {
	this->enableCO2 = enableCO2;
}

float Device::getEnvironmentHumiditySCD4X() {
	return environmentHumiditySCD4X;
}

void Device::setEnvironmentHumiditySCD4X(float environmentHumiditySCD4X) {
	this->environmentHumiditySCD4X = environmentHumiditySCD4X;
}

float Device::getEnvironmentTempSCD4X() {
	return environmentTempSCD4X;
}

void Device::setEnvironmentTempSCD4X(float environmentTempSCD4X) {
	this->environmentTempSCD4X = environmentTempSCD4X;
}

int Device::getCO2(){
	return co2;
}

void Device::setCO2(int co2) {
	this->co2 = co2;
}

int Device::getEnvironmentLight() {
	return environmentLight;
}

void Device::setEnvironmentLight(int environmentLight) {
	this->environmentLight = environmentLight;
}

int Device::getSoilHumidity() {
	return soilHumidity;
}

void Device::setSoilHumidity(int soilHumidity) {
	this->soilHumidity = soilHumidity;
}

float Device::getEnvironmentTemp() {
	return environmentTemp;
}

void Device::setEnvironmentTemp(float environmentTemp) {
	this->environmentTemp = environmentTemp;
}

float Device::getEnvironmentHumidity() {
	return environmentHumidity;
}

void Device::setEnvironmentHumidity(float environmentHumidity) {
	this->environmentHumidity = environmentHumidity;
}

float Device::getRain() {
	return rain;
}

void Device::setRain(float rain) {
	this->rain = rain;
}

bool Device::getACPower() {
	return acPower;
}

void Device::setACPower(bool acPower) {
	this->acPower = acPower;
}

int Device::getACTemp() {
	return acTemp;
}

void Device::setACTemp(int acTemp) {
	if(acTemp >= AC_MIN_TEMP && acTemp <= AC_MAX_TEMP) {
		this->acTemp = acTemp;
	}
}

ACMode Device::getACMode() {
	return acMode;
}

void Device::setACMode(ACMode acMode) {
	this->acMode = acMode;
}

ACFan Device::getACFan() {
	return acFan;
}

void Device::setACFan(ACFan acFan) {
	this->acFan = acFan;
}

bool Device::isPIRMessage() {
	return pirMessage;
}

void Device::setPIRMessage(bool pirMessage) {
	this->pirMessage = pirMessage;
}

int Device::getACCurrent() {
	return acCurrent;
}

void Device::setACCurrent(int acCurrent) {
	this->acCurrent = acCurrent;
}

bool Device::getACAutoOff() {
	return acAutoOff;
}

void Device::setACAutoOff(bool acAutoOff) {
	this->acAutoOff = acAutoOff;
}

int Device::getACTurnOffTime() {
	return acTurnOffTime;
}

void Device::setACTurnOffTime(int acTurnOffTime) {
	this->acTurnOffTime = acTurnOffTime;
}

IAirConditioner* Device::getAC() {
	return ac;
};

void Device::setAC(IAirConditioner* ac) {
	this->ac = ac;
}

IRrecv* Device::getACIRRecv() {
	return acIRRecv;
}

void Device::setACIRRecv(IRrecv* acIRRecv) {
	this->acIRRecv = acIRRecv;
}

SensirionI2cScd4x* Device::getCO2Meter() {
	return co2Meter;
}

void Device::setCO2Meter(SensirionI2cScd4x* co2Meter) {
	this->co2Meter = co2Meter;
}

BH1750* Device::getLightMeter() {
	return lightMeter;
}	

void Device::setLightMeter(BH1750* lightMeter) {
	this->lightMeter = lightMeter;
}	

Adafruit_SHT31* Device::getSHT() {
	return sht;
}

void Device::setSHT(Adafruit_SHT31* sht) {
	this->sht = sht;
}

QueueHandle_t Device::getACCommandQueue() {
	return acCommandQueue;
}

void Device::setACCommandQueue(QueueHandle_t commandQueue) {
	this->acCommandQueue = commandQueue;
}

int Device::getDeviceMode() {
	return deviceMode;
}

void Device::setDeviceMode(int deviceMode) {
	this->deviceMode = deviceMode;
}

boolean Device::getDeviceInitialized() {
	return deviceInitialized;
}

void Device::setDeviceInitialized(boolean deviceInitialized) {
	this->deviceInitialized = deviceInitialized;
}

String Device::getWIFISSID() {
	return wifiSSID;
}

void Device::setWIFISSID(String wifiSSID) {
	this->wifiSSID = wifiSSID;
}

String Device::getWIFIPassword() {
	return wifiPassword;
}

void Device::setWIFIPassword(String wifiPassword) {
	this->wifiPassword = wifiPassword;
}

LiquidCrystal_I2C* Device::getLCD() {
	return lcd;
}

void Device::setLCD(LiquidCrystal_I2C* lcd) {
	this->lcd = lcd;
}

PubSubClient* Device::getAWSPubSubclient() {
	return awsPubSubclient;
}

void Device::setAWSPubSubclient(PubSubClient* awsPubSubclient) {
	this->awsPubSubclient = awsPubSubclient;
}

WiFiClientSecure* Device::getWIFIClientSecure() {
	return wiFiClientSecure;
}

void Device::setWIFIClientSecure(WiFiClientSecure* wiFiClientSecure) {
	this->wiFiClientSecure = wiFiClientSecure;
}

 TwoWire* Device::getWire() {
	return wire;
}

EEPROMClass* Device::getEEPROM() {
	return eemprom;
}

WiFiClass* Device::getWIFI() {
	return wifi;
}

EspClass* Device::getESP() {
	return esp;
}

HardwareSerial* Device::getSerial() {
	return serial;
}