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
#include "CabbagePluginEditor.h"

#include <memory>

#ifdef JUCE_WINDOWS
#include <windows.h>
#endif

class CabbageCheckbox;

//==============================================================================
CabbagePluginEditor::CabbagePluginEditor (CabbagePluginProcessor& p)
    : AudioProcessorEditor (&p),
      cabbageForm(this),
      lookAndFeel(),
    cabbageProcessor(p)
#if Cabbage_IDE_Build
    , layoutEditor (cabbageProcessor.cabbageWidgets)
#endif
{
    setName ("PluginEditor");
    cabbageProcessor.editorIsOpen = true;
    setLookAndFeel (&lookAndFeel);
    customFont = cabbageProcessor.getCustomFont();
    customFontFile = cabbageProcessor.getCustomFontFile();
    viewportContainer = std::make_unique<ViewportContainer> ();
    addAndMakeVisible(viewportContainer.get());
    viewportContainer->addAndMakeVisible(cabbageForm);
    viewport = std::make_unique<Viewport> ();
    addAndMakeVisible (viewport.get());
    viewport->setViewedComponent(viewportContainer.get(), false);
    viewport->setScrollBarsShown(false, false);
    cabbageForm.setInterceptsMouseClicks (true, true);
	cabbageForm.addMouseListener(this, false);
    setSize (50, 50);
	cabbageForm.addKeyListener(this);
	//cabbageForm.setWantsKeyboardFocus(true);
    setWantsKeyboardFocus(false);
    createEditorInterface (cabbageProcessor.cabbageWidgets);

#if Cabbage_IDE_Build
    viewportContainer->addAndMakeVisible (layoutEditor);
    layoutEditor.setTargetComponent (&cabbageForm);
    layoutEditor.updateFrames();
    layoutEditor.setEnabled (false);
    layoutEditor.toFront (false);
    layoutEditor.setInterceptsMouseClicks (true, true);
#endif
    resized();

    tooltipWindow.getObject().setLookAndFeel(&lookAndFeel);
    if(cabbageProcessor.getEngine())
        cabbageProcessor.getEngine()->SetControlChannel ("IS_EDITOR_OPEN", 1.0);

    if(cabbageProcessor.currentPluginScale != -1)
        resizePlugin(cabbageProcessor.currentPluginScale);

}

CabbagePluginEditor::~CabbagePluginEditor()
{
    setLookAndFeel (nullptr);
    popupPlants.clear();
    components.clear();
    radioGroups.clear();
    radioComponents.clear();
    cabbageProcessor.editorIsOpen = false;

    detachOpenGL();

    
    if(cabbageProcessor.getEngine())
    	cabbageProcessor.getEngine()->SetChannel ("IS_EDITOR_OPEN", 0.0);
}

void CabbagePluginEditor::refreshValueTreeListeners()
{
	//refresh listeners each time the editor is opened by the Cabbage host
	for (auto component : components)
	{
		if(auto* valueTreeListener = dynamic_cast<ValueTree::Listener*>(component))
            cabbageProcessor.cabbageWidgets.addListener(valueTreeListener);
	}
}

void CabbagePluginEditor::setCurrentPreset(String preset)
{
    cabbageProcessor.currentPresetName = preset;
}

String CabbagePluginEditor::getCurrentPreset() const
{
    return cabbageProcessor.currentPresetName;
}

void CabbagePluginEditor::resized()
{
    sendChannelDataToCsound("SCREEN_WIDTH", getWidth());
    sendChannelDataToCsound("SCREEN_HEIGHT", getHeight());
#if Cabbage_IDE_Build
    layoutEditor.setBounds (getLocalBounds());
    
#endif
    if(viewportContainer)
        viewportContainer->setBounds ( 0, 0, instrumentBounds.getX(), instrumentBounds.getY() );
    cabbageForm.setBounds ( 0, 0, instrumentBounds.getX(), instrumentBounds.getY() );

    
    if(viewport)
    {
     viewport->setBounds ( getLocalBounds() );
     if(showScrollbars)
     {
         if (instrumentBounds.getX() > viewport->getWidth() && instrumentBounds.getY() > viewport->getHeight())
             viewport->setScrollBarsShown(true, true);
         else if (instrumentBounds.getX() > viewport->getWidth() && instrumentBounds.getY() <= viewport->getHeight())
             viewport->setScrollBarsShown(false, true);
         else if (instrumentBounds.getX() <= viewport->getWidth() && instrumentBounds.getY() > viewport->getHeight())
             viewport->setScrollBarsShown(true, false);
     }
     else
         viewport->setScrollBarsShown(false, false);
    }
}

void CabbagePluginEditor::moveBehind(String thisComp, String otherComp)
{
    auto thisWidget = getComponentFromName(thisComp);
    auto otherWidget = getComponentFromName(otherComp);
    if(thisWidget != nullptr && otherWidget != nullptr)
         thisWidget->toBehind(otherWidget);
}
////======================================================================================================
//void CabbagePluginEditor::filesDropped(const StringArray &files, int x, int y)
//{
//    sendChannelStringDataToCsound("LAST_FILE_DROPPED", files[0]);
//}
//
//bool CabbagePluginEditor::isInterestedInFileDrag(const StringArray &files)
//{
//    return true;
//}
//======================================================================================================

void CabbagePluginEditor::mouseMove (const MouseEvent& e)
{
    handleMouseMovement (e);
}

void CabbagePluginEditor::mouseDrag (const MouseEvent& e)
{
    handleMouseMovement (e);
}

void CabbagePluginEditor::mouseDown (const MouseEvent& e)
{
    handleMouseClicks (e, true);
}

void CabbagePluginEditor::mouseUp (const MouseEvent& e)
{
    handleMouseClicks (e, false);
}

void CabbagePluginEditor::mouseWheelMove(const MouseEvent &event, const MouseWheelDetails &wheel)
{
    ignoreUnused(event);
    double scale = cabbageProcessor.currentPluginScale == -1 ? 1 : pluginSizes[cabbageProcessor.currentPluginScale-1];
    sendChannelDataToCsound (CabbageIdentifierIds::mousewheeldeltax, wheel.deltaX/scale);
    sendChannelDataToCsound (CabbageIdentifierIds::mousewheeldeltay, wheel.deltaY/scale);

}

