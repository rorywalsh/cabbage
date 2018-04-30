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

    setButtonText (getText());

    mode = CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::mode);
    filetype = CabbageWidgetData::getStringProp (wData, CabbageIdentifierIds::filetype).removeCharacters ("*.");

    setImgProperties (*this, wData, "buttonon");
    setImgProperties (*this, wData, "buttonoff");
    addListener (this);

}

//===============================================================================
void CabbageFileButton::buttonClicked (Button* button)
{

    if (mode == "file")
    {
        const String lastKnownDirectory = owner->getLastOpenedDirectory();
        FileChooser fc ("Choose File", lastKnownDirectory.isEmpty() ? File (getCsdFile()).getParentDirectory() : File (lastKnownDirectory), "", CabbageUtilities::shouldUseNativeBrowser());

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
        FileChooser fc ("Choose File", lastKnownDirectory.isEmpty() ? File (getCsdFile()).getParentDirectory() : File (lastKnownDirectory), "", CabbageUtilities::shouldUseNativeBrowser());

        if (fc.browseForFileToSave(true))
        {
            owner->sendChannelStringDataToCsound (getChannel(), returnValidPath (fc.getResult()));
            CabbageWidgetData::setStringProp (widgetData, CabbageIdentifierIds::file, returnValidPath (fc.getResult()));
            //owner->refreshComboBoxContents();
        }

        owner->setLastOpenedDirectory (fc.getResult().getParentDirectory().getFullPathName());
    }

    else if (mode == "directory")
    {
        const String lastKnownDirectory = owner->getLastOpenedDirectory();
        FileChooser fc ("Open Directory", lastKnownDirectory.isEmpty() ? File (getCsdFile()).getChildFile (getFilename()) : File (lastKnownDirectory), "", CabbageUtilities::shouldUseNativeBrowser());

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
        owner->refreshComboBoxContents();
    }

    owner->getProcessor().updateHostDisplay();
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
}
