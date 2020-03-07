# DSPbasics

Doing some simple Digital Signal Processing (DSP) stuff to learn.

Such as:

* Generating a tone (sine wave)
* Generating white-ish noise
* Bit crushing using sample reduction and bit reduction

It builds on both Windows (Visual Studio) and Linux (at least on Ubuntu 18.04). I'm providing a cmake file so it is recommended to use cmake to build.

It uses sndfile to read/write WAV files. Install sndfile:

sudo apt-get install libsndfile-dev

or

Windows installer from  http://www.mega-nerd.com/libsndfile/#Download

Look in CMakeLists.txt and maybe change paths to sndfile lib.


