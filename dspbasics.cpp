// DSPbasics.cpp : Defines the entry point for the application.
//

#define _USE_MATH_DEFINES
#include <cmath>
#include <vector>
#include <iostream>
#include "dspbasics.h"
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

// Create a sine tone and write to buffer

void createTone(float* buf, int frequency, int numFrames, int sampleRate)
{
    for (int i = 0; i < numFrames; i++)
    {
        float sample = 0.0;
        // Generate a sample value
        // Samples are normalized [-1.0, 1.0]
        // Multiplication with 0.30 descreases the amplitude      
        sample += 0.30 * sin(2 * M_PI * i * frequency / sampleRate);
        // Here we can add som harmonics if we want
        //sample += 0.25 * sin(2 * M_PI * i * 2 * frequency / sampleRate);
        //sample += 0.20 * sin(2 * M_PI * i * 3 * frequency / sampleRate);
        //sample += 0.15 * sin(2 * M_PI * i * 4 * frequency / sampleRate);
        //sample += 0.10 * sin(2 * M_PI * i * 5 * frequency / sampleRate);
         
        // Write sample to the buffer
        *buf++ = sample; 
    }
}


int main()
{
    int sampleRate = 44100;
    int durationSec = 3;
    int numFrames = sampleRate * durationSec;
    vector<float> outputBuf(numFrames);
    createTone(outputBuf.data(), 440, numFrames, sampleRate);
    // SndfileHandle implementation see sndfile.hh
    int numChannels = 1; // mono
    SndfileHandle outFile = SndfileHandle("outfile.wav", SFM_WRITE, SF_FORMAT_WAV | SF_FORMAT_PCM_16, numChannels, sampleRate);
    // Write buffer to file. SndfileHandle.write() wants a float* 
    outFile.write(outputBuf.data(), numFrames);
    	
	return 0;
}
