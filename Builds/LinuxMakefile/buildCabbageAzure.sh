#!/bin/bash  
echo "==========================================="
echo "======== Build Script for Cabbage ========="
echo "==========================================="

echo "In order to build Cabbage you will need to have Csound installed, and"
echo "the Projucer from version 5.0 of JUCE built and installed."

echo "This scripts assumes that Csound is installed in the default location, i.e, includes files" 
echo "should be located in /user/local/include/csound while the Csound library itself should be"
echo "located in /user/local/lib "
echo "It is also assumes that the VST SDK is located in ~/SDKs/"
echo ""

if [[ $1 == "debug" ]]; then
  echo "Hello debug"
  xvfb-run ../../../JUCE/extras/Projucer/Builds/LinuxMakefile/build/Projucer --resave ../../CabbageIDE.jucer
  mv Makefile MakeCabbageIDE
  make -f MakeCabbageIDE clean
  make -f MakeCabbageIDE -j6
  cp ./build/Cabbage /usr/bin/Cabbage
  mv Makefile MakePluginEffect
  make -f MakePluginEffect clean 
  make -f MakePluginEffect -j6

  xvfb-run ../../../JUCE/extras/Projucer/Builds/LinuxMakefile/build/Projucer --resave ../../CabbagePluginSynth.jucer
  mv Makefile MakePluginSynth
  make -f MakePluginSynth clean
  make -f MakePluginSynth -j6 
else
  #release mode  default

  mkdir -p ./install/bin ./install/images ./install/desktop

  xvfb-run ../../../JUCE/extras/Projucer/Builds/LinuxMakefile/build/Projucer --resave ../../CabbageIDE.jucer
  mv Makefile MakeCabbageIDE
  echo "$(tput bold)Building CabbageIDE…$(tput sgr0)"

  make -f MakeCabbageIDE clean CONFIG=Release
  make -f MakeCabbageIDE -j6 CONFIG=Release
  cp ./build/Cabbage ./install/bin/Cabbage

  xvfb-run ../../../JUCE/extras/Projucer/Builds/LinuxMakefile/build/Projucer --resave ../../CabbagePluginNoStandalone.jucer
  mv Makefile MakePluginEffect
  echo "$(tput bold)Building PluginEffect…$(tput sgr0)"

  make -f MakePluginEffect clean CONFIG=Release
  make -f MakePluginEffect -j6 CONFIG=Release
  cp ./build/CabbagePlugin.so ./install/bin/CabbagePluginEffect.so

  xvfb-run ../../../JUCE/extras/Projucer/Builds/LinuxMakefile/build/Projucer --resave ../../CabbagePluginSynth.jucer
  mv Makefile MakePluginSynth
  echo "$(tput bold)Building PluginSynth…$(tput sgr0)"

  make -f MakePluginSynth clean CONFIG=Release
  make -f MakePluginSynth -j6 CONFIG=Release
  cp ./build/CabbagePlugin.so ./install/bin/CabbagePluginSynth.so

  xvfb-run ../../../JUCE/extras/Projucer/Builds/LinuxMakefile/build/Projucer --resave ../../CabbageLite.jucer
  mv Makefile MakeCabbageLite
  echo "$(tput bold)Building CabbageLite…$(tput sgr0)"

  make -f MakeCabbageLite clean CONFIG=Release
  make -f MakeCabbageLite -j6 CONFIG=Release
  cp ./build/CabbageLite ./install/bin/CabbageLite
fi  

echo "$(tput bold)Copying over docs and icons…$(tput sgr0)"
cp ./../opcodes.txt ./install/bin/opcodes.txt
cp ./../../Images/cabbage.png ./install/images/cabbage.png
cp ./../../Images/cabbage.png ./install/images/cabbagelite.png
cp -rf ../../Examples ./install/
cp -rf ../../Themes ./install/

g++ ../../Source/testCsoundFile.cpp -o testCsoundFile -I"/usr/local/include/csound" -I"/usr/include/csound" -lcsound64
# cp testCsoundFile ./install/bin/testCsoundFile
#cp -rf ../../Docs/_book CabbageBuild/Docs

sed "s@CURRENTDIR@$(pwd)@" Cabbage.desktop > ./install/desktop/cabbage.desktop
sed "s@CURRENTDIR@$(pwd)@" Cabbage.desktop > ./install/desktop/cabbageLite.desktop
# sed "s@CURRENTDIR@$(pwd)@" dummy.desktop > CabbageBuild/cabbagelite.desktop
