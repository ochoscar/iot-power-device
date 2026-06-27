#ifndef DEVICE_H
#define DEVICE_H

#include <WebServer.h>
#include <EEPROM.h>
#include <LiquidCrystal_I2C.h>
#include <ArduinoJson.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <IRsend.h>
#include <IRrecv.h>
#include <Adafruit_SHT31.h>
#include <BH1750.h>
#include <SensirionI2CScd4x.h>
#include <Wire.h>
#include <ir_Coolix.h>
#include <ir_Samsung.h>
#include <ir_Daikin.h>
#include <ir_LG.h>
#include "ac/IAirConditioner.h"
#include "ac/ACStates.h"

class Device {
	private:
		String acBrand;
		bool enableAC;
		bool enableRain;
		bool enableHT;
		bool enableSoilHumidity;
		bool enableLight;
		bool enableCO2;
		float environmentHumiditySCD4X;
		float environmentTempSCD4X;
		unsigned int co2;
		float environmentLight;
		int soilHumidity;
		float environmentTemp;
		float environmentHumidity;
		float rain;
		bool acPower;
		int acTemp;
		ACMode acMode;
		ACFan acFan;
		bool pirMessage;
		int acCurrent;
		bool acAutoOff;
		int acTurnOffTime;
		IAirConditioner* ac;
		IRrecv* acIRRecv;
		SensirionI2cScd4x* co2Meter;
		BH1750* lightMeter;
		Adafruit_SHT31* sht;
		QueueHandle_t acCommandQueue;
		int deviceMode;
		boolean deviceInitialized;
		String wifiSSID;
		String wifiPassword;
		TwoWire* wire;
		LiquidCrystal_I2C* lcd;
		PubSubClient* awsPubSubclient;
		WiFiClientSecure* wiFiClientSecure;
		EEPROMClass* eemprom;
		WiFiClass* wifi;
		EspClass* esp;
		HardwareSerial* serial;

	public:
		Device(
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
		);

		void setup();
		void loop();
		void setDeviceState(String strMessage);

		String getACBrand();
		void setACBrand(String acBrand);
		bool getEnableAC();
		void setEnableAC(bool enableAC);
		bool getEnableRain();
		void setEnableRain(bool enableRain);
		bool getEnableHT();
		void setEnableHT(bool enableHT);
		bool getEnableSoilHumidity();
		void setEnableSoilHumidity(bool enableSoilHumidity);
		bool getEnableLight();
		void setEnableLight(bool enableLight);
		bool getEnableCO2();
		void setEnableCO2(bool enableCO2);
		float getEnvironmentHumiditySCD4X();
		void setEnvironmentHumiditySCD4X(float environmentHumiditySCD4X);
		float getEnvironmentTempSCD4X();
		void setEnvironmentTempSCD4X(float environmentTempSCD4X);
		int getCO2();
		void setCO2(int co2);
		int getEnvironmentLight();
		void setEnvironmentLight(int environmentLight);
		int getSoilHumidity();
		void setSoilHumidity(int soilHumidity);
		float getEnvironmentTemp();
		void setEnvironmentTemp(float environmentTemp);
		float getEnvironmentHumidity();
		void setEnvironmentHumidity(float environmentHumidity);
		float getRain();
		void setRain(float rain);
		bool getACPower();
		void setACPower(bool acPower);
		int getACTemp();
		void setACTemp(int acTemp);
		ACMode getACMode();
		void setACMode(ACMode acMode);
		ACFan getACFan();
		void setACFan(ACFan acFan);
		bool isPIRMessage();
		void setPIRMessage(bool pirMessage);
		int getACCurrent();
		void setACCurrent(int acCurrent);
		bool getACAutoOff();
		void setACAutoOff(bool acAutoOff);
		int getACTurnOffTime();
		void setACTurnOffTime(int acTurnOffTime);
		IAirConditioner* getAC();
		void setAC(IAirConditioner* ac);
		IRrecv* getACIRRecv();
		void setACIRRecv(IRrecv* acIRRecv);
		SensirionI2cScd4x* getCO2Meter();
		void setCO2Meter(SensirionI2cScd4x* co2Meter);
		BH1750* getLightMeter();
		void setLightMeter(BH1750* lightMeterFactory);
		Adafruit_SHT31* getSHT();
		void setSHT(Adafruit_SHT31* sht);
		QueueHandle_t getACCommandQueue();
		void setACCommandQueue(QueueHandle_t commandQueue);
		int getDeviceMode();
		void setDeviceMode(int deviceMode);
		boolean getDeviceInitialized();
		void setDeviceInitialized(boolean deviceInitialized);
		String getWIFISSID();
		void setWIFISSID(String wifiSSID);
		String getWIFIPassword();
		void setWIFIPassword(String wifiPassword);
		LiquidCrystal_I2C* getLCD();
		void setLCD(LiquidCrystal_I2C* lcd);
		PubSubClient* getAWSPubSubclient();
		void setAWSPubSubclient(PubSubClient* awsPubSubclient);
		WiFiClientSecure* getWIFIClientSecure();
		void setWIFIClientSecure(WiFiClientSecure* wiFiClientSecure);
		TwoWire* getWire();
		EEPROMClass* getEEPROM();
		WiFiClass* getWIFI();
		EspClass* getESP();
		HardwareSerial* getSerial();

