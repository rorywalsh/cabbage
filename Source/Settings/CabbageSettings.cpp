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

#include "CabbageSettings.h"
#include "../Utilities/CabbageUtilities.h"
#include "../Utilities/CabbageStrings.h"
//==============================================================================

CabbageSettings::CabbageSettings():
    ApplicationProperties(),
    valueTree ("Settings")
{
    valueTree.addListener (this);
}


void CabbageSettings::setDefaultColourSettings()
{

}

void CabbageSettings::setDefaultSettings()
{
    defaultPropSet.reset (new PropertySet());
    std::unique_ptr<XmlElement> xml (new XmlElement ("PLANTS"));
    String homeDir = File::getSpecialLocation (File::userHomeDirectory).getFullPathName();
    String manualPath, examplesDir, cabbageHelp, themePath;

    examplesDir = File::getSpecialLocation (File::currentExecutableFile).getParentDirectory().getFullPathName() + "/Examples";
    cabbageHelp = File::getSpecialLocation (File::currentExecutableFile).getParentDirectory().getFullPathName() + "/CabbageManual";
	themePath = File::getSpecialLocation(File::currentExecutableFile).getParentDirectory().getFullPathName() + "/Themes/modern-darkBG";
#if defined(WIN32)
    manualPath = "C:\\Program Files\\Csound6_x64\\doc\\manual";
#elif !defined(MACOSX)
    //manualPath = File::getSpecialLocation (File::currentExecutableFile).getParentDirectory().getFullPathName() + "/CsoundDocs";
    examplesDir = "/usr/share/doc/cabbage/Examples";
    cabbageHelp = "/usr/share/doc/cabbage/CabbageManual";
    manualPath = "/usr/share/doc/csound-doc";
    themePath = "/usr/share/cabbage/Themes/modern-darkBG";
#else
    examplesDir = File::getSpecialLocation (File::currentExecutableFile).getParentDirectory().getParentDirectory().getFullPathName() + "/Examples";
    cabbageHelp = File::getSpecialLocation (File::currentExecutableFile).getParentDirectory().getParentDirectory().getFullPathName() + "/CabbageManual";
    themePath = File::getSpecialLocation(File::currentExecutableFile).getParentDirectory().getParentDirectory().getFullPathName() + "/Themes/modern-darkBG";
    manualPath = "/Library/Frameworks/CsoundLib64.framework/Versions/6.0/Resources/Manual";
    defaultPropSet->setValue ("lastPluginScanPath_VST", "/Users/walshr/Library/Audio/Plug-Ins/VST");
    defaultPropSet->setValue ("lastPluginScanPath_VST3", "/Library/Audio/Plug-Ins/VST3;~/Library/Audio/Plug-Ins/VST3");
    defaultPropSet->setValue ("lastPluginScanPath_AudioUnit", "");
#endif

#ifdef CabbagePro
    defaultPropSet->setValue ("CsoundPath", "/Library/Frameworks/CsoundLib64.framework/CsoundLib64"); 
#endif
    
    defaultPropSet->setValue ("CsoundManualDir", manualPath);
    
    defaultPropSet->setValue ("CabbagePlantDir", homeDir + "/Plants");
    defaultPropSet->setValue ("CabbageExamplesDir", examplesDir);
    defaultPropSet->setValue ("CabbageManualDir", cabbageHelp);
	defaultPropSet->setValue ("CustomThemeDir", themePath);
    defaultPropSet->setValue ("MostRecentDirectory", homeDir);
    defaultPropSet->setValue ("UserFilesDir", homeDir);
    defaultPropSet->setValue ("DisablePluginInfo", 0);
    defaultPropSet->setValue ("ShowEditorConsole", 1);
    defaultPropSet->setValue ("ShowFileBrowser", 1);
    defaultPropSet->setValue ("ExternalEditor", 0);
    defaultPropSet->setValue ("UseCabbageIO", 1);
    defaultPropSet->setValue ("OpenMostRecentFileOnStartup", 1);
    defaultPropSet->setValue ("FontSize", 14);
    defaultPropSet->setValue ("FontSizeConsole", 14);
    defaultPropSet->setValue ("ShowConsoleWithEditor", 1);
    defaultPropSet->setValue ("UsingCabbageCsound", 1);
    defaultPropSet->setValue ("AudioEnabled", 1);
    defaultPropSet->setValue ("NumberOfOpenFiles", 1);
    defaultPropSet->setValue ("DisableCompilerErrorWarning", 0);
    defaultPropSet->setValue ("DisableAutoComplete", 0);
    defaultPropSet->setValue ("SetAlwaysOnTopPlugin", 1);
    defaultPropSet->setValue ("SetAlwaysOnTopGraph", 0);
    defaultPropSet->setValue ("GridSize", 4);
    defaultPropSet->setValue ("CompileOnSave", 1);
    defaultPropSet->setValue ("PlantRepository", xml.get());
    defaultPropSet->setValue ("EditorColourScheme", 0);
    defaultPropSet->setValue ("showTabs", 1);
    defaultPropSet->setValue ("showTabs", 1);
    defaultPropSet->setValue ("EnablePopupDisplay", 1);
    defaultPropSet->setValue ("SpacesInTabs", 4);
    defaultPropSet->setValue ("ShowAutoComplete", 0);
    defaultPropSet->setValue ("ShowNativeFileDialogues", 1);
    defaultPropSet->setValue ("EnableNativePopup", 0);
    defaultPropSet->setValue ("autoConnectNodes", 1);
    defaultPropSet->setValue ("SSHHomeDir", "/pi");
    defaultPropSet->setValue ("SSHAddress", "empty");
    defaultPropSet->setValue ("SSHLocalDir", File::getSpecialLocation (File::SpecialLocationType::userHomeDirectory).getFullPathName());
    defaultPropSet->setValue ("windowX", 100);
    defaultPropSet->setValue ("windowY", 100);
    defaultPropSet->setValue ("numberOfLinesToScroll", 1);
    defaultPropSet->setValue ("audioSetup", "");

    defaultPropSet->setValue ("IDE_LastKnownWidth", 1200);
    defaultPropSet->setValue ("IDE_LastKnownHeight", 800);
    defaultPropSet->setValue ("IDE_LastKnownX", 10);
    defaultPropSet->setValue ("IDE_LastKnownY", 10);
    defaultPropSet->setValue ("IDE_StatusBarPos", 500);
    defaultPropSet->setValue ("searchCaseSensitive", 0);

	//=====================================================================
	defaultPropSet->setValue("AudioDriversWarning_dismiss", 0);

	//=====================================================================

    defaultPropSet->setValue ("Colours_" + CabbageColourIds::menuBarBackground, "ff52636a" /*"ff636262"*/);
    defaultPropSet->setValue ("Colours_" + CabbageColourIds::menuBarText,  "FFFFFFFF");
    defaultPropSet->setValue ("Colours_" + CabbageColourIds::menuBarMouseOverBackground,  "ff5c7387" /*"ff9a9a9a"*/);
    defaultPropSet->setValue ("Colours_" + CabbageColourIds::popupMenuBackground, "ff474e54"/*"fff5f5f5"*/);
    defaultPropSet->setValue ("Colours_" + CabbageColourIds::popupMenuMouseOverBackground,  "ff6c8090"/*"ffa4a4a4"*/);
    defaultPropSet->setValue ("Colours_" + CabbageColourIds::popupMenuText,  "ffd9e4e7"/*"ff484848"*/);
    defaultPropSet->setValue ("Colours_" + CabbageColourIds::popupMenuHighlightedText, "ffffffff" /*"ff000000"*/);
    defaultPropSet->setValue ("Colours_" + CabbageColourIds::mainBackground, "ff52636a");
    defaultPropSet->setValue ("Colours_" + CabbageColourIds::statusBar, "ff000000");
    defaultPropSet->setValue ("Colours_" + CabbageColourIds::statusBarText, "ffffffff");
    defaultPropSet->setValue ("Colours_" + CabbageColourIds::propertyPanelBackground, "ffa6b3b9");
    defaultPropSet->setValue ("Colours_" + CabbageColourIds::propertyLabelText, "ff000000");
    defaultPropSet->setValue ("Colours_" + CabbageColourIds::propertyLabelBackground, "ffd4d4d4");
    defaultPropSet->setValue ("Colours_" + CabbageColourIds::alertWindowBackground, "ff52636a");
    defaultPropSet->setValue ("Colours_" + CabbageColourIds::codeBackground,  "ff263238"/*"ff222222"*/);
    defaultPropSet->setValue ("Colours_" + CabbageColourIds::lineNumberBackground,  "ff323e44"/*"ff000000"*/);
    defaultPropSet->setValue ("Colours_" + CabbageColourIds::lineNumbers,  "e999a7ae"/*"E9B2B2B2"*/);
    defaultPropSet->setValue ("Colours_" + CabbageColourIds::selectTextBackground, "ff3f616c" /*"FF2859AC"*/);
    defaultPropSet->setValue ("Colours_" + CabbageColourIds::caret,  "fff39636"/*"ffe2a4a4"*/);
    defaultPropSet->setValue ("Colours_" + CabbageColourIds::identifierLiteral,  "ffcfcfcf"/*"ff007dff"*/);
    defaultPropSet->setValue ("Colours_" + CabbageColourIds::stringLiteral,  "ff8ac3f3"/*"ff3fff00"*/);
    defaultPropSet->setValue ("Colours_" + CabbageColourIds::keyword, "ffee6f6f"/*"ffc5c5c5"*/);
    defaultPropSet->setValue ("Colours_" + CabbageColourIds::comment, "ff72d20c"/*"FF72D20C"*/);
    defaultPropSet->setValue ("Colours_" + CabbageColourIds::numbers, "ffe9ec64"/*"ffffffff"*/);
    defaultPropSet->setValue ("Colours_" + CabbageColourIds::csdtags, "ffbf74c5"/*"ff575757"*/);
    defaultPropSet->setValue ("Colours_" + CabbageColourIds::consoleText, "ff566c7b"/*"FFFFFFFF"*/);
    defaultPropSet->setValue ("Colours_" + CabbageColourIds::consoleBackground, "ff16191d"/*"FF000000"*/);
    defaultPropSet->setValue ("Colours_" + CabbageColourIds::consoleOutline, "ff16191d"/*"ffababab"*/);
    defaultPropSet->setValue ("Colours_" + CabbageColourIds::genericPluginInterface, "FF666666");
    defaultPropSet->setValue ("Colours_" + CabbageColourIds::scrollbars,  "ff5a626f"/*"ff222222"*/);
	defaultPropSet->setValue ("Colours_" + CabbageColourIds::fileTabBar, "ff323e44");
	defaultPropSet->setValue ("Colours_" + CabbageColourIds::fileTabButton, "ff52636a");
	defaultPropSet->setValue ("Colours_" + CabbageColourIds::fileTabText, "ffffffff");
	defaultPropSet->setValue("Colours_" + CabbageColourIds::fileTabPlayButton, "ff323e44");
	defaultPropSet->setValue ("Colours_" + CabbageColourIds::patcher, Colour(200, 200, 200).toString());
    getUserSettings()->setFallbackPropertySet (defaultPropSet.get());

    valueTree.addChild (ValueTree ("Colours"), -1, 0);
    const StringArray colourIDStrings = CabbageStrings::getColourIDStrings();

    for (int i = 0; i < colourIDStrings.size(); i++)
    {
        valueTree.getChildWithName ("Colours").setProperty (colourIDStrings[i],
                                                            getUserSettings()->getValue ("Colours_" + colourIDStrings[i]), 0);
    }


    audioSettingsXml = getUserSettings()->getValue ("audioSetup").replace ("<?xml version=\"1.0\" encoding=\"UTF-8\"?>", "");

}

