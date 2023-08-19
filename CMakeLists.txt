cmake_minimum_required(VERSION 3.15)

# Having used JUCE 5.4.7 for so long, this project uses quite a function that have been mark
# few deprecated in JUCE 6. Until I have time to fix them all, I'm disabling the warnings.. 
set( CMAKE_WARN_DEPRECATED OFF )
set(CMAKE_WARN_DEPRECATED OFF CACHE BOOL "" FORCE)

if(NOT DEFINED PROJECT_NAME)
    set(PROJECT_NAME "Cabbage")
endif()

if(NOT DEFINED CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()


if(NOT DEFINED BUILD_VERSION)
	set(BUILD_VERSION 2.9.85)
endif()

project(${PROJECT_NAME} VERSION ${BUILD_VERSION})

#Adds all the module sources so they appear correctly in the IDE
set_property(GLOBAL PROPERTY USE_FOLDERS YES)
option(JUCE_ENABLE_MODULE_SOURCE_GROUPS "Enable Module Source Groups" ON)

add_subdirectory(JUCE)                    # If you've put JUCE in a subdirectory called JUCE

if(NOT DEFINED JucePlugin_Manufacturer)
    set(JucePlugin_Manufacturer "CabbageAudio")
    set(CabbageManufacturer "CabbageAudio")
else()
    set(CabbageManufacturer ${JucePlugin_Manufacturer})
endif()

if(NOT DEFINED BuildStatic)
    set(BuildStatic 0)
endif()

if(NOT DEFINED CabbagePro)
    set(CabbagePro 0)
endif()

if(NOT DEFINED JucePlugin_ManufacturerCode)
    set(JucePlugin_ManufacturerCode CABB)
endif()


if(NOT DEFINED JucePlugin_Desc)
    set(JucePlugin_Desc "CabbagePlugin")
    set(PluginDesc "CabbagePlugin")
else()
    set(PluginDesc ${JucePlugin_Desc})
endif()


# ==== Find out what kind of project this is
string(FIND ${PROJECT_NAME} "Synth" synthProject)
string(FIND ${PROJECT_NAME} "Effect" effectProject)
string(FIND ${PROJECT_NAME} "MidiEffect" midiEffectProject)

IF(${synthProject} GREATER 0)
    set(PLUGIN_IS_A_SYNTH ON)
    set(IS_IDE_Build 0)
ELSEIF(${effectProject} GREATER 0)
    set(PLUGIN_IS_A_SYNTH OFF)
    set(IS_IDE_Build 0)
ELSE()
    set(IS_IDE_Build 1)
ENDIF()

macro(SetIfSynth var value1 value2)
    if(${PLUGIN_IS_A_SYNTH})
        set(${var} "${value1}")
    else()
        set(${var} "${value2}")
    endif()
endmacro()

# these are needed for both hosting and creating plugins
if(APPLE)
juce_set_vst2_sdk_path("$ENV{HOME}/SDKs/VST_SDK/VST3_SDK")
elseif(MSVC)
juce_set_vst2_sdk_path("C:/SDKs/VST_SDK/VST3_SDK")
else()
juce_set_vst2_sdk_path("$ENV{HOME}/SDKs/VST_SDK/VST3_SDK")
endif()

if ("${PROJECT_NAME}" STREQUAL "CLIConverter")
juce_add_console_app(${PROJECT_NAME}
    PRODUCT_NAME ${PROJECT_NAME})     # The name of the final executable, which can differ from the target name

elseif("${PROJECT_NAME}" STREQUAL "Cabbage")
    juce_add_gui_app(${PROJECT_NAME}
        ICON_BIG ${CMAKE_CURRENT_SOURCE_DIR}/Images/cabbage.png              
        ICON_SMALL ${CMAKE_CURRENT_SOURCE_DIR}/Images/cabbage.png 
        NEEDS_WEB_BROWSER TRUE
        HARDENED_RUNTIME_ENABLED TRUE
        PLUGINHOST_AU TRUE
        HARDENED_RUNTIME_OPTIONS "com.apple.security.device.audio-input" "com.apple.security.cs.disable-library-validation" 
        # DOCUMENT_EXTENSIONS ".csd"          # Specify file extensions that should be associated with this app
        COMPANY_NAME ${JucePlugin_Manufacturer}
        MICROPHONE_PERMISSION_ENABLED TRUE
        MICROPHONE_PERMISSION_TEXT "Cabbage needs access to your audio interface" 
        PRODUCT_NAME ${PROJECT_NAME})     # The name of the final executable, which can differ from the target name
else()
# ============== This part of the CMake setup deals with plugins only.....
    set(PLUGIN_NEEDS_OUTPUT PLUGIN_IS_A_SYNTH)
    SetIfSynth(PLUGIN_VST2_CATEGORY "kPlugCategSynth" "kPlugCategEffect")
    SetIfSynth(PLUGIN_AU_MAIN_TYPE "aumi" "aumf")
    SetIfSynth(PLUGIN_VST3_CATEGORIES "Instrument|Synth" "Fx")
    SetIfSynth(SYNTH_OR_EFFECT "Synth" "Effect")

    if(${PLUGIN_IS_A_SYNTH})
        set(MIDI_OUTPUT True)
    else()
        set(MIDI_OUTPUT False)
    endif()

    IF(${midiEffectProject} GREATER 0)
        set(PLUGIN_AU_MAIN_TYPE "kAudioUnitType_MIDIProcessor")
        set(MIDI_EFFECT True)
    else()
        set(MIDI_EFFECT False)  
    endif()

    juce_add_plugin(${PROJECT_NAME}
        PLUGIN_NAME ${PROJECT_NAME}
        FORMATS VST VST3 Standalone AU
        PLUGIN_MANUFACTURER_CODE ${JucePlugin_ManufacturerCode}
        PLUGIN_CODE RORY
        DESCRIPTION ${JucePlugin_Desc}
        NEEDS_MIDI_INPUT TRUE
        NEEDS_MIDI_OUTPUT TRUE
        IS_SYNTH ${PLUGIN_IS_A_SYNTH}
        BUNDLE_ID com.CabbageAudio.plugin
        IS_MIDI_EFFECT ${MIDI_EFFECT}
        EDITOR_WANTS_KEYBOARD_FOCUS TRUE
        VST_NUM_MIDI_INS 16
        VST_NUM_MIDI_OUTS 16
        VST2_CATEGORY "${PLUGIN_VST2_CATEGORY}"
        VST3_CATEGORIES "${PLUGIN_VST3_CATEGORIES}"
        AU_EXPORT_PREFIX "${PROJECT_NAME}AU"
        AU_SANDBOX_SAFE TRUE
        COPY_PLUGIN_AFTER_BUILD FALSE
        COMPANY_NAME ${JucePlugin_Manufacturer}
        HARDENED_RUNTIME_ENABLED TRUE
        HARDENED_RUNTIME_OPTIONS "com.apple.security.device.audio-input"
        PRODUCT_NAME "${PROJECT_NAME}")   
endif()

juce_generate_juce_header(${PROJECT_NAME})

if(MSVC)
    # C4996 are warnings caused by deprecated function
    # C26495 are warnings caused by JUCE classes
    add_definitions( "/wd26495 /wd4996" )
endif()


add_compile_options ( -Wno-reorder -Wno-deprecated -Wno-deprecated-declarations)

target_compile_features("${PROJECT_NAME}" PRIVATE cxx_std_17)

set(COMMON_SOURCES
Source/Opcodes/CabbageIdentifierOpcodes.cpp
Source/Opcodes/CabbageIdentifierOpcodes.h
Source/Opcodes/opcodes.hpp
Source/Audio/Plugins/CabbageCsoundBreakpointData.h
Source/Audio/Plugins/CabbagePluginEditor.cpp
Source/Audio/Plugins/CabbagePluginEditor.h
Source/Audio/Plugins/CabbagePluginProcessor.cpp
Source/Audio/Plugins/CabbagePluginProcessor.h
Source/Audio/Plugins/CsoundPluginEditor.cpp
Source/Audio/Plugins/CsoundPluginEditor.h
Source/Audio/Plugins/CsoundPluginProcessor.cpp
Source/Audio/Plugins/CsoundPluginProcessor.h
Source/Audio/Plugins/GenericCabbageEditor.cpp
Source/Audio/Plugins/GenericCabbageEditor.h
Source/Audio/Plugins/GenericCabbagePluginProcessor.cpp
Source/Audio/Plugins/GenericCabbagePluginProcessor.h
Source/BinaryData/CabbageBinaryData.cpp
Source/BinaryData/CabbageBinaryData.h
Source/LookAndFeel/CabbageGenericPluginLookAndFeel.cpp
Source/LookAndFeel/CabbageGenericPluginLookAndFeel.h
Source/LookAndFeel/CabbageLookAndFeel2.cpp
Source/LookAndFeel/CabbageLookAndFeel2.h
Source/LookAndFeel/FlatButtonLookAndFeel.cpp
Source/LookAndFeel/FlatButtonLookAndFeel.h
Source/LookAndFeel/PropertyPanelLookAndFeel.cpp
Source/LookAndFeel/PropertyPanelLookAndFeel.h
Source/Utilities/CabbageColourProperty.cpp
Source/Utilities/CabbageColourProperty.h
Source/Utilities/CabbageStrings.h
Source/Utilities/CabbageUtilities.h
Source/Widgets/Legacy/FrequencyRangeDisplayComponent.h
Source/Widgets/Legacy/Soundfiler.cpp
Source/Widgets/Legacy/Soundfiler.h
Source/Widgets/Legacy/TableManager.cpp
Source/Widgets/Legacy/TableManager.h
Source/Widgets/CabbageForm.h
Source/Widgets/CabbageForm.cpp
Source/Widgets/CabbageForm.cpp
Source/Widgets/CabbageForm.h
Source/Widgets/CabbagePath.cpp
Source/Widgets/CabbagePath.h
Source/Widgets/CabbageOptionButton.cpp
Source/Widgets/CabbageOptionButton.h
Source/Widgets/CabbageRackWidgets.cpp
Source/Widgets/CabbageRackWidgets.h
Source/Widgets/CabbageKeyboardDisplay.cpp
Source/Widgets/CabbageKeyboardDisplay.h
Source/Widgets/CabbageListBox.h
Source/Widgets/CabbageListBox.cpp
Source/Widgets/CabbageWidgetDataTextMethods.cpp
Source/Widgets/CabbageButton.cpp
Source/Widgets/CabbageButton.h
Source/Widgets/CabbageCheckbox.cpp
Source/Widgets/CabbageCheckbox.h
Source/Widgets/CabbageComboBox.cpp
Source/Widgets/CabbageComboBox.h
Source/Widgets/CabbageCsoundConsole.cpp
Source/Widgets/CabbageNumberSlider.cpp
Source/Widgets/CabbageNumberSlider.h
Source/Widgets/CabbageCsoundConsole.h
Source/Widgets/CabbageCustomWidgets.cpp
Source/Widgets/CabbageCustomWidgets.h
Source/Widgets/CabbageEncoder.cpp
Source/Widgets/CabbageEncoder.h
Source/Widgets/CabbageFileButton.cpp
Source/Widgets/CabbageFileButton.h
Source/Widgets/CabbagePresetButton.cpp
Source/Widgets/CabbagePresetButton.h
Source/Widgets/CabbageGenTable.cpp
Source/Widgets/CabbageGenTable.h
Source/Widgets/CabbageGroupBox.cpp
Source/Widgets/CabbageGroupBox.h
Source/Widgets/CabbageImage.cpp
Source/Widgets/CabbageImage.h
Source/Widgets/CabbageInfoButton.cpp
Source/Widgets/CabbageInfoButton.h
Source/Widgets/CabbageKeyboard.cpp
Source/Widgets/CabbageKeyboard.h
Source/Widgets/CabbageLabel.cpp
Source/Widgets/CabbageLabel.h
Source/Widgets/CabbageUnlockButton.cpp
Source/Widgets/CabbageUnlockButton.h
Source/Widgets/CabbageRangeSlider.cpp
Source/Widgets/CabbageRangeSlider.h
Source/Widgets/CabbageSignalDisplay.cpp
Source/Widgets/CabbageSignalDisplay.h
Source/Widgets/CabbageSlider.cpp
Source/Widgets/CabbageSlider.h
Source/Widgets/CabbageSoundfiler.cpp
Source/Widgets/CabbageSoundfiler.h
Source/Widgets/CabbageEventSequencer.cpp
Source/Widgets/CabbageEventSequencer.h
Source/Widgets/CabbageTextBox.cpp
Source/Widgets/CabbageTextBox.h
Source/Widgets/CabbageTextEditor.cpp
Source/Widgets/CabbageTextEditor.h
Source/Widgets/CabbageWidgetBase.cpp
Source/Widgets/CabbageWidgetBase.h
Source/Widgets/CabbageWidgetData.cpp
Source/Widgets/CabbageWidgetData.h
Source/Widgets/CabbageWidgetDataInitMethods.cpp
Source/Widgets/CabbageXYPad.cpp
Source/Widgets/CabbageXYPad.h
Source/CabbageCommonHeaders.h
Source/CabbageIds.h
Source/Opcodes/CabbageMidiOpcodes.cpp
Source/Opcodes/CabbageMidiOpcodes.h)

set(BATCH_CONVERTER_SOURCES
Source/Converter/Main.cpp
Source/Utilities/CabbageExportPlugin.h
Source/Utilities/CabbageExportPlugin.cpp
Source/CabbageCommonHeaders.h
Source/Widgets/CabbageWidgetBase.cpp
Source/Widgets/CabbageWidgetBase.h
Source/Widgets/CabbageWidgetData.cpp
Source/Widgets/CabbageWidgetData.h
Source/Widgets/CabbageWidgetDataInitMethods.cpp
Source/LookAndFeel/CabbageIDELookAndFeel.cpp
Source/LookAndFeel/CabbageIDELookAndFeel.h
Source/Settings/CabbageSettings.cpp
Source/Settings/CabbageSettings.h
Source/BinaryData/CabbageBinaryData.cpp
Source/BinaryData/CabbageBinaryData.h
)

set(IDE_SOURCES
    Source/Application/FileTab.h
    Source/Application/FileTab.cpp
    Source/Application/CabbageMainComponent.cpp
    Source/Application/CabbageMainComponent.h
    Source/Application/CabbageDocumentWindow.cpp
    Source/Application/CabbageDocumentWindow.h
    Source/Application/CabbageToolbarFactory.cpp
    Source/Application/CabbageToolbarFactory.h
    Source/Audio/Filters/FilterGraph.cpp
    Source/Audio/Filters/FilterGraph.h
    Source/Audio/Filters/FilterIOConfiguration.cpp
    Source/Audio/Filters/FilterIOConfiguration.h
    Source/Audio/Filters/InternalFilters.cpp
    Source/Audio/Filters/InternalFilters.h
    Source/Audio/Plugins/CabbageInternalPluginFormat.cpp
    Source/Audio/Plugins/CabbageInternalPluginFormat.h
    Source/Audio/UI/CabbageTransportComponent.cpp
    Source/Audio/UI/CabbageTransportComponent.h
    Source/Audio/UI/GraphEditorPanel.cpp
    Source/Audio/UI/GraphEditorPanel.h
    Source/Audio/UI/IOConfigurationWindow.cpp
    Source/Audio/UI/IOConfigurationWindow.h
    Source/Audio/UI/PluginWindow.h
    Source/CodeEditor/CabbageCodeEditor.cpp
    Source/CodeEditor/CabbageCodeEditor.h
    Source/CodeEditor/CabbageEditorContainer.cpp
    Source/CodeEditor/CabbageEditorContainer.h
    Source/CodeEditor/CabbageOutputConsole.h
    Source/CodeEditor/CsoundTokeniser.h
    Source/CodeEditor/JavascriptCodeTokeniser.cpp
    Source/CodeEditor/JavascriptCodeTokeniser.h
    Source/GUIEditor/CabbagePropertiesPanel.cpp
    Source/GUIEditor/CabbagePropertiesPanel.h
    Source/GUIEditor/ComponentLayoutEditor.cpp
    Source/GUIEditor/ComponentLayoutEditor.h
    Source/GUIEditor/ComponentOverlay.cpp
    Source/GUIEditor/ComponentOverlay.h
    Source/LookAndFeel/CabbageIDELookAndFeel.cpp
    Source/LookAndFeel/CabbageIDELookAndFeel.h
    Source/Settings/CabbageSettings.cpp
    Source/Settings/CabbageSettings.h
    Source/Settings/CabbageSettingsWindow.cpp
    Source/Settings/CabbageSettingsWindow.h
    Source/Utilities/CabbagePluginList.cpp
    Source/Utilities/CabbagePluginList.h
    Source/Utilities/CabbageExportPlugin.h
    Source/Utilities/CabbageExportPlugin.cpp
    Source/Utilities/CabbageFilePropertyComponent.h
    Source/Utilities/CabbageNewProjectWindow.cpp
    Source/Utilities/CabbageNewProjectWindow.h
    Source/Utilities/CabbageSSHFileBrowser.cpp
    Source/Utilities/CabbageSSHFileBrowser.h
    Source/Cabbage.cpp
    Source/Cabbage.h )
    
if ("${PROJECT_NAME}" STREQUAL "CLIConverter")
    target_sources(${PROJECT_NAME} PRIVATE  ${BATCH_CONVERTER_SOURCES} )
    source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/Source PREFIX "Cabbage Source" FILES ${BATCH_CONVERTER_SOURCES})

