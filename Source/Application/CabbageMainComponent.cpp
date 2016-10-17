/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "CabbageMainComponent.h"


//==============================================================================
MainContentComponent::MainContentComponent(ValueTree settings)
{
    addAndMakeVisible(editor = new CabbageCodeEditorComponent(csoundDocument, &csoundTokeniser));
    addAndMakeVisible(outputConsole = new CabbageOutputConsoleComponent());
    editor->setLineNumbersShown(true);
    editor->setFont(Font(14));
    setSize (1200, 800);
}

MainContentComponent::~MainContentComponent()
{
    editor = nullptr;
    outputConsole = nullptr;

}

void MainContentComponent::paint (Graphics& g)
{
    g.fillAll (Colour (0xff001F36));

    g.setFont (Font (16.0f));
    g.setColour (Colours::white);
    g.drawText ("Hello World!", getLocalBounds(), Justification::centred, true);
}

void MainContentComponent::resized()
{
    Rectangle<int> area (getLocalBounds());
    outputConsole->setBounds(area.removeFromBottom(getHeight()*.15));
    editor->setBounds(area);

    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
}
