#!/bin/bash  
echo "==========================================="
echo "======== Build Script for Cabbage ========="
echo "==========================================="
	
export PROJUCER=/Users/walshr/sourcecode/JUCE/extras/Projucer/Builds/MacOSX/build/Debug/Projucer.app/Contents/MacOS/Projucer

$PROJUCER --resave ../../CabbageIDE.jucer	
git apply ../../patches/*.patch
echo "Building Universal build"

xcodebuild -project Cabbage.xcodeproj clean
xcodebuild -project Cabbage.xcodeproj/ ARCHS="x86_64" ONLY_ACTIVE_ARCH=YES -configuration Release
cp -rf ../../Themes ./build/Release/Cabbage.app/Contents/Themes

$PROJUCER --resave ../../CabbageLite.jucer
git apply ../../patches/*.patch

xcodebuild -project CabbageLite.xcodeproj clean
xcodebuild -project CabbageLite.xcodeproj/ ARCHS="x86_64" ONLY_ACTIVE_ARCH=YES -configuration Release

$PROJUCER --resave ../../CabbagePluginSynth.jucer
git apply ../../patches/*.patch

xcodebuild -project CabbagePlugin.xcodeproj clean
xcodebuild -project CabbagePlugin.xcodeproj/ ARCHS="x86_64" ONLY_ACTIVE_ARCH=YES -configuration Release GCC_PREPROCESSOR_DEFINITIONS="Cabbage_Plugin_Synth=1 USE_DOUBLE=1 CSOUND6=1 MACOSX=1"
cp -rf ./build/Release/CabbagePlugin.vst/ ./build/Release/Cabbage.app/Contents/CabbagePluginSynth.vst
cp -rf ./build/Release/CabbagePlugin.vst/ ./build/Release/CabbageLite.app/Contents/CabbagePluginSynth.vst
cp -rf ./build/Release/CabbagePlugin.component/ ./build/Release/Cabbage.app/Contents/CabbagePluginSynth.component
cp -rf ./build/Release/CabbagePlugin.component/ ./build/Release/CabbageLite.app/Contents/CabbagePluginSynth.component

$PROJUCER --resave ../../CabbagePluginMIDIEffect.jucer
git apply ../../patches/*.patch

xcodebuild -project CabbagePlugin.xcodeproj clean
xcodebuild -project CabbagePlugin.xcodeproj/ ARCHS="i386 x86_64" ONLY_ACTIVE_ARCH=YES -configuration Release GCC_PREPROCESSOR_DEFINITIONS="Cabbage_Plugin_Synth=1 Cabbage_MIDI_Effect=1 USE_DOUBLE=1 CSOUND6=1 MACOSX=1"
cp -rf ./build/Release/CabbagePlugin.component/ ./build/Release/Cabbage.app/Contents/CabbagePluginMIDIEffect.component
cp -rf ./build/Release/CabbagePlugin.component/ ./build/Release/CabbageLite.app/Contents/CabbagePluginMIDIEffect.component

$PROJUCER --resave ../../CabbagePlugin.jucer
git apply ../../patches/*.patch

xcodebuild -project CabbagePlugin.xcodeproj clean
xcodebuild -project CabbagePlugin.xcodeproj/ -configuration Release ARCHS="x86_64" ONLY_ACTIVE_ARCH=YES 
cp -rf ./build/Release/CabbagePlugin.vst/ ./build/Release/Cabbage.app/Contents/CabbagePluginEffect.vst
cp -rf ./build/Release/CabbagePlugin.app/ ./build/Release/Cabbage.app/Contents/CabbagePlugin.app
cp -rf ./build/Release/CabbagePlugin.vst/ ./build/Release/CabbageLite.app/Contents/CabbagePluginEffect.vst
cp -rf ./build/Release/CabbagePlugin.component/ ./build/Release/Cabbage.app/Contents/CabbagePluginEffect.component
cp -rf ./build/Release/CabbagePlugin.component/ ./build/Release/CabbageLite.app/Contents/CabbagePluginEffect.component

rm -rf ./build/Release/CabbagePluginEffect.vst
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
