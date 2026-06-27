#ifndef DEVICE_CONSTANTS_H
#define DEVICE_CONSTANTS_H

// Config
#define USE_EEPROM_SETTINGS                 true
#define DEVICE_ID                           "67068cb7-a8d3-404d-92c8-3340f8f0ae7c"
#define AWS_IOT_TOPIC_ENV                   "prod"
#define DEFAULT_AC_BRAND                    "COOLIX"

// AC brands
// Supported values Coolix, Samsung
#define AC_BRAND_COOLIX                     "COOLIX"
#define AC_BRAND_SAMSUNG                    "SAMSUNG"
#define AC_BRAND_DAIKIN_280                 "DAIKIN280"
#define AC_BRAND_LG                         "LG"

// System parameters
#define LARGE_TASK_STACK_SIZE               8192
#define MEDIUM_TASK_STACK_SIZE              4096
#define SMALL_TASK_STACK_SIZE               2048
#define CORE_0                              0
#define CORE_1                              1
#define CO2_TASK                            "CO2_TASK"
#define LIGHT_TASK                          "LIGHT_TASK"
#define SOIL_HUMIDITY_TASK                  "SOIL_HUMIDITY_TASK" 
#define HT_TASK                             "HT_TASK"
#define RAIN_TASK                           "RAIN_TASK"
#define INIT_TASK                           "INIT_TASK"
#define SENSOR_TASK                         "SENSOR_TASK"
#define SETUP_TASK                          "SETUP_TASK"
#define AWS_TASK                            "AWS_TASK"
#define IR_TASK                             "IR_TASK"
#define AC_COMMANDS                         "AC_COMMANDS_TASK"
#define AC_COMMANDS_QUEUE_SIZE              10
#define TASK_PRIORITY_MIN                   1
#define TASK_PRIORITY_HIGH1                 10
#define TASK_PRIORITY_HIGH2                 7
#define TASK_TICKS_500                      500
#define TASK_TICKS_200                      200
#define TASK_TICKS_100                      100
#define TASK_TICKS_50                       50
#define TASK_TICKS_10                       10
#define ONESECOND_MS                        1000
#define ONEMINUTE_MS                        60000
#define FIVEMINUTES_MS                      300000      
#define TENMINUTES_MS                       600000

// Device settings
#define SERIAL_BAUD_RATE                    115000
#define RESTART_DELAY_MS                    1500
#define DELAY_BETWEEN_INIT_STAGES_MS	    700

// Pin layout
#define KEYBOARD_LEFT_PIN                   23
#define KEYBOARD_RIGHT_PIN                  15
#define KEYBOARD_UP_PIN                     5
#define KEYBOARD_DOWN_PIN                   18
#define KEYBOARD_OK_PIN                     12
#define PIR_PIN                             13
#define IR_PIN                              4
#define STATUS_LED_PIN                      2
#define ANALOG_INPUT_CURRENT_S1_PIN         32
#define ANALOG_INPUT_CURRENT_S2_PIN         34
#define IR_RECEIVE_PIN                      14
#define AC_CONTROL_PIN                      27
#define RAIN_SENSOR_PIN                     25
#define SOIL_HUMIDITY_CONTROL_PIN           19
#define ANALOG_INPUT_SOIL_HUMIDITY_PIN      33
#define I2C_SDA                             21
#define I2C_SCL                             22

// Keyboard interaction
#define PUSHED_BUTTON                       LOW

// LCD settings
#define LCD_ROWS                            2
#define LCD_COLUMNS                         16
#define LCD_FIRST_CHAR                      0
#define LCD_FIRST_ROW                       0
#define LCD_SECOND_ROW                      1

// EEPROM settings
#define EEPROM_SIZE                         704
#define EEPROM_SSID_POSITION                0
#define EEPROM_PASSWORD_POSITION            64
#define EEPROM_AC_BRAND_POSITION            128
#define EEPROM_AUTOOFF_POSITION             192
#define EEPROM_TURNOFF_TIME_POSITION        256
#define EEPROM_ENABLE_AC_POSITION           320
#define EEPROM_ENABLE_RAIN_POSITION         384
#define EEPROM_ENABLE_HT_POSITION           448
#define EEPROM_ENABLE_SOIL_POSITION         512
#define EEPROM_ENABLE_LIGHT_POSITION        576
#define EEPROM_ENABLE_CO2_POSITION          640
#define EEPROM_MAX_WORD_SIZE                64
#define EEPROM_ON_VALUE                     "ON"
#define EEPROM_OFF_VALUE                    "OFF"
#define AFTER_SAVE_EEMPORM_DELAY            2000

