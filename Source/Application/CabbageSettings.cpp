/*
  ==============================================================================

    CabbageSettings.cpp
    Created: 12 Oct 2016 12:12:34pm
    Author:  rory

  ==============================================================================
*/

#include "CabbageSettings.h"
#include "../Utilities/CabbageUtilities.h"
#include "../Utilities/CabbageStrings.h"
//==============================================================================

CabbageSettings::CabbageSettings():
ApplicationProperties(), 
valueTree("Settings")
{
    valueTree.addListener(this);
}


void CabbageSettings::setDefaultColourSettings()
{

}

void CabbageSettings::setDefaultSettings()
{
    defaultPropSet = new PropertySet();
    ScopedPointer<XmlElement> xml;
    xml = new XmlElement("PLANTS");
    String homeDir = getCommonSettings(true)->getFile().getParentDirectory().getFullPathName();
    String manualPath;
#if !defined(MACOSX)
    manualPath = File::getSpecialLocation(File::currentExecutableFile).getParentDirectory().getFullPathName()+"/CsoundDocs";
#else
    manualPath = "/Library/Frameworks/CsoundLib64.framework/Versions/6.0/Resources/Manual";
#endif

	

    defaultPropSet->setValue("CsoundHelpDir", manualPath);
    defaultPropSet->setValue("CsoundHelpDir", manualPath);
    defaultPropSet->setValue("PlantFileDir", homeDir+"/Plants");
    defaultPropSet->setValue("ExamplesDir", homeDir+"/Examples/Examples");
    defaultPropSet->setValue("MostRecentDirectory", homeDir);
    defaultPropSet->setValue("DisablePluginInfo", 0);
    defaultPropSet->setValue("ShowEditorConsole", 1);
    defaultPropSet->setValue("ExternalEditor", 0);
    defaultPropSet->setValue("UseCabbageIO", 1);
    defaultPropSet->setValue("ShowConsoleWithEditor", 1);
    defaultPropSet->setValue("UsingCabbageCsound", 1);
    defaultPropSet->setValue("AudioEnabled", 1);
    defaultPropSet->setValue("DisableCompilerErrorWarning", 0);
    defaultPropSet->setValue("SetAlwaysOnTop", 1);
    defaultPropSet->setValue("GridSize", 4);
    defaultPropSet->setValue("PlantRepository", xml);
    defaultPropSet->setValue("EditorColourScheme", 0);
    defaultPropSet->setValue("showTabs", 1);
    defaultPropSet->setValue("EnablePopupDisplay", 1);
    defaultPropSet->setValue("ShowAutoComplete", 0);
    defaultPropSet->setValue("ShowNativeFileDialogues", 1);
    defaultPropSet->setValue("EnableNativePopup", 0);
    defaultPropSet->setValue("windowX", 100);
    defaultPropSet->setValue("windowY", 100);
	defaultPropSet->setValue("audioSetup", "");

	defaultPropSet->setValue("Colours"+CabbageColourIds::menuBarBackground,  "ff636363");
	defaultPropSet->setValue("Colours"+CabbageColourIds::menuBarText,  "FFFFFFFF");
	defaultPropSet->setValue("Colours"+CabbageColourIds::menuBarMouseOverBackground,  "ff9a9a9a");
	defaultPropSet->setValue("Colours"+CabbageColourIds::popupMenuBackground,  "fff5f5f5");
	defaultPropSet->setValue("Colours"+CabbageColourIds::popupMenuMouseOverBackground,  "ffa4a4a4");
	defaultPropSet->setValue("Colours"+CabbageColourIds::popupMenuText,  "ff484848");
	defaultPropSet->setValue("Colours"+CabbageColourIds::popupMenuHighlightedText,  "ff000000");
	defaultPropSet->setValue("Colours"+CabbageColourIds::codeBackground,  "FF222222");
	defaultPropSet->setValue("Colours"+CabbageColourIds::lineNumberBackground,  "44C1C1C1");
	defaultPropSet->setValue("Colours"+CabbageColourIds::lineNumbers,  "E9B2B2B2");
	defaultPropSet->setValue("Colours"+CabbageColourIds::plainText,  "FFCECECE");
	defaultPropSet->setValue("Colours"+CabbageColourIds::selectTextBackground,  "FF2859AC");
	defaultPropSet->setValue("Colours"+CabbageColourIds::caret,  "FFFFFFFF");
	defaultPropSet->setValue("Colours"+CabbageColourIds::preprocessor,  "FFF8F631");
	defaultPropSet->setValue("Colours"+CabbageColourIds::punctuation,  "FFCFBEFF");
	defaultPropSet->setValue("Colours"+CabbageColourIds::bracket,  "FF058202");
	defaultPropSet->setValue("Colours"+CabbageColourIds::stringLiteral,  "FFBC45DD");
	defaultPropSet->setValue("Colours"+CabbageColourIds::bracket,  "ff885500");
	defaultPropSet->setValue("Colours"+CabbageColourIds::integerLiteral,  "FF42C8C4");
	defaultPropSet->setValue("Colours"+CabbageColourIds::identifierLiteral,  "FFCFCFCF");
	defaultPropSet->setValue("Colours"+CabbageColourIds::operatorLiteral,  "FFC4EB19");
	defaultPropSet->setValue("Colours"+CabbageColourIds::keyword,  "FFEE6F6F");
	defaultPropSet->setValue("Colours"+CabbageColourIds::comment,  "FF72D20C");
	defaultPropSet->setValue("Colours"+CabbageColourIds::error,  "FFE60000");
	defaultPropSet->setValue("Colours"+CabbageColourIds::alertWindowBackground, Colour(147, 210,0).toString());

    getUserSettings()->setFallbackPropertySet(defaultPropSet);

	valueTree.addChild(ValueTree("Colours"), -1, 0);
	const StringArray colourIDStrings = CabbageStrings::getColourIDStrings();
	
	for(int i=0;i<colourIDStrings.size();i++)
	{
		valueTree.getChildWithName("Colours").setProperty(colourIDStrings[i],  
												getUserSettings()->getValue("Colours"+colourIDStrings[i]), 0);
	}
	
	
	audioSettingsXml = getUserSettings()->getValue("audioSetup").replace("<?xml version=\"1.0\" encoding=\"UTF-8\"?>", "");

}

