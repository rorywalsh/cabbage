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

#include "CabbagePresetButton.h"
#include "../Audio/Plugins/CabbagePluginEditor.h"

CabbagePresetButton::CabbagePresetButton (ValueTree wData, CabbagePluginEditor* owner)
    : TextButton(),
    owner (owner),
    widgetData (wData),
    CabbageWidgetBase(owner)
{
    widgetData.addListener (this);              //add listener to valueTree so it gets notified when a widget's property changes
    initialiseCommonAttributes (this, wData);   //initialise common attributes such as bounds, name, rotation, etc..
    setLookAndFeelColours (wData);

    tooltipText = CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::popuptext);
    if (tooltipText.isNotEmpty())
        setTooltip(tooltipText);
    
    const String valueText = CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::value);

    if(valueText.isEmpty())
        setButtonText (getText());
    else if(File(valueText).existsAsFile())
        setButtonText (File(valueText).getFileNameWithoutExtension());
    else
        setButtonText(valueText);


    replaceTextWithPreset = CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::presetnameastext) == 1 ? true : false;
    
    filetype = CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::filetype).replaceCharacters (" ", ";");

    setImgProperties (*this, wData, "buttonon");
    setImgProperties (*this, wData, "buttonoff");
    addListener (this);

    const String imgOff = CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::imgbuttonoff);
    const String imgOver = CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::imgbuttonover);
    const String imgOn = CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::imgbuttonon);
    
    getProperties().set("outlinecolour", CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::outlinecolour));
    getProperties().set("outlinethickness", CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::outlinethickness));
    getProperties().set("corners", CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::corners));
    
    const String globalStyle = owner->globalStyle;
    
    mode = CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::mode);
    
    currentPresetDir = CabbageWidgetData::getStringProp (widgetData, CabbageIdentifierIds::currentdir);
    currentPresetDir = CabbageUtilities::expandDirectoryMacro(currentPresetDir);
    
    
    if(globalStyle == "legacy")
    {
        int fontstyle = CabbageWidgetData::getProperty (wData, CabbageIdentifierIds::fontstyle);
        owner->customFont.setStyleFlags(fontstyle);
        return;
    }
    
    //if users are passing custom images, use old style look and feel
    if (CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::style) == "flat" &&
        imgOff.isEmpty() && imgOn.isEmpty() && imgOver.isEmpty())
    {
        int fontstyle = CabbageWidgetData::getProperty (wData, CabbageIdentifierIds::fontstyle);
        owner->customFont.setStyleFlags(fontstyle);
        flatLookAndFeel.customFont = owner->customFont;
        setLookAndFeel(&flatLookAndFeel);
        lookAndFeelChanged();
        setLookAndFeel(&flatLookAndFeel);
    }
    
	var userFolder = CabbageWidgetData::getProperty(wData, CabbageIdentifierIds::userFolder);
	var factoryFolder = CabbageWidgetData::getProperty(wData, CabbageIdentifierIds::factoryFolder);
    DBG(factoryFolder.toString());

	if (userFolder.size() == 0)
	{
		user.folder = "undefined";
	}

	if (factoryFolder.size() == 0)
	{
        factory.folder = "undefined";
	}

	user.extension = "snaps";
	factory.extension = "snaps";

	if (userFolder.size() > 0)
    {
        userFolder[0] = CabbageUtilities::expandDirectoryMacro(userFolder[0]);
        user.folder = File(getCsdFile()).getParentDirectory().getChildFile(userFolder[0].toString()).getFullPathName();
    }
	if (userFolder.size() > 1)
		user.extension = userFolder[1].toString().trim();

	if (factoryFolder.size() > 0)
    {
        factoryFolder[0] = CabbageUtilities::expandDirectoryMacro(factoryFolder[0]);
        factory.folder = File(getCsdFile()).getParentDirectory().getChildFile(factoryFolder[0].toString()).getFullPathName();
    }
	if (factoryFolder.size() > 1)
		factory.extension = factoryFolder[1].toString().trim();
	
}