void CabbagePluginEditor::handleMouseMovement (const MouseEvent& e)
{
    if(e.eventComponent->getName().isNotEmpty())
    {
        const ValueTree vt = getValueTreeForComponent(e.eventComponent->getName());
        sendChannelStringDataToCsound(CabbageIdentifierIds::currentWidgetChannel.toString(), CabbageWidgetData::getStringProp(vt, CabbageIdentifierIds::channel));
    }
    int x = e.eventComponent->getTopLevelComponent()->getMouseXYRelative().x;
    int yOffset = (CabbageUtilities::getTarget() == CabbageUtilities::TargetTypes::IDE ? 27 : 0 );
    int y = e.eventComponent->getTopLevelComponent()->getMouseXYRelative().y - yOffset; //27 is the height of the standalone window frame
    double scale = cabbageProcessor.currentPluginScale == -1 ? 1 : pluginSizes[cabbageProcessor.currentPluginScale-1];
    sendChannelDataToCsound (CabbageIdentifierIds::mousex, x/scale);
    sendChannelDataToCsound (CabbageIdentifierIds::mousey, y/scale);
}

void CabbagePluginEditor::handleMouseClicks (const MouseEvent& e, bool isMousePressed)
{
    if (e.mods.isLeftButtonDown())
        sendChannelDataToCsound (CabbageIdentifierIds::mousedownleft, (isMousePressed ? 1 : 0));
    else if (e.mods.isRightButtonDown())
        sendChannelDataToCsound (CabbageIdentifierIds::mousedownright, (isMousePressed ? 1 : 0));
    else if (e.mods.isMiddleButtonDown())
        sendChannelDataToCsound (CabbageIdentifierIds::mousedownlmiddle, (isMousePressed ? 1 : 0));
}
//==============================================================================
void CabbagePluginEditor::createEditorInterface (ValueTree widgets)
{
    components.clear();
	keyboardCount = 0;

    for (int widget = 0; widget < widgets.getNumChildren(); widget++)
    {
        const String widgetType = widgets.getChild (widget).getProperty (CabbageIdentifierIds::type).toString();

        if (widgetType == CabbageWidgetTypes::form)
            setupWindow (widgets.getChild (widget));
        else
        {
            insertWidget (widgets.getChild (widget));
        }
    }
    
    lookAndFeelChanged();
}

//======================================================================================================
void CabbagePluginEditor::setupWindow (ValueTree widgetData)
{
    instrumentName = CabbageWidgetData::getStringProp (widgetData, CabbageIdentifierIds::caption);
    setName (CabbageWidgetData::getStringProp (widgetData, CabbageIdentifierIds::caption));
    const int width = CabbageWidgetData::getNumProp (widgetData, CabbageIdentifierIds::width);
    showScrollbars = bool(CabbageWidgetData::getNumProp (widgetData, CabbageIdentifierIds::scrollbars));
    const int height = CabbageWidgetData::getNumProp (widgetData, CabbageIdentifierIds::height);
    const String backgroundColourString = CabbageWidgetData::getStringProp (widgetData, CabbageIdentifierIds::colour);
    const String titlebarColourString = CabbageWidgetData::getStringProp(widgetData, CabbageIdentifierIds::titlebarcolour);
    titlebarGradientAmount = CabbageWidgetData::getNumProp(widgetData, CabbageIdentifierIds::titlebargradient);
    const String fontColourString = CabbageWidgetData::getStringProp(widgetData, CabbageIdentifierIds::fontcolour);
    const String channelName = CabbageWidgetData::getStringProp(widgetData, CabbageIdentifierIds::channel);
    cabbageForm.setValueTree(widgetData);
    globalStyle = CabbageWidgetData::getStringProp(widgetData, CabbageIdentifierIds::style);
    
    backgroundColour = Colour::fromString (backgroundColourString);
    titlebarColour   = Colour::fromString (titlebarColourString);
    fontColour       = Colour::fromString (fontColourString);

    if (fontColourString != "")
        defaultFontColour = false;

    lookAndFeel.setColour(ScrollBar::backgroundColourId, backgroundColour);
    cabbageForm.setColour (backgroundColour);
    instrumentBounds.setXY(width, height);
    setSize (width, height);

    if(CabbageWidgetData::getNumProp(widgetData, CabbageIdentifierIds::openGL) == 1)
        attachOpenGL();
    
    repaint();
}
//======================================================================================================
void CabbagePluginEditor::addNewWidget (String widgetType, juce::Point<int> position, bool isCustomPlant)
{

    if(!isCustomPlant)
    {
        StringArray csdArray = cabbageProcessor.getCurrentCsdFileAsStringArray();
        const String widgetTreeIdentifier = "newlyAddedWidget";
        ValueTree newWidget(widgetTreeIdentifier);

        CabbageWidgetData::setWidgetState(newWidget, widgetType, components.size()+10000);
        CabbageWidgetData::setStringProp(newWidget, CabbageIdentifierIds::csdfile,
            cabbageProcessor.getCsdFile().getFullPathName());
        newWidget.setProperty(CabbageIdentifierIds::top, position.getY(), nullptr);
        newWidget.setProperty(CabbageIdentifierIds::left, position.getX(), nullptr);

        cabbageProcessor.cabbageWidgets.addChild(newWidget, -1, nullptr);

        
        setCurrentlySelectedComponents(StringArray(CabbageWidgetData::getStringProp(newWidget, CabbageIdentifierIds::name)));

        insertWidget(newWidget);
        updateLayoutEditorFrames();

        sendChangeMessage();    //update code in editor

    }
    else
    {
        changeMessage = widgetType;
        customPlantPosition = position;
        sendChangeMessage();    //update code in editor
    }
}

