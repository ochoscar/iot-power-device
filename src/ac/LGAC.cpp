#include "DeviceConstants.h"
#include "ac/LGAC.h"
#include "ac/ACStates.h"

LGAC::LGAC(
    uint16_t irPin,
    std::function<IRsend*(uint16_t irSendPin)> irSendFactory,
    std::function<IRLgAc*(uint16_t irSendPin)> acFactory
) {
    irSend = irSendFactory(irPin);
    lg = acFactory(irPin);
    irSend->begin();
    lg->begin();
}

String LGAC::getACBrand() {
    return AC_BRAND_COOLIX;
}

void LGAC::setPower(bool on) {
    on ? lg->on() : lg->off();
}

void LGAC::setTemperature(uint8_t temp) {
    lg->setTemp(temp);
}

void LGAC::setMode(ACMode mode) {
    switch (mode) {
        case ACMode::AC_MODE_AUTO:
            lg->setMode(kLgAcAuto);
            break;
        case ACMode::AC_MODE_COOL:
            lg->setMode(kLgAcCool);
            break;
        case ACMode::AC_MODE_DRY:
            lg->setMode(kLgAcDry);
            break;
        case ACMode::AC_MODE_FAN:
            lg->setMode(kLgAcFan);
            break;
        case ACMode::AC_MODE_HEAT:
            lg->setMode(kLgAcHeat);
            break;
    }
}

void LGAC::setFanSpeed(ACFan speed) {
    switch (speed) {
        case ACFan::AC_FAN_AUTO:
            lg->setFan(kLgAcFanAuto);
            break;
        case ACFan::AC_FAN_LOW:
            lg->setFan(kLgAcFanLow);
            break;
        case ACFan::AC_FAN_MED:
            lg->setFan(kLgAcFanMedium);
            break;
        case ACFan::AC_FAN_HIGH:
            lg->setFan(kLgAcFanMax);
            break;
        case ACFan::AC_FAN_AUTO_0:
            lg->setFan(kLgAcFanAuto);
            break;
        default:
            lg->setFan(kLgAcFanAuto);
            break;
    }
}

bool LGAC::getPower() {
    return lg->getPower();
}

uint8_t LGAC::getTemp() {
    return lg->getTemp();
}


ACMode LGAC::getMode() {
    uint8_t mode = lg->getMode();
    switch (mode) {
        case kLgAcAuto:
            return ACMode::AC_MODE_AUTO;
        case kLgAcCool:
            return ACMode::AC_MODE_COOL;
        case kLgAcDry:
            return ACMode::AC_MODE_DRY;
        case kLgAcFan:
            return ACMode::AC_MODE_FAN;
        case kLgAcHeat:
            return ACMode::AC_MODE_HEAT;
        default:
            return ACMode::AC_MODE_AUTO;
    }
}

ACFan LGAC::getFan() {
    uint8_t fan = lg->getFan();
    switch (fan) {
        case kLgAcFanAuto:
            return ACFan::AC_FAN_AUTO;
        case kLgAcFanLow:
            return ACFan::AC_FAN_LOW;
        case kLgAcFanMedium:
            return ACFan::AC_FAN_MED;
        case kLgAcFanMax:
            return ACFan::AC_FAN_HIGH;
        default:
            return ACFan::AC_FAN_AUTO;
    }
}

String LGAC::toString() {
    return lg->toString();
}

bool LGAC::setRawFromDecode(const decode_results& results) {
    if (results.decode_type == decode_type_t::LG) {
        if (results.bits == 32) {
            lg->setRaw(results.value); 
            return false;
        }
    }
    return true;
}


void LGAC::sendCommand() {
    lg->send();
}