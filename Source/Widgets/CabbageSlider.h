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

#ifndef CABBAGESLIDER_H_INCLUDED
#define CABBAGESLIDER_H_INCLUDED

#include "../CabbageCommonHeaders.h"
#include "CabbageWidgetBase.h"
#include "../LookAndFeel/FlatButtonLookAndFeel.h"

class CabbagePluginEditor;

//https://forum.juce.com/t/skew-friendly-filmstrip-knob/15360
class FilmStripSlider : public Slider
{
public:
    FilmStripSlider(ValueTree cAttr, const int numFrames, const bool stripIsHorizontal)
        : Slider(),
        numFrames_(numFrames),
        isHorizontal_(stripIsHorizontal)
    {

        const File imageFile = File(CabbageWidgetData::getStringProp(cAttr, CabbageIdentifierIds::csdfile)).getParentDirectory().getChildFile(CabbageWidgetData::getStringProp(cAttr, CabbageIdentifierIds::filmstripimage));
        if(imageFile.existsAsFile())
        {
            filmStrip = ImageFileFormat::loadFrom(imageFile);
            if (!filmStrip.isNull())
            {
                setTextBoxStyle(NoTextBox, 0, 0, 0);
                imageIsNull = false;
                if (isHorizontal_) {
                    frameHeight = filmStrip.getHeight();
                    frameWidth = filmStrip.getWidth() / numFrames_;
                }
                else {
                    frameHeight = filmStrip.getHeight() / numFrames_;
                    frameWidth = filmStrip.getWidth();
                }
            }
        }
    }

    void paint(Graphics& g)
    {
        if (imageIsNull == false)
        {
            const float sliderPos = (float)valueToProportionOfLength(getValue());

            int value = sliderPos * (numFrames_-1);
            if (isHorizontal_) {
                g.drawImage(filmStrip, 0, 0, getWidth(), getHeight(),
                    value * frameWidth, 0, frameWidth, frameHeight);
            }
            else {
                g.drawImage(filmStrip, 0, 0, getWidth(), getHeight(),
                    0, value * frameHeight, frameWidth, frameHeight);
            }
        }
    }
    
    bool isFilmStripValid(){    return imageIsNull ? false : true; }
    int getFrameWidth() const { return frameWidth; }
    int getFrameHeight() const { return frameHeight; }

private:
    const int numFrames_;
    const bool isHorizontal_;
    bool imageIsNull = true;
    Image filmStrip;
    int frameWidth = 32, frameHeight = 32;
};


class CabbageSlider
    : public Component,
      public ValueTree::Listener,
      public CabbageWidgetBase
{
    CabbagePluginEditor* owner;
    ValueTree widgetData;
    Label textLabel;
    float velocity = 0, sliderIncrement = 0, sliderSkew = 0, min = 0, max = 0, value = 0, shouldShowTextBox = 0, trackerInnerRadius = 0, trackerOuterRadius = 0, trackerThickness = 0;
    int decimalPlaces = 0;
    String colour, fontColour, textColour, outlineColour, sliderType, trackerColour, channel, popupText;
    bool shouldDisplayPopup = true;
    Slider slider;
    FilmStripSlider filmSlider;
    BubbleMessageComponent popupBubble;

    void mouseDrag (const MouseEvent& event) override;
    void mouseMove (const MouseEvent& event) override;
    void mouseEnter (const MouseEvent& event) override;
    void mouseExit (const MouseEvent& event) override;
    void createPopupBubble();
    void showPopupBubble (int time);
    void setLookAndFeelColours (ValueTree wData);
    String prefix = "";
    String postfix = "";
    String popupPrefix = "";
    String popupPostfix = "";
    bool isFilmStripSlider = false;

    FlatButtonLookAndFeel flatLookAndFeel;

public:
    CabbageSlider (ValueTree cAttr, CabbagePluginEditor* _owner);
    ~CabbageSlider();

    void setTextBoxWidth();
    void setSliderVelocity (ValueTree wData);
    void resized() override;
    void initialiseSlider (ValueTree wData, Slider& currentSlider);

    void setTextBoxOrientation (String type, int textBox);
    void valueTreePropertyChanged (ValueTree& valueTree, const Identifier& prop) override;
    void valueTreeChildAdded (ValueTree&, ValueTree&) override {};
    void valueTreeChildRemoved (ValueTree&, ValueTree&, int) override {}
    void valueTreeChildOrderChanged (ValueTree&, int, int) override {}
    void valueTreeParentChanged (ValueTree&) override {};
    Slider& getSlider()
    {
        if(filmSlider.isFilmStripValid())
            return filmSlider;
        else
            return slider;
    };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CabbageSlider)

};

#endif  // CABBAGESLIDER_H_INCLUDED