//======================================================================================================
void CabbagePluginEditor::insertWidget (const ValueTree& cabbageWidgetData)
{
    const String widgetType = cabbageWidgetData.getProperty (CabbageIdentifierIds::type).toString();

    if (widgetType == CabbageWidgetTypes::checkbox)
        insertCheckbox (cabbageWidgetData);

    else if (widgetType == CabbageWidgetTypes::combobox)
        insertComboBox (cabbageWidgetData);

    else if (widgetType == CabbageWidgetTypes::image)
        insertImage (cabbageWidgetData);

    else if (widgetType == CabbageWidgetTypes::line)
        insertLine (cabbageWidgetData);

    else if (widgetType == CabbageWidgetTypes::rslider
             || widgetType == CabbageWidgetTypes::vslider
             || widgetType == CabbageWidgetTypes::hslider)
        insertSlider (cabbageWidgetData);

    else if (widgetType == CabbageWidgetTypes::label)
        insertLabel (cabbageWidgetData);

    else if (widgetType == CabbageWidgetTypes::groupbox)
        insertGroupBox (cabbageWidgetData);

    else if (widgetType == CabbageWidgetTypes::keyboard)
        insertKeyboard (cabbageWidgetData);

	else if (widgetType == CabbageWidgetTypes::keyboarddisplay)
		insertKeyboardDisplay(cabbageWidgetData);

    else if (widgetType == CabbageWidgetTypes::csoundoutput)
        insertCsoundOutputConsole (cabbageWidgetData);

    else if (widgetType == CabbageWidgetTypes::nslider)
        insertNumberSlider (cabbageWidgetData);

    else if (widgetType == CabbageWidgetTypes::textbox)
        insertTextBox (cabbageWidgetData);

    else if (widgetType == CabbageWidgetTypes::texteditor)
        insertTextEditor (cabbageWidgetData);

    else if (widgetType == CabbageWidgetTypes::encoder)
        insertEncoder (cabbageWidgetData);

    else if (widgetType == CabbageWidgetTypes::soundfiler)
        insertSoundfiler (cabbageWidgetData);

    else if (widgetType == CabbageWidgetTypes::button)
        insertButton (cabbageWidgetData);

    else if (widgetType == CabbageWidgetTypes::filebutton)
        insertFileButton (cabbageWidgetData);

    else if (widgetType == CabbageWidgetTypes::presetbutton)
        insertPresetButton (cabbageWidgetData);
    
    else if (widgetType == CabbageWidgetTypes::infobutton)
        insertInfoButton (cabbageWidgetData);

    else if (widgetType == CabbageWidgetTypes::optionbutton)
        insertOptionButton (cabbageWidgetData);
    
    else if (widgetType == CabbageWidgetTypes::signaldisplay)
        insertSignalDisplay (cabbageWidgetData);

    else if (widgetType == CabbageWidgetTypes::gentable)
        insertGenTable (cabbageWidgetData);

    else if (widgetType == CabbageWidgetTypes::xypad)
        insertXYPad (cabbageWidgetData);

    else if (widgetType == CabbageWidgetTypes::listbox)
        insertListBox (cabbageWidgetData);

    else if (widgetType == CabbageWidgetTypes::screw)
        insertScrew (cabbageWidgetData);

    else if (widgetType == CabbageWidgetTypes::light)
        insertLight (cabbageWidgetData);
 
    else if (widgetType == CabbageWidgetTypes::cvinput || widgetType == CabbageWidgetTypes::cvoutput)
        insertPort (cabbageWidgetData);
    
    else if (widgetType == CabbageWidgetTypes::eventsequencer)
        insertStringSequencer (cabbageWidgetData);

    else if (widgetType == CabbageWidgetTypes::hmeter || widgetType == CabbageWidgetTypes::vmeter)
        insertMeter (cabbageWidgetData);

    else if (widgetType == CabbageWidgetTypes::path)
        insertPath (cabbageWidgetData);
    
    else if (widgetType == CabbageWidgetTypes::hrange
             || widgetType == CabbageWidgetTypes::vrange)
        insertRangeSlider (cabbageWidgetData);

}

/* these could be replaced with a templated function */

void CabbagePluginEditor::insertCheckbox (const ValueTree& cabbageWidgetData)
{
    CabbageCheckbox* checkbox;
    components.add (checkbox = new CabbageCheckbox (cabbageWidgetData, this));
    checkbox->addListener (this);
    addToEditorAndMakeVisible (checkbox, cabbageWidgetData);
    addMouseListenerAndSetVisibility (checkbox, cabbageWidgetData);
}

void CabbagePluginEditor::insertComboBox (const ValueTree& cabbageWidgetData)
{
    CabbageComboBox* combobox;
    components.add (combobox = new CabbageComboBox (cabbageWidgetData, this));

    if (CabbageWidgetData::getStringProp (cabbageWidgetData, CabbageIdentifierIds::filetype).contains ("snaps")
        || CabbageWidgetData::getStringProp (cabbageWidgetData, CabbageIdentifierIds::channeltype).contains ("string"))
        combobox->addListener (combobox);
    else
        combobox->addListener (this);

    addToEditorAndMakeVisible (combobox, cabbageWidgetData);
    addMouseListenerAndSetVisibility (combobox, cabbageWidgetData);
}

void CabbagePluginEditor::insertRangeSlider (const ValueTree& cabbageWidgetData)
{
    CabbageRangeSlider* rangeSlider;
    components.add (rangeSlider = new CabbageRangeSlider (cabbageWidgetData, this));
    rangeSlider->getSlider().addListener (this);
    addToEditorAndMakeVisible (rangeSlider, cabbageWidgetData);
    addMouseListenerAndSetVisibility (rangeSlider, cabbageWidgetData);
}

void CabbagePluginEditor::insertLabel (const ValueTree& cabbageWidgetData)
{
    CabbageLabel* label;
    components.add (label = new CabbageLabel (cabbageWidgetData, this));
    addToEditorAndMakeVisible (label, cabbageWidgetData);
    addMouseListenerAndSetVisibility (label, cabbageWidgetData);
}

void CabbagePluginEditor::insertListBox (const ValueTree& cabbageWidgetData)
{
    CabbageListBox* label;
    components.add (label = new CabbageListBox (cabbageWidgetData, this));
    addToEditorAndMakeVisible (label, cabbageWidgetData);
    addMouseListenerAndSetVisibility (label, cabbageWidgetData);
}

