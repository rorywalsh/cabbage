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

#include "CabbageComboBox.h"
#include "../Audio/Plugins/CabbagePluginEditor.h"

//================================================================================================================
// combobox widget
//================================================================================================================
CabbageComboBox::CabbageComboBox (ValueTree wData, CabbagePluginEditor* _owner)
    : pivotx (CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::pivotx)),
    pivoty (CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::pivoty)),
    refresh (0),
    name (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::name)),
    tooltipText (String()),
    workingDir (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::workingdir)),
    rotate (CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::rotate)),
    owner (_owner),
    widgetData (wData),
    lookAndFeel()
{
    widgetData.addListener (this);
    setColour (ComboBox::backgroundColourId, Colour::fromString (CabbageWidgetData::getStringProp (widgetData, CabbageIdentifierIds::colour)));
    setColour (ComboBox::textColourId, Colour::fromString (CabbageWidgetData::getStringProp (widgetData, CabbageIdentifierIds::fontcolour)));
    setTooltip (tooltipText = CabbageWidgetData::getStringProp (widgetData, CabbageIdentifierIds::popuptext));

    setColour (PopupMenu::ColourIds::backgroundColourId, Colour::fromString (CabbageWidgetData::getStringProp (widgetData, CabbageIdentifierIds::fontcolour)).brighter (.8f));
    setColour (PopupMenu::ColourIds::highlightedBackgroundColourId, Colour::fromString (CabbageWidgetData::getStringProp (widgetData, CabbageIdentifierIds::colour)));
    setColour (PopupMenu::ColourIds::textColourId, Colour::fromString (CabbageWidgetData::getStringProp (widgetData, CabbageIdentifierIds::colour)));
    setColour (PopupMenu::ColourIds::highlightedTextColourId, Colour::fromString (CabbageWidgetData::getStringProp (widgetData, CabbageIdentifierIds::colour)).darker());

    setEditableText (false);
    setTextWhenNothingSelected (text);
    setWantsKeyboardFocus (false);
    getProperties().set("isPresetCombo", false);
    initialiseCommonAttributes (this, widgetData);

    lookAndFeel.customFont = owner->customFont;
    setLookAndFeel(&lookAndFeel);

    if (CabbageWidgetData::getStringProp (widgetData, CabbageIdentifierIds::filetype).isNotEmpty())
        CabbageWidgetData::setProperty (widgetData, CabbageIdentifierIds::text, "");

    addItemsToCombobox (widgetData);

    if(CabbageWidgetData::getStringProp (widgetData, CabbageIdentifierIds::mode) == "resize")
        return;
        
    if (CabbageWidgetData::getProperty (widgetData, CabbageIdentifierIds::channeltype) == "string" &&
		!CabbageWidgetData::getStringProp(widgetData, CabbageIdentifierIds::filetype).contains("snaps"))
    {
        isStringCombo = true;

        if (CabbageWidgetData::getStringProp (widgetData, CabbageIdentifierIds::filetype).isNotEmpty())
            CabbageWidgetData::setProperty (widgetData, CabbageIdentifierIds::text, "");

        currentValueAsText = CabbageWidgetData::getProperty (widgetData, CabbageIdentifierIds::value).toString();
        workingDir = CabbageWidgetData::getStringProp (widgetData, CabbageIdentifierIds::workingdir);
        int index = 0;
        if (workingDir.isNotEmpty())
            pluginDir = File::getCurrentWorkingDirectory().getChildFile (workingDir);
        else
            pluginDir = File::getCurrentWorkingDirectory();
        
        if(pluginDir.getChildFile(currentValueAsText).existsAsFile())
        {
            currentValueAsText = pluginDir.getChildFile(currentValueAsText).getFileNameWithoutExtension();
            index = stringItems.indexOf (currentValueAsText);
        }
        else
        {
            index = stringItems.indexOf (currentValueAsText);
        }
        //this index if different for strings and files?
        if (index >= 0)
            setSelectedItemIndex (index, dontSendNotification);
        
        owner->sendChannelStringDataToCsound(getChannel(), folderFiles[index].getFullPathName().getCharPointer());
        
//        currentValueAsText = CabbageWidgetData::getProperty (widgetData, CabbageIdentifierIds::value).toString();
//        const int index = stringItems.indexOf (currentValueAsText);
//        owner->sendChannelStringDataToCsound(getChannel(), folderFiles[index].getFullPathName().getCharPointer());
//
//        if (index != -1)
//            setSelectedItemIndex (index, dontSendNotification);
    }
    else
    {

        if (CabbageWidgetData::getStringProp (widgetData, CabbageIdentifierIds::filetype).contains ("snaps"))
        {
            isPresetCombo = true;  
            getProperties().set("isPresetCombo", true);
            String presetName = CabbageWidgetData::getProperty(widgetData, CabbageIdentifierIds::value).toString();
            
            const int index = presets.indexOf(presetName);

            //owner->currentPresetName =
            //don't send notification here, otherwise the saved session settings will be overwriten by the presets...
            owner->currentPresetName = getItemText((index-1 >= 0 ? index : 0));
            setSelectedItemIndex ((index-1 >= 0 ? index : 0), dontSendNotification);
            //onChange = [this] 
            //{
            //    if (getSelectedItemIndex() >= 0)
            //    {
            //        setTextWhenNothingSelected(presets[getSelectedItemIndex()]);
            //        DBG(presets[getSelectedItemIndex()]);
            //        setSelectedId(0);
            //    }
            //};

        }
        else
        {
            owner->sendChannelDataToCsound (getChannel(), getValue());
            setSelectedItemIndex (getValue() - 1, dontSendNotification);
        }
    }


}
//---------------------------------------------
CabbageComboBox::~CabbageComboBox()
{
    widgetData.removeListener(this);
    setLookAndFeel(nullptr);
}

