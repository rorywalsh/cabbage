#!/bin/bash   

echo "================================================="
echo "======== Generate Makefiles for Cabbage ========="
echo "================================================="
echo "This script will generate makefiles for Cabbage by"
echo "running the ProJUCEr application. Please make sure"
echo "you have already built the JUCE Projucer."
echo ""
echo "Usage:"
echo "./projucerConfig ../path_to/projucer"


if [ "$#" -ne 1 ]; then
	echo $#
    echo "Please pass the full name and path to the Projucer binary."
    exit 1
else
	"$1" --resave CabbageIDE.jucer
	cp Makefile MakefileIDE
	"$1" --resave CabbagePlugin.jucer
	cp Makefile MakefilePluginSynth
	sed 's/CabbagePlugin.so/CabbagePluginSynth.so/g' <Makefile>MakefilePluginSynth
	cp Makefile MakefilePluginEffect
	sed 's/-DCabbage_Plugin_Synth=1//g;s/CabbagePlugin.so/CabbagePluginEffect.so/g' <Makefile>MakefilePluginEffect
fi