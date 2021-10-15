import os
import shutil
import sys
import platform
import requests
import argparse
import zipfile

RED   = "\033[1;31m"  
BLUE  = "\033[1;34m"
CYAN  = "\033[1;36m"
GREEN = "\033[0;32m"
YELLOW = "\033[0;93m"
RESET = "\033[0;0m"
BOLD    = "\033[;1m"


sys.stdout.write(BOLD)
info = """
================================================
=== Build script for Cabbage Plugin Framework ==
================================================
=== Run python build.py -h for help...        ==
================================================
"""

print(info)
rootDir = os.getcwd()
sys.stdout.write(RESET)
platformArch = os.popen('arch').read()

def getVersionNumber():
    with open(rootDir+"/CMakeLists.txt", "rt") as inputFile:
        for line in inputFile:
            if "set" in line and "BUILD_VERSION" in line:
                number = line.replace("set", "").replace("(", "").replace(")", ""). replace("BUILD_VERSION", "")
                return number.strip()
        return -1

       
# ==============================================================================================
# Instantiate the parser
# ==============================================================================================
parser = argparse.ArgumentParser(description="")
parser.add_argument('--config', type=str,
                    help='Debug/Release')

parser.add_argument('--project', type=str,
                    help='Cabbage, CabbagePluginEffect, CabagePluginSynth')

parser.add_argument('--packageType', type=str,
                    help='"Local", "Remote", "Minimal"')

parser.add_argument('--disable-feature', dest='feature', 
                    action='store_false')

parser.add_argument('--build', type=str, 
                    help='Set to "False" to disable building, in which case CMake will just generate the respective project in the build folder. You must specify a project if you disable building.')

parser.add_argument('--manufacturer', type=str,
                    help='"CabbageAudio"')

parser.add_argument('--pluginDescription', type=str,
                    help='"CabbagePlugin"')

parser.add_argument('--manufacturerCode', type=str,
                    help='A unique 4 character ID with an uppercase first character, i.e, "Cabb"')

parser.add_argument('--installLinuxDeps', type=str,
                    help='Set to False, set to True if you wish to download the following dependencies needed to build JUCE applications on Linux - libfreetype6-dev libx11-dev libstdc++6 libc++-dev libxinerama-dev libxrandr-dev libxcursor-dev libxcomposite-dev mesa-common-dev libasound2-dev freeglut3-dev libcurl4-gnutls-dev libasound2-dev libsndfile1 libsndfile-dev libjack-dev flex bison libwebkit2gtk-4.0-37 libwebkit2gtk-4.0-dev')

parser.add_argument('--license', type=str,
                    help='GPL by default')

parser.add_argument('--generator', type=str,
                    help='Defaults VS2019 on Windows, XCode on Mac, and Ninja or Linux')

args = parser.parse_args()

if args.config is None:
    print("You must pass a valid config, i.e, --config=Debug ")
    exit()

configType = args.config


if args.generator is not None:
    generator = args.generator
else:
    if platform.system() == "Windows":
        generator = "Visual Studio 16 2019"
    elif platform.system() == "Linux":
        generator = "Ninja"
    elif platform.system() == "Darwin":
        generator = "XCode"

if args.manufacturer is not None:
    manufacturer = args.manufacturer
else:
    manufacturer = "CabbageAudio"

if args.project is not None:
    projects = [args.project]
else:
    if manufacturer == "CabbageAudio":
        projects = ["Cabbage", "CabbagePluginEffect", "CabbagePluginSynth"]
    else:
        projects = ["Cabbage", manufacturer+"Effect", manufacturer+"Synth"]

if args.packageType is not None:
    packageType = args.packageType
else:
    packageType = "Local"

executeBuild = True

if args.build is not None:
    if "False" in args.build and args.project is not None:
        executeBuild = False
    elif "False" in args.build and args.project is None:
        sys.stdout.write(RED)
        print('Warning: If you are generating build files, without actually building, you must specify a project')
        sys.stdout.write(RESET)
        exit()
    else:
        executeBuild = True 

if args.pluginDescription is not None:
    pluginDescription = args.pluginDescription
else:
    pluginDescription = "CabbagePlugin"

