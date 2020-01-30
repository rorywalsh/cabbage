#!/bin/bash  

#these commands ands being triggered from the LinuxMakefiles/Builds dir
mkdir -p ../../JuceLibraryCode/modules/juce_audio_plugin_client/LV2 
cp -rf ../../LV2/LV2 ../../JuceLibraryCode/modules/juce_audio_plugin_client
cp ../../LV2/juce_audio_plugin_client_LV2.cpp ../../JuceLibraryCode/modules/juce_audio_plugin_client/juce_audio_plugin_client_LV2.cpp

echo "Modifying JUCE source to add support for LV2 plugin format"make -j8

sed -i "/** Creates the processor's GUI./c\   #if ! JUCE_AUDIOPROCESSOR_NO_GUI\n/** Creates the processor's GUI." ../../JuceLibraryCode/modules/juce_audio_processors/processors/juce_AudioProcessor.h

sed -i "/AudioProcessorEditor\* createEditorIfNeeded();/c\   AudioProcessorEditor\* createEditorIfNeeded();\n   #endif" ../../JuceLibraryCode/modules/juce_audio_processors/processors/juce_AudioProcessor.h

sed -i "/virtual void processorLayoutsChanged();/c\    virtual void processorLayoutsChanged();\n    /** LV2 specific calls, saving/restore as string. */\n    virtual String getStateInformationString () { return String(); }\n    virtual void setStateInformationString (const String&) {}" ../../JuceLibraryCode/modules/juce_audio_processors/processors/juce_AudioProcessor.h

sed -i "/virtual CurveData getResponseCurve (CurveData::Type \/*curveType\*\/) const      { return {}; }/c\ /virtual CurveData getResponseCurve (CurveData::Type \/*curveType\*/) const      { return {}; \n   #if ! JUCE_AUDIOPROCESSOR_NO_GUI" ../../JuceLibraryCode/modules/juce_audio_processors/processors/juce_AudioProcessor.h

sed -i "/void editorBeingDeleted (AudioProcessorEditor\*) noexcept;/c\    #if ! JUCE_AUDIOPROCESSOR_NO_GUI\n    void editorBeingDeleted (AudioProcessorEditor\*) noexcept;\n   #endif" ../../JuceLibraryCode/modules/juce_audio_processors/processors/juce_AudioProcessor.h

sed -i "/wrapperType_Unity/c\        wrapperType_Unity,\n        wrapperType_LV2" ../../JuceLibraryCode/modules/juce_audio_processors/processors/juce_AudioProcessor.h

sed -i "/Component::SafePointer<AudioProcessorEditor> activeEditor;/c\   #if ! JUCE_AUDIOPROCESSOR_NO_GUI\n    Component::SafePointer<AudioProcessorEditor> activeEditor;\n   #endif" ../../JuceLibraryCode/modules/juce_audio_processors/processors/juce_AudioProcessor.h

# echo "include ../../LV2/LV2.mk" >> Makefile