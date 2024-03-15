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

#include "CabbageSlider.h"
#include "../Audio/Plugins/CabbagePluginEditor.h"

void SliderThumb::move(double value, Range<double> range)
{
    if(isEnabled())
    {
        if (owner->getSlider().getSliderStyle() == Slider::SliderStyle::LinearVertical) 
        {
            float pos = owner->getSlider().valueToProportionOfLength(value);
            const auto yPos = jmap(pos, 1.f, 0.f, (float)0, float(owner->getHeight() - getHeight()));
            setTopLeftPosition(getX(), yPos);
        }
        if (owner->getSlider().getSliderStyle() == Slider::SliderStyle::LinearHorizontal)
        {
            float pos = owner->getSlider().valueToProportionOfLength(value);
            const auto xPos = jmap(pos, 0.f, 1.f, (float)0, float(owner->getWidth() - getWidth()));
            setTopLeftPosition(xPos, getY());
        }
    }
}

void SliderThumb::mouseMove(const MouseEvent& e)
{
    if (isEnabled())
    {
        if (owner->shouldDisplayPopup)
            owner->showPopupBubble(500);
    }
}

void SliderThumb::mouseEnter(const MouseEvent& e)
{
    if (isEnabled())
    {
        if (owner->shouldDisplayPopup)
            owner->showPopupBubble(500);

    }
}

void SliderThumb::mouseUp(const MouseEvent& e)
{
    owner->getEditor()->sliderDragEnded(&owner->getSlider());
}

void SliderThumb::mouseDrag(const MouseEvent& e)
{
    if(isEnabled())
    {
        if (owner->getSlider().getSliderStyle() == Slider::SliderStyle::LinearVertical)
        {
            int yPos = jlimit(0.f, float(owner->getHeight() - getHeight()), float(e.getEventRelativeTo(owner).getPosition().getY()) + yOffset);
            float multiple = ((float)owner->getHeight() - getHeight()) * (owner->getSlider().getInterval() / owner->getSlider().getRange().getLength());

            int remainder = fmod(yPos, multiple);
            yPos = (yPos + multiple - remainder) - multiple;
            setTopLeftPosition(getX(), yPos);

            const auto prop = jmap(jlimit(0.f, (float)owner->getHeight() - getHeight(), (float)yPos), (float)0, (float)owner->getHeight() - getHeight(), 1.f, 0.f);
            const auto value = owner->getSlider().proportionOfLengthToValue(prop);
            
            //this should be calling paramete gesture changes I think...

            owner->getSlider().setValue(value, sendNotification);
        }

        if (owner->getSlider().getSliderStyle() == Slider::SliderStyle::LinearHorizontal)
        {
            int xPos = jlimit(0.f, float(owner->getWidth() - getWidth()), float(e.getEventRelativeTo(owner).getPosition().getX()) + xOffset);
            float multiple = ((float)owner->getWidth() - getWidth()) * (owner->getSlider().getInterval() / owner->getSlider().getRange().getLength());

            int remainder = fmod(xPos, multiple);
            xPos = (xPos + multiple - remainder) - multiple;
            setTopLeftPosition(xPos, getY());

            const auto prop = jmap(jlimit(0.f, (float)owner->getWidth() - getWidth(), (float)xPos), (float)0, (float)owner->getWidth() - getWidth(), 0.f, 1.f);
            const auto value = owner->getSlider().proportionOfLengthToValue(prop);
            owner->getSlider().setValue(value, sendNotification);
        }
        if (owner->shouldDisplayPopup)
            owner->showPopupBubble(500);
    }
}

void SliderThumb::mouseDown(const MouseEvent& e)
{
    if (isEnabled())
    {
        if (owner->getSlider().getSliderStyle() == Slider::SliderStyle::LinearVertical)
        {
            yOffset = getY() - e.getEventRelativeTo(owner).getPosition().getY();
        }
        if (owner->getSlider().getSliderStyle() == Slider::SliderStyle::LinearHorizontal)
        {
            xOffset = getX() - e.getEventRelativeTo(owner).getPosition().getX();
        }
        
        owner->getEditor()->sliderDragStarted(&owner->getSlider());


    }
}