if args.manufacturerCode is not None:
    manufacturerCode = args.manufacturerCode
else:
    manufacturerCode = "Cabb"

buildPro = 0
if args.license is not None:
    if "GPL" in args.license or "gpl" in args.license:
        buildPro = 0
    else:
        buildPro = 1

installLinuxDeps = False
if args.installLinuxDeps is not None:
    if "True" in args.installLinuxDeps:
        installLinuxDeps = True
    else:
        installLinuxDeps = False

if platform.system() == "Linux" and installLinuxDeps is True:
    os.system('sudo apt-get update --fix-missing')
    os.system('sudo apt-get update -qq')
    os.system('sudo apt-get install -y libfreetype6-dev')
    os.system('sudo apt-get install -y libx11-dev')
    os.system('sudo apt-get install -y libstdc++6')
    os.system('sudo apt-get install -y libc++-dev')
    os.system('sudo apt-get install -y libxinerama-dev')
    os.system('sudo apt-get install -y libxrandr-dev')
    os.system('sudo apt-get install -y libxcursor-dev')
    os.system('sudo apt-get install -y libxcomposite-dev')
    os.system('sudo apt-get install -y mesa-common-dev')
    os.system('sudo apt-get install -y libasound2-dev')
    os.system('sudo apt-get install -y freeglut3-dev')
    os.system('sudo apt-get install -y libcurl4-gnutls-dev+')
    os.system('sudo apt-get install -y libasound2-dev')
    os.system('sudo apt-get install -y libsndfile1')
    os.system('sudo apt-get install -y libsndfile-dev')
    os.system('sudo apt-get install -y libjack-dev')
    os.system('sudo add-apt-repository -y ppa:webkit-team/ppa')
    os.system('sudo apt-get install flex')
    os.system('sudo apt-get install bison')
    os.system('sudo apt-get update')
    os.system('sudo apt-get install libwebkit2gtk-4.0-37 libwebkit2gtk-4.0-dev')
# ==============================================================================================
if platform.system() == "Windows" and os.path.exists("CabbageInstall"):
    os.system('rm -rf CabbageInstall')

if platform.system() == "Linux" and os.path.exists("CabbageInstall"):
   os.system('rm -rf CabbageInstall')

if platform.system() == "Windows" or platform.system() == "Linux":   
    os.system('mkdir CabbageInstall')

if platform.system() == "Linux":
    os.system('mkdir CabbageInstall/bin')
    os.system('mkdir CabbageInstall/images')
    os.system('mkdir CabbageInstall/desktop')

if "Remote" in packageType:
    if platform.system() == "Darwin" or platform.system() == "Linux":
        stagingDir = os.popen('echo $BUILD_ARTIFACTSTAGINGDIRECTORY').read()
    elif platform.system() == "Windows":
        stagingDir = 'D:/a/1/a'
else:
    stagingDir = rootDir

