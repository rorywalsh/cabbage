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

#include "CabbageGroupBox.h"
#include "../Audio/Plugins/CabbagePluginEditor.h"

CabbageGroupBox::CabbageGroupBox (ValueTree wData, CabbagePluginEditor* _owner)
    : GroupComponent (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::name)),
    outlineThickness (CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::outlinethickness)),
    lineThickness (CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::linethickness)),
    corners (CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::corners)),
     text (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::text)),
    colour (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::colour)),
    fontColour (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::fontcolour)),
    justification (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::align)),
    outlineColour (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::outlinecolour)),
    owner (_owner),
    widgetData (wData),
    lookAndFeel(),
    CabbageWidgetBase(_owner)
{
    setName (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::name));
    widgetData.addListener (this);              //add listener to valueTree so it gets notified when a widget's property changes
    initialiseCommonAttributes (this, wData);   //initialise common attributes such as bounds, name, rotation, etc..

    setColour (TextButton::buttonColourId, Colour::fromString (colour));
    setColour (GroupComponent::textColourId, Colour::fromString (fontColour));
    setColour (GroupComponent::outlineColourId, Colour::fromString (outlineColour));

    setName (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::name));
    setText (text);
    setWantsKeyboardFocus (false);

    setTextLabelPosition (CabbageUtilities::getJustification (justification));

    getProperties().set ("outlinethickness", outlineThickness);
    getProperties().set ("linethickness", lineThickness);
    getProperties().set ("cornersize", corners);
    auto csdPath = owner->getProcessor().getCsdFile().getFullPathName();
    setImgProperties (*this, wData, csdPath, "groupbox");
    
    lookAndFeel.customFont = owner->customFont;
    setLookAndFeel(&lookAndFeel);
}

void CabbageGroupBox::changeListenerCallback (ChangeBroadcaster* source)
{
    CabbageWidgetData::setNumProp (widgetData, CabbageIdentifierIds::visible, 0);
}

void CabbageGroupBox::valueTreePropertyChanged (ValueTree& valueTree, const Identifier& prop)
{
    if (CabbagePluginEditor::PopupDocumentWindow* parentComp = dynamic_cast<CabbagePluginEditor::PopupDocumentWindow*> (getParentComponent()))
    {
        const int parentIsVisible = CabbageWidgetData::getNumProp (valueTree, CabbageIdentifierIds::visible);
        owner->addChangeListener (this);

        if (parentIsVisible == 1)
        {
            parentComp->setVisible (true);
            parentComp->toFront (true);
        }
        else
            parentComp->setVisible (false);
    }

    getProperties().set ("groupLine", var (CabbageWidgetData::getNumProp (valueTree, CabbageIdentifierIds::linethickness)));
    getProperties().set ("cornersize", CabbageWidgetData::getNumProp (valueTree, CabbageIdentifierIds::corners));
    setTextLabelPosition (CabbageUtilities::getJustification (CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::align)));
    setColour (TextButton::buttonColourId, Colour::fromString (CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::colour)));
    setColour (GroupComponent::textColourId, Colour::fromString (CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::fontcolour)));
    setColour (GroupComponent::outlineColourId, Colour::fromString (CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::outlinecolour)));
    setText (CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::text));
    getProperties().set ("cornersize", CabbageWidgetData::getNumProp (valueTree, CabbageIdentifierIds::corners));
    getProperties().set ("outlinethickness", CabbageWidgetData::getNumProp (valueTree, CabbageIdentifierIds::outlinethickness));
    getProperties().set ("linethickness", CabbageWidgetData::getNumProp (valueTree, CabbageIdentifierIds::linethickness));
    
    isVisible = CabbageWidgetData::getNumProp (valueTree, CabbageIdentifierIds::visible);
    
    handleCommonUpdates (this, valueTree, prop);      //handle comon updates such as bounds, alpha, rotation, visible, etc

    
    
}

