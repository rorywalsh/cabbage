#!/bin/bash  
echo "==========================================="
echo "======== Build Script for Cabbage ========="
echo "==========================================="
	
echo "Removing old binaries"
rm -rf ./build/Release/Cabbage.app
rm -rf ./build/Release/CabbagePlugin.vst

/Users/walshr/sourcecode/JUCE/extras/Projucer/Builds/MacOSX/build/Debug/Projucer.app/Contents/MacOS/Projucer --resave ../../CabbageIDE.jucer	

echo "Building Universal build"

xcodebuild -project Cabbage.xcodeproj clean
xcodebuild -project Cabbage.xcodeproj/ ARCHS="i386 x86_64" ONLY_ACTIVE_ARCH=NO -configuration Release

/Users/walshr/sourcecode/JUCE/extras/Projucer/Builds/MacOSX/build/Debug/Projucer.app/Contents/MacOS/Projucer --resave ../../CabbageLite.jucer

xcodebuild -project CabbageLite.xcodeproj clean
xcodebuild -project CabbageLite.xcodeproj/ ARCHS="i386 x86_64" ONLY_ACTIVE_ARCH=NO -configuration Release

/Users/walshr/sourcecode/JUCE/extras/Projucer/Builds/MacOSX/build/Debug/Projucer.app/Contents/MacOS/Projucer --resave ../../CabbagePluginSynth.jucer

xcodebuild -project CabbagePlugin.xcodeproj/ ARCHS="i386 x86_64" ONLY_ACTIVE_ARCH=NO -configuration Release 

cp -rf ./build/Release/CabbagePlugin.vst/ ./build/Release/Cabbage.app/Contents/CabbagePluginSynth.vst
cp -rf ./build/Release/CabbagePlugin.vst/ ./build/Release/CabbageLite.app/Contents/CabbagePluginSynth.vst
cp -rf ./build/Release/CabbagePlugin.component/ ./build/Release/Cabbage.app/Contents/CabbagePluginSynth.component
cp -rf ./build/Release/CabbagePlugin.component/ ./build/Release/CabbageLite.app/Contents/CabbagePluginSynth.component

/Users/walshr/sourcecode/JUCE/extras/Projucer/Builds/MacOSX/build/Debug/Projucer.app/Contents/MacOS/Projucer --resave ../../CabbagePlugin.jucer

xcodebuild -project CabbagePlugin.xcodeproj/ -configuration Release ARCHS="i386 x86_64" ONLY_ACTIVE_ARCH=NO 
cp -rf ./build/Release/CabbagePlugin.vst/ ./build/Release/Cabbage.app/Contents/CabbagePluginEffect.vst
cp -rf ./build/Release/CabbagePlugin.vst/ ./build/Release/CabbageLite.app/Contents/CabbagePluginEffect.vst
cp -rf ./build/Release/CabbagePlugin.component/ ./build/Release/Cabbage.app/Contents/CabbagePluginEffect.component
cp -rf ./build/Release/CabbagePlugin.component/ ./build/Release/CabbageLite.app/Contents/CabbagePluginEffect.component

rm -rf ./build/Release/CabbagePluginEffect.vst
rm -rf ./build/Release/CabbagePluginEffect.component
rm -rf ~/Library/Audio/Plug-Ins/VST/CabbagePlugin.vst
rm -rf ~/Library/Audio/Plug-Ins/VST/CabbagePlugin.component
cp -rf ../../Examples ./build/Release/Cabbage.app/Contents/Examples
cp -rf ../../Examples ./build/Release/CabbageLite.app/Contents/Examples

# cp -rf  /Users/walshr/sourcecode/FMOD\ Programmers\ API/api/lowlevel/examples/xcode32/_builds/Debug/fmod_csound.dylib ./build/Release/Cabbage.app/Contents/fmod_csound.dylib

# echo "Bundling all files"	
# cp -rf ../../Docs/_book ./build/Release/Cabbage.app/Contents/MacOS/Docs
# cp -rf ../../Docs/_book ./build/Release/CabbageStudio.app/Contents/MacOS/Docs


cp ../opcodes.txt ./build/Release/Cabbage.app/Contents/MacOS/opcodes.txt 

/Users/walshr/sourcecode/JUCE/extras/Projucer/Builds/MacOSX/build/Debug/Projucer.app/Contents/MacOS/Projucer --resave ../../CabbageIDE.jucer