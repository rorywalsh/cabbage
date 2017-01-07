Build Cabbage under Linux
-------------------------

Requirements
============

The following are dependencies you may need, you can simply copy and past the following as needed.

sudo apt-get install libfreetype6-dev
sudo apt-get install libx11-dev
sudo apt-get install libxinerama-dev
sudo apt-get install libxcursor-dev
sudo apt-get install mesa-common-dev
sudo apt-get install libasound2-dev
sudo apt-get install freeglut3-dev
sudo apt-get install libxcomposite-dev
sudo apt-get install csound
sudo apt-get install libcsound64-dev
sudo apt-get install libcsnd-dev
sudo apt-get install libsndfile1
sudo apt-get install libsndfile1-dev
sudo apt-get install libjack-dev
sudo apt-get install libxrandr-dev
sudo apt-get install msttcorefonts

Please make sure that the version of Csound you use is 6.07 or newer, otherwise you will get build problems. 
Cabbage needs the VST SDK in order to build its plugins libraries. The VST SDK can be downloed from [here](https://www.steinberg.net/en/company/developers.html)

Compiling Cabbage
=================

Make sure that the VST SDK resides in a folder called SDKs in your home directory, i.e, ~/SDKs, and Csound is installed in its default location. 

To build Cabbage, run this command from the current directory:

./bulidCabbage

If you really need to use custom paths for Csound and the VST SDKs, you will need to:
  - Download the latest version of JUCE
  - Build the Projucer
  - Open the CabbageIDE.jucer and CabbagePlugin.jucer files, modify the paths accordingly, save and exit.
  - Run the configureMakefiles.py Python script from this directory. 
  - Run the ./buildCabage script. 


Documentation
=============
You will need to install gitbook to build the Docs. It can be installed from NPM using:

npm install gitbook-cli -g

Once installed, cd to the Docs/ folder and run the following commands:

gitbook init
gitbook build

The manual will be built in folder called _book. The buildCabbage script(see below) will look for this folder when it builds and installs the full package. 


