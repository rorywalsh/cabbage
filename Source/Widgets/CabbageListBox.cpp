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
    widgetData (wData),
    justify(Justification::centred),
    owner(_owner)
{
    addAndMakeVisible(listBox);
    listBox.setRowHeight (20);
    listBox.setModel (this);
    getProperties().set("isPresetCombo", false);
    highlightColour = CabbageWidgetData::getStringProp (widgetData, CabbageIdentifierIds::highlightcolour);
    colour = CabbageWidgetData::getStringProp (widgetData, CabbageIdentifierIds::colour);
    fontColour = CabbageWidgetData::getStringProp (widgetData, CabbageIdentifierIds::fontcolour);
    setName (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::name));
    widgetData.addListener (this);              //add listener to valueTree so it gets notified when a widget's property changes
    initialiseCommonAttributes (this, wData);   //initialise common attributes such as bounds, name, rotation, etc..
    //listBox.setBounds(CabbageWidgetData::getBounds(wData).withTop(0).withLeft(0));
    addItemsToListbox(wData);

    
    //lookAndFeel.customFont = owner->customFont;
    setLookAndFeel(&lookAndFeel);
    
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

	const Colour backgroundColour = Colour::fromString(CabbageWidgetData::getStringProp(widgetData, CabbageIdentifierIds::colour));
	
    listBox.getVerticalScrollBar().getLookAndFeel().setColour(ScrollBar::backgroundColourId, Colours::red);
	listBox.setColour(ListBox::backgroundColourId, backgroundColour);
	listBox.lookAndFeelChanged();
	resized();

}

void CabbageListBox::resized()
{
    listBox.setBounds(getLocalBounds());
}

void CabbageListBox::addItemsToListbox (ValueTree wData)
{
    Array<File> dirFiles;
    stringItems.clear();
    folderFiles.clear();
    presets.clear();

    //load items from text file
    if (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::file).isNotEmpty())
    {
        String listBoxFile = File (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::file)).loadFileAsString();
        StringArray lines = StringArray::fromLines (listBoxFile);

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
    else if (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::filetype) == "preset"
             || CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::filetype) == "*.snaps"
             || CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::filetype) == ".snaps"
             || CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::filetype) == "snaps") //load items from directory
    {
//        const File fileName = File (getCsdFile()).withFileExtension (".snaps");
//
//        if (fileName.existsAsFile())
//        {
//            std::unique_ptr<XmlElement> xmlElement = XmlDocument::parse (fileName);
//
//            if (xmlElement)
//                if (xmlElement->hasTagName ("CABBAGE_PRESETS"))
//                {
//                    forEachXmlChildElement (*xmlElement, e)
//                    {
//                        presets.add (e->getStringAttribute ("PresetName"));
//                        stringItems.add (e->getStringAttribute ("PresetName"));
//                    }
//                }
//
//            xmlElement = nullptr;
//        }
    }
    else
    {
        workingDir = CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::workingdir);

		if (File::getCurrentWorkingDirectory().getChildFile(workingDir).exists())
			listboxDir = File::getCurrentWorkingDirectory().getChildFile(workingDir);
		else if(workingDir.isNotEmpty())
			listboxDir = File(getCsdFile()).getParentDirectory().getChildFile (workingDir);
        else
			listboxDir = File(getCsdFile()).getParentDirectory();

        filetype = CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::filetype);
        listboxDir.findChildFiles (dirFiles, 2, false, filetype);
        stringItems.add ("Select..");

        for (int i = 0; i < dirFiles.size(); ++i)
            folderFiles.add (dirFiles[i]);

        folderFiles.sort();

        for ( int i = 0; i < folderFiles.size(); i++)
        {
            stringItems.add (folderFiles[i].getFileNameWithoutExtension());
        }

    }


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
        if (isPresetCombo == false)
        {
            if (isStringCombo == false)
            {
                int listBoxValue = CabbageWidgetData::getNumProp (valueTree, CabbageIdentifierIds::value);

//                if (CabbageWidgetData::getNumProp (valueTree, CabbageIdentifierIds::update) == 1)
//                    listBox.selectRow(value - 1, sendNotification);
//                else
                listBox.selectRow(listBoxValue - 1);//, dontSendNotification);
            }
            else
            {
                currentValueAsText = CabbageWidgetData::getProperty (valueTree, CabbageIdentifierIds::value).toString();
                const int index = stringItems.indexOf (currentValueAsText);

                if (index != -1)
                    listBox.selectRow(index);

                CabbageWidgetData::setProperty (valueTree, CabbageIdentifierIds::value, currentValueAsText);
            }
        }

    }

    else
    {
        handleCommonUpdates (this, valueTree, false, prop);
        highlightColour = CabbageWidgetData::getStringProp (widgetData, CabbageIdentifierIds::highlightcolour);
        colour = CabbageWidgetData::getStringProp (widgetData, CabbageIdentifierIds::colour);
        fontColour = CabbageWidgetData::getStringProp (widgetData, CabbageIdentifierIds::fontcolour);
        //setTooltip (getCurrentPopupText (valueTree));

        if (workingDir != CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::workingdir))
        {
            addItemsToListbox (valueTree);
            workingDir = CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::workingdir);
        }

        if (CabbageWidgetData::getNumProp (valueTree, CabbageIdentifierIds::refreshfiles)==1)
        {
            CabbageWidgetData::setNumProp(valueTree, CabbageIdentifierIds::refreshfiles, 0);
            addItemsToListbox (valueTree);
        }
      
        listBox.repaint();
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
        const String presetFileName = CabbageWidgetData::getStringProp (widgetData, "fileType");
        File fileName;
        
        if(presetFileName.length() > 6 )
            fileName = File (getCsdFile()).getParentDirectory().getChildFile(presetFileName);
        else
            fileName = File (getCsdFile()).withFileExtension (".snaps");
        
        if (!fileName.existsAsFile())
        {
#ifdef JUCE_MAC
            String path = CabbageUtilities::getRealUserHomeDirectory().getFullPathName() + "/Library/" + String(JucePlugin_Manufacturer) + "/" + File(getCsdFile()).getParentDirectory().getParentDirectory().getFileNameWithoutExtension() + "/" + fileName.getFileName();
#else
            String path = File::getSpecialLocation(File::userApplicationDataDirectory).getFullPathName() + "/" + String(JucePlugin_Manufacturer) + "/" + File(getCsdFile()).getFileNameWithoutExtension() + "/" + fileName.getFileName();
#endif
            fileName = File(path);
            fileName = File(path);
        }
        
        owner->restorePluginStateFrom (presets[row], fileName.getFullPathName());
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
    if(owner->customFont.getHeight()<900)
        g.setFont(owner->customFont);
    
    if (rowIsSelected)
        g.fillAll (Colour::fromString(highlightColour));
    else
        g.fillAll(Colour::fromString(colour));

    g.setColour(Colour::fromString(fontColour));
    g.drawFittedText(stringItems[rowNumber], Rectangle<int> (width, height), justify, 0);
}

void CabbageListBox::selectedRowsChanged (int /*lastRowselected*/)
{

}
