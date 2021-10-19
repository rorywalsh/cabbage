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
    CabbageWidgetBase(owner),
    lAndF()
{
    widgetData.addListener (this);              //add listener to valueTree so it gets notified when a widget's property changes
    initialiseCommonAttributes (this, wData);   //initialise common attributes such as bounds, name, rotation, etc..
    setLookAndFeelColours (wData);

    tooltipText = CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::popuptext);
    if (tooltipText.isNotEmpty())
        setTooltip(tooltipText);
    
    setButtonText (getText());

    
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


	if (userFolder.size() == 0)
	{
		user.folder = File(getCsdFile()).getParentDirectory().getFullPathName();
	}

	if (factoryFolder.size() == 0)
	{
		factory.folder = File(getCsdFile()).getParentDirectory().getFullPathName();
	}

	user.extension = "snaps";
	factory.extension = "snaps";

	if (userFolder.size() > 0)
		user.folder = userFolder[0].toString();
	if (userFolder.size() > 1)
		user.extension = userFolder[1].toString();

	if (factoryFolder.size() > 0)
		factoryFolder = factoryFolder[0].toString();
	if (factoryFolder.size() > 1)
		factory.extension = factoryFolder[1].toString();
	
}

//===============================================================================
void CabbagePresetButton::buttonClicked (Button* button)
{

    PopupMenu m = addPresetsToMenu(currentPresetDir, "");
    
    m.showMenuAsync(juce::PopupMenu::Options(), [this](int r) {
        //minus the two default items on the menu
        DBG(this->fullPresetList[r-2]);
        if(r == 1){
            FileChooser fc ("Choose preset folder", this->currentPresetDir, "", CabbageUtilities::shouldUseNativeBrowser());

            if (fc.browseForDirectory())
            {
                this->currentPresetDir = fc.getResult().getFullPathName();
            }
        }
    });

    
}

PopupMenu CabbagePresetButton::addPresetsToMenu(String user, String factory)
{
    PopupMenu m;
    m.addItem(1, "Load Preset Folder");
    m.addItem(2, "Save Preset");
    m.addSeparator();
    String workingDir = user;
    String fileType = CabbageWidgetData::getStringProp (widgetData, CabbageIdentifierIds::filetype);
    
    Array<File> directories;
    Array<File> presetFiles;

    File dirToSearch = File::getCurrentWorkingDirectory().getChildFile(workingDir);
    directories = dirToSearch.findChildFiles (File::TypesOfFileToFind::findDirectories, true);

    
    presetFiles = File(workingDir).findChildFiles (File::TypesOfFileToFind::findFiles, false, fileType);
    for ( int x = 0 ; x < presetFiles.size() ; x++)
    {
        m.addItem(2+x, presetFiles[x].getFileNameWithoutExtension());
        fullPresetList.add(presetFiles[x].getFullPathName());
    }
    
    int numPresetFiles = presetFiles.size()+2;
    m.addSeparator();
//    DBG(directories.size());
    
    for ( int i = 0 ; i < directories.size() ; i++)
    {
        PopupMenu subM;
//        DBG(directories[i].getFullPathName());
        presetFiles = directories[i].findChildFiles (File::TypesOfFileToFind::findFiles, false, fileType);
        for ( int x = 0 ; x < presetFiles.size() ; x++)
        {
            subM.addItem(numPresetFiles, presetFiles[x].getFileNameWithoutExtension());
            fullPresetList.add(presetFiles[x].getFullPathName());
            numPresetFiles++;
        }
    
        m.addSubMenu(directories[i].getFileNameWithoutExtension(), subM);
        
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
    setLookAndFeelColours (valueTree);
    setButtonText (getText());
    setTooltip(getCurrentPopupText(valueTree));

	handleCommonUpdates(this, valueTree, false, prop);      //handle comon updates such as bounds, alpha, rotation, visible, etc

}