elseif("${PROJECT_NAME}" STREQUAL "Cabbage")
    target_sources(${PROJECT_NAME} PRIVATE ${COMMON_SOURCES} ${IDE_SOURCES} )
    source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/Source PREFIX "Cabbage Source" FILES ${COMMON_SOURCES} ${IDE_SOURCES})
else()
# ============== This part of the CMake setup deals with plugins only.....
    target_sources(${PROJECT_NAME} PRIVATE ${COMMON_SOURCES} )
    source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/Source PREFIX "Cabbage Source" FILES ${COMMON_SOURCES})
endif()

# set_target_properties(${PROJECT_NAME} PROPERTIES 
# 	XCODE_ATTRIBUTE_ENABLE_HARDENED_RUNTIME YES
#     //XCODE_ATTRIBUTE_CODE_SIGN_ENTITLEMENTS "${CMAKE_CURRENT_SOURCE_DIR}/entitlements.xcent"
# )

set(JUCE_MODULES
    juce::juce_core
    juce::juce_events
    juce::juce_graphics
    juce::juce_data_structures
    juce::juce_gui_basics
    juce::juce_gui_extra
    juce::juce_cryptography
    juce::juce_audio_basics
    juce::juce_audio_devices
    juce::juce_audio_formats
    juce::juce_audio_processors
    juce::juce_product_unlocking
    juce::juce_audio_utils
    juce::juce_audio_plugin_client
    juce::juce_opengl)

