/*
  ==============================================================================

    CabbageTransport.h
    Created: 15 Oct 2019 7:15:48pm
    Author:  rory

  ==============================================================================
*/

#pragma once

#include "../../../JuceLibraryCode/JuceHeader.h"
#include "../../CabbageCommonHeaders.h"
#include "GraphEditorPanel.h"


class GraphDocumentComponent;
//==============================================================================
// transport prop
//==============================================================================
class CabbageTransportComponent : public Component,
    public Button::Listener,
    public Slider::Listener,
    public Timer,
public Label::Listener
{
public:
    CabbageTransportComponent(GraphDocumentComponent* graph);

    ~CabbageTransportComponent()
    {
        setLookAndFeel(nullptr);
        owner = nullptr;
    }

    void resized();
    void refresh() {}
    void buttonClicked (Button* button);
    void sliderValueChanged (Slider* sliderThatWasMoved);
    void sliderDragEnded (Slider *);

    void setTimeLabel(String value)
    {
        timeLabel.setText(value, dontSendNotification);
    }
    void setBeatsLabel(String value)
    {
        beatsLabel.setText(value, dontSendNotification);
    }

    static const Image drawPlayPauseIcon (int width, int height, Colour colour, bool isPlaying, bool isPressed = false);
    static const Image drawRecordStopIcon (int width, int height, Colour colour, bool isRecord, bool isPressed = false);

    void timerCallback() override;

    void setTimeIsRunning(bool value)
    {
        if(value)
            timeLabel.setColour(Label::textColourId, Colours::yellow);
        else
            timeLabel.setColour(Label::textColourId, Colours::cornflowerblue);
    }

    void paint(Graphics& g) override
    {
        g.fillAll(Colours::transparentBlack);
    }

    void labelTextChanged (Label *labelThatHasChanged) override;

    class RecordOverlayComponent : public Component, public Timer
    {
        Colour overlayColour = {Colours::transparentBlack};
        bool toggle = false;
    public:
        RecordOverlayComponent():Component(){}
        ~RecordOverlayComponent(){}

        void timerCallback() override
        {
                if(toggle)
                    overlayColour = overlayColour.withAlpha(0.5f);
                else
                    overlayColour = overlayColour.withAlpha(0.f);

                toggle=!toggle;

                repaint();
        }

        void resetColour()
        {
            overlayColour = overlayColour.withAlpha(0.f);
            repaint();
        }

        void paint(Graphics& g) override
        {
            g.fillAll(Colours::transparentBlack);
            g.setColour(overlayColour);
            g.fillEllipse(getLocalBounds().reduced(2).toFloat());
        }


    };
    void setDrawableImages (DrawableButton& button, int width, int height, String type);
private:
    RecordOverlayComponent overlay;
    DrawableButton playButton;
    DrawableButton stopButton;
    DrawableButton recordButton;
    GraphDocumentComponent* owner;
    Label bpmLabel;
    Label timeLabel;
    Label beatsLabel;

    DrawablePath timingInfoBox;
    ScopedPointer<LookAndFeel_V4> lookAndFeel;
    Colour playButtonColour = {0, 100, 0};
    Colour stopButtonColour = {0, 100, 0};
    Colour recordButtonColour = {255, 0, 0};
};
