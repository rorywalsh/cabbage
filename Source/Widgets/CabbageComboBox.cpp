/*
  Copyright (C) 2016 Rory Walsh

  Cabbage is free software; you can redistribute it
  and/or modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  Cabbage is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with Csound; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
  02111-1307 USA
*/

#include "CabbageComboBox.h"

//================================================================================================================
// combobox widget
//================================================================================================================
CabbageComboBox::CabbageComboBox(ValueTree wData, CabbagePluginEditor* _owner):
    name(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::name)),
    rotate(CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::rotate)),
    pivotx(CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::pivotx)),
    pivoty(CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::pivoty)),
    tooltipText(String::empty),
    refresh(0),
    owner(_owner),
    widgetData(wData)
{
    widgetData.addListener(this);
    isPresetCombo = false;
    setColour(ComboBox::backgroundColourId, Colour::fromString(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::colour)));
    setColour(ComboBox::textColourId, Colour::fromString(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::fontcolour)));
    setTooltip(tooltipText = CabbageWidgetData::getStringProp(widgetData, CabbageIdentifierIds::popuptext));
    setEditableText (false);
    setTextWhenNothingSelected(text);
    setWantsKeyboardFocus(false);

    addItemsToCombobox(wData);
    setSelectedItemIndex(CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::value)-1, isPresetCombo ? sendNotification : dontSendNotification);

    initialiseCommonAttributes(this, wData);

}
//---------------------------------------------
CabbageComboBox::~CabbageComboBox()
{

}

void CabbageComboBox::addItemsToCombobox(ValueTree wData)
{
    Array<File> dirFiles;
    StringArray fileNames;

    if(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::file).isNotEmpty())
    {
        clear(dontSendNotification);
        String file = File(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::file)).loadFileAsString();
        StringArray lines = StringArray::fromLines(file);
        for (int i = 0; i < lines.size(); ++i)
        {
            addItem(lines[i], i+1);
        }
    }

    else if(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::filetype).isEmpty())
    {
        clear(dontSendNotification);
        var items = CabbageWidgetData::getProperty(wData, CabbageIdentifierIds::text);

        for(int i=0; i<items.size(); i++)
        {
            const String item  = items[i].toString();
            addItem(item, i+1);
        }
    }
    else
    {
        clear(dontSendNotification);
        pluginDir = File(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::workingdir));
        filetype = CabbageWidgetData::getStringProp(wData, "filetype");
        pluginDir.findChildFiles(dirFiles, 2, false, filetype);

        for (int i = 0; i < dirFiles.size(); ++i)
        {
            String filename;
            if(filetype.contains("snaps"))
            {
                filename = dirFiles[i].getFileNameWithoutExtension();
                isPresetCombo = true;
            }
            else
                filename = dirFiles[i].getFileName();

            fileNames.add(filename);
        }

        fileNames.sort(true);

        for( int i=0; i<fileNames.size(); i++)
            addItem(fileNames[i], i+1);
    }

    Justification justify(Justification::centred);

    if(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::align)=="left")
        justify = Justification::left;
    else if(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::align)=="centre")
        justify = Justification::centred;
    else
        justify = Justification::right;

    setJustificationType (justify);
}
//update controls
void CabbageComboBox::valueTreePropertyChanged (ValueTree& valueTree, const Identifier& prop)
{
    if(prop==CabbageIdentifierIds::value)
    {
        //keeps getting 0 on startup...
        int value = CabbageWidgetData::getNumProp(valueTree, CabbageIdentifierIds::value);
        setSelectedItemIndex(value-1, sendNotification);
    }

    else
    {
        handleCommonUpdates(this, valueTree);
        setColour(ComboBox::backgroundColourId, Colour::fromString(CabbageWidgetData::getStringProp(valueTree, CabbageIdentifierIds::colour)));
        setColour(ComboBox::textColourId, Colour::fromString(CabbageWidgetData::getStringProp(valueTree, CabbageIdentifierIds::fontcolour)));
        setColour(PopupMenu::backgroundColourId, Colour::fromString(CabbageWidgetData::getStringProp(valueTree, CabbageIdentifierIds::menucolour)));

        setTooltip(getCurrentString(valueTree, "popuptext"));

        if(refresh != CabbageWidgetData::getNumProp(valueTree, CabbageIdentifierIds::refreshfiles))
        {
//			refresh = CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::refreshfiles);
//			owner->refreshDiskReadingGUIControls("combobox");
        }
    }
    repaint();
}