void CabbagePluginEditor::insertStringSequencer (const ValueTree& cabbageWidgetData)
{
    CabbageEventSequencer* stringSeq;
    components.add (stringSeq = new CabbageEventSequencer (cabbageWidgetData, this));
    addToEditorAndMakeVisible (stringSeq, cabbageWidgetData);
    addMouseListenerAndSetVisibility (stringSeq, cabbageWidgetData);
}

void CabbagePluginEditor::insertMeter (const ValueTree& cabbageWidgetData)
{
    CabbageMeter* meter;
    components.add (meter = new CabbageMeter (cabbageWidgetData, this));
    addToEditorAndMakeVisible (meter, cabbageWidgetData);
    addMouseListenerAndSetVisibility (meter, cabbageWidgetData);
}

void CabbagePluginEditor::insertPath (const ValueTree& cabbageWidgetData)
{
    CabbagePath* path;
    components.add (path = new CabbagePath (cabbageWidgetData, this));
    addToEditorAndMakeVisible (path, cabbageWidgetData);
    addMouseListenerAndSetVisibility (path, cabbageWidgetData);
}

void CabbagePluginEditor::insertTextEditor (const ValueTree& cabbageWidgetData)
{
    CabbageTextEditor* editor;
    components.add (editor = new CabbageTextEditor (cabbageWidgetData, this));
    addToEditorAndMakeVisible (editor, cabbageWidgetData);
    addMouseListenerAndSetVisibility (editor, cabbageWidgetData);
}

void CabbagePluginEditor::insertSoundfiler (const ValueTree& cabbageWidgetData)
{
    CabbageSoundfiler* soundfiler;
    components.add (soundfiler = new CabbageSoundfiler (cabbageWidgetData, this, processor.getSampleRate()));
    addToEditorAndMakeVisible (soundfiler, cabbageWidgetData);
    addMouseListenerAndSetVisibility (soundfiler, cabbageWidgetData);
}

void CabbagePluginEditor::insertSlider (const ValueTree& cabbageWidgetData)
{
    CabbageSlider* slider;
    components.add (slider = new CabbageSlider (cabbageWidgetData, this));
    slider->getSlider().addListener (this);
    addToEditorAndMakeVisible (slider, cabbageWidgetData);
    addMouseListenerAndSetVisibility (slider, cabbageWidgetData);
}

void CabbagePluginEditor::insertEncoder (const ValueTree& cabbageWidgetData)
{
    CabbageEncoder* encoder;
    components.add (encoder = new CabbageEncoder (cabbageWidgetData, this));
    addToEditorAndMakeVisible (encoder, cabbageWidgetData);
    addMouseListenerAndSetVisibility (encoder, cabbageWidgetData);
}

void CabbagePluginEditor::insertXYPad (const ValueTree& cabbageWidgetData)
{
    CabbageXYPad* xyPad;
    components.add (xyPad = new CabbageXYPad (cabbageWidgetData, this));
    xyPad->getSliderX().addListener (this);
    xyPad->getSliderY().addListener (this);
    addToEditorAndMakeVisible (xyPad, cabbageWidgetData);
    cabbageProcessor.addXYAutomator (xyPad, cabbageWidgetData);
    addMouseListenerAndSetVisibility (xyPad, cabbageWidgetData);
}

void CabbagePluginEditor::insertFileButton (const ValueTree& cabbageWidgetData)
{
    CabbageFileButton* fileButton;
    components.add (fileButton = new CabbageFileButton (cabbageWidgetData, this));
    addToEditorAndMakeVisible (fileButton, cabbageWidgetData);
    addMouseListenerAndSetVisibility (fileButton, cabbageWidgetData);
}

void CabbagePluginEditor::insertPresetButton (const ValueTree& cabbageWidgetData)
{
    CabbagePresetButton* presetButton;
    components.add (presetButton = new CabbagePresetButton (cabbageWidgetData, this));
    addToEditorAndMakeVisible (presetButton, cabbageWidgetData);
    addMouseListenerAndSetVisibility (presetButton, cabbageWidgetData);
}

void CabbagePluginEditor::insertSignalDisplay (const ValueTree& cabbageWidgetData)
{
    CabbageSignalDisplay* signalDisplay;
    components.add (signalDisplay = new CabbageSignalDisplay (cabbageWidgetData, this));
    addToEditorAndMakeVisible (signalDisplay, cabbageWidgetData);
    addMouseListenerAndSetVisibility (signalDisplay, cabbageWidgetData);
}

void CabbagePluginEditor::insertInfoButton (const ValueTree& cabbageWidgetData)
{
    CabbageInfoButton* infoButton;
    components.add (infoButton = new CabbageInfoButton (cabbageWidgetData, this, globalStyle));
    addToEditorAndMakeVisible (infoButton, cabbageWidgetData);
    addMouseListenerAndSetVisibility (infoButton, cabbageWidgetData);
}

void CabbagePluginEditor::insertOptionButton (const ValueTree& cabbageWidgetData)
{
    CabbageOptionButton* optionButton;
    components.add (optionButton = new CabbageOptionButton (cabbageWidgetData, this));
    optionButton->addListener (this);
    addToEditorAndMakeVisible (optionButton, cabbageWidgetData);
    addMouseListenerAndSetVisibility (optionButton, cabbageWidgetData);
}

void CabbagePluginEditor::insertButton (const ValueTree& cabbageWidgetData)
{
    CabbageButton* button;
    components.add (button = new CabbageButton (cabbageWidgetData, this));
    button->addListener (this);
    addToEditorAndMakeVisible (button, cabbageWidgetData);
    addMouseListenerAndSetVisibility (button, cabbageWidgetData);
}

void CabbagePluginEditor::insertNumberSlider (const ValueTree& cabbageWidgetData)
{
    CabbageNumberSlider* numberBox;
    components.add (numberBox = new CabbageNumberSlider (cabbageWidgetData, this));
    numberBox->getSlider().addListener (this);
    addToEditorAndMakeVisible (numberBox, cabbageWidgetData);
    addMouseListenerAndSetVisibility (numberBox, cabbageWidgetData);
}

void CabbagePluginEditor::insertGenTable (const ValueTree& cabbageWidgetData)
{
    CabbageGenTable* genTable;
    components.add (genTable = new CabbageGenTable (cabbageWidgetData, this));
    addToEditorAndMakeVisible (genTable, cabbageWidgetData);
    addMouseListenerAndSetVisibility (genTable, cabbageWidgetData);
}

