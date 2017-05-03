ECHO "Build Script for Cabbage on Windows. Please edit this file to point to the location of your Projucer application"
ECHO "Then modify the jucer files in the Cabbage root directory to ensure all paths are correct. Then run this script"

C:\\Users\\rory\\sourcecode\\JUCE\\extras\\Projucer\\Builds\\VisualStudio2015\\x64\\Debug\\Projucer.exe --resave ../../CabbageIDE.jucer
msbuild Cabbage.vcxproj /p:Configuration=Release /property:Platform=x64

C:\\Users\\rory\\sourcecode\\JUCE\\extras\\Projucer\\Builds\\VisualStudio2015\\x64\\Debug\\Projucer.exe --resave ../../CabbagePlugin.jucer
msbuild CabbagePlugin.vcxproj /p:Configuration=Release /property:Platform=x64 

xcopy ".\x64\Release\CabbagePlugin.dll" ".\x64\Release\CabbagePluginEffect.dll\" /y

C:\\Users\\rory\\sourcecode\\JUCE\\extras\\Projucer\\Builds\\VisualStudio2015\\x64\\Debug\\Projucer.exe --resave ../../CabbagePlugin.jucer
msbuild CabbagePlugin.vcxproj /p:Configuration=Release /property:Platform=x64 /p:Cabbage_Plugin_Synth=1

xcopy ".\x64\Release\CabbagePlugin.dll" ".\x64\Release\CabbagePluginSynth.dll\"  /y

C:\\Users\\rory\\sourcecode\\JUCE\\extras\\Projucer\\Builds\\VisualStudio2015\\x64\\Debug\\Projucer.exe --resave ../../CabbageIDE.jucer