CabbageSlider::CabbageSlider(ValueTree wData, CabbagePluginEditor* _owner)
    : owner(_owner),
    widgetData(wData),
    sliderType(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::kind)),
    channel(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::channel)),
    popupBubble(250),
    thumb(this),
    CabbageWidgetBase(_owner)
{

    setName(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::name));
    widgetData.addListener(this);
    addAndMakeVisible(textLabel);

    addAndMakeVisible(&slider);
    addAndMakeVisible(thumb);
    slider.setName(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::name));
    slider.getProperties().set("trackerthickness", CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::trackerthickness));
    slider.getProperties().set("trackerbgcolour", CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::trackerbgcolour));
    slider.getProperties().set("markercolour", CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::markercolour));
    slider.getProperties().set("markerthickness", CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::markerthickness));
    slider.getProperties().set("markerstart", CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::markerstart));
    slider.getProperties().set("markerend", CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::markerend));
    slider.getProperties().set("gapmarkers", CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::gapmarkers));
    auto csdPath = owner->getProcessor().getCsdFile().getFullPathName();
    setImgProperties(this->slider, wData, csdPath, "slider");
    setImgProperties(this->slider, wData, csdPath, "sliderbg");
    
    slider.getProperties().set("trackerCentre", CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::trackercentre));
    
    
    filmStripValueBox.setEditable(true);
    filmStripValueBox.setJustificationType(Justification::centred);
    prefix = CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::valueprefix);
    postfix = CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::valuepostfix);
    popupPrefix = CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::popupprefix);
    popupPostfix = CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::popuppostfix);
    initialiseSlider(wData, slider);
    initFilmStrip(wData);
    
    sliderBounds = CabbageWidgetData::getProperty(wData, CabbageIdentifierIds::sliderbounds);

    auto csdFile = owner->getProcessor().getCsdFile();
    const File sliderImageFile = File(csdFile).getParentDirectory().getChildFile(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::imgslider));
    const File sliderBackgroundFile = File(csdFile).getParentDirectory().getChildFile(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::imgsliderbg));
    if (!isFilmStripSlider) {
        if (sliderImageFile.existsAsFile())
        {
            sliderThumbImage = ImageCache::getFromFile(sliderImageFile);
            thumb.setThumbImage(sliderThumbImage);
        }
        if (sliderBackgroundFile.existsAsFile())
        {
            sliderBgImage = ImageCache::getFromFile(sliderBackgroundFile);
        }
        
    }
    
    
    setLookAndFeelColours(widgetData);
    setTextBoxOrientation(sliderType, shouldShowTextBox);
    const String sliderImg = CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::imgslider);
    const String sliderImgBg = CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::imgsliderbg);
    const String globalStyle = owner->globalStyle;
    if (CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::style) == "flat"
        && sliderImg.isEmpty() && sliderImgBg.isEmpty())
    {
        flatLookAndFeel.customFont = owner->customFont;
        slider.setLookAndFeel(&flatLookAndFeel);
        textLabel.setLookAndFeel(&flatLookAndFeel);
    }
    slider.setTextValueSuffix(postfix);
    
    
    slider.onValueChange = [this] {
        if (isFilmStripSlider || sliderThumbImage.isValid())
            repaint();
        thumb.move(slider.getValue(), slider.getRange());
        
        auto newValue = slider.getTextFromValue(slider.getValue());
        filmStripValueBox.setFont(25.f);
        filmStripValueBox.setText(newValue, dontSendNotification);
    };
    
    filmStripValueBox.onTextChange = [this] {
        slider.setValue(float(filmStripValueBox.getTextValue().getValue()));
    };
    
    textLabel.setVisible(false);
    initialiseCommonAttributes(this, wData);
    createPopupBubble();
    
    
}