print('======+++++++++++++++++++++===========')
print(stagingDir)
print('======+++++++++++++++++++++===========')
# ================================================================================================
# ======= Setup for MacOS - this will only install the things it can't find
# ================================================================================================
sys.stdout.write(YELLOW)
if platform.system() == "Darwin":
    if not os.path.exists("CabbageManual"):
        url = "http://cabbageaudio.com/beta/CabbageManual.zip"
        r = requests.get(url, allow_redirects=True, verify=False)
        open('CabbageManual.zip', 'wb').write(r.content)       
        with zipfile.ZipFile("CabbageManual.zip", 'r') as zip_ref:
            zip_ref.extractall()
    else:
        print("Found Cabbage Manual...")

    if not os.path.exists("CabbageRack"):
        print("================== Installing CabbageRack libs ========================")
        url = "https://github.com/rorywalsh/CabbageRack/releases/download/v1.0/CabbageRack-1.0.0-mac.zip"
        r = requests.get(url, allow_redirects=True, verify=False)
        open('CabbageRack-1.0.0-mac.zip', 'wb').write(r.content)  
        with zipfile.ZipFile("CabbageRack-1.0.0-mac.zip", 'r') as zip_ref:
            zip_ref.extractall()    
    else:
        print("Found CabbageRack...") 
    
    if not os.path.exists("fmod_csound_fx.dylib"):
        print("================== Installing FMOD libs ========================")
        url = "https://github.com/rorywalsh/csoundfmod/releases/download/v2.0/fmod_csound_fx.dylib"
        r = requests.get(url, allow_redirects=True, verify=False)
        open('fmod_csound_fx.dylib', 'wb').write(r.content) 
    else:
        print("Found fmod_csound_fx.dylib...") 

    if not os.path.exists("fmod_csound.dylib"):
        print("================== Installing FMOD libs ========================")
        url = "https://github.com/rorywalsh/csoundfmod/releases/download/v2.0/fmod_csound.dylib"
        r = requests.get(url, allow_redirects=True, verify=False)
        open('fmod_csound.dylib', 'wb').write(r.content)  
    else:
        print("Found fmod_csound_.dylib...")        

    if not os.path.exists('/Applications/Packages.app'):
        print("================== Installing Packages ========================")
        url = "http://s.sudre.free.fr/Software/files/Packages.dmg"
        r = requests.get(url, allow_redirects=True, verify=False)
        open('Packages.dmg', 'wb').write(r.content)  
        os.system("hdiutil mount Packages.dmg")
        os.system("sudo installer -pkg /Volumes/Packages\ 1.2.9/Install\ Packages.pkg -target /")
        os.system("hdiutil detach /Volumes/Packages\ 1.2.9/")
    else:
        print("Found Packages.app...")

    if not os.path.exists('/Library/Frameworks/CsoundLib64.framework'):
        print("================== Installing Csound ========================")
        url = 'https://github.com/csound/csound/releases/download/6.16.2/csound-MacOS_x86_64-6.16.2.dmg'
        r = requests.get(url, allow_redirects=True, verify=False)
        open('csound6.16.0-MacOS_x86_64.dmg', 'wb').write(r.content)  
        os.system('hdiutil attach '+rootDir+'/csound6.16.0-MacOS_x86_64.dmg')
        os.system('cp -R /Volumes/Csound6.16.2/ Csound')
        os.system('hdiutil detach /Volumes/Csound6.16.2/')
        os.chdir(rootDir+'/Csound')
        os.system('sudo installer -pkg csound-MacOS_x86_64-6.16.2.pkg -target /')
        os.system('sudo install_name_tool -id /Library/Frameworks/CsoundLib64.framework/CsoundLib64  /Library/Frameworks/CsoundLib64.framework/CsoundLib64')
    else:
        print("Found Csound...")

    os.chdir(rootDir)

    if not os.path.exists(os.path.expanduser('~/SDKs/VST_SDK/VST3_SDK/pluginterfaces')):
        os.system('mkdir ~/SDKs')
        url = "https://download.steinberg.net/sdk_downloads/vstsdk3611_22_10_2018_build_34.zip"
        r = requests.get(url, allow_redirects=True, verify=False)
        open('vstsdk3611_22_10_2018_build_34.zip', 'wb').write(r.content)       
        with zipfile.ZipFile("vstsdk3611_22_10_2018_build_34.zip", 'r') as zip_ref:
            zip_ref.extractall(os.path.expanduser('~/SDKs/VST_SDK'))

        url = "http://cabbageaudio.com/beta/heads.zip"
        r = requests.get(url, allow_redirects=True, verify=False)
        open('heads.zip', 'wb').write(r.content)       
        with zipfile.ZipFile("heads.zip", 'r') as zip_ref:
            zip_ref.extractall(os.path.expanduser('~/SDKs/VST_SDK/VST3_SDK/pluginterfaces')) 
    else:
        print('Found VST SDK...')


