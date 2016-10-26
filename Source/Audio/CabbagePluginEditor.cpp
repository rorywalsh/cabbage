/*
  ==============================================================================

    CabbagePluginEditor.cpp
    Created: 24 Oct 2016 6:38:36pm
    Author:  rory

  ==============================================================================
*/

#include "CabbagePluginEditor.h"

class CabbageCheckbox;

//==============================================================================
CabbagePluginEditor::CabbagePluginEditor (CabbagePluginProcessor& p)
    : AudioProcessorEditor (&p), processor (p), lookAndFeel()
{
    setSize (400, 300);
	setLookAndFeel(&lookAndFeel);
	createEditorInterface(processor.cabbageWidgets);
}

CabbagePluginEditor::~CabbagePluginEditor()
{

}

//==============================================================================
void CabbagePluginEditor::paint (Graphics& g)
{
    g.fillAll (backgroundColour);
}

void CabbagePluginEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
//==============================================================================
void CabbagePluginEditor::createEditorInterface(ValueTree widgets)
{
	
	CabbageUtilities::writeValueTreeToFile(processor.cabbageWidgets, "/home/rory/Desktop/test.xml");
	
	for(int widget=0;widget<widgets.getNumChildren();widget++)
	{
		const String widgetType = widgets.getChild(widget).getProperty(CabbageIdentifierIds::type).toString();

		if(widgetType==CabbageIdentifierIds::form)
			SetupWindow(widgets.getChild(widget));
		else if(widgetType==CabbageIdentifierIds::rslider)
			InsertSlider(widgets.getChild(widget));
		else if(widgetType==CabbageIdentifierIds::checkbox)
			InsertCheckbox(widgets.getChild(widget));		
	}
}

void CabbagePluginEditor::SetupWindow(ValueTree widgetData)
{
	const String name = CabbageWidget::getStringProp(widgetData, CabbageIdentifierIds::caption);
	setName(CabbageWidget::getStringProp(widgetData, CabbageIdentifierIds::caption));
	const int width = CabbageWidget::getNumProp(widgetData, CabbageIdentifierIds::width);
    const int height = CabbageWidget::getNumProp(widgetData, CabbageIdentifierIds::height);
	const String backgroundColourString = CabbageWidget::getStringProp(widgetData, CabbageIdentifierIds::colour);
	backgroundColour = Colour::fromString(backgroundColourString);	
	setSize(width, height);	
	repaint();
}

void CabbagePluginEditor::InsertCheckbox(ValueTree cabbageWidgetData)
{
	CabbageCheckbox* checkbox;
	components.add(checkbox = new CabbageCheckbox(cabbageWidgetData));
	checkbox->addListener(this);
	addAndMakeVisible(checkbox);
}

void CabbagePluginEditor::buttonClicked(Button* button)
{	
	ValueTree widgetData = CabbageWidget::getValueTreeForComponent(processor.cabbageWidgets, button->getName());
	
	if (CabbageAudioParameter* param = getParameterForButton (button))
	{
		param->beginChangeGesture();
		param->setValue(button->getToggleState()==true ? 1.f : 0.f);
		param->endChangeGesture();
	}	
}