CabbageSlider::~CabbageSlider()
{
    widgetData.removeListener(this);
    slider.setLookAndFeel(nullptr);
    textLabel.setLookAndFeel(nullptr);
}

void CabbageSlider::paint(Graphics& g)
{
    g.fillAll(Colours::transparentWhite);
    if (isFilmStripSlider)
    {
        const float sliderPos = (float)slider.valueToProportionOfLength(slider.getValue());

        int sliderValue = sliderPos * (numFrames - 1);

        if(!sliderBounds.isArray())
        {
        g.drawImage(filmStrip, filmStripBounds.getX(), filmStripBounds.getY(), filmStripBounds.getWidth(), filmStripBounds.getHeight(), 0, sliderValue * frameHeight, frameWidth, frameHeight);
        }
        else
        {
            g.drawImage(filmStrip, (int)sliderBounds[0], (int)sliderBounds[1], (int)sliderBounds[2], (int)sliderBounds[3], 0, sliderValue * frameHeight, frameWidth, frameHeight);
        }
    }
    else if (sliderBgImage.isValid())
    {
        if (sliderType.contains("vertical"))
            g.drawImage(sliderBgImage, slider.getWidth() / 2 - sliderBgImage.getWidth() / 2.f, slider.getY(), sliderBgImage.getWidth(), slider.getHeight(), 0, 0, sliderBgImage.getWidth(), sliderBgImage.getHeight(), false);
        else if(sliderType.contains("horizontal"))
            g.drawImage(sliderBgImage, sliderThumbImage.getWidth() /2, slider.getHeight() / 2.f - sliderBgImage.getHeight()/2.f, slider.getWidth()*.95f, sliderBgImage.getHeight(), 0, 0, sliderBgImage.getWidth(), sliderBgImage.getHeight(), false);
    }
}

void CabbageSlider::paintOverChildren(Graphics& g)
{

}

void CabbageSlider::initFilmStrip(ValueTree wData)
{
    numFrames = CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::filmstripframes);
    String path = CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::csdfile);
    File imageFile;
    if (path.isEmpty())
    {
        imageFile = File::getCurrentWorkingDirectory().getChildFile(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::filmstripimage)).getFullPathName();
    }
    else
    {
        imageFile = File(path).getParentDirectory().getChildFile(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::filmstripimage)).getFullPathName();
    }
    
    if (imageFile.existsAsFile())
    {
        isFilmStripSlider = true;
        //ImageCache::addImageToCache(ImageFileFormat::loadFrom(imageFile), imageFile.hashCode64());
        filmStrip = ImageCache::getFromFile(imageFile);
        if (!filmStrip.isNull())
        {
            slider.getProperties().set("filmstrip", 1);
            frameHeight = filmStrip.getHeight() / numFrames;
            frameWidth = filmStrip.getWidth();
        }
    }
}
void CabbageSlider::initialiseSlider(ValueTree wData, Slider& currentSlider)
{
    remove1 = CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::filmStripRemoveFrom1);
    remove2 = CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::filmStripRemoveFrom2);
    decimalPlaces = CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::decimalplaces);
    sliderIncrement = CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::increment);
    sliderSkew = CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::sliderskew);
    min = CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::min);
    max = CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::max);
    value = CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::value);
    shouldShowTextBox = CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::valuetextbox);
    trackerThickness = CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::trackerthickness);
    trackerInnerRadius = CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::trackerinsideradius);
    trackerOuterRadius = CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::trackeroutsideradius);
    currentSlider.getProperties().set("trackerthickness", trackerThickness);
    currentSlider.getProperties().set("trackerinnerradius", trackerInnerRadius);
    currentSlider.getProperties().set("trackerouterradius", trackerOuterRadius);
    currentSlider.getProperties().set("trackerCentre", CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::trackercentre));

    currentSlider.setSkewFactor(sliderSkew);
    currentSlider.setRange(min, max, sliderIncrement);
    currentSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);

    const String popup = CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::popuptext);
    if (popup == "0" || (popup == "" && popupPrefix == "" && popupPostfix == "" && shouldShowTextBox == 1))
        shouldDisplayPopup = false;
    else
        shouldDisplayPopup = true;

    if (getCurrentText(wData).isNotEmpty())
        textLabel.setVisible(true);
    else
        textLabel.setVisible(false);

    //currentSlider.setDoubleClickReturnValue(true, value);
    getSlider().setDoubleClickReturnValue(true, CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::defaultValue));
    setSliderVelocity(wData);
    currentSlider.addMouseListener(this, false);

