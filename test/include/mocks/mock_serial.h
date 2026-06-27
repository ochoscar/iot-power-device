#ifndef MOCK_SERIAL_H
#define MOCK_SERIAL_H

#include <iostream>
#include <string>
#include <vector>

class MockSerial {
public:
    std::vector<std::string> output;
    int baudRate = 0;

    void begin(int baud) {
        baudRate = baud;
    }

    void println(const std::string& message) {
        output.push_back(message);
    }

    void clear() {
        output.clear();
    }
};
extern MockSerial Serial;

#endif