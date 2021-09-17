::C:\\Users\\rory\\Documents\\sourcecode\\JUCE\\extras\\Projucer\\Builds\\VisualStudio2017\\x64\\Debug\\Projucer.exe
SET PROJUCER="C:\\Users\\rory\\sourcecode\\JUCE\\extras\\Projucer\\Builds\\VisualStudio2019\\x64\\Debug\\App\\Projucer.exe"

REM "BUILDING THE COMMAND LI FOR CSOUND"
%PROJUCER% --resave ../../CabbageCsoundCLI.jucer
msbuild CabbageCsoundCLI.sln /p:Configuration=Release /property:Platform=x64 /m
copy ".\x64\Release\ConsoleApp\CabbageCsoundCLI.exe" ".\x64\CabbageCsoundCLI.exe"

REM "BUILDING THE MAIN APP"
%PROJUCER% --resave ../../CabbageIDE.jucer
cd ../../
git apply ./patches/*.patch
cd Builds\VisualStudio2019
msbuild Cabbage.sln /p:Configuration=Release /property:Platform=x64 /m
copy ".\x64\Release\App\Cabbage.exe" ".\x64\Cabbage.exe"


REM "BUILDING EFFECT PLUGIN"
%PROJUCER% --resave ../../CabbagePlugin.jucer
cd ../../
git apply ./patches/*.patch
cd Builds\VisualStudio2019
msbuild CabbagePlugin.sln /p:Configuration=Release /property:Platform=x64 /m
copy ".\x64\Release\Standalone Plugin\CabbagePlugin.exe" ".\x64\CabbagePlugin.exe"
copy ".\x64\Release\VST\CabbagePlugin.dll" ".\x64\CabbagePluginEffect.dll"
copy ".\x64\Release\VST3\CabbagePlugin.vst3" ".\x64\CabbagePluginEffect.vst3"


REM "BUILDING VST PLUGIN"
%PROJUCER% --resave ../../CabbagePluginSynth.jucer
cd ../../
git apply ./patches/*.patch
cd Builds\VisualStudio2019
msbuild CabbagePlugin.sln /p:Configuration=Release /property:Platform=x64 /m

copy ".\x64\Release\VST\CabbagePlugin.dll" ".\x64\CabbagePluginSynth.dll"
copy ".\x64\Release\VST3\CabbagePlugin.vst3" ".\x64\CabbagePluginSynth.vst3"

@REM REM "BUILDING CABBAGE LITE"
@REM %PROJUCER% --resave ../../CabbageLite.jucer
@REM cd ../../
@REM git apply ./patches/*.patch
@REM cd Builds\VisualStudio2019
@REM msbuild CabbageLite.sln /p:Configuration=Release /property:Platform=x64 /m
@REM copy ".\x64\Release\App\CabbageLite.exe" ".\x64\CabbageLite.exe"


for /f %%i in ('%PROJUCER% --get-version ../../CabbageIDE.jucer') do set EXECUTABLE=Cabbage-%%i.exe
ren ".\x64\Cabbage.exe" %EXECUTABLE%
