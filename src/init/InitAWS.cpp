#include "Dependencies.h"
#include "device.h"
#include "DeviceConstants.h"
#include "certs.h"
#include "init/InitAWS.h"
#include "handlers/AWSHandler.h"
#include "handlers/LCDHandler.h"

void connectAWS(Device* device);

void awsIncomingMessage(char* topic, unsigned char* payload, unsigned int length) {
	handleAWSIncomingMessage(topic, payload, length, Dependencies::getInstance()->cimaDevice);
}

void initAWS(Device* device) {
	device->getSerial()->println("Initializing AWS...");
	if(device->getDeviceMode() != SETTINGS_MODE) {
		WiFiClientSecure* wiFiClientSecure = device->wifiClientSecureFactory();
		PubSubClient* awsPubSubclient = device->pubSubClientFactory(*wiFiClientSecure);
		device->setWIFIClientSecure(wiFiClientSecure);
		device->setAWSPubSubclient(awsPubSubclient);
        connectAWS(device);
    }
	printLCD("AWS OK", LCD_FIRST_CHAR, LCD_SECOND_ROW);
    device->getSerial()->println("Init AWS done. OK.\n");
	device->delayFunc(DELAY_BETWEEN_INIT_STAGES_MS);
}

void connectAWS(Device* device) {
	device->getWIFIClientSecure()->setCACert(AWS_CERT_CA);
	device->getWIFIClientSecure()->setCertificate(AWS_CERT_CRT);
	device->getWIFIClientSecure()->setPrivateKey(AWS_CERT_PRIVATE);
	device->getAWSPubSubclient()->setServer(AWS_IOT_ENDPOINT, AWS_IOT_PORT);
	device->getAWSPubSubclient()->setCallback(awsIncomingMessage);
	device->getAWSPubSubclient()->setKeepAlive(5 * (TIME_BETWEEN_SAMPLES_MS / 1000));

	int attempts = 0;
	String thingName = String(AWS_IOT_PREFIX) + "-" + String(DEVICE_ID) + String(THING_NAME_SUFIX);
	while (!device->getAWSPubSubclient()->connect(thingName.c_str())) {
		device->getSerial()->print(".");
		printLCD("AWS tries: " + String(attempts), LCD_FIRST_ROW, LCD_SECOND_ROW);
		device->delayFunc(AWS_RECONNECT_DELAY_MS);
		attempts++;
	}
	if (!device->getAWSPubSubclient()->connected()) {
		printLCD("AWS EROR", LCD_FIRST_CHAR, LCD_SECOND_ROW);
		device->getSerial()->println("\nRestart due to AWS connecty retries");
		device->delayFunc(RESTART_DELAY_MS);
		device->getESP()->restart();
	} else {
		String topic = String(AWS_IOT_PREFIX) + "/" + String(DEVICE_ID) + "/" + String(AWS_IOT_TOPIC_ENV) + String(AWS_IOT_SUBSCRIBE_TOPIC_SUFIX);
		device->getAWSPubSubclient()->subscribe(topic.c_str());
	}
}