void CabbageComboBox::addItemsToCombobox (ValueTree wData)
{
    Justification justify (Justification::centred);
    
    if (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::align) == "left")
        justify = Justification::left;
    else if (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::align) == "centre")
        justify = Justification::centred;
    else
        justify = Justification::right;
    
    setJustificationType (justify);
    
    
    if(CabbageWidgetData::getStringProp (widgetData, CabbageIdentifierIds::mode) == "resize")
    {
        clear();
        StringArray sizeItems = {"50%", "75%", "100%", "125%", "150%", "175%", "200%"};
        for (int i = 0; i < sizeItems.size(); ++i)
        {
            addItem (sizeItems[i], i + 1);
        }

        int currentSize = -1;
        if(owner->getPluginEditorScale() == -1)
            currentSize = CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::value);
        else
            currentSize = owner->getPluginEditorScale();
        
        setSelectedId(currentSize, sendNotification);
        //owner->resizePlugin(currentSize);
        return;
    }
    
    Array<File> dirFiles;
    presets.clear();
    folderFiles.clear();

    //load items from text file
    if (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::file).isNotEmpty())
    {
        clear (dontSendNotification);
            stringItems.clear();
        auto tempFile = File(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::csdfile)).getParentDirectory().getChildFile(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::file));
        String comboFile = tempFile.loadFileAsString();
        StringArray lines = StringArray::fromLines (comboFile);
        for (int i = 0; i < lines.size(); ++i)
        {
            if (lines[i].isNotEmpty()) 
            {
                addItem(lines[i], i + 1);
                stringItems.add(lines[i]);
            }                
        }
    }

    //load items from text() list
    else if (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::filetype).isEmpty())
    {
        var items = CabbageWidgetData::getProperty (wData, CabbageIdentifierIds::text);
        PopupMenu subMenu;
            stringItems.clear();
        clear (dontSendNotification);
        vector<StringArray> menus;
        int menuIndex = -1;
        for( int i = 0 ; i < items.size(); i++)
        {
            if(items[i].toString().contains("subM:"))
            {
                menus.push_back(StringArray());
                menuIndex++;
            }
            
            if(menus.size() > 0)
                menus[menuIndex].add(items[i]);
        }
        
        
        
        //if working with submenus....
        menuIndex = 1;
        if(menus.size()>0)
        {
            for( int i = 0 ; i < menus.size() ; i++)
            {
                subMenu.clear();
                for( int x = 1 ; x < menus[i].size() ; x++)
                {
                    subMenu.addItem(menuIndex, menus[i][x]);
                    menuIndex++;
                    if(x == menus[i].size() - 1){
                        const String subMenuName = menus[i][0].substring(6);
                        getRootMenu()->addSubMenu(subMenuName, subMenu);
                    }
                }
            }
        
        }
        else{
            for (int i = 0; i < items.size(); i++)
            {
                const String item  = items[i].toString();
                addItem (item, i + 1);
                stringItems.add (item);
            }
        }
    }
    //if dealing with preset files...
    else if ( CabbageWidgetData::getStringProp (wData, "fileType") == "*.snaps"
             || CabbageWidgetData::getStringProp (wData, "fileType") == ".snaps"
             || CabbageWidgetData::getStringProp (wData, "fileType") == "snaps") //load items from directory
    {
        const File fileName = File (getCsdFile()).withFileExtension (".snaps");
        clear (dontSendNotification);
        stringItems.clear();
        if (fileName.existsAsFile())
        {
            std::unique_ptr<XmlElement> xmlElement = XmlDocument::parse (fileName);

            int itemIndex = 1;

            if (xmlElement)
                if (xmlElement->hasTagName ("CABBAGE_PRESETS"))
                {
                    forEachXmlChildElement (*xmlElement, e)
                    {
                        if(e->getStringAttribute ("PresetName").isNotEmpty())
                        {
                            presets.add (e->getStringAttribute ("PresetName"));
                            addItem (e->getStringAttribute ("PresetName"), itemIndex++);
                        }
                    }
                }

            xmlElement = nullptr;
        }
    }
    else
    {
        
        workingDir = CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::workingdir);
       
        
        if (workingDir.isNotEmpty())
            pluginDir = File::getCurrentWorkingDirectory().getChildFile (workingDir);
        else
            pluginDir = File::getCurrentWorkingDirectory();

        filetype = CabbageWidgetData::getStringProp (wData, "filetype");
        pluginDir.findChildFiles (dirFiles, 2, false, filetype);
        //addItem ("Select..", 1);
        StringArray tempStrings;
        for (int i = 0; i < dirFiles.size(); ++i){
            folderFiles.add (dirFiles[i]);
            tempStrings.add(dirFiles[i].getFileNameWithoutExtension());
        }

        folderFiles.sort();
        
        if(stringItems == tempStrings)
            return;
        else
        {
            clear (dontSendNotification);
            stringItems.clear();
        }
        
        for ( int i = 0; i < folderFiles.size(); i++)
        {
			stringItems.add(folderFiles[i].getFileNameWithoutExtension());
            addItem (folderFiles[i].getFileNameWithoutExtension(), i + 1);
        }


        //setSelectedItemIndex(getNumItems()-1, dontSendNotification);

    }

}