void CabbageSettings::set (String child, Identifier identifier, var value)
{
    if (valueTree.getChildWithName (child).isValid())
        valueTree.getChildWithName (child).setProperty (identifier,  value, 0);
    else
    {
        valueTree.addChild (ValueTree (child), -1, 0);
        valueTree.getChildWithName (child).setProperty (identifier,  value, 0);
    }
}

void CabbageSettings::setProperty (String child, var value)
{
    getUserSettings()->setValue (child, value);
}

int CabbageSettings::getIntProperty (String child)
{
    return getUserSettings()->getIntValue (child);
}

String CabbageSettings::get (String child, String identifier)
{
    return valueTree.getChildWithName (child).getProperty (identifier).toString();
}

void CabbageSettings::set (ValueTree tree, String child, Identifier identifier, var value)
{
    if (tree.getChildWithName (child).isValid())
        tree.getChildWithName (child).setProperty (identifier,  value.toString(), 0);
    else
    {
        tree.addChild (ValueTree (child), -1, 0);
        tree.getChildWithName (child).setProperty (identifier,  value.toString(), 0);
    }
}

String CabbageSettings::get (ValueTree tree, String child, String identifier)
{
    return tree.getChildWithName (child).getProperty (identifier);
}

Colour CabbageSettings::getColourFromValueTree (ValueTree tree, Identifier identifier, Colour defaultColour)
{
    const String colour = tree.getChildWithName ("Colours").getProperty (identifier, defaultColour.toString()).toString();
    return Colour::fromString (colour);
}

