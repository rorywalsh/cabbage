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
: AudioProcessorEditor (&p),
processor (p), 
lookAndFeel(), 
layoutEditor(processor.cabbageWidgets),
mainComponent()
{
	setName("PluginEditor");
    setSize (400, 300);
	setLookAndFeel(&lookAndFeel);
	createEditorInterface(processor.cabbageWidgets);	
	addAndMakeVisible(layoutEditor);
	addAndMakeVisible(mainComponent);
	layoutEditor.setTargetComponent(&mainComponent);
	layoutEditor.updateFrames();
	layoutEditor.setEnabled(false);
    layoutEditor.toFront(false);
	layoutEditor.setInterceptsMouseClicks(true, true);
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
	layoutEditor.setBounds(getLocalBounds());
	mainComponent.setBounds(getLocalBounds());
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
//==============================================================================
void CabbagePluginEditor::createEditorInterface(ValueTree widgets)
{
	
	for(int widget=0;widget<widgets.getNumChildren();widget++)
	{
		const String widgetType = widgets.getChild(widget).getProperty(CabbageIdentifierIds::type).toString();

		if(widgetType==CabbageIdentifierIds::form)
			SetupWindow(widgets.getChild(widget));
		else if(widgetType==CabbageIdentifierIds::rslider)
			InsertSlider(widgets.getChild(widget));
		else if(widgetType==CabbageIdentifierIds::checkbox)
			InsertCheckbox(widgets.getChild(widget));
		else if(widgetType==CabbageIdentifierIds::combobox)
			InsertComboBox(widgets.getChild(widget));		
	}
}

void CabbagePluginEditor::SetupWindow(ValueTree widgetData)
{
	const String name = CabbageWidgetData::getStringProp(widgetData, CabbageIdentifierIds::caption);
	setName(CabbageWidgetData::getStringProp(widgetData, CabbageIdentifierIds::caption));
	const int width = CabbageWidgetData::getNumProp(widgetData, CabbageIdentifierIds::width);
    const int height = CabbageWidgetData::getNumProp(widgetData, CabbageIdentifierIds::height);
	const String backgroundColourString = CabbageWidgetData::getStringProp(widgetData, CabbageIdentifierIds::colour);
	backgroundColour = Colour::fromString(backgroundColourString);	
	setSize(width, height);	
	repaint();
}

void CabbagePluginEditor::InsertCheckbox(ValueTree cabbageWidgetData)
{
	CabbageCheckbox* checkbox;
	components.add(checkbox = new CabbageCheckbox(cabbageWidgetData));
	checkbox->addListener(this);
	mainComponent.addAndMakeVisible(checkbox);
}

void CabbagePluginEditor::InsertComboBox(ValueTree cabbageWidgetData)
{
	CabbageComboBox* combobox;
	components.add(combobox = new CabbageComboBox(cabbageWidgetData, this));
	combobox->addListener(this);
	mainComponent.addAndMakeVisible(combobox);
}

//======================================================================================================
void CabbagePluginEditor::comboBoxChanged (ComboBox* combo)
{	
	if (CabbageAudioParameter* param = getParameterForComboBox(combo))
	{
		param->beginChangeGesture();
		const int value = combo->getSelectedItemIndex()+1;
		param->setValue(value);
		param->endChangeGesture();
	}	

}
 
void CabbagePluginEditor::buttonClicked(Button* button)
{	
	if (CabbageAudioParameter* param = getParameterForButton(button))
	{
		param->beginChangeGesture();
		param->setValue(button->getToggleState()==true ? 1 : 0);
		param->endChangeGesture();
	}	
}

//======================================================================================================
void CabbagePluginEditor::enableGUIEditor(bool enable)
{
	layoutEditor.setEnabled(enable);
	isGUIEnabled = enable;
	layoutEditor.toFront(false);
}

void CabbagePluginEditor::setCurrentlySelectedComponent(String componentName)
{
	currentlySelectedComponentName = componentName;
}

ValueTree CabbagePluginEditor::getCurrentlySelectedComponent()
{
	return CabbageWidgetData::getValueTreeForComponent(processor.cabbageWidgets, currentlySelectedComponentName);
}

void CabbagePluginEditor::updateLayoutEditorFrames()
{
	if(isGUIEnabled)
		layoutEditor.updateFrames();
}

