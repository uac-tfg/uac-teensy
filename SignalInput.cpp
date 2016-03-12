/*
 * SignalInput.cpp
 *
 *  Created on: 04.03.2016
 *      Author: t
 */

#include "SignalInput.h"

IntervalTimer samplingTimer;

float d0High[GOERTZEL_COUNT];
float d1High[GOERTZEL_COUNT];
float d2High[GOERTZEL_COUNT];
float magHigh[GOERTZEL_COUNT];

float d0Low[GOERTZEL_COUNT];
float d1Low[GOERTZEL_COUNT];
float d2Low[GOERTZEL_COUNT];
float magLow[GOERTZEL_COUNT];

bool enabled[GOERTZEL_COUNT];

int offset;
int max;

int samples[AUDIO_BUFFER_SIZE];
int samplesIn;
int samplesOut;

char* syncbits = SYNC_BITS;

// DEBUG
bool ledOn = false;
bool ledOn2 = false;

float maxMagHigh = 0;
float maxMagLow = 0;

void signalPrint() {
	Serial.print("High: ");
	Serial.println(maxMagHigh);
	Serial.print("Low:  ");
	Serial.println(maxMagLow);
	maxMagHigh = 0;
	maxMagLow = 0;
}

float getMaxMagHigh() {
	return maxMagHigh;
}

float getMaxMagLow() {
	return maxMagLow;
}

////////////////////////////////////////////////////////////////////////////////
// SIGNAL
////////////////////////////////////////////////////////////////////////////////
void signalInit() {
	audioInit();
	goertzelInit();
}

void signalSynchronize() {
	audioBegin();
	int pos[GOERTZEL_COUNT];
	float sumHigh[GOERTZEL_COUNT];
	float sumLow[GOERTZEL_COUNT];
	int i;
	for(i = 0; i < GOERTZEL_COUNT; i++) {
		pos[i] = 0;
		sumHigh[i] = 0;
		sumLow[i] = 0;
		enabled[i] = true;
	}
	int maxIndex = -1;
	while (true) {
		goertzelSample();
		if(offset % GOERTZEL_DISTANCE == 0) {
			int index = offset / GOERTZEL_DISTANCE;
			char next = SYNC_BITS[pos[index]];
			float high = magHigh[index];
			float low = magLow[index];
			float diff = high - low;
			if (index == 0 && diff > 0) {
				digitalWrite(13, HIGH);
			} else if(index == 0) {
				digitalWrite(13, LOW);
			}
			if (diff > 0 && next == '1') {
				pos[index] = pos[index] + 1;
				sumHigh[index] = sumHigh[index] + high;
			} else if(next == '0') {
				pos[index] = pos[index] + 1;
				sumLow[index] = sumLow[index] + low;
			} else {
				pos[index] = 0;
				sumHigh[index] = 0;
				sumLow[index] = 0;
			}
			if (pos[index] == SYNC_BITS_C) {
				if(maxIndex == -1 || (sumHigh[index] + sumLow[index] > sumHigh[maxIndex] + sumLow[maxIndex])) {
					maxIndex = index;
				}
			}
			if(maxIndex != -1) {
				i++;
				if(i == SAMPLES_PER_BIT) {
					break;
				}
			}
		}
	}
	max = maxIndex;
	for(i = 0; i < GOERTZEL_COUNT; i++) {
		if(i != max) {
			enabled[i] = false;
		}
	}
}

char signalReadBit() {
	int i;
	for (i = 0; i < SAMPLES_PER_BIT; i++) {
		goertzelSample();
	}
	float diff = magHigh[max] - magLow[max];
	if (diff > 0) {
		return '1';
	} else {
		return '0';
	}
}

char signalReadByte() {
	int i;
	char byte = 0;
	char bit;
	for(i = 0; i < 8; i++) {
		bit = signalReadBit();
		if(bit == '1') {
			byte |= (1 << (7 - i));
		}
	}
	return byte;
}

////////////////////////////////////////////////////////////////////////////////
// GOERTZEL
////////////////////////////////////////////////////////////////////////////////

