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

class CabbageAudioParameter;

class CabbagePluginProcessor : public CsoundPluginProcessor,
public Timer
{
public:

    class CabbageJavaClass  : public DynamicObject
    {

        CabbagePluginProcessor* owner;
    public:

        CabbageJavaClass (CabbagePluginProcessor* owner): owner (owner)
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

	
    CabbagePluginProcessor (File inputFile = File(), const int ins=2, const int outs=2);
	void createCsound(File inputFile, bool shouldCreateParameters = true);
    ~CabbagePluginProcessor();

    ValueTree cabbageWidgets;
    void getChannelDataFromCsound() override;
    void triggerCsoundEvents() override;
    void setWidthHeight();
    bool addImportFiles (StringArray& lineFromCsd);
    void parseCsdFile (StringArray& linesFromCsd);
    void createParameters();
    void updateWidgets (String csdText);
    void handleXmlImport (XmlElement* xml, StringArray& linesFromCsd);
    void getMacros (StringArray& csdText);
    void generateCabbageCodeFromJS (PlantImportStruct& importData, String text);
    void insertUDOCode (PlantImportStruct importData, StringArray& linesFromCsd);
    void insertPlantCode (StringArray& linesFromCsd);
    bool isWidgetPlantParent (StringArray linesFromCsd, int lineNumber);
    bool shouldClosePlant (StringArray linesFromCsd, int lineNumber);
    void setPluginName (String name) {    pluginName = name;  }
    String getPluginName() { return pluginName;  }
    void expandMacroText (String &line, ValueTree wData);
	void prepareToPlay(double sampleRate, int samplesPerBlock) override;
	void setCabbageParameter(String channel, float value);
    CabbageAudioParameter* getParameterForXYPad (String name);
    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;
    //===== XYPad methods =========
    void addXYAutomator (CabbageXYPad* xyPad, ValueTree wData);
    void enableXYAutomator (String name, bool enable, Line<float> dragLine);
    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    void setParametersFromXml (XmlElement* e);
    XmlElement savePluginState (String tag, File xmlFile = File(), String presetName="");
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

	StringArray getCurrentCsdFileAsStringArray()
	{
		StringArray csdArray;
		csdArray.addLines(csdFile.loadFileAsString());
		return csdArray;
	}
private:
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


};

class CabbageAudioParameter : public AudioParameterFloat
{

public:
	CabbageAudioParameter(CabbagePluginProcessor* owner, ValueTree wData, Csound& csound, String channel, String name, float minValue, float maxValue, float def, float incr, float skew)
		: AudioParameterFloat(name, channel, NormalisableRange<float>(minValue, maxValue, incr, skew), def), channel(channel), widgetName(name), currentValue(def), owner(owner)
	{
		// widgetType = CabbageWidgetData::getStringProp (widgetData, CabbageIdentifierIds::type);
        if(name.contains("combobox"))
            isCombo = true;
	}
	~CabbageAudioParameter() {}

	float getValue() const override
	{
		return range.convertTo0to1(currentValue);
	}

	void setValue(float newValue) override
	{
		//csound.SetChannel (channel.toUTF8(), range.convertFrom0to1 (newValue));
        currentValue = isCombo ? juce::roundToInt(range.convertFrom0to1 (newValue)) : range.convertFrom0to1 (newValue);
		owner->setCabbageParameter(channel, currentValue);

	}

	const String getWidgetName() { return widgetName; }

	String channel;
	String widgetName;
	float currentValue;
    bool isCombo = false;

	CabbagePluginProcessor* owner;
};



#endif  // CABBAGEPLUGINPROCESSOR_H_INCLUDED
