/*
  ==============================================================================

    CabbageOptionButton.cpp
    Created: 16 Mar 2021 7:17:06pm
    Author:  walshr

  ==============================================================================
*/

#include "CabbageOptionButton.h"


CabbageOptionButton::CabbageOptionButton(ValueTree wData, CabbagePluginEditor* _owner)
: TextButton(),
owner(_owner),
widgetData(wData),
CabbageWidgetBase(_owner)
{
    widgetData.addListener(this);              //add listener to valueTree so it gets notified when a widget's property changes
    initialiseCommonAttributes(this, wData);   //initialise common attributes such as bounds, name, rotation, etc..
    setButtonText(getTextArray()[getValue()]);
    
    tooltipText = CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::popuptext);
    if (tooltipText.isNotEmpty())
        setTooltip(tooltipText);
    

    setImgProperties(*this, wData, "buttonon");
    setImgProperties(*this, wData, "buttonoff");
    setImgProperties(*this, wData, "buttonover");
    
    const String imgOff = CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::imgbuttonoff);
    const String imgOver = CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::imgbuttonover);
    const String imgOn = CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::imgbuttonon);
    
    setLookAndFeelColours(wData);
    
    getProperties().set("outlinecolour", CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::outlinecolour));
    getProperties().set("outlinethickness", CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::outlinethickness));
    getProperties().set("corners", CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::corners));

    if (owner->globalStyle == "legacy")
    {
        return;
    }
    
    //if users are passing custom images, use old style look and feel
    if (CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::style) == "flat" &&
        imgOff.isEmpty() && imgOn.isEmpty() && imgOver.isEmpty())
    {
        int fontstyle = CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::fontstyle);
        owner->customFont.setStyleFlags(fontstyle);
        flatLookAndFeel.customFont = owner->customFont;
        setLookAndFeel(&flatLookAndFeel);
        lookAndFeelChanged();
        setLookAndFeel(&flatLookAndFeel);
    }
    
    
}

void CabbageOptionButton::setLookAndFeelColours(ValueTree wData)
{
    setColour(TextButton::textColourOffId, Colour::fromString(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::fontcolour)));
    setColour(TextButton::buttonColourId, Colour::fromString(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::colour)));
    setColour(TextButton::textColourOnId, Colour::fromString(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::onfontcolour)));
    setColour(TextButton::buttonOnColourId, Colour::fromString(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::oncolour)));
    
}

void CabbageOptionButton::valueTreePropertyChanged(ValueTree& valueTree, const Identifier& prop)
{
    
    if (prop == CabbageIdentifierIds::value)
    {
        CabbageUtilities::debug(CabbageWidgetData::getStringProp(valueTree, CabbageIdentifierIds::name));
        CabbageUtilities::debug(CabbageWidgetData::getNumProp(valueTree, CabbageIdentifierIds::value));
        setValue(CabbageWidgetData::getNumProp(valueTree, CabbageIdentifierIds::value));
        //setToggleState(getValue() == 0 ? false : true, dontSendNotification);
        setButtonText(getTextArray()[getValue()]);
    }
    else
    {
        setLookAndFeelColours(valueTree);
        handleCommonUpdates(this, valueTree, false, prop);      //handle comon updates such as bounds, alpha, rotation, visible, etc
        //populateTextArrays(valueTree);
        //const String newText = CabbageWidgetData::getStringProp(valueTree, CabbageIdentifierIds::text);
        //if(newText != getTextArray()[getValue()])
        //CabbageUtilities::debug(getTextArray()[getValue()]);
        setButtonText(getTextArray()[getValue()]);
        setTooltip(getCurrentPopupText(valueTree));
    }
}
