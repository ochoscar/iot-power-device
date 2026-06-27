#include <IRrecv.h>
#include <IRremoteESP8266.h>
#include <IRutils.h>
#include "Device.h"
#include "DeviceConstants.h"
#include "loops/ACIRLoop.h"
#include "handlers/AWSHandler.h"
#include "handlers/PublishVariables.h"

void ACIRLoop(Device* device) {
    decode_results results;
    PublishVariables publish = {
        .txAC = true,
        .txRain = false,
        .txHT = false,
        .txSoil = false,
        .txLight = false,
        .txCo2 = false,
        .init = false
    };

    if (device->getACIRRecv()->decode(&results)) {
        String message = resultToHumanReadableBasic(&results);
        device->getACIRRecv()->resume();
        bool validRaw = device->getAC()->setRawFromDecode(results);

        if(validRaw) {
            device->getSerial()->println("IR Message: " + message);
            device->setACTemp(device->getAC()->getTemp());
            device->setACMode(device->getAC()->getMode());
            device->setACFan(device->getAC()->getFan());
            device->setACPower(device->getAC()->getPower());
    
            device->getSerial()->printf("Temp: %d, Mode: %d, Fan: %d, Power: %d\n",
                device->getACTemp(),
                device->getACMode(),
                device->getACFan(),
                device->getACPower()
            );
    
            publishMessage(device, publish);
        }
        
    }
}