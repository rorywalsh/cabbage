Build Cabbage under Linux
-------------------------
Note that if you are using Arch Linux you can install Cabbage using the package manager ```pacaur -S cabbage``` For Debian based distros you can follow the instructions below.  

Requirements
============

- You will also need to download version 5.2.0 of JUCE, either clone the git repo or grab a zip archive. Once you have the source you will need to build the Projucer. Makefiles can be found in JUCE/extras/Projucer/Builds/LinuxMakefile. 

- Please make sure you have a fairly recent version of Csound installed, otherwise you may get build problems. 

- Cabbage needs the VST SDK in order to build its plugins libraries. The VST SDK can be downloed from [here](https://www.steinberg.net/en/company/developers.html)

- The following are dependencies you **may** need. It's probably best to skip this step for now and try building Cabbage. If you have any unmet dependencies you can simply ```sudo apt-get install``` any of the packages you may need from the following list. Your compiler output should specify what's missing. 

libfreetype6-dev 
libx11-dev 
libxinerama-dev 
libxcursor-dev 
mesa-common-dev l
ibasound2-dev 
freeglut3-dev 
libxcomposite-dev 
libcsound64-dev 
libcsnd-dev 
libsndfile1 
libsndfile1-dev 
libjack-dev 
libxrandr-dev 
msttcorefonts 

- if you experience build issues relating to webkit, try installing the latest version using the commands below. 

sudo add-apt-repository ppa:webkit-team/ppa
sudo apt-get update
sudo apt-get install libwebkit2gtk-4.0-37 libwebkit2gtk-4.0-dev


Compiling Cabbage
=================

In order to simplfy the build process, it's best that the JUCE folder resides in the same parent directory as the Cabbage folder. Make sure that the VST SDK resides in a folder called SDKs in your home directory, i.e, ~/SDKs, and Csound is installed in its default location. 

If any of these packages and programs are not in the directories outlined above, you will need to modify the buildCabbage.sh script so the compiler can find the correct locations. 

To build Cabbage run the following command from the cabbage/Builds/LinuxMakefile 

./buildCabbage

If all goes well this should build Cabbage, and install it on your system. If you have an issues building please post to the Cabbage [forum](forum.cabbageaudio.com)

