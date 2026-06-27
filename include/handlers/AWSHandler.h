#ifndef AWS_HANDLER_H
#define AWS_HANDLER_H

#include <LiquidCrystal_I2C.h>
#include "handlers/PublishVariables.h"

void handleAWSIncomingMessage(char* topic, unsigned char* payload, unsigned int length, Device* device);
void publishMessage(Device* device, PublishVariables publish);

#endif