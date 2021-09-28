import os
import shutil
import sys
import platform
import requests
import argparse
import zipfile

info = """
================================================
=== Build script for Cabbage Plugin Framework ==
================================================
=== Run python build.py -h for help...        ==
================================================"""

print(info)

# Instantiate the parser
parser = argparse.ArgumentParser(description="")
parser.add_argument('--config', type=str,
                    help='Debug/Release')

parser.add_argument('--project', type=str,
                    help='Cabbage, CabbagePluginEffect, CabagePluginSynth')

parser.add_argument('--build_type', type=str,
                    help='"Local Release", "Remote Release", "Local Debug"')

args = parser.parse_args()

if args.config is None:
    print("You must pass a valid config, i.e, --config=Debug ")
    exit()



configType = args.config
if args.project is not None:
    projects = [args.project]
else:
    projects = ["Cabbage", "CabbagePluginEffect", "CabbagePluginSynth"]

if args.build_type is not None:
    buildType = [args.build_type]
else:
    buildType = "Local Debug"

if buildType is not "Local Debug":
    if not os.path.exists("CabbageManual.zip") and not os.path.exists("CabbageManual"):
        url = "http://cabbageaudio.com/beta/CabbageManual.zip"
        r = requests.get(url, allow_redirects=True)
        open('CabbageManual.zip', 'wb').write(r.content)       
        with zipfile.ZipFile("CabbageManual.zip", 'r') as zip_ref:
            zip_ref.extractall('CabbageManual')


rootDir = os.getcwd()

if len(sys.argv) < 2:
    print("Please pass a build type")
    exit()
else:
    configType = sys.argv[1]


if not os.path.exists("JUCE"):
    os.system('git clone https://github.com/juce-framework/JUCE.git')
    os.system('git apply ./patches/StandaloneWrapper.patch')
    os.system('git apply ./patches/AUWrapper.patch')
    os.system('git apply ./patches/UtilityWrapper.patch')
    os.system('git apply ./patches/VST2Wrapper.patch')
    os.system('git apply ./patches/VST3Wrapper.patch')

# curl -L -o CabbageManual.zip 'http://cabbageaudio.com/beta/CabbageManual.zip'

if platform.system() == "Darwin" and os.path.exists("Cabbage.app"):
    os.system('rm -rf Cabbage.app')

if platform.system() == "Windows" and os.path.exists("CabbageInstall"):
    os.system('rm -rf CabbageInstall')

if platform.system() == "Windows":   
    os.system('mkdir CabbageInstall')


for project in projects:
    if os.path.exists("build"):
        shutil.rmtree("build")
    os.system('mkdir build')
    os.chdir('build')
    if platform.system() == "Darwin": 
        os.system('cmake -DCMAKE_BUILD_TYPE='+configType+' -DCMAKE_OSX_ARCHITECTURES="arm64;x86_64" -GXcode .. -DPROJECT_NAME="'+project+'"')
    elif platform.system() == "Windows": 
        os.system('cmake -DCMAKE_BUILD_TYPE='+configType+'  -G "Visual Studio 16 2019" .. -DPROJECT_NAME="'+project+'"')
    print("===========================================================")
    print(" Running build for "+project)
    print("===========================================================")    
    os.system('cmake --build . --config '+configType)

    # post build steps - take binaries and palce them outside build folder
    print("===========================================================")
    print(" Copying binaries before removing build folder")
    print("===========================================================")
    if platform.system() == "Darwin": 
        if project == "Cabbage":
            os.system('cp -Rf Cabbage_artefacts/'+configType+'/Cabbage.app '+rootDir+'/Cabbage.app')
            if buildType is not "Local Debug":
                os.system('cp -rf Examples Cabbage.app/Contents/Examples')
                os.system('cp -rf Themes Cabbage.app/Contents/Themes')
                os.system('cp -rf CabbageManual ././build/Release/Cabbage.app/Contents/CabbageManual')

        elif project == "CabbagePluginEffect" or project == "CabbagePluginSynth":
            os.system('cp -Rf '+project+'_artefacts/'+configType+'/VST/'+project+'.vst ' +rootDir+'/Cabbage.app/Contents/'+project+'.vst')
            os.system('cp -Rf '+project+'_artefacts/'+configType+'/VST3/'+project+'.vst3 ' +rootDir+'/Cabbage.app/Contents/'+project+'.vst3')
            os.system('cp -Rf '+project+'_artefacts/'+configType+'/AU/'+project+'.component ' +rootDir+'/Cabbage.app/Contents/'+project+'.component')
            if project == "CabbagePluginSynth":
                os.system('cp -Rf '+project+'_artefacts/'+configType+'/Standalone/'+project+'.app ' +rootDir+'/Cabbage.app/Contents/CabbagePlugin.app')

    if platform.system() == "Windows": 
        if project == "Cabbage":
            os.system('cp -Rf Cabbage_artefacts/'+configType+'/Cabbage.exe '+rootDir+'/CabbageBinaries/Cabbage.exe')
        elif project == "CabbagePluginEffect" or project == "CabbagePluginSynth":
            os.system('cp -Rf '+project+'_artefacts/'+configType+'/VST/'+project+'.dll ' +rootDir+'/CabbageBinaries/'+project+'.dll')
            os.system('cp -Rf '+project+'_artefacts/'+configType+'/VST3/'+project+'.vst3/Contents/x86_64-win/'+project+'.vst3 ' +rootDir+'/CabbageBinaries/'+project+'.vst3')
            if project == "CabbagePluginSynth":
                os.system('cp -Rf '+project+'_artefacts/'+configType+'/Standalone/'+project+'.exe ' +rootDir+'/CabbageBinaries/CabbagePlugin.exe')

    os.chdir('..')

