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

    auto csdPath = owner->getProcessor().getCsdFile().getFullPathName();
    setImgProperties (*this, wData, csdPath, "buttonon");
    setImgProperties (*this, wData, csdPath, "buttonoff");
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
    
	readonly = CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::readonly);

	var userFolder = CabbageWidgetData::getProperty(wData, CabbageIdentifierIds::userFolder);
	var factoryFolder = CabbageWidgetData::getProperty(wData, CabbageIdentifierIds::factoryFolder);


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
    if (userFolder.size() > 2)
        user.useNameAsSubMenu = userFolder[2].toString().trim().getIntValue() == 1 ? true : false;

	if (factoryFolder.size() > 0)
    {
        factoryFolder[0] = CabbageUtilities::expandDirectoryMacro(factoryFolder[0]);
        factory.folder = File(getCsdFile()).getParentDirectory().getChildFile(factoryFolder[0].toString()).getFullPathName();
    }
	if (factoryFolder.size() > 1)
		factory.extension = factoryFolder[1].toString().trim();
    if (factoryFolder.size() > 2)
        factory.useNameAsSubMenu = factoryFolder[2].toString().trim().getIntValue() == 1 ? true : false;
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

    const int numDefaultMenuItems = 4;

    m.showMenuAsync(juce::PopupMenu::Options(), [this, numDefaultMenuItems](int choice) {
        if(choice == 0){
            owner->sendChannelDataToCsound("PRESET_STATE", 0.0);
            return;
        }
            

		if (choice == 1 && !currentPreset.existsAsFile())
			choice = 2;

        if(choice == 1){
			owner->savePluginStateToFile (this->currentPreset.getFileNameWithoutExtension(), this->currentPreset.getFullPathName(), false);
            owner->sendChannelStringDataToCsound(this->getChannel(), this->currentPreset.getFullPathName());
            owner->sendChannelDataToCsound("PRESET_STATE", 1.0);
        }
        else if(choice == 2){
            FileChooser fc("Save as", File(this->user.folder), this->user.extension, CabbageUtilities::shouldUseNativeBrowser());

            if (fc.browseForFileToSave(true))
            {
                owner->sendChannelDataToCsound("PRESET_STATE", 2.0);
                owner->savePluginStateToFile (fc.getResult().getFileNameWithoutExtension(), fc.getResult().getFullPathName(), false);
                owner->sendChannelStringDataToCsound(this->getChannel(), fc.getResult().getFullPathName());
                CabbageWidgetData::setStringProp(widgetData, CabbageIdentifierIds::value, fc.getResult().getFullPathName());
            }
        }
        else if(choice == 3)
        {
            if(File(this->user.folder).exists()){
                File(this->user.folder).revealToUser();
                owner->sendChannelDataToCsound("PRESET_STATE", 3.0);
            }
        }
        else if(choice > 3 && choice < 3000)
        {
            owner->sendChannelDataToCsound("PRESET_STATE",4.0);
            owner->sendChannelStringDataToCsound(this->getChannel(), File(this->fullPresetList[choice-numDefaultMenuItems]).getFullPathName());
            owner->restorePluginStateFrom (File(this->fullPresetList[choice-numDefaultMenuItems]).getFileNameWithoutExtension(), this->fullPresetList[choice-numDefaultMenuItems]);
            CabbageWidgetData::setStringProp(widgetData, CabbageIdentifierIds::value, this->fullPresetList[choice-numDefaultMenuItems]);
            currentPreset = File(this->fullPresetList[choice-numDefaultMenuItems]);
            
            if(replaceTextWithPreset)
            {
                setButtonText(File(this->fullPresetList[choice-numDefaultMenuItems]).getFileNameWithoutExtension());
            }
        }
        else if(choice==3000)
        {
            showPopupWindow();
        }
        else if(choice == 3001)
        {       
            owner->sendChannelDataToCsound("PRESET_STATE", 6.0);
            int currentIndex = fullPresetList.indexOf(currentPreset.getFullPathName());
            currentPreset.deleteFile();
            fullPresetList.remove(currentIndex);
            owner->sendChannelStringDataToCsound(this->getChannel(), File(fullPresetList[fullPresetList.size()-1]).getFullPathName());
            owner->restorePluginStateFrom (File(fullPresetList[fullPresetList.size()-1]).getFileNameWithoutExtension(), this->fullPresetList[fullPresetList.size()-1]);
            CabbageWidgetData::setStringProp(widgetData, CabbageIdentifierIds::value, fullPresetList[fullPresetList.size()-1]);
            currentPreset = File(fullPresetList[fullPresetList.size()-1]);
            
        }
    });

    m.setLookAndFeel(nullptr);
}

