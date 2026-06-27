#include <WString.h>
#include "ac/ACStates.h"

ACMode string2ACMode(const String str) {
    if (str.equals("AC_MODE_AUTO")) return ACMode::AC_MODE_AUTO;
    if (str.equals("AC_MODE_COOL")) return ACMode::AC_MODE_COOL;
    if (str.equals("AC_MODE_DRY")) return ACMode::AC_MODE_DRY;
    if (str.equals("AC_MODE_FAN")) return ACMode::AC_MODE_FAN;
    if (str.equals("AC_MODE_HEAT")) return ACMode::AC_MODE_HEAT;

    return ACMode::AC_MODE_AUTO;
}

String acMode2String(ACMode mode) {
    switch (mode) {
        case ACMode::AC_MODE_AUTO: return "AC_MODE_AUTO";
        case ACMode::AC_MODE_COOL: return "AC_MODE_COOL";
        case ACMode::AC_MODE_DRY: return "AC_MODE_DRY";
        case ACMode::AC_MODE_FAN: return "AC_MODE_FAN";
        case ACMode::AC_MODE_HEAT: return "AC_MODE_HEAT";
        default: return "AC_MODE_AUTO";
    }
}

ACFan string2ACFan(const String str) {
    if (str.equals("AC_FAN_AUTO_0")) return ACFan::AC_FAN_AUTO_0;
    if (str.equals("AC_FAN_AUTO")) return ACFan::AC_FAN_AUTO;
    if (str.equals("AC_FAN_LOW")) return ACFan::AC_FAN_LOW;
    if (str.equals("AC_FAN_MED")) return ACFan::AC_FAN_MED;
    if (str.equals("AC_FAN_HIGH")) return ACFan::AC_FAN_HIGH;

    return ACFan::AC_FAN_AUTO;
}

String acFan2String(ACFan mode) {
    switch (mode) {
        case ACFan::AC_FAN_AUTO_0: return "AC_FAN_AUTO_0";
        case ACFan::AC_FAN_AUTO: return "AC_FAN_AUTO";
        case ACFan::AC_FAN_LOW: return "AC_FAN_LOW";
        case ACFan::AC_FAN_MED: return "AC_FAN_MED";
        case ACFan::AC_FAN_HIGH: return "AC_FAN_HIGH";
        default: return "AC_FAN_AUTO";
    }
}