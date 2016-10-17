/*
  ==============================================================================

    CabbageSettings.cpp
    Created: 12 Oct 2016 12:12:34pm
    Author:  rory

  ==============================================================================
*/

#include "CabbageSettings.h"

//==============================================================================

CabbageSettings::CabbageSettings():ApplicationProperties(), valueTree("Settings")
{
    valueTree.addListener(this);
}


void CabbageSettings::setDefaultColourSettings()
{

}

void CabbageSettings::setDefaultSettings()
{
    ScopedPointer<PropertySet> defaultPropSet = new PropertySet();
    ScopedPointer<XmlElement> xml;
    xml = new XmlElement("PLANTS");
    String homeDir = getCommonSettings(true)->getFile().getParentDirectory().getFullPathName();
    String manualPath;
#if !defined(MACOSX)
    manualPath = File::getSpecialLocation(File::currentExecutableFile).getParentDirectory().getFullPathName()+"/CsoundDocs";
#else
    manualPath = "/Library/Frameworks/CsoundLib64.framework/Versions/6.0/Resources/Manual";
#endif
    ValueTree defaultValueTree("Settings");
    defaultValueTree.addChild(ValueTree("Misc"), -1, 0);
    defaultValueTree.getChildWithName("Misc").setProperty("CsoundHelpDir", manualPath, 0);
    defaultValueTree.getChildWithName("Misc").setProperty("CsoundHelpDir", manualPath, 0);
    defaultValueTree.getChildWithName("Misc").setProperty("PlantFileDir", homeDir+"/Plants", 0);
    defaultValueTree.getChildWithName("Misc").setProperty("ExamplesDir", homeDir+"/Examples/Examples", 0);
    defaultValueTree.getChildWithName("Misc").setProperty("MostRecentDirectory", homeDir, 0);
    defaultValueTree.getChildWithName("Misc").setProperty("DisablePluginInfo", 0, 0);
    defaultValueTree.getChildWithName("Misc").setProperty("ShowEditorConsole", 1, 0);
    defaultValueTree.getChildWithName("Misc").setProperty("ExternalEditor", 0, 0);
    defaultValueTree.getChildWithName("Misc").setProperty("UseCabbageIO", 1, 0);
    defaultValueTree.getChildWithName("Misc").setProperty("ShowConsoleWithEditor", 1, 0);
    defaultValueTree.getChildWithName("Misc").setProperty("UsingCabbageCsound", 1, 0);
    defaultValueTree.getChildWithName("Misc").setProperty("AudioEnabled", 1, 0);
    defaultValueTree.getChildWithName("Misc").setProperty("DisableCompilerErrorWarning", 0, 0);
    defaultValueTree.getChildWithName("Misc").setProperty("SetAlwaysOnTop", 1, 0);
    defaultValueTree.getChildWithName("Misc").setProperty("GridSize", 4, 0);
    //defaultValueTree.getChildWithName("Misc").setProperty("PlantRepository", xml, 0);
    defaultValueTree.getChildWithName("Misc").setProperty("EditorColourScheme", 0, 0);
    defaultValueTree.getChildWithName("Misc").setProperty("showTabs", 1, 0);
    defaultValueTree.getChildWithName("Misc").setProperty("EnablePopupDisplay", 1, 0);
    defaultValueTree.getChildWithName("Misc").setProperty("ShowAutoComplete", 0, 0);
    defaultValueTree.getChildWithName("Misc").setProperty("ShowNativeFileDialogues", 1, 0);
    defaultValueTree.getChildWithName("Misc").setProperty("EnableNativePopup", 0, 0);
    defaultValueTree.getChildWithName("Misc").setProperty("windowX", 100, 0);
    defaultValueTree.getChildWithName("Misc").setProperty("windowY", 100, 0);

    defaultValueTree.addChild(ValueTree("Colours"), -1, 0);
    defaultValueTree.getChildWithName("Colours").setProperty(ColourIds::menuBarBackground,  "FF29292A", 0);
    defaultValueTree.getChildWithName("Colours").setProperty(ColourIds::menuBarText,  "FFFFFFFF", 0);
    defaultValueTree.getChildWithName("Colours").setProperty(ColourIds::menuBarBackground,  "FF29292A", 0);
    defaultValueTree.getChildWithName("Colours").setProperty(ColourIds::menuBarMouseOverBackground,  "FF29292A", 0);
    defaultValueTree.getChildWithName("Colours").setProperty(ColourIds::popupMenuBackground,  "FFFFFFFF", 0);
    defaultValueTree.getChildWithName("Colours").setProperty(ColourIds::popupMenuMouseOverBackground,  "FFFFFFFF", 0);
    defaultValueTree.getChildWithName("Colours").setProperty(ColourIds::popupMenuText,  "FFFF0000", 0);
    defaultValueTree.getChildWithName("Colours").setProperty(ColourIds::popupMenuHighlightedText,  "FFFFFF00", 0);
    defaultValueTree.getChildWithName("Colours").setProperty(ColourIds::codeBackground,  "FF222222", 0);
    defaultValueTree.getChildWithName("Colours").setProperty(ColourIds::lineNumberBackground,  "44C1C1C1", 0);
    defaultValueTree.getChildWithName("Colours").setProperty(ColourIds::lineNumbers,  "E9B2B2B2", 0);
    defaultValueTree.getChildWithName("Colours").setProperty(ColourIds::plainText,  "FFCECECE", 0);
    defaultValueTree.getChildWithName("Colours").setProperty(ColourIds::selectTextBackground,  "FF2859AC", 0);
    defaultValueTree.getChildWithName("Colours").setProperty(ColourIds::caret,  "FFFFFFFF", 0);
    defaultValueTree.getChildWithName("Colours").setProperty(ColourIds::preprocessor,  "FFF8F631", 0);
    defaultValueTree.getChildWithName("Colours").setProperty(ColourIds::punctuation,  "FFCFBEFF", 0);
    defaultValueTree.getChildWithName("Colours").setProperty(ColourIds::bracket,  "FF058202", 0);
    defaultValueTree.getChildWithName("Colours").setProperty(ColourIds::stringLiteral,  "FFBC45DD", 0);
    defaultValueTree.getChildWithName("Colours").setProperty(ColourIds::bracket,  "ff885500", 0);
    defaultValueTree.getChildWithName("Colours").setProperty(ColourIds::integerLiteral,  "FF42C8C4", 0);
    defaultValueTree.getChildWithName("Colours").setProperty(ColourIds::identifierLiteral,  "FFCFCFCF", 0);
    defaultValueTree.getChildWithName("Colours").setProperty(ColourIds::operatorLiteral,  "FFC4EB19", 0);
    defaultValueTree.getChildWithName("Colours").setProperty(ColourIds::keyword,  "FFEE6F6F", 0);
    defaultValueTree.getChildWithName("Colours").setProperty(ColourIds::comment,  "FF72D20C", 0);
    defaultValueTree.getChildWithName("Colours").setProperty(ColourIds::error,  "FFE60000", 0);


    //write properties to settings file..
    ScopedPointer<XmlElement> data (defaultValueTree.createXml());
    getUserSettings()->setValue ("PROJECT_DEFAULT_SETTINGS", data);
    //write new xml settings files based on data from user settings file, but using ValueTree
    ScopedPointer<XmlElement> xmlData = getUserSettings()->createXml("Colours");
    ValueTree newValueTree = ValueTree::fromXml(*xmlData);
    XmlElement * el = newValueTree.createXml();
    el->writeToFile(File("/home/rory/Desktop/NewSettings.xml"), String::empty);
    delete el;


    //if(getUserSettings()->getValue("PROJECT_DEFAULT_SETTINGS", "").isEmpty())

    //else
    //{

    //}

    //defaultPropSet->setValue("PROJECT_DEFAULT_SETTINGS", data);
    //getUserSettings()->setFallbackPropertySet(defaultPropSet);
    //XmlElement* xmlData = getUserSettings()->createXml("Settings");
    //valueTree.fromXml(*xmlData);



}

void CabbageSettings::set(String child, Identifier identifier, var value)
{
    valueTree.getChildWithName(child).setProperty(identifier,  value, 0);
}

String CabbageSettings::get(String child, String identifier)
{
    valueTree.getChildWithName(child).getProperty(identifier);
}

void CabbageSettings::set(ValueTree tree, String child, Identifier identifier, var value)
{
    tree.getChildWithName(child).setProperty(identifier,  value, 0);
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

XmlElement* CabbageSettings::getXML(String identifier)
{
    return getUserSettings()->getXmlValue(identifier);
}

int CabbageSettings::getIndexOfProperty(String child, String identifier)
{
    return valueTree.indexOf(valueTree.getChildWithName(child));
}

