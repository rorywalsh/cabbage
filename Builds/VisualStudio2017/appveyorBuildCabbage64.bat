
REM "Build Script for Cabbage on Windows. 
REM "Please edit this file to point to the location of your Projucer application"
REM "And change the variable MYPATH to the directory of projucer.exe"
REM "Asio driver and VST SDK should be placed under : C:\SDKs\ASIOSDK2.3\   and C:\SDKs\VST_SDK"
REM "If not, you have to modify the jucer files in the Cabbage root directory to ensure all paths are correct."
REM "Then run this script"
REM "exe and dll will be stored under .\x64"
 
::C:\\Users\\rory\\Documents\\sourcecode\\JUCE\\extras\\Projucer\\Builds\\VisualStudio2017\\x64\\Debug\\Projucer.exe
SET PROJUCER="c:\\JUCE\\extras\\Projucer\\Builds\\VisualStudio2017\\x64\\Debug\\App\\Projucer.exe"

REM "BUILDING THE MAIN APP"
 %PROJUCER% --resave ../../CabbageIDE.jucer
  msbuild Cabbage.sln /p:Configuration=Release /property:Platform=x64
 copy ".\x64\Release\App\Cabbage.exe" ".\x64\Cabbage.exe"

 
REM REM "BUILDING EFFECT PLUGIN"
REM  %PROJUCER% --resave ../../CabbagePlugin.jucer
REM  msbuild CabbagePlugin.sln /p:Configuration=Release /property:Platform=x64 
REM  copy ".\x64\Release\VST\CabbagePlugin.dll" ".\x64\CabbagePluginEffect.dll"


REM REM "BUILDING VST PLUGIN"
REM  %PROJUCER% --resave ../../CabbagePluginSynth.jucer
REM  msbuild CabbagePlugin.sln /p:Configuration=Release /property:Platform=x64

REM  copy ".\x64\Release\VST\CabbagePlugin.dll" ".\x64\CabbagePluginSynth.dll"

REM REM "BUILDING CABBAGE LITE"
REM  %PROJUCER% --resave ../../CabbageLite.jucer
REM   msbuild CabbageLite.sln /p:Configuration=Release /property:Platform=x64
REM  copy ".\x64\Release\App\CabbageLite.exe" ".\x64\CabbageLite.exe"


REM REM %MYPATH%Projucer.exe --resave ../../CabbageIDE.jucer

