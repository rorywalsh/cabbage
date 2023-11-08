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
    workingDir (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::currentdir)),
    rotate (CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::rotate)),
    owner (_owner),
    widgetData (wData),
    lookAndFeel(),
    isStringCombo(CabbageWidgetData::getProperty (widgetData, CabbageIdentifierIds::channeltype) == "string"    ),
    CabbageWidgetBase(_owner)
{
    
    widgetData.addListener (this);
    setLookAndFeel(&lookAndFeel);

    setColour (ComboBox::backgroundColourId, Colour::fromString (CabbageWidgetData::getStringProp (widgetData, CabbageIdentifierIds::colour)));
    
    setColour (ComboBox::textColourId, Colour::fromString (CabbageWidgetData::getStringProp (widgetData, CabbageIdentifierIds::fontcolour)));
    setColour (ComboBox::outlineColourId, Colour::fromString (CabbageWidgetData::getStringProp (widgetData, CabbageIdentifierIds::outlinecolour)));
    
    setTooltip (tooltipText = CabbageWidgetData::getStringProp (widgetData, CabbageIdentifierIds::popuptext));
    setColour (PopupMenu::ColourIds::backgroundColourId, Colour::fromString( CabbageWidgetData::getStringProp (widgetData, CabbageIdentifierIds::menucolour)));
//    setColour (PopupMenu::ColourIds::highlightedBackgroundColourId, Colour::fromString (CabbageWidgetData::getStringProp (widgetData, CabbageIdentifierIds::colour)));
    setColour (PopupMenu::ColourIds::highlightedTextColourId, Colour::fromString( CabbageWidgetData::getStringProp (widgetData, CabbageIdentifierIds::menucolour)).withAlpha(0.5f));
    setColour (PopupMenu::ColourIds::textColourId, Colour::fromString (CabbageWidgetData::getStringProp (widgetData, CabbageIdentifierIds::fontcolour)));
    
    lookAndFeelChanged();
    setEditableText (false);
    setTextWhenNothingSelected (text);
    setWantsKeyboardFocus (false);
    getProperties().set("isPresetCombo", false);
    initialiseCommonAttributes (this, widgetData);

    if (CabbageWidgetData::getStringProp (widgetData, CabbageIdentifierIds::filetype).isNotEmpty())
        CabbageWidgetData::setProperty (widgetData, CabbageIdentifierIds::text, "");

    addItemsToCombobox (widgetData);

    if(CabbageWidgetData::getStringProp (widgetData, CabbageIdentifierIds::mode) == "resize")
        return;
        
    if (CabbageWidgetData::getProperty (widgetData, CabbageIdentifierIds::channeltype) == "string" &&
		!CabbageWidgetData::getStringProp(widgetData, CabbageIdentifierIds::filetype).contains("snaps"))
    {


        if (CabbageWidgetData::getStringProp (widgetData, CabbageIdentifierIds::filetype).isNotEmpty())
            CabbageWidgetData::setProperty (widgetData, CabbageIdentifierIds::text, "");

        currentValueAsText = CabbageWidgetData::getProperty (widgetData, CabbageIdentifierIds::value).toString();
        workingDir = CabbageWidgetData::getStringProp (widgetData, CabbageIdentifierIds::currentdir);
        workingDir = CabbageUtilities::expandDirectoryMacro(workingDir);
        
        int index = 0;
        if (workingDir.isNotEmpty())
            pluginDir = File(getCsdFile()).getParentDirectory().getChildFile (workingDir);
        else
            pluginDir = File(getCsdFile()).getParentDirectory();
        
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
        else
            setSelectedItemIndex (0, dontSendNotification);
        

        if (currentValueAsText.containsOnly("0123456789.-"))
        {
            index = currentValueAsText.getIntValue();
        }

        if(CabbageWidgetData::getStringProp (widgetData, CabbageIdentifierIds::filetype).isNotEmpty())
            owner->sendChannelStringDataToCsound(getChannel(), folderFiles[index].getFileNameWithoutExtension());
        else
            owner->sendChannelStringDataToCsound(getChannel(), stringItems[index]);
    }
    else
    {
        const String cType = CabbageWidgetData::getProperty (widgetData, CabbageIdentifierIds::channeltype);
        if (CabbageWidgetData::getStringProp (widgetData, CabbageIdentifierIds::filetype).contains ("snaps"))
        {
            isPresetCombo = true;
            getProperties().set("isPresetCombo", true);
            String presetName = CabbageWidgetData::getProperty(widgetData, CabbageIdentifierIds::value).toString();
            
            if(owner->getCurrentPreset().isNotEmpty())
            {
                presetName = owner->getCurrentPreset();
            }
            
            const int index = presets.indexOf(presetName);
            if(index == -1 && presets.size()>0)
                presetName = presets[0];
            
            owner->setCurrentPreset(presetName);
            owner->sendChannelStringDataToCsound (getChannel(), presetName);
            setSelectedItemIndex ((index-1 >= 0 ? index : 0), dontSendNotification);
        }
        else
        {
            owner->sendChannelDataToCsound (getChannel(), getValue());
            setSelectedItemIndex (getValue() - 1, dontSendNotification);
        }
    }

    lookAndFeel.customFont = owner->customFont;
    
}
//---------------------------------------------
CabbageComboBox::~CabbageComboBox()
{
    setLookAndFeel(nullptr);
    widgetData.removeListener(this);
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
            currentSize = CabbageWidgetData::getProperty (wData, CabbageIdentifierIds::value);
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
        std::vector<StringArray> menus;
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
            for( int i = 0 ; i < int(menus.size()) ; i++)
            {
                subMenu.clear();
                for( int x = 1 ; x < int(menus[i].size()) ; x++)
                {
                    subMenu.addItem(menuIndex, menus[i][x]);
                    menuIndex++;
                    if(x == int(menus[i].size() - 1)){
                        const String subMenuName = menus[i][0].substring(5);
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
    else if ( CabbageWidgetData::getStringProp (wData, "fileType").contains(".snaps")) //load items from directory
    {
        const String presetFileName = CabbageWidgetData::getStringProp (wData, "fileType");
        File fileName;
        
        if(presetFileName!="*.snaps" && presetFileName!=".snaps")
            fileName = File (getCsdFile()).getParentDirectory().getChildFile(presetFileName);
        else
            fileName = File (getCsdFile()).withFileExtension (".snaps");

        if (!fileName.existsAsFile())
        {
#ifdef JUCE_MAC
            String path = CabbageUtilities::getRealUserHomeDirectory().getFullPathName() + "/Library/" + String(CabbageManufacturer) + "/" + File(getCsdFile()).getParentDirectory().getParentDirectory().getFileNameWithoutExtension() + "/" + fileName.getFileName();
            
            if(File(path).existsAsFile() == false && PluginHostType::getPluginLoadedAs() == AudioProcessor::wrapperType_AudioUnit)
            {
                const String pluginBundleName = File::getSpecialLocation(File::currentExecutableFile).getParentDirectory().getParentDirectory().getParentDirectory().getFileNameWithoutExtension();
            
                if(!File(path).existsAsFile())
                {
                    path = CabbageUtilities::getRealUserHomeDirectory().getFullPathName() + "/Library/" + String(CabbageManufacturer) + "/" + pluginBundleName + "/"+pluginBundleName+".snaps";
                }
            }
#else
            String path = File::getSpecialLocation(File::userApplicationDataDirectory).getFullPathName() + "/" + String(CabbageManufacturer) + "/" + File(getCsdFile()).getFileNameWithoutExtension() + "/" + fileName.getFileName();
#endif
            if(File(path).existsAsFile())
                fileName = File(path);
        }

        presetFile = fileName;
        
        clear (dontSendNotification);
        stringItems.clear();
        var presetNames;
        if (fileName.existsAsFile() && fileName.loadFileAsString().isNotEmpty())
        {
            if(json::accept(fileName.loadFileAsString().toStdString()) == false)
               return;
               
            auto j = nlohmann::ordered_json::parse(fileName.loadFileAsString().toRawUTF8());

            for (nlohmann::ordered_json::iterator it = j.begin(); it != j.end(); ++it) {
                presets.add (it.key());
            }
            
            int itemIndex = 1;
            if(CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::sort) == 1)
                presets.sort(true);
            
            for(auto item : presets){
                addItem (item, itemIndex++);
                presetNames.append(item);
            }
            if(presets.size() == 0)
                owner->setCurrentPreset("");
            
            CabbageWidgetData::setProperty(wData, CabbageIdentifierIds::text, presetNames);
        }
    }
    else
    {
        
        workingDir = CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::currentdir);
        workingDir = CabbageUtilities::expandDirectoryMacro(workingDir);
        
        if (workingDir.isNotEmpty())
            pluginDir = File(getCsdFile()).getParentDirectory().getChildFile(workingDir);
        else
            pluginDir = File(getCsdFile()).getParentDirectory();

        filetype = CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::filetype);
        pluginDir.findChildFiles (dirFiles, File::TypesOfFileToFind::findFilesAndDirectories, false, filetype);
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

        if(currentValueAsText.isNotEmpty())
            setText(File(getCsdFile()).getParentDirectory().getChildFile(currentValueAsText).getFileNameWithoutExtension());

        var items;
        for(auto& s : folderFiles)
            items.append(s.getFileNameWithoutExtension());
        
        CabbageWidgetData::setProperty(wData, CabbageIdentifierIds::text, items, this);
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
        const String channelName = getChannel();
        owner->sendChannelStringDataToCsound (getChannel(), presets[combo->getSelectedItemIndex()]);
        owner->setCurrentPreset(presets[combo->getSelectedItemIndex()]);
        CabbageWidgetData::setProperty (widgetData, CabbageIdentifierIds::value, presets[combo->getSelectedItemIndex()]);
        owner->restorePluginStateFrom (presets[combo->getSelectedItemIndex()], presetFile.getFullPathName());
        
        

        auto menu = getRootMenu();
        auto id = getSelectedId();

        juce::PopupMenu::MenuItemIterator iterator(*menu);

        while (iterator.next())
        {
            auto item = &iterator.getItem();

            if (item->itemID == id)
            {
                item->setAction([this, combo] {
                    owner->restorePluginStateFrom(presets[combo->getSelectedItemIndex()], presetFile.getFullPathName());
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
			owner->sendChannelStringDataToCsound(getChannel(), folderFiles[index].getFileNameWithoutExtension());
            CabbageWidgetData::setProperty (widgetData, CabbageIdentifierIds::value, folderFiles[index].getFileName());
		}
        else
        {
            owner->sendChannelStringDataToCsound (getChannel(), stringItems[index]);
            if (CabbagePluginParameter* param = owner->getParameterForComponent(combo->getName()))
            {

                param->setValueNotifyingHost(float(index+.9) / (float)stringItems.size());
            }
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
                const int comboValue = CabbageWidgetData::getProperty (valueTree, CabbageIdentifierIds::value);

                if (CabbageWidgetData::getNumProp (valueTree, CabbageIdentifierIds::update) == 1)
                    setSelectedItemIndex (comboValue - 1, sendNotification);
                else
                    setSelectedItemIndex (comboValue - 1, dontSendNotification);
                
                currentItemIndex = comboValue - 1;
            }
            else
            {   
                const String s = CabbageWidgetData::getProperty(valueTree, CabbageIdentifierIds::value).toString().removeCharacters("\"");

                if (s.containsOnly("0123456789.-"))
                {
                    currentValueAsText = stringItems[juce::roundToInt(s.getIntValue()-.25)];
                }

                else
                {
                    currentValueAsText = s;
                }  
                

                currentValueAsText = File(getCsdFile()).getParentDirectory().getChildFile (currentValueAsText).getFileNameWithoutExtension();
                
             
                workingDir = CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::currentdir);
                workingDir = CabbageUtilities::expandDirectoryMacro(workingDir);
                int index = 0;
                if (workingDir.isNotEmpty())
                    pluginDir = File(getCsdFile()).getParentDirectory().getChildFile (workingDir);
                else
                    pluginDir = File(getCsdFile()).getParentDirectory();
                
                if(pluginDir.getChildFile(currentValueAsText).existsAsFile())
                {
                    currentValueAsText = pluginDir.getChildFile(currentValueAsText).getFileName();
                    index = stringItems.indexOf (currentValueAsText);
                }
                else
                    index = stringItems.indexOf (currentValueAsText);
                

                //this index if different for strings and files?
                if (index >= 0)
                    setSelectedItemIndex (index, sendNotification);
    
                //can't update the channel value from here as it might update on the same cycle as a cabbageSetValue
                //this in turn will update the string channel pointer and mess up further called to cabbageSetValue...
                owner->sendChannelStringDataToCsound (getChannel(), currentValueAsText);
                
                currentItemIndex = index;
                CabbageWidgetData::setProperty (valueTree, CabbageIdentifierIds::value, currentValueAsText);
            }
        }
		else
		{
            if (owner->getCurrentPreset().isNotEmpty())
            {
                String presetName = owner->getCurrentPreset();

                const int index = presets.indexOf(presetName);
                if (index == -1 && presets.size() > 0)
                    presetName = presets[0];

                owner->setCurrentPreset(presetName);
                owner->sendChannelStringDataToCsound(getChannel(), presetName);
                setSelectedItemIndex((index - 1 >= 0 ? index : 0), dontSendNotification);
                currentItemIndex = index;
            }
		}

    }

    else
    {
        //handleCommonUpdates (this, valueTree, false, prop);
//        setColour (ComboBox::backgroundColourId, Colour::fromString (CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::colour)));
//        setColour (ComboBox::textColourId, Colour::fromString (CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::fontcolour)));
//        setColour (PopupMenu::ColourIds::backgroundColourId, Colour::fromString( CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::menucolour)));
//        setColour (PopupMenu::ColourIds::textColourId, Colour::fromString (CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::fontcolour)));
//        
        setTooltip (getCurrentPopupText (valueTree));

        if((prop == CabbageIdentifierIds::currentdir || prop == CabbageIdentifierIds::populate || prop == CabbageIdentifierIds::update) && isPresetCombo == false)
        {
            addItemsToCombobox (valueTree);
            workingDir = CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::currentdir);
            workingDir = CabbageUtilities::expandDirectoryMacro(workingDir);
        }

        if((prop == CabbageIdentifierIds::text && isStringCombo == true) ||
           (CabbageWidgetData::getNumProp(valueTree, CabbageIdentifierIds::automatable) == 0 && (prop == CabbageIdentifierIds::text)))
        {
                addItemsToCombobox(valueTree);
        }

        if (CabbageWidgetData::getNumProp (valueTree, CabbageIdentifierIds::refreshfiles)==1)
        {
            CabbageWidgetData::setNumProp(valueTree, CabbageIdentifierIds::refreshfiles, 0);
            addItemsToCombobox (valueTree);
        }
        
        handleCommonUpdates(this, valueTree, false, prop);
    }

    repaint();
}
