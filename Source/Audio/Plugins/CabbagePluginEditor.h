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

#ifdef Cabbage_IDE_Build
    #include "../../GUIEditor/ComponentLayoutEditor.h"
#endif

#include "../../Widgets/CabbageCheckbox.h"
#include "../../Widgets/CabbageComboBox.h"
#include "../../Widgets/CabbageImage.h"
#include "../../Widgets/CabbageButton.h"
#include "../../Widgets/CabbageFileButton.h"
#include "../../Widgets/CabbageInfoButton.h"
#include "../../Widgets/CabbageGroupBox.h"
#include "../../Widgets/CabbageGenTable.h"
#include "../../Widgets/CabbageNumberSlider.h"
#include "../../Widgets/CabbageEncoder.h"
#include "../../Widgets/CabbageTextBox.h"
#include "../../Widgets/CabbageSignalDisplay.h"
#include "../../Widgets/CabbageTextEditor.h"
#include "../../Widgets/CabbageCsoundConsole.h"
#include "../../Widgets/CabbageLabel.h"
#include "../../Widgets/CabbageXYPad.h"
#include "../../Widgets/CabbageKeyboard.h"
#include "../../Widgets/CabbageSlider.h"
#include "../../Widgets/CabbageSoundfiler.h"
#include "../../Widgets/CabbageRangeSlider.h"
#include "../../Widgets/CabbageCustomWidgets.h"
#include "../../Widgets/CabbageEventSequencer.h"

class CabbagePluginEditor;

//==============================================================================
__attribute__ ((unused)) static CabbagePluginEditor* getPluginEditor (Component* child)
{
    if (CabbagePluginEditor* c = child->findParentComponentOfClass<CabbagePluginEditor>())
        return c;
    else
        return nullptr;
}

