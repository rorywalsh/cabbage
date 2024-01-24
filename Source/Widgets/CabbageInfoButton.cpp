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

#include "CabbageInfoButton.h"
#include "../Audio/Plugins/CabbagePluginEditor.h"


CabbageInfoButton::CabbageInfoButton (ValueTree wData, CabbagePluginEditor* _owner, String style)
    : widgetData (wData),
      TextButton(),
    CabbageWidgetBase(_owner)
{
    widgetData.addListener (this);              //add listener to valueTree so it gets notified when a widget's property changes
    initialiseCommonAttributes (this, wData);   //initialise common attributes such as bounds, name, rotation, etc..
    setLookAndFeelColours (wData);

    setButtonText (getText());

    filename = CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::file);
    auto csdPath = owner->getProcessor().getCsdFile().getFullPathName();
    setImgProperties (*this, wData, csdPath, "buttonon");
    setImgProperties (*this, wData, csdPath, "buttonoff");
    addListener (this);

    const String imgOff = CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::imgbuttonoff);
    const String imgOver = CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::imgbuttonover);
    const String imgOn = CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::imgbuttonon);
    
    if(style == "legacy")
    {
        return;
    }
    
    //if users are passing custom images, use old style look and feel
    if (CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::style) == "flat" &&
        imgOff.isEmpty() && imgOn.isEmpty() && imgOver.isEmpty())
    {
        setLookAndFeel(&flatLookAndFeel);
    }
}

//===============================================================================
void CabbageInfoButton::buttonClicked (Button* button)
{    
    if (File(getCsdFile()).getParentDirectory().getChildFile (filename).existsAsFile() || filename.contains("http://") || filename.contains("https://"))
    {
        URL url (filename);
        url.launchInDefaultBrowser();
    }

}

//===============================================================================
void CabbageInfoButton::setLookAndFeelColours (ValueTree wData)
{
    setColour (TextButton::textColourOffId, Colour::fromString (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::fontcolour)));
    setColour (TextButton::buttonColourId, Colour::fromString (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::colour)));
    setColour (TextButton::textColourOnId, Colour::fromString (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::onfontcolour)));
    setColour (TextButton::buttonOnColourId, Colour::fromString (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::oncolour)));
}

//===============================================================================
void CabbageInfoButton::valueTreePropertyChanged (ValueTree& valueTree, const Identifier& prop)
{
    setLookAndFeelColours (valueTree);
    handleCommonUpdates (this, valueTree, false, prop);      //handle comon updates such as bounds, alpha, rotation, visible, etc

    setButtonText (getText());
}