# ================================================================================================
# ======= Setup for Windows - this will only install the things it can't find
# ================================================================================================
elif platform.system() == "Windows":  

    if not os.path.exists("C:/Program Files/Csound6_x64"):
        url = "https://github.com/rorywalsh/cabbage/releases/download/v2.0.00/csound-windows_x64-6.16.0.zip"
        r = requests.get(url, allow_redirects=True, verify=False)
        open('csound-windows_x64-6.16.0.zip', 'wb').write(r.content)       
        with zipfile.ZipFile("csound-windows_x64-6.16.0.zip", 'r') as zip_ref:
            zip_ref.extractall('C:/Program Files')
    else:
        print('Found Csound...')

    if not os.path.exists("C:/SDKs/ASIOSDK2.3.2"):
        os.system('mkdir C:/SDKs')
        url = "https://download.steinberg.net/sdk_downloads/asiosdk_2.3.3_2019-06-14.zip"
        r = requests.get(url, allow_redirects=True, verify=False)
        open('asiosdk_2.3.3_2019-06-14.zip', 'wb').write(r.content)       
        with zipfile.ZipFile("asiosdk_2.3.3_2019-06-14.zip", 'r') as zip_ref:
            zip_ref.extractall('C:/SDKs/ASIOSDK2.3.2')
    else:
        print("Found ASIO SDK.....") 

    if not os.path.exists("C:/SDKs/VST_SDK/VST3_SDK/pluginterfaces"):
        url = "https://download.steinberg.net/sdk_downloads/vstsdk3611_22_10_2018_build_34.zip"
        r = requests.get(url, allow_redirects=True, verify=False)
        open('vstsdk3611_22_10_2018_build_34.zip', 'wb').write(r.content)       
        with zipfile.ZipFile("vstsdk3611_22_10_2018_build_34.zip", 'r') as zip_ref:
            zip_ref.extractall('C:/SDKs/VST_SDK')

        url = "http://cabbageaudio.com/beta/heads.zip"
        r = requests.get(url, allow_redirects=True, verify=False)
        open('heads.zip', 'wb').write(r.content)       
        with zipfile.ZipFile("heads.zip", 'r') as zip_ref:
            zip_ref.extractall("C:/SDKs/VST_SDK/VST3_SDK/pluginterfaces") 
    else:
        print('Found VST SDK...')

    if not os.path.exists(rootDir+'/CabbageInstall/CabbageRack-1.0.0-win.zip'):
        url = "https://github.com/rorywalsh/CabbageRack/releases/download/v1.0/CabbageRack-1.0.0-win.zip"
        r = requests.get(url, allow_redirects=True, verify=False)
        open(rootDir+'/CabbageInstall/CabbageRack-1.0.0-win.zip', 'wb').write(r.content)  
        with zipfile.ZipFile(rootDir+"/CabbageInstall/CabbageRack-1.0.0-win.zip", 'r') as zip_ref:
            zip_ref.extractall(rootDir+'/CabbageInstall') 
    else:
        print('Found CabbageRack...')

    if not os.path.exists("fmod_csound64_fx.dll"):
        url = "https://github.com/rorywalsh/csoundfmod/releases/download/v2.0/fmod_csound64_fx.dll"
        r = requests.get(url, allow_redirects=True, verify=False)
        open(rootDir+'/CabbageInstall/fmod_csound64_fx.dll', 'wb').write(r.content)  
    else:
        print('Found fmod_csound64_fx...')

    if not os.path.exists("fmod_csound64.dll"):
        url = "https://github.com/rorywalsh/csoundfmod/releases/download/v2.0/fmod_csound64.dll"
        r = requests.get(url, allow_redirects=True, verify=False)
        open(rootDir+'/CabbageInstall/fmod_csound64.dll', 'wb').write(r.content)
    else:
        print('Found fmod_csound64...')

    if not os.path.exists("CabbageManual"):
        url = "http://cabbageaudio.com/beta/CabbageManual.zip"
        r = requests.get(url, allow_redirects=True, verify=False)
        open('CabbageManual.zip', 'wb').write(r.content)       
        with zipfile.ZipFile("CabbageManual.zip", 'r') as zip_ref:
            zip_ref.extractall(rootDir+'/CabbageInstall/CabbageManual')
    else:
        print('Found CabbageManual...')