if ("${PROJECT_NAME}" STREQUAL "CLIConverter")
    target_compile_definitions(${PROJECT_NAME}
        PRIVATE
            CLIConverter=1
            PluginDesc="${PluginDesc}"
            JUCE_WEB_BROWSER=0
            JUCE_MODAL_LOOPS_PERMITTED=1
            JUCE_USE_CURL=0
            CabbagePro=${CabbagePro}
            CabbageManufacturer="${CabbageManufacturer}"
    )
    target_link_libraries(${PROJECT_NAME}
        PRIVATE
            juce::juce_core
        PUBLIC
            juce::juce_recommended_config_flags
            juce::juce_recommended_warning_flags)
elseif("${PROJECT_NAME}" STREQUAL "Cabbage")  
    target_compile_definitions(${PROJECT_NAME}
        PRIVATE
            # JUCE_WEB_BROWSER and JUCE_USE_CURL would be on by default, but you might not need them.
            JUCE_WEB_BROWSER=1  # If you remove this, add `NEEDS_WEB_BROWSER TRUE` to the `juce_add_gui_app` call
            JUCE_USE_CURL=0     # If you remove this, add `NEEDS_CURL TRUE` to the `juce_add_gui_app` call
            JUCE_APPLICATION_NAME_STRING="$<TARGET_PROPERTY:${PROJECT_NAME},JUCE_PRODUCT_NAME>"
            JUCE_APPLICATION_VERSION_STRING="$<TARGET_PROPERTY:${PROJECT_NAME},JUCE_VERSION>"
            JUCE_MODAL_LOOPS_PERMITTED=1
            CabbagePro=${CabbagePro}
            PluginDesc="${PluginDesc}"
            CabbageManufacturer="${CabbageManufacturer}"
            Use_Native_File_Browser=1
            JucePlugin_WantsMidiInput=1
            JUCE_ASIO=1
            JUCE_VST3_CAN_REPLACE_VST2=0
            JUCE_PLUGINHOST_VST3=1
            JUCE_PLUGINHOST_VST=1
            JucePlugin_ProducesMidiOutput=1
            Cabbage_IDE_Build=${IS_IDE_Build}
            JUCE_DISPLAY_SPLASH_SCREEN=0
            )
