import os
import shutil
import sys
import platform

rootDir = os.getcwd()

print("===========================================================")
print("Uasge: python buildAll.py BuildType(Debug/Release) taget")
print("===========================================================")

if len(sys.argv) < 2:
    print("Please pass a build type")
    exit()
else:
    buildType = sys.argv[1]

if len(sys.argv) == 3:
    projects = [sys.argv[2]]
else:
    projects = ["Cabbage", "CabbagePluginEffect", "CabbagePluginSynth"]


if not os.path.exists("JUCE"):
    os.system('got clone https://github.com/juce-framework/JUCE.git')

if os.path.exists("Cabbage.app"):
    os.system('rm -rf Cabbage.app')

for project in projects:
    if os.path.exists("build"):
        shutil.rmtree("build")
    os.system('mkdir build')
    os.chdir('build')
    print('cmake -DCMAKE_BUILD_TYPE='+buildType+' -DCMAKE_OSX_ARCHITECTURES="arm64;x86_64" -GXcode .. -DPROJECT_NAME="'+project+'"')
    os.system('cmake -DCMAKE_BUILD_TYPE='+buildType+' -DCMAKE_OSX_ARCHITECTURES="arm64;x86_64" -GXcode .. -DPROJECT_NAME="'+project+'"')
    print("===========================================================")
    print(" Running build for "+project)
    print("===========================================================")    
    os.system('cmake --build . --config '+buildType)

    # post build steps - take binaries and palce them outside build folder
    print("===========================================================")
    print(" Copying binaries before removing build folder")
    print("===========================================================")
    if platform.system() == "Darwin": 
        if project == "Cabbage":
            os.system('cp -Rf Cabbage_artefacts/'+buildType+'/Cabbage.app '+rootDir+'/Cabbage.app')
        elif project == "CabbagePluginEffect" or project == "CabbagePluginSynth":
            os.system('cp -Rf '+project+'_artefacts/'+buildType+'/VST/'+project+'.vst ' +rootDir+'/Cabbage.app/Contents/'+project+'.vst')
            os.system('cp -Rf '+project+'_artefacts/'+buildType+'/VST3/'+project+'.vst3 ' +rootDir+'/Cabbage.app/Contents/'+project+'.vst3')
            os.system('cp -Rf '+project+'_artefacts/'+buildType+'/AU/'+project+'.component ' +rootDir+'/Cabbage.app/Contents/'+project+'.component')
            if project == "CabbagePluginSynth":
                os.system('cp -Rf '+project+'_artefacts/'+buildType+'/Standalone/'+project+'.app ' +rootDir+'/Cabbage.app/Contents/CabbagePlugin.app')

    os.chdir('..')

