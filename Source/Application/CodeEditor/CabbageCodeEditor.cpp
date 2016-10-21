/*
  ==============================================================================

    CabbageCodeEditor.cpp
    Created: 16 Oct 2016 2:59:39pm
    Author:  rory

  ==============================================================================
*/

#include "CabbageCodeEditor.h"
#include "../Settings/CabbageSettings.h"

//==============================================================================
CabbageCodeEditorComponent::CabbageCodeEditorComponent(ValueTree valueTree, CodeDocument &document, CodeTokeniser *codeTokeniser)
    : CodeEditorComponent(document, codeTokeniser), valueTree(valueTree)
{
	this->setColour(CodeEditorComponent::ColourIds::lineNumberBackgroundId, CabbageSettings::getColourFromValueTree(valueTree, CabbageColourIds::lineNumberBackground, Colour(70,70,70)));
	this->setColour(CodeEditorComponent::ColourIds::lineNumberTextId, CabbageSettings::getColourFromValueTree(valueTree, CabbageColourIds::lineNumbers, Colours::white));
}

void CabbageCodeEditorComponent::updateColourScheme()
{
	struct Type
	{
		const char* name;
		uint32 colour;
	};

	const Type types[] =
	{
		{ "Error",              CabbageSettings::getColourFromValueTree(valueTree, CabbageColourIds::numbers, Colours::grey.darker()).getARGB() },
		{ "Comment",            CabbageSettings::getColourFromValueTree(valueTree, CabbageColourIds::comment, Colours::grey.darker()).getARGB() },
		{ "Keyword",            CabbageSettings::getColourFromValueTree(valueTree, CabbageColourIds::keyword, Colours::grey.darker()).getARGB() },
		{ "Identifier",         CabbageSettings::getColourFromValueTree(valueTree, CabbageColourIds::identifierLiteral, Colours::grey.darker()).getARGB() },
		{ "Integer",            Colours::grey.darker().getARGB() },
		{ "Float",              Colours::grey.darker().getARGB() },
		{ "String",             Colours::grey.darker().getARGB() },
		{ "Operator",           CabbageSettings::getColourFromValueTree(valueTree, CabbageColourIds::operatorLiteral, Colours::grey.darker()).getARGB() },
		{ "Bracket",            Colours::grey.darker().getARGB() },
		{ "Punctuation",        Colours::grey.darker().getARGB() },
		{ "Preprocessor Text",  Colours::grey.darker().getARGB() },
		{ "Csd Tag",			CabbageSettings::getColourFromValueTree(valueTree, CabbageColourIds::csdtags, Colours::grey.darker()).getARGB()}
	};

	CodeEditorComponent::ColourScheme cs;

	setColour(CodeEditorComponent::ColourIds::lineNumberBackgroundId, CabbageSettings::getColourFromValueTree(valueTree, CabbageColourIds::lineNumberBackground, Colour(70,70,70)));
	setColour(CodeEditorComponent::ColourIds::lineNumberTextId, CabbageSettings::getColourFromValueTree(valueTree, CabbageColourIds::lineNumbers, Colours::white));
	setColour(CodeEditorComponent::ColourIds::backgroundColourId, CabbageSettings::getColourFromValueTree(valueTree, CabbageColourIds::codeBackground, Colours::white));
	setColour(CaretComponent::ColourIds::caretColourId, CabbageSettings::getColourFromValueTree(valueTree, CabbageColourIds::caret, Colours::white));
	setColour(CodeEditorComponent::ColourIds::highlightColourId, CabbageSettings::getColourFromValueTree(valueTree, CabbageColourIds::selectTextBackground, Colours::white));

	for (int i = 0; i < sizeof (types) / sizeof (types[0]); ++i)  // (NB: numElementsInArray doesn't work here in GCC4.2)
		cs.set (types[i].name, Colour (types[i].colour));

	this->setColourScheme(cs);
}