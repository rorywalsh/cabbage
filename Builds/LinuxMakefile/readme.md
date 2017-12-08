Build Cabbage under Linux
-------------------------

Requirements
============

The following are dependencies you may need, you can simply copy and past the following as needed.

sudo apt-get install libfreetype6-dev libx11-dev libxinerama-dev libxcursor-dev mesa-common-dev libasound2-dev freeglut3-dev libxcomposite-dev csound libcsound64-dev libcsnd-dev libsndfile1 libsndfile1-dev libjack-dev libxrandr-dev msttcorefonts 
sudo add-apt-repository ppa:webkit-team/ppa
sudo apt-get update
sudo apt-get install libwebkit2gtk-4.0-37 libwebkit2gtk-4.0-dev

Please make sure that the version of Csound you use is 6.07 or newer, otherwise you will get build problems. 
Cabbage needs the VST SDK in order to build its plugins libraries. The VST SDK can be downloed from [here](https://www.steinberg.net/en/company/developers.html)

You will also need to download the latest version of JUCE (master branch, 5.2)
- Build the Projucer

Compiling Cabbage
=================

Make sure that the VST SDK resides in a folder called SDKs in your home directory, i.e, ~/SDKs, and Csound is installed in its default location. You may need to edit the buildCabbage.sh script so that it points to the location of your Projucer binary. When these paths are set up run run this command from the LinuxMakefiles directory:

./buildCabbage