void goertzelInit() {
	int i;
	for (i = 0; i < GOERTZEL_COUNT; i++) {
		d0High[i] = 0;
		d1High[i] = 0;
		d2High[i] = 0;

		d0Low[i] = 0;
		d1Low[i] = 0;
		d2Low[i] = 0;

		enabled[i] = true;
	}
	offset = 0;
}

void goertzelBlock(int i) {
	float realHigh;
	float imaHigh;

	float realLow;
	float imaLow;

	d0High[i] = GOERTZEL_A_HIGH * d1High[i] - d2High[i];
	d2High[i] = d1High[i];
	d1High[i] = d0High[i];
	realHigh = d1High[i] - d2High[i] * GOERTZEL_COSINUS_HIGH;
	imaHigh = d2High[i] * GOERTZEL_SINUS_HIGH;
	magHigh[i] = realHigh * realHigh + imaHigh * imaHigh;
	if (magHigh[i] > maxMagHigh) {
		maxMagHigh = magHigh[i];
	}
	d0High[i] = 0;
	d1High[i] = 0;
	d2High[i] = 0;

	d0Low[i] = GOERTZEL_A_LOW * d1Low[i] - d2Low[i];
	d2Low[i] = d1Low[i];
	d1Low[i] = d0Low[i];
	realLow = d1Low[i] - d2Low[i] * GOERTZEL_COSINUS_LOW;
	imaLow = d2Low[i] * GOERTZEL_SINUS_LOW;
	magLow[i] = realLow * realLow + imaLow * imaLow;
	if (magLow[i] > maxMagLow) {
		maxMagLow = magLow[i];
	}
	d0Low[i] = 0;
	d1Low[i] = 0;
	d2Low[i] = 0;
}

void goertzelSample() {
	int sample = audioRead() - AUDIO_MIDDLE;
	offset++;
	if (offset == SAMPLES_PER_BIT) {
		offset = 0;
	}
	int i;
	for (i = 0; i < GOERTZEL_COUNT; i++) {
		if (enabled[i]) {
			d0High[i] = GOERTZEL_A_HIGH * d1High[i] - d2High[i] + sample;
			d2High[i] = d1High[i];
			d1High[i] = d0High[i];
			d0Low[i] = GOERTZEL_A_LOW * d1Low[i] - d2Low[i] + sample;
			d2Low[i] = d1Low[i];
			d1Low[i] = d0Low[i];

			if (i * GOERTZEL_DISTANCE == offset) {
				goertzelBlock(i);
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
// AUDIO
////////////////////////////////////////////////////////////////////////////////
void audioInit() {
	pinMode(AUDIO_INPUT_PIN, INPUT);
	analogReadResolution(AUDIO_READ_RESOLUTION);
	analogReadAveraging(AUDIO_READ_AVERAGING);
}

void audioBegin() {
	samplesIn = 0;
	samplesOut = 0;
	// start callback at necessary rate
	samplingTimer.begin(audioCallback, 1000000 / SAMPLE_RATE_HZ);
}

void audioEnd() {
	samplingTimer.end();
}

void audioCallback() {
	// Read from the ADC and store the sample data
	int sample = analogRead(AUDIO_INPUT_PIN);
	samples[samplesIn] = sample;
	samplesIn++;
	if (samplesIn == AUDIO_BUFFER_SIZE) {
		samplesIn = 0;
	}
	if (samplesIn == samplesOut) {
		if (ledOn) {
			digitalWrite(11, LOW);
			ledOn = false;
		} else {
			digitalWrite(11, HIGH);
			ledOn = true;
		}
		samplesOut++;
		if (samplesOut == AUDIO_BUFFER_SIZE) {
			samplesOut = 0;
		}
	}
}

bool audioAvailable() {
	return samplesOut != samplesIn;
}

int audioRead() {
	while (!audioAvailable()) {
		Serial.print("");
	}
	int sample = samples[samplesOut];
	samplesOut++;
	if (samplesOut == AUDIO_BUFFER_SIZE) {
		samplesOut = 0;
	}
	return sample;
}