void CabbagePluginEditor::insertTextBox (const ValueTree& cabbageWidgetData)
{
    CabbageTextBox* textBox;
    components.add (textBox = new CabbageTextBox (cabbageWidgetData, this));
    addToEditorAndMakeVisible (textBox, cabbageWidgetData);
    addMouseListenerAndSetVisibility (textBox, cabbageWidgetData);
}

void CabbagePluginEditor::insertCsoundOutputConsole (const ValueTree& cabbageWidgetData)
{
    if (consoleCount < 1)
    {
        CabbageCsoundConsole* csoundConsole;
        components.add (csoundConsole = new CabbageCsoundConsole (cabbageWidgetData, this));
        addToEditorAndMakeVisible (csoundConsole, cabbageWidgetData);
        addMouseListenerAndSetVisibility (csoundConsole, cabbageWidgetData);
        consoleCount++;
    }
}

void CabbagePluginEditor::insertKeyboard (const ValueTree& cabbageWidgetData)
{
    if (keyboardCount < 1)
    {
        CabbageKeyboard* midiKeyboard;
        components.add (midiKeyboard = new CabbageKeyboard (cabbageWidgetData, this, cabbageProcessor.keyboardState));
        //midiKeyboard->setKeyPressBaseOctave (3); // <-- now you can set this with 'keypressbaseoctave' identifier
        
//#if !Cabbage_IDE_Build
//        for(int i = 0 ; i < 128 ; i++)
//            midiKeyboard->removeKeyPressForNote(i);
//#endif
        addToEditorAndMakeVisible (midiKeyboard, cabbageWidgetData);
        addMouseListenerAndSetVisibility (midiKeyboard, cabbageWidgetData);
        keyboardCount++;
        

    }
}

void CabbagePluginEditor::insertKeyboardDisplay(const ValueTree& cabbageWidgetData)
{
	CabbageKeyboardDisplay* midiKeyboard;
	MidiKeyboardState dummy;
	components.add(midiKeyboard = new CabbageKeyboardDisplay(cabbageWidgetData, this));
	//midiKeyboard->setKeyPressBaseOctave (3); // <-- now you can set this with 'keypressbaseoctave' identifier
	addToEditorAndMakeVisible(midiKeyboard, cabbageWidgetData);
	addMouseListenerAndSetVisibility(midiKeyboard, cabbageWidgetData);
}
//======================================================================================================
void CabbagePluginEditor::insertGroupBox (const ValueTree& cabbageWidgetData)
{
    CabbageGroupBox* groupBox;
    components.add (groupBox = new CabbageGroupBox (cabbageWidgetData, this));
    addToEditorAndMakeVisible (groupBox, cabbageWidgetData);
    addMouseListenerAndSetVisibility (groupBox, cabbageWidgetData);
    addPlantToPopupPlantsArray (cabbageWidgetData, groupBox); // only groupboxes and images can be plants
}

void CabbagePluginEditor::insertImage (const ValueTree& cabbageWidgetData)
{
    CabbageImage* image;
    components.add (image = new CabbageImage (cabbageWidgetData, this));
    addToEditorAndMakeVisible (image, cabbageWidgetData);
    addMouseListenerAndSetVisibility (image, cabbageWidgetData);
    addPlantToPopupPlantsArray (cabbageWidgetData, image); // only groupboxes and images can be plants
}

void CabbagePluginEditor::insertLine (const ValueTree& cabbageWidgetData)
{
    CabbageImage* line;
    components.add (line = new CabbageImage (cabbageWidgetData, this, true));
    addToEditorAndMakeVisible (line, cabbageWidgetData);
    addMouseListenerAndSetVisibility (line, cabbageWidgetData);
}

void CabbagePluginEditor::insertPort (const ValueTree& cabbageWidgetData)
{
    CabbagePort* port;
    components.add (port = new CabbagePort (cabbageWidgetData, this));
    addToEditorAndMakeVisible (port, cabbageWidgetData);
    addMouseListenerAndSetVisibility (port, cabbageWidgetData);
}

void CabbagePluginEditor::insertScrew (const ValueTree& cabbageWidgetData)
{
    CabbageScrew* screw;
    components.add (screw = new CabbageScrew (cabbageWidgetData, this));
    addToEditorAndMakeVisible (screw, cabbageWidgetData);
    addMouseListenerAndSetVisibility (screw, cabbageWidgetData);
}

void CabbagePluginEditor::insertLight (const ValueTree& cabbageWidgetData)
{
    CabbageLight* light;
    components.add (light = new CabbageLight (cabbageWidgetData, this));
    addToEditorAndMakeVisible (light, cabbageWidgetData);
    addMouseListenerAndSetVisibility (light, cabbageWidgetData);
}
//======================================================================================================
CabbagePluginParameter* CabbagePluginEditor::getParameterForComponent (const String& name)
{
    for (auto param : cabbageProcessor.getCabbageParameters())
    {
        if (name == param->getWidgetName())
            return param;
    }

    return nullptr;
}

