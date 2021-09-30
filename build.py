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
================================================
"""

print(info)
rootDir = os.getcwd()

platformArch = os.popen('arch').read()



def getVersionNumber():
    with open(rootDir+"/CMakeLists.txt", "rt") as inputFile:
        for line in inputFile:
            if "set" in line and "BUILD_VERSION" in line:
                number = line.replace("set", "").replace("(", "").replace(")", ""). replace("BUILD_VERSION", "")
                return number.strip()
        return -1

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
    buildType = args.build_type
else:
    buildType = "Local Debug"

if platform.system() == "Windows" and os.path.exists("CabbageInstall"):
    os.system('rm -rf CabbageInstall')

if platform.system() == "Windows":   
    os.system('mkdir CabbageInstall')

if buildType is not "Local Debug":
        
    if platform.system() == "Darwin":
        if not os.path.exists("CabbageManual"):
            url = "http://cabbageaudio.com/beta/CabbageManual.zip"
            r = requests.get(url, allow_redirects=True)
            open('CabbageManual.zip', 'wb').write(r.content)       
            with zipfile.ZipFile("CabbageManual.zip", 'r') as zip_ref:
                zip_ref.extractall()
        if not os.path.exists("CabbageRack"):
            print("================== Installing CabbageRack libs ========================")
            url = "https://github.com/rorywalsh/CabbageRack/releases/download/v1.0/CabbageRack-1.0.0-mac.zip"
            r = requests.get(url, allow_redirects=True)
            open('CabbageRack-1.0.0-mac.zip', 'wb').write(r.content)  
            with zipfile.ZipFile("CabbageRack-1.0.0-mac.zip", 'r') as zip_ref:
                zip_ref.extractall()     
        if not os.path.exists("fmod_csound_fx.dylib"):
            print("================== Installing FMOD libs ========================")
            url = "https://github.com/rorywalsh/csoundfmod/releases/download/v2.0/fmod_csound_fx.dylib"
            r = requests.get(url, allow_redirects=True)
            open('fmod_csound_fx.dylib', 'wb').write(r.content)  
        if not os.path.exists("fmod_csound.dylib"):
            print("================== Installing FMOD libs ========================")
            url = "https://github.com/rorywalsh/csoundfmod/releases/download/v2.0/fmod_csound.dylib"
            r = requests.get(url, allow_redirects=True)
            open('fmod_csound.dylib', 'wb').write(r.content)  

    elif platform.system() == "Windows":   
        if not os.path.exists("CabbageRack"):
            url = "https://github.com/rorywalsh/CabbageRack/releases/download/v1.0/CabbageRack-1.0.0-win.zip"
            r = requests.get(url, allow_redirects=True)
            open(rootDir+'/CabbageInstall/CabbageRack-1.0.0-win.zip', 'wb').write(r.content)  
            with zipfile.ZipFile(rootDir+"/CabbageInstall/CabbageRack-1.0.0-win.zip", 'r') as zip_ref:
                zip_ref.extractall() 
            shutil.copytree('CabbageRack', rootDir+'/CabbageInstall/CabbageRack')
        if not os.path.exists("fmod_csound64_fx.dll"):
            url = "https://github.com/rorywalsh/csoundfmod/releases/download/v2.0/fmod_csound64_fx.dll"
            r = requests.get(url, allow_redirects=True)
            open(rootDir+'/CabbageInstall/fmod_csound64_fx.dll', 'wb').write(r.content)  
        if not os.path.exists("fmod_csound64.dll"):
            url = "https://github.com/rorywalsh/csoundfmod/releases/download/v2.0/fmod_csound64.dll"
            r = requests.get(url, allow_redirects=True)
            open(rootDir+'/CabbageInstall/fmod_csound64.dll', 'wb').write(r.content)
        if not os.path.exists("CabbageManual"):
            url = "http://cabbageaudio.com/beta/CabbageManual.zip"
            r = requests.get(url, allow_redirects=True)
            open('CabbageManual.zip', 'wb').write(r.content)       
            with zipfile.ZipFile("CabbageManual.zip", 'r') as zip_ref:
                zip_ref.extractall(rootDir+'/CabbageInstall/CabbageManual')


if "Remote Release" in buildType:        
    print("================== Setting up for Release build ========================")
    if platform.system() == "Darwin":
        stagingDir = os.popen('echo $BUILD_ARTIFACTSTAGINGDIRECTORY').read()
        print("================== Installing Pacakges ========================")
        url = "http://s.sudre.free.fr/Software/files/Packages.dmg"
        r = requests.get(url, allow_redirects=True)
        open('Packages.dmg', 'wb').write(r.content)  
        os.system("hdiutil mount Packages.dmg")
        os.system("sudo installer -pkg /Volumes/Packages\ 1.2.9/Install\ Packages.pkg -target /")
        os.system("hdiutil detach /Volumes/Packages\ 1.2.9/")

        print("================== Installing Csound ========================")
        url = 'https://github.com/csound/csound/releases/download/6.16.2/csound-MacOS_x86_64-6.16.2.dmg'
        r = requests.get(url, allow_redirects=True)
        open('csound6.16.0-MacOS_x86_64.dmg', 'wb').write(r.content)  
        os.system('hdiutil attach '+rootDir+'/csound6.16.0-MacOS_x86_64.dmg')
        os.system('ls')
        os.system('cp -R /Volumes/Csound6.16.2/ Csound')
        os.system('hdiutil detach /Volumes/Csound6.16.2/')
        os.chdir(rootDir+'/Csound')
        os.system('sudo installer -pkg csound-MacOS_x86_64-6.16.2.pkg -target /')
        os.system('sudo install_name_tool -id /Library/Frameworks/CsoundLib64.framework/CsoundLib64  /Library/Frameworks/CsoundLib64.framework/CsoundLib64')

        os.chdir(rootDir)

        url = "https://download.steinberg.net/sdk_downloads/vstsdk3611_22_10_2018_build_34.zip"
        r = requests.get(url, allow_redirects=True)
        open('vstsdk3611_22_10_2018_build_34.zip', 'wb').write(r.content)       
        with zipfile.ZipFile("vstsdk3611_22_10_2018_build_34.zip", 'r') as zip_ref:
            zip_ref.extractall()
        os.system('mkdir ~/SDKs')
        os.system('cp -rf VST_SDK ~/SDKs')
        os.system('cp -rf vst2.x ~/SDKs/VST_SDK/VST3_SDK/pluginterfaces')

        url = "http://cabbageaudio.com/beta/heads.zip"
        r = requests.get(url, allow_redirects=True)
        open('heads.zip', 'wb').write(r.content)       
        with zipfile.ZipFile("heads.zip", 'r') as zip_ref:
            zip_ref.extractall()

    if platform.system() == "Windows":
        stagingDir = 'D:/a/1/a'
        url = "https://github.com/rorywalsh/cabbage/releases/download/v2.0.00/csound-windows_x64-6.16.0.zip"
        r = requests.get(url, allow_redirects=True)
        open('csound-windows_x64-6.16.0.zip', 'wb').write(r.content)       
        with zipfile.ZipFile("csound-windows_x64-6.16.0.zip", 'r') as zip_ref:
            zip_ref.extractall('C:/Program Files')

        os.system('mkdir C:/SDKs')
        url = "https://download.steinberg.net/sdk_downloads/asiosdk_2.3.3_2019-06-14.zip"
        r = requests.get(url, allow_redirects=True)
        open('asiosdk_2.3.3_2019-06-14.zip', 'wb').write(r.content)       
        with zipfile.ZipFile("asiosdk_2.3.3_2019-06-14.zip", 'r') as zip_ref:
            zip_ref.extractall('C:/SDKs/ASIOSDK2.3.2')

        url = "https://download.steinberg.net/sdk_downloads/vstsdk3611_22_10_2018_build_34.zip"
        r = requests.get(url, allow_redirects=True)
        open('vstsdk3611_22_10_2018_build_34.zip', 'wb').write(r.content)       
        with zipfile.ZipFile("vstsdk3611_22_10_2018_build_34.zip", 'r') as zip_ref:
            zip_ref.extractall('C:/SDKs/VST_SDK')

        url = "http://cabbageaudio.com/beta/heads.zip"
        r = requests.get(url, allow_redirects=True)
        open('heads.zip', 'wb').write(r.content)       
        with zipfile.ZipFile("heads.zip", 'r') as zip_ref:
            zip_ref.extractall("C:/SDKs/VST_SDK/VST3_SDK/pluginterfaces")
        


os.chdir(rootDir)

if not os.path.exists("JUCE"):
    os.system('git clone https://github.com/juce-framework/JUCE.git')
    os.system('git apply ./patches/StandaloneWrapper.patch')
    os.system('git apply ./patches/AUWrapper.patch')
    os.system('git apply ./patches/UtilityWrapper.patch')
    os.system('git apply ./patches/VST2Wrapper.patch')
    os.system('git apply ./patches/VST3Wrapper.patch')
    newFileText = ""
    with open("JUCE/extras/Build/juceaide/CMakeLists.txt", "rt") as cmakeFile:
        for line in cmakeFile:
            if "juce_add_console_app(juceaide)" in line:
                line += "\njuce_add_console_app(juceaide)\n"
            newFileText += line      


if platform.system() == "Darwin" and os.path.exists("Cabbage.app"):
    os.system('rm -rf Cabbage.app')

for project in projects:
    if os.path.exists("build"):
        shutil.rmtree("build")
    os.system('mkdir build')
    os.chdir('build')
    if platform.system() == "Darwin" and 'arm64' in platformArch: 
        os.system('cmake -DCMAKE_BUILD_TYPE='+configType+' -DCMAKE_OSX_ARCHITECTURES="arm64;x86_64" -GXcode .. -DPROJECT_NAME="'+project+'"')
    elif platform.system() == "Darwin":
        os.system('cmake -DCMAKE_BUILD_TYPE='+configType+' -DCMAKE_OSX_ARCHITECTURES="x86_64" -GXcode .. -DPROJECT_NAME="'+project+'"')
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
                os.system('cp -Rf ../Examples '+rootDir+'/Cabbage.app/Contents/Examples')
                os.system('cp -Rf ../Themes '+rootDir+'/Cabbage.app/Contents/Themes')
                os.system('cp -Rf ../CabbageManual '+rootDir+'/Cabbage.app/Contents/CabbageManual')
                os.system('cp -Rf ../CabbageRack '+rootDir+'/Cabbage.app/Contents/CabbageRack')
                os.system('cp ../fmod_csound_fx.dylib '+rootDir+'/Cabbage.app/Contents/fmod_csound_fx.dylib')
                os.system('cp ../fmod_csound.dylib '+rootDir+'/Cabbage.app/Contents/fmod_csound.dylib')

        elif project == "CabbagePluginEffect" or project == "CabbagePluginSynth":
            os.system('cp -Rf '+project+'_artefacts/'+configType+'/VST/'+project+'.vst ' +rootDir+'/Cabbage.app/Contents/'+project+'.vst')
            os.system('cp -Rf '+project+'_artefacts/'+configType+'/VST3/'+project+'.vst3 ' +rootDir+'/Cabbage.app/Contents/'+project+'.vst3')
            os.system('cp -Rf '+project+'_artefacts/'+configType+'/AU/'+project+'.component ' +rootDir+'/Cabbage.app/Contents/'+project+'.component')
            if project == "CabbagePluginSynth":
                os.system('cp -Rf '+project+'_artefacts/'+configType+'/Standalone/'+project+'.app ' +rootDir+'/Cabbage.app/Contents/CabbagePlugin.app')

    if platform.system() == "Windows": 
        if project == "Cabbage":
            os.system('cp -Rf Cabbage_artefacts/'+configType+'/Cabbage.exe '+rootDir+'/CabbageInstall/Cabbage.exe')
            if buildType is not "Local Debug":
                os.system('cp -Rf ../Examples '+rootDir+'/CabbageInstall/Examples')
                os.system('cp -Rf ../Themes '+rootDir+'/CabbageInstall/Themes')
                os.system('cp -Rf ../CabbageRack '+rootDir+'/CabbageInstall/CabbageRack')
                os.system('cp ../fmod_csound_fx.dylib '+rootDir+'/CabbageInstall/fmod_csound_fx.dylib')
                os.system('cp ../fmod_csound.dylib '+rootDir+'/CabbageInstall/fmod_csound.dylib')
        elif project == "CabbagePluginEffect" or project == "CabbagePluginSynth":
            os.system('cp -Rf '+rootDir+'/build/'+project+'_artefacts/'+configType+'/VST/'+project+'.dll ' +rootDir+'/CabbageInstall/'+project+'.dll')
            os.system('cp -Rf '+rootDir+'/build/'+project+'_artefacts/'+configType+'/VST3/'+project+'.vst3/Contents/x86_64-win/'+project+'.vst3 ' +rootDir+'/CabbageInstall/'+project+'.vst3')
            if project == "CabbagePluginSynth":
                os.system('cp -Rf '+rootDir+'/build/'+project+'_artefacts/'+configType+'/Standalone/'+project+'.exe ' +rootDir+'/CabbageInstall/CabbagePlugin.exe')

    os.chdir('..')

# If local release is specified, then packages things from the current dir
if "Local Release" in buildType:
    if platform.system() == "Darwin":
        os.chdir(rootDir+'/Installers/MacOS') 
        os.system('sed -i "" -e "s|SOURCE_PATH|'+rootDir+'|" Installer.pkgproj')
        os.system('packagesbuild Installer.pkgproj')
        os.system('mv ./build/Cabbage.pkg '+rootDir+'/CabbageOSXInstaller-'+getVersionNumber()+'.pkg')

    if platform.system() == "Windows":
        os.chdir(rootDir+'/Installers/Windows') 
        os.system('set PATH=%PATH%;"C:\\Program Files (x86)\\Inno Setup 5"')
        os.system('iscc Installer.iss')

# If remote release is specified, package things into teh staging directory
if "Remote Release" in buildType:
    if platform.system() == "Darwin":
        os.chdir(rootDir+'/Installers/MacOS') 
        os.system('sed -i "" -e "s|SOURCE_PATH|'+rootDir+'|" Installer.pkgproj')
        os.system('packagesbuild Installer.pkgproj')
        os.system('mv ./build/Cabbage.pkg '+stagingDir+'/CabbageOSXInstaller-'+getVersionNumber()+'.pkg')

    if platform.system() == "Windows":
        os.chdir(rootDir+'/Installers/Windows') 
        os.system('set PATH=%PATH%;"C:\\Program Files (x86)\\Inno Setup 5"')
        os.system('iscc Installer.iss')
        os.system(rootDir+'/Installers/Windows/Output')
        os.system('cp '+rootDir+'/Installers/Windows/Output/Cabbage64Setup.exe '+stagingDir+'/Cabbage64Setup-'+getVersionNumber()+'.exe')