//    PI * 1 - start
//    PI * 3 - end
    
    
    const float trackerStart = MathConstants<float>::pi * (1+(CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::trackerstart)*2.f));
    const float trackerEnd = MathConstants<float>::pi * (1+(CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::trackerend)*2.f));
    
    //const float trackerEnd(MathConstants<float>::pi * CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::trackerend));
    
    currentSlider.setRotaryParameters(trackerStart, trackerEnd, false);

    if (sliderType.contains("rotary"))
    {
        
        currentSlider.setSliderStyle(Slider::RotaryVerticalDrag);
        currentSlider.setRotaryParameters(trackerStart, trackerEnd, true);
    }
    else if (sliderType.contains("vertical"))
        currentSlider.setSliderStyle(Slider::LinearVertical);
    else if (sliderType.contains("horizontal"))
        currentSlider.setSliderStyle(Slider::LinearHorizontal);

    auto newValue = slider.getTextFromValue(value);
    filmStripValueBox.setText(newValue, dontSendNotification);

}

void CabbageSlider::setTextBoxOrientation(String type, int showTextBox)
{
    if (showTextBox == 1)
    {
        if (!shouldDisplayPopup)
            shouldDisplayPopup = false;
        setTextBoxWidth();
    }
    else
    {
        getSlider().setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    }
}

void CabbageSlider::setTextBoxWidth()
{
    //override value text box bounds if needed...
    var textBoxBounds = CabbageWidgetData::getProperty(widgetData, CabbageIdentifierIds::valuetextboxbounds);
    if(textBoxBounds.isArray())
    {
        slider.getProperties().set("valueTextBoxBounds", textBoxBounds);
    }

    int w = jmin(55.f, getWidth() * .65f);
    int h = 15;

    if (!isFilmStripSlider)
    {
        if (sliderType.contains("horizontal"))
            getSlider().setTextBoxStyle(Slider::TextBoxRight, false, w, h);
        else
            getSlider().setTextBoxStyle(Slider::TextBoxBelow, false, w, h);
    }
    else
    {
        if (sliderType.contains("rotary"))
            getSlider().setTextBoxStyle(Slider::TextBoxBelow, false, w, h);
    }


    slider.sendLookAndFeelChange();
    
    
    
    
}

