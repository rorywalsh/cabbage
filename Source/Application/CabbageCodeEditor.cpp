/*
  ==============================================================================

    CabbageCodeEditor.cpp
    Created: 16 Oct 2016 2:59:39pm
    Author:  rory

  ==============================================================================
*/

#include "CabbageCodeEditor.h"
#include "CabbageSettings.h"

//==============================================================================
CabbageCodeEditorComponent::CabbageCodeEditorComponent(ValueTree valueTree, CodeDocument &document, CodeTokeniser *codeTokeniser)
    : CodeEditorComponent(document, codeTokeniser), valueTree(valueTree)
{
	this->setColour(CodeEditorComponent::ColourIds::lineNumberBackgroundId, CabbageSettings::getColourFromValueTree(valueTree, CabbageColourIds::lineNumberBackground, Colour(70,70,70)));
	this->setColour(CodeEditorComponent::ColourIds::lineNumberTextId, CabbageSettings::getColourFromValueTree(valueTree, CabbageColourIds::lineNumbers, Colours::white));
}