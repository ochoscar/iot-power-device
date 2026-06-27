#include <WebServer.h>
#include <EEPROM.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <LiquidCrystal_I2C.h>
#include <HardwareSerial.h>
#include <ArduinoJson.h>
#include <Adafruit_SHT31.h>
#include <BH1750.h>
#include <SensirionI2CScd4x.h>
#include "DeviceConstants.h"
#include "Dependencies.h"
#include "Device.h"
#include "ac/AirConditionerFactory.h"

Dependencies* Dependencies::dependenciesInstance = nullptr;

Dependencies* Dependencies::getInstance() {
    if (dependenciesInstance == nullptr) {
        dependenciesInstance = new Dependencies();
    }
    return dependenciesInstance;
}

Dependencies::Dependencies() {
    this->cimaDevice = new Device(
		&Wire,
		&EEPROM,
		&WiFi,
		&ESP,
		&Serial,
		delay,
		delayMicroseconds,
		millis,
		micros,
		pinMode,
		digitalWrite,
		digitalRead,
		analogRead,
		temperatureRead,
		deserializeJson,
		serializeJson,
		[]() {
			return new SensirionI2cScd4x();
		},
		[]() {
			return new BH1750();
		},
		[]() {
			return new Adafruit_SHT31();
		},
		[](int addr, int cols, int rows) {
			return new LiquidCrystal_I2C(addr, cols, rows);
		},
		[](int port) {
			return new WebServer(port);
		},
		[](WiFiClientSecure& net) {
			return new PubSubClient(net);
		},
		[]() {
			return new WiFiClientSecure();
		},
		[](TaskHandle_t xTaskToDelete) {
			return vTaskDelete(xTaskToDelete);
		},
		[](TickType_t xTicksToDelay) {
			return vTaskDelay(xTicksToDelay);
		},
		[](TaskFunction_t fn, const char* name, uint32_t stack, void* param,
			UBaseType_t priority, TaskHandle_t* handle, BaseType_t core) {
			return xTaskCreatePinnedToCore(fn, name, stack, param, priority, handle, core);
		},
		[](QueueHandle_t xQueue, void * const pvBuffer, TickType_t xTicksToWait) {
			return xQueueReceive(xQueue, pvBuffer, xTicksToWait);
		},
		[](const UBaseType_t uxQueueLength, const UBaseType_t uxItemSize) {
			return xQueueCreate(uxQueueLength, uxItemSize);
		},
		[](uint8_t pin, void (*interruptFunction)(), int mode) {
			return attachInterrupt(pin, interruptFunction, mode);
		},
		[](uint16_t irSendPin) {
			return new IRsend(irSendPin);
		},
		[](uint16_t irPin) {
			return new IRrecv(irPin);
		},
		[](uint16_t irSendPin) {
			return new IRCoolixAC(irSendPin);
		},
		[](uint16_t irSendPin) {
			return new IRSamsungAc(irSendPin);
		},
		[](uint16_t irSendPin) {
			return new IRDaikinESP(irSendPin);
		},
		[](uint16_t irSendPin) {
			return new IRLgAc(irSendPin);
		}
	);
}