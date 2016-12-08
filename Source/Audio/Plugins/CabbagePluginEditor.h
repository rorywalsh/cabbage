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

#ifndef CABBAGEPLUGINEDITOR_H_INCLUDED
#define CABBAGEPLUGINEDITOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "CabbagePluginProcessor.h"
#include "CabbageAudioParameter.h"

#include "../../InterfaceEditor/ComponentLayoutEditor.h"

#include "../../Widgets/CabbageCheckbox.h"
#include "../../Widgets/CabbageComboBox.h"
#include "../../Widgets/CabbageImage.h"
#include "../../Widgets/CabbageGroupBox.h"
#include "../../Widgets/CabbageNumberBox.h"
#include "../../Widgets/CabbageEncoder.h"
#include "../../Widgets/CabbageTextBox.h"
#include "../../Widgets/CabbageTextEditor.h"
#include "../../Widgets/CabbageCsoundConsole.h"
#include "../../Widgets/CabbageLabel.h"
#include "../../Widgets/CabbageKeyboard.h"
#include "../../Widgets/CabbageSlider.h"
#include "../../Widgets/CabbageCustomWidgets.h"

class CabbagePluginEditor;

//==============================================================================
static CabbagePluginEditor* getPluginEditor(Component* child)
{
    if(CabbagePluginEditor* c = child->findParentComponentOfClass<CabbagePluginEditor>())
        return c;
    else
        return nullptr;
}

//==============================================================================
class CabbagePluginEditor  
	: public AudioProcessorEditor, 
	public Button::Listener,
    public ChangeBroadcaster, 
	public ComboBoxListener,
	public Slider::Listener
{
public:
    CabbagePluginEditor (CabbagePluginProcessor&);
    ~CabbagePluginEditor();

    void createEditorInterface(ValueTree widgets);
    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    //==============================================================================
    void setupWindow(ValueTree cabbageWidgetData);

    void insertWidget(ValueTree cabbageWidgetData);
    // the following methods instantiate controls that CAN
    // be automated in a host...
    void insertSlider(ValueTree cabbageWidgetData);
    void insertComboBox(ValueTree cabbageWidgetData);
    void insertButton(ValueTree cabbageWidgetData) {};
    void insertCheckbox(ValueTree cabbageWidgetData);
    void insertXYPad(ValueTree cabbageWidgetData) {};
    void insertRangeSlider(ValueTree cabbageWidgetData) {};
	void insertNumberBox(ValueTree cabbageWidgetData);
	void insertEncoder(ValueTree cabbageWidgetData);
    //the following methods instantiate controls that CANNOT
    // be automated in a host...
    void insertGroupBox(ValueTree cabbageWidgetData);
    void insertSoundfiler(ValueTree cabbageWidgetData) {};
    void insertSourceButton(ValueTree cabbageWidgetData) {};
    void insertDirectoryList(ValueTree cabbageWidgetData) {};
    void insertTextEditor(ValueTree cabbageWidgetData);
    void insertCsoundOutputConsole(ValueTree cabbageWidgetData);
    void insertMIDIKeyboard(ValueTree cabbageWidgetData);
    void insertFileButton(ValueTree cabbageWidgetData) {};
    void insertRecordButton(ValueTree cabbageWidgetData) {};
    void insertImage(ValueTree cabbageWidgetData);
    void insertLabel(ValueTree cabbageWidgetData);
    void insertTable(ValueTree cabbageWidgetData) {};
    void insertMultiTab(ValueTree cabbageWidgetData) {};
    void insertInfoButton(ValueTree cabbageWidgetData) {};
    void insertLineSeparator(ValueTree cabbageWidgetData) {};
    void insertScope(ValueTree cabbageWidgetData) {};
    void insertPatternMatrix(ValueTree cabbageWidgetData) {};
    void insertSnapshot(ValueTree cabbageWidgetData) {};
    void insertTransport(ValueTree cabbageWidgetData) {};
    void insertListbox(ValueTree cabbageWidgetData) {};
    void insertPopupMenu(ValueTree cabbageWidgetData) {};
    void insertGenTable(ValueTree cabbageWidgetData) {};
    void insertTextBox(ValueTree cabbageWidgetData);
    void insertSignalDisplay(ValueTree cabbageWidgetData) {};
    void insertStepper(ValueTree cabbageWidgetData) {};
	//=============================================================================
	void sendChannelDataToCsound(String channel, float value);
	void sendChannelStringDataToCsound(String channel, String value);
	const String getCsoundOutputFromProcessor();
    //=============================================================================
    void addNewWidget(String widgetType, Point<int> point);
    ValueTree getValueTreeForlastWidgetAdded();
    //=============================================================================
    void enableEditMode(bool enable);
    void setCurrentlySelectedComponents(StringArray componentNames);
    void resetCurrentlySelectedComponents();
    Array<ValueTree> getValueTreesForCurrentlySelectedComponents();
    ValueTree getValueTreeForComponent(String compName);
    Component* getComponentFromName(String name);
    void addToEditorAndMakeVisible(Component* comp, ValueTree widgetData);
    void updateLayoutEditorFrames();
    void componentMovedOrResized (Component &component, bool wasMoved, bool wasResized);
    //=============================================================================
    void buttonClicked(Button *button);
    void comboBoxChanged (ComboBox* combo);
    void sliderValueChanged(Slider* slider);
	//=============================================================================
    CabbageAudioParameter* getParameterForComponent (Component* button);
    //=============================================================================




    ComponentLayoutEditor& getLayoutEditor()
    {
        return layoutEditor;
    }

    bool isEditModeEnabled()
    {
        return editModeEnabled;
    }

    Colour backgroundColour;
private:
    OwnedArray<Component> components;
    Component mainComponent;
	int keyboardCount=0;
	int consoleCount=0;
    CabbageLookAndFeel2 lookAndFeel;
    int newlyAddedWidgetIndex = 10000;
    TooltipWindow tooltipWindow;
    bool editModeEnabled=false;
    CabbagePluginProcessor& processor;
    ComponentLayoutEditor layoutEditor;
    StringArray currentlySelectedComponentNames;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CabbagePluginEditor)
};


#endif  // CABBAGEPLUGINEDITOR_H_INCLUDED
