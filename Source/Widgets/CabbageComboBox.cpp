/*
  ==============================================================================

    CabbageComboBox.cpp
    Created: 1 Nov 2016 12:06:26pm
    Author:  rory

  ==============================================================================
*/

#include "CabbageComboBox.h"

//================================================================================================================
// combobox widget
//================================================================================================================
CabbageComboBox::CabbageComboBox(ValueTree &wData, CabbagePluginEditor* _owner):
    name( CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::name)),
    caption(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::caption)),
    colour(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::colour)),
    fontcolour(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::fontcolour)),
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
    setName(name);
    offX=offY=offWidth=offHeight=0;
    StringArray fileNames;

	setColour(ComboBox::backgroundColourId, Colour::fromString(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::colour)));
	setColour(ComboBox::textColourId, Colour::fromString(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::fontcolour)));
	CabbageUtilities::debug(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::menucolour));	

    lookAndFeelChanged();
    getProperties().set("svgpath", CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::svgpath));

    if(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::popuptext).isNotEmpty())
    {
        tooltipText = CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::popuptext);
        setTooltip(tooltipText);
    }

	setEditableText (false);

    Justification justify(Justification::centred);

    if(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::align)=="left")
        justify = Justification::left;
    else if(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::align)=="centre")
        justify = Justification::centred;
    else
        justify = Justification::right;

    setJustificationType (justify);
    setTextWhenNothingSelected(text);
    setWantsKeyboardFocus(false);
    setAlpha(CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::alpha));
    Array<File> dirFiles;


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
		StringArray items = CabbageWidgetData::getStringArrayProp(wData, CabbageIdentifierIds::text);		
		
        for(int i=0; i<items.size(); i++)
        {
            const String item  = items[i];
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


	CabbageUtilities::debug(CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::value)-1);
    setSelectedItemIndex(CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::value)-1, isPresetCombo ? sendNotification : dontSendNotification);	
	setBounds(CabbageWidgetData::getBounds(wData));

}
//---------------------------------------------
CabbageComboBox::~CabbageComboBox()
{

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
		if(refresh != CabbageWidgetData::getNumProp(valueTree, CabbageIdentifierIds::refreshfiles))
		{
//			refresh = CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::refreshfiles);
//			owner->refreshDiskReadingGUIControls("combobox");
		}
	}
    repaint();
}
