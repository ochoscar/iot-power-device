#include "DeviceConstants.h"
#include "ac/SamsungAC.h"

SamsungAC::SamsungAC(
    uint16_t irPin,
    std::function<IRsend*(uint16_t irSendPin)> irSendFactory,
    std::function<IRSamsungAc*(uint16_t irSendPin)> acFactory
) {
    irSend = irSendFactory(irPin);
    samsung = acFactory(irPin);
    irSend->begin();
    samsung->begin();
}

String SamsungAC::getACBrand() {
    return AC_BRAND_SAMSUNG;
}

void SamsungAC::setPower(bool on) {
    on ? samsung->on() : samsung->off();
}

void SamsungAC::setTemperature(uint8_t temp) {
    samsung->setTemp(temp);
}

void SamsungAC::setMode(ACMode mode) {
    switch (mode) {
        case ACMode::AC_MODE_AUTO:
            samsung->setMode(kSamsungAcAuto);
            break;
        case ACMode::AC_MODE_COOL:
            samsung->setMode(kSamsungAcCool);
            break;
        case ACMode::AC_MODE_DRY:
            samsung->setMode(kSamsungAcDry);
            break;
        case ACMode::AC_MODE_FAN:
            samsung->setMode(kSamsungAcFan);
            break;
        case ACMode::AC_MODE_HEAT:
            samsung->setMode(kSamsungAcHeat);
            break;
    }
}

void SamsungAC::setFanSpeed(ACFan speed) {
    switch (speed) {
        case ACFan::AC_FAN_AUTO:
        case ACFan::AC_FAN_AUTO_0:
            samsung->setFan(kSamsungAcFanAuto);
            break;
        case ACFan::AC_FAN_LOW:
            samsung->setFan(kSamsungAcFanLow);
            break;
        case ACFan::AC_FAN_MED:
            samsung->setFan(kSamsungAcFanMed);
            break;
        case ACFan::AC_FAN_HIGH:
            samsung->setFan(kSamsungAcFanHigh);
            break;
        default:
            samsung->setFan(kSamsungAcFanAuto);
            break;
    }
}


bool SamsungAC::getPower() {
    return samsung->getPower();
}

uint8_t SamsungAC::getTemp() {
    return samsung->getTemp();
}

ACMode SamsungAC::getMode() {
    switch (samsung->getMode()) {
        case kSamsungAcAuto:
            return ACMode::AC_MODE_AUTO;
        case kSamsungAcCool:
            return ACMode::AC_MODE_COOL;
        case kSamsungAcDry:
            return ACMode::AC_MODE_DRY;
        case kSamsungAcFan:
            return ACMode::AC_MODE_FAN;
        case kSamsungAcHeat:
            return ACMode::AC_MODE_HEAT;
        default:
            return ACMode::AC_MODE_AUTO;
    }
}

ACFan SamsungAC::getFan() {
    switch (samsung->getFan()) {
        case kSamsungAcFanAuto:
            return ACFan::AC_FAN_AUTO;
        case kSamsungAcFanLow:
            return ACFan::AC_FAN_LOW;
        case kSamsungAcFanMed:
            return ACFan::AC_FAN_MED;
        case kSamsungAcFanHigh:
            return ACFan::AC_FAN_HIGH;
        default:
            return ACFan::AC_FAN_AUTO;
    }
}

String SamsungAC::toString() {
    return samsung->toString();
}

bool SamsungAC::setRawFromDecode(const decode_results& results) {
    if (results.decode_type == decode_type_t::SAMSUNG) {
        uint8_t samsungState[14] = {0};
        for (int i = 0; i < 14 && i < results.bits / 8; ++i) {
            samsungState[i] = results.state[i];
        }
        samsung->setRaw(samsungState, 14);
        return false;
    }
    return true;
}

void SamsungAC::sendCommand() {
    samsung->send();
}