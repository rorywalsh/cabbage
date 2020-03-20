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
    : widgetData (wData),
      TextButton(),
      owner (owner)
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
	const String file = CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::file);
	const String relativePath = File(getCsdFile()).getParentDirectory().getChildFile(file).getFullPathName();
	if(File(relativePath).existsAsFile())
		owner->sendChannelStringDataToCsound(getChannel(), relativePath.replaceCharacters("\\", "/"));
	else
		owner->sendChannelStringDataToCsound(getChannel(), file.replaceCharacters("\\", "/"));

    
}

//===============================================================================
void CabbageFileButton::buttonClicked (Button* button)
{
    if (mode == "file")
    {
        const String lastKnownDirectory = owner->getLastOpenedDirectory();
        FileChooser fc ("Choose File", lastKnownDirectory.isEmpty() ? File (getCsdFile()).getParentDirectory() : File (lastKnownDirectory), filetype, CabbageUtilities::shouldUseNativeBrowser());

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
        FileChooser fc ("Choose File", lastKnownDirectory.isEmpty() ? File (getCsdFile()).getParentDirectory() : File (lastKnownDirectory), filetype, CabbageUtilities::shouldUseNativeBrowser());

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
    
    else if (mode == "named snapshot")
    {
        String newFileName;
        if (owner->isAudioUnit())
            newFileName = File(getCsdFile()).withFileExtension(".snaps").getFullPathName();
        else
            newFileName = owner->createNewGenericNameForPresetFile();
        
        
#if JUCE_MODAL_LOOPS_PERMITTED
        String presetname;
        AlertWindow w ("Preset",
                       "Set preset name (warning, will overwrite previous preset of same name)",
                       AlertWindow::NoIcon);
        w.setLookAndFeel(&getLookAndFeel());
        w.setSize(100, 100);
        w.addTextEditor ("text", "enter name here", "");
        w.addButton ("OK",     1, KeyPress (KeyPress::returnKey, 0, 0));
        w.addButton ("Cancel", 0, KeyPress (KeyPress::escapeKey, 0, 0));
        
        if (w.runModalLoop() != 0) // is they picked 'ok'
        {
            presetname = w.getTextEditorContents ("text");
        }
#endif
        
        owner->sendChannelStringDataToCsound (getChannel(), newFileName);
        owner->savePluginStateToFile (File (newFileName), presetname);
        owner->refreshComboListBoxContents();
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
    const String file = CabbageWidgetData::getStringProp (valueTree, CabbageIdentifierIds::file);
    setTooltip(getCurrentPopupText(valueTree));
}
