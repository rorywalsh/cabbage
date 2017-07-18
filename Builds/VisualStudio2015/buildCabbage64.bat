ECHO "Build Script for Cabbage on Windows. Please edit this file to point to the location of your Projucer application"
ECHO "Then modify the jucer files in the Cabbage root directory to ensure all paths are correct. Then run this script"

C:\\Users\\rory\\sourcecode\\JUCE\\extras\\Projucer\\Builds\\VisualStudio2015\\x64\\Debug\\Projucer.exe --resave ../../CabbageIDE.jucer
msbuild Cabbage.vcxproj /p:Configuration=Release /property:Platform=x64

cp ".\x64\Release\Cabbage.exe" ".\x64\Cabbage.exe"

C:\\Users\\rory\\sourcecode\\JUCE\\extras\\Projucer\\Builds\\VisualStudio2015\\x64\\Debug\\Projucer.exe --resave ../../CabbagePlugin.jucer
msbuild CabbagePlugin.vcxproj /p:Configuration=Release /property:Platform=x64 

cp ".\x64\Release\CabbagePlugin.dll" ".\x64\CabbagePluginEffect.dll"

C:\\Users\\rory\\sourcecode\\JUCE\\extras\\Projucer\\Builds\\VisualStudio2015\\x64\\Debug\\Projucer.exe --resave ../../CabbagePluginSynth.jucer
msbuild CabbagePlugin.vcxproj /p:Configuration=Release /property:Platform=x64

cp ".\x64\Release\CabbagePlugin.dll" ".\x64\CabbagePluginSynth.dll"

C:\\Users\\rory\\sourcecode\\JUCE\\extras\\Projucer\\Builds\\VisualStudio2015\\x64\\Debug\\Projucer.exe --resave ../../CabbageIDE.jucer

