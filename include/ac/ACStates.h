#ifndef AC_STATES_H
#define AC_STATES_H

enum class ACMode {
    AC_MODE_AUTO,
    AC_MODE_COOL,
    AC_MODE_DRY,
    AC_MODE_FAN,
    AC_MODE_HEAT
};

enum class ACFan {
    AC_FAN_AUTO_0,
    AC_FAN_AUTO,
    AC_FAN_LOW,
    AC_FAN_MED,
    AC_FAN_HIGH,
};

ACMode string2ACMode(const String str);
String acMode2String(ACMode mode);
ACFan string2ACFan(const String str);
String acFan2String(ACFan mode);

#endif