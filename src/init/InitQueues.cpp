#include "init/InitQueues.h"
#include <freertos/queue.h>
#include "device.h"
#include "DeviceConstants.h"
#include "handlers/LCDHandler.h"
#include "ac/ACCommand.h"

void initQueues(Device* device) {
    device->getSerial()->println("Initializing queues...");
    device->setACCommandQueue(xQueueCreate(AC_COMMANDS_QUEUE_SIZE, sizeof(ACCommand)));
    printLCD("Queues OK", LCD_FIRST_CHAR, LCD_SECOND_ROW);
    device->getSerial()->println("Init queues done. OK.\n");
    device->delayFunc(DELAY_BETWEEN_INIT_STAGES_MS);
}