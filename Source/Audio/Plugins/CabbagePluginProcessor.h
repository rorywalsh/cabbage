/*
  Copyright (C) 2016 Rory Walsh

  Cabbage is free software; you can redistribute it
  and/or modify it under the terms of the GNU General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  Cabbage is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU General Public
  License along with Csound; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
  02111-1307 USA
*/

#ifndef CABBAGEPLUGINPROCESSOR_H_INCLUDED
#define CABBAGEPLUGINPROCESSOR_H_INCLUDED

#include "CsoundPluginProcessor.h"
#include "../../Widgets/CabbageWidgetData.h"
#include "../../CabbageIds.h"
#include "../../Widgets/CabbageXYPad.h"

class CabbagePluginParameter;

class CabbagePluginProcessor : public CsoundPluginProcessor,
public Timer
{
public:

    class CabbageJavaClass  : public DynamicObject
    {

        CabbagePluginProcessor* owner;
    public:

        explicit CabbageJavaClass (CabbagePluginProcessor* thisOwner): owner (thisOwner)
        {
            setMethod ("print", print);
        }

        static Identifier getClassName()   { return "Cabbage"; }

        static var print (const var::NativeFunctionArgs& args)
        {
            if (args.numArguments > 0)
                if (CabbageJavaClass* thisObject = dynamic_cast<CabbageJavaClass*> (args.thisObject.getObject()))
                    thisObject->owner->cabbageScriptGeneratedCode.add (args.arguments[0].toString());

            return var::undefined();
        }



        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CabbageJavaClass)
    };

    struct PlantImportStruct
    {
        String nsp, name, csoundCode;
        StringArray cabbageCode;
    };

	CabbagePluginProcessor (File inputFile, BusesProperties IOBuses);
	void createCsound(File inputFile, bool shouldCreateParameters = true);
    ~CabbagePluginProcessor() override;

    ValueTree cabbageWidgets;
    CachedValue<var> cachedValue;
    void getChannelDataFromCsound() override;
    void getIdentifierDataFromCsound() override;
    void triggerCsoundEvents() override;
    void setWidthHeight();
    int chnsetGestureMode = 1;
    CabbageWidgetIdentifiers** pd;
    CabbageWidgetIdentifiers* identData;
    int autoUpdateCount = 0;
    bool autoUpdateIsOn = false;

    
    //save and restore user plugin presets
    void addPluginPreset(String presetName, String fileName, bool remove);
    void restorePluginPreset(String presetName, String filename);
    
    bool addImportFiles (StringArray& lineFromCsd);
    void parseCsdFile (StringArray& linesFromCsd);
    // use this instead of AudioProcessor::addParameter
    void addCabbageParameter(std::unique_ptr<CabbagePluginParameter> parameter);
    void createCabbageParameters();
    void updateWidgets (String csdText);
    void handleXmlImport (XmlElement* xml, StringArray& linesFromCsd);
    void getMacros (const StringArray& csdText);
    void generateCabbageCodeFromJS (PlantImportStruct& importData, String text);
    void insertUDOCode (PlantImportStruct importData, StringArray& linesFromCsd);
    void insertPlantCode (StringArray& linesFromCsd);
    bool isWidgetPlantParent (StringArray& linesFromCsd, int lineNumber);
    bool shouldClosePlant (StringArray& linesFromCsd, int lineNumber);
    void setPluginName (String name) {    pluginName = name;  }
    String getPluginName() { return pluginName;  }
    void expandMacroText (String &line, ValueTree wData);
	void prepareToPlay(double sampleRate, int samplesPerBlock) override;
	void setCabbageParameter(String& channel, float value, ValueTree& wData);
    CabbagePluginParameter* getParameterForXYPad (StringRef name);
    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool editorIsOpen = false;
    void releaseResources() override;
    bool hasEditor() const override;
    //===== XYPad methods =========
    void addXYAutomator (CabbageXYPad* xyPad, ValueTree wData);
    void enableXYAutomator (String name, bool enable, Line<float> dragLine);
    void disableXYAutomators();
    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    void setParametersFromXml (XmlElement* e);
    XmlElement savePluginState (String tag);
    void restorePluginState (XmlElement* xmlElement);
    //==============================================================================
    StringArray cabbageScriptGeneratedCode;
    Array<PlantImportStruct> plantStructs;

    int64 csdLastModifiedAt;
    void timerCallback() override;
	//uid needed for Cabbage host
	AudioProcessorGraph::NodeID nodeId;

	File getCsdFile()
	{
		return csdFile;
	}

    static BusesProperties readBusesPropertiesFromXml(File csdFile)
    {
        BusesProperties buses;

        String csdString = csdFile.loadFileAsString();

        const int numOutChannels = CabbageUtilities::getHeaderInfo(csdString, "nchnls");
        int numInChannels = numOutChannels;
        if (CabbageUtilities::getHeaderInfo(csdString, "nchnls_i") != -1 && CabbageUtilities::getHeaderInfo(csdString, "nchnls_i") != 0)
            numInChannels = CabbageUtilities::getHeaderInfo(csdString, "nchnls_i") ;

        // repeat this for every bus in the xml file
        for (int i = 0, cnt = 1; i < numOutChannels; i+=2, cnt++)
            buses.addBus(false, "Output #" + String(cnt), AudioChannelSet::stereo());

        for (int i = 0, cnt = 1; i < numInChannels; i+=2, cnt++)
            buses.addBus(true, "n'Input #" + String(cnt), AudioChannelSet::stereo());

        return buses;
    }

	StringArray getCurrentCsdFileAsStringArray()
	{
		StringArray csdArray;
		csdArray.addLines(csdFile.loadFileAsString());
		return csdArray;
	}
    
    Font getCustomFont(){        return customFont;    }
    File getCustomFontFile(){        return customFontFile;    }
    
    // use this instead of AudioProcessor::getParameters
    const OwnedArray<CabbagePluginParameter>& getCabbageParameters() const { return parameters; }
    int currentPluginScale = -1;
    String currentPresetName = "";
    