		void (*delayFunc)(unsigned int);
		void (*delayMicrosecondsFunc)(uint32_t us);
		unsigned long (*millisFunc)(void);
		unsigned long (*microsFunc)(void);
		void (*pinModeFunc)(uint8_t pin, uint8_t mode);
		void (*digitalWriteFunc)(uint8_t pin, uint8_t val);
		int (*digitalReadFunc)(uint8_t pin);
		uint16_t (*analogReadFunc)(uint8_t pin);
		float (*temperatureReadFunc)();
		DeserializationError (*deserializeJsonFunc)(JsonDocument &doc, unsigned char *input);
		size_t (*serializeJsonFunc)(JsonVariantConst source, char (&destination)[1024]);
		std::function<SensirionI2cScd4x*()> co2Factory;
		std::function<BH1750*()> lightMeterFactory;
		std::function<Adafruit_SHT31*()> sHTFactory;
		std::function<LiquidCrystal_I2C*(
			uint8_t lcd_Addr, uint8_t lcd_cols, uint8_t lcd_rows
		)> lcdFactory;
		std::function<WebServer*(int port)> webServerFactory;
		std::function<PubSubClient*(WiFiClientSecure& net)> pubSubClientFactory;
		std::function<WiFiClientSecure*()> wifiClientSecureFactory;
		std::function<void (TaskHandle_t xTaskToDelete)> taskDelete;
		std::function<void (TickType_t xTicksToDelay)> taskDelay;
		std::function<BaseType_t (
			TaskFunction_t pvTaskCode, const char *pcName, uint32_t usStackDepth, void *pvParameters, 
			UBaseType_t uxPriority, TaskHandle_t *pvCreatedTask, BaseType_t xCoreID
		)> taskCreator;
		std::function<BaseType_t (
			QueueHandle_t xQueue,
			void * const pvBuffer,
			TickType_t xTicksToWait
		)> queueReceive;
		std::function<QueueHandle_t (
			const UBaseType_t uxQueueLength,
			const UBaseType_t uxItemSize
		)> queueCreator;
		std::function<void (uint8_t pin, void (*)(), int mode)> attachInterruptFunc;
		std::function<IRsend*(uint16_t irSendPin)> irSendFactory;
		std::function<IRrecv*(uint16_t irPin)> irRecvFactory;
		std::function<IRCoolixAC*(uint16_t irSendPin)> acFactoryCoolix;
		std::function<IRSamsungAc*(uint16_t irSendPin)> acFactorySamsung;
		std::function<IRDaikinESP*(uint16_t irSendPin)> acFactoryDaikin280;
		std::function<IRLgAc*(uint16_t irSendPin)> acFactoryLG;
};

#endif