# ================================================================================================
# ======= Setup for Linux - this will only install the things it can't find
# ================================================================================================
elif platform.system() == "Linux":   
    if not os.path.exists(rootDir+'/CabbageRack-1.0.0-lin.zip'):
        sys.stdout.write(RED)
        print('Did not find CabbageRack-1.0.0-lin.zip. Downloading....')
        sys.stdout.write(RESET)
        print('')
        url = "https://github.com/rorywalsh/CabbageRack/releases/download/v1.0/CabbageRack-1.0.0-lin.zip"
        r = requests.get(url, allow_redirects=True, verify=False)
        open(rootDir+'/CabbageRack-1.0.0-lin.zip', 'wb').write(r.content)  
    else:
        print('Found CabbageRack...')

    #place CabbageRack into install folder...
    with zipfile.ZipFile(rootDir+"/CabbageRack-1.0.0-lin.zip", 'r') as zip_ref:
        zip_ref.extractall(rootDir+'/CabbageInstall') 

    if not os.path.exists(rootDir+"/CabbageManual.zip"):
        sys.stdout.write(RED)
        print('Did not find CabbageManual.zip. Downloading....')
        sys.stdout.write(RESET)
        print('')
        url = "http://cabbageaudio.com/beta/CabbageManual.zip"
        r = requests.get(url, allow_redirects=True, verify=False)
        open(rootDir+'/CabbageManual.zip', 'wb').write(r.content)       
    else:
        print('Found CabbageManual...')
        print('')

    #place CabbageManual into install folder...
    with zipfile.ZipFile(rootDir+'/CabbageManual.zip', 'r') as zip_ref:
            zip_ref.extractall(rootDir+'/CabbageInstall')

    if not os.path.exists(os.path.expanduser('~/SDKs/VST_SDK/VST3_SDK/pluginterfaces')):
        url = "https://download.steinberg.net/sdk_downloads/vstsdk3611_22_10_2018_build_34.zip"
        r = requests.get(url, allow_redirects=True, verify=False)
        open('vstsdk3611_22_10_2018_build_34.zip', 'wb').write(r.content)       
        with zipfile.ZipFile("vstsdk3611_22_10_2018_build_34.zip", 'r') as zip_ref:
            zip_ref.extractall(os.path.expanduser('~/SDKs/VST_SDK'))

        url = "http://cabbageaudio.com/beta/heads.zip"
        r = requests.get(url, allow_redirects=True, verify=False)
        open('heads.zip', 'wb').write(r.content)       
        with zipfile.ZipFile("heads.zip", 'r') as zip_ref:
            zip_ref.extractall(os.path.expanduser('~/SDKs/VST_SDK/VST3_SDK/pluginterfaces')) 
    else:
        print('Found VST SDK...')

    if not os.path.exists('/usr/local/bin/csound'):
        os.chdir(rootDir)
        sys.stdout.write(RED)
        print('Did not find Csound. Downloading and installing to....')
        sys.stdout.write(RESET)
        os.system('git clone https://github.com/csound/csound.git')
        # os.system("sudo sed -i -- 's/#deb-src/deb-src/g' /etc/apt/sources.list && sudo sed -i -- 's/# deb-src/deb-src/g' /etc/apt/sources.list")
        # os.system('sudo apt-get update')
        os.system('sudo apt-get build-dep csound')
        os.system('mkdir '+rootDir+'/csound/build')
        os.chdir(rootDir+'/csound/build')
        os.system('cmake .. -DUSE_GETTEXT=0 -DBUILD_CSOUNDVST=0 -DBUILD_PD_CLASS=0 -DBUILD_STATIC_LIBRARY=1 -DBUILD_VST4CS_OPCODES=0 -DBUILD_VST4CS_OPCODES=0 -DBUILD_CSOUND_AC_LUA_INTERFACE=0 -DBUILD_LUA_OPCODES:BOOL=0 -DBUILD_CSOUND_AC=0 -DBUILD_CSOUND_AC_LUA_INTERFACE=0 -DBUILD_CSOUND_AC_PYTHON_INTERFAC=0') 
        os.system('sudo make -j4')
        os.system('sudo make install')
        os.system('sudo ldconfig') 
    else:
        print('Found Csound... ')
        sys.stdout.write(RED)
        print("Warning: Please make sure you are running the most up to date version of Csound. If you uninstall Csound")
        print("and run this script again, it will automatically build and install the latest version of Csound")
        sys.stdout.write(RESET)
        print('')

sys.stdout.write(RESET)
print('')      
os.chdir(rootDir)


sys.stdout.write(GREEN)

