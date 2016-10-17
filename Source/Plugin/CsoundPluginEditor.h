/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#ifndef PLUGINEDITOR_H_INCLUDED
#define PLUGINEDITOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "CsoundPluginProcessor.h"


//==============================================================================
/**
*/
class CsoundAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    CsoundAudioProcessorEditor (CsoundAudioProcessor&);
    ~CsoundAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

	//Simple MIDIGuiObject class. FOr now this just creates a red square. 
	class MIDIObjectGUI : public Component
	{
		int index;

	public:
		MIDIObjectGUI() :Component()
		{}
		~MIDIObjectGUI() {}

		//displays the object umber...
		void mouseDown(const MouseEvent& event)
		{
			AlertWindow::showMessageBoxAsync(AlertWindow::WarningIcon,
				"Message",
				"Object number:"+String(index),
				"Ok");
		}

		//sets object index
		void setIndex(int objIndex)
		{
			index = objIndex;
		}

		//paints the content of the component
		void paint(Graphics& g)
		{
			g.fillAll(Colours::red);
		}
	};

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    CsoundAudioProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CsoundAudioProcessorEditor)
};


#endif  // PLUGINEDITOR_H_INCLUDED
