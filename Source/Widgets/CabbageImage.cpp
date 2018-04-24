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

#include "CabbageImage.h"
#include "../Audio/Plugins/CabbagePluginEditor.h"

CabbageImage::CabbageImage (ValueTree wData, CabbagePluginEditor* owner, bool isLineWidget) : CabbageWidgetBase(),
    widgetData (wData),
    owner (owner),
    isLineWidget (isLineWidget),
    shape (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::shape)),
    corners (CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::corners)),
    cropy (CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::cropy)),
    cropx (CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::cropx)),
    cropwidth (CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::cropwidth)),
    cropheight (CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::cropheight)),
    lineThickness (CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::outlinethickness)),
    outlineColour (Colour::fromString (CabbageWidgetData::getStringProp (widgetData, CabbageIdentifierIds::outlinecolour))),
    mainColour (Colour::fromString (CabbageWidgetData::getStringProp (widgetData, CabbageIdentifierIds::colour)))
{
    widgetData.addListener (this);
	String file = File(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::csdfile)).getFullPathName();
    imgFile = File(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::csdfile)).getParentDirectory().getChildFile (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::file));
    img = ImageFileFormat::loadFrom(imgFile);
    this->setWantsKeyboardFocus (false);
    initialiseCommonAttributes (this, wData);
}

//==============================================================================
void CabbageImage::paint (Graphics& g)
{
    if (isLineWidget)
    {
        g.setColour (mainColour);
        g.fillRoundedRectangle (0, 0, getWidth(), getHeight(), 1);

        g.setColour (CabbageUtilities::getBackgroundSkin());
        g.fillRoundedRectangle (0, 0, getWidth() - 1, getHeight() - 1, 1);
    }
    else
    {
        
        if (imgFile.existsAsFile())
        {
            if (imgFile.hasFileExtension (".svg"))
            {
                CabbageLookAndFeel2::drawFromSVG (g, imgFile, 0, 0, getWidth(), getHeight(), AffineTransform::identity);
            }
            else
                g.drawImage (img, 0, 0, getWidth(), getHeight(), cropx, cropy,
                             cropwidth == 0 ? img.getWidth() : cropwidth,
                             cropheight == 0 ? img.getHeight() : cropheight);
        }
        else
        {
            g.fillAll (Colours::transparentBlack);
            g.setColour (mainColour);

            if (shape == "square")
                g.fillRoundedRectangle (0, 0, getWidth(), getHeight(), corners);
            else
                g.fillEllipse (lineThickness, lineThickness, getWidth(), getHeight());


            g.setColour (outlineColour);

            if (shape == "square")
                g.drawRoundedRectangle (0, 0, jmax (1, getWidth()), jmax (1, getHeight()), corners, lineThickness);
            else
                g.drawEllipse (0, 0, jmax (1, getWidth()), jmax (1, getHeight()), lineThickness);
        }
    }
}
//==============================================================================
void CabbageImage::mouseDown (const MouseEvent& e)
{
    owner->sendChannelDataToCsound (getChannel(), currentToggleValue);
    currentToggleValue =! currentToggleValue;
}

void CabbageImage::changeListenerCallback (ChangeBroadcaster* source)
{
    CabbageWidgetData::setNumProp (widgetData, CabbageIdentifierIds::visible, 0);
}
//==============================================================================
void CabbageImage::valueTreePropertyChanged (ValueTree& valueTree, const Identifier& prop)
{

    if (CabbagePluginEditor::PopupDocumentWindow* owner = dynamic_cast<CabbagePluginEditor::PopupDocumentWindow*> (getParentComponent()))
    {
        const int visible = CabbageWidgetData::getNumProp (valueTree, CabbageIdentifierIds::visible);
        owner->addChangeListener (this);

        if (visible == 1)
        {
            owner->setVisible (true);
            owner->toFront (true);
        }
        else
            owner->setVisible (false);
    }

    lineThickness = CabbageWidgetData::getNumProp (valueTree, CabbageIdentifierIds::outlinethickness);
    outlineColour = Colour::fromString (CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::outlinecolour));
    mainColour = Colour::fromString (CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::colour));
    shape = CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::shape);
    imgFile = File(CabbageWidgetData::getStringProp(valueTree, CabbageIdentifierIds::csdfile)).getParentDirectory().getChildFile (CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::file));
    cropy = CabbageWidgetData::getNumProp (valueTree, CabbageIdentifierIds::cropy);
    cropx = CabbageWidgetData::getNumProp (valueTree, CabbageIdentifierIds::cropx);
    cropwidth = CabbageWidgetData::getNumProp (valueTree, CabbageIdentifierIds::cropwidth);
    cropheight = CabbageWidgetData::getNumProp (valueTree, CabbageIdentifierIds::cropheight);
    handleCommonUpdates (this, valueTree);
    repaint();
}