//======================================================================================================
void CabbagePluginEditor::resizePlugin (int sizeIndex)
{
    
    
    cabbageProcessor.currentPluginScale =  sizeIndex;
    //param->setValueNotifyingHost (param->getNormalisableRange().convertTo0to1 (combo->getSelectedItemIndex()+1));
    
    
    setScaleFactor(pluginSizes[cabbageProcessor.currentPluginScale-1]);
    resized();
}
//======================================================================================================
void CabbagePluginEditor::comboBoxChanged (ComboBox* combo)
{
    const String mode = CabbageWidgetData::getStringProp (getValueTreeForComponent (combo->getName()), CabbageIdentifierIds::mode);
    const String channel = CabbageWidgetData::getStringProp (getValueTreeForComponent (combo->getName()), CabbageIdentifierIds::channel);
    int range = CabbageWidgetData::getNumProp (getValueTreeForComponent (combo->getName()), CabbageIdentifierIds::comborange);
    if (CabbagePluginParameter* param = getParameterForComponent (combo->getName()))
    {
        if(mode == "resize")
        {
            if(combo->getSelectedItemIndex() != -1)
            {
//                param->setValueNotifyingHost (param->getNormalisableRange().convertTo0to1 (combo->getSelectedItemIndex()+1));
                CabbageWidgetData::setNumProp(getValueTreeForComponent (combo->getName()), CabbageIdentifierIds::value, combo->getSelectedItemIndex()+1);
                resizePlugin(combo->getSelectedId());
            }
        }
        else
        {
            param->beginChangeGesture();
            //preset combos work with 0 index, Cabbage string combos start at 1..
            if (CabbageWidgetData::getStringProp (getValueTreeForComponent (combo->getName()), CabbageIdentifierIds::filetype).contains ("snaps"))
                param->setValueNotifyingHost (param->getNormalisableRange().convertTo0to1 (combo->getSelectedItemIndex()));
            else
                param->setValueNotifyingHost (param->getNormalisableRange().convertTo0to1 (juce::jmin(combo->getSelectedItemIndex()+1, range)));
            param->endChangeGesture();
        }
        
    }
}
//======================================================================================================
//======================================================================================================
void CabbagePluginEditor::buttonClicked(Button* button)
{
	const bool buttonState = button->getToggleState();

	if (CabbageButton* cabbageButton = dynamic_cast<CabbageButton*> (button))
	{
		const StringArray textItems = cabbageButton->getTextArray();
		const ValueTree valueTree = CabbageWidgetData::getValueTreeForComponent(cabbageProcessor.cabbageWidgets, cabbageButton->getName());
		const int latched = CabbageWidgetData::getNumProp(valueTree, CabbageIdentifierIds::latched);

		if (textItems.size() > 0)
			cabbageButton->setButtonText(textItems[!buttonState ? 0 : 1]);

		if (latched == 1)
			toggleButtonState(button, buttonState);

		return;
	}
	else if (CabbageCheckbox* checkButton = dynamic_cast<CabbageCheckbox*> (button))
	{
		const StringArray textItems = checkButton->getTextArray();
		const ValueTree valueTree = CabbageWidgetData::getValueTreeForComponent(cabbageProcessor.cabbageWidgets, checkButton->getName());
		// const int latched = CabbageWidgetData::getNumProp(valueTree, CabbageIdentifierIds::latched);

		if (textItems.size() > 0)
            checkButton->setButtonText(textItems[!buttonState ? 0 : 1]);

		toggleButtonState(button, buttonState);
		return;
	}
    else if (CabbageOptionButton* optionButton = dynamic_cast<CabbageOptionButton*> (button))
    {
        const ValueTree valueTree = CabbageWidgetData::getValueTreeForComponent(cabbageProcessor.cabbageWidgets, optionButton->getName());
        int value = CabbageWidgetData::getNumProp(valueTree, CabbageIdentifierIds::value);
        var textArray = CabbageWidgetData::getProperty(valueTree, CabbageIdentifierIds::text);
        int comboRange = textArray.size();
        value = (value < comboRange -1 ? value + 1 : 0);
        //CabbageWidgetData::setNumProp(valueTree, CabbageIdentifierIds::value, value);
        if (CabbagePluginParameter* param = getParameterForComponent(button->getName()))
        {
            param->beginChangeGesture();
            param->setValueNotifyingHost (param->getNormalisableRange().convertTo0to1 (value));
            param->endChangeGesture();
        }
        
        return;
    }

}

void CabbagePluginEditor::buttonStateChanged(Button* button)
{
	if (CabbageButton* cabbageButton = dynamic_cast<CabbageButton*> (button))
	{
		const ValueTree valueTree = CabbageWidgetData::getValueTreeForComponent(cabbageProcessor.cabbageWidgets, cabbageButton->getName());
		const int latched = CabbageWidgetData::getNumProp(valueTree, CabbageIdentifierIds::latched);
        //DBG("ValueTreeListener:"+cabbageButton->getName());
        
		if (latched == 0)
		{
			if (button->isMouseButtonDown())
				toggleButtonState(button, true);
			else
				toggleButtonState(button, false);
		}
		else if (latched == 1)
		{
//            int value = CabbageWidgetData::getNumProp(valueTree, CabbageIdentifierIds::value);
//            if (button->isMouseButtonDown() && value == 1)
//                toggleButtonState(button, false);
//            else if (button->isMouseButtonDown() && value == 0)
//				toggleButtonState(button, false);
		}
	}

}

void CabbagePluginEditor::toggleButtonState(Button* button, bool state)
{
	if (CabbagePluginParameter* param = getParameterForComponent(button->getName()))
	{
		param->beginChangeGesture();
		param->setValueNotifyingHost(state ? 1 : 0);
		param->endChangeGesture();
	}
}
//======================================================================================================
void CabbagePluginEditor::sliderValueChanged (Slider* slider)
{
    
    if (slider->getSliderStyle() != Slider::TwoValueHorizontal && slider->getSliderStyle() != Slider::TwoValueVertical)
    {
        if (CabbagePluginParameter* param = getParameterForComponent (slider->getName()))
        {
            param->setValueNotifyingHost (param->getNormalisableRange().convertTo0to1 (slider->getValue()));
        }
    }
    else
    {
        if (CabbagePluginParameter* param = getParameterForComponent (slider->getName() + "_min"))
        {
            param->setValueNotifyingHost (param->getNormalisableRange().convertTo0to1 (slider->getMinValue()));
        }

        if (CabbagePluginParameter* param = getParameterForComponent (slider->getName() + "_max"))
        {
            param->setValueNotifyingHost (param->getNormalisableRange().convertTo0to1 (slider->getMaxValue()));
        }
    }
}

