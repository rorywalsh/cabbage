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

sudo apt-get build-dep csound
sudo apt-get update --fix-missing
sudo apt-get update -qq
sudo apt-get install -y libfreetype6-dev
sudo apt-get install -y libx11-dev
sudo apt-get install -y libstdc++6
sudo apt-get install -y libc++-dev
sudo apt-get install -y libxinerama-dev
sudo apt-get install -y libxrandr-dev
sudo apt-get install -y libxcursor-dev
sudo apt-get install -y libxcomposite-dev
sudo apt-get install -y mesa-common-dev
sudo apt-get install -y libasound2-dev
sudo apt-get install -y freeglut3-dev
sudo apt-get install -y libcurl4-gnutls-dev+
sudo apt-get install -y libasound2-dev
sudo apt-get install -y libsndfile1
sudo apt-get install -y libsndfile-dev
sudo apt-get install -y libjack-dev
sudo add-apt-repository -y ppa:webkit-team/ppa
sudo apt-get install flex
sudo apt-get install bison
sudo apt-get update
sudo apt-get install libwebkit2gtk-4.0-37 libwebkit2gtk-4.0-dev
sudo apt-get update --fix-missing
cd $AGENT_BUILDDIRECTORY
git clone https://github.com/WeAreROLI/JUCE.git
cd $AGENT_BUILDDIRECTORY/JUCE/extras/Projucer/JuceLibraryCode
sed -i '' "s/#define JUCER_ENABLE_GPL_MODE 1/#define JUCER_ENABLE_GPL_MODE 1/" AppConfig.h
sed -i '' "s/#define JUCE_USE_DARK_SPLASH_SCREEN 1/#define JUCE_USE_DARK_SPLASH_SCREEN 0/" AppConfig.h
cat AppConfig.h
cd $AGENT_BUILDDIRECTORY/JUCE/extras/Projucer/Builds/LinuxMakefile
make -j6
cd $AGENT_BUILDDIRECTORY
mkdir csoundMaster
cd csoundMaster
git clone https://github.com/csound/csound.git
cd csound
mkdir build
cd build
cmake .. -DUSE_GETTEXT=0 -DBUILD_CSOUNDVST=0 -DBUILD_PD_CLASS=0 -DBUILD_STATIC_LIBRARY=1 -DBUILD_VST4CS_OPCODES=0 -DBUILD_VST4CS_OPCODES=0 -DBUILD_CSOUND_AC_LUA_INTERFACE=0 -DBUILD_LUA_OPCODES:BOOL=0 -DBUILD_CSOUND_AC=0 -DBUILD_CSOUND_AC_LUA_INTERFACE=0 -DBUILD_CSOUND_AC_PYTHON_INTERFAC=0 
sudo make -j4
sudo make install
sudo ldconfig

pwd
cd ~
pwd
ls
curl -L -o vstsdk3611_22_10_2018_build_34.zip https://download.steinberg.net/sdk_downloads/vstsdk3611_22_10_2018_build_34.zip
unzip -q vstsdk3611_22_10_2018_build_34.zip
ls
mkdir ~/SDKs
cp -rf VST_SDK ~/SDKs
curl -L -o heads.zip http://cabbageaudio.com/beta/heads.zip
unzip -q heads.zip
cp -rf vst2.x ~/SDKs/VST_SDK/VST3_SDK/pluginterfaces
ls ~/SDKs/VST_SDK/VST3_SDK/pluginterfaces
echo "\n"

cd $SYSTEM_DEFAULTWORKINGDIRECTORY/Builds/LinuxMakefile
pwd
ls
./buildCabbage
./installCabbage
./buildDebPackage.sh

export PROJUCER=$AGENT_BUILDDIRECTORY/JUCE/extras/Projucer/Builds/MacOSX/build/Debug/Projucer.app/Contents/MacOS/Projucer
VERSION="CabbageOSXInstaller"-$($PROJUCER --get-version ../../CabbageIDE.jucer)".pkg"

cp $SYSTEM_DEFAULTWORKINGDIRECTORY/Builds/LinuxMakefile/Cabbage/Cabbage.deb $BUILD_ARTIFACTSTAGINGDIRECTORY/$VERSION
