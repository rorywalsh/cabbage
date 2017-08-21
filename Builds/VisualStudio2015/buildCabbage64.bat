
REM "Build Script for Cabbage on Windows. 
REM "Please edit this file to point to the location of your Projucer application"
REM "And change the variable MYPATH to the directory of projucer.exe"
REM "Asio driver and VST SDK should be placed under : C:\SDKs\ASIOSDK2.3\   and C:\SDKs\VST_SDK"
REM "If not, you have to modify the jucer files in the Cabbage root directory to ensure all paths are correct."
REM "Then run this script"
REM "exe and dll will be stored under .\x64"

PAUSE

::C:\\Users\\rory\\sourcecode\\JUCE\\extras\\Projucer\\Builds\\VisualStudio2015\\x64\\Debug\\Projucer.exe

SET MYPATH="C:\Users\Cube\Documents\JUCE-master\extras\Projucer\Builds\VisualStudio2015\x64\Debug\App\"

REM BUILDING THE MAIN APP IS OK
 %MYPATH%Projucer.exe --resave ../../CabbageIDE.jucer
  msbuild Cabbage.sln /p:Configuration=Release /property:Platform=x64
 copy ".\x64\Release\App\Cabbage.exe" ".\x64\Cabbage.exe"

 
REM BUILDING EFFECT PLUGIN IS OK : See CHANGES in projucer file
 %MYPATH%Projucer.exe --resave ../../CabbagePlugin.jucer
 msbuild CabbagePlugin.sln /p:Configuration=Release /property:Platform=x64 
 copy ".\x64\Release\VST\CabbagePlugin.dll" ".\x64\CabbagePluginEffect.dll"


REM BUILDING VST PLUGIN IS OK
 %MYPATH%Projucer.exe --resave ../../CabbagePluginSynth.jucer
 msbuild CabbagePlugin.sln /p:Configuration=Release /property:Platform=x64

 copy ".\x64\Release\VST\CabbagePlugin.dll" ".\x64\CabbagePluginSynth.dll"

REM %MYPATH%Projucer.exe --resave ../../CabbageIDE.jucer

