#!/bin/bash  

if [[ "$OSTYPE" == "linux-gnu" ]]; then
echo "Linux"

elif [[ "$OSTYPE" == "darwin"* ]]; then
../JUCE/extras/Projucer/Builds/MacOSX/build/Debug/Projucer.app/Contents/MacOS/Projucer --bump-version CabbageIDE.jucer
../JUCE/extras/Projucer/Builds/MacOSX/build/Debug/Projucer.app/Contents/MacOS/Projucer --bump-version CabbagePlugin.jucer
../JUCE/extras/Projucer/Builds/MacOSX/build/Debug/Projucer.app/Contents/MacOS/Projucer --bump-version CabbagePluginSynth.jucer
../JUCE/extras/Projucer/Builds/MacOSX/build/Debug/Projucer.app/Contents/MacOS/Projucer --bump-version CabbageLite.jucer
../JUCE/extras/Projucer/Builds/MacOSX/build/Debug/Projucer.app/Contents/MacOS/Projucer --bump-version CabbagePluginMIDIEffect.jucer

elif [[ "$OSTYPE" == "cygwin" ]]; then
echo "cygwin"

elif [[ "$OSTYPE" == "msys" ]]; then
../JUCE/extras/Projucer/Builds/VisualStudio2019/x64/Debug/App/Projucer.exe --bump-version CabbageIDE.jucer
../JUCE/extras/Projucer/Builds/VisualStudio2019/x64/Debug/App/Projucer.exe --bump-version CabbagePlugin.jucer
../JUCE/extras/Projucer/Builds/VisualStudio2019/x64/Debug/App/Projucer.exe --bump-version CabbagePluginSynth.jucer
../JUCE/extras/Projucer/Builds/VisualStudio2019/x64/Debug/App/Projucer.exe --bump-version CabbageLite.jucer
../JUCE/extras/Projucer/Builds/VisualStudio2019/x64/Debug/App/Projucer.exe --bump-version CabbagePluginMIDIEffect.jucer

elif [[ "$OSTYPE" == "win32" ]]; then
../JUCE/extras/Projucer/Builds/VisualStudio2019/x64/Debug/App/Projucer.exe --bump-version CabbageIDE.jucer
../JUCE/extras/Projucer/Builds/VisualStudio2019/x64/Debug/App/Projucer.exe --bump-version CabbagePlugin.jucer
../JUCE/extras/Projucer/Builds/VisualStudio2019/x64/Debug/App/Projucer.exe --bump-version CabbagePluginSynth.jucer
../JUCE/extras/Projucer/Builds/VisualStudio2019/x64/Debug/App/Projucer.exe --bump-version CabbageLite.jucer
../JUCE/extras/Projucer/Builds/VisualStudio2019/x64/Debug/App/Projucer.exe --bump-version CabbagePluginMIDIEffect.jucer

elif [[ "$OSTYPE" == "freebsd"* ]]; then
echo "Unlikely"
else
echo "What bloody OS are you using dude?!?!"
fi

git add *.jucer
git add Source