else()
    target_compile_definitions(${PROJECT_NAME}
    PRIVATE
        # JUCE_WEB_BROWSER and JUCE_USE_CURL would be on by default, but you might not need them.
        JUCE_WEB_BROWSER=1  # If you remove this, add `NEEDS_WEB_BROWSER TRUE` to the `juce_add_gui_app` call
        JUCE_USE_CURL=0     # If you remove this, add `NEEDS_CURL TRUE` to the `juce_add_gui_app` call
        JUCE_APPLICATION_NAME_STRING="$<TARGET_PROPERTY:${PROJECT_NAME},JUCE_PRODUCT_NAME>"
        JUCE_APPLICATION_VERSION_STRING="$<TARGET_PROPERTY:${PROJECT_NAME},JUCE_VERSION>"
        JUCE_MODAL_LOOPS_PERMITTED=1
        CabbagePro=${CabbagePro}
        PluginDesc="${PluginDesc}"
        CabbageManufacturer="${CabbageManufacturer}"
        Use_Native_File_Browser=1
        JUCE_VST3_CAN_REPLACE_VST2=0
        JUCE_ASIO=1
        Cabbage_IDE_Build=${IS_IDE_Build}
        JUCE_DISPLAY_SPLASH_SCREEN=0
        )
endif()

if(MSVC)
    # SET(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "C:/Program Files/Csound6_x64/include/csound")
    include_directories("C:\\Program Files\\Csound6_x64\\include\\csound" "C:\\SDKs\\ASIOSDK2.3.3\\asiosdk_2.3.3_2019-06-14\\common")
    if(CabbagePro MATCHES 1)
        # find_library(CSOUND_LIBRARY NAME libcsound64 HINTS "c:\\Program Files\\Csound6_x64\\lib\\libcsound64.lib")
        set(CSOUND_LIBRARY "c:\\Program Files\\Csound6_x64\\lib\\libcsound64.lib")
        set(EXTERNAL_LIBS "c:\\Program Files\\Csound6_x64\\lib\\sndfile.lib" "c:\\Program Files\\Csound6_x64\\lib\\opus.lib" "c:\\Program Files\\Csound6_x64\\lib\\syn123.lib" "c:\\Program Files\\Csound6_x64\\lib\\libmpghip-static.lib" "c:\\Program Files\\Csound6_x64\\lib\\out123.lib" "c:\\Program Files\\Csound6_x64\\lib\\mpg123.lib" "c:\\Program Files\\Csound6_x64\\lib\\libmp3lame-static.lib" "c:\\Program Files\\Csound6_x64\\lib\\FLAC.lib")
    else()
    find_library(CSOUND_LIBRARY NAMES csound64 HINTS "c:\\Program Files\\Csound6_x64\\lib")
    endif()
    target_link_libraries(${PROJECT_NAME} 
    PRIVATE
        ${JUCE_MODULES}
        ${CSOUND_LIBRARY}
        ${EXTERNAL_LIBS}
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)