private:
#if !Cabbage_IDE_Build
    PluginHostType pluginType;
#endif
    controlChannelInfo_s* csoundChanList;
    int numberOfLinesInPlantCode = 0;
    String pluginName;
    File csdFile;
    int linesToSkip = 0;
    NamedValueSet macroText;
    var macroNames;
    var macroStrings;
    bool xyAutosCreated = false;
    OwnedArray<XYPadAutomator> xyAutomators;
	int samplingRate = 44100;
    int samplesInBlock = 64;
	int screenWidth, screenHeight;
	bool isUnityPlugin = false;
    int automationMode = 0;
    OwnedArray<CabbagePluginParameter> parameters;
    Font customFont;
    File customFontFile;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CabbagePluginProcessor)

};


/*
 This is a thin shim around juce::AudioParameterFloat that allows us to use a subset of its interface
 while being able to intercept calls to the host for parameters that are non-automatable.
 */

class CabbagePluginParameter
{
    
public:
    CabbagePluginParameter(CabbagePluginProcessor* thisOwner,
                          ValueTree wData,
                          Csound& csound,
                          const String& channelToUse,
                          const String& name,
                          float minValue,
                          float maxValue,
                          float def,
                          float incr,
                          float skew,
                          bool automatable = true,
                          const String& prefix  = String(),
                          const String& postfix = String(),
                          bool isCombo = false)
    : parameter(new CabbageHostParameter(*this, thisOwner, wData, csound, channelToUse, name, prefix, postfix, minValue, maxValue, def, incr, skew, isCombo)),
    widgetName(name),
    isAutomatable(automatable)
    {
        ignoreUnused(thisOwner);
    }
    
    ~CabbagePluginParameter()
    {
        if (!hostParameterReleased)
        {
            delete parameter;
        }
    }
    
    void setValue(float newValue)
    {
        parameter->setValue(newValue);
    }
    
    float getValue() const
    {
        return parameter->getValue();
    }
    
    void setValueNotifyingHost(float newValue)
    {
        if (isAutomatable)
        {
            parameter->setValueNotifyingHost(newValue);
        }
        else
        {
            setValue(newValue);
        }
    }
    
