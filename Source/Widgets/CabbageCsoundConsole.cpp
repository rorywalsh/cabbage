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

#include "CabbageCsoundConsole.h"
#include "../Audio/Plugins/CabbagePluginEditor.h"

CabbageCsoundConsole::CabbageCsoundConsole (ValueTree wData, CabbagePluginEditor* _owner)
    :  TextEditor (""),
    owner (_owner),
    widgetData (wData)
{
    setName (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::name));
    widgetData.addListener (this);              //add listener to valueTree so it gets notified when a widget's property changes
    initialiseCommonAttributes (this, wData);   //initialise common attributes such as bounds, name, rotation, etc..
    this->setMultiLine (true, false);
    this->setScrollbarsShown (true);

    setColour (TextEditor::textColourId, Colour::fromString (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::fontcolour)));
    setColour (TextEditor::backgroundColourId, Colour::fromString (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::colour)));
    setColour (TextEditor::outlineColourId, Colours::transparentBlack);
    setColour (TextEditor::focusedOutlineColourId, Colours::transparentBlack);
    setColour (TextEditor::highlightColourId, Colour::fromString (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::fontcolour)).contrasting (.5f));

    if (CabbageUtilities::getTarget() == CabbageUtilities::TargetTypes::IDE)
    {
        const String initText ("========================================\nCsound output messages are only sent to\nthis widget when your Cabbage instrument\nis running in plugin mode.\n========================================");
        setText (initText);
    }
    else
        startTimer (100);

    this->monospacedFont.setTypefaceName(Font::getDefaultMonospacedFontName());
    setMonospaced(wData);
}

void CabbageCsoundConsole::setMonospaced(bool valueToUse)
{
    if (this->monospaced == valueToUse)
    {
        return;
    }
    this->monospaced = valueToUse;
    applyFontToAllText(this->monospaced ? monospacedFont : defaultFont);
}

void CabbageCsoundConsole::setMonospaced(const ValueTree &valueTree)
{
    setMonospaced(CabbageWidgetData::getStringProp(valueTree, CabbageIdentifierIds::style).contains("monospaced"));
}

void CabbageCsoundConsole::timerCallback()
{
    if (CabbageUtilities::getTarget() == CabbageUtilities::TargetTypes::IDE)
    {
        const String initText ("===========================\nCsound output messages are only sent to\nthis widget when your Cabbage instrument\nis running in plugin mode.\n===========================");
        setText (initText);
    }
    else
    {
        const String csoundOutputString = owner->getCsoundOutputFromProcessor();

        if (csoundOutputString.isNotEmpty())
        {
            insertTextAtCaret (csoundOutputString);
        }
    }
}

void CabbageCsoundConsole::valueTreePropertyChanged (ValueTree& valueTree, const Identifier& prop)
{
    setColour (TextEditor::textColourId, Colour::fromString (CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::fontcolour)));
    setColour (TextEditor::backgroundColourId, Colour::fromString (CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::colour)));
    setMonospaced(valueTree);
    lookAndFeelChanged();
    repaint();
    handleCommonUpdates (this, valueTree);      //handle comon updates such as bounds, alpha, rotation, visible, etc
}
