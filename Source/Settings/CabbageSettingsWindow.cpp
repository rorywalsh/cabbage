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
	listBox(this),
    valueTree (settings.getValueTree()),
    audioSettingsButton ("AudioSettingsButton"),
    miscSettingsButton ("MiscSettingsButton"),
    colourSettingsButton ("ColourSettingsButton"),
	codeRepoButton ("CodeRepoButton"),
    audioDeviceSelector (audioDevice),
	viewport("AudioSettingsViewport")
{
    setLookAndFeel (&lookAndFeel);
    addColourProperties();
    addMiscProperties();
    addAndMakeVisible (colourPanel);
    addAndMakeVisible (miscPanel);
    colourPanel.setVisible (false);
    miscPanel.setVisible (false);
	//addAndMakeVisible(listBox);
	listBox.setVisible (false);
	addAndMakeVisible(codeEditor = new CodeEditorComponent(document, &csoundTokeniser));	
	codeEditor->setVisible(false);
	listBox.getLookAndFeel().setColour(ListBox::backgroundColourId, Colours::transparentWhite);
	updateColourScheme();
    addAndMakeVisible (audioSettingsButton);
    addAndMakeVisible (miscSettingsButton);
    addAndMakeVisible (colourSettingsButton);
	//addAndMakeVisible (codeRepoButton);
	viewport.setViewedComponent(audioDeviceSelector, false);

	audioDeviceSelector->setVisible(true);
	viewport.setScrollBarsShown(true, false);
	viewport.setViewPosition(0, 0);
	addAndMakeVisible(viewport);
    audioSettingsButton.addListener (this);
    audioSettingsButton.addMouseListener (this, false);
    miscSettingsButton.addListener (this);
    miscSettingsButton.addMouseListener (this, false);
    colourSettingsButton.addListener (this);
    colourSettingsButton.addMouseListener (this, false);
    codeRepoButton.addListener (this);
    codeRepoButton.addMouseListener (this, false);
	
    const Image audioSettingsImage = ImageCache::getFromMemory (CabbageBinaryData::AudioSettingsButton_png, CabbageBinaryData::AudioSettingsButton_pngSize);
    CabbageUtilities::setImagesForButton (&audioSettingsButton, audioSettingsImage);

    const Image miscSettingsImage = ImageCache::getFromMemory (CabbageBinaryData::MiscSettingsButton_png, CabbageBinaryData::MiscSettingsButton_pngSize);
    CabbageUtilities::setImagesForButton (&miscSettingsButton, miscSettingsImage);

    const Image colourSettingsImage = ImageCache::getFromMemory (CabbageBinaryData::ColourSettingsButton_png, CabbageBinaryData::ColourSettingsButton_pngSize);
    CabbageUtilities::setImagesForButton (&colourSettingsButton, colourSettingsImage);

    CabbageUtilities::setImagesForButton (&codeRepoButton, colourSettingsImage);
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


CabbageSettingsWindow::RepoListBox::RepoListBox(CabbageSettingsWindow* _owner):owner(_owner)
{
    addAndMakeVisible(listBox);
    listBox.setRowHeight (20);
    listBox.setModel (this);   // Tell the listbox where to get its data model
	
	StringArray customCodeSnippets;
	ScopedPointer<XmlElement> repoXml;
	XmlElement *newEntryXml, *newEntryXml1;
	
	repoXml = owner->settings.getUserSettings()->getXmlValue("CopeRepoXmlData");
	
	if(repoXml)
	{	
		for( int i = 0 ; i<repoXml->getNumAttributes() ; i++)
		{
			items.add(repoXml->getAttributeName(i));
			codeSnippets.add(repoXml->getAttributeValue(i));
		}
	}	
	
	listBox.updateContent();
}

CabbageSettingsWindow::RepoListBox::~RepoListBox()
{

}

void CabbageSettingsWindow::updateColourScheme()
{
    struct Type
    {
        const char* name;
        uint32 colour;
    };

    const Type types[] =
    {
        { "Error",              CabbageSettings::getColourFromValueTree (valueTree, CabbageColourIds::numbers, Colours::grey.darker()).getARGB() },
        { "Comment",            CabbageSettings::getColourFromValueTree (valueTree, CabbageColourIds::comment, Colours::grey.darker()).getARGB() },
        { "Keyword",            CabbageSettings::getColourFromValueTree (valueTree, CabbageColourIds::keyword, Colours::grey.darker()).getARGB() },
        { "Identifier",         CabbageSettings::getColourFromValueTree (valueTree, CabbageColourIds::identifierLiteral, Colours::grey.darker()).getARGB() },
        { "Integer",            Colours::grey.darker().getARGB() },
        { "Float",              Colours::grey.darker().getARGB() },
        { "String",             CabbageSettings::getColourFromValueTree (valueTree, CabbageColourIds::stringLiteral, Colours::grey.darker()).getARGB() },
        { "Operator",           Colours::grey.darker().getARGB() },
        { "Bracket",            Colours::grey.darker().getARGB() },
        { "Punctuation",        Colours::grey.darker().getARGB() },
        { "Preprocessor Text",  Colours::grey.darker().getARGB() },
        { "Csd Tag",            CabbageSettings::getColourFromValueTree (valueTree, CabbageColourIds::csdtags, Colours::grey.darker()).getARGB()}
    };

    CodeEditorComponent::ColourScheme cs;
    codeEditor->setColour (CodeEditorComponent::ColourIds::lineNumberBackgroundId, CabbageSettings::getColourFromValueTree (valueTree, CabbageColourIds::lineNumberBackground, Colour (70, 70, 70)));
    codeEditor->setColour (CodeEditorComponent::ColourIds::lineNumberTextId, CabbageSettings::getColourFromValueTree (valueTree, CabbageColourIds::lineNumbers, Colours::white));
    codeEditor->setColour (CodeEditorComponent::ColourIds::backgroundColourId, CabbageSettings::getColourFromValueTree (valueTree, CabbageColourIds::codeBackground, Colours::white));
    codeEditor->setColour (CaretComponent::ColourIds::caretColourId, CabbageSettings::getColourFromValueTree (valueTree, CabbageColourIds::caret, Colours::white));
    codeEditor->setColour (CodeEditorComponent::ColourIds::highlightColourId, CabbageSettings::getColourFromValueTree (valueTree, CabbageColourIds::selectTextBackground, Colours::white));

    for (std::size_t i = 0; i < sizeof (types) / sizeof (types[0]); ++i)  // (NB: numElementsInArray doesn't work here in GCC4.2)
        cs.set (types[i].name, Colour (types[i].colour));

    codeEditor->setColourScheme (cs);
}

void CabbageSettingsWindow::RepoListBox::paintListBoxItem (int rowNumber, Graphics& g,
                                       int width, int height, bool rowIsSelected)
{
    if (rowIsSelected)
        g.fillAll (Colours::black.withAlpha(.1f));
    else
        g.fillAll(Colours::white.withAlpha(.3f));//CabbageSettings::getColourFromValueTree (owner->valueTree, CabbageColourIds::codeBackground, Colours::white));

    g.setColour(Colour(20, 20, 20));
    g.drawFittedText(items[rowNumber], Rectangle<int> (width, height), Justification::left, 0);
}

void CabbageSettingsWindow::RepoListBox::listBoxItemDoubleClicked(int row, const MouseEvent &e)
{
	
}

void CabbageSettingsWindow::RepoListBox::listBoxItemClicked (int row, const MouseEvent &)
{
	owner->loadRepoCode(codeSnippets[row]);
}

void CabbageSettingsWindow::addCodeRepoProperties()
{
	
}

void CabbageSettingsWindow::loadRepoCode(String snippet)
{
	codeEditor->loadContent(snippet);
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
	codeRepoButton.setBounds (10, 220, 60, 60);

	if (audioDeviceSelector)
	{
		audioDeviceSelector->setBounds(0, 0, r.getWidth() - 120, 1000);
		viewport.setBounds(100, 30, r.getWidth() - 100, r.getHeight() - 30);
	}

    colourPanel.setBounds (100, 30, r.getWidth() - 100, r.getHeight() - 30);
    miscPanel.setBounds (100, 30, r.getWidth() - 100, r.getHeight() - 30);
	listBox.setBounds (100, 30, r.getWidth() - 120, listBox.getNumRows()*22);
	codeEditor->setBounds(100, 140, r.getWidth() - 120, 200);
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
    else if (viewport.isVisible())
        g.drawFittedText ("Audio and MIDI", 100, 10, r.getWidth() - 100, 20, Justification::centred, 1);
    else if (listBox.isVisible())
        g.drawFittedText ("Code Repository", 100, 10, r.getWidth() - 100, 20, Justification::centred, 1);
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
        viewport.setVisible (true);
        colourPanel.setVisible (false);
        miscPanel.setVisible (false);
		listBox.setVisible(false);
		codeEditor->setVisible(false);
    }
    else if (button == "ColourSettingsButton")
    {
		viewport.setVisible (false);
        colourPanel.setVisible (true);
        miscPanel.setVisible (false);
		listBox.setVisible(false);
		codeEditor->setVisible(false);
    }
    else if (button == "MiscSettingsButton")
    {
		viewport.setVisible (false);
        colourPanel.setVisible (false);
        miscPanel.setVisible (true);
		listBox.setVisible(false);
		codeEditor->setVisible(false);
    }
    else if (button == "CodeRepoButton")
    {
		viewport.setVisible (false);
        colourPanel.setVisible (false);
        miscPanel.setVisible (false);
		listBox.setVisible(true);	
		codeEditor->setVisible(true);
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