PopupMenu CabbagePresetButton::addPresetsToMenu(String custom)
{

    fullPresetList.clear();
    PopupMenu m;
	if (readonly == 0)
	{
        if(CabbageWidgetData::getNumProp(widgetData, CabbageIdentifierIds::presetBrowser) == 1.f)
        {
            m.addItem(1, "Save");
            if(CabbageWidgetData::getNumProp(widgetData, CabbageIdentifierIds::saveas) == 1.f)
                m.addItem(2, "Save As");
            m.addItem(3, "Preset Folder");
        }
        else
        {
            m.addItem(3000, "Save");
            m.addItem(3001, "Remove");
        }

	}

    m.addSeparator();
    int numPresetFiles = 4;
    StringArray presets = {"User", "Factory"};
    
    for ( auto preset : presets)
    {
        PopupMenu subMenu;

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
                if(user.useNameAsSubMenu)
                    subMenu.addItem(numPresetFiles, presetFiles[x].getFileNameWithoutExtension());
                else
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
                {
                    if(user.useNameAsSubMenu == false)
                        m.addSubMenu(directories[i].getFileNameWithoutExtension(), subM);
                    else
                        subMenu.addSubMenu(directories[i].getFileNameWithoutExtension(), subM);
                }
                    
            }
            
            if(user.useNameAsSubMenu)
                m.addSubMenu((preset == "User" ? File(user.folder).getFileNameWithoutExtension() : File(factory.folder).getFileNameWithoutExtension()), subMenu);

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
void CabbagePresetButton::showPopupWindow()
{
    String presetName = "";
    popupWindow = std::make_unique<CabbagePopupWindow>(widgetData, svgText, true);
    if(!owner)
        popupWindow->addToDesktop(1);
    else
        owner->getParentComponent()->addChildComponent(popupWindow.get());

    popupWindow->setBounds(owner->getWidth()/2 - 150,
                           100, 300, 150);
    popupWindow->setVisible(true);
    popupWindow->toFront(true);
    popupWindow->setAlwaysOnTop(true);
    Component::SafePointer<CabbagePresetButton> parent{ this };
    popupWindow->enterModalState(true, juce::ModalCallbackFunction::create([parent](int)
                                                                            {
        if (parent == nullptr)
            return;

        parent->popupWindow = nullptr;
    }));

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
    else if(prop == CabbageIdentifierIds::svgElement)
    {
        svgText = CabbageWidgetData::getStringProp(valueTree, CabbageIdentifierIds::svgElement);
    }
    else if(prop == Identifier("NEW_PRESET_NAME"))
    {
        File presetFile = File(user.folder).getChildFile(valueTree.getProperty(prop).toString());
        const String ext = user.extension.substring(user.extension.indexOf(".")+1);
        owner->savePluginStateToFile (presetFile.getFileNameWithoutExtension(), presetFile.withFileExtension(ext).getFullPathName(), false);
        owner->sendChannelDataToCsound("PRESET_STATE", 5.0);
        owner->sendChannelStringDataToCsound(this->getChannel(), presetFile.getFullPathName());
        CabbageWidgetData::setStringProp(widgetData, CabbageIdentifierIds::value, presetFile.getFullPathName());
    }
    else
    {
        setLookAndFeelColours (valueTree);
        setButtonText (getText());
        setTooltip(getCurrentPopupText(valueTree));

        handleCommonUpdates(this, valueTree, false, prop);      //handle comon updates such as bounds, alpha, rotation, visible, etc
    }

}