//===============================================================================
void CabbagePresetButton::buttonClicked (Button* button)
{

    PopupMenu m = addPresetsToMenu("");
    laf.setColour(PopupMenu::ColourIds::backgroundColourId, Colour::fromString (CabbageWidgetData::getStringProp (widgetData, CabbageIdentifierIds::backgroundcolour)));
    laf.setColour(PopupMenu::ColourIds::textColourId, Colour::fromString (CabbageWidgetData::getStringProp (widgetData, CabbageIdentifierIds::textcolour)));
    laf.setColour(PopupMenu::ColourIds::highlightedTextColourId, Colour::fromString (CabbageWidgetData::getStringProp (widgetData, CabbageIdentifierIds::highlightedtextcolour)));
    laf.setColour(PopupMenu::ColourIds::highlightedBackgroundColourId, Colour::fromString (CabbageWidgetData::getStringProp (widgetData, CabbageIdentifierIds::highlighteditemcolour)));
    
    m.setLookAndFeel(&laf);
    
    m.showMenuAsync(juce::PopupMenu::Options(), [this](int choice) {
        if(choice == 0)
            return;
        
        if(choice == 2){
            FileChooser fc ("Choose preset folder", File(this->currentPresetDir), "", CabbageUtilities::shouldUseNativeBrowser());

            if (fc.browseForDirectory())
            {
                this->currentPresetDir = fc.getResult().getFullPathName();
            }
        }
        else if(choice == 1){
            FileChooser fc("Save preset", File(this->user.folder), this->user.extension, CabbageUtilities::shouldUseNativeBrowser());

            if (fc.browseForFileToSave(true))
            {
                owner->savePluginStateToFile (fc.getResult().getFileNameWithoutExtension(), fc.getResult().getFullPathName(), false);
                owner->sendChannelStringDataToCsound(this->getChannel(), fc.getResult().getFullPathName());
                CabbageWidgetData::setStringProp(widgetData, CabbageIdentifierIds::value, fc.getResult().getFullPathName());
            }
        }
        else
        {
//            DBG(File(this->fullPresetList[choice-3]).getFileNameWithoutExtension());
//            DBG(File(this->fullPresetList[choice-3]).getFullPathName());
            owner->sendChannelStringDataToCsound(this->getChannel(), File(this->fullPresetList[choice-3]).getFullPathName());
            owner->restorePluginStateFrom (File(this->fullPresetList[choice-3]).getFileNameWithoutExtension(), this->fullPresetList[choice-3]);
            CabbageWidgetData::setStringProp(widgetData, CabbageIdentifierIds::value, this->fullPresetList[choice-3]);
            if(replaceTextWithPreset)
            {
                setButtonText(File(this->fullPresetList[choice-3]).getFileNameWithoutExtension());
            }
        }
    });

    m.setLookAndFeel(nullptr);
}

PopupMenu CabbagePresetButton::addPresetsToMenu(String custom)
{
    fullPresetList.clear();
    PopupMenu m;
    m.addItem(1, "Save");
    m.addItem(2, "Load Preset Folder");

    m.addSeparator();
    int numPresetFiles = 3;
    StringArray presets = {"User", "Factory"};
    
    for ( auto preset : presets)
    {
        String workingDir = (preset == "User" ? user.folder : factory.folder);
        if(workingDir != "undefined")
        {
            m.addSeparator();
            String fileType = (preset == "User" ? user.extension : factory.extension);
            
            Array<File> directories;
            Array<File> presetFiles;

            File dirToSearch = File::getCurrentWorkingDirectory().getChildFile(workingDir);
            directories = dirToSearch.findChildFiles (File::TypesOfFileToFind::findDirectories, true);
            directories.sort();
            presetFiles = File(workingDir).findChildFiles (File::TypesOfFileToFind::findFiles, false, fileType);
            presetFiles.sort();
            for ( int x = 0 ; x < presetFiles.size() ; x++)
            {
                m.addItem(numPresetFiles, presetFiles[x].getFileNameWithoutExtension());
                fullPresetList.add(presetFiles[x].getFullPathName());
                numPresetFiles++;
            }

            
            for ( int i = 0 ; i < directories.size() ; i++)
            {
                PopupMenu subM;
                presetFiles = directories[i].findChildFiles (File::TypesOfFileToFind::findFiles, false, fileType);
                presetFiles.sort();
                for ( int x = 0 ; x < presetFiles.size() ; x++)
                {
                    subM.addItem(numPresetFiles, presetFiles[x].getFileNameWithoutExtension());
                    fullPresetList.add(presetFiles[x].getFullPathName());
                    numPresetFiles++;
                }
                if(presetFiles.size()>0)
                    m.addSubMenu(directories[i].getFileNameWithoutExtension(), subM);
            }
        }
    }
    
    return m;
}

//===============================================================================
String CabbagePresetButton::returnValidPath (File fc)
{
    return fc.getFullPathName().replaceCharacters ("\\", "/");
}


//===============================================================================
void CabbagePresetButton::setLookAndFeelColours (ValueTree wData)
{
    setColour (TextButton::textColourOffId, Colour::fromString (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::fontcolour)));
    setColour (TextButton::buttonColourId, Colour::fromString (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::colour)));
    setColour (TextButton::textColourOnId, Colour::fromString (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::onfontcolour)));
    setColour (TextButton::buttonOnColourId, Colour::fromString (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::oncolour)));
}

//===============================================================================
void CabbagePresetButton::valueTreePropertyChanged (ValueTree& valueTree, const Identifier& prop)
{
    if(prop == CabbageIdentifierIds::value)
    {
        
        const auto file = File(CabbageWidgetData::getStringProp(valueTree, prop));
        if(file.existsAsFile())
        {
            owner->restorePluginStateFrom (file.getFileNameWithoutExtension(), file.getFullPathName());
        }
        if(replaceTextWithPreset)
            setButtonText(file.getFileNameWithoutExtension());
    }
    else
    {
        setLookAndFeelColours (valueTree);
        setButtonText (getText());
        setTooltip(getCurrentPopupText(valueTree));

        handleCommonUpdates(this, valueTree, false, prop);      //handle comon updates such as bounds, alpha, rotation, visible, etc
    }

}