void CabbagePluginEditor::sliderDragStarted(Slider* slider)
{
    isSliderDragging = true;
    if (slider->getSliderStyle() != Slider::TwoValueHorizontal && slider->getSliderStyle() != Slider::TwoValueVertical)
    {
        if (CabbagePluginParameter* param = getParameterForComponent(slider->getName()))
        {
#if !Cabbage_IDE_Build
            if(pluginType.isAbletonLive())
                if(param->isPerformingGesture)
                    param->endChangeGesture();
#endif
            param->beginChangeGesture();
        }
    }
    else
    {
        if (CabbagePluginParameter* param = getParameterForComponent(slider->getName() + "_min"))
        {
            param->beginChangeGesture();
        }

        if (CabbagePluginParameter* param = getParameterForComponent(slider->getName() + "_max"))
        {
            param->beginChangeGesture();
        }
    }
}
void CabbagePluginEditor::sliderDragEnded(Slider* slider)
{
    // If the Csound pointer is null then the given slider is invalid and should not be used (its private pimpl pointer is null and will cause a crash).
    if (!cabbageProcessor.getEngine())
        return;
    
    if (slider->getSliderStyle() != Slider::TwoValueHorizontal && slider->getSliderStyle() != Slider::TwoValueVertical)
    {
        if (CabbagePluginParameter* param = getParameterForComponent(slider->getName()))
        {
#if !Cabbage_IDE_Build
            if(!pluginType.isAbletonLive())
#endif
            param->endChangeGesture();
        }
    }
    else
    {
        if (CabbagePluginParameter* param = getParameterForComponent(slider->getName() + "_min"))
        {
            param->endChangeGesture();
        }

        if (CabbagePluginParameter* param = getParameterForComponent(slider->getName() + "_max"))
        {
            param->endChangeGesture();
        }
    }
}
//======================================================================================================
void CabbagePluginEditor::enableEditMode (bool enable)
{
#if Cabbage_IDE_Build
    layoutEditor.setEnabled (enable);
    editModeEnabled = enable;
    layoutEditor.toFront (false);
//    if(enable)
//        viewport->setViewedComponent(&layoutEditor, false);
//    else
//        viewport->setViewedComponent(&cabbageForm, false);

#endif
}
//======================================================================================================
void CabbagePluginEditor::setCurrentlySelectedComponents (StringArray componentNames)
{
    currentlySelectedComponentNames = componentNames;
}

void CabbagePluginEditor::resetCurrentlySelectedComponents()
{
    currentlySelectedComponentNames.clear();
}

Component* CabbagePluginEditor::getComponentFromName (String& name)
{
    for (auto comp : components)
    {
        if (name == comp->getName())
            return comp;
    }

    return nullptr;
}

Array<ValueTree> CabbagePluginEditor::getValueTreesForCurrentlySelectedComponents()
{
    Array<ValueTree> valueTreeArray;

    for (const String& compName : currentlySelectedComponentNames)
        valueTreeArray.add (CabbageWidgetData::getValueTreeForComponent (cabbageProcessor.cabbageWidgets, compName));

    return valueTreeArray;
}

ValueTree CabbagePluginEditor::getValueTreeForComponent (String compName)
{
    if(compName == "form")//special case
    {
        resetCurrentlySelectedComponents();
        return CabbageWidgetData::getValueTreeForComponent (cabbageProcessor.cabbageWidgets, "form");
    }
    else if(getComponentFromName (compName))
        return CabbageWidgetData::getValueTreeForComponent (cabbageProcessor.cabbageWidgets, getComponentFromName (compName)->getName());
    
    else
        return {};
}

void CabbagePluginEditor::updateLayoutEditorFrames()
{
#if Cabbage_IDE_Build

    if (editModeEnabled)
        layoutEditor.updateFrames();

#endif
}

//======================================================================================================
void CabbagePluginEditor::addToEditorAndMakeVisible (Component* comp, ValueTree widgetData)
{
    String parent = CabbageWidgetData::getStringProp (widgetData, CabbageIdentifierIds::parentcomponent);


    if (auto parentComp = getComponentFromName (parent))
    {
        parentComp->addAndMakeVisible (comp);
    }
    else
        cabbageForm.addAndMakeVisible (comp);

    if(comp->getWidth()+comp->getX() > cabbageForm.getWidth())
        instrumentBounds.setX(comp->getWidth()+comp->getX());

    if(comp->getHeight()+comp->getY() > cabbageForm.getHeight())
        instrumentBounds.setY(comp->getHeight()+comp->getY());


}

void CabbagePluginEditor::addMouseListenerAndSetVisibility (Component* comp, ValueTree wData)
{
    comp->addMouseListener (this, true);
    int visible = CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::visible);
    comp->setVisible (visible==1 || 0);
}

void CabbagePluginEditor::addPlantToPopupPlantsArray (const ValueTree& wData, Component* plant)
{
    const int isPopup = CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::popup);

    if (isPopup == 1)
    {
        const String caption = CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::text);
        const String name = CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::name);
        PopupDocumentWindow* popupPlant;
        popupPlants.add (popupPlant = new PopupDocumentWindow (caption, backgroundColour));
        popupPlant->setLookAndFeel (&getLookAndFeel());
        popupPlant->setWidgetData(wData);
        popupPlant->setContentNonOwned (plant, true);
        popupPlant->setName (name);
    }
}
//======================================================================================================
void CabbagePluginEditor::updatefTableData (GenTable* table)
{
    if (!cabbageProcessor.getEngine())
        return;
    
    Array<double> pFields = table->getPfields();

    if ( table->genRoutine == 5 || table->genRoutine == 7 || table->genRoutine == 2)
    {
        FUNC* ftpp;
        EVTBLK  evt;
#ifdef JUCE_WINDOWS
        ZeroMemory(&evt, sizeof(EVTBLK));
#else
        memset (&evt, 0, sizeof (EVTBLK));
#endif
        evt.pcnt = 5 + pFields.size();
        evt.opcod = 'f';
        evt.p[0] = 0;

        //setting table number to 0.
        evt.p[1] = 0;
        evt.p[2] = 0;
        evt.p[3] = table->tableSize;
        evt.p[4] = table->realGenRoutine;

        if (table->genRoutine == 5)
        {
            for (int i = 0; i < pFields.size() - 1; i++)
                evt.p[5 + i] = jmax (0.00001, pFields[i + 1]);
        }
        else if (table->genRoutine == 7)
        {
            for (int i = 0; i < pFields.size() - 1; i++)
                evt.p[5 + i] = pFields[i + 1];
        }
        else
        {
            for (int i = 0; i < pFields.size(); i++)
                evt.p[5 + i] = pFields[i];
        }

        StringArray fStatement;
        int pCnt = 0;

        for (int i = 0; i < evt.pcnt - 1; i++)
        {
            fStatement.add (String (evt.p[i]));
            pCnt = i;
        }

        if (table->genRoutine != 2)
        {
            fStatement.add (String (1));
            fStatement.add (String (evt.p[pCnt]));
        }

        //now set table number and set score char to f
        fStatement.set (1, String (table->tableNumber));
        fStatement.set (0, "f");

        cabbageProcessor.getEngine()->GetCsound()->hfgens (cabbageProcessor.getEngine()->GetCsound(), &ftpp, &evt, 1);
        Array<float, CriticalSection> points;

        points = Array<float, CriticalSection> (ftpp->ftable, static_cast<int>(ftpp->flen));
        table->setWaveform (points, false);
        //table->enableEditMode(fStatement);

        cabbageProcessor.getEngine()->InputMessage (fStatement.joinIntoString (" ").toUTF8());
    }

}

