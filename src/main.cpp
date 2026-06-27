#include "Dependencies.h"

void setup() {
    Dependencies::getInstance()->cimaDevice->setup();
}

void loop() {
	Dependencies::getInstance()->cimaDevice->loop();
}