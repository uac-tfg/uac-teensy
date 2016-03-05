#include "UACTeensy.h"

void setup() {
	Serial.begin(38400);
	pinMode(POWER_LED, OUTPUT);

	signalInit();
}

void loop() {
	signalSynchronize();
}
