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

#include "CabbageLabel.h"
#include "../Audio/Plugins/CabbagePluginEditor.h"


CabbageLabel::CabbageLabel (ValueTree wData, CabbagePluginEditor* _owner)
    : widgetData (wData),
      text (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::text)),
      owner (_owner),
      colour (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::colour)),
      fontcolour (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::fontcolour)),
      align (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::align)),
      textAlign (Justification::centred),
      fontstyle (CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::fontstyle)),
      corners (CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::corners)),
      counter (0)
{
    setName (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::name));

    widgetData.addListener (this);              //add listener to valueTree so it gets notified when a widget's property changes
    initialiseCommonAttributes (this, wData);   //initialise common attributes such as bounds, name, rotation, etc..

    textAlign = CabbageUtilities::getJustification (align);

}

void CabbageLabel::paint (Graphics& g)
{
    g.setColour (Colour::fromString (colour));
    g.fillRoundedRectangle (getLocalBounds().toFloat(), corners);
    g.setColour (Colour::fromString (fontcolour));
    g.setFont (CabbageUtilities::getComponentFont (fontstyle));
    Label tmp;
    g.setFont (getLookAndFeel().getLabelFont(tmp));

    g.setFont (getHeight());
    g.drawFittedText (text, 0, 0, jmax (1, getWidth()), jmax (1, getHeight()), textAlign, 1, 1);
}

void CabbageLabel::setText (String _text)
{
    text = _text;
    repaint();
}

void CabbageLabel::mouseDown (const MouseEvent& event)
{
    if (!event.mods.isPopupMenu())
    {
        counter = (counter == 0 ? 1 : 0);
        owner->sendChannelDataToCsound (getChannel(), counter);
    }
}

void CabbageLabel::valueTreePropertyChanged (ValueTree& valueTree, const Identifier& prop)
{
    textAlign = CabbageUtilities::getJustification (CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::align));
    setText (this->getCurrentText (valueTree));

    if (fontstyle != CabbageWidgetData::getNumProp (valueTree, CabbageIdentifierIds::fontstyle))
    {
        fontstyle = CabbageWidgetData::getNumProp (valueTree, CabbageIdentifierIds::fontstyle);
    }

    if (colour != CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::colour))
    {
        colour = CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::colour);
    }

    if (fontcolour != CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::fontcolour))
    {
        fontcolour = CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::fontcolour);
    }

    handleCommonUpdates (this, valueTree);      //handle comon updates such as bounds, alpha, rotation, visible, etc

    repaint();

}
