#include "UACTeensy.h"

void setup() {
	Serial.begin(38400);
	pinMode(POWER_LED, OUTPUT);

	signalInit();
	signalSynchronize();
}

void loop() {
	signalPrint();
	digitalWrite(POWER_LED, HIGH);
	delay(500);
	digitalWrite(POWER_LED, LOW);
	delay(500);
}
