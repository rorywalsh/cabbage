/*
  Copyright (C) 2016 Rory Walsh

  Cabbage is free software; you can redistribute it
  and/or modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  Cabbage is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with Csound; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
  02111-1307 USA
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
    mainComponent.setInterceptsMouseClicks(false, true);
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
}
//==============================================================================
void CabbagePluginEditor::createEditorInterface(ValueTree widgets)
{
    components.clear();

    for(int widget=0; widget<widgets.getNumChildren(); widget++)
    {
        const String widgetType = widgets.getChild(widget).getProperty(CabbageIdentifierIds::type).toString();

        if(widgetType==CabbageIdentifierIds::form)
            setupWindow(widgets.getChild(widget));
        else
        {
            insertWidget(widgets.getChild(widget));
        }
    }
}

//======================================================================================================
void CabbagePluginEditor::setupWindow(ValueTree widgetData)
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
//======================================================================================================
void CabbagePluginEditor::addNewWidget(String widgetType, Point<int> position)
{

    StringArray csdArray = processor.getCurrentCsdFileAsStringArray();
    const String widgetTreeIdentifier = "newlyAddedWidget";
    ValueTree newWidget(widgetTreeIdentifier);

    CabbageWidgetData::setWidgetState(newWidget, widgetType, newlyAddedWidgetIndex);
    newWidget.setProperty(CabbageIdentifierIds::top, position.getY(), 0);
    newWidget.setProperty(CabbageIdentifierIds::left, position.getX(), 0);

    processor.cabbageWidgets.addChild(newWidget, -1, 0);

    setCurrentlySelectedComponents(StringArray(CabbageWidgetData::getStringProp(newWidget, CabbageIdentifierIds::name)));

    insertWidget(newWidget);
    updateLayoutEditorFrames();

    sendChangeMessage(); 	//update code in editor

    newlyAddedWidgetIndex++;

}

//======================================================================================================
void CabbagePluginEditor::insertWidget(ValueTree cabbageWidgetData)
{
    const String widgetType = cabbageWidgetData.getProperty(CabbageIdentifierIds::type).toString();

    if(widgetType==CabbageIdentifierIds::checkbox)
        insertCheckbox(cabbageWidgetData);
    else if(widgetType==CabbageIdentifierIds::combobox)
        insertComboBox(cabbageWidgetData);
    else if(widgetType==CabbageIdentifierIds::image)
        insertImage(cabbageWidgetData);
    else if(widgetType==CabbageIdentifierIds::rslider 
			|| widgetType==CabbageIdentifierIds::vslider
			|| widgetType==CabbageIdentifierIds::hslider)
        insertSlider(cabbageWidgetData);

}

void CabbagePluginEditor::insertCheckbox(ValueTree cabbageWidgetData)
{
    CabbageCheckbox* checkbox;
    components.add(checkbox = new CabbageCheckbox(cabbageWidgetData));
    checkbox->addListener(this);
    addToEditorAndMakeVisible(checkbox, cabbageWidgetData);
}

void CabbagePluginEditor::insertComboBox(ValueTree cabbageWidgetData)
{
    CabbageComboBox* combobox;
    components.add(combobox = new CabbageComboBox(cabbageWidgetData, this));
    combobox->addListener(this);
    addToEditorAndMakeVisible(combobox, cabbageWidgetData);
}

void CabbagePluginEditor::insertImage(ValueTree cabbageWidgetData)
{
    CabbageImage* image;
    components.add(image = new CabbageImage(cabbageWidgetData, this));
    addToEditorAndMakeVisible(image, cabbageWidgetData);
}

void CabbagePluginEditor::insertSlider(ValueTree cabbageWidgetData)
{
    CabbageSlider* slider;
    components.add(slider = new CabbageSlider(cabbageWidgetData, this));
    addToEditorAndMakeVisible(slider, cabbageWidgetData);
}
//======================================================================================================
CabbageAudioParameter* CabbagePluginEditor::getParameterForComponent (Component* comp)
{
    const OwnedArray<AudioProcessorParameter>& params = processor.getParameters();
    for( int i = 0 ; i < params.size() ; i++)
    {
        if(comp->getName()==params[i]->getName(512))
            return  dynamic_cast<CabbageAudioParameter*> (params[i]);
    }

    return nullptr;
}

//======================================================================================================
void CabbagePluginEditor::comboBoxChanged (ComboBox* combo)
{
    if (CabbageAudioParameter* param = getParameterForComponent(combo))
    {
        param->beginChangeGesture();
        const int value = combo->getSelectedItemIndex()+1;
        param->setValue(value);
        param->endChangeGesture();
    }
}

void CabbagePluginEditor::buttonClicked(Button* button)
{
    if (CabbageAudioParameter* param = getParameterForComponent(button))
    {
        param->beginChangeGesture();
        param->setValue(button->getToggleState()==true ? 1 : 0);
        param->endChangeGesture();
    }
}

//======================================================================================================
void CabbagePluginEditor::enableEditMode(bool enable)
{
    layoutEditor.setEnabled(enable);
    editModeEnabled = enable;
    layoutEditor.toFront(false);
}
//======================================================================================================
void CabbagePluginEditor::setCurrentlySelectedComponents(StringArray componentNames)
{
    currentlySelectedComponentNames = componentNames;
}

void CabbagePluginEditor::resetCurrentlySelectedComponents()
{
    currentlySelectedComponentNames.clear();
}

Component* CabbagePluginEditor::getComponentFromName(String name)
{
    for (auto comp : components)
    {
        if(name == comp->getName())
            return comp;
    }

    return nullptr;
}

Array<ValueTree> CabbagePluginEditor::getValueTreesForCurrentlySelectedComponents()
{
    Array<ValueTree> valueTreeArray;
    for(String compName : currentlySelectedComponentNames)
        valueTreeArray.add(CabbageWidgetData::getValueTreeForComponent(processor.cabbageWidgets, compName));

    return valueTreeArray;
}

ValueTree CabbagePluginEditor::getValueTreeForComponent(String compName)
{
    return CabbageWidgetData::getValueTreeForComponent(processor.cabbageWidgets, getComponentFromName(compName)->getName());
}

void CabbagePluginEditor::updateLayoutEditorFrames()
{
    if(editModeEnabled)
        layoutEditor.updateFrames();
}

//======================================================================================================
void CabbagePluginEditor::addToEditorAndMakeVisible(Component* comp, ValueTree widgetData)
{
    const String parent = CabbageWidgetData::getStringProp(widgetData, CabbageIdentifierIds::parentcomponent);
    if(auto parentComp = getComponentFromName(parent))
    {
        parentComp->addAndMakeVisible(comp);
    }
    else
        mainComponent.addAndMakeVisible(comp);
}
//======================================================================================================
void CabbagePluginEditor::mouseDown(const MouseEvent& e)
{
    PopupMenu menu;

}