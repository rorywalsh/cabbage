/*
 ==============================================================================
 
 This file is part of the JUCE library.
 Copyright (c) 2017 - ROLI Ltd.
 
 JUCE is an open source library subject to commercial or open-source
 licensing.
 
 By using JUCE, you agree to the terms of both the JUCE 5 End-User License
 Agreement and JUCE 5 Privacy Policy (both updated and effective as of the
 27th April 2017).
 
 End User License Agreement: www.juce.com/juce-5-licence
 Privacy Policy: www.juce.com/juce-5-privacy-policy
 
 Or: You may also use this code under the terms of the GPL v3 (see
 www.gnu.org/licenses).
 
 JUCE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY, AND ALL WARRANTIES, WHETHER
 EXPRESSED OR IMPLIED, INCLUDING MERCHANTABILITY AND FITNESS FOR PURPOSE, ARE
 DISCLAIMED.
 
 ==============================================================================
 */

#pragma once

#include "../Filters/FilterGraph.h"
#include "../../CabbageCommonHeaders.h"

class CabbageTransportComponent;
class CabbageMainComponent;

//==============================================================================
/**
 A panel that displays and edits a FilterGraph.
 */
class GraphEditorPanel   : public Component,
public ChangeListener
{
public:
    GraphEditorPanel (FilterGraph& graph);
    ~GraphEditorPanel() override;
    
    void createNewPlugin (const PluginDescription&, juce::Point<double> position);
    
    void paint (Graphics&) override;
    void resized() override;
    
    void mouseDown (const MouseEvent&) override;
    void mouseUp   (const MouseEvent&) override;
    void mouseDrag (const MouseEvent&) override;
    
    void changeListenerCallback (ChangeBroadcaster*) override;
    
    //==============================================================================
    void updateComponents();
    
    //==============================================================================
    void showPopupMenu (juce::Point<int> position);
    
    //==============================================================================
    void beginConnectorDrag (AudioProcessorGraph::NodeAndChannel source,
                             AudioProcessorGraph::NodeAndChannel dest,
                             const MouseEvent&);
    void dragConnector (const MouseEvent&);
    void endDraggingConnector (const MouseEvent&);
    
    //==============================================================================
    FilterGraph& graph;
    
    //mod RW
    void showEditorForNode(AudioProcessorGraph::NodeID pluginID);
    Colour backgroundColour = { 30, 30, 30 };
    void setBackgroundColour(Colour colour) {
        backgroundColour = colour;
        repaint();
    }
    
private:
    struct FilterComponent;
    struct ConnectorComponent;
    struct PinComponent;
    
    OwnedArray<FilterComponent> nodes;
    OwnedArray<ConnectorComponent> connectors;
    std::unique_ptr<ConnectorComponent> draggingConnector;
    std::unique_ptr<PopupMenu> menu;
    FilterComponent* getComponentForFilter (AudioProcessorGraph::NodeID) const;
    ConnectorComponent* getComponentForConnection (const AudioProcessorGraph::Connection&) const;
    PinComponent* findPinAt (juce::Point<float>) const;
    
    //==============================================================================
    juce::Point<int> originalTouchPos;
    CabbageLookAndFeel2 lookAndFeel;
    //void timerCallback() override;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GraphEditorPanel)
};


//==============================================================================
/**
 A panel that embeds a GraphEditorPanel with a midi keyboard at the bottom.
 
 It also manages the graph itself, and plays it.
 */
