Build Cabbage under OSX
-------------------------

Requirements
============

You will need to install Csound. The most current version is available [here](http://csound.com/download.html)

Please make sure that the version of Csound you use is 6.07 or newer, otherwise you will get build problems. 

Cabbage needs the VST SDK in order to build its plugins libraries. The VST SDK can be downloaded from [here](https://www.steinberg.net/en/company/developers.html)
It's best to extract the VST SDK to ~/SDKs

You will also need to download the latest version of JUCE (master branch, 5.2). It's best to download and extract JUCE so that the JUCE folder has the same root as the main Cabbage dir, i.e, 

- some folder:
    -JUCE
    -cabbage

Build the Projucer using Xcode; there are project files included with JUCE. 

Building
=========
The easiest way to build Cabbage is to simply run the buildCabbage.sh file that resides in the Cabbage Builds/MacOSX/ directory. It will run the Projucer and generate Xcode files for each of the Cabbage projects. It will then build and compile those projects using the command line interface. 

If you wish to build and compile a project yourself, or wish to modify or edit some aspect of Cabbage, launch the Projucer, open the relevant *.jucer file for the Cabbage project you wish to build, i.e, and hit save. This will auto-generate Xcode files for that particular project. Open Xcode and open the recently created xcode project, it will be found in Cabbage/Builds/MacOSX folder. 
