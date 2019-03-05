dir  
cd c:/
Invoke-WebRequest -Uri "https://github.com/rorywalsh/cabbage/releases/download/v2.0.00/csound-windows_x64-6.12.0.zip" -OutFile "C:\csound-windows_x64-6.12.0.zip" 
7z.exe x csound-windows_x64-6.12.0.zip -o"C:/Program Files"
cd "C:/Program Files/Csound6_x64"
dir
cd c:/
Invoke-WebRequest -Uri "https://download.steinberg.net/sdk_downloads/vstsdk3611_22_10_2018_build_34.zip" -OutFile "C:\vstsdk3611_22_10_2018_build_34.zip" 
Invoke-WebRequest -Uri "http://www.steinberg.net/sdk_downloads/ASIOSDK2.3.2.zip" -OutFile "C:\ASIOSDK2.3.2.zip" 
Invoke-WebRequest -Uri "http://cabbageaudio.com/beta/heads.zip" -OutFile "C:\heads.zip" 

7z.exe x vstsdk3611_22_10_2018_build_34.zip
7z.exe x asiosdk2.3.2.zip
7z.exe x heads.zip
mkdir SDKs  
dir
Get-ChildItem -Path "C:/VST_SDK" | Copy-Item -Destination "C:/SDKs/VST_SDK" -Recurse -Container
Get-ChildItem -Path "C:/ASIOSDK2.3.2" | Copy-Item -Destination "C:/SDKs/ASIOSDK2.3.2" -Recurse -Container
Copy-Item "C:/vst2.x" -Destination "C:/SDKs/VST_SDK/VST3_SDK/pluginterfaces/" -Recurse
cd c:/SDKs/VST_SDK/VST3_SDK/pluginterfaces/
dir
cd c:/SDKs/VST_SDK/VST3_SDK/pluginterfaces/vst2.x/
dir
cd c:/
git clone https://github.com/WeAreROLI/JUCE.git
cd c:/JUCE
git checkout tags/5.4.3
cd c:/JUCE/extras/Projucer/JuceLibraryCode
(Get-Content -Path "AppConfig.h") | ForEach-Object {$_ -Replace "#define JUCER_ENABLE_GPL_MODE 1", "#define JUCER_ENABLE_GPL_MODE 0"} | Set-Content -Path "AppConfig.h"
(Get-Content -Path "AppConfig.h") | ForEach-Object {$_ -Replace "#define JUCE_USE_DARK_SPLASH_SCREEN 1", "#define JUCE_USE_DARK_SPLASH_SCREEN 0"} | Set-Content -Path "AppConfig.h"

cd ../Builds/VisualStudio2017/
"C:\Program Files (x86)\Microsoft Visual Studio\2017\Enterprise\MSBuild\15.0\Bin\msbuild.exe" Projucer.sln /property:Platform=x64
cd c:/cabbage/Builds/VisualStudio2017/
appveyorBuildCabbage64.bat  
cd c:/cabbage/CsoundTestWin64 
"C:\Program Files (x86)\Microsoft Visual Studio\2017\Enterprise\MSBuild\15.0\Bin\msbuild.exe" CsoundTest.sln /property:Platform=x64
cd C:/cabbage
Invoke-WebRequest -Uri "http://cabbageaudio.com/beta/CabbageManual.zip" -OutFile "C:\CabbageManual.zip" 
7z.exe x CabbageManual.zip  -oC:\cabbage\CabbageManual

Invoke-WebRequest -Uri "https://github.com/rorywalsh/CabbageRack/releases/download/v1.0/CabbageRack-0.5.0-win.zip" -OutFile "C:\CabbageRack-0.5.0-win.zip" 

7z.exe x CabbageRack-0.5.0-win.zip -oC:\cabbage\Builds\VisualStudio2017\x64\CabbageRack
ls
cd c:/cabbage/Builds/VisualStudio2017/x64

Invoke-WebRequest -Uri "https://github.com/rorywalsh/csoundfmod/releases/download/v1.1/fmod_csoundL64.dll" -OutFile "C:\fmod_csoundL64.dll" 

ls
cd c:/cabbage/Builds/VisualStudio2017
set PATH=%PATH%;"C:\\Program Files (x86)\\Inno Setup 5"
iscc CabbageCannonicalInstaller.iss
