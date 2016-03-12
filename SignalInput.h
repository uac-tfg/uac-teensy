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
#define PERIODS_PER_BIT 4
#define SAMPLES_PER_BIT 40
//#define SYNC_BITS "11110000111100001111000011110000"
#define SYNC_BITS "10011001110011010110000111001100"
#define SYNC_BITS_C 32

// Goertzel
#define GOERTZEL_SINUS_HIGH 0.7705132427757893
#define GOERTZEL_COSINUS_HIGH 0.6374239897486896
#define GOERTZEL_A_HIGH 1.2748479794973793
#define GOERTZEL_SINUS_LOW 0.5877852522924731
#define GOERTZEL_COSINUS_LOW  0.8090169943749475
#define GOERTZEL_A_LOW 1.618033988749895

/*#define GOERTZEL_PRECISION 1000000
#define GOERTZEL_SINUS_HIGH 770513
#define GOERTZEL_COSINUS_HIGH 637423
#define GOERTZEL_A_HIGH 1274847
#define GOERTZEL_SINUS_LOW 587785
#define GOERTZEL_COSINUS_LOW  809016
#define GOERTZEL_A_LOW 1618033*/

/*#define GOERTZEL_PRECISION 1000
#define GOERTZEL_SINUS_HIGH 770
#define GOERTZEL_COSINUS_HIGH 637
#define GOERTZEL_A_HIGH 1274
#define GOERTZEL_SINUS_LOW 587
#define GOERTZEL_COSINUS_LOW  809
#define GOERTZEL_A_LOW 1618*/

#define GOERTZEL_DISTANCE 2
#define GOERTZEL_COUNT SAMPLES_PER_BIT / GOERTZEL_DISTANCE

// Audio
#define AUDIO_INPUT_PIN 14        // Input ADC pin for audio data.
#define AUDIO_READ_RESOLUTION 12 // Bits of resolution for the ADC.
#define AUDIO_READ_AVERAGING 1  // Number of samples to average with each ADC reading.
#define AUDIO_MIDDLE 1500
#define AUDIO_BUFFER_SIZE 10

// DEBUG
void signalPrint();
float getMaxMagHigh();
float getMaxMagLow();

// Signal
void signalInit();
void signalSynchronize();
char signalReadBit();
char signalReadByte();

// Goertzel
void goertzelInit();
void goertzelBlock(int i);
void goertzelSample();

// Audio
void audioInit();
void audioBegin();
void audioEnd();
void audioCallback();
bool audioAvailable();
int audioRead();


#endif /* SIGNALINPUT_H_ */
