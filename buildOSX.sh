
echo "AGENT_WORKFOLDER is $AGENT_WORKFOLDER"
echo "AGENT_WORKFOLDER contents:"
ls -1 $AGENT_WORKFOLDER
echo "AGENT_BUILDDIRECTORY is $AGENT_BUILDDIRECTORY"
echo "AGENT_BUILDDIRECTORY contents:"
ls -1 $AGENT_BUILDDIRECTORY
echo "SYSTEM_DEFAULTWORKINGDIRECTORY is $SYSTEM_DEFAULTWORKINGDIRECTORY"
echo "SYSTEM_DEFAULTWORKINGDIRECTORY contents:"
ls -1 $SYSTEM_DEFAULTWORKINGDIRECTORY
echo "BUILD_ARTIFACTSTAGINGDIRECTORY is $BUILD_ARTIFACTSTAGINGDIRECTORY"
echo "BUILD_ARTIFACTSTAGINGDIRECTORY contents:"
ls -1 $BUILD_ARTIFACTSTAGINGDIRECTORY

  
pwd
curl -L -o csound6.15.0-MacOS_x86_64.dmg 'https://github.com/csound/csound/releases/download/6.15.0/csound-MacOS_x86_64-6.15.0.dmg'
ls
hdiutil attach csound6.15.0-MacOS_x86_64.dmg
cp -R /Volumes/Csound6.15.0/ Csound
hdiutil detach /Volumes/Csound6.15.0/
cd Csound
sudo installer -pkg csound-MacOS_x86_64-6.15.0.pkg -target /
sudo install_name_tool -id /Library/Frameworks/CsoundLib64.framework/CsoundLib64  /Library/Frameworks/CsoundLib64.framework/CsoundLib64

cd $AGENT_BUILDDIRECTORY
git clone https://github.com/juce-framework/JUCE.git
cd JUCE
git checkout 5.4.7
git branch
touch JUCE/
cd $AGENT_BUILDDIRECTORY/JUCE
cd $AGENT_BUILDDIRECTORY/JUCE/extras/Projucer/JuceLibraryCode
sed -i '' "s/#define JUCER_ENABLE_GPL_MODE 1/#define JUCER_ENABLE_GPL_MODE 1/" AppConfig.h
sed -i '' "s/#define JUCE_USE_DARK_SPLASH_SCREEN 1/#define JUCE_USE_DARK_SPLASH_SCREEN 0/" AppConfig.h
cat AppConfig.h
cd $AGENT_BUILDDIRECTORY/JUCE/extras/Projucer/Builds/MacOSX/
xcodebuild -project Projucer.xcodeproj

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

cd $AGENT_BUILDDIRECTORY

curl -L -o Packages.dmg 'http://s.sudre.free.fr/Software/files/Packages.dmg'
hdiutil mount Packages.dmg
sudo installer -pkg /Volumes/Packages\ 1.2.9/Install\ Packages.pkg -target /
hdiutil detach /Volumes/Packages\ 1.2.9/

cd $SYSTEM_DEFAULTWORKINGDIRECTORY/Builds/MacOSX
export PROJUCER=$AGENT_BUILDDIRECTORY/JUCE/extras/Projucer/Builds/MacOSX/build/Debug/Projucer.app/Contents/MacOS/Projucer

#building sinmple Csound CLI for testing files
$PROJUCER --resave ../../CabbageCsoundCLI.jucer
xcodebuild -project CabbageCsoundCLI.xcodeproj/ ARCHS="x86_64" ONLY_ACTIVE_ARCH=NO -configuration Release

$PROJUCER --resave ../../CabbageIDE.jucer
xcodebuild -project Cabbage.xcodeproj/ clean
xcodebuild -project Cabbage.xcodeproj/ ARCHS="x86_64" ONLY_ACTIVE_ARCH=NO -configuration Release 
cp -rf ../../Themes ./build/Release/Cabbage.app/Contents/Themes

$PROJUCER --resave ../../CabbageLite.jucer
xcodebuild -project CabbageLite.xcodeproj/ ARCHS="x86_64" ONLY_ACTIVE_ARCH=NO -configuration Release

$PROJUCER --resave ../../CabbagePluginMIDIEffect.jucer
xcodebuild -project CabbagePlugin.xcodeproj clean
xcodebuild -project CabbagePlugin.xcodeproj/ ARCHS="x86_64" ONLY_ACTIVE_ARCH=NO -configuration Release GCC_PREPROCESSOR_DEFINITIONS="Cabbage_Plugin_Synth=1 Cabbage_MIDI_Effect=1 USE_DOUBLE=1 CSOUND6=1 MACOSX=1" 
cp -rf ./build/Release/CabbagePlugin.component/ ./build/Release/Cabbage.app/Contents/CabbagePluginMIDIEffect.component
cp -rf ./build/Release/CabbagePlugin.component/ ./build/Release/CabbageLite.app/Contents/CabbagePluginMIDIEffect.component

