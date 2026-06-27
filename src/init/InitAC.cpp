#include <IRrecv.h>
#include "Device.h"
#include "DeviceConstants.h"
#include "init/InitAC.h"
#include "ac/AirConditionerFactory.h"
#include "handlers/LCDHandler.h"

void initAC(Device* device) {
    if(!device->getEnableAC()) {
        device->getSerial()->println("No AC enabled. Skipping AC initialization.");
        printLCD("SKIPPING AC", LCD_FIRST_CHAR, LCD_SECOND_ROW);
        device->delayFunc(DELAY_BETWEEN_INIT_STAGES_MS);
        return;
    }
    device->getSerial()->println("Initializing AC communication...");
    IAirConditioner* ac = AirConditionerFactory::createAC(
        device->getACBrand(),
        IR_PIN,
        device->irSendFactory,
        device->acFactoryCoolix,
        device->acFactorySamsung,
        device->acFactoryDaikin280,
        device->acFactoryLG
    );
    device->setAC(ac);

    IRrecv* irrecv = device->irRecvFactory(IR_RECEIVE_PIN);
    irrecv->enableIRIn();
    device->setACIRRecv(irrecv);
    
    if(irrecv != nullptr && ac != nullptr) {
        printLCD("AC OK", LCD_FIRST_CHAR, LCD_SECOND_ROW);
        device->getSerial()->println("Init AC communication done. OK.\n");
    } else {
        printLCD("AC ERROR", LCD_FIRST_CHAR, LCD_SECOND_ROW);
        device->getSerial()->println("Init AC communication Error. Restarging.\n");
        device->delayFunc(RESTART_DELAY_MS);
        device->getESP()->restart();
    }
    device->delayFunc(DELAY_BETWEEN_INIT_STAGES_MS);
}