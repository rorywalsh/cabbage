curl -L -o Csound6.12.1-MacOS_universal.dmg 'https://github.com/csound/csound/releases/download/6.12.2/Csound6.12.1-MacOS_universal.dmg'
ls
hdiutil attach Csound6.12.1-MacOS_universal.dmg
cp -R /Volumes/Csound6.12.1/ Csound
hdiutil detach /Volumes/Csound6.12.1/
cd Csound
sudo installer -pkg csound6.12.1-OSX-universal.pkg -target /
sudo install_name_tool -id /Library/Frameworks/CsoundLib64.framework/CsoundLib64  /Library/Frameworks/CsoundLib64.framework/CsoundLib64

cd /Users/vsts/agent/2.148.0/work/1
git clone https://github.com/WeAreROLI/JUCE.git
touch JUCE/
cd /Users/vsts/agent/2.148.0/work/1/JUCE
git checkout tags/5.4.3
cd /Users/vsts/agent/2.148.0/work/1/JUCE/extras/Projucer/JuceLibraryCode
sed -i '' "s/#define JUCER_ENABLE_GPL_MODE 1/#define JUCER_ENABLE_GPL_MODE 1/" AppConfig.h
sed -i '' "s/#define JUCE_USE_DARK_SPLASH_SCREEN 1/#define JUCE_USE_DARK_SPLASH_SCREEN 0/" AppConfig.h
cat AppConfig.h
cd /Users/vsts/agent/2.148.0/work/1/JUCE/extras/Projucer/Builds/MacOSX/
xcodebuild -project Projucer.xcodeproj


echo "====================\n=======\n======\n============\n===========\n==========="
pwd
cd ~
pwd
ls
curl -L -o vstsdk3611_22_10_2018_build_34.zip.zip https://download.steinberg.net/sdk_downloads/vstsdk3611_22_10_2018_build_34.zip
unzip -q vstsdk3611_22_10_2018_build_34.zip
ls

mkdir ~/SDKs
cp -rf VST_SDK ~/SDKs
curl -L -o heads.zip http://cabbageaudio.com/beta/heads.zip
unzip -q heads.zip
cp -rf vst2.x ~/SDKs/VST_SDK/VST3_SDK/pluginterfaces
ls ~/SDKs/VST_SDK/VST3_SDK/pluginterfaces
echo "\n"
ls /Users/vsts/agent/2.148.0/work/1/
# cd /Users/vsts/agent/2.148.0/work/1/s/Builds/MacOSX

curl -L -o Packages.dmg 'http://s.sudre.free.fr/Software/files/Packages.dmg'
hdiutil mount Packages.dmg
sudo installer -pkg /Volumes/Packages\ 1.2.5/Install\ Packages.pkg -target /
hdiutil detach /Volumes/Packages\ 1.2.5/

cd /Users/vsts/agent/2.148.0/work/1/s/Builds/MacOSX
export PROJUCER=/Users/vsts/agent/2.148.0/work/1/JUCE/extras/Projucer/Builds/MacOSX/build/Debug/Projucer.app/Contents/MacOS/Projucer

# $PROJUCER --resave ../../CabbageIDE.jucer
# xcodebuild -project Cabbage.xcodeproj/ clean
# xcodebuild -project Cabbage.xcodeproj/ ARCHS="i386 x86_64" ONLY_ACTIVE_ARCH=NO -configuration Release 
# $PROJUCER --resave ../../CabbageLite.jucer
# xcodebuild -project CabbageLite.xcodeproj/ ARCHS="i386 x86_64" ONLY_ACTIVE_ARCH=NO -configuration Release

