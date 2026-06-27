#ifndef AIR_CONDITIONER_FACTORY_H
#define AIR_CONDITIONER_FACTORY_H

#include <ac/CoolixAC.h>
#include <ir_Samsung.h>
#include "ac/IAirConditioner.h"

class AirConditionerFactory {
    public:
        static IAirConditioner* createAC(
            const String& brand, 
            uint16_t irPin,
            std::function<IRsend*(uint16_t irSendPin)> irSendFactory,
            std::function<IRCoolixAC*(uint16_t irSendPin)> acFactory,
            std::function<IRSamsungAc*(uint16_t irSendPin)> acFactorySamsung,
            std::function<IRDaikinESP*(uint16_t irSendPin)> acFactoryDaikin280,
            std::function<IRLgAc*(uint16_t irSendPin)> acFactoryLG
        );
};

#endif