if not os.path.exists("JUCE/modules"):
    print('Cloning JUCE and applying patches....')
    os.system('git submodule init')
    os.system('git submodule update')
    os.system('git apply ./patches/StandaloneWrapper.patch')
    os.system('git apply ./patches/AUWrapper.patch')
    os.system('git apply ./patches/UtilityWrapper.patch')
    os.system('git apply ./patches/VST2Wrapper.patch')
    os.system('git apply ./patches/VST3Wrapper.patch')
    if platform.system() == "Linux":
         os.system('git apply ./patches/LinuxFile.patch')
    newFileText = ""
    with open(rootDir+"/JUCE/extras/Build/juceaide/CMakeLists.txt", "rt") as cmakeFile:
        for line in cmakeFile:
            if "juce_add_console_app(juceaide)" in line:
                line += "\n\ttarget_compile_features(juceaide PRIVATE cxx_std_17)\n"
            newFileText += line    

    
    with open(rootDir+"/JUCE/extras/Build/juceaide/CMakeLists.txt", "w") as cmakeFile:
        cmakeFile.write(newFileText)



if platform.system() == "Darwin" and os.path.exists("Cabbage.app"):
    os.system('rm -rf Cabbage.app')

if platform.system() == "Windows":
    print("##vso[task.setvariable variable=CABBAGE_VERSION_NUMBER]" + getVersionNumber())
    
