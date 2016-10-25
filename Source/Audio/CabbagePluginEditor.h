/*
  ==============================================================================

    CabbagePluginEditor.h
    Created: 24 Oct 2016 6:38:36pm
    Author:  rory

  ==============================================================================
*/

#ifndef CABBAGEPLUGINEDITOR_H_INCLUDED
#define CABBAGEPLUGINEDITOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "CabbagePluginProcessor.h"

class CabbagePluginEditor  : public AudioProcessorEditor
{
public:
    CabbagePluginEditor (CabbagePluginProcessor&);
    ~CabbagePluginEditor();

	ValueTree cabbageWidgets;	
	void createEditorInterface(ValueTree widgets);
    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

	//==============================================================================
	void SetupWindow(ValueTree cabbageWidgetData);
	void InsertSlider(ValueTree cabbageWidgetData);
    void InsertGroupBox(ValueTree cabbageWidgetData){};
    void InsertComboBox(ValueTree cabbageWidgetData){};
    void InsertSoundfiler(ValueTree cabbageWidgetData){};
    void InsertSourceButton(ValueTree cabbageWidgetData){};
    void InsertDirectoryList(ValueTree cabbageWidgetData){};
    void InsertTextEditor(ValueTree cabbageWidgetData){};
    void InsertButton(ValueTree cabbageWidgetData){};
    void InsertCheckBox(ValueTree cabbageWidgetData){};
    void InsertCsoundOutput(ValueTree cabbageWidgetData){};
    void InsertMIDIKeyboard(ValueTree cabbageWidgetData){};
    void InsertXYPad(ValueTree cabbageWidgetData){};
    void InsertFileButton(ValueTree cabbageWidgetData){};
    void InsertRecordButton(ValueTree cabbageWidgetData){};
    void InsertImage(ValueTree cabbageWidgetData){};
    void InsertLabel(ValueTree cabbageWidgetData){};
    void InsertTable(ValueTree cabbageWidgetData){};
    void InsertMultiTab(ValueTree cabbageWidgetData){};
    void InsertEncoder(ValueTree cabbageWidgetData){};
    void InsertInfoButton(ValueTree cabbageWidgetData){};
    void InsertLineSeparator(ValueTree cabbageWidgetData){};
    void InsertScope(ValueTree cabbageWidgetData){};
    void InsertPatternMatrix(ValueTree cabbageWidgetData){};
    void InsertSnapshot(ValueTree cabbageWidgetData){};
    void InsertTransport(ValueTree cabbageWidgetData){};
    void InsertListbox(ValueTree cabbageWidgetData){};
    void InsertPopupMenu(ValueTree cabbageWidgetData){};
    void InsertGenTable(ValueTree cabbageWidgetData){};
    void InsertTextbox(ValueTree cabbageWidgetData){};
    void InsertRangeSlider(ValueTree cabbageWidgetData){};
    void InsertSignalDisplay(ValueTree cabbageWidgetData){};
    void InsertStepper(ValueTree cabbageWidgetData){};
    void InsertNumberBox(ValueTree cabbageWidgetData){};

	Colour backgroundColour;
private:

    CabbagePluginProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CabbagePluginEditor)
};


#endif  // CABBAGEPLUGINEDITOR_H_INCLUDED
