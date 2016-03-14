#include "UACTeensy.h"

LiquidCrystal_I2C lcd(0x27, 20, 4);

void setup() {
	Serial.begin(9600);
	pinMode(POWER_LED, OUTPUT);
	pinMode(12, OUTPUT);
	pinMode(11, OUTPUT);

	signalInit();

	lcd.init();
	lcd.backlight();
}

void loop() {
	/*int i;
	digitalWrite(13, LOW);
	digitalWrite(12, HIGH);
	audioBegin();
	for(i = 0; i < SAMPLES_PER_BIT * 70; i++) {
		goertzelSample();
	}
	audioEnd();
	digitalWrite(12, LOW);
	digitalWrite(13, HIGH);
	signalPrint();*/
	//lcd.setCursor(0, 0);
	//lcd.print(getMaxMagHigh());
	//lcd.setCursor(0, 1);
	//lcd.print(getMaxMagLow());*/

	signalSynchronize();

	char count = signalReadByte();
	Serial.println(count);
	char i;
	char row = 0;
	char column = 0;
	lcd.setCursor(column, row);
	for(i = 0; i < count; i++) {
		char c = signalReadByte();
		if(c == '\n' || column == 20) {
			row++;
			column = 0;
			if(row == 4) {
				break;
			}
			lcd.setCursor(column, row);
		}
		lcd.print(c);
		column++;
	}
	audioEnd();
	for(i = column; i < 20; i++) {
		lcd.print(" ");
	}
	row++;
	for(i = row; i < 4; i++) {
		lcd.setCursor(0, i);
		lcd.print("                    ");
	}

	/*int i;
	int bitsc = 8 * 2;
	lcd.setCursor(2, 1);
	char bits[bitsc];
	for(i = 0; i < bitsc; i++) {
		char bit = signalReadBit();
		bits[i] = bit;
		Serial.print(bits[i]);
		lcd.print(bits[i]);
	}
	audioEnd();*/
	Serial.println();

	/*for(i = 0; i < bitsc; i++) {
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
	digitalWrite(12, LOW);*/

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
