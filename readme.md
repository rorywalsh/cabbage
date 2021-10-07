
![logo_cabbage_black_no_text](https://f.cloud.github.com/assets/5950903/1797870/31b2206e-6b15-11e3-9a9b-553c1c8edaad.png)

Cabbage. A framework for developing audio plugins and standalone instruments using the Csound programming language.  

[Homepage](http://cabbageaudio.com)

_______

## Building

Cabbage comprises of several different components. An IDE, a synth plugin and an effect plugin. Each component's build project is generated using CMake. CMake is called by the build.py script which resides in the repo's root folder. In order to run the Python script you will need to install [requests](https://pypi.org/project/requests/). You will need access to the internet the first time you run this script as it will need to download and install various SDKS.

To build the entire system run the following command from the root directory:

`python build.py --config=Release`

When run, the Python script will check for the relevant SDKs (ASIO, VST) and libraries (FMOD, CabbageRack) and download and install them if they are not found. 

If you are on Linux you may need to install some additional dependencies. You can do so by setting the `installLinuxDeps` parameter to True:

`python build.py --config=Release --installLinuxDeps=True`

If you just wish to simply generate the CMake build files for a project, and disable automatic building, use the following command:

`python build.py --config=Debug --project=CabbagePluginSynth --build=False`


_______

## Installing

If you successfully built Cabbage using the build.py script you will find platform installers in the `Installers` folder. Simply double click teh Windows or MacOS installers to install locally. If you are on Linux, cd to the `Installers/Linux` directory and run the following:

`chmod +x installCabbage.sh`
`./installCabbage.sh`

If you wish to uninstall Cabbage on Linux run the `uninstallCabbage.sh` script. 

_______

A full overview of the options provided by the build script can be seen here:

```
usage: build.py [-h] [--config CONFIG] [--project PROJECT]
                [--packageType PACKAGETYPE] [--build BUILD]
                [--manufacturer MANUFACTURER]
                [--pluginDescription PLUGINDESCRIPTION]
                [--manufacturerCode MANUFACTURERCODE]
                [--installLinuxDeps INSTALLLINUXDEPS] [--license LICENSE]

optional arguments:
  -h, --help            show this help message and exit
  --config CONFIG       Debug/Release
  --project PROJECT     Cabbage, CabbagePluginEffect, CabagePluginSynth
  --packageType PACKAGETYPE
                        "Local", "Remote", "Minimal"
  --build BUILD         Set to "False" to disable building, in which case
                        CMake will just generate the respective project in the
                        build folder. If you do not specify a project, this
                        will by default generate project files for the main
                        Cabbage application.
  --manufacturer MANUFACTURER
                        "CabbageAudio"
  --pluginDescription PLUGINDESCRIPTION
                        "CabbagePlugin"
  --manufacturerCode MANUFACTURERCODE
                        A unique 4 character ID with an uppercase first
                        character, i.e, "Cabb"
  --installLinuxDeps INSTALLLINUXDEPS
                        Set to False, set to True if you wish to download the
                        following dependencies needed to build JUCE
                        applications on Linux - libfreetype6-dev libx11-dev
                        libstdc++6 libc++-dev libxinerama-dev libxrandr-dev
                        libxcursor-dev libxcomposite-dev mesa-common-dev
                        libasound2-dev freeglut3-dev libcurl4-gnutls-dev
                        libasound2-dev libsndfile1 libsndfile-dev libjack-dev
                        flex bison libwebkit2gtk-4.0-37 libwebkit2gtk-4.0-dev
  --license LICENSE     GPL by default
```