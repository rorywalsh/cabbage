/*
  ==============================================================================

    CabbageListBox.h
    Created: 29 Aug 2018 6:21:32pm
    Author:  rory

  ==============================================================================
*/

#pragma once



#include "../CabbageCommonHeaders.h"
#include "CabbageWidgetBase.h"

class CabbagePluginEditor;

// Add any new custom widgets here to avoid having to edit makefiles and projects
// Each Cabbage widget should inherit from ValueTree listener, and CabbageWidgetBase
class CabbageListBox : public Component, public ListBoxModel, public ValueTree::Listener, public CabbageWidgetBase
{
    
    Font userFont;
public:

    CabbageListBox (ValueTree wData, CabbagePluginEditor* _owner);
    ~CabbageListBox() override {
        widgetData.removeListener(this);
        setLookAndFeel(nullptr);
    }

    //ValueTree::Listener virtual methods....
    void valueTreePropertyChanged (ValueTree& valueTree, const Identifier&) override;
    void valueTreeChildAdded (ValueTree&, ValueTree&)override {}
    void valueTreeChildRemoved (ValueTree&, ValueTree&, int) override {}
    void valueTreeChildOrderChanged (ValueTree&, int, int) override {}
    void valueTreeParentChanged (ValueTree&) override {}

    ValueTree widgetData;
    int offX, offY, offWidth, offHeight, pivotx, pivoty, refresh;
    String name, tooltipText, caption, text, filetype, workingDir;
    String align, colour, highlightColour, fontColour, channelType;
    Justification justify;
    CabbagePluginEditor* owner;
    //CabbageLookAndFeel2 lookAndFeel;
    LookAndFeel_V4 lookAndFeel;
    File listboxDir;
    bool isStringCombo = false;
    bool isPresetCombo = false;
    String currentValueAsText = "";
    Array<File> folderFiles;
    StringArray stringItems;
    StringArray presets;
    ListBox listBox;
    int numberOfClicks = 1;


    int getNumRows() override;
    void listBoxItemDoubleClicked(int row, const MouseEvent &e) override;
    void listBoxItemClicked(int row, const MouseEvent &e) override;
    void clicked(int row);
    void paintListBoxItem (int rowNumber, Graphics& g,
                           int width, int height, bool rowIsSelected) override;
    void selectedRowsChanged (int /*lastRowselected*/) override;
    void addItemsToListbox (ValueTree wData);
    //void paint(Graphics& g) override;
    void resized() override;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CabbageListBox)
};
