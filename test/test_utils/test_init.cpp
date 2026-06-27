#include <unity.h>
#include "utils/init.h"
#include "device_constants.h"
#include "../include/mocks/mock_serial.h"
#include "../include/mocks/mock_arduino.h"
#include "../include/utils/test_init.h"

void test_init_serial_communication() {
    // Given
    Serial.clear();

    // When
    init_serial_communication();

    // Then
    TEST_ASSERT_EQUAL(115000, Serial.baudRate);
    TEST_ASSERT_EQUAL_STRING("\n\n", Serial.output[0].c_str());
    TEST_ASSERT_EQUAL_STRING("***************************************", Serial.output[1].c_str());
    TEST_ASSERT_EQUAL_STRING("CimaIoT - PowerDevice1 - Copyright@2025", Serial.output[2].c_str());
    TEST_ASSERT_EQUAL_STRING("***************************************", Serial.output[3].c_str());
    TEST_ASSERT_EQUAL_STRING("\n", Serial.output[4].c_str());
}

void test_init_pin_layout() {
    // Given
    Serial.clear();

    // When
    init_pin_layout();

    // Then
    TEST_ASSERT_EQUAL(7, pinModeCalls.size());
    TEST_ASSERT_EQUAL(KEYBOARD_LEFT_PIN, pinModeCalls[0].pin);
    TEST_ASSERT_EQUAL(INPUT, pinModeCalls[0].mode);
    TEST_ASSERT_EQUAL(KEYBOARD_RIGHT_PIN, pinModeCalls[1].pin);
    TEST_ASSERT_EQUAL(INPUT, pinModeCalls[1].mode);
    TEST_ASSERT_EQUAL(KEYBOARD_UP_PIN, pinModeCalls[2].pin);
    TEST_ASSERT_EQUAL(INPUT, pinModeCalls[2].mode);
    TEST_ASSERT_EQUAL(KEYBOARD_DOWN_PIN, pinModeCalls[3].pin);
    TEST_ASSERT_EQUAL(INPUT, pinModeCalls[3].mode);
    TEST_ASSERT_EQUAL(DEVICE_UNDER_CONTROL_PIN, pinModeCalls[4].pin);
    TEST_ASSERT_EQUAL(OUTPUT, pinModeCalls[4].mode);
    TEST_ASSERT_EQUAL(PROXIMITY_SENSOR_PIN, pinModeCalls[5].pin);
    TEST_ASSERT_EQUAL(INPUT, pinModeCalls[5].mode);
    TEST_ASSERT_EQUAL(STATUS_LED_PIN, pinModeCalls[6].pin);
    TEST_ASSERT_EQUAL(OUTPUT, pinModeCalls[6].mode);
    TEST_ASSERT_EQUAL_STRING("Init communications done. OK.", Serial.output[0].c_str());
}

