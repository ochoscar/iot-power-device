#include "DeviceConstants.h"
#include "ac/CoolixAC.h"
#include "ac/ACStates.h"

CoolixAC::CoolixAC(
    uint16_t irPin,
    std::function<IRsend*(uint16_t irSendPin)> irSendFactory,
    std::function<IRCoolixAC*(uint16_t irSendPin)> acFactory
) {
    irSend = irSendFactory(irPin);
    coolix = acFactory(irPin);
    irSend->begin();
    coolix->begin();
}

String CoolixAC::getACBrand() {
    return AC_BRAND_COOLIX;
}

void CoolixAC::setPower(bool on) {
    on ? coolix->on() : coolix->off();
}

void CoolixAC::setTemperature(uint8_t temp) {
    coolix->setTemp(temp);
}

void CoolixAC::setMode(ACMode mode) {
    switch (mode) {
        case ACMode::AC_MODE_AUTO:
            coolix->setMode(kCoolixAuto);
            break;
        case ACMode::AC_MODE_COOL:
            coolix->setMode(kCoolixCool);
            break;
        case ACMode::AC_MODE_DRY:
            coolix->setMode(kCoolixDry);
            break;
        case ACMode::AC_MODE_FAN:
            coolix->setMode(kCoolixFan);
            break;
        case ACMode::AC_MODE_HEAT:
            coolix->setMode(kCoolixHeat);
            break;
    }
}

void CoolixAC::setFanSpeed(ACFan speed) {
    switch (speed) {
        case ACFan::AC_FAN_AUTO:
            coolix->setFan(kCoolixFanAuto);
            break;
        case ACFan::AC_FAN_LOW:
            coolix->setFan(kCoolixFanMin);
            break;
        case ACFan::AC_FAN_MED:
            coolix->setFan(kCoolixFanMed);
            break;
        case ACFan::AC_FAN_HIGH:
            coolix->setFan(kCoolixFanMax);
            break;
        case ACFan::AC_FAN_AUTO_0:
            coolix->setFan(kCoolixFanAuto0);
            break;
        default:
            coolix->setFan(kCoolixFanAuto);
            break;
    }
}

bool CoolixAC::getPower() {
    return coolix->getPower();
}

uint8_t CoolixAC::getTemp() {
    return coolix->getTemp();
}


ACMode CoolixAC::getMode() {
    uint8_t mode = coolix->getMode();
    switch (mode) {
        case kCoolixAuto:
            return ACMode::AC_MODE_AUTO;
        case kCoolixCool:
            return ACMode::AC_MODE_COOL;
        case kCoolixDry:
            return ACMode::AC_MODE_DRY;
        case kCoolixFan:
            return ACMode::AC_MODE_FAN;
        case kCoolixHeat:
            return ACMode::AC_MODE_HEAT;
        default:
            return ACMode::AC_MODE_AUTO;
    }
}

ACFan CoolixAC::getFan() {
    uint8_t fan = coolix->getFan();
    switch (fan) {
        case kCoolixFanAuto:
            return ACFan::AC_FAN_AUTO;
        case kCoolixFanMin:
            return ACFan::AC_FAN_LOW;
        case kCoolixFanMed:
            return ACFan::AC_FAN_MED;
        case kCoolixFanMax:
            return ACFan::AC_FAN_HIGH;
        case kCoolixFanAuto0:
            return ACFan::AC_FAN_AUTO_0;
        default:
            return ACFan::AC_FAN_AUTO;
    }
}

String CoolixAC::toString() {
    return coolix->toString();
}

bool CoolixAC::setRawFromDecode(const decode_results& results) {
    if (results.decode_type == decode_type_t::COOLIX) {
        coolix->setRaw(results.value);
        return true;
    }
    return false;
}

void CoolixAC::sendCommand() {
    coolix->send();
}