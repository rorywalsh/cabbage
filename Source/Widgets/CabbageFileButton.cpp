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

#include "CabbageFileButton.h"
#include "../Audio/Plugins/CabbagePluginEditor.h"

CabbageFileButton::CabbageFileButton (ValueTree wData, CabbagePluginEditor* owner)
    : TextButton(),
    owner (owner),
    widgetData (wData),
lAndF()
{
    widgetData.addListener (this);              //add listener to valueTree so it gets notified when a widget's property changes
    initialiseCommonAttributes (this, wData);   //initialise common attributes such as bounds, name, rotation, etc..
    setLookAndFeelColours (wData);

    tooltipText = CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::popuptext);
    if (tooltipText.isNotEmpty())
        setTooltip(tooltipText);
    
    setButtonText (getText());

    mode = CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::mode);
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
    if(globalStyle == "legacy")
    {
        return;
    }
    
    //if users are passing custom images, use old style look and feel
    if (CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::style) == "flat" &&
        imgOff.isEmpty() && imgOn.isEmpty() && imgOver.isEmpty())
    {
        setLookAndFeel(&flatLookAndFeel);
    }
    
    setFile(wData);
    
}

//===============================================================================
void CabbageFileButton::buttonClicked (Button* button)
{
    DBG(owner->currentPresetName);
    
    
    String workingDir = CabbageWidgetData::getStringProp (widgetData, CabbageIdentifierIds::workingdir);
    File currentDir;
    if (workingDir.isNotEmpty())
        currentDir = File::getCurrentWorkingDirectory().getChildFile (workingDir);
    else
        currentDir = File::getCurrentWorkingDirectory();
    
    if (mode == "file")
    {
        const String lastKnownDirectory = owner->getLastOpenedDirectory();
        if(lastKnownDirectory.isNotEmpty())
            currentDir = File(lastKnownDirectory);
            
        FileChooser fc ("Choose File", currentDir, filetype, CabbageUtilities::shouldUseNativeBrowser());

        if (fc.browseForFileToOpen())
        {
            owner->sendChannelStringDataToCsound (getChannel(), returnValidPath (fc.getResult()));
            CabbageWidgetData::setStringProp (widgetData, CabbageIdentifierIds::file, returnValidPath (fc.getResult()));
            //owner->refreshComboBoxContents();
        }

        owner->setLastOpenedDirectory (fc.getResult().getParentDirectory().getFullPathName());
    }

    else if (mode == "save")
    {
        const String lastKnownDirectory = owner->getLastOpenedDirectory();
        if(lastKnownDirectory.isNotEmpty())
            currentDir = File(lastKnownDirectory);
        
        FileChooser fc ("Choose File", currentDir, filetype, CabbageUtilities::shouldUseNativeBrowser());

        if (fc.browseForFileToSave(true))
        {
            owner->sendChannelStringDataToCsound (getChannel(), returnValidPath (fc.getResult()));
            CabbageWidgetData::setStringProp (widgetData, CabbageIdentifierIds::file, returnValidPath (fc.getResult()));
            //owner->refreshComboBoxContents();
        }

        owner->setLastOpenedDirectory (fc.getResult().getParentDirectory().getFullPathName());
		owner->refreshComboListBoxContents();
        startTimer(500);
    }

    else if (mode == "directory")
    {
        const String lastKnownDirectory = owner->getLastOpenedDirectory();
        FileChooser fc ("Open Directory", lastKnownDirectory.isEmpty() ? File (getCsdFile()).getChildFile (getFilename()) : File (lastKnownDirectory), filetype, CabbageUtilities::shouldUseNativeBrowser());

        if (fc.browseForDirectory())
        {
            owner->sendChannelStringDataToCsound (getChannel(), returnValidPath (fc.getResult()));
            CabbageWidgetData::setStringProp (widgetData, CabbageIdentifierIds::file, returnValidPath (fc.getResult()));
        }

        owner->setLastOpenedDirectory (fc.getResult().getParentDirectory().getFullPathName());
        
    }

    else if (mode == "snapshot")
    {
        String newFileName;
        if (owner->isAudioUnit())
            newFileName = File(getCsdFile()).withFileExtension(".snaps").getFullPathName();
        else
            newFileName = owner->createNewGenericNameForPresetFile();
        
        owner->sendChannelStringDataToCsound (getChannel(), newFileName);
        owner->savePluginStateToFile (File (newFileName));
        owner->refreshComboListBoxContents();
    }
    
    else if (mode == "remove preset")
    {
        String newFileName;
        if (owner->isAudioUnit())
            newFileName = File(getCsdFile()).withFileExtension(".snaps").getFullPathName();
        else
            newFileName = owner->createNewGenericNameForPresetFile();
        
        //owner->sendChannelStringDataToCsound (getChannel(), newFileName);
        CabbageLookAndFeel2* tempLAF = static_cast<CabbageLookAndFeel2*>(&owner->getLookAndFeel());
        
        tempLAF->setColour (AlertWindow::ColourIds::textColourId, Colour (255, 255, 255));
        tempLAF->setColour (TextButton::ColourIds::buttonColourId, Colour (50, 50, 50));
        tempLAF->setColour (TextButton::ColourIds::textColourOffId, Colour (150, 150, 150));
        tempLAF->setColour (TextButton::ColourIds::buttonOnColourId, Colour (150, 150, 150));
        tempLAF->setColour (TextButton::ColourIds::textColourOnId, Colour (250, 250, 250));
        const int result = CabbageUtilities::showYesNoMessage("Are you sure you wish to remove\nthis preset?", tempLAF, 1);
        if(result == 1)
        {
            owner->savePluginStateToFile (File (newFileName), owner->currentPresetName, true);
            owner->refreshComboListBoxContents();
        }
    }
    
    else if (mode == "named snapshot")
    {
        String newFileName;
        if (owner->isAudioUnit())
            newFileName = File(getCsdFile()).withFileExtension(".snaps").getFullPathName();
        else
            newFileName = owner->createNewGenericNameForPresetFile();
        
        
#if JUCE_MODAL_LOOPS_PERMITTED
        String presetname;
//        AlertWindow w ("Set Preset Name",
//                       "(warning, will overwrite previous presets of the same name)",
//                       AlertWindow::NoIcon);
//        w.setLookAndFeel(&lAndF);
//        //w.getLookAndFeel().setColour(AlertWindow::ColourIds::backgroundColourId, Colour(43, 43, 43));
//        //w.setSize(8500, 300);
//        w.addTextEditor ("text", "enter name here", "");
//        w.addButton ("OK",     1, KeyPress (KeyPress::returnKey, 0, 0));
//        w.addButton ("Cancel", 0, KeyPress (KeyPress::escapeKey, 0, 0));

        CabbageLookAndFeel2* tempLAF = static_cast<CabbageLookAndFeel2*>(&owner->getLookAndFeel());
        
        tempLAF->setColour (AlertWindow::ColourIds::textColourId, Colour (255, 255, 255));
        tempLAF->setColour (TextEditor::ColourIds::highlightedTextColourId, Colour (90, 90, 90));
        tempLAF->setColour (TextButton::ColourIds::buttonColourId, Colour (50, 50, 50));
        tempLAF->setColour (TextButton::ColourIds::textColourOffId, Colour (150, 150, 150));
        tempLAF->setColour (TextButton::ColourIds::buttonOnColourId, Colour (150, 150, 150));
        tempLAF->setColour (TextButton::ColourIds::textColourOnId, Colour (250, 250, 250));
        String presetName;
        const int result = CabbageUtilities::showAlertMessageWithTextEditor("Are you sure you wish to remove\nthis preset?", tempLAF, &presetName);
        
        
        if (result == 1)
        {
            owner->sendChannelStringDataToCsound (getChannel(), newFileName);
            owner->savePluginStateToFile (File (newFileName), presetName, false);
            owner->refreshComboListBoxContents();
        }
        else
            return;
#endif
        

    }

    owner->getProcessor().updateHostDisplay();
}