    void beginChangeGesture()
    {
        isPerformingGesture = true;
        if (isAutomatable)
        {
            parameter->beginChangeGesture();
        }
    }
    
    void endChangeGesture()
    {
        isPerformingGesture = false;
        if (isAutomatable)
        {
            parameter->endChangeGesture();
        }
    }
    
    AudioParameterFloat* releaseHostParameter()
    {
        hostParameterReleased = true;
        return parameter;
    }
    
    const NormalisableRange<float>& getNormalisableRange() const { return parameter->getNormalisableRange(); }
    
    const String getChannel() const { return parameter->getChannel(); }
    const String getWidgetName() { return widgetName; }
    bool getIsAutomatable() const { return isAutomatable; }
    bool isPerformingGesture = false;
    
private:
    class CabbageHostParameter : public AudioParameterFloat
    {
    public:
        virtual ~CabbageHostParameter() override { }
        float getValue() const override { return range.convertTo0to1(currentValue); }
        
        void setValue(float newValue) override
        {
//            DBG(channel);
            DBG(newValue);
            currentValue = isCombo ? juce::roundToInt(range.convertFrom0to1 (newValue)) : range.convertFrom0to1 (newValue);
            processor->setCabbageParameter(channel, currentValue, valueTree);
        }
        
        String getText(float normalizedValue, int length) const override
        {
            const int minNumCharsForValue = 3;
            const int decimalPlaces = 3;        // TODO: this probably shouldn't be hardcoded
            const auto scaledValue = range.convertFrom0to1(normalizedValue);
            String asText = "";
            
            // if we can't fit a minimum number of digits for our value within
            // the requested length, we'll ditch the prefix and postfix
            if (length > 0 && minNumCharsForValue + prefix.length() + postfix.length() > length)
            {
                showingAffixes = false;
                asText = String(scaledValue, decimalPlaces);
            }
            else
            {
                showingAffixes = true;
                
                asText = prefix;
                asText += String(scaledValue, decimalPlaces);
                
                if (length > 0 && asText.length() + postfix.length() > length)
                {
                    asText = asText.substring(0, asText.length() - postfix.length());
                }
                
                asText += postfix;
            }
            
            return asText;
        }
        
        float getValueForText(const String& text) const override
        {
            if (showingAffixes)
            {
                return text.substring(prefix.length()).
                dropLastCharacters(postfix.length()).getFloatValue();
            }
            
            return text.getFloatValue();
        }
        
        const String& getChannel() const { return channel; }
        
    private:
        CabbageHostParameter(CabbagePluginParameter& thisOwner,
                             CabbagePluginProcessor* proc,
                             ValueTree wData,
                             Csound& csound,
                             const String& channelToUse,
                             const String& thisName,
                             const String& prefixToUse,
                             const String& postfixToUse,
                             float minValue,
                             float maxValue,
                             float def,
                             float incr,
                             float skew,
                             bool isACombo)
        : AudioParameterFloat(thisName, channelToUse, NormalisableRange<float>(minValue, maxValue, incr, skew), def),
        channel(channelToUse),
        prefix(prefixToUse),
        postfix(postfixToUse),
        currentValue(def),
        isCombo(isACombo),
        processor(proc),
        valueTree(wData)
        {
            ignoreUnused(thisOwner);
            ignoreUnused(csound);
        }
        
        String channel;
        const String prefix { };
        const String postfix { };
        float currentValue;
        bool isCombo = false;
        
        CabbagePluginProcessor* processor;
        ValueTree valueTree;
        
        mutable bool showingAffixes = true;
        
        friend class CabbagePluginParameter;
    };
    
    CabbageHostParameter* parameter;
    bool hostParameterReleased = false;
    
    const String widgetName;
    const bool isAutomatable = true;

    //CabbagePluginProcessor* owner;
    
//    bool isCombo(const String name)
//    {
//        if(name.contains("combobox") || name.contains("optionbutton"))
//            return true;
//        return false;
//    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CabbagePluginParameter)
};

#endif  // CABBAGEPLUGINPROCESSOR_H_INCLUDED