//======================================================================================================
void CabbagePluginEditor::sendChannelDataToCsound (const String& channel, float value)
{
    if (csdCompiledWithoutError() && cabbageProcessor.getEngine())
        cabbageProcessor.getEngine()->SetChannel (channel.getCharPointer(), value);
}

float CabbagePluginEditor::getChannelDataFromCsound (const String& channel)
{
    if (csdCompiledWithoutError() && cabbageProcessor.getEngine())
        return cabbageProcessor.getEngine()->GetChannel (channel.getCharPointer());
    
    return 0;
}

void CabbagePluginEditor::sendChannelStringDataToCsound (const String& channel, String value)
{
    if (cabbageProcessor.csdCompiledWithoutError() && cabbageProcessor.getEngine())
        cabbageProcessor.getEngine()->SetStringChannel (channel.getCharPointer(), value.toUTF8().getAddress());
}

void CabbagePluginEditor::sendScoreEventToCsound (const String& scoreEvent)
{
    if (cabbageProcessor.csdCompiledWithoutError() && cabbageProcessor.getEngine())
        cabbageProcessor.getEngine()->InputMessage(scoreEvent.toUTF8());
}

void CabbagePluginEditor::createEventMatrix(int cols, int rows, String channel)
{
    if (cabbageProcessor.csdCompiledWithoutError())
        cabbageProcessor.createMatrixEventSequencer(rows, cols, std::move(channel));
}

void CabbagePluginEditor::setEventMatrixData(int cols, int rows, const String& channel, String data)
{
    if (cabbageProcessor.csdCompiledWithoutError())
        cabbageProcessor.setMatrixEventSequencerCellData(cols, rows, channel, data);
}


Array<float, CriticalSection> CabbagePluginEditor::getArrayForSignalDisplay (const String signalVariable, const String displayType)
{
    if (csdCompiledWithoutError())
        return cabbageProcessor.getSignalArray (signalVariable, displayType)->getPoints();

    return Array<float, CriticalSection>();
}

bool CabbagePluginEditor::shouldUpdateSignalDisplay(String signalVariableName)
{
    return cabbageProcessor.shouldUpdateSignalDisplay(signalVariableName);
}

void CabbagePluginEditor::enableXYAutomator (String name, bool enable, Line<float> dragLine)
{
    cabbageProcessor.enableXYAutomator (name, enable, dragLine);
}

void CabbagePluginEditor::disableXYAutomators()
{
    cabbageProcessor.disableXYAutomators();
}

bool CabbagePluginEditor::csdCompiledWithoutError()
{
    return cabbageProcessor.csdCompiledWithoutError();
}

StringArray CabbagePluginEditor::getTableStatement (int tableNumber)
{
    if (csdCompiledWithoutError())
        return cabbageProcessor.getTableStatement (tableNumber);

    return {};
}

const Array<float, CriticalSection> CabbagePluginEditor::getTableFloats (int tableNumber)
{
    if (csdCompiledWithoutError())
        return cabbageProcessor.getTableFloats (tableNumber);

    return Array<float, CriticalSection>();
}

CabbagePluginProcessor& CabbagePluginEditor::getProcessor()
{
    return cabbageProcessor;
}

void CabbagePluginEditor::savePluginStateToFile (String presetName, const String& fileName, bool removePreset)
{
    //the error happens when we create a new preset not when we modify or delete one...
    cabbageProcessor.addPluginPreset (presetName, fileName, removePreset);
}

void CabbagePluginEditor::restorePluginStateFrom (String presetName, String filename)
{
    cabbageProcessor.restorePluginPreset (presetName, filename);
}

void CabbagePluginEditor::refreshComboListBoxContents(const String&)
{
    for ( int i = 0 ; i < cabbageProcessor.cabbageWidgets.getNumChildren() ; i++)
    {
        const String type = CabbageWidgetData::getStringProp (cabbageProcessor.cabbageWidgets.getChild (i), CabbageIdentifierIds::type);

        if ( type == "combobox" || type == "listbox")
        {
            String name = CabbageWidgetData::getStringProp (cabbageProcessor.cabbageWidgets.getChild (i), CabbageIdentifierIds::name);
            const String fileType = CabbageWidgetData::getProperty (cabbageProcessor.cabbageWidgets.getChild (i), CabbageIdentifierIds::filetype);


            if (auto* combo = dynamic_cast<CabbageComboBox*> (getComponentFromName (name)))
            {
                if (fileType.isNotEmpty())
                {
                    combo->addItemsToCombobox (cabbageProcessor.cabbageWidgets.getChild (i));
                }

                if(bool(combo->getProperties().getWithDefault("isPresetCombo", false)))
                {
                    combo->setText(getCurrentPreset());
                }
            }

            else if (auto* listbox = dynamic_cast<CabbageListBox*> (getComponentFromName (name)))
            {
                if (fileType.isNotEmpty())
                {
                    listbox->addItemsToListbox(cabbageProcessor.cabbageWidgets.getChild (i));
                }

                if(bool(listbox->getProperties().getWithDefault("isPresetCombo", false)))
                    listbox->listBox.selectRow(listbox->stringItems.size()-1);
            }

        }
    }
}

//======================================================================================================
String CabbagePluginEditor::getCsoundOutputFromProcessor()
{
    return cabbageProcessor.getCsoundOutput();
}
