/*
  Copyright (C) 2019 Rory Walsh

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

#include "CabbageTransportComponent.h"


//==============================================================================
//transport controls
//==============================================================================
CabbageTransportComponent::CabbageTransportComponent(GraphDocumentComponent* graph):
    lookAndFeel(),
    playButton("playButton", DrawableButton::ImageFitted),
    stopButton("stopButton", DrawableButton::ImageFitted),
    recordButton("recordButton", DrawableButton::ImageFitted),
    timeLabel("TimeLabel"),
    beatsLabel("beatsLabel"),
    bpmLabel("60 bpm"),
    timingInfoBox(),
    overlay(),
    owner(graph)
{
    AudioPlayHead::CurrentPositionInfo info;
    info.bpm = 60;
    info.isPlaying = false;
    info.isRecording = false;
    info.timeInSamples = 0;
    info.timeSigDenominator  = 4;
    info.timeSigNumerator = 4;
    info.ppqPosition = 0;
    owner->graph->setPlayHeadInfo(info);


    timeLabel.setJustificationType(Justification::left);
    timeLabel.setFont(Font(24, 1));
    timeLabel.setLookAndFeel(lookAndFeel);
    timeLabel.setColour(Label::backgroundColourId, Colours::transparentBlack);
    timeLabel.setColour(Label::textColourId, Colours::cornflowerblue);
    timeLabel.setText("00 : 00 : 00", dontSendNotification);

    beatsLabel.setJustificationType(Justification::right);
    beatsLabel.setFont(Font(18, 1));
    beatsLabel.setLookAndFeel(lookAndFeel);
    beatsLabel.setColour(Label::backgroundColourId, Colours::transparentBlack);
    beatsLabel.setColour(Label::textColourId, Colours::cornflowerblue);
    beatsLabel.setText("Beat 1", dontSendNotification);

    bpmLabel.setJustificationType(Justification::right);
    bpmLabel.setFont(Font(18, 1));
    bpmLabel.setLookAndFeel(lookAndFeel);
    bpmLabel.setEditable(true, true);
    bpmLabel.addListener(this);
    bpmLabel.setColour(Label::backgroundColourId, Colours::transparentBlack);
    bpmLabel.setColour(Label::textColourId, Colours::cornflowerblue);
    bpmLabel.setColour(Label::outlineWhenEditingColourId, Colours::transparentBlack);
    bpmLabel.setColour(Label::ColourIds::textWhenEditingColourId, Colours::white);
    bpmLabel.setText("60 bpm", dontSendNotification);
    bpmLabel.setAlwaysOnTop(true);

    playButton.setLookAndFeel(lookAndFeel);
    playButton.setColour(DrawableButton::backgroundColourId, Colours::transparentBlack);
    playButton.setColour(DrawableButton::backgroundOnColourId, Colours::transparentBlack);

    playButton.setClickingTogglesState(true);
    recordButton.setClickingTogglesState(true);
    recordButton.setColour(DrawableButton::backgroundColourId, Colours::transparentBlack);
    recordButton.setColour(DrawableButton::backgroundOnColourId, Colours::transparentBlack);

    stopButton.setLookAndFeel(lookAndFeel);


    recordButton.setLookAndFeel(lookAndFeel);

    addAndMakeVisible (timingInfoBox);
    addAndMakeVisible (playButton);
    addAndMakeVisible (stopButton);
    addAndMakeVisible (recordButton);
    addAndMakeVisible (bpmLabel);
    addAndMakeVisible (timeLabel);
    addAndMakeVisible (beatsLabel);
    addAndMakeVisible (overlay);
    overlay.setAlwaysOnTop(true);

    overlay.setInterceptsMouseClicks(false, true);

    playButton.addListener (this);
    stopButton.addListener (this);
}

void CabbageTransportComponent::labelTextChanged (Label *labelThatHasChanged)
{

    const String text = labelThatHasChanged->getText(true).replace("bpm", "");
    if(text.getIntValue() > 0)
        owner->graph->setBPM(text.getIntValue());

    labelThatHasChanged->setText(text+ " bpm", dontSendNotification);
}


void CabbageTransportComponent::timerCallback()
{
    const double ellapsedTime = owner->graph->getTimeInSeconds();
    const int hours = (int(ellapsedTime) / 60 / 60) % 24;
    const int minutes = (int(ellapsedTime) / 60) % 60;
    const int seconds = int(ellapsedTime) % 60;
    String time = String::formatted("%02d", hours)+" : "+String::formatted("%02d", minutes)+" : "+String::formatted("%02d", seconds);


    const double elaspsedQNs = owner->graph->getPPQPosition();

    if(elaspsedQNs>=1)
    {
        String ppqPos = "Beat "+String(elaspsedQNs);
        setBeatsLabel(String(ppqPos));
    }
    setTimeLabel(time);

}

const Image CabbageTransportComponent::drawRecordStopIcon (int width, int height, Colour colour, bool isRecordButton, bool isRecording)
{
    Image img = Image (Image::ARGB, width, height, true);
    Graphics g (img);
    g.fillAll(Colours::transparentBlack);
    Path p;

    if(!isRecordButton) {
        g.setColour(colour);
        p.addRectangle(0, 0, width, height);
    }
    else{
            g.setColour(isRecording ? colour : colour.darker(.7f));
            p.addEllipse(0, 0, width, height);
    }

    p.closeSubPath();
    g.fillPath (p);
    return img;
}

const Image CabbageTransportComponent::drawPlayPauseIcon (int width, int height, Colour colour, bool isPlaying, bool isPressed)
{
    Image img = Image (Image::ARGB, width, height, true);
    Graphics g (img);
    g.fillAll(Colours::transparentBlack);
    Path p;

    const int newWidth = (isPressed == true ? width - 1 : width);
    const int newHeight = (isPressed == true ? height - 1 : height);


    if (isPlaying == false)
    {
        g.setColour (colour);
        p.addTriangle (0,0, newWidth, height/2, 0, newHeight);
    }
    else
    {
        g.setColour (colour);
        p.addRectangle (0, 0, width*.3, height);
        p.addRectangle (width*.6, 0, width*.3, height);
    }

    p.closeSubPath();
    g.fillPath (p);
    return img;
}

void CabbageTransportComponent::setDrawableImages (DrawableButton& button, int width, int height, String type)
{
    DrawableImage imageNormal, imageNormalPressed, imageDownPressed;

    if (type == "play") {
        DrawableImage imageDown;
        imageNormalPressed.setImage(drawPlayPauseIcon(width, height, playButtonColour, false, true));
        imageDownPressed.setImage(drawPlayPauseIcon(width, height, playButtonColour, true, true));
        imageNormal.setImage(drawPlayPauseIcon(width, height, playButtonColour, false));
        imageDown.setImage(drawPlayPauseIcon(width, height, playButtonColour, true));
        button.setImages(&imageNormal, &imageNormal, &imageNormalPressed, &imageNormal, &imageDown, nullptr,
                         &imageDownPressed, &imageDownPressed);
    }
    else if (type == "stop") {
        DrawableImage imageDown;
        imageNormalPressed.setImage(drawRecordStopIcon(width, height, stopButtonColour, false, true));
        imageDownPressed.setImage(drawRecordStopIcon(width, height, stopButtonColour, false, true));
        imageNormal.setImage(drawRecordStopIcon(width, height, stopButtonColour, false));
        imageDown.setImage(drawRecordStopIcon(width, height, stopButtonColour, false));
        button.setImages(&imageNormal, &imageNormal, &imageNormalPressed, &imageNormal, &imageDown, nullptr,
                         &imageDownPressed, &imageDownPressed);
    }
    else if (type == "record") {
        DrawableImage imageDown;
        imageNormalPressed.setImage(drawRecordStopIcon(width, height, recordButtonColour, true, true));
        imageDownPressed.setImage(drawRecordStopIcon(width, height, recordButtonColour, true, true));
        imageNormal.setImage(drawRecordStopIcon(width, height, recordButtonColour, true, false));
        imageDown.setImage(drawRecordStopIcon(width, height, recordButtonColour, true, true));
        button.setImages(&imageNormal, &imageNormal, &imageNormalPressed, &imageNormal, &imageDown, nullptr,
                         &imageDownPressed, &imageDownPressed);
    }
}



void CabbageTransportComponent::sliderValueChanged(Slider* slider)
{
    //owner.setCurrentBPM(slider->getValue());
}

void CabbageTransportComponent::sliderDragEnded(Slider* slider)
{
//    owner.setCurrentBPM(slider->getValue());
}

void CabbageTransportComponent::resized()
{
    auto r = getLocalBounds();

    stopButton.setBounds(r.removeFromLeft(40).reduced(5));
    playButton.setBounds(r.removeFromLeft(40).reduced(5));
    recordButton.setBounds(playButton.getX()+playButton.getWidth()+2, 5, getHeight()-9, getHeight()-9);
    overlay.setBounds(playButton.getX()+playButton.getWidth()+2, 5, getHeight()-9, getHeight()-9);
    r.removeFromLeft(40);
    timeLabel.setBounds(r.withHeight(getHeight()/1.5).reduced(30, 0));
    beatsLabel.setBounds(r.getTopLeft().x+80, r.getTopLeft().y+23, 70, 30);
    bpmLabel.setBounds(r.getTopLeft().x+15, r.getTopLeft().y+23, 70, 30);

    Path timeBox;
    timeBox.addRoundedRectangle(r, 5);
    timingInfoBox.setPath(timeBox);
    timingInfoBox.setFill(Colours::black);

    timingInfoBox.setBounds(r.removeFromLeft(5).withHeight(50));
    setDrawableImages (playButton, playButton.getWidth(), playButton.getHeight(), "play");
    setDrawableImages (stopButton, stopButton.getWidth(), stopButton.getHeight(), "stop");
    setDrawableImages (recordButton, recordButton.getWidth(), recordButton.getHeight(), "record");

}

void CabbageTransportComponent::buttonClicked (Button* button)
{
    if(button->getName()=="stopButton")
    {
        playButton.setToggleState(false, sendNotification);
        recordButton.setToggleState(false, sendNotification);
        overlay.stopTimer();
        stopTimer();
        overlay.resetColour();
        owner->graph->setIsHostPlaying(false, true);
        setTimeLabel("00 : 00 : 00");
        setBeatsLabel("Beat 1");
        owner->graph->setIsRecording(false);
    }
    else if(button->getName()=="playButton")
    {
        if(button->getToggleState() == true) {
            owner->graph->setIsHostPlaying(true, false);
            startTimer(10);
            if (recordButton.getToggleState() == true) {
                owner->graph->setIsRecording(true);
                overlay.startTimer(500);
            }
        } else{
            startTimer(10);;
            owner->graph->setIsHostPlaying(false, false);

        }

    }
}