//==============================================================================
class CabbagePluginEditor
    : public AudioProcessorEditor,
      public Button::Listener,
      public ChangeBroadcaster,
      public ActionBroadcaster,
      public ComboBoxListener,
      public Slider::Listener
{
public:
    CabbagePluginEditor (CabbagePluginProcessor&);
    ~CabbagePluginEditor();

    void createEditorInterface (ValueTree widgets);
    //==============================================================================
    void resized() override;
    void paint (Graphics& g)  override{}
    //==============================================================================
    void setupWindow (ValueTree cabbageWidgetData);

    void insertWidget (ValueTree cabbageWidgetData);
    // the following methods instantiate controls that CAN
    // be automated in a host...
    void insertSlider (ValueTree cabbageWidgetData);
    void insertComboBox (ValueTree cabbageWidgetData);
    void insertButton (ValueTree cabbageWidgetData);
    void insertCheckbox (ValueTree cabbageWidgetData);
    void insertXYPad (ValueTree cabbageWidgetData);
    void insertRangeSlider (ValueTree cabbageWidgetData);
    void insertNumberSlider (ValueTree cabbageWidgetData);
    void insertEncoder (ValueTree cabbageWidgetData);
    //the following methods instantiate controls that CANNOT
    // be automated in a host...
    void insertStringSequencer (ValueTree cabbageWidgetData);
    void insertGroupBox (ValueTree cabbageWidgetData);
    void insertSoundfiler (ValueTree cabbageWidgetData);
    void insertSourceButton (ValueTree cabbageWidgetData) {};
    void insertTextEditor (ValueTree cabbageWidgetData);
    void insertCsoundOutputConsole (ValueTree cabbageWidgetData);
    void insertMIDIKeyboard (ValueTree cabbageWidgetData);
    void insertFileButton (ValueTree cabbageWidgetData);
    void insertImage (ValueTree cabbageWidgetData);
    void insertLine (ValueTree cabbageWidgetData);
    void insertLabel (ValueTree cabbageWidgetData);
    void insertTable (ValueTree cabbageWidgetData) {};
    void insertInfoButton (ValueTree cabbageWidgetData);
    void insertGenTable (ValueTree cabbageWidgetData);
    void insertTextBox (ValueTree cabbageWidgetData);
    void insertSignalDisplay (ValueTree cabbageWidgetData);
    void insertStepper (ValueTree cabbageWidgetData) {};
    void insertMeter (ValueTree cabbageWidgetData);
    void addMouseListenerAndSetVisibility (Component* comp, ValueTree wData);
    //=============================================================================
    // all these methods expose public methods in CabagePluginProcessor
    void sendChannelDataToCsound (String channel, float value);
    void sendChannelStringDataToCsound (String channel, String value);
    void sendScoreEventToCsound (String scoreEvent);
    void createEventMatrix(int cols, int rows, String channel);
    void setEventMatrixData(int cols, int rows, String channel, String data);
    void setEventMatrixCurrentPosition(int cols, int rows, String channel, int position);
    bool isAudioUnit()
    {
        return processor.wrapperType_AudioUnit;
    }
    bool shouldUpdateSignalDisplay();
    void savePluginStateToFile (File snapshotFile);
    void restorePluginStateFrom (String childPreset);
    const Array<float, CriticalSection> getArrayForSignalDisplay (const String signalVariable, const String displayType);
    const String getCsoundOutputFromProcessor();
    StringArray getTableStatement (int tableNumber);
    bool csdCompiledWithoutError();
    const Array<float, CriticalSection> getTableFloats (int tableNum);
    CabbagePluginProcessor& getProcessor();
    void enableXYAutomator (String name, bool enable, Line<float> dragLine = Line<float> (0, 0, 1, 1));

    //=============================================================================
    void mouseMove (const MouseEvent& e) override;
    void mouseDrag (const MouseEvent& e) override;
    void mouseDown (const MouseEvent& e) override;
    void mouseUp (const MouseEvent& e) override;
    void handleMouseClicks (const MouseEvent& e, bool isMousePressed);
    void handleMouseMovement (const MouseEvent& e);
    //=============================================================================
    String createNewGenericNameForPresetFile();
    void addNewWidget (String widgetType, Point<int> point, bool isPlant = false);
    //=============================================================================
    void refreshComboBoxContents();
    void enableEditMode (bool enable);
    void setCurrentlySelectedComponents (StringArray componentNames);
    void resetCurrentlySelectedComponents();
    Array<ValueTree> getValueTreesForCurrentlySelectedComponents();
    ValueTree getValueTreeForComponent (String compName);
    Component* getComponentFromName (String name);
    void addToEditorAndMakeVisible (Component* comp, ValueTree widgetData);
    void updateLayoutEditorFrames();
    void addPlantToPopupPlantsArray (ValueTree wData, Component* plant);
    //=============================================================================
    void buttonClicked (Button* button) override;
    void buttonStateChanged (Button* button);
    void toggleButtonState (Button* button, bool state);
    void comboBoxChanged (ComboBox* combo) override;
    void sliderValueChanged (Slider* slider) override;
    //=============================================================================
    CabbageAudioParameter* getParameterForComponent (const String name);
    //=============================================================================
    void setLastOpenedDirectory (const String lastOpenedDirectory)
    {
        this->lastOpenedDirectory = lastOpenedDirectory;
    }
    const String getLastOpenedDirectory() const
    {
        return lastOpenedDirectory;
    }
    //=============================================================================
    void updatefTableData (GenTable* table);

#ifdef Cabbage_IDE_Build
    ComponentLayoutEditor& getLayoutEditor()
    {
        return layoutEditor;
    }
#endif

    bool isEditModeEnabled()
    {
        return editModeEnabled;
    }
    Colour backgroundColour;

    //---- popup plant window ----
    class PopupDocumentWindow : public DocumentWindow, public ChangeBroadcaster
    {
        Colour colour;
        ValueTree plantWidgetData;
    public:
        PopupDocumentWindow (String caption, Colour backgroundColour)
            : DocumentWindow (caption, backgroundColour, DocumentWindow::TitleBarButtons::allButtons), colour (backgroundColour)
        {

        }

        void closeButtonPressed() override
        {
            setVisible (false);
            CabbageWidgetData::setNumProp(plantWidgetData, CabbageIdentifierIds::visible, 0);
        }

        void paint (Graphics& g){               g.fillAll (colour);         }
        void setWidgetData(ValueTree wData){    plantWidgetData = wData;    }
    };

    String changeMessage = "";
    Point<int> customPlantPosition;
private:

    //---- main component that holds widgets -----
    class MainComponent : public Component
    {
        Colour colour;
    public:
        MainComponent() : Component()
        {
            setOpaque (false);
        }
        void setColour (Colour col)
        {
            colour = col;
        }
        void paint (Graphics& g)  override
        {
            g.setOpacity (0);
            g.fillAll (colour);
        }
    };

    OwnedArray<Component> components;
    OwnedArray<PopupDocumentWindow> popupPlants;
    String lastOpenedDirectory;
    MainComponent mainComponent;
    int keyboardCount = 0;
    int xyPadIndex = 0;
    int consoleCount = 0;
    CabbageLookAndFeel2 lookAndFeel;
    int newlyAddedWidgetIndex = 10000;
    TooltipWindow tooltipWindow;
    bool editModeEnabled = false;
    CabbagePluginProcessor& processor;
    String instrumentName;

#ifdef Cabbage_IDE_Build
    ComponentLayoutEditor layoutEditor;
#endif

    StringArray currentlySelectedComponentNames;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CabbagePluginEditor)
};


#endif  // CABBAGEPLUGINEDITOR_H_INCLUDED
