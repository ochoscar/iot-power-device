#ifndef I_AIR_CONDITIONER_H
#define I_AIR_CONDITIONER_H

#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h> 
#include "ac/ACStates.h"

class IAirConditioner {
    public:
        virtual String getACBrand() = 0;
        virtual void setPower(bool on) = 0;
        virtual void setTemperature(uint8_t temp) = 0;
        virtual void setMode(ACMode mode) = 0;
        virtual void setFanSpeed(ACFan speed) = 0; 
        virtual bool getPower() = 0;
        virtual uint8_t getTemp() = 0;
        virtual ACMode getMode() = 0;
        virtual ACFan getFan() = 0;
        virtual String toString() = 0;
        virtual bool setRawFromDecode(const decode_results& results) = 0;
        virtual void sendCommand() = 0;
        virtual ~IAirConditioner() = default;
};

#endif