// Wifi settings
#define MAXIMUM_WIFI_ATTEMPTS               60
#define DEAFULT_WIFI_SSID                   "OCHOSCAR"
#define DEFAULT_WIFI_PASSWORD               "0303228485"
#define WIFI_RECONNECT_DELAY_MS             500
#define SCAN_WIFI_DELAY_MS                  10
#define WIFI_AP_SSID                        "CIMA-IOT"
#define WIFI_PASSWORD                       "cimaiotpd1"
#define WEB_SERVER_PORT                     80
#define DISPLAY_IP_DELAY                    3000

// Menu settings
#define SETTINGS_MODE_TIMEOUT_MS            3000
#define SETTINGS_MENU_TYPING_MS             100
#define SETTINGS_MODE                       0
#define NORMAL_MODE                         1
#define MAX_SETUP_MENU_OPTIONS              11
#define WORD_DELIMITER_CHAR                 '-'
#define WIFI_SSID_OPTION                    1
#define WIFI_PASSWORD_OPTION                2
#define ENABLE_AC_OPTION                    3
#define AC_BRAND_OPTION                     4
#define AUTOFF_OPTION                       5
#define TURNOFF_TIME_OPTION                 6
#define ENABLE_RAIN_OPTION                  7
#define ENABLE_HT_OPTION                    8
#define ENABLE_SOIL_OPTION                  9
#define ENABLE_LIGHT_OPTION                 10
#define ENABLE_CO2_OPTION                   11

// AWS settings
#define AWS_IOT_ENDPOINT				    "a2vduqt2lbdw5l-ats.iot.us-east-1.amazonaws.com"
#define AWS_IOT_PORT					    8883
#define AWS_RECONNECT_DELAY_MS			    100
#define THING_NAME_SUFIX                    "-device"
#define AWS_IOT_PREFIX                      "cimaiot"
#define AWS_IOT_SUBSCRIBE_TOPIC_SUFIX       "_device/read"
#define AWS_IOT_PUBLISH_TOPIC_SUFIX         "_device/data"
#define INIT_AWS_MAX_TIME_SEC               60000
#define INIT_AWS_DELAY_TIME_MS              1000

// Sensor settings
#define LCD_ADDRESS                         0x27
#define SHT_ADDRESS                         0x44
#define CO2_ADDRESS                         0x62
#define BH1750_ADDRESS                      0x23
#define MAX_ADC_RESOLUTION                  4095 

// Power device settings
#define TIME_BETWEEN_SAMPLES_MS             30000
#define AC_MAX_TEMP                         30
#define AC_MIN_TEMP                         16
#define DEVICE_OFF                          "DEVICE_OFF"
#define DEVICE_ON                           "DEVICE_ON"
#define TEMP_ONBOARD                        "TEMP_ONBOARD"
#define TEMP_UP                             "TEMP_UP"
#define TEMP_DOWN                           "TEMP_DOWN"
#define FAN                                 "AC_FAN"
#define AC_MODE                             "AC_MODE"
#define NO_MOTION_TIME                      300000
#define AWS_DEVICE_ID                       "device_id"
#define CURRENT_VARIABLE_NAME               "current_1p"
#define TEMP_VARIABLE_NAME                  "ac_temp"
#define POWER_VARIABLE_NAME                 "ac_on_off"
#define MODE_VARIABLE_NAME                  "ac_mode"
#define FAN_VARIABLE_NAME                   "ac_fan"
#define PIR_OFF_VARIABLE_NAME               "pir_off"
#define RAIN_VARIABLE_NAME                  "rain"
#define ENV_TEMP_VARIABLE_NAME              "env_temp"
#define ENV_HUMIDITY_VARIABLE_NAME          "env_humidity"
#define ENV_LIGHT_VARIABLE_NAME             "env_light"
#define SOIL_HUMIDITY_VARIABLE_NAME         "soil_humidity"
#define CO2_VARIABLE_NAME                   "co2"
#define ENV_TEMP_SCD4X_VARIABLE_NAME        "env_temp_scd"
#define ENV_HUMIDITY_SCD4X_VARIABLE_NAME    "env_humidity_scd"
#define DEVICE_TEMP_VARIABLE_NAME           "onboard_temp"
#define INIT_FROM_AWS                       "init"
#define ACTION_ON_OFF                       "on-off"
#define ACTION_SET_TEMP_UP_DOWN             "temp-up-down"
#define ACTION_SET_TEMP                     "set-temp"
#define ACTION_SET_FAN                      "set-fan"
#define ACTION_AC_MODE                      "ac-mode"
#define TOTAL_CURRENT_SAMPLES               5000
#define CURRENT_DELAY_BETWEEN_SAMPLES       17
#define AC_INIT_POWER                       false
#define AC_INIT_TEMP                        20
#define RAIN_SENSOR_MM_PER_PULSE            0.2f

#endif