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
    if( mode == "file" || mode == "save" || mode == "directory")
        setFile(wData);
    
    
    if(globalStyle == "legacy")
    {        
        int fontstyle = CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::fontstyle);
        owner->customFont.setStyleFlags(fontstyle);
        return;
    }
    
    //if users are passing custom images, use old style look and feel
    if (CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::style) == "flat" &&
        imgOff.isEmpty() && imgOn.isEmpty() && imgOver.isEmpty())
    {
        int fontstyle = CabbageWidgetData::getNumProp (wData, CabbageIdentifierIds::fontstyle);
        owner->customFont.setStyleFlags(fontstyle);
        flatLookAndFeel.customFont = owner->customFont;
        setLookAndFeel(&flatLookAndFeel);
        lookAndFeelChanged();
        setLookAndFeel(&flatLookAndFeel);
    }
}

//===============================================================================
void CabbageFileButton::buttonClicked (Button* button)
{   
    DBG(getChannel());
    String workingDir = CabbageWidgetData::getStringProp (widgetData, CabbageIdentifierIds::currentdir);
    workingDir = CabbageUtilities::expandDirectoryMacro(workingDir);
    const String csdFile = CabbageWidgetData::getStringProp(widgetData, CabbageIdentifierIds::csdfile);
    File currentDir;
    if(File(csdFile).getParentDirectory().getChildFile(workingDir).exists())
        currentDir = File(workingDir);
    else if (workingDir.isNotEmpty())
        currentDir = File(csdFile).getChildFile (workingDir).getParentDirectory();
    else
        currentDir = File(csdFile).getParentDirectory();
    
    const String presetFileName = CabbageWidgetData::getStringProp (widgetData, "fileType");
    File fileName;
    
    if(presetFileName!="*.snaps" && presetFileName!=".snaps" && presetFileName!="*")
        fileName = File (getCsdFile()).getParentDirectory().getChildFile(presetFileName);
    else
        fileName = File (getCsdFile()).withFileExtension (".snaps");
    
    if (!fileName.existsAsFile())
    {
#ifdef JUCE_MAC
            String path = CabbageUtilities::getRealUserHomeDirectory().getFullPathName() + "/Library/" + String(CabbageManufacturer) + "/" + File(getCsdFile()).getParentDirectory().getParentDirectory().getFileNameWithoutExtension() + "/" + fileName.getFileName();
#else
            String path = File::getSpecialLocation(File::userApplicationDataDirectory).getFullPathName() + "/" + String(CabbageManufacturer) + "/" + File(getCsdFile()).getFileNameWithoutExtension() + "/" + fileName.getFileName();
#endif        
        if(File(path).existsAsFile())
            fileName = File(path);
    }
    
    LookAndFeel_V4* tempLAF = new LookAndFeel_V4();
    
    if (mode == "file")
    {
        if(CabbageWidgetData::getNumProp(widgetData, CabbageIdentifierIds::ignorelastdir) == 0)
        {
            const String lastKnownDirectory = owner->getLastOpenedDirectory();
            if(lastKnownDirectory.isNotEmpty())
                currentDir = File(lastKnownDirectory);
        }
        fc.reset(new FileChooser("Choose File", currentDir, filetype, CabbageUtilities::shouldUseNativeBrowser()));


        fc->launchAsync(FileBrowserComponent::openMode | FileBrowserComponent::canSelectFiles,
            [this](const FileChooser& fc) mutable
        {
            if(fc.getResult() != File())
            {
                owner->sendChannelStringDataToCsound(getChannel(), returnValidPath(fc.getResult()));
                CabbageWidgetData::setStringProp(widgetData, CabbageIdentifierIds::file, returnValidPath(fc.getResult()));
                //owner->refreshComboListBoxContents();
                owner->setLastOpenedDirectory(fc.getResult().getParentDirectory().getFullPathName());
            }
        });
    }

    else if (mode == "save")
    {
        if(CabbageWidgetData::getNumProp(widgetData, CabbageIdentifierIds::ignorelastdir) == 0)
        {
            const String lastKnownDirectory = owner->getLastOpenedDirectory();
            if(lastKnownDirectory.isNotEmpty())
                currentDir = File(lastKnownDirectory);
        }
        
        //FileChooser fc ("Choose File", currentDir, filetype, CabbageUtilities::shouldUseNativeBrowser());
        fc.reset(new FileChooser("Choose File", currentDir, filetype, CabbageUtilities::shouldUseNativeBrowser()));

        fc->launchAsync(FileBrowserComponent::saveMode | FileBrowserComponent::canSelectFiles,
            [this](const FileChooser& fc) mutable
        {
            owner->sendChannelStringDataToCsound(getChannel(), returnValidPath(fc.getResult()));
            CabbageWidgetData::setStringProp(widgetData, CabbageIdentifierIds::file, returnValidPath(fc.getResult()));
            owner->setLastOpenedDirectory(fc.getResult().getParentDirectory().getFullPathName());
            owner->refreshComboListBoxContents();
            startTimer(500);
        });

      

       
    }

    else if (mode == "directory")
    {
        if(CabbageWidgetData::getNumProp(widgetData, CabbageIdentifierIds::ignorelastdir) == 0)
        {
            const String lastKnownDirectory = owner->getLastOpenedDirectory();
            if(lastKnownDirectory.isNotEmpty())
                currentDir = File(lastKnownDirectory);
        }
        
        fc.reset(new FileChooser("Open Directory", currentDir, filetype, CabbageUtilities::shouldUseNativeBrowser()));

        fc->launchAsync(FileBrowserComponent::openMode | FileBrowserComponent::canSelectDirectories,
            [this](const FileChooser& fc) mutable
        {
            owner->sendChannelStringDataToCsound(getChannel(), returnValidPath(fc.getResult()));
            CabbageWidgetData::setStringProp(widgetData, CabbageIdentifierIds::file, returnValidPath(fc.getResult()));
            owner->setLastOpenedDirectory(fc.getResult().getParentDirectory().getFullPathName());
        });

        
        
    }

    else if (mode == "snapshot" || mode == "preset")
    {
        //owner->sendChannelStringDataToCsound(getChannel(), "");
        owner->savePluginStateToFile("", fileName.getFullPathName(), false);
        owner->refreshComboListBoxContents("");
    }
    
    else if (mode == "remove preset")
    {
        if(allowPresetChanges(owner->getCurrentPreset()))
        {
           
            AlertWindow w("Preset",
                          "Are you sure you wish to remove this preset?",
                          AlertWindow::NoIcon);
            
            w.setLookAndFeel(tempLAF);
            w.setSize(200, 100);
            w.addButton("Yes", 1, KeyPress(KeyPress::returnKey, 0, 0));
            w.addButton("No", 0, KeyPress(KeyPress::escapeKey, 0, 0));

            if (w.runModalLoop() != 0) // if they picked 'ok'
            {
                owner->savePluginStateToFile(owner->getCurrentPreset(), fileName.getFullPathName(), true);
                owner->refreshComboListBoxContents();
            }
        }
        else
        {
            AlertWindow w("Preset",
                          "This preset cannot be removed",
                          AlertWindow::NoIcon);
            w.setLookAndFeel(tempLAF);
            w.setSize(200, 100);
            w.addButton("Ok", 1, KeyPress(KeyPress::returnKey, 0, 0));

            w.runModalLoop();
        }
    }
    
    else if (mode == "named preset" || mode == "named snapshot")
    {
#if JUCE_MODAL_LOOPS_PERMITTED
        String presetName;
        AlertWindow w("Preset",
            "(will overwrite previous preset of same name)",   
            AlertWindow::NoIcon);

        w.setLookAndFeel(tempLAF);
        w.setSize(200, 100);
        w.addTextEditor("text", "enter preset name", "");
        w.addButton("OK", 1, KeyPress(KeyPress::returnKey, 0, 0));
        w.addButton("Cancel", 0, KeyPress(KeyPress::escapeKey, 0, 0));

        if (w.runModalLoop() != 0) // if they picked 'ok'
        {
            presetName = w.getTextEditorContents("text");
        }
        
        if(presetName.isEmpty())
            return;
        
        if(allowPresetChanges(presetName))
        {
            owner->setCurrentPreset(presetName);
            owner->sendChannelStringDataToCsound(getChannel(), presetName);
            owner->savePluginStateToFile(presetName, fileName.getFullPathName(), false);
            owner->refreshComboListBoxContents(presetName);
            
        }
        else
        {
            AlertWindow::showMessageBox(AlertWindow::AlertIconType::NoIcon, "Preset", "You can not remove this preset");
        }
#endif
    }

    tempLAF = nullptr;
    owner->getProcessor().updateHostDisplay();
}