$PROJUCER --resave ../../CabbagePluginSynth.jucer
xcodebuild -project CabbagePlugin.xcodeproj clean
xcodebuild -project CabbagePlugin.xcodeproj/ ARCHS="x86_64" ONLY_ACTIVE_ARCH=NO -configuration Release GCC_PREPROCESSOR_DEFINITIONS="Cabbage_Plugin_Synth=1 USE_DOUBLE=1 CSOUND6=1 MACOSX=1"
cp -rf ./build/Release/CabbagePlugin.vst/ ./build/Release/Cabbage.app/Contents/CabbagePluginSynth.vst
cp -rf ./build/Release/CabbagePlugin.vst/ ./build/Release/CabbageLite.app/Contents/CabbagePluginSynth.vst
cp -rf ./build/Release/CabbagePlugin.vst3/ ./build/Release/Cabbage.app/Contents/CabbagePluginSynth.vst3
cp -rf ./build/Release/CabbagePlugin.vst3/ ./build/Release/CabbageLite.app/Contents/CabbagePluginSynth.vst3
cp -rf ./build/Release/CabbagePlugin.component/ ./build/Release/Cabbage.app/Contents/CabbagePluginSynth.component
cp -rf ./build/Release/CabbagePlugin.component/ ./build/Release/CabbageLite.app/Contents/CabbagePluginSynth.component

$PROJUCER --resave ../../CabbagePlugin.jucer 
xcodebuild -project CabbagePlugin.xcodeproj clean
xcodebuild -project CabbagePlugin.xcodeproj/ -configuration Release ARCHS="x86_64" ONLY_ACTIVE_ARCH=NO GCC_PREPROCESSOR_DEFINITIONS="MACOSX=1 USE_DOUBLE=1" 
cp -rf ./build/Release/CabbagePlugin.vst/ ./build/Release/Cabbage.app/Contents/CabbagePluginEffect.vst
cp -rf ./build/Release/CabbagePlugin.app/ ./build/Release/Cabbage.app/Contents/CabbagePlugin.app
cp -rf ./build/Release/CabbagePlugin.vst/ ./build/Release/CabbageLite.app/Contents/CabbagePluginEffect.vst
cp -rf ./build/Release/CabbagePlugin.vst3/ ./build/Release/Cabbage.app/Contents/CabbagePluginEffect.vst3
cp -rf ./build/Release/CabbagePlugin.vst3/ ./build/Release/CabbageLite.app/Contents/CabbagePluginEffect.vst3
cp -rf ./build/Release/CabbagePlugin.component/ ./build/Release/Cabbage.app/Contents/CabbagePluginEffect.component
cp -rf ./build/Release/CabbagePlugin.component/ ./build/Release/CabbageLite.app/Contents/CabbagePluginEffect.component
cp -rf ../../Examples ./build/Release/Cabbage.app/Contents/Examples
cp -rf ../../Examples ./build/Release/CabbageLite.app/Contents/Examples

$PROJUCER --resave ../../CabbageCsoundCLI.jucer
xcodebuild -project CabbageCsoundCLI.xcodeproj clean
xcodebuild -project CabbageCsoundCLI.xcodeproj -configuration Release



# cp ../../CsoundTestXcode/Build/Release/CsoundTest ./build/Release/Cabbage.app/Contents/MacOS/CsoundTest 
cp -rf ./build/Release/CabbageCsoundCLI ./build/Release/Cabbage.app/Contents/MacOS/CabbageCsoundCLI 
curl -L -o CabbageManual.zip 'http://cabbageaudio.com/beta/CabbageManual.zip'
<<<<<<< HEAD
ls
unzip -q -o "CabbageManual.zip" 
ls
=======
unzip -q "CabbageManual.zip"
>>>>>>> 1e820444e917c7643cf8771faf4913d7d02865f2
cp -rf CabbageManual ././build/Release/Cabbage.app/Contents/CabbageManual
cp -rf CabbageManual ././build/Release/CabbageLite.app/Contents/CabbageManual

cd $SYSTEM_DEFAULTWORKINGDIRECTORY/Builds/MacOSX/build/Release/
curl -L -o CabbageRack-1.0.0-mac.zip https://github.com/rorywalsh/CabbageRack/releases/download/v1.0/CabbageRack-1.0.0-mac.zip
unzip -q CabbageRack-1.0.0-mac.zip
ls
cp -R CabbageRack Cabbage.app/Contents/CabbageRack
ls Cabbage.app/Contents/
curl -L -o fmod_csound_fx.dylib 'https://github.com/rorywalsh/csoundfmod/releases/download/v2.0/fmod_csound_fx.dylib'
curl -L -o fmod_csound.dylib 'https://github.com/rorywalsh/csoundfmod/releases/download/v2.0/fmod_csound.dylib'
ls
cp fmod_csound.dylib Cabbage.app/Contents/fmod_csound.dylib
cp fmod_csound_fx.dylib Cabbage.app/Contents/fmod_csound_fx.dylib
ls Cabbage.app/Contents/
zip -r CabbageOSX.zip Cabbage.app CabbageLite.app 

cd $SYSTEM_DEFAULTWORKINGDIRECTORY/Builds/MacOSX/

sed -i "" -e "s|SOURCE_PATH|$SYSTEM_DEFAULTWORKINGDIRECTORY|" InstallerAzure.pkgproj

packagesbuild InstallerAzure.pkgproj
ls build  
pwd

VERSION="CabbageOSXInstaller"-$($PROJUCER --get-version ../../CabbageIDE.jucer)".pkg"

cp $SYSTEM_DEFAULTWORKINGDIRECTORY/Builds/MacOSX/build/Cabbage.pkg $BUILD_ARTIFACTSTAGINGDIRECTORY/$VERSION

