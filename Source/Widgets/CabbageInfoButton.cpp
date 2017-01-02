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

CabbageInfoButton::CabbageInfoButton(ValueTree wData)
    : widgetData(wData),
      TextButton()
{
    widgetData.addListener(this); 				//add listener to valueTree so it gets notified when a widget's property changes
    initialiseCommonAttributes(this, wData); 	//initialise common attributes such as bounds, name, rotation, etc..
    setLookAndFeelColours(wData);

    setButtonText(getText());

    filename = CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::file);

    setImgProperties(*this, wData, "buttonon");
    setImgProperties(*this, wData, "buttonoff");
    addListener(this);

}

//===============================================================================
void CabbageInfoButton::buttonClicked(Button* button)
{
    CabbageUtilities::debug(File(getCsdFile()).getParentDirectory().getChildFile(filename).getFullPathName());
    if(File(getCsdFile()).getParentDirectory().getChildFile(filename).existsAsFile())
    {
        URL url(filename);
        url.launchInDefaultBrowser();
    }
}

//===============================================================================
void CabbageInfoButton::setLookAndFeelColours(ValueTree wData)
{
    setColour(TextButton::textColourOffId, Colour::fromString(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::fontcolour)));
    setColour(TextButton::buttonColourId, Colour::fromString(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::colour)));
    setColour(TextButton::textColourOnId, Colour::fromString(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::onfontcolour)));
    setColour(TextButton::buttonOnColourId, Colour::fromString(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::oncolour)));
}

//===============================================================================
void CabbageInfoButton::valueTreePropertyChanged (ValueTree& valueTree, const Identifier& prop)
{
    setLookAndFeelColours(valueTree);
    handleCommonUpdates(this, valueTree);		//handle comon updates such as bounds, alpha, rotation, visible, etc

    setButtonText(getText());
}