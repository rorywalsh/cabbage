/*
  ==============================================================================

    CabbageListBox.cpp
    Created: 29 Aug 2018 6:21:32pm
    Author:  rory

  ==============================================================================
*/

#include "CabbageListBox.h"
#include "../Audio/Plugins/CabbagePluginEditor.h"


CabbageListBox::CabbageListBox(ValueTree wData, CabbagePluginEditor* _owner):
    owner(_owner),
    justify(Justification::centred),
    widgetData (wData)
{
    addAndMakeVisible(listBox);
    listBox.setRowHeight (20);
    listBox.setModel (this);
    getProperties().set("isPresetCombo", false);

    setName (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::name));
    widgetData.addListener (this);              //add listener to valueTree so it gets notified when a widget's property changes
    initialiseCommonAttributes (this, wData);   //initialise common attributes such as bounds, name, rotation, etc..
    //listBox.setBounds(CabbageWidgetData::getBounds(wData).withTop(0).withLeft(0));
    addItemsToListbox(wData, true);

    if (CabbageWidgetData::getProperty (wData, CabbageIdentifierIds::channeltype) == "string")
    {
        isStringCombo = true;

        if (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::filetype).isNotEmpty())
            CabbageWidgetData::setProperty (wData, CabbageIdentifierIds::text, "");

        currentValueAsText = CabbageWidgetData::getProperty (wData, CabbageIdentifierIds::value).toString();
        owner->sendChannelStringDataToCsound (getChannel(), currentValueAsText);
        const int index = stringItems.indexOf (currentValueAsText);

        if (index != -1)
            listBox.selectRow(index, dontSendNotification);
    }
    else
    {

        if (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::filetype).contains ("snaps"))
        {
            isPresetCombo = true;
            getProperties().set("isPresetCombo", true);
            listBox.selectRow ((getValue()-1 >= 0 ? getValue() - 1 : 0), dontSendNotification);
        }
        else
        {
            owner->sendChannelDataToCsound (getChannel(), getValue());
            listBox.selectRow (getValue() - 1, dontSendNotification);
        }
    }

}

void CabbageListBox::resized()
{
    listBox.setBounds(getLocalBounds());
}

void CabbageListBox::paint(Graphics& g)
{
    g.fillAll(Colour::fromString(CabbageWidgetData::getStringProp (widgetData, CabbageIdentifierIds::colour)));
}

void CabbageListBox::addItemsToListbox (ValueTree wData, bool refreshedFromDisk)
{
    Array<File> dirFiles;
    StringArray fileNames;
    stringItems.clear();
    folderFiles.clear();
    presets.clear();

    //load items from text file
    if (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::file).isNotEmpty())
    {
        String file = File (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::file)).loadFileAsString();
        StringArray lines = StringArray::fromLines (file);

        for (int i = 0; i < lines.size(); ++i)
        {
            stringItems.add (lines[i]);
        }
    }

        //load items from text() list
    else if (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::filetype).isEmpty())
    {
        var items = CabbageWidgetData::getProperty (wData, CabbageIdentifierIds::text);

        for (int i = 0; i < items.size(); i++)
        {
            const String item  = items[i].toString();
            stringItems.add (item);
        }
    }
        //if dealing with preset files...
    else if (CabbageWidgetData::getStringProp (wData, "filetype") == "preset"
             || CabbageWidgetData::getStringProp (wData, "filetype") == "*.snaps"
             || CabbageWidgetData::getStringProp (wData, "filetype") == ".snaps"
             || CabbageWidgetData::getStringProp (wData, "filetype") == "snaps") //load items from directory
    {
        const File fileName = File (getCsdFile()).withFileExtension (".snaps");

        if (fileName.existsAsFile())
        {
            ScopedPointer<XmlElement> xmlElement = XmlDocument::parse (fileName);

            if (xmlElement)
                if (xmlElement->hasTagName ("CABBAGE_PRESETS"))
                {
                    forEachXmlChildElement (*xmlElement, e)
                    {
                        presets.add (e->getStringAttribute ("PresetName"));
                        stringItems.add (e->getStringAttribute ("PresetName"));
                    }
                }

            xmlElement = nullptr;
        }
    }
    else
    {
        const String workingDir = CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::workingdir);

        if (workingDir.isNotEmpty())
            pluginDir = File::getCurrentWorkingDirectory().getChildFile (workingDir);
        else
            pluginDir = File::getCurrentWorkingDirectory().getParentDirectory();

        filetype = CabbageWidgetData::getStringProp (wData, "filetype");
        pluginDir.findChildFiles (dirFiles, 2, false, filetype);
        stringItems.add ("Select..");

        for (int i = 0; i < dirFiles.size(); ++i)
            folderFiles.add (dirFiles[i]);

        folderFiles.sort();

        for ( int i = 0; i < folderFiles.size(); i++)
        {
            stringItems.add (folderFiles[i].getFileNameWithoutExtension());
        }

    }

    Justification justify (Justification::centred);

    if (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::align) == "left")
        justify = Justification::left;
    else if (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::align) == "centre")
        justify = Justification::centred;
    else
        justify = Justification::right;

    listBox.updateContent();
}

void CabbageListBox::valueTreePropertyChanged (ValueTree& valueTree, const Identifier& prop)
{
    if (prop == CabbageIdentifierIds::value)
    {
        //set value. This is only needed for widgets that can have their value changed dioverlayRectly using a chnset
    }
    else
    {



        handleCommonUpdates (this, valueTree);      //handle common updates such as bounds, alpha, rotation, visible, etc
    }
}

int CabbageListBox::getNumRows()
{
    return stringItems.size();
}

void CabbageListBox::listBoxItemDoubleClicked(int row, const MouseEvent &e)
{
    if (CabbageWidgetData::getStringProp (widgetData, CabbageIdentifierIds::filetype).contains ("snaps")
        || CabbageWidgetData::getStringProp (widgetData, CabbageIdentifierIds::filetype).contains ("preset"))
    {
        owner->restorePluginStateFrom (presets[row]);
        owner->sendChannelDataToCsound (getChannel(), row);
    }
    else if (CabbageWidgetData::getStringProp (widgetData, CabbageIdentifierIds::channeltype).contains ("string"))
    {
        const String fileType = CabbageWidgetData::getStringProp (widgetData, CabbageIdentifierIds::filetype);
        const int index = row;

        if (fileType.isNotEmpty())
            owner->sendChannelStringDataToCsound (getChannel(), folderFiles[index - 1].getFullPathName());
        else
            owner->sendChannelStringDataToCsound (getChannel(), stringItems[index]);

    }
    else
    {
        owner->sendChannelDataToCsound(getChannel(), row);
    }
}


void CabbageListBox::paintListBoxItem (int rowNumber, Graphics& g,
                                       int width, int height, bool rowIsSelected)
{
    if (rowIsSelected)
        g.fillAll (Colour::fromString(CabbageWidgetData::getStringProp (widgetData, CabbageIdentifierIds::highlightcolour)));
    else
        g.fillAll(Colour::fromString(CabbageWidgetData::getStringProp (widgetData, CabbageIdentifierIds::colour)));

    g.setColour(Colour::fromString(CabbageWidgetData::getStringProp (widgetData, CabbageIdentifierIds::fontcolour)));
    g.drawFittedText(stringItems[rowNumber], Rectangle<int> (width, height), justify, 0);
}

void CabbageListBox::selectedRowsChanged (int /*lastRowselected*/)
{

}