bool CabbageFileButton::allowPresetChanges(String presetName)
{
    auto vt = owner->getProcessor().cabbageWidgets;
    for ( int i = 0 ; i < vt.getNumChildren() ; i++)
    {
        const String type = CabbageWidgetData::getStringProp (vt.getChild (i), CabbageIdentifierIds::type);
        
        if ( type == "combobox" || type == "listbox")
        {
            if(CabbageWidgetData::getStringProp (vt.getChild (i), CabbageIdentifierIds::filetype).contains("snaps"))
            {
                int protectedItems = CabbageWidgetData::getNumProp (vt.getChild (i), CabbageIdentifierIds::protecteditems);
                var items = CabbageWidgetData::getProperty (vt.getChild (i), CabbageIdentifierIds::text);
                if(protectedItems > items.size())
                    return false;
                
                for (int i = 0; i < protectedItems ; i++)
                {
                    if(items[i].toString() == presetName)
                    {
                        return false;
                    }
                }
            }
        }
    }
    return true;
}
//===============================================================================
void CabbageFileButton::timerCallback()
{
    //owner->refreshComboListBoxContents();
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
    getProperties().set("outlinecolour", CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::outlinecolour));
    getProperties().set("outlinethickness", CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::outlinethickness));
    getProperties().set("corners", CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::corners));
    repaint();
}

//===============================================================================
void CabbageFileButton::valueTreePropertyChanged (ValueTree& valueTree, const Identifier& prop)
{
    DBG(getChannel());
    setLookAndFeelColours (valueTree);
    handleCommonUpdates (this, valueTree, prop);      //handle comon updates such as bounds, alpha, rotation, visible, etc
    setButtonText (getText());
    if( mode == "file" || mode == "save" || mode == "directory")
        setFile(valueTree);
    setTooltip(getCurrentPopupText(valueTree));
}
