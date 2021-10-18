#!/bin/bash  

echo "This uninstall script assumes that Cabbage and its resources were installed into the default directories" 

rm /usr/bin/Cabbage
rm /usr/bin/CabbagePluginEffect.so
rm /usr/bin/CabbagePluginSynth.so
rm /usr/bin/CabbagePlugin
rm -rf /usr/bin/CabbageRack
rm /usr/share/icons/hicolor/512x512/apps/cabbage*
rm -rf /usr/share/doc/cabbage
rm -rf /usr/share/cabbage
rm /usr/share/applications/Cabbage.desktop
