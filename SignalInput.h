/*
 * SignalInput.h
 *
 *  Created on: 04.03.2016
 *      Author: t
 */

#ifndef SIGNALINPUT_H_
#define SIGNALINPUT_H_

#include "Arduino.h"

// Signal
#define SAMPLE_RATE_HZ 2500
#define FREQUENCY_HIGH 350
#define FREQUENCY_LOW 250
#define PERIODS_PER_BIT 3
#define SAMPLES_PER_BIT 30

// Goertzel
#define GOERTZEL_SINUS_HIGH 0.7705132427757893
#define GOERTZEL_COSINUS_HIGH 0.6374239897486896
#define GOERTZEL_A_HIGH 1.2748479794973793
#define GOERTZEL_SINUS_LOW 0.5877852522924731
#define GOERTZEL_COSINUS_LOW  0.8090169943749475
#define GOERTZEL_A_LOW 1.618033988749895

// Audio
#define AUDIO_INPUT_PIN 14        // Input ADC pin for audio data.
#define AUDIO_READ_RESOLUTION 12 // Bits of resolution for the ADC.
#define AUDIO_READ_AVERAGING 1  // Number of samples to average with each ADC reading.
#define AUDIO_MIDDLE 1522
#define AUDIO_BUFFER_SIZE 128

// DEBUG
void signalPrint();

// Signal
void signalInit();
void signalSynchronize();

// Goertzel
struct goertzel {
	float d0;
	float d1;
	float d2;
	float mag;
	bool enabled;
};

void goertzelInit();
void goertzelBlock(int i);
void goertzelSample();

// Audio
void audioInit();
void audioBegin();
void audioCallback();
bool audioAvailable();
float audioRead();


#endif /* SIGNALINPUT_H_ */