for project in projects:
    os.chdir(rootDir)
    if os.path.exists(rootDir+"/build"):
        shutil.rmtree(rootDir+"/build")
    os.mkdir(rootDir+'/build')
    os.chdir(rootDir+'/build')

    sys.stdout.write(CYAN)
    print("===========================================================")
    print(" Running CMake for "+project + '('+configType+')')
    print("===========================================================")   
    sys.stdout.write(RESET)
    print('')

    if platform.system() == "Darwin" and 'arm64' in platformArch: 
        os.system('cmake -DCMAKE_BUILD_TYPE='+configType+' -DCMAKE_OSX_ARCHITECTURES="arm64;x86_64" -G"'+generator+'" .. -DPROJECT_NAME="'+project+'" -DJucePlugin_Manufacturer="'+manufacturer+'" -DJucePlugin_ManufacturerCode='+manufacturerCode+' -DJucePlugin_Desc="'+pluginDescription+'" -DCabbagePro='+str(buildPro))
    elif platform.system() == "Darwin":
        os.system('cmake -DCMAKE_BUILD_TYPE='+configType+' -DCMAKE_OSX_ARCHITECTURES="x86_64" -G"'+generator+'" .. -DPROJECT_NAME="'+project+'" -DJucePlugin_Manufacturer="'+manufacturer+'" -DJucePlugin_ManufacturerCode='+manufacturerCode+' -DJucePlugin_Desc="'+pluginDescription+'" -DCabbagePro='+str(buildPro))
    elif platform.system() == "Linux":        
        os.system('cmake -DCMAKE_BUILD_TYPE='+configType+' -DCMAKE_OSX_ARCHITECTURES="x86_64" -G "'+generator+'" .. -DPROJECT_NAME="'+project+'" -DJucePlugin_Manufacturer="'+manufacturer+'" -DJucePlugin_ManufacturerCode='+manufacturerCode+' -DJucePlugin_Desc="'+pluginDescription+'" -DCabbagePro='+str(buildPro))
    elif platform.system() == "Windows": 
        os.system('cmake -DCMAKE_BUILD_TYPE='+configType+'  -G"'+generator+'" .. -DPROJECT_NAME="'+project+'" -DJucePlugin_Manufacturer="'+manufacturer+'" -DJucePlugin_ManufacturerCode='+manufacturerCode+' -DJucePlugin_Desc="'+pluginDescription+'" -DCabbagePro='+str(buildPro))
    
    if executeBuild == True:
        sys.stdout.write(CYAN)
        print("===========================================================")
        print(" Building "+project + '('+configType+')')
        print("===========================================================")   
        sys.stdout.write(RESET)
        print('')
        if platform.system() != "Linux":
            os.system('cmake --build . --config '+configType)
        else:
            os.system('cmake --build . -j8 --config '+configType)
        # post build steps - take binaries and place them outside build folder
        sys.stdout.write(CYAN)
        print("===========================================================")
        print(" Copying binaries before removing build folder")
        print("===========================================================")
        sys.stdout.write(RESET)
        print('')
        if platform.system() == "Darwin": 
            if project == "Cabbage":
                os.system('cp -Rf Cabbage_artefacts/'+configType+'/Cabbage.app '+rootDir+'/Cabbage.app')
                os.system('cp -Rf ../Examples '+rootDir+'/Cabbage.app/Contents/Examples')
                os.system('cp -Rf ../Themes '+rootDir+'/Cabbage.app/Contents/Themes')
                os.system('cp -Rf ../CabbageManual '+rootDir+'/Cabbage.app/Contents/CabbageManual')
                os.system('cp -Rf ../CabbageRack '+rootDir+'/Cabbage.app/Contents/CabbageRack')
                os.system('cp ../fmod_csound_fx.dylib '+rootDir+'/Cabbage.app/Contents/fmod_csound_fx.dylib')
                os.system('cp ../fmod_csound.dylib '+rootDir+'/Cabbage.app/Contents/fmod_csound.dylib')

            elif "Effect" in project or "Synth" in project:
                newProjectName = project.replace("CabbagePlugin", pluginDescription)
                # os.system('mv '+rootDir+'/build/'+project+'_artefacts/'+configType+'/VST/'+project+'.vst/Contents/MacOS/'+project+' ' +rootDir+'/build/'+project+'_artefacts/'+configType+'/VST/'+project+'.vst/Contents/MacOS/'+newProjectName) 
                # os.system('mv '+rootDir+'/build/'+project+'_artefacts/'+configType+'/VST3/'+project+'.vst3/Contents/MacOS/'+project+' '+rootDir+'/build/'+project+'_artefacts/'+configType+'/VST3/'+project+'.vst3/Contents/MacOS/'+newProjectName) 
                # os.system('mv '+rootDir+'/build/'+project+'_artefacts/'+configType+'/AU/'+project+'.component/Contents/MacOS/'+project+' '+rootDir+'/build/'+project+'_artefacts/'+configType+'/AU/'+project+'.component/Contents/MacOS/'+newProjectName) 
                
                os.system('cp -Rf '+project+'_artefacts/'+configType+'/VST/'+project+'.vst ' +rootDir+'/Cabbage.app/Contents/'+newProjectName+'.vst')
                os.system('cp -Rf '+project+'_artefacts/'+configType+'/VST3/'+project+'.vst3 ' +rootDir+'/Cabbage.app/Contents/'+newProjectName+'.vst3')
                os.system('cp -Rf '+project+'_artefacts/'+configType+'/AU/'+project+'.component ' +rootDir+'/Cabbage.app/Contents/'+newProjectName+'.component')
                if "Synth" in project:
                    if "Cabbage" in newProjectName:
                        os.system('cp -Rf '+project+'_artefacts/'+configType+'/Standalone/'+project+'.app ' +rootDir+'/Cabbage.app/Contents/CabbagePlugin.app')
                    else:
                        os.system('cp -Rf '+project+'_artefacts/'+configType+'/Standalone/'+project+'.app ' +rootDir+'/Cabbage.app/Contents/'+newProjectName+'.app')

        if platform.system() == "Windows":
            if project == "Cabbage":
                os.system('cp -Rf Cabbage_artefacts/'+configType+'/Cabbage.exe '+rootDir+'/CabbageInstall/Cabbage.exe')
                os.system('cp -Rf ../Examples '+rootDir+'/CabbageInstall/Examples')
                os.system('cp -Rf ../Themes '+rootDir+'/CabbageInstall/Themes')
                os.system('cp -Rf ../Icons '+rootDir+'/CabbageInstall/Icons')
            elif "Effect" in project or "Synth" in project:
                newProjectName = project.replace("CabbagePlugin", pluginDescription)
                os.system('cp -Rf '+rootDir+'/build/'+project+'_artefacts/'+configType+'/VST/'+project+'.dll ' +rootDir+'/CabbageInstall/'+newProjectName+'.dll')
                os.system('cp -Rf '+rootDir+'/build/'+project+'_artefacts/'+configType+'/VST3/'+project+'.vst3/Contents/x86_64-win/'+project+'.vst3 ' +rootDir+'/CabbageInstall/'+newProjectName+'.vst3')
                if "Synth" in project:
                    os.system('cp -Rf '+rootDir+'/build/'+project+'_artefacts/'+configType+'/Standalone/'+project+'.exe ' +rootDir+'/CabbageInstall/'+pluginDescription+'.exe')

        if platform.system() == "Linux":
            if project == "Cabbage":
                os.system('cp Cabbage_artefacts/'+configType+'/Cabbage '+rootDir+'/CabbageInstall/bin/Cabbage')
                os.system('cp '+rootDir+'/Images/cabbage.png '+rootDir+'/CabbageInstall/images/cabbage.png')
                os.system('cp -rf ../Examples '+rootDir+'/CabbageInstall/Examples')
                os.system('cp -rf ../Themes '+rootDir+'/CabbageInstall/Themes')
                os.system('cp -rf ../Icons '+rootDir+'/CabbageInstall/Icons')
            elif "Effect" in project or "Synth" in project:
                newProjectName = project.replace("CabbagePlugin", pluginDescription)
                os.system('cp '+rootDir+'/build/'+project+'_artefacts/'+configType+'/VST/lib'+project+'.so ' +rootDir+'/CabbageInstall/bin/'+newProjectName+'.so')
                os.system('cp -rf '+rootDir+'/build/'+project+'_artefacts/'+configType+'/VST3/'+project+'.vst3 ' +rootDir+'/CabbageInstall/bin/'+newProjectName+'.vst3')
                if "Synth" in project:
                    os.system('cp '+rootDir+'/build/'+project+'_artefacts/'+configType+'/Standalone/'+project+' ' +rootDir+'/CabbageInstall/bin/'+pluginDescription)