void CabbageSlider::resized()
{
    if (sliderType.contains("rotary"))
    {
        if (getText().isNotEmpty())
        {
            textLabel.setText(getText(), dontSendNotification);
            textLabel.setJustificationType(Justification::centred);
            textLabel.setVisible(true);

            if (shouldShowTextBox == 1)
            {
                textLabel.setBounds(0, 0, getWidth(), getHeight() * .2f);
                if (isFilmStripSlider)
                    filmStripBounds.setBounds(getWidth() * .2f, getHeight() * .2f, getWidth() * .6f, getHeight() * .6f);

                getSlider().setBounds(0, getHeight() * .2f, getWidth(), getHeight() - getHeight() * .2f);
            }
            else
            {
                textLabel.setBounds(0, getHeight() - getHeight() * .2f, getWidth(), getHeight() * .2f);
                if (isFilmStripSlider)
                    filmStripBounds.setBounds(getWidth() * .09f, 0, getWidth() - getWidth() * .175f, getHeight() - getHeight() * .175f);
                //getSlider().setBounds(-getWidth() * .1f, -getHeight()*.1f, getWidth() + getWidth() * .2f, getHeight()+ getHeight() * .1f);

                getSlider().setBounds(0, 0, getWidth(), getHeight() - getHeight() * .15f);
            }
        }
        else
        {
            if (isFilmStripSlider)
            {
                if (shouldShowTextBox)
                    filmStripBounds.setBounds(getWidth() * .1f, 0, getWidth() - getWidth() * .195f, getHeight() - getHeight() * .195f);
                else
                    filmStripBounds.setBounds(getWidth() * .09f, getHeight() * .05f, getWidth() - getWidth() * .15f, getHeight() - getHeight() * .15f);
            }

            getSlider().setBounds(0, 0, getWidth(), getHeight());
        }

        if (shouldShowTextBox == 1)
            setTextBoxWidth();
    }

    //else if vertical
    else if (sliderType.contains("vertical"))
    {
        if (getText().isNotEmpty())
        {
            textLabel.setJustificationType(Justification::centred);
            textLabel.setText(getText(), dontSendNotification);
            textLabel.setVisible(true);

            if (shouldShowTextBox == 1)
            {

                textLabel.setBounds(0, 1, getWidth(), getHeight() * .1f);
                if (isFilmStripSlider)
                {
                    addAndMakeVisible(filmStripValueBox);
                    filmStripValueBox.setBounds(0, getHeight() - 20, getWidth(), 20);
                    filmStripBounds.setBounds(0, getHeight() * .1f, getWidth(), getHeight() - getHeight() * .15f);
                    getSlider().setBounds(0, getHeight() * remove1, getWidth(), getHeight() - getHeight() * remove2);
                }
                else
                    getSlider().setBounds(0, getHeight() * .1f, getWidth(), getHeight() - getHeight() * .1f);
            }
            else
            {
                textLabel.setBounds(0, getHeight() - getHeight() * .1f, getWidth(), getHeight() * .1f);
                if (isFilmStripSlider)
                {
                    filmStripBounds.setBounds(0, 0, getWidth(), getHeight() * .9);
                    getSlider().setBounds(0, getHeight() * remove1, getWidth(), getHeight() - getHeight() * remove2);
                }
                else
                    getSlider().setBounds(0, 0, getWidth(), getHeight() - getHeight() * .1f);
            }
        }
        else
        {
            if (isFilmStripSlider)
            {
                if (shouldShowTextBox)
                {
                    addAndMakeVisible(filmStripValueBox);
                    filmStripValueBox.setBounds(0, getHeight() - 20, getWidth(), 20);
                    filmStripBounds.setBounds(0, 0, getWidth(), getHeight() * .9f);
                    getSlider().setBounds(0, getHeight() * remove1, getWidth(), getHeight() - getHeight() * remove2);
                }
                else
                {
                    filmStripBounds.setBounds(0, 0, getWidth(), getHeight());
                    getSlider().setBounds(0, getHeight() *  remove1, getWidth(), getHeight() - getHeight() * remove2);
                }
            }
            else if (sliderThumbImage.isValid())
            {
                getSlider().setBounds(0, sliderThumbImage.getHeight() / 3, getWidth(), getHeight() - sliderThumbImage.getHeight() * .66f);
                const float sliderPos = (float)slider.valueToProportionOfLength(slider.getValue());
                const float pos = jmap(sliderPos, 1.f, 0.f, 0.f, float(getHeight() - sliderThumbImage.getHeight()));
                thumb.setBounds(getWidth() / 2 - sliderThumbImage.getWidth() / 2, pos, sliderThumbImage.getWidth(), sliderThumbImage.getHeight());

            }
            else
                getSlider().setBounds(0, 0, getWidth(), getHeight());
        }

        if (shouldShowTextBox == 1)
            setTextBoxWidth();
    }

    //else if horizontal
    else
    {
        if (getText().isNotEmpty())
        {
            //h=60, w=300
            const float width = textLabel.getFont().getStringWidthFloat(getText()) + 10.f;
            textLabel.setText(getText(), dontSendNotification);
            textLabel.setVisible(true);

            if (shouldShowTextBox == 1)
            {
                textLabel.setBounds(0, 0, width, getHeight());
                if (isFilmStripSlider)
                {
                    addAndMakeVisible(filmStripValueBox);
                    filmStripValueBox.setBounds(getWidth() - 50, getHeight() / 2.f - 12.5f, 50, 25.f);
                    filmStripBounds.setBounds(width - 3, 0, getWidth() - 90, getHeight());
                    getSlider().setBounds(width - 3 + getWidth() * remove1, 0, getWidth() - (width - 4) - getWidth() * remove2, getHeight());
                }
                else
                    getSlider().setBounds(width - 3, 0, getWidth() - (width - 4), getHeight());
            }
            else
            {
                textLabel.setBounds(0, 0, width, getHeight());
                if (isFilmStripSlider)
                {
                    filmStripBounds.setBounds(width - 3, 0, getWidth() - (width - 4), getHeight());
                    getSlider().setBounds(width - 3 + getWidth() * remove1, 0, getWidth() - (width - 4) - getWidth() * remove2, getHeight());
                }
                else
                {
                    getSlider().setBounds(width - 3, 0, getWidth() - (width - 4), getHeight());
                }

            }
        }
        else
        {
            if (isFilmStripSlider)
            {
                if (shouldShowTextBox)
                {
                    addAndMakeVisible(filmStripValueBox);
                    filmStripValueBox.setBounds(getWidth() - 50, getHeight() / 2.f - 12.5f, 50, 25.f);
                    filmStripBounds.setBounds(0, 0, getWidth() - 50, getHeight());
                    getSlider().setBounds(getWidth() * remove1, 0, getWidth() - getWidth() * remove2, getHeight());
                }
                else
                {
                    filmStripBounds.setBounds(0, 0, getWidth(), getHeight());
                    getSlider().setBounds(getWidth() * remove1, 0, getWidth() - getWidth() * remove2, getHeight());
                }

            }
            else if (sliderThumbImage.isValid())
            {
                //getSlider().setBounds(0, sliderThumbImage.getHeight() / 3, getWidth(), getHeight() - sliderThumbImage.getHeight() * .66f);
                getSlider().setBounds(sliderThumbImage.getWidth()*.33, 0, getWidth() - sliderThumbImage.getWidth()*.66, getHeight());
                const float sliderPos = (float)slider.valueToProportionOfLength(slider.getValue());
                const float pos = jmap(sliderPos, 1.f, 0.f, 0.f, float(getHeight() - sliderThumbImage.getHeight()));
               // thumb.setBounds(getWidth() / 2 - sliderThumbImage.getWidth() / 2, pos, sliderThumbImage.getWidth(), sliderThumbImage.getHeight());
                thumb.setBounds(pos, getHeight() / 2 - sliderThumbImage.getHeight() / 2, sliderThumbImage.getWidth(), sliderThumbImage.getHeight());

            }
            else
                getSlider().setBounds(0, 0, getWidth(), getHeight());
        }


        if (shouldShowTextBox == 1)
            setTextBoxWidth();


        if (shouldShowTextBox == 1)
            setTextBoxWidth();
    }

    getSlider().setValue(value, dontSendNotification);
    if (sliderThumbImage.isValid())
        thumb.move(value, slider.getRange());

    if(getActive() == 0)
    {
        thumb.setEnabled(false);
    }
}

