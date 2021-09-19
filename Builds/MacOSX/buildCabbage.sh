#!/bin/bash  
echo "==========================================="
echo "======== Build Script for Cabbage ========="
echo "==========================================="
	
export PROJUCER=../../../JUCE/extras/Projucer/Builds/MacOSX/build/Debug/Projucer.app/Contents/MacOS/Projucer

apply_patches () {
    cd ../../
    git apply ./patches/StandaloneWrapper.patch
    git apply ./patches/AUWrapper.patch
    git apply ./patches/UtilityWrapper.patch
    git apply ./patches/VST2Wrapper.patch
    git apply ./patches/VST3Wrapper.patch
    cd Builds/MacOSX/
}

$PROJUCER --resave ../../CabbageIDE.jucer	
apply_patches

xcodebuild -project Cabbage.xcodeproj clean 
xcodebuild -project Cabbage.xcodeproj -configuration Release
cp -rf ../../Themes ./build/Release/Cabbage.app/Contents/Themes

$PROJUCER --resave ../../CabbagePluginSynth.jucer
apply_patches

xcodebuild -project CabbagePlugin.xcodeproj -configuration Release GCC_PREPROCESSOR_DEFINITIONS="Cabbage_Plugin_Synth=1 USE_DOUBLE=1 CSOUND6=1 MACOSX=1"
cp -rf ./build/Release/CabbagePlugin.vst ./build/Release/Cabbage.app/Contents/CabbagePluginSynth.vst
cp -rf ./build/Release/CabbagePlugin.vst ./build/Release/CabbageLite.app/Contents/CabbagePluginSynth.vst
cp -rf ./build/Release/CabbagePlugin.vst3 ./build/Release/Cabbage.app/Contents/CabbagePluginSynth.vst3
cp -rf ./build/Release/CabbagePlugin.vst3 ./build/Release/CabbageLite.app/Contents/CabbagePluginSynth.vst3
cp -rf ./build/Release/CabbagePlugin.component/ ./build/Release/Cabbage.app/Contents/CabbagePluginSynth.component
cp -rf ./build/Release/CabbagePlugin.component/ ./build/Release/CabbageLite.app/Contents/CabbagePluginSynth.component

$PROJUCER --resave ../../CabbagePluginMIDIEffect.jucer
apply_patches

xcodebuild -project CabbagePlugin.xcodeproj -configuration Release GCC_PREPROCESSOR_DEFINITIONS="Cabbage_Plugin_Synth=1 Cabbage_MIDI_Effect=1 USE_DOUBLE=1 CSOUND6=1 MACOSX=1"
cp -rf ./build/Release/CabbagePlugin.component/ ./build/Release/Cabbage.app/Contents/CabbagePluginMIDIEffect.component
cp -rf ./build/Release/CabbagePlugin.component/ ./build/Release/CabbageLite.app/Contents/CabbagePluginMIDIEffect.component

$PROJUCER --resave ../../CabbagePlugin.jucer
apply_patches

xcodebuild -project CabbagePlugin.xcodeproj/  -configuration Release 
cp -rf ./build/Release/CabbagePlugin.vst/ ./build/Release/Cabbage.app/Contents/CabbagePluginEffect.vst
cp -rf ./build/Release/CabbagePlugin.vst/ ./build/Release/CabbageLite.app/Contents/CabbagePluginEffect.vst
cp -rf ./build/Release/CabbagePlugin.vst3/ ./build/Release/Cabbage.app/Contents/CabbagePluginEffect.vst3
cp -rf ./build/Release/CabbagePlugin.vst3/ ./build/Release/CabbageLite.app/Contents/CabbagePluginEffect.vst3
cp -rf ./build/Release/CabbagePlugin.app/ ./build/Release/Cabbage.app/Contents/CabbagePlugin.app
cp -rf ./build/Release/CabbagePlugin.component/ ./build/Release/Cabbage.app/Contents/CabbagePluginEffect.component
cp -rf ./build/Release/CabbagePlugin.component/ ./build/Release/CabbageLite.app/Contents/CabbagePluginEffect.component

rm -rf ./build/Release/CabbagePluginEffect.vst
rm -rf ./build/Release/CabbagePluginEffect.vst3
rm -rf ./build/Release/CabbagePluginEffect.component
rm -rf ~/Library/Audio/Plug-Ins/VST/CabbagePlugin.vst
rm -rf ~/Library/Audio/Plug-Ins/VST/CabbagePlugin.component
cp -rf ../../Examples ./build/Release/Cabbage.app/Contents/Examples
cp -rf ../../Examples ./build/Release/CabbageLite.app/Contents/Examples

cp ../opcodes.txt ./build/Release/Cabbage.app/Contents/MacOS/opcodes.txt 


xcodebuild -project ../../CsoundTestXcode/CsoundTest.xcodeproj clean
xcodebuild -project ../../CsoundTestXcode/CsoundTest.xcodeproj -configuration Release

cp ../../CsoundTestXcode/build/CsoundTest/Release/CsoundTest ./build/Release/Cabbage.app/Contents/MacOS/CsoundTest 

cp -rf CsoundLib64.framework ./build/Release/Cabbage.app/Contents/CsoundLib64.framework

VERSION="CabbageOSXInstaller"-$($PROJUCER --get-version ../../CabbageIDE.jucer)".pkg"
zip $VERSION ./build/Release/Cabbage.app
