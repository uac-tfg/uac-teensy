#include "UACTeensy.h"

void setup() {
	Serial.begin(9600);
	pinMode(POWER_LED, OUTPUT);
	pinMode(12, OUTPUT);
	pinMode(11, OUTPUT);

	signalInit();
}

void loop() {
	/*
	int i;
	digitalWrite(13, LOW);
	digitalWrite(12, HIGH);
	for(i = 0; i < SAMPLES_PER_BIT * 5; i++) {
		goertzelSample();
	}
	digitalWrite(12, LOW);
	digitalWrite(13, HIGH);
	signalPrint();*/

	signalSynchronize();
	int i;
	int bitsc = 8 * 1;
	char bits[bitsc];
	for(i = 0; i < bitsc; i++) {
		char bit = signalReadBit();
		bits[i] = bit;
	}
	audioEnd();

	for(i = 0; i < bitsc; i++) {
		digitalWrite(13, HIGH);
		if(bits[i] == '1') {
			digitalWrite(12, HIGH);
		} else {
			digitalWrite(12, LOW);
		}
		delay(100);
		digitalWrite(13, LOW);
		delay(900);
	}
	digitalWrite(12, LOW);

	/*Serial.println("Start");
	int i = 0;
	while(true) {
		char next = SYNC_BITS[i];
		char curr = signalReadBit();
		if(curr == '1') {
			digitalWrite(13, HIGH);
		} else {
			digitalWrite(13, LOW);
		}
		if(curr == next) {
			i++;
		} else {
			i = 0;
		}
		if(i > SYNC_BITS_C / 2) {
			Serial.println(i);
		}
		if(i == SYNC_BITS_C) {
			break;
		}
	}
	Serial.println("Signal");
	for(i = 0; i < 32; i++) {
		char bit = signalReadBit();
		Serial.print(bit);
	}
	Serial.println();*/

}
