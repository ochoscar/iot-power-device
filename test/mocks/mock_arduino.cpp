#include "../include/mocks/mock_arduino.h"

std::vector<PinModeCall> pinModeCalls;

void pinMode(int pin, int mode) {
    pinModeCalls.push_back({pin, mode});
}