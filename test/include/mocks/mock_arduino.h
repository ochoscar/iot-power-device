#ifndef MOCK_ARDUINO_H
#define MOCK_ARDUINO_H

#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

#define HIGH        1
#define LOW         0
#define INPUT       0
#define OUTPUT      1
#define delay(ms)   std::this_thread::sleep_for(std::chrono::milliseconds(ms))

struct PinModeCall {
    int pin;
    int mode;
};
extern std::vector<PinModeCall> pinModeCalls;
void pinMode(int, int);

#endif