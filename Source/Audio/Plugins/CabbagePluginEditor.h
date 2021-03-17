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

#ifdef Cabbage_IDE_Build
    #include "../../GUIEditor/ComponentLayoutEditor.h"
#endif

#include "../../Widgets/CabbageCheckbox.h"
#include "../../Widgets/CabbageComboBox.h"
#include "../../Widgets/CabbageImage.h"
#include "../../Widgets/CabbageButton.h"
#include "../../Widgets/CabbageFileButton.h"
#include "../../Widgets/CabbageListBox.h"
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
#include "../../Widgets/CabbageOptionButton.h"
#include "../../Widgets/CabbageXYPad.h"
#include "../../Widgets/CabbageKeyboard.h"
#include "../../Widgets/CabbageKeyboardDisplay.h"
#include "../../Widgets/CabbageSlider.h"
#include "../../Widgets/CabbageSoundfiler.h"
#include "../../Widgets/CabbageRackWidgets.h"
#include "../../Widgets/CabbageRangeSlider.h"
#include "../../Widgets/CabbageCustomWidgets.h"
#include "../../Widgets/CabbageEventSequencer.h"

class CabbagePluginEditor;

//==============================================================================
class CabbagePluginEditor
    : public AudioProcessorEditor,
      public Button::Listener,
      public ChangeBroadcaster,
      public ActionBroadcaster,
      public ComboBox::Listener,
      public Slider::Listener,
      //public FileDragAndDropTarget,
	  public KeyListener
{
public:
    explicit CabbagePluginEditor (CabbagePluginProcessor&);
    ~CabbagePluginEditor();

    void createEditorInterface (ValueTree widgets);
    //==============================================================================
    void resized() override;
    void paint (Graphics& g)  override {}
    //==============================================================================
    void setupWindow (ValueTree cabbageWidgetData);

    Component* getMainComponent(){  return &mainComponent;   }
    
    void insertWidget (ValueTree cabbageWidgetData);
    // the following methods instantiate controls that CAN
    // be automated in a host...
    void insertSlider (ValueTree cabbageWidgetData);
    void insertComboBox (ValueTree cabbageWidgetData);
    void insertButton (ValueTree cabbageWidgetData);
    void insertOptionButton (ValueTree cabbageWidgetData);
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
    void insertKeyboard (ValueTree cabbageWidgetData);
	void insertKeyboardDisplay(ValueTree cabbageWidgetData);
    void insertFileButton (ValueTree cabbageWidgetData);
    void insertImage (ValueTree cabbageWidgetData);
    void insertLine (ValueTree cabbageWidgetData);
    void insertLabel (ValueTree cabbageWidgetData);
    void insertListBox (ValueTree cabbageWidgetData);
    void insertTable (ValueTree cabbageWidgetData) {};
    void insertInfoButton (ValueTree cabbageWidgetData);
    void insertGenTable (ValueTree cabbageWidgetData);
    void insertTextBox (ValueTree cabbageWidgetData);
    void insertSignalDisplay (ValueTree cabbageWidgetData);
    void insertStepper (ValueTree cabbageWidgetData) {};
    void insertMeter (ValueTree cabbageWidgetData);
    
    void insertPort (ValueTree cabbageWidgetData);
    void insertScrew (ValueTree cabbageWidgetData);
    void insertLight (ValueTree cabbageWidgetData);
    
    void addMouseListenerAndSetVisibility (Component* comp, ValueTree wData);
    //=============================================================================
	void refreshValueTreeListeners();

	//=============================================================================
    // all these methods expose public methods in CabagePluginProcessor
    void sendChannelDataToCsound (String channel, float value);
    void sendChannelStringDataToCsound (String channel, String value);
    float getChannelDataFromCsound (String channel);
    void sendScoreEventToCsound (String scoreEvent);
    void createEventMatrix(int cols, int rows, String channel);
    void setEventMatrixData(int cols, int rows, String channel, String data);
    void setEventMatrixCurrentPosition(int cols, int rows, String channel, int position);
    bool isAudioUnit()
    {
        return processor.wrapperType_AudioUnit;
    }
    bool shouldUpdateSignalDisplay(String variableName);
    String currentPresetName;
    void savePluginStateToFile (File snapshotFile, String presetName="", bool remove = false);
    void restorePluginStateFrom (String childPreset, File xmlFile);
    const Array<float, CriticalSection> getArrayForSignalDisplay (const String signalVariable, const String displayType);
    const String getCsoundOutputFromProcessor();
    StringArray getTableStatement (int tableNumber);
    bool csdCompiledWithoutError();
    const Array<float, CriticalSection> getTableFloats (int tableNum);
    CabbagePluginProcessor& getProcessor();
    void enableXYAutomator (String name, bool enable, Line<float> dragLine = Line<float> (0, 0, 1, 1));


   /* void filesDropped(const StringArray &files, int x, int y) override;
    bool isInterestedInFileDrag(const StringArray &files) override;*/
    //=============================================================================
    void mouseMove (const MouseEvent& e) override;
    void mouseDrag (const MouseEvent& e) override;
    void mouseDown (const MouseEvent& e) override;
    void mouseUp (const MouseEvent& e) override;
    void handleMouseClicks (const MouseEvent& e, bool isMousePressed);
    void handleMouseMovement (const MouseEvent& e);
    //=============================================================================
	virtual bool keyPressed(const KeyPress& key, Component* originatingComponent) override
	{
        cabbageProcessor.getCsound()->SetChannel("KEY_PRESSED", key.getKeyCode());
		return false;
	}

	virtual bool keyStateChanged(bool isKeyDown, Component* originatingComponent) override
	{
        cabbageProcessor.getCsound()->SetChannel ("KEY_DOWN", isKeyDown);
		return false;
	}
	//=============================================================================

    
    String createNewGenericNameForPresetFile();
    void addNewWidget (String widgetType, juce::Point<int> point, bool isPlant = false);
    //=============================================================================
    void refreshComboListBoxContents(String presetName = "");
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
    void buttonStateChanged (Button* button) override;
    void toggleButtonState (Button* button, bool state);
    void comboBoxChanged (ComboBox* combo) override;
    void sliderValueChanged (Slider* slider) override;
    void sliderDragStarted(Slider* slider) override;
    void sliderDragEnded(Slider* slider) override;
    //=============================================================================
    CabbagePluginParameter* getParameterForComponent (const String name);
    //=============================================================================
    void setLastOpenedDirectory (const String lastDirectory)
    {
        this->lastOpenedDirectory = lastDirectory;
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
    Colour titlebarColour;
    Colour fontColour;
    String globalStyle = "";
    bool defaultFontColour = true;
    float titlebarGradientAmount;
    NamedValueSet radioGroups;

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

        void paint (Graphics& g) override {               g.fillAll (colour);         }
        void setWidgetData(ValueTree wData){    plantWidgetData = wData;    }
    };

    void addToRadioComponents(Component* button)
    {
        radioComponents.add(button);
    }
    
    String changeMessage = "";
    juce::Point<int> customPlantPosition;
private:
	  
    //---- main component that holds widgets -----
    class MainComponent : public Component, public FileDragAndDropTarget, public TextDragAndDropTarget
    {
        Colour colour;
        CabbagePluginEditor* owner;
    public:
        explicit MainComponent(CabbagePluginEditor* _owner) : Component(), owner(_owner)
        {
            setOpaque (false);
        }
        void setColour (Colour col)
        {
            colour = col;
        }
        void paint (Graphics& g)  override
        {
            //g.setOpacity (0);
            g.fillAll (colour);
        }
        
        bool isInterestedInFileDrag (const StringArray& /*files*/) override{ return true; }
        void fileDragEnter (const StringArray& /*files*/, int /*x*/, int /*y*/) override{}
        void fileDragMove (const StringArray& /*files*/, int /*x*/, int /*y*/) override {}
        void fileDragExit (const StringArray& /*files*/) override {}
        void filesDropped (const StringArray& files, int x, int y) override
        {
			owner->sendChannelDataToCsound(CabbageIdentifierIds::mousex, x);
			owner->sendChannelDataToCsound(CabbageIdentifierIds::mousey, y); 
            owner->sendChannelStringDataToCsound(CabbageIdentifierIds::lastFileDropped, files[0]);
        }
        
        bool isInterestedInTextDrag (const String& /*files*/) override{ return true; }
        void textDragEnter (const String& /*files*/, int /*x*/, int /*y*/) override {}
        void textDragMove (const String& /*files*/, int /*x*/, int /*y*/) override {}
        void textDragExit (const String& /*files*/) override {}
        void textDropped (const String& text, int x, int y) override
        {
            owner->sendChannelDataToCsound(CabbageIdentifierIds::mousex, x);
            owner->sendChannelDataToCsound(CabbageIdentifierIds::mousey, y);
            owner->sendChannelStringDataToCsound(CabbageIdentifierIds::lastTextDropped, text);
        }       
    };

    class ViewportContainer : public Component
    {

    public:
        ViewportContainer() : Component("CabbageViewportComponent")
        {
            this->setInterceptsMouseClicks(false, true);
        }
        ~ViewportContainer() {}

        void paint(Graphics &g)
        {
            Viewport* const vp = findParentComponentOfClass<Viewport>(); //Get the parent viewport
            if(vp != nullptr) //Check for nullness
            {
                juce::Rectangle<int> viewRect(vp->getViewPositionX(), vp->getViewPositionY(), vp->getViewWidth(), vp->getViewHeight()); //Get the current displayed area in the viewport
            }
        }
    };

    std::unique_ptr<Viewport> viewport;
    std::unique_ptr<ViewportContainer> viewportContainer;
    OwnedArray<Component> components;
    Array<Component*> radioComponents;
    OwnedArray<PopupDocumentWindow> popupPlants;
    String lastOpenedDirectory;
    MainComponent mainComponent;
    int keyboardCount = 0;
    //int xyPadIndex = 0;
    int consoleCount = 0;
    bool showScrollbars = false;
    CabbageLookAndFeel2 lookAndFeel;
    int newlyAddedWidgetIndex = 10000;

    bool editModeEnabled = false;
    CabbagePluginProcessor& cabbageProcessor;
    String instrumentName;
    juce::Point<int> instrumentBounds;
    SharedResourcePointer<TooltipWindow> tooltipWindow;


#ifdef Cabbage_IDE_Build
    ComponentLayoutEditor layoutEditor;
#endif

    StringArray currentlySelectedComponentNames;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CabbagePluginEditor)
};


#endif  // CABBAGEPLUGINEDITOR_H_INCLUDED
