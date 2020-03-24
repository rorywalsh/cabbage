#!/bin/bash  

if [[ "$OSTYPE" == "linux-gnu" ]]; then
/home/rory/sourcecode/JUCE/extras/Projucer/Builds/LinuxMakefile/build/Projucer --bump-version CabbageIDE.jucer
/home/rory/sourcecode/JUCE/extras/Projucer/Builds/LinuxMakefile/build/Projucer --bump-version CabbagePlugin.jucer
/home/rory/sourcecode/JUCE/extras/Projucer/Builds/LinuxMakefile/build/Projucer --bump-version CabbagePluginNoStandalone.jucer
/home/rory/sourcecode/JUCE/extras/Projucer/Builds/LinuxMakefile/build/Projucer --bump-version CabbagePluginSynth.jucer
/home/rory/sourcecode/JUCE/extras/Projucer/Builds/LinuxMakefile/build/Projucer --bump-version CabbageLite.jucer
/home/rory/sourcecode/JUCE/extras/Projucer/Builds/LinuxMakefile/build/Projucer --bump-version CabbagePluginMIDIEffect.jucer



elif [[ "$OSTYPE" == "darwin"* ]]; then
# ../JUCE/extras/Projucer/Builds/MacOSX/build/Debug/Projucer.app/Contents/MacOS/Projucer --set-version 2.3.0 CabbageIDE.jucer
# ../JUCE/extras/Projucer/Builds/MacOSX/build/Debug/Projucer.app/Contents/MacOS/Projucer --set-version 2.3.0 CabbagePlugin.jucer
# ../JUCE/extras/Projucer/Builds/MacOSX/build/Debug/Projucer.app/Contents/MacOS/Projucer --set-version 2.3.0 CabbagePluginSynth.jucer
# ../JUCE/extras/Projucer/Builds/MacOSX/build/Debug/Projucer.app/Contents/MacOS/Projucer --set-version 2.3.0 CabbageLite.jucer
# ../JUCE/extras/Projucer/Builds/MacOSX/build/Debug/Projucer.app/Contents/MacOS/Projucer --set-version 2.3.0 CabbagePluginMIDIEffect.jucer

../JUCE/extras/Projucer/Builds/MacOSX/build/Debug/Projucer.app/Contents/MacOS/Projucer --bump-version CabbageIDE.jucer
../JUCE/extras/Projucer/Builds/MacOSX/build/Debug/Projucer.app/Contents/MacOS/Projucer --bump-version CabbagePlugin.jucer
../JUCE/extras/Projucer/Builds/MacOSX/build/Debug/Projucer.app/Contents/MacOS/Projucer --bump-version CabbagePluginNoStandalone.jucer
../JUCE/extras/Projucer/Builds/MacOSX/build/Debug/Projucer.app/Contents/MacOS/Projucer --bump-version CabbagePluginSynth.jucer
../JUCE/extras/Projucer/Builds/MacOSX/build/Debug/Projucer.app/Contents/MacOS/Projucer --bump-version CabbageLite.jucer
../JUCE/extras/Projucer/Builds/MacOSX/build/Debug/Projucer.app/Contents/MacOS/Projucer --bump-version CabbagePluginMIDIEffect.jucer
elif [[ "$OSTYPE" == "cygwin" ]]; then
echo "cygwin"

elif [[ "$OSTYPE" == "msys" ]]; then
../JUCE/extras/Projucer/Builds/VisualStudio2019/x64/Debug/App/Projucer.exe --bump-version CabbageIDE.jucer
../JUCE/extras/Projucer/Builds/VisualStudio2019/x64/Debug/App/Projucer.exe --bump-version CabbagePlugin.jucer
../JUCE/extras/Projucer/Builds/VisualStudio2019/x64/Debug/App/Projucer.exe --bump-version CabbagePluginNoStandalone.jucer
../JUCE/extras/Projucer/Builds/VisualStudio2019/x64/Debug/App/Projucer.exe --bump-version CabbagePluginSynth.jucer
../JUCE/extras/Projucer/Builds/VisualStudio2019/x64/Debug/App/Projucer.exe --bump-version CabbageLite.jucer
../JUCE/extras/Projucer/Builds/VisualStudio2019/x64/Debug/App/Projucer.exe --bump-version CabbagePluginMIDIEffect.jucer

elif [[ "$OSTYPE" == "win32" ]]; then
../JUCE/extras/Projucer/Builds/VisualStudio2019/x64/Debug/App/Projucer.exe --bump-version CabbageIDE.jucer
../JUCE/extras/Projucer/Builds/VisualStudio2019/x64/Debug/App/Projucer.exe --bump-version CabbagePlugin.jucer
../JUCE/extras/Projucer/Builds/VisualStudio2019/x64/Debug/App/Projucer.exe --bump-version CabbagePluginNoStandalone.jucer
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