String CabbageSettings::getColourPropertyName (ValueTree tree, int index)
{
    return tree.getChildWithName ("Colours").getPropertyName (index).toString();
}

Colour CabbageSettings::getColourFromValueTree (ValueTree tree, int index, Colour defaultColour)
{
    if (index <= tree.getChildWithName ("Colours").getNumProperties())
    {
        const String colour = tree.getChildWithName ("Colours").getProperty (tree.getChildWithName ("Colours").getPropertyName (index), defaultColour.toString()).toString();
        return Colour::fromString (colour);
    }
    else
        return defaultColour;
}

ValueTree CabbageSettings::getValueTree()
{
    return valueTree;
}

void CabbageSettings::valueTreePropertyChanged (ValueTree& tree, const Identifier& value)
{
    if (valueTree.getChildWithName ("Colours").getProperty (value).toString().isNotEmpty())
        getUserSettings()->setValue ("Colours_" + value.toString(), valueTree.getChildWithName ("Colours").getProperty (value).toString());

    sendChangeMessage();
}

XmlElement* CabbageSettings::getXML (String identifier)
{
    return getUserSettings()->getXmlValue (identifier).get();
}

int CabbageSettings::getIndexOfProperty (String child, String identifier)
{
    return valueTree.indexOf (valueTree.getChildWithName (child));
}

void CabbageSettings::updateRecentFilesList (File file)
{
    recentFiles.restoreFromString (getUserSettings()->getValue ("recentlyOpenedFiles"));
    recentFiles.addFile (file);
    getUserSettings()->setValue ("recentlyOpenedFiles", recentFiles.toString());
}

void CabbageSettings::updateRecentFilesList()
{
    recentFiles.restoreFromString (getUserSettings()->getValue ("recentlyOpenedFiles"));
    getUserSettings()->setValue ("recentlyOpenedFiles", recentFiles.toString());
}

File CabbageSettings::getMostRecentFile (int index)
{
    //recentFiles.restoreFromString (getUserSettings()->getValue ("recentlyOpenedFiles"));
    return recentFiles.getFile (index);
}