os.chdir('..')

# If local release is specified, then package things from the current dir
if "Minimal" not in packageType and executeBuild == True:
    if platform.system() == "Darwin":
        os.chdir(rootDir+'/Installers/MacOS') 
        os.system('sed -i "" -e "s|SOURCE_PATH|'+rootDir+'|" Installer.pkgproj')
        os.system('packagesbuild Installer.pkgproj')
        print('======+++++++++++++++++++++===========')
        os.system('ls '+rootDir+'/Installers/MacOS')
        os.system('ls')
        os.system('ls '+rootDir+'/Installers/MacOS/build')
        print('======+++++++++++++++++++++===========')
        if "Remote" in packageType:
            os.system('cp -rf '+rootDir+'/Installers/MacOS/build/Cabbage.pkg '+stagingDir+'/Cabbage.pkg')
            os.system('mv '+stagingDir+'/Cabbage.pkg '+stagingDir+'/CabbageOSXInstaller-'+getVersionNumber()+'.pkg')

    if platform.system() == "Windows":
        os.chdir(rootDir+'/Installers/Windows') 
        os.system('set PATH=%PATH%;"C:\\Program Files (x86)\\Inno Setup 5"')
        os.system('iscc Installer.iss')
        os.system('cp '+rootDir+'/Installers/Windows/Output/Cabbage64Setup.exe '+stagingDir+'/Cabbage64Setup-'+getVersionNumber()+'.exe')

    if platform.system() == "Linux":
        os.chdir(rootDir)
        os.system('cp ./Installers/Linux/installCabbage.sh ' +rootDir+'/CabbageInstall')

        installerText = ''
        with open(rootDir+'/CabbageInstall/installCabbage.sh', "rt") as installer:
            installerText = installer.read().replace('build_path="../../CabbageInstall"', 'build_path="."')
        with open(rootDir+'/CabbageInstall/installCabbage.sh', "w") as installer:
            installer.write(installerText)

        shutil.make_archive(rootDir+'/Installers/Linux/CabbageLinux-'+getVersionNumber(), 'zip', rootDir+'/CabbageInstall') 
        if "Remote" in packageType:
            os.system('mv '+rootDir+'/Installers/Linux/CabbageLinux-'+getVersionNumber()+'.zip '+stagingDir+'/CabbageLinux-'+getVersionNumber()+'.zip')
    