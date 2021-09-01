from xml.dom import minidom
import xml.etree.ElementTree as ET
import sys
import os

def increment_ver(version):
    version = version.split('.')
    version[2] = str(int(version[2]) + 1)
    return '.'.join(version)

newVersionNum = ""

if len(sys.argv) < 2:
    print("Please provide a commit message enclosed in quotation marks")

if len(sys.argv) == 3:
    newVersionNum = sys.argv[2]

files = ["CabbageIDE.jucer", "CabbagePlugin.jucer", "CabbagePluginSynth.jucer",  "CabbageLite.jucer", "CabbagePluginMIDIEffect.jucer", "CabbagePluginNoStandalone.jucer", "CabbagePluginStandalone.jucer"]

#bump jucer version numbers

for filename in files:
    tree = ET.parse(filename)
    for element in tree.getiterator('JUCERPROJECT'):
        # print(element.attrib['version'])
        verStr = element.attrib['version']
        if len(newVersionNum) == 0:
            newVersionNum = increment_ver(verStr)
        element.set('version', newVersionNum)
        # print(element.attrib['version'])

    tree.write(filename, encoding='utf8')

os.system('git add Source')
os.system('git add Examples')
os.system('git add Docs')
os.system('git commit -m "'+sys.argv[1]+' - Version number:'+newVersionNum+'"')
