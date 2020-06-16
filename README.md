# DSPbasics

Doing some simple Digital Signal Processing (DSP) stuff for learning purposes.

Such as:

* Generating a tone (sine wave)
* Generating white-ish noise
* Bit crushing using sample reduction and bit reduction

It builds on both Windows (Visual Studio) and Linux (at least on Ubuntu 18.04). I'm providing a cmake file so it is recommended to use cmake to build.

It uses sndfile to read/write WAV files. Install sndfile:

sudo apt-get install libsndfile-dev

or

Windows installer from  http://www.mega-nerd.com/libsndfile/#Download

Build using cmake:

> cmake -B build
> cmake --build build

If you need to specify the location of the 
> cmake -B build -DSNDFILE_DIR=<sndfilelocation>