void CabbageComboBox::comboBoxChanged (ComboBox* combo) //this listener is only enabled when combo is loading presets or strings...
{
    
    if(CabbageWidgetData::getStringProp (widgetData, CabbageIdentifierIds::mode) == "resize")
        return;
    
    //check here to see if string has changed, if so update XML :|
    if (CabbageWidgetData::getStringProp (widgetData, CabbageIdentifierIds::filetype).contains ("snaps")
        || CabbageWidgetData::getStringProp (widgetData, CabbageIdentifierIds::filetype) == ("preset"))
    {
        String presetFilename;
        if (owner->isAudioUnit())
            presetFilename = File(getCsdFile()).withFileExtension(".snaps").getFullPathName();
        else
            presetFilename = owner->createNewGenericNameForPresetFile();
        
        owner->restorePluginStateFrom (presets[combo->getSelectedItemIndex()], presetFilename);
        owner->sendChannelStringDataToCsound (getChannel(), presets[combo->getSelectedItemIndex()]);
        CabbageWidgetData::setProperty (widgetData, CabbageIdentifierIds::value, presets[combo->getSelectedItemIndex()]);

        auto menu = getRootMenu();
        auto id = getSelectedId();

        juce::PopupMenu::MenuItemIterator iterator(*menu);

        while (iterator.next())
        {
            auto item = &iterator.getItem();

            if (item->itemID == id)
            {
                item->setAction([this, combo, presetFilename] {
                    owner->restorePluginStateFrom(presets[combo->getSelectedItemIndex()], presetFilename);
                    owner->sendChannelStringDataToCsound(getChannel(), presets[combo->getSelectedItemIndex()]);
                    CabbageWidgetData::setProperty(widgetData, CabbageIdentifierIds::value, presets[combo->getSelectedItemIndex()]); 
                });
            }
            else
            {
                item->setAction(nullptr);
            }
        }

    }
    else if (CabbageWidgetData::getStringProp (widgetData, CabbageIdentifierIds::channeltype).contains ("string"))
    {
        const String fileType = CabbageWidgetData::getStringProp (widgetData, CabbageIdentifierIds::filetype);
        const int index = combo->getSelectedItemIndex();

		if (fileType.isNotEmpty())
		{
			String test = folderFiles[index].getFullPathName();
			owner->sendChannelStringDataToCsound(getChannel(), folderFiles[index].getFullPathName().replaceCharacters("\\", "/"));
            CabbageWidgetData::setProperty (widgetData, CabbageIdentifierIds::value, folderFiles[index].getFullPathName().replaceCharacters("\\", "/"));
		}
        else
        {
            owner->sendChannelStringDataToCsound (getChannel(), stringItems[index]);
            CabbageWidgetData::setProperty (widgetData, CabbageIdentifierIds::value, stringItems[index]);
        }
        
    }
}

