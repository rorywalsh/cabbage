#!/bin/bash  

echo "This uninstall script assumes that Cabbage and its resources were installed into the default directories" 

rm /usr/local/bin/Cabbage
rm /usr/local/bin/CabbagePluginEffect.so
rm /usr/local/bin/CabbagePluginSynth.so
rm /usr/local/bin/CabbagePlugin
rm -rf /usr/local/bin/CabbageRack
rm /usr/local/share/icons/hicolor/512x512/apps/cabbage*
rm -rf /usr/local/share/doc/cabbage
rm -rf /usr/local/share/cabbage
rm /usr/local/share/applications/Cabbage.desktop

# ... missed by original uninstaller
rm /usr/local/bin/CabbagePluginMidiEffect.so
rm -rf /usr/local/bin/CabbagePluginSynth.vst3
rm -rf /usr/local/bin/CabbagePluginEffect.vst3
