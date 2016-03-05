#include "UACTeensy.h"

void setup() {
	Serial.begin(38400);
	pinMode(POWER_LED, OUTPUT);
	pinMode(12, OUTPUT);
	pinMode(11, OUTPUT);

	signalInit();
}

void loop() {
	delay(1000 * 5);
	Serial.println("Start");
	signalSynchronize();
	Serial.println("Signal!");
	/*int i;
	for(i = 0; i < 32; i++) {
		char bit = signalReadBit();
		if(bit == 1) {
			Serial.print('1');
		} else {
			Serial.print('0');
		}
	}*/
}
