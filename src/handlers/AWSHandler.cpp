#include "device.h"
#include "DeviceConstants.h"
#include "handlers/AWSHandler.h"
#include "handlers/LCDHandler.h"
#include "handlers/PublishVariables.h"

void handleAWSIncomingMessage(char* topic, unsigned char* payload, unsigned int length, Device* device) {
    device->getSerial()->printf("\n-----> New message from AWS topic: %s\n", topic);
    printLCD("New AWS Message", LCD_FIRST_CHAR, LCD_FIRST_ROW);
    StaticJsonDocument<200> doc;
    device->deserializeJsonFunc(doc, payload);
    const char* message = doc["message"];
    String strMessage = String(message);
    device->getSerial()->printf("-----> message: %s\n", message);
    device->setDeviceState(strMessage);
}

void publishMessage(Device* device, PublishVariables publish) {
    int onBoardTemp = (int)device->temperatureReadFunc(); 
    StaticJsonDocument<1024> doc;
    doc[AWS_DEVICE_ID] = DEVICE_ID;
    if(publish.init) {
        doc[INIT_FROM_AWS] = 0;
    } else {
        if(publish.txAC && device->getEnableAC()) {
            doc[CURRENT_VARIABLE_NAME] = device->getACCurrent();
            doc[POWER_VARIABLE_NAME] = device->getACPower() ? DEVICE_ON : DEVICE_OFF;
            if(device->getACPower()) {
                doc[TEMP_VARIABLE_NAME] = device->getACTemp();
                doc[MODE_VARIABLE_NAME] = acMode2String(device->getACMode());
                doc[FAN_VARIABLE_NAME] = acFan2String(device->getACFan());
            }
            if(device->isPIRMessage()) {
                doc[PIR_OFF_VARIABLE_NAME] = true;
            }
        }
        if(publish.txRain && device->getEnableRain()) {
            doc[RAIN_VARIABLE_NAME] = device->getRain();
        }
        if(publish.txHT && device->getEnableHT()) {
            doc[ENV_TEMP_VARIABLE_NAME] = device->getEnvironmentTemp();
            doc[ENV_HUMIDITY_VARIABLE_NAME] = device->getEnvironmentHumidity();
        }
        if(publish.txSoil && device->getEnableSoilHumidity()) {
            doc[SOIL_HUMIDITY_VARIABLE_NAME] = device->getSoilHumidity();
        }
        if(publish.txLight && device->getEnableLight()) {
            doc[ENV_LIGHT_VARIABLE_NAME] = device->getEnvironmentLight();
        }
        if(publish.txCo2 && device->getEnableCO2()) {
            doc[CO2_VARIABLE_NAME] = device->getCO2();
            doc[ENV_TEMP_SCD4X_VARIABLE_NAME] = device->getEnvironmentTempSCD4X();
            doc[ENV_HUMIDITY_SCD4X_VARIABLE_NAME] = device->getEnvironmentHumiditySCD4X();
        }
        doc[DEVICE_TEMP_VARIABLE_NAME] = onBoardTemp;
    }

    char jsonBuffer[1024];
    device->serializeJsonFunc(doc, jsonBuffer);
    device->getSerial()->printf("Sending data: %s\n", jsonBuffer);
    String topic = String(AWS_IOT_PREFIX) + "/" + String(DEVICE_ID) + "/" + String(AWS_IOT_TOPIC_ENV) + String(AWS_IOT_PUBLISH_TOPIC_SUFIX);
    bool publishMessageResponse = device->getAWSPubSubclient()->publish(topic.c_str(), jsonBuffer);
    if (!publishMessageResponse) {
        printLCD("TX RESTART", LCD_FIRST_CHAR, LCD_SECOND_ROW);
        device->getSerial()->println("\nRestart by Tx");
        device->delayFunc(RESTART_DELAY_MS);
        device->getESP()->restart();
    }
}