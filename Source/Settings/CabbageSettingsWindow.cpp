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

#include "CabbageSettingsWindow.h"
//==============================================================================
static void addCustomListener (Array<PropertyComponent*> comps, CabbageSettingsWindow* owner)
{
    for ( int i = 0; i < comps.size(); i++)
    {
        if (TextPropertyComponent* textProperty = dynamic_cast<TextPropertyComponent*> (comps[i]))
        {
            textProperty->addListener (owner);
        }
        else if (ColourPropertyComponent* colourProperty = dynamic_cast<ColourPropertyComponent*> (comps[i]))
        {
            colourProperty->addChangeListener (owner);
        }
        else if (CabbageFilePropertyComponent* fileComp = dynamic_cast<CabbageFilePropertyComponent*> (comps[i]))
        {
            fileComp->filenameComp.addListener (owner);
        }
    }
}

CabbageSettingsWindow::CabbageSettingsWindow (CabbageSettings& settings, AudioDeviceSelectorComponent* audioDevice):
    Component (""),
    settings (settings),
    valueTree (settings.getValueTree()),
    audioSettingsButton ("AudioSettingsButton"),
    miscSettingsButton ("MiscSettingsButton"),
    colourSettingsButton ("ColourSettingsButton"),
    audioDeviceSelector (audioDevice)
{
    setLookAndFeel (&lookAndFeel);
    addColourProperties();
    addMiscProperties();
    addAndMakeVisible (colourPanel);
    addAndMakeVisible (miscPanel);
    colourPanel.setVisible (false);
    miscPanel.setVisible (false);
    addAndMakeVisible (audioSettingsButton);
    addAndMakeVisible (miscSettingsButton);
    addAndMakeVisible (colourSettingsButton);
    addAndMakeVisible (audioDeviceSelector);
    audioSettingsButton.addListener (this);
    audioSettingsButton.addMouseListener (this, false);
    miscSettingsButton.addListener (this);
    miscSettingsButton.addMouseListener (this, false);
    colourSettingsButton.addListener (this);
    colourSettingsButton.addMouseListener (this, false);

    const Image audioSettingsImage = ImageCache::getFromMemory (CabbageBinaryData::AudioSettingsButton_png, CabbageBinaryData::AudioSettingsButton_pngSize);
    CabbageUtilities::setImagesForButton (&audioSettingsButton, audioSettingsImage);

    const Image miscSettingsImage = ImageCache::getFromMemory (CabbageBinaryData::MiscSettingsButton_png, CabbageBinaryData::MiscSettingsButton_pngSize);
    CabbageUtilities::setImagesForButton (&miscSettingsButton, miscSettingsImage);

    const Image colourSettingsImage = ImageCache::getFromMemory (CabbageBinaryData::ColourSettingsButton_png, CabbageBinaryData::ColourSettingsButton_pngSize);
    CabbageUtilities::setImagesForButton (&colourSettingsButton, colourSettingsImage);

}

void CabbageSettingsWindow::addColourProperties()
{
    Array<PropertyComponent*> editorProps, interfaceProps, consoleProps;

    for (int index = 0; index < valueTree.getChildWithName ("Colours").getNumProperties(); ++index)
    {
        const String name = CabbageSettings::getColourPropertyName (valueTree, index);
        const Colour colour = CabbageSettings::getColourFromValueTree (valueTree, index, Colours::red);

        if (name.contains ("Editor -"))
            editorProps.add (new ColourPropertyComponent (name, colour.toString(), true));
        else if (name.contains ("Console -"))
            consoleProps.add (new ColourPropertyComponent (name, colour.toString(), true));
        else if (name.contains ("Interface -"))
            interfaceProps.add (new ColourPropertyComponent (name, colour.toString(), true ));
    }

    colourPanel.clear();
    addCustomListener (interfaceProps, this);
    addCustomListener (editorProps, this);
    addCustomListener (consoleProps, this);

    colourPanel.addSection ("Interface", interfaceProps);
    colourPanel.addSection ("Editor", editorProps);
    colourPanel.addSection ("Console", consoleProps);

}

