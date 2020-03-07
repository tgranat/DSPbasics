// DSPbasics.cpp : Defines the entry point for the application.
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

// Create pseudorandom white-ish noise 
// Algorithm from https://www.musicdsp.org/en/latest/Synthesis/216-fast-whitenoise-generator.html
// buf - array to write samples to
// numFrames - number of samples, also minimum size of buffer

void createNoise(float* buf, int numFrames, float level = 0.30f)
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

// Effects
// =======
// Bitcrusher

// Bit crush using Sample rate reduction. Reduces the sample rate (but keeps the number of samples in the buffer)
// If this implementaion is used in for example a plugin, where the buffer is small due to low latency requirements,
// then the sample rate reduction should be set to factors of the buffer size. For example reduction 8 or 16 if buffer size is 32.
// This is due to the simplified handling of how odd frames are filled out in this implementation. 
// If the buffer is large (read from a file like here) it doesn't really matter.

void bitCrusherSamplerate(float* inbuf, float* outbuf, int numFrames, int sampleRate, int reduction) {
    // Sample rate reduction
    // Read a number of samples (for example 2 if sample rate reduced to half, or 10 if reduced to a 1/10 of original rate). 
    // Calculate average of samples
    // Write the average values to output buf for the next 2 (or 10) samples. We need to keep original sample rate
 
    int outframes = 0;
    float sample = 0.f;
    // If inbuffer is smaller than reduction we will set sample which will
    // be used filling up remaining out buffer at the end.
    // This is an unusual case, perhaps in a DAW with buf size set to very low.
    if (numFrames < reduction) {
        sample = 0.f;
        for (int x = 0; x < numFrames; x++) {
            sample += inbuf[x];
        }
        sample = sample / reduction;
    }

    // Read samples forward to calculate average. Write average sample to outbuf.
    for (int i = 0; i < numFrames - reduction+1 ; i++) {
        sample = 0.f;
        for (int x = i; x < i + reduction; x++) {
            sample += inbuf[x];
        }
        sample = sample / reduction;
        for (int x = 0; x < reduction - 1; x++) {
            i++;
            outbuf[outframes++] = sample;
        }
        outbuf[outframes++] = sample;
    }
 
    // if odd frames left, fill with last used sample until end
    while (outframes < numFrames) {
        //cout << "Fill last frames, outframes index = " << outframes << endl;
        outbuf[outframes++] = sample;
    }
}

// Bit crush using "Resolution reduction". Reduces the number of bits used for audio samples

void bitCrusherResolution(float* inbuf, float* outbuf, int numFrames, int sampleRate, int resolution) {
 
    // Resolution reduction
    // For example 16 bit is −32768 through 32767 (but converted to -1.0 to 1.0 float)
    //   Example to change to 8 bit (256):
    //   multiply float with 256. Divide resulting integer value with (float)256. The resoultion will only be 8 bits (-128 through 127).
 
    int bits = pow(2, resolution);
     
    for (int i = 0; i < numFrames; i++) {
        float sample = inbuf[i];
        int crushed = sample * bits;
        *outbuf++ = crushed/(float) bits;
    }
}


// Write a float* buffer to a WAV file (mono). Default 16 bit PCM and 44100 kHz
// Using the sndfile lib. For the SndfileHandle implementation, see sndfile.hh

void writeBufToWavFile(string fileName, float* buf, int bufLength, int format = SF_FORMAT_WAV | SF_FORMAT_PCM_16, int sampleRate = 44100) {
    int numChannels = 1; // mono
    SndfileHandle outFile = SndfileHandle(fileName.c_str(), SFM_WRITE, format, numChannels, sampleRate);
    outFile.write(buf, bufLength);
}


int main()
{
    // Initiating things used when createing "tone" and "white noise"
    int sampleRate = 44100;
    int durationSec = 3;
    int numFrames = sampleRate * durationSec;
    vector<float> outputBuf(numFrames);

    // Create a tone and write to WAV file
    createTone(outputBuf.data(), 440, numFrames, sampleRate);
    writeBufToWavFile("tone.wav", outputBuf.data(), outputBuf.size());

    // Create white noise and write to WAV file
    createNoise(outputBuf.data(), numFrames);

    writeBufToWavFile("whitenoise.wav", outputBuf.data(), outputBuf.size());

    // Running "bitcrusher" on sounds from file
    // Read buffer with test sounds from mono WAV file
    SndfileHandle infile = SndfileHandle("../testdata/single-note-clean-mono.wav"); 
    // printFileInfo(infile);
    sf_count_t frames = infile.frames();
    int format = infile.format();
    int rate = infile.samplerate();
    vector<float> inBuf(frames, 0);
    infile.read(&inBuf[0], frames);
    vector<float> outBuf(frames);

    // Bit resolution reduction

    int bitResolution = 2;
    string filename = "bitcrushed_" + to_string(bitResolution) + "_bits.wav";
    bitCrusherResolution(inBuf.data(), outBuf.data(), frames, rate, bitResolution);
    writeBufToWavFile(filename.c_str(), outBuf.data(), outBuf.size(), format, rate);

    // Sample rate reduction

    int sampleRateReduction = 16;
    filename = "bitcrushed_" + to_string(sampleRateReduction) + "_times_reduction.wav";
    bitCrusherSamplerate(inBuf.data(), outBuf.data(), frames, rate, sampleRateReduction);
    writeBufToWavFile(filename.c_str(), outBuf.data(), outBuf.size(), format, rate);
    	
	return 0;
}