void CabbageSlider::createPopupBubble()
{
    //create popup display for showing value of sliders.
    popupBubble.setColour(BubbleComponent::backgroundColourId, Colours::white);
    popupBubble.setBounds(0, 0, 50, 20);
    owner->addChildComponent(popupBubble);
    popupBubble.setVisible(false);
    popupBubble.setAlwaysOnTop(true);
}

void CabbageSlider::showPopupBubble(int time)
{
    popupText = createPopupBubbleText(getSlider().getValue(),
        decimalPlaces,
        channel,
        popupPrefix,
        popupPostfix);

    popupBubble.showAt(&getSlider(), AttributedString(popupText), time);
}

void CabbageSlider::mouseDrag(const MouseEvent& event)
{
    if (shouldDisplayPopup)
        showPopupBubble(10);
}

void CabbageSlider::mouseDown(const MouseEvent& event)
{
    if (shouldDisplayPopup)
        showPopupBubble(1000);
}

void CabbageSlider::mouseMove(const MouseEvent& event)
{
    if (shouldDisplayPopup)
        showPopupBubble(100);
}

void CabbageSlider::mouseUp(const MouseEvent& event)
{
    if (shouldDisplayPopup)
        popupBubble.setVisible(false);
}

void CabbageSlider::mouseEnter(const MouseEvent& event)
{
    if (shouldDisplayPopup)
        showPopupBubble(100);
}