void CabbageSettingsWindow::addMiscProperties()
{
    Array<PropertyComponent*> editorProps;
    Array<PropertyComponent*> dirProps;
    Array<PropertyComponent*> sshProps;

    showLastOpenedFileValue.setValue (settings.getUserSettings()->getIntValue ("OpenMostRecentFileOnStartup"));
    showLastOpenedFileValue.addListener (this);
    alwaysOnTopPluginValue.setValue (settings.getUserSettings()->getIntValue ("SetAlwaysOnTopPlugin"));
    alwaysOnTopPluginValue.addListener (this);
    alwaysOnTopGraphValue.setValue (settings.getUserSettings()->getIntValue ("SetAlwaysOnTopGraph"));
    alwaysOnTopGraphValue.addListener (this);
    compileOnSaveValue.setValue (settings.getUserSettings()->getIntValue ("CompileOnSave"));
    compileOnSaveValue.addListener (this);

    autoCompleteValue.setValue (settings.getUserSettings()->getIntValue ("DisableAutoComplete"));
    autoCompleteValue.addListener (this);

    editorProps.add (new BooleanPropertyComponent (showLastOpenedFileValue, "Auto-load", "Auto-load last opened file"));
    editorProps.add (new BooleanPropertyComponent (alwaysOnTopPluginValue, "Plugin Window", "Always show plugin on top"));
    editorProps.add (new BooleanPropertyComponent (alwaysOnTopGraphValue, "Graph Window", "Always show graph on top"));
    editorProps.add (new BooleanPropertyComponent (compileOnSaveValue, "Compiling", "Compile on save"));
    editorProps.add (new BooleanPropertyComponent (autoCompleteValue, "Auto-complete", "Show auto complete popup"));

    const int scrollBy = settings.getUserSettings()->getIntValue ("numberOfLinesToScroll");
    editorProps.add (new TextPropertyComponent (Value (scrollBy), "Editor lines to scroll with MouseWheel", 10, false));

    const String examplesDir = settings.getUserSettings()->getValue ("CabbageExamplesDir");
    const String manualDir = settings.getUserSettings()->getValue ("CsoundManualDir");
    const String cabbageManualDir = settings.getUserSettings()->getValue ("CabbageManualDir");
    const String plantDir = settings.getUserSettings()->getValue ("CabbagePlantDir");
    const String userFilesDir = settings.getUserSettings()->getValue ("UserFilesDir");

    dirProps.add (new CabbageFilePropertyComponent ("Csound manual dir.", true, false,  "*", manualDir));
    dirProps.add (new CabbageFilePropertyComponent ("Cabbage manual dir.", true, false,  "*", cabbageManualDir));
    dirProps.add (new CabbageFilePropertyComponent ("Cabbage examples dir.", true, false, "*", examplesDir));
    dirProps.add (new CabbageFilePropertyComponent ("Cabbage plants dir.", true, false, "*", plantDir));
    dirProps.add (new CabbageFilePropertyComponent ("User files dir.", true, false, "*", userFilesDir));

    const String sshAddress = settings.getUserSettings()->getValue ("SSHAddress");
    sshProps.add (new TextPropertyComponent (Value (sshAddress), "SSH Address", 200, false));

    const String sshHomeDir = settings.getUserSettings()->getValue ("SSHHomeDir");
    sshProps.add (new TextPropertyComponent (Value (sshHomeDir), "SSH Home Directory", 200, false));


    addCustomListener (editorProps, this);
    addCustomListener (sshProps, this);
    addCustomListener (dirProps, this);
    miscPanel.clear();
    miscPanel.addSection ("Editor", editorProps);
    miscPanel.addSection ("Directories", dirProps);
    miscPanel.addSection ("SSH", sshProps);
}

void CabbageSettingsWindow::textPropertyComponentChanged (TextPropertyComponent* comp)
{
    if (comp->getName() == "SSH Address")
        settings.getUserSettings()->setValue ("SSHAddress", comp->getValue().toString());
    else if (comp->getName() == "SSH Home Directory")
        settings.getUserSettings()->setValue ("SSHHomeDir", comp->getValue().toString());
    else if (comp->getName() == "Editor lines to scroll with MouseWheel")
        settings.getUserSettings()->setValue ("numberOfLinesToScroll", comp->getValue().toString());
}

void CabbageSettingsWindow::resized()
{
    Rectangle<int> r (getLocalBounds());
    audioSettingsButton.setBounds (10, 10, 60, 60);
    miscSettingsButton.setBounds (10, 80, 60, 60);
    colourSettingsButton.setBounds (10, 150, 60, 60);

    if (audioDeviceSelector)
        audioDeviceSelector->setBounds (100, 30, r.getWidth() - 100, r.getHeight() - 30);

    colourPanel.setBounds (100, 30, r.getWidth() - 100, r.getHeight() - 30);
    miscPanel.setBounds (100, 30, r.getWidth() - 100, r.getHeight() - 30);

}

