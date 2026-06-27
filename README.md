# CIMA IOT Power Device 1

## Settings

1. To Enter in settings mode: turn on device with UP and DOWN button pushed for 3 seconds
2. To save SSID or PASSWORD: Use keyboard to set SSID or PASSWORD and then hold UP and DOWN buttons until your see a message in LCD.

## Pinout

### LCD

* PIN21 = SDA
* PIN22 = SCL

### Keyboard

* PIN23 = LEFT
* PIN15 = RIGHT
* PIN05 = UP
* PIN18 = DOWN

### Sensors

* PIN32 = I1
* PIN33 = I2
* PIN34 = I3

### Device under control

* PIN13 = DEVICE ON/OFF 

ON  = HIGH
OFF = LOW

## EEMPROM

```
[000-015] -> SSID
[016-031] -> PASSWORD
```

## Messages

### FROM AWS

* Topic: cimaiot/THING_NAME/dev_device/read
* Topic: cimaiot/THING_NAME/prod_device/read
* Body: 

```
INIT-DEVICE_OFF
INIT-DEVICE_ON
PULSE_OFF
DEVICE_OFF
DEVICE_ON
TEMP_ONBOARD
```