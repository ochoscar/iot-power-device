#ifndef LG_AC_H
#define LG_AC_H

#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <ir_LG.h>
#include <IRrecv.h>
#include "ac/IAirConditioner.h"

class LGAC : public IAirConditioner {
private:
    IRsend* irSend;
    IRLgAc* lg;

public:
    LGAC(
        uint16_t irPin,
        std::function<IRsend*(uint16_t irSendPin)> irSendFactory,
        std::function<IRLgAc*(uint16_t irSendPin)> acFactory
    );
    String getACBrand() override;
    void setPower(bool on) override;
    void setTemperature(uint8_t temp) override;
    void setMode(ACMode mode) override;
    void setFanSpeed(ACFan speed) override; 
    bool getPower() override;
    uint8_t getTemp() override;
    ACMode getMode() override;
    ACFan getFan() override;
    String toString() override;
    bool setRawFromDecode(const decode_results& results) override;
    void sendCommand() override;
};

#endif