void CabbageSlider::mouseExit(const MouseEvent& event)
{
    if (shouldDisplayPopup)
        popupBubble.setVisible(false);
}

void CabbageSlider::setSliderVelocity(ValueTree wData)
{
    velocity = CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::velocity);

    if (velocity > 0)
    {
        getSlider().setVelocityModeParameters(velocity, 1, 0.0, true);
        getSlider().setVelocityBasedMode(true);
    }
    else
        getSlider().setVelocityBasedMode(false);
}

void CabbageSlider::setLookAndFeelColours(ValueTree wData)
{

    getSlider().setColour(Slider::thumbColourId, Colour::fromString(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::colour)));
    getSlider().setColour(Slider::backgroundColourId, Colour::fromString(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::backgroundcolour)));
    getSlider().setColour(Slider::trackColourId, Colour::fromString(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::trackercolour)));
    getSlider().setColour(Slider::rotarySliderOutlineColourId, Colour::fromString(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::outlinecolour)));

    textLabel.setColour(Label::outlineColourId, Colours::transparentBlack);
    getSlider().setColour(Slider::textBoxHighlightColourId, Colours::lime.withAlpha(.2f));
    getSlider().setColour(TextEditor::textColourId, Colour::fromString(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::fontcolour)));
    textLabel.setColour(Label::textColourId, Colour::fromString(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::textcolour)));

    getSlider().setColour(Slider::textBoxTextColourId, Colour::fromString(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::fontcolour)));
    getSlider().setColour(Slider::textBoxBackgroundColourId, Colour::fromString(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::textboxcolour)));
    getSlider().setColour(Slider::textBoxHighlightColourId, Colour::fromString(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::textboxcolour)).contrasting());
    getSlider().setColour(Slider::textBoxOutlineColourId, Colour::fromString(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::textboxoutlinecolour)));

    getSlider().setColour(Label::textColourId, Colour::fromString(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::fontcolour)));
    getSlider().setColour(Label::backgroundColourId, CabbageUtilities::getBackgroundSkin());

    filmStripValueBox.setColour(Label::textColourId, Colour::fromString(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::fontcolour)));
    filmStripValueBox.setColour(Label::backgroundColourId, Colour::fromString(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::textboxcolour)));
    //filmStripValueBox.setColour(Label::ColourIds::backgroundWhenEditingColourId, Colour::fromString(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::textboxcolour)).contrasting());
    filmStripValueBox.lookAndFeelChanged();

    getSlider().getProperties().set("trackerthickness", CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::trackerthickness));
    getSlider().getProperties().set("trackerbgcolour", CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::trackerbgcolour));
    getSlider().getProperties().set("markerthickness", CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::markerthickness));
    getSlider().getProperties().set("markerstart", CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::markerstart));
    getSlider().getProperties().set("markerend", CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::markerend));
    getSlider().getProperties().set("gapmarkers", CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::gapmarkers));

    getSlider().getProperties().set("markercolour", CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::markercolour));
    getSlider().getProperties().set("trackerthickness", CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::trackerthickness));
    getSlider().getProperties().set("trackerbgcolour", CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::trackerbgcolour));
    getSlider().getProperties().set("markerthickness", CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::markerthickness));
    getSlider().getProperties().set("markerstart", CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::markerstart));
    getSlider().getProperties().set("markerend", CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::markerend));
    getSlider().getProperties().set("gapmarkers", CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::gapmarkers));

    getSlider().setColour(Label::outlineColourId, CabbageUtilities::getBackgroundSkin());
    getSlider().lookAndFeelChanged();
}