void CabbageComboBox::valueTreePropertyChanged (ValueTree& valueTree, const Identifier& prop)
{
    if (prop == CabbageIdentifierIds::value)
    {
        if (isPresetCombo == false)
        {
            if (isStringCombo == false)
            {
                const int comboValue = CabbageWidgetData::getNumProp (valueTree, CabbageIdentifierIds::value);

                if (CabbageWidgetData::getNumProp (valueTree, CabbageIdentifierIds::update) == 1)
                    setSelectedItemIndex (comboValue - 1, sendNotification);
                else
                    setSelectedItemIndex (comboValue - 1, dontSendNotification);
            }
            else
            {
                currentValueAsText = CabbageWidgetData::getProperty (valueTree, CabbageIdentifierIds::value).toString();
                workingDir = CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::workingdir);
                int index = 0;
                if (workingDir.isNotEmpty())
                    pluginDir = File::getCurrentWorkingDirectory().getChildFile (workingDir);
                else
                    pluginDir = File::getCurrentWorkingDirectory();
                
                if(pluginDir.getChildFile(currentValueAsText).existsAsFile())
                {
                    currentValueAsText = File(currentValueAsText).getFileNameWithoutExtension();
                    index = stringItems.indexOf (currentValueAsText)+1;
                }
                else
                    index = stringItems.indexOf (currentValueAsText);
                //this index if different for strings and files?
                if (index >= 0)
                    setSelectedItemIndex (index, dontSendNotification);
    
                
                CabbageWidgetData::setProperty (valueTree, CabbageIdentifierIds::value, currentValueAsText);
            }
        }
		else
		{
			currentValueAsText = CabbageWidgetData::getProperty(valueTree, CabbageIdentifierIds::value).toString();
			const int index = stringItems.indexOf(currentValueAsText);

			if (index != -1)
				setSelectedItemIndex(index, dontSendNotification);
			//CabbageWidgetData::setProperty(valueTree, CabbageIdentifierIds::value, currentValueAsText);
		}

    }

    else
    {
        handleCommonUpdates (this, valueTree, false, prop);
        setColour (ComboBox::backgroundColourId, Colour::fromString (CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::colour)));
        setColour (ComboBox::textColourId, Colour::fromString (CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::fontcolour)));
        setColour (PopupMenu::backgroundColourId, Colour::fromString (CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::menucolour)));

        setTooltip (getCurrentPopupText (valueTree));

        if (workingDir != CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::workingdir) || prop == CabbageIdentifierIds::populate)
        {
            addItemsToCombobox (valueTree);
            workingDir = CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::workingdir);
        }

        if(prop == CabbageIdentifierIds::text && isStringCombo == true)
        {
                addItemsToCombobox(valueTree);
        }

        if (CabbageWidgetData::getNumProp (valueTree, CabbageIdentifierIds::refreshfiles)==1)
        {
            CabbageWidgetData::setNumProp(valueTree, CabbageIdentifierIds::refreshfiles, 0);
            addItemsToCombobox (valueTree);
        }
    }

    repaint();
}