void CabbageSettingsWindow::paint (Graphics& g)
{
    Rectangle<int> r (getLocalBounds());
    g.fillAll (Colour (147, 210, 0));
    g.setColour (Colours::black.withAlpha (.1f));
    g.fillRect (r.withLeft (90));
    g.setFont (Font (18, 1));
    g.setColour (Colours::black);

    if (miscPanel.isVisible())
        g.drawFittedText ("Miscellaneous", 100, 10, r.getWidth() - 100, 20, Justification::centred, 1);
    else if (colourPanel.isVisible())
        g.drawFittedText ("Colours", 100, 10, r.getWidth() - 100, 20, Justification::centred, 1);
    else if (audioDeviceSelector->isVisible())
        g.drawFittedText ("Audio and MIDI", 100, 10, r.getWidth() - 100, 20, Justification::centred, 1);
}
//=====================================================================
void CabbageSettingsWindow::valueChanged (Value& value)
{
    if (value.refersToSameSourceAs (showLastOpenedFileValue))
        settings.getUserSettings()->setValue ("OpenMostRecentFileOnStartup", value.getValue().toString());
    else if (value.refersToSameSourceAs (alwaysOnTopPluginValue))
        settings.getUserSettings()->setValue ("SetAlwaysOnTopPlugin", value.getValue().toString());
    else if (value.refersToSameSourceAs (alwaysOnTopGraphValue))
        settings.getUserSettings()->setValue ("SetAlwaysOnTopGraph", value.getValue().toString());
    else if (value.refersToSameSourceAs (compileOnSaveValue))
        settings.getUserSettings()->setValue ("CompileOnSave", value.getValue().toString());
    else if (value.refersToSameSourceAs (breakLinesValue))
        settings.getUserSettings()->setValue ("IdentifiersBeforeLineBreak", value.getValue().toString());
    else if (value.refersToSameSourceAs (autoCompleteValue))
        settings.getUserSettings()->setValue ("DisableAutoComplete", value.getValue().toString());
}

void CabbageSettingsWindow::filenameComponentChanged (FilenameComponent* fileComponent)
{

    if (fileComponent->getName() == "Csound manual dir.")
        settings.getUserSettings()->setValue ("CsoundManualDir", fileComponent->getCurrentFileText());
    else if (fileComponent->getName() == "Cabbage manual dir.")
        settings.getUserSettings()->setValue ("CabbageManualDir", fileComponent->getCurrentFileText());
    else if (fileComponent->getName() == "Cabbage plants dir.")
        settings.getUserSettings()->setValue ("CabbagePlantDir", fileComponent->getCurrentFileText());
    else if (fileComponent->getName() == "Cabbage examples dir.")
        settings.getUserSettings()->setValue ("CabbageExamplesDir", fileComponent->getCurrentFileText());
    else if (fileComponent->getName() == "User files dir.")
        settings.getUserSettings()->setValue ("UserFilesDir", fileComponent->getCurrentFileText());
}

void CabbageSettingsWindow::selectPanel (String button)
{
    if (button == "AudioSettingsButton")
    {
        audioDeviceSelector->setVisible (true);
        colourPanel.setVisible (false);
        miscPanel.setVisible (false);
    }
    else if (button == "ColourSettingsButton")
    {
        audioDeviceSelector->setVisible (false);
        colourPanel.setVisible (true);
        miscPanel.setVisible (false);
    }
    else if (button == "MiscSettingsButton")
    {
        audioDeviceSelector->setVisible (false);
        colourPanel.setVisible (false);
        miscPanel.setVisible (true);
    }

    repaint();
}

void CabbageSettingsWindow::buttonClicked (Button* button)
{
    selectPanel (button->getName());

}

void CabbageSettingsWindow::mouseEnter (const MouseEvent& e)
{
    selectPanel (e.eventComponent->getName());
}


void CabbageSettingsWindow::changeListenerCallback (ChangeBroadcaster* source)
{
    if (ColourPropertyComponent* colourProperty = dynamic_cast<ColourPropertyComponent*> (source))
    {
        CabbageSettings::set (settings.getValueTree(), "Colours", colourProperty->getName(), colourProperty->getCurrentColourString());
    }
}