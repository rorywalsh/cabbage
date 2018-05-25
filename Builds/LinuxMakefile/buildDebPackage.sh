#!/bin/bash

rm -rf Cabbage
mkdir -p Cabbage/DEBIAN
mkdir -p Cabbage/usr/bin
mkdir -p Cabbage/usr/share/doc/cabbage/Examples
mkdir -p Cabbage/usr/share/doc/cabbage/CabbageManual
mkdir -p Cabbage/usr/share/icons/hicolor/512x512/apps


cp /usr/bin/CabbagePluginEffect.so Cabbage/usr/bin/CabbagePluginEffect.so
cp /usr/bin/CabbagePluginSynth.so Cabbage/usr/bin/CabbagePluginSynth.so
cp /usr/bin/CabbageLite Cabbage/usr/bin/Cabbage
cp /usr/bin/testCsoundFile Cabbage/usr/bin/testCsoundFile

cp ./../opcodes.txt Cabbage/usr/bin/opcodes.txt
cp ./../../Images/cabbage.png Cabbage/usr/share/icons/hicolor/512x512/apps/cabbage.png
cp ./../../Images/cabbage.png Cabbage/usr/share/icons/hicolor/512x512/apps/cabbagelite.png
cp -rf  /usr/share/doc/cabbage/Examples Cabbage/usr/share/doc/cabbage/Examples
cp -rf  ../../CabbageManual Cabbage/usr/share/doc/cabbage/CabbageManual

cp Cabbage.desktop ~/.local/share/applications/
cp CabbageLite.desktop ~/.local/share/applications/

cp control Cabbage/DEBIAN/control

dpkg-deb --build Cabbage
