
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
curl -L -o csound6.16.0-MacOS_x86_64.dmg 'https://github.com/csound/csound/releases/download/6.16.2/csound-MacOS_x86_64-6.16.2.dmg'
ls
hdiutil attach csound6.16.0-MacOS_x86_64.dmg 
cp -R /Volumes/Csound6.16.2/ Csound
hdiutil detach /Volumes/Csound6.16.2/
cd Csound
sudo installer -pkg csound-MacOS_x86_64-6.16.2.pkg -target /
sudo install_name_tool -id /Library/Frameworks/CsoundLib64.framework/CsoundLib64  /Library/Frameworks/CsoundLib64.framework/CsoundLib64

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

python build.py --config=Release

# curl -L -o Packages.dmg 'http://s.sudre.free.fr/Software/files/Packages.dmg'
# hdiutil mount Packages.dmg
# sudo installer -pkg /Volumes/Packages\ 1.2.9/Install\ Packages.pkg -target /
# hdiutil detach /Volumes/Packages\ 1.2.9/



# cd $SYSTEM_DEFAULTWORKINGDIRECTORY
# curl -L -o CabbageRack-1.0.0-mac.zip https://github.com/rorywalsh/CabbageRack/releases/download/v1.0/CabbageRack-1.0.0-mac.zip
# unzip -q CabbageRack-1.0.0-mac.zip
# ls
# cp -R CabbageRack Cabbage.app/Contents/CabbageRack
# ls Cabbage.app/Contents/
# curl -L -o fmod_csound_fx.dylib 'https://github.com/rorywalsh/csoundfmod/releases/download/v2.0/fmod_csound_fx.dylib'
# curl -L -o fmod_csound.dylib 'https://github.com/rorywalsh/csoundfmod/releases/download/v2.0/fmod_csound.dylib'
# ls
# cp fmod_csound.dylib Cabbage.app/Contents/fmod_csound.dylib
# cp fmod_csound_fx.dylib Cabbage.app/Contents/fmod_csound_fx.dylib
# ls Cabbage.app/Contents/
# zip -r CabbageOSX.zip Cabbage.app CabbageLite.app 

# cd $SYSTEM_DEFAULTWORKINGDIRECTORY/Builds/MacOSX/

# sed -i "" -e "s|SOURCE_PATH|$SYSTEM_DEFAULTWORKINGDIRECTORY|" InstallerAzure.pkgproj

# packagesbuild InstallerAzure.pkgproj
# ls build  
# pwd

# VERSION="CabbageOSXInstaller"-$($PROJUCER --get-version ../../CabbageIDE.jucer)".pkg"

# cp $SYSTEM_DEFAULTWORKINGDIRECTORY/Builds/MacOSX/build/Cabbage.pkg $BUILD_ARTIFACTSTAGINGDIRECTORY/$VERSION