//==============================================================================
void CabbageSlider::valueTreePropertyChanged(ValueTree& valueTree, const Identifier& prop)
{

    if (prop == CabbageIdentifierIds::value)
    {
        if (PluginHostType::getPluginLoadedAs() != AudioProcessor::wrapperType_AudioUnit)
        {
           const MessageManagerLock lock;
        }
        
        //this is causing some weird jumpy issue in Live
        getSlider().setValue(CabbageWidgetData::getNumProp(valueTree, CabbageIdentifierIds::value), dontSendNotification);
        if (sliderThumbImage.isValid())
            thumb.move(CabbageWidgetData::getNumProp(valueTree, CabbageIdentifierIds::value), slider.getRange());

        repaint();
    }
    else if(prop == CabbageIdentifierIds::defaultValue)
    {
        getSlider().setDoubleClickReturnValue(true, CabbageWidgetData::getNumProp(valueTree, CabbageIdentifierIds::defaultValue));
    }
    else if(prop == CabbageIdentifierIds::imgslider)
    {
        auto csdPath = owner->getProcessor().getCsdFile().getFullPathName();
        setImgProperties(this->slider, valueTree, csdPath, "slider");
        setImgProperties(this->slider, valueTree, csdPath, "sliderbg");
        repaint();
    }
    else
    {
        textLabel.setText(getCurrentText(valueTree), dontSendNotification);
        textLabel.setVisible(getCurrentText(valueTree).isNotEmpty() ? true : false);
        shouldShowTextBox = CabbageWidgetData::getNumProp(valueTree, CabbageIdentifierIds::valuetextbox);
        setTextBoxOrientation(sliderType, shouldShowTextBox);
        

        getSlider().getProperties().set("trackerthickness", CabbageWidgetData::getNumProp(valueTree, CabbageIdentifierIds::trackerthickness));
        getSlider().getProperties().set("trackerinnerradius", CabbageWidgetData::getNumProp(valueTree, CabbageIdentifierIds::trackerinsideradius));
        getSlider().getProperties().set("trackerouterradius", CabbageWidgetData::getNumProp(valueTree, CabbageIdentifierIds::trackeroutsideradius));

        
        handleCommonUpdates(this, valueTree, false, prop);
        
        if(prop == CabbageIdentifierIds::left || prop == CabbageIdentifierIds::top ||
           prop == CabbageIdentifierIds::width || prop == CabbageIdentifierIds::height ||
           prop == CabbageIdentifierIds::bounds)
        {
            //after resizing, make sure the slider's thumb is in the right position
            DBG(CabbageWidgetData::getNumProp(valueTree, CabbageIdentifierIds::value));
            getSlider().setValue(CabbageWidgetData::getNumProp(valueTree, CabbageIdentifierIds::value), dontSendNotification);
        }
        
        setLookAndFeelColours(valueTree);

        const String popup = CabbageWidgetData::getStringProp(valueTree, CabbageIdentifierIds::popuptext);
        if (popup == "0" || (popup == "" && popupPrefix == "" && popupPostfix == "" && shouldShowTextBox == 1))
        {
            getSlider().setTooltip("");
            shouldDisplayPopup = false;
        }
        else 
        {
            shouldDisplayPopup = true;
            getSlider().setTooltip(getCurrentPopupText(valueTree));
        }
        
        
    }

}
