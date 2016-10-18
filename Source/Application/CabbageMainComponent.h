/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "CabbageCodeEditor.h"
#include "CabbageOutputConsoleComponent.h"
#include "../Utilities/CabbageUtilities.h"


//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainContentComponent   : public Component
{
public:
    //==============================================================================
    MainContentComponent(ValueTree settings);
    ~MainContentComponent();

    void paint (Graphics&) override;
    void resized() override;
	Image createBackground();
	
	void openFile(File file);

    ScopedPointer<CabbageCodeEditorComponent> editor;
    ScopedPointer<CabbageOutputConsoleComponent> outputConsole;
    CodeDocument csoundDocument;
    CsoundTokeniser csoundTokeniser;

private:
	Image bgImage;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


#endif  // MAINCOMPONENT_H_INCLUDED
