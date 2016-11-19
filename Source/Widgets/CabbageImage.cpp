/*
  ==============================================================================

    CabbageImage.cpp
    Created: 14 Nov 2016 2:30:11pm
    Author:  rory

  ==============================================================================
*/

#include "CabbageImage.h"

CabbageImage::CabbageImage(ValueTree wData, CabbagePluginEditor* owner) : CabbageWidgetBase(),
    widgetData(wData),
    owner(owner),
    shape(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::shape)),
    corners(CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::corners)),
    lineThickness(CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::linethickness)),
    outlineColour(Colour::fromString(CabbageWidgetData::getStringProp(widgetData, CabbageIdentifierIds::outlinecolour))),
    mainColour(Colour::fromString(CabbageWidgetData::getStringProp(widgetData, CabbageIdentifierIds::colour)))
{
    widgetData.addListener(this);
    File imgFile = File::getCurrentWorkingDirectory().getChildFile(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::file));
    this->setWantsKeyboardFocus(false);
    initialiseCommonAttributes(this, wData);
}

//==============================================================================
void CabbageImage::paint(Graphics& g)
{

    if(imgFile.existsAsFile())
    {
        if(imgFile.hasFileExtension(".svg"))
        {
            CabbageLookAndFeel2::drawFromSVG(g, imgFile, getSVGWidth(imgFile), getSVGWidth(imgFile), getWidth(), getHeight(), AffineTransform::identity);
        }
        else
            g.drawImage(ImageCache::getFromFile(imgFile), 0, 0, getWidth(), getHeight(), 0, 0,
                        ImageCache::getFromFile(imgFile).getWidth(),
                        ImageCache::getFromFile(imgFile).getHeight());
    }
    else
    {
        g.fillAll(Colours::transparentBlack);
        g.setColour(outlineColour);

        if(shape=="square")
            g.fillRoundedRectangle(0,0, getWidth(), getHeight(), corners);
        else
            g.fillEllipse(0,0, getWidth(), getHeight());

        g.setColour(mainColour);

        if(shape=="square")
            g.fillRoundedRectangle(lineThickness,lineThickness, getWidth()-(lineThickness*2), getHeight()-(lineThickness*2), corners);
        else
            g.fillEllipse(lineThickness,lineThickness, getWidth()-(lineThickness*2), getHeight()-(lineThickness*2));
    }
}


//==============================================================================
void CabbageImage::valueTreePropertyChanged (ValueTree& valueTree, const Identifier& prop)
{

    if(prop==CabbageIdentifierIds::value)
    {
        bool state = CabbageWidgetData::getNumProp(valueTree, CabbageIdentifierIds::value)==1 ? true : false;
        //setToggleState(getValue(valueTree)==1 ? true : false, dontSendNotification);
    }

    lineThickness = CabbageWidgetData::getNumProp(valueTree, CabbageIdentifierIds::linethickness),
    outlineColour = Colour::fromString(CabbageWidgetData::getStringProp(valueTree, CabbageIdentifierIds::outlinecolour));
    mainColour = Colour::fromString(CabbageWidgetData::getStringProp(valueTree, CabbageIdentifierIds::colour));
    shape = CabbageWidgetData::getStringProp(valueTree, CabbageIdentifierIds::shape);

    handleCommonUpdates(this, valueTree);
    repaint();
}

