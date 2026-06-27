#include <ac/CoolixAC.h>
#include <ir_Samsung.h>
#include <ir_Daikin.h>
#include <ir_LG.h>
#include "DeviceConstants.h"
#include "ac/AirConditionerFactory.h"
#include "ac/CoolixAC.h"
#include "ac/SamsungAC.h"
#include "ac/DaikinAC280.h"
#include "ac/LGAC.h"

IAirConditioner* AirConditionerFactory::createAC(
    const String& brand, 
    uint16_t irPin,
    std::function<IRsend*(uint16_t irSendPin)> irSendFactory,
    std::function<IRCoolixAC*(uint16_t irSendPin)> acFactoryCoolix,
    std::function<IRSamsungAc*(uint16_t irSendPin)> acFactorySamsung,
    std::function<IRDaikinESP*(uint16_t irSendPin)> acFactoryDaikin280,
    std::function<IRLgAc*(uint16_t irSendPin)> acFactoryLG
) {
    if (brand.equals(AC_BRAND_COOLIX)) {
        return new CoolixAC(irPin, irSendFactory, acFactoryCoolix);
    } else if(brand.equals(AC_BRAND_SAMSUNG)) {
        return new SamsungAC(irPin, irSendFactory, acFactorySamsung);
    } else if(brand.equals(AC_BRAND_DAIKIN_280)) {
        return new DaikinAC280(irPin, irSendFactory, acFactoryDaikin280);
    } else if(brand.equals(AC_BRAND_LG)) {
        return new LGAC(irPin, irSendFactory, acFactoryLG);
    }
    return nullptr;
}