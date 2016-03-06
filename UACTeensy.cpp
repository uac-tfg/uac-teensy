#include "UACTeensy.h"

void setup() {
	Serial.begin(38400);
	pinMode(POWER_LED, OUTPUT);
	pinMode(12, OUTPUT);
	pinMode(11, OUTPUT);

	signalInit();
	audioBegin();
}

void loop() {
	int i;
	digitalWrite(12, HIGH);
	for(i = 0; i < SAMPLES_PER_BIT * 80; i++) {
		goertzelSample();
	}
	digitalWrite(12, LOW);
	signalPrint();

	//Serial.println("Start");
	//signalSynchronize();
	//Serial.println("Signal!");
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