void CabbageSettings::set(String child, Identifier identifier, var value)
{
	if(valueTree.getChildWithName(child).isValid())
		valueTree.getChildWithName(child).setProperty(identifier,  value, 0);
	else{
		valueTree.addChild(ValueTree(child), -1, 0);
		valueTree.getChildWithName(child).setProperty(identifier,  value, 0);
	}	
}

void CabbageSettings::setProperty(String child, var value)
{
	getUserSettings()->setValue(child, value);
}

String CabbageSettings::get(String child, String identifier)
{
    valueTree.getChildWithName(child).getProperty(identifier);
}

void CabbageSettings::set(ValueTree tree, String child, Identifier identifier, var value)
{
	CabbageUtilities::debug(value.toString());
	if(tree.getChildWithName(child).isValid())
		tree.getChildWithName(child).setProperty(identifier,  value.toString(), 0);
	else{
		tree.addChild(ValueTree(child), -1, 0);
		tree.getChildWithName(child).setProperty(identifier,  value.toString(), 0);
	}
}

String CabbageSettings::get(ValueTree tree, String child, String identifier)
{
	tree.getChildWithName(child).getProperty(identifier);	
}

Colour CabbageSettings::getColourFromValueTree(ValueTree tree, Identifier identifier, Colour defaultColour)
{
    const String colour = tree.getChildWithName("Colours").getProperty(identifier, defaultColour.toString()).toString();
    return Colour::fromString(colour);
}

String CabbageSettings::getColourPropertyName(ValueTree tree, int index)
{
    return tree.getChildWithName("Colours").getPropertyName(index).toString();
}

Colour CabbageSettings::getColourFromValueTree(ValueTree tree, int index, Colour defaultColour)
{
    if(index<=tree.getChildWithName("Colours").getNumProperties())
    {
        const String colour = tree.getChildWithName("Colours").getProperty(tree.getChildWithName("Colours").getPropertyName(index), defaultColour.toString()).toString();
        return Colour::fromString(colour);
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
	CabbageUtilities::debug(value.toString());
	if(valueTree.getChildWithName("Colours").getProperty(value).toString().isNotEmpty())
		getUserSettings()->setValue("Colours"+value.toString(), valueTree.getChildWithName("Colours").getProperty(value).toString());

	sendChangeMessage();
}
	
XmlElement* CabbageSettings::getXML(String identifier)
{
    return getUserSettings()->getXmlValue(identifier);
}

int CabbageSettings::getIndexOfProperty(String child, String identifier)
{
    return valueTree.indexOf(valueTree.getChildWithName(child));
}

void CabbageSettings::updateRecentFilesList(File file)
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

