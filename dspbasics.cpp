﻿// DSPbasics.cpp : Defines the entry point for the application.
//

#define _USE_MATH_DEFINES
#include <cmath>
#include <vector>
#include <iostream>
#include "sndfile.hh"


using namespace std;

// Utility method. Print some info about a sndfile

void printFileInfo(SndfileHandle sf) {
    cout << "    Sample rate : " << sf.samplerate() << endl;
    cout << "    Channels    : " << sf.channels() << endl;
    cout << "    Frames      : " << sf.frames() << endl;
    cout << "    Format      : ";

    switch (sf.format() & SF_FORMAT_TYPEMASK) {
    case SF_FORMAT_CAF:
        cout << "CAF" << endl;
        break;
    case SF_FORMAT_WAV:
        cout << "WAV" << endl;
        break;
    case SF_FORMAT_AIFF:
        cout << "AIFF" << endl;
        break;
    default:
        cout << "other: " << (sf.format() & SF_FORMAT_TYPEMASK) << endl;
    }
    cout << "    PCM         : ";
    switch (sf.format() & SF_FORMAT_SUBMASK) {
    case SF_FORMAT_PCM_16:
        cout << "16 bit" << endl;
        break;
    case SF_FORMAT_PCM_24:
        cout << "24 bit" << endl;
        break;
    case SF_FORMAT_PCM_32:
        cout << "32 bit" << endl;;
        break;
    case SF_FORMAT_FLOAT:
        cout << "32 bit float" << endl;
        break;
    default:
        cout << "other: " << (sf.format() & SF_FORMAT_SUBMASK) << endl;
    }
}

// Synthesis
// =========

// Create a sine tone by sampling a sine wave
// buf - array to write samples to
// frequency - in Hertz, for example 440
// numFrames - number of samples, also minimum size of buffer
// sampleRate - in samples/second, for example 44100

void createTone(float* buf, int frequency, int numFrames, int sampleRate)
{
    for (int i = 0; i < numFrames; i++)
    {
        float sample = 0.0;
        // Generate a sample value. Samples are normalized [-1.0, 1.0]
        // Multiplication with 0.30 descreases the amplitude      
        sample += 0.3 * sin(2 * M_PI * i * frequency / sampleRate);

        // Here we can add som harmonics if we want.
        // Double the frequency
        //sample += 0.25 * sin(2 * M_PI * i * 2 * frequency / sampleRate);
        // Triple the frequency
        //sample += 0.20 * sin(2 * M_PI * i * 3 * frequency / sampleRate);
        // Etc.
        //sample += 0.15 * sin(2 * M_PI * i * 4 * frequency / sampleRate);
        //sample += 0.10 * sin(2 * M_PI * i * 5 * frequency / sampleRate);
         
        // Write sample to the buffer
        *buf++ = sample; 
    }
}

// Create white noise
// Algorithm from https://www.musicdsp.org/en/latest/Synthesis/216-fast-whitenoise-generator.html

void createWhiteNoise(float* buf, int numFrames, float level = 0.30f)
{
    float scale = 2.0f / 0xffffffff;
    int x1 = 0x67452301;
    int x2 = 0xefcdab89;
    level *= scale;

    while (numFrames--)
    {
        x1 ^= x2;
        *buf++ = x2 * level;
        x2 += x1;
     }
}

// Write a float* buffer to a WAV file (mono), 16 bit PCM
// Using the sndfile lib. For the SndfileHandle implementation, see sndfile.hh

void writeBufToWavFile(string fileName, float* buf, int bufLength, int sampleRate) {
    int numChannels = 1; // mono
    SndfileHandle outFile = SndfileHandle(fileName.c_str(), SFM_WRITE, SF_FORMAT_WAV | SF_FORMAT_PCM_16, numChannels, sampleRate);
    outFile.write(buf, bufLength);
}

int main()
{
    int sampleRate = 44100;
    int durationSec = 3;
    int numFrames = sampleRate * durationSec;
    vector<float> outputBuf(numFrames);
    // Create a tone and write to WAV file
    createTone(outputBuf.data(), 440, numFrames, sampleRate);
    writeBufToWavFile("tone.wav", outputBuf.data(), outputBuf.size(), 44100);
    // Create white noise and write to WAV file
    createWhiteNoise(outputBuf.data(), numFrames);
    writeBufToWavFile("whitenoise.wav", outputBuf.data(), outputBuf.size(), 44100);
    	
	return 0;
}
