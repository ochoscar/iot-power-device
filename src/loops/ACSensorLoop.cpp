#include "Device.h"
#include "DeviceConstants.h"
#include "loops/ACSensorLoop.h"
#include "handlers/LCDHandler.h"
#include "handlers/AWSHandler.h"
#include "handlers/PublishVariables.h"

void acSensorLoop(Device* device) {
	static unsigned long lastSampleTime = 0;
    static unsigned long lastMotionTime = millis();
	unsigned long now = device->millisFunc();
    PublishVariables publish = {
        .txAC = true,
        .txRain = false,
        .txHT = false,
        .txSoil = false,
        .txLight = false,
        .txCo2 = false,
        .init = false
    };

	// PIR
    if(device->getACAutoOff()) {
        int pirState = digitalRead(PIR_PIN);
        if (pirState == HIGH) {
            lastMotionTime = now;
        }
        if ((now - lastMotionTime) > device->getACTurnOffTime() * ONESECOND_MS && device->getACPower()) {
            device->getSerial()->println("No motion detected. Turning off A/C...");
            device->setPIRMessage(true);
            device->setDeviceState(String(ACTION_ON_OFF) + ":" + DEVICE_OFF);
            publishMessage(device, publish);
            device->setPIRMessage(false);
        }
    }

	// Current
	if(now - lastSampleTime >= TIME_BETWEEN_SAMPLES_MS) {
		int current = getACElectricCurrent(ANALOG_INPUT_CURRENT_S1_PIN, device);
		device->setACCurrent(current);
		String sampleCurrent = String(current);

		publishMessage(device, publish);
		printLCD("Cima IoT-Tx AC", LCD_FIRST_CHAR, LCD_FIRST_ROW);
		printLCD(sampleCurrent, LCD_FIRST_CHAR, LCD_SECOND_ROW);

		device->getSerial()->println("ISensor: " + sampleCurrent + " was sent to AWS");
		lastSampleTime = now;
	}
}

int getACElectricCurrent(int sensor, Device* device) {
    int samples_count = 0;
    long sum_analog_reads = 0;
    unsigned long startTime = device->microsFunc();

    while (samples_count < TOTAL_CURRENT_SAMPLES) {
        unsigned long now = micros();
        if (now - startTime >= CURRENT_DELAY_BETWEEN_SAMPLES) {
            int analog_value = device->analogReadFunc(sensor);
            sum_analog_reads += analog_value;
            samples_count++;
            startTime = now;
        }
    }

    return sum_analog_reads / samples_count;
}