//===============================================================================
void CabbageFileButton::timerCallback()
{
    owner->refreshComboListBoxContents();
}

//===============================================================================
String CabbageFileButton::returnValidPath (File fc)
{
    return fc.getFullPathName().replaceCharacters ("\\", "/");
}
//===============================================================================
void CabbageFileButton::setFile(ValueTree wData)
{
    const String buttonFile = CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::file);
    const String relativePath = File(getCsdFile()).getParentDirectory().getChildFile(buttonFile).getFullPathName();
    if(File(relativePath).existsAsFile())
        owner->sendChannelStringDataToCsound(getChannel(), relativePath.replaceCharacters("\\", "/"));
    else
        owner->sendChannelStringDataToCsound(getChannel(), buttonFile.replaceCharacters("\\", "/"));
}
//===============================================================================
void CabbageFileButton::setLookAndFeelColours (ValueTree wData)
{
    setColour (TextButton::textColourOffId, Colour::fromString (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::fontcolour)));
    setColour (TextButton::buttonColourId, Colour::fromString (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::colour)));
    setColour (TextButton::textColourOnId, Colour::fromString (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::onfontcolour)));
    setColour (TextButton::buttonOnColourId, Colour::fromString (CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::oncolour)));
}

//===============================================================================
void CabbageFileButton::valueTreePropertyChanged (ValueTree& valueTree, const Identifier& prop)
{
    setLookAndFeelColours (valueTree);
    handleCommonUpdates (this, valueTree);      //handle comon updates such as bounds, alpha, rotation, visible, etc
    setButtonText (getText());
    setFile(valueTree);
    setTooltip(getCurrentPopupText(valueTree));
}
