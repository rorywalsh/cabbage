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
    if( mode == "file" || mode == "save" || mode == "directory")
        setFile(wData);
    
    
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
    

}

//===============================================================================
void CabbageFileButton::buttonClicked (Button* button)
{
   
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
        newFileName = File(getCsdFile()).withFileExtension(".snaps").getFullPathName();

#ifdef JUCE_WINDOWS
        newFileName = File::getSpecialLocation(File::userApplicationDataDirectory).getFullPathName() + "/" + String(JucePlugin_Manufacturer) + "/" + File::getSpecialLocation(File::currentExecutableFile).getFileNameWithoutExtension() + "/" + File::getSpecialLocation(File::currentExecutableFile).withFileExtension(String(".snaps")).getFileName();
        if (!File(newFileName).existsAsFile())
        {
            newFileName = File(getCsdFile()).withFileExtension(".snaps").getFullPathName();
        }
#endif 
        
        owner->sendChannelStringDataToCsound (getChannel(), newFileName);
        owner->savePluginStateToFile (File (newFileName));
        owner->refreshComboListBoxContents();
    }
    
    else if (mode == "remove preset")
    {
        String newFileName;
        newFileName = File(getCsdFile()).withFileExtension(".snaps").getFullPathName();

#ifdef JUCE_WINDOWS
        newFileName = File::getSpecialLocation(File::userApplicationDataDirectory).getFullPathName() + "/" + String(JucePlugin_Manufacturer) + "/" + File::getSpecialLocation(File::currentExecutableFile).getFileNameWithoutExtension() + "/" + File::getSpecialLocation(File::currentExecutableFile).withFileExtension(String(".snaps")).getFileName();
        if (!File(newFileName).existsAsFile())
        {
            newFileName = File(getCsdFile()).withFileExtension(".snaps").getFullPathName();
        }
#endif 
        
        //owner->sendChannelStringDataToCsound (getChannel(), newFileName);
        AlertWindow w("Preset",
            "Are you sure you wish to remove\nthis preset ?",
            AlertWindow::NoIcon);
        String presetName;
        LookAndFeel_V4* tempLAF = new LookAndFeel_V4();
        tempLAF->setColour(AlertWindow::ColourIds::backgroundColourId, Colour(34, 34, 34));
        tempLAF->setColour(AlertWindow::ColourIds::textColourId, Colour(180, 180, 180));
        tempLAF->setColour(TextEditor::ColourIds::highlightedTextColourId, Colour(190, 190, 190));
        tempLAF->setColour(TextButton::ColourIds::buttonColourId, Colour(50, 50, 50));
        tempLAF->setColour(TextButton::ColourIds::textColourOffId, Colour(150, 150, 150));
        tempLAF->setColour(TextButton::ColourIds::buttonOnColourId, Colour(150, 150, 150));
        tempLAF->setColour(TextButton::ColourIds::textColourOnId, Colour(250, 250, 250));
        w.setLookAndFeel(tempLAF);
        w.setSize(100, 100);
        w.addButton("OK", 1, KeyPress(KeyPress::returnKey, 0, 0));
        w.addButton("Cancel", 0, KeyPress(KeyPress::escapeKey, 0, 0));
        if (w.runModalLoop() != 0) // if they picked 'ok'
        {
            presetName = w.getTextEditorContents("text");
            owner->sendChannelStringDataToCsound(getChannel(), owner->currentPresetName);
            owner->savePluginStateToFile(File(newFileName), owner->currentPresetName, true);
            owner->refreshComboListBoxContents();
        }

       /* const int result = CabbageUtilities::showYesNoMessage("Are you sure you wish to remove\nthis preset?", tempLAF);
        if(result == 1)
        {
            owner->sendChannelStringDataToCsound (getChannel(), owner->currentPresetName);
            owner->savePluginStateToFile (File (newFileName), owner->currentPresetName, true);
            owner->refreshComboListBoxContents();
        }*/
    }
    
    else if (mode == "named snapshot")
    {
        String newFileName;
        newFileName = File(getCsdFile()).withFileExtension(".snaps").getFullPathName();

#ifdef JUCE_WINDOWS
        newFileName = File::getSpecialLocation(File::userApplicationDataDirectory).getFullPathName() + "/" + String(JucePlugin_Manufacturer) + "/" + File::getSpecialLocation(File::currentExecutableFile).getFileNameWithoutExtension() + "/" + File::getSpecialLocation(File::currentExecutableFile).withFileExtension(String(".snaps")).getFileName();
        if (!File(newFileName).existsAsFile())
        {
            newFileName = File(getCsdFile()).withFileExtension(".snaps").getFullPathName();
        }
#endif        
        
#if JUCE_MODAL_LOOPS_PERMITTED


        //String presetname;

        //CabbageLookAndFeel2* tempLAF = static_cast<CabbageLookAndFeel2*>(&owner->getLookAndFeel());
        //tempLAF->setColour (AlertWindow::ColourIds::textColourId, Colour (255, 255, 255));
        //tempLAF->setColour (TextEditor::ColourIds::highlightColourId, Colour (185, 185, 185));
        //tempLAF->setColour (TextEditor::ColourIds::highlightedTextColourId, Colour (50, 50, 50));
        //tempLAF->setColour (TextButton::ColourIds::buttonColourId, Colour (50, 50, 50));
        //tempLAF->setColour (TextButton::ColourIds::textColourOffId, Colour (150, 150, 150));
        //tempLAF->setColour (TextButton::ColourIds::buttonOnColourId, Colour (150, 150, 150));
        //tempLAF->setColour (TextButton::ColourIds::textColourOnId, Colour (250, 250, 250));
        //String presetName;
        //const int result = CabbageUtilities::showAlertMessageWithTextEditor("Set Preset Name", "Warning: This will overwrite any\npreviously saved presets of the same name.", tempLAF, &presetName);
        //


        String presetName;
        AlertWindow w("Preset",
            "(will overwrite previous preset of same name)",
            AlertWindow::NoIcon);
        LookAndFeel_V4* tempLAF = new LookAndFeel_V4();
        tempLAF->setColour(AlertWindow::ColourIds::backgroundColourId, Colour(34, 34, 34));
        tempLAF->setColour(AlertWindow::ColourIds::textColourId, Colour(180, 180, 180));
        tempLAF->setColour(TextEditor::ColourIds::highlightedTextColourId, Colour(190, 190, 190));
        tempLAF->setColour(TextButton::ColourIds::buttonColourId, Colour(50, 50, 50));
        tempLAF->setColour(TextButton::ColourIds::textColourOffId, Colour(150, 150, 150));
        tempLAF->setColour(TextButton::ColourIds::buttonOnColourId, Colour(150, 150, 150));
        tempLAF->setColour(TextButton::ColourIds::textColourOnId, Colour(250, 250, 250));
        w.setLookAndFeel(tempLAF);
        w.setSize(200, 100);
        w.addTextEditor("text", "enter preset name", "");
        w.addButton("OK", 1, KeyPress(KeyPress::returnKey, 0, 0));
        w.addButton("Cancel", 0, KeyPress(KeyPress::escapeKey, 0, 0));

        if (w.runModalLoop() != 0) // if they picked 'ok'
        {
            presetName = w.getTextEditorContents("text");
            owner->sendChannelStringDataToCsound(getChannel(), presetName);
            owner->savePluginStateToFile(File(newFileName), presetName, false);
            owner->refreshComboListBoxContents(presetName);
        }

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
    handleCommonUpdates (this, valueTree, false, prop);      //handle comon updates such as bounds, alpha, rotation, visible, etc
    setButtonText (getText());
    if( mode == "file" || mode == "save" || mode == "directory")
        setFile(valueTree);
    setTooltip(getCurrentPopupText(valueTree));
}
