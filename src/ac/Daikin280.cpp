#include "DeviceConstants.h"
#include "ac/DaikinAC280.h"
#include "ac/ACStates.h"

DaikinAC280::DaikinAC280(
    uint16_t irPin,
    std::function<IRsend*(uint16_t irSendPin)> irSendFactory,
    std::function<IRDaikinESP*(uint16_t irSendPin)> acFactory
) {
    irSend = irSendFactory(irPin);
    daikin = acFactory(irPin);
    irSend->begin();
    daikin->begin();
}

String DaikinAC280::getACBrand() {
    return AC_BRAND_COOLIX;
}

void DaikinAC280::setPower(bool on) {
    on ? daikin->on() : daikin->off();
}

void DaikinAC280::setTemperature(uint8_t temp) {
    daikin->setTemp(temp);
}

void DaikinAC280::setMode(ACMode mode) {
    switch (mode) {
        case ACMode::AC_MODE_AUTO:
            daikin->setMode(kDaikinAuto);
            break;
        case ACMode::AC_MODE_COOL:
            daikin->setMode(kDaikinCool);
            break;
        case ACMode::AC_MODE_DRY:
            daikin->setMode(kDaikinDry);
            break;
        case ACMode::AC_MODE_FAN:
            daikin->setMode(kDaikinFan);
            break;
        case ACMode::AC_MODE_HEAT:
            daikin->setMode(kDaikinHeat);
            break;
    }
}

void DaikinAC280::setFanSpeed(ACFan speed) {
    switch (speed) {
        case ACFan::AC_FAN_AUTO:
            daikin->setFan(kDaikinFanAuto);
            break;
        case ACFan::AC_FAN_LOW:
            daikin->setFan(kDaikinFanMin);
            break;
        case ACFan::AC_FAN_MED:
            daikin->setFan(kDaikinFanQuiet);
            break;
        case ACFan::AC_FAN_HIGH:
            daikin->setFan(kDaikinFanMax);
            break;
        case ACFan::AC_FAN_AUTO_0:
            daikin->setFan(kDaikinFanAuto);
            break;
        default:
            daikin->setFan(kDaikinFanAuto);
            break;
    }
}

bool DaikinAC280::getPower() {
    return daikin->getPower();
}

uint8_t DaikinAC280::getTemp() {
    return daikin->getTemp();
}


ACMode DaikinAC280::getMode() {
    uint8_t mode = daikin->getMode();
    switch (mode) {
        case kDaikinAuto:
            return ACMode::AC_MODE_AUTO;
        case kDaikinCool:
            return ACMode::AC_MODE_COOL;
        case kDaikinDry:
            return ACMode::AC_MODE_DRY;
        case kDaikinFan:
            return ACMode::AC_MODE_FAN;
        case kDaikinHeat:
            return ACMode::AC_MODE_HEAT;
        default:
            return ACMode::AC_MODE_AUTO;
    }
}

ACFan DaikinAC280::getFan() {
    uint8_t fan = daikin->getFan();
    switch (fan) {
        case kDaikinFanAuto:
            return ACFan::AC_FAN_AUTO;
        case kDaikinFanMin:
            return ACFan::AC_FAN_LOW;
        case kDaikinFanQuiet:
            return ACFan::AC_FAN_MED;
        case kDaikinFanMax:
            return ACFan::AC_FAN_HIGH;
        default:
            return ACFan::AC_FAN_AUTO;
    }
}

String DaikinAC280::toString() {
    return daikin->toString();
}

bool DaikinAC280::setRawFromDecode(const decode_results& results) {
    if (results.decode_type == decode_type_t::DAIKIN) {
        uint8_t daikinState[35] = {0};
        for (int i = 0; i < 35 && i < results.bits / 8; ++i) {
            daikinState[i] = results.state[i];
        }
        daikin->setRaw(daikinState, 35);
        return false;
    }
    return true;
}


void DaikinAC280::sendCommand() {
    daikin->send();
}