elseif(APPLE)
    if(CabbagePro MATCHES 0)
        set(CMAKE_OSX_DEPLOYMENT_TARGET "10.12" CACHE STRING "Minimum OS X deployment version" FORCE)
        SET(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "/Library/Frameworks/CsoundLib64")
        find_library(CSOUND_LIBRARY NAMES CsoundLib64 HINTS /Library/Frameworks/CsoundLib64.framework/
        "/Library/Frameworks/CsoundLib64.framework")  
        include_directories("/Library/Frameworks/CsoundLib64.framework/Headers")
    else()
        set(CMAKE_OSX_DEPLOYMENT_TARGET "10.12" CACHE STRING "Minimum OS X deployment version" FORCE)
        SET(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "/Volumes/SSD_Drive/Mac/csoundCabbagePro/csound/include")
        set(CSOUND_LIBRARY "/Volumes/SSD_Drive/Mac/csoundCabbagePro/csound/build/libCsoundLib64.a")  
        #  set(CSOUND_LIBRARY "/Users/rwalsh/sourcecode/csound/build/libCsoundLib64.a")  
        include_directories("/Volumes/SSD_Drive/Mac/csoundCabbagePro/csound/include")
        #  include_directories("/Users/rwalsh/sourcecode/csound/include")
        set(LIB_SNDFILE "/Volumes/SSD_Drive/Mac/libsndfile/build/libsndfile.a")
        #  set(LIB_SNDFILE "/Users/rwalsh/sourcecode/libsndfile/build/libsndfile.a")
    endif()

    target_link_libraries(${PROJECT_NAME} 
    PRIVATE
        ${JUCE_MODULES}
        ${CSOUND_LIBRARY}
        ${LIB_SNDFILE}
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)
else()
    include_directories("/usr/local/include/csound")
    include_directories("/usr/include/csound")
    find_library(CSOUND_LIBRARY NAMES csound64)
    find_library(LIBSNDFILE_LIBRARY NAMES sndfile libsndfile-1 libsndfile)
    add_compile_options(-j4)
    target_link_libraries(${PROJECT_NAME} 
    PRIVATE
        ${JUCE_MODULES}
        ${CSOUND_LIBRARY}
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)
endif()
