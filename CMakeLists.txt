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
	set(BUILD_VERSION 2.8.14)
endif()

project(${PROJECT_NAME} VERSION ${BUILD_VERSION})
add_subdirectory(JUCE)                    # If you've put JUCE in a subdirectory called JUCE

if(NOT DEFINED JucePlugin_Manufacturer)
    set(JucePlugin_Manufacturer "CabbageAudio")
endif()

if(NOT DEFINED CabbagePro)
    set(CabbagePro 0)
endif()

if(NOT DEFINED JucePlugin_ManufacturerCode)
    set(JucePlugin_ManufacturerCode Cabb)
endif()

if(NOT DEFINED JucePlugin_Desc)
    set(JucePlugin_Desc "CabbagePlugin")
endif()


# ==== Find out what kind of project this is
string(FIND ${PROJECT_NAME} "Synth" synthProject)
string(FIND ${PROJECT_NAME} "Effect" effectProject)

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

if("${PROJECT_NAME}" STREQUAL "Cabbage")
    juce_add_gui_app(${PROJECT_NAME}
        ICON_BIG ${CMAKE_CURRENT_SOURCE_DIR}/Images/cabbage.png              
        ICON_SMALL ${CMAKE_CURRENT_SOURCE_DIR}/Images/cabbage.png  
        DOCUMENT_EXTENSIONS ".csd"          # Specify file extensions that should be associated with this app
        COMPANY_NAME ${JucePlugin_Manufacturer}
        PRODUCT_NAME ${PROJECT_NAME})     # The name of the final executable, which can differ from the target name
else()
# ============== This part of the CMake setup deals with plugins only.....
if(APPLE)
    juce_set_vst2_sdk_path("$ENV{HOME}/SDKs/VST_SDK/VST3_SDK")
elseif(MSVC)
    juce_set_vst2_sdk_path("C:/SDKs/VST_SDK/VST3_SDK")
else()
    juce_set_vst2_sdk_path("$ENV{HOME}/SDKs/VST_SDK/VST3_SDK")
endif()



    set(PLUGIN_NEEDS_OUTPUT PLUGIN_IS_A_SYNTH)
    SetIfSynth(PLUGIN_VST2_CATEGORY "kPlugCategSynth" "kPlugCategEffect")
    SetIfSynth(PLUGIN_AU_MAIN_TYPE "aumi" "aumf")
    SetIfSynth(PLUGIN_VST3_CATEGORIES "Instrument|Synth" "Fx")
    SetIfSynth(SYNTH_OR_EFFECT "Synth" "Effect")


    juce_add_plugin(${PROJECT_NAME}
        PLUGIN_NAME ${PROJECT_NAME}
        FORMATS VST VST3 Standalone AU
        PLUGIN_MANUFACTURER_CODE "${JucePlugin_ManufacturerCode}"
        PLUGIN_CODE "RORY"
        DESCRIPTION ${JucePlugin_Desc}
        NEEDS_MIDI_INPUT TRUE
        IS_SYNTH ${PLUGIN_IS_A_SYNTH}
        BUNDLE_ID com.CabbageAudio.plugin
        NEEDS_MIDI_OUTPUT ${PLUGIN_NEEDS_MIDI_OUTPUT}
        IS_MIDI_EFFECT FALSE
        EDITOR_WANTS_KEYBOARD_FOCUS FALSE
        VST_NUM_MIDI_INS 16
        VST_NUM_MIDI_OUTS 16
        VST2_CATEGORY "${PLUGIN_VST2_CATEGORY}"
        VST3_CATEGORIES "${PLUGIN_VST3_CATEGORIES}"
        AU_EXPORT_PREFIX "${PROJECT_NAME}AU"
        AU_SANDBOX_SAFE TRUE
        COPY_PLUGIN_AFTER_BUILD FALSE
        COMPANY_NAME ${JucePlugin_Manufacturer}
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
Source/CabbageIds.h)

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

if("${PROJECT_NAME}" STREQUAL "Cabbage")
    target_sources(${PROJECT_NAME} PRIVATE ${COMMON_SOURCES} ${IDE_SOURCES} )
    source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/Source PREFIX "Cabbage Source" FILES ${COMMON_SOURCES} ${IDE_SOURCES})
else()
# ============== This part of the CMake setup deals with plugins only.....
    target_sources(${PROJECT_NAME} PRIVATE ${COMMON_SOURCES} )
    source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/Source PREFIX "Cabbage Source" FILES ${COMMON_SOURCES})
endif()

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
    juce::juce_audio_utils
    juce::juce_audio_plugin_client
    juce::juce_opengl)

target_compile_definitions(${PROJECT_NAME}
    PRIVATE
        # JUCE_WEB_BROWSER and JUCE_USE_CURL would be on by default, but you might not need them.
        JUCE_WEB_BROWSER=0  # If you remove this, add `NEEDS_WEB_BROWSER TRUE` to the `juce_add_gui_app` call
        JUCE_USE_CURL=0     # If you remove this, add `NEEDS_CURL TRUE` to the `juce_add_gui_app` call
        JUCE_APPLICATION_NAME_STRING="$<TARGET_PROPERTY:${PROJECT_NAME},JUCE_PRODUCT_NAME>"
        JUCE_APPLICATION_VERSION_STRING="$<TARGET_PROPERTY:${PROJECT_NAME},JUCE_VERSION>"
        JUCE_MODAL_LOOPS_PERMITTED=1
        JucePlugin_Manufacturer="${JucePlugin_Manufacturer}"
        JucePlugin_Desc="${JucePlugin_Desc}"
        CabbagePro=${CabbagePro}
        PLUGIN_MANUFACTURER_CODE "${JucePlugin_ManufacturerCode}"
        Use_Native_File_Browser=1
        JUCE_VST3_CAN_REPLACE_VST2=0
        Cabbage_IDE_Build=${IS_IDE_Build}
        JUCE_DISPLAY_SPLASH_SCREEN=0)

if(MSVC)
    # SET(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "C:/Program Files/Csound6_x64/include/csound")
    include_directories("C:\\Program Files\\Csound6_x64\\include\\csound")
    if(${CabbagePro} EQUAL 1)
        find_library(CSOUND_LIBRARY NAMES libcsound64 HINTS "c:\\Program Files\\Csound6_x64\\lib")
    else()
    find_library(CSOUND_LIBRARY NAMES csound64 HINTS "c:\\Program Files\\Csound6_x64\\lib")
    endif()
    target_link_libraries(${PROJECT_NAME} 
    PRIVATE
        ${JUCE_MODULES}
        ${CSOUND_LIBRARY}
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)

elseif(APPLE)
    set(CMAKE_OSX_DEPLOYMENT_TARGET "10.13" CACHE STRING "Minimum OS X deployment version" FORCE)
    SET(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "/Library/Frameworks/CsoundLib64")
    find_library(CSOUND_LIBRARY NAMES CsoundLib64 HINTS /Library/Frameworks/CsoundLib64.framework/
    "/Library/Frameworks/CsoundLib64.framework")  
    include_directories("/Library/Frameworks/CsoundLib64.framework/Headers")

    target_link_libraries(${PROJECT_NAME} 
    PRIVATE
        ${JUCE_MODULES}
        ${CSOUND_LIBRARY}
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