# $PROJUCER --resave ../../CabbagePluginMIDIEffect.jucer
# xcodebuild -project CabbagePlugin.xcodeproj clean
# xcodebuild -project CabbagePlugin.xcodeproj/ ARCHS="i386 x86_64" ONLY_ACTIVE_ARCH=NO -configuration Release GCC_PREPROCESSOR_DEFINITIONS="Cabbage_Plugin_Synth=1 Cabbage_MIDI_Effect=1 USE_DOUBLE=1 CSOUND6=1 MACOSX=1" 
# cp -rf ./build/Release/CabbagePlugin.component/ ./build/Release/Cabbage.app/Contents/CabbagePluginMIDIEffect.component
# cp -rf ./build/Release/CabbagePlugin.component/ ./build/Release/CabbageLite.app/Contents/CabbagePluginMIDIEffect.component
# $PROJUCER --resave ../../CabbagePluginSynth.jucer
# xcodebuild -project CabbagePlugin.xcodeproj clean
# xcodebuild -project CabbagePlugin.xcodeproj/ ARCHS="i386 x86_64" ONLY_ACTIVE_ARCH=NO -configuration Release GCC_PREPROCESSOR_DEFINITIONS="Cabbage_Plugin_Synth=1 USE_DOUBLE=1 CSOUND6=1 MACOSX=1"
# cp -rf ./build/Release/CabbagePlugin.vst/ ./build/Release/Cabbage.app/Contents/CabbagePluginSynth.vst
# cp -rf ./build/Release/CabbagePlugin.vst/ ./build/Release/CabbageLite.app/Contents/CabbagePluginSynth.vst
# cp -rf ./build/Release/CabbagePlugin.component/ ./build/Release/Cabbage.app/Contents/CabbagePluginSynth.component
# cp -rf ./build/Release/CabbagePlugin.component/ ./build/Release/CabbageLite.app/Contents/CabbagePluginSynth.component

$PROJUCER --resave ../../CabbagePlugin.jucer 
xcodebuild -project CabbagePlugin.xcodeproj clean
xcodebuild -project CabbagePlugin.xcodeproj/ -configuration Release ARCHS="i386 x86_64" ONLY_ACTIVE_ARCH=NO GCC_PREPROCESSOR_DEFINITIONS="MACOSX=1 USE_DOUBLE=1" 
# cp -rf ./build/Release/CabbagePlugin.vst/ ./build/Release/Cabbage.app/Contents/CabbagePluginEffect.vst
# cp -rf ./build/Release/CabbagePlugin.vst/ ./build/Release/CabbageLite.app/Contents/CabbagePluginEffect.vst
# cp -rf ./build/Release/CabbagePlugin.component/ ./build/Release/Cabbage.app/Contents/CabbagePluginEffect.component
# cp -rf ./build/Release/CabbagePlugin.component/ ./build/Release/CabbageLite.app/Contents/CabbagePluginEffect.component
# cp -rf ../../Examples ./build/Release/Cabbage.app/Contents/Examples
# cp -rf ../../Examples ./build/Release/CabbageLite.app/Contents/Examples

# curl -L -o CabbageManual.zip 'http://cabbageaudio.com/beta/CabbageManual.zip'
# ls
# unzip -q "CabbageManual.zip"
# ls
# cp -rf CabbageManual ././build/Release/Cabbage.app/Contents/CabbageManual
# cp -rf CabbageManual ././build/Release/CabbageLite.app/Contents/CabbageManual

# cd /Users/vsts/agent/2.148.0/work/1/s/Builds/MacOSX/build/Release/
# curl -L -o CabbageRack-0.5.0-mac.zip https://github.com/rorywalsh/CabbageRack/blob/master/dist/CabbageRack-0.5.0-mac.zip?raw=true
# unzip -q CabbageRack-0.5.0-mac.zip
# ls
# cp -R CabbageRack Cabbage.app/Contents/CabbageRack
# ls Cabbage.app/Contents/
# curl -L -o fmod_csound.dylib 'https://github.com/rorywalsh/csoundfmod/releases/download/v1.1/fmod_csound.dylib'
# ls
# cp fmod_csound.dylib Cabbage.app/Contents/fmod_csound.dylib
# ls Cabbage.app/Contents/
# zip -r CabbageOSX.zip Cabbage.app CabbageLite.app 

# cd /Users/vsts/agent/2.148.0/work/1/s/Builds/MacOSX/

# packagesbuild InstallerTravis.pkgproj
# ls build