class GraphDocumentComponent  : public Component,
public DragAndDropTarget,
public DragAndDropContainer,
public AudioIODeviceCallback
{
public:
    GraphDocumentComponent (AudioPluginFormatManager& formatManager,
                            AudioDeviceManager& deviceManager,
                            KnownPluginList& pluginList);
    
    ~GraphDocumentComponent() override;
    
    //==============================================================================
    void createNewPlugin (const PluginDescription&, juce::Point<double> position);
    void setDoublePrecision (bool doublePrecision);
    bool closeAnyOpenPluginWindows();
    
    //==============================================================================
    std::unique_ptr<FilterGraph> graph;
    
    void resized() override;
    void unfocusKeyboardComponent();
    void releaseGraph();
    //void paint(Graphics&) override;
    //==============================================================================
    bool isInterestedInDragSource (const SourceDetails&) override;
    void itemDropped (const SourceDetails&) override;
    
    //==============================================================================
    std::unique_ptr<GraphEditorPanel> graphPanel;
    std::unique_ptr<MidiKeyboardComponent> keyboardComp;
    //RW
    std::unique_ptr<CabbageTransportComponent> transportControls;
    
    //==============================================================================
    void showSidePanel (bool isSettingsPanel);
    void hideLastSidePanel();
    BurgerMenuComponent burgerMenu;
    
    //RW edit...
    //==============================================================================
    void enableAudioInput()
    {
        Timer::callAfterDelay(500, [this](){
            shouldMuteInput = false;
        });
    }
    
    void enableGraph(bool shouldEnable){
        if(shouldEnable){
            graphPlayer.setProcessor (&graph->graph);
        }
        else{
            graphPlayer.setProcessor (nullptr);
            CabbageUtilities::debug("disabling graph");
        }
    }
    bool shouldMuteInput = true;
    AudioSampleBuffer emptyBuffer;
    //inherting audioIODeviceCallback so as to get rid of feedback when graph first starts..
    //==============================================================================
    void audioDeviceIOCallbackWithContext(const float* const* inputChannelData,
        int numInputChannels,
        float* const* outputChannelData,
        int numOutputChannels,
        int numSamples,
        const AudioIODeviceCallbackContext& context) override
    {
        
        if (shouldMuteInput)
        {
            emptyBuffer.clear();
            inputChannelData = emptyBuffer.getArrayOfReadPointers();
        }
        
        graphPlayer.audioDeviceIOCallbackWithContext(inputChannelData, numInputChannels,
                                           outputChannelData, numOutputChannels, numSamples, context);
    }
    
    void audioDeviceAboutToStart (AudioIODevice* device) override
    {
        emptyBuffer.setSize (device->getActiveInputChannels().countNumberOfSetBits(), device->getCurrentBufferSizeSamples());
        emptyBuffer.clear();
        
        graphPlayer.audioDeviceAboutToStart (device);
    }
    
    void audioDeviceStopped() override
    {
        graphPlayer.audioDeviceStopped();
        //        emptyBuffer.setSize (0, 0);
    }
    
    //end RW ==============================================================================
private:
    //==============================================================================
    AudioDeviceManager& deviceManager;
    KnownPluginList& pluginList;
    
    AudioProcessorPlayer graphPlayer;
    MidiKeyboardState keyState;
    
    struct TooltipBar;
    std::unique_ptr<TooltipBar> statusBar;
    
    class TitleBarComponent;
    std::unique_ptr<TitleBarComponent> titleBarComponent;
    
    //==============================================================================
    struct PluginListBoxModel;
    std::unique_ptr<PluginListBoxModel> pluginListBoxModel;
    
    ListBox pluginListBox;
    
    SidePanel mobileSettingsSidePanel { "Settings", 300, true };
    SidePanel pluginListSidePanel    { "Plugins", 250, false };
    SidePanel* lastOpenedSidePanel = nullptr;
    
    //==============================================================================
    void init();
    void checkAvailableWidth();
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GraphDocumentComponent)
};

//RW
class FilterGraphDocumentWindow : public DocumentWindow
{
	Colour colour;
	CabbageMainComponent* owner;
public:
	FilterGraphDocumentWindow(String caption, Colour backgroundColour, CabbageMainComponent* owner)
		: DocumentWindow(caption, backgroundColour, DocumentWindow::TitleBarButtons::allButtons), colour(backgroundColour), owner(owner)
	{
		setSize(600, 600);
		setName(caption);
		this->setTitleBarHeight(15);
		this->setResizable(true, true);

	}

	void closeButtonPressed() override { setVisible(false); }

	CabbageMainComponent* getOwner() {
		return owner;
	}

};
