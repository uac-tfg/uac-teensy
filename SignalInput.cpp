/*
 * SignalInput.cpp
 *
 *  Created on: 04.03.2016
 *      Author: t
 */

#include "SignalInput.h"

IntervalTimer samplingTimer;

struct goertzel goertzels[SAMPLES_PER_BIT * 2];
int offset;
int max;

float samples[AUDIO_BUFFER_SIZE];
int samplesIn;
int samplesOut;

// DEBUG
float maxMagnitudeHigh;
float maxMagnitudeLow;

void signalPrint() {
	Serial.print("High: ");
	Serial.println(maxMagnitudeHigh);
	Serial.print("Low:  ");
	Serial.println(maxMagnitudeLow);
	maxMagnitudeHigh = 0;
	maxMagnitudeLow = 0;
}

////////////////////////////////////////////////////////////////////////////////
// SIGNAL
////////////////////////////////////////////////////////////////////////////////
void signalInit() {
	audioInit();
	goertzelInit();

	// DEBUG
	pinMode(11, OUTPUT);
	maxMagnitudeHigh = 0;
	maxMagnitudeLow = 0;
}

void signalSynchronize() {
	int i;
	for(i = 0; i < SAMPLES_PER_BIT * 2; i++) {
		struct goertzel *g = goertzels + i;
		g->enabled = true;
	}
	audioBegin();
	for(int i = 0; i < SAMPLES_PER_BIT * 100; i++) {
		goertzelSample();
	}
	signalPrint();
}

////////////////////////////////////////////////////////////////////////////////
// GOERTZEL
////////////////////////////////////////////////////////////////////////////////

void goertzelInit() {
	int i;
	for(i = 0; i < SAMPLES_PER_BIT * 2; i++) {
		struct goertzel gHigh;
		gHigh.d0 = 0;
		gHigh.d1 = 0;
		gHigh.d2 = 0;
		gHigh.mag = 0;
		gHigh.enabled = true;
		goertzels[i] = gHigh;

		struct goertzel gLow;
		gLow.d0 = 0;
		gLow.d1 = 0;
		gLow.d2 = 0;
		gLow.mag = 0;
		gLow.enabled = true;
		goertzels[i] = gLow;
	}
	offset = 0;
}

void goertzelBlock(int i) {
	struct goertzel *gHigh = goertzels + i * 2;
	float realHigh;
	float imaHigh;

	struct goertzel *gLow = goertzels + i * 2 + 1;
	float realLow;
	float imaLow;

	gHigh->d0 = GOERTZEL_A_HIGH * gHigh->d1 - gHigh->d2;
	gHigh->d2 = gHigh->d1;
	gHigh->d1 = gHigh->d0;
	realHigh = gHigh->d1 - gHigh->d2 * GOERTZEL_COSINUS_HIGH;
	imaHigh = gHigh->d2 * GOERTZEL_SINUS_HIGH;
	gHigh->mag = realHigh * realHigh + imaHigh * imaHigh;
	if(gHigh->mag > maxMagnitudeHigh) {
		maxMagnitudeHigh = gHigh->mag;
	}
	gHigh->d0 = 0;
	gHigh->d1 = 0;
	gHigh->d2 = 0;

	gLow->d0 = GOERTZEL_A_LOW * gLow->d1 - gLow->d2;
	gLow->d2 = gLow->d1;
	gLow->d1 = gLow->d0;
	realLow = gLow->d1 - gLow->d2 * GOERTZEL_COSINUS_LOW;
	imaLow = gLow->d2 * GOERTZEL_SINUS_LOW;
	gLow->mag = realLow * realLow + imaLow * imaLow;
	if(gLow->mag > maxMagnitudeLow) {
		maxMagnitudeLow = gLow->mag;
	}
	gLow->d0 = 0;
	gLow->d1 = 0;
	gLow->d2 = 0;
}

void goertzelSample() {
	float sample = audioRead();
	offset++;
	if(offset == SAMPLES_PER_BIT) {
		offset = 0;
	}
	int i;
	for(i = 0; i < SAMPLES_PER_BIT * 2; i++) {
		struct goertzel *gHigh = goertzels + i * 2;
		if(gHigh->enabled) {
			struct goertzel *gLow = goertzels + i * 2 + 1;

			gHigh->d0 = GOERTZEL_A_HIGH * gHigh->d1 - gHigh->d2 + sample;
			gHigh->d2 = gHigh->d1;
			gHigh->d1 = gHigh->d0;
			gLow->d0 = GOERTZEL_A_LOW * gLow->d1 - gLow->d2 + sample;
			gLow->d2 = gLow->d1;
			gLow->d1 = gLow->d0;

			if(i == offset) {
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

void audioCallback() {
	// Read from the ADC and store the sample data
	int sample = analogRead(AUDIO_INPUT_PIN);
	float fSample = (sample - AUDIO_MIDDLE) / (float) AUDIO_MIDDLE;
	samples[samplesIn] = fSample;
	samplesIn++;
	if(samplesIn == AUDIO_BUFFER_SIZE) {
		samplesIn = 0;
	}
	if(samplesIn == samplesOut) {
		samplesOut++;
		if(samplesOut == AUDIO_BUFFER_SIZE) {
			samplesOut = 0;
		}
	}
}

bool audioAvailable() {
	return samplesOut != samplesIn;
}

float audioRead() {
	//while(!audioAvailable()) { delay(1); }
	float sample = samples[samplesOut];
	samplesOut++;
	if(samplesOut == AUDIO_BUFFER_SIZE) {
		samplesOut = 0;
	}
	return sample;
}
