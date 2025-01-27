
Cabbage. A framework for developing audio plugins and standalone instruments using the Csound programming language.  

[Homepage](http://cabbageaudio.com)

_______

## Building

Cabbage comprises of several different components. There is the main Cabbage IDE, as well as synth and effect plugin interface. Each component's build project is generated using CMake, which is in turned called by the `build.py` script which resides in the Cabbage root folder. In order to run the build script you will need to install [requests](https://pypi.org/project/requests/). You will need access to the internet the first time you run this script as it will need to download and install various SDKs, as well as some additional auxiliary interfaces such as CabbageRack and CsoundFMOD. 

By default, Cabbage will try building with Visual Studio 2019 on Windows, XCode on macOS, and GCC on Linux. You obviously need to have those compilers installed before attempting to build on your chosen platform.

To build the entire Cabbage framework (on any platform) run the following command from the root directory:

`python build.py --config=Release`

When run, the Python script will check for the relevant libraries and SDKs, and will download and install them if they are not found. If you just wish to simply generate the CMake build files for a project, and disable automatic building, use the following command:

`python build.py --config=Debug --project=CabbagePluginSynth --build=False`

This will generate the relevant build files in the `build` directory, but will not trigger a build to take place.

### Note for Linux users

If you are on Linux you may need to install some additional dependencies. You can do so by setting the `installLinuxDeps` parameter to True. :

`python build.py --config=Release --installLinuxDeps=True`

You only have to do this once. You can drop the `--installLinuxDeps=True` from subsequent calls. Note that the CabbageRack interface that gets bundled with Cabbage when you run build.py was built on Ubuntu 18.04. If it does not work you will need to build it yourself. You can then drop the resulted CabbageRack folder into the `CabbageInstall` directory before installing.

The build script will look for the `csound` executable in `/usr/bin` and `/usr/local/bin`; if it finds this it will assume you have the corresponding development files installed.
(For Debian and Ubuntu, you can install the `libcsnd-dev` package.) If `csound` is not found, the latest Csound 6 will be built from source and installed into `/usr/local/bin`.

_______

## Installing

If you successfully built Cabbage using the build.py script you will find platform installers in the `Installers` folder. Simply double click teh Windows or MacOS installers to install locally. If you are on Linux, cd to the `Installers/Linux` directory and run the following:

`chmod +x installCabbage.sh`
`./installCabbage.sh`

If you wish to uninstall Cabbage on Linux run the `uninstallCabbage.sh` script. 

_______

A full overview of the options provided by the build.py script are shown below:

```
usage: build.py [-h] [--config CONFIG] [--project PROJECT]
                [--packageType PACKAGETYPE] [--disable-feature]
                [--build BUILD] [--manufacturer MANUFACTURER]
                [--pluginDescription PLUGINDESCRIPTION]
                [--manufacturerCode MANUFACTURERCODE]
                [--installLinuxDeps INSTALLLINUXDEPS] [--license LICENSE]

optional arguments:
  -h, --help            show this help message and exit
  --config CONFIG       Debug/Release
  --project PROJECT     Cabbage, CabbagePluginEffect, CabagePluginSynth
  --packageType PACKAGETYPE
                        "Local", "Remote", "Minimal"
  --disable-feature
  --build BUILD         Set to "False" to disable building, in which case
                        CMake will just generate the respective project in the
                        build folder. You must specify a project if you
                        disable building.
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
