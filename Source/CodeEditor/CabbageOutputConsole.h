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
#ifndef CABBAGEOUTPUCONSOLECOMPONENT_H_INCLUDED
#define CABBAGEOUTPUCONSOLECOMPONENT_H_INCLUDED

#include "../CabbageIds.h"
#include "../Settings/CabbageSettings.h"
#include "CsoundTokeniser.h"

class CabbageOutputConsole : public Component, public KeyListener
{
    std::unique_ptr<CodeEditorComponent> textEditor;
    int fontSize;
    Typeface::Ptr fontPtr;
public:
    CabbageOutputConsole (ValueTree valueTree, CodeDocument& document): Component(), value (valueTree)
    {
        textEditor.reset (new CodeEditorComponent(document, &consoleTokeniser));
        addAndMakeVisible (textEditor.get(), true);
        textEditor->setColour (Label::outlineColourId, Colours::white);
        textEditor->setColour (CodeEditorComponent::ColourIds::defaultTextColourId, CabbageSettings::getColourFromValueTree (valueTree, CabbageColourIds::consoleText, Colour(100, 100, 100)));
        textEditor->setColour (CodeEditorComponent::ColourIds::backgroundColourId, CabbageSettings::getColourFromValueTree (valueTree, CabbageColourIds::consoleBackground, Colours::grey.darker()));
        textEditor->setColour (CodeEditorComponent::ColourIds::highlightColourId, CabbageSettings::getColourFromValueTree (valueTree, CabbageColourIds::selectTextBackground, Colours::grey));
        //textEditor->setReadOnly (true);
        textEditor->setLineNumbersShown(false);
        fontPtr = Typeface::createSystemTypefaceFor (CabbageBinaryData::DejaVuSansMonoBold_ttf,  CabbageBinaryData::DejaVuSansMonoBold_ttfSize);
        textEditor->setFont (Font (fontPtr).withHeight (12));
        textEditor->addKeyListener(this);
        setText ("Csound output message console\n");
        updateColourScheme();


    }

    ~CabbageOutputConsole() 
    {
        textEditor->getDocument().replaceAllContent("");
        setLookAndFeel(nullptr);
        DBG("cleaing");
    }

    void setText (String text)
    {
        const MessageManagerLock lock;
        String currentText = textEditor->getDocument().getAllContent();
        textEditor->getDocument().replaceAllContent(currentText + text);

        CodeDocument::Position endPos(textEditor->getDocument(), textEditor->getDocument().getAllContent().length());
        textEditor->moveCaretTo(endPos, false);
       
    }

    String getText()
    {
        const MessageManagerLock lock;
        return textEditor->getDocument().getAllContent();
    }

    void updateColourScheme()
    {
#ifndef CabbageLite
        struct Type
        {
            const char* name;
            uint32 colour;
        };

        const Type types[] =
        {
            { "Error",              Colours::yellow.getARGB() },
            { "Comment",            Colours::yellow.getARGB() },
            { "Keyword",            Colours::grey.getARGB() },
            { "Identifier",         Colours::grey.getARGB() },
            { "Integer",            Colours::cornflowerblue.getARGB() },
            { "Float",              Colours::cornflowerblue.getARGB() },
            { "String",             Colours::red.getARGB() }
        };


        CodeEditorComponent::ColourScheme cs;

        for (std::size_t i = 0;
            i < sizeof(types) / sizeof(types[0]); ++i) // (NB: numElementsInArray doesn't work here in GCC4.2)
            cs.set(types[i].name, Colour(types[i].colour));

        textEditor->setColourScheme(cs);

        textEditor->setColour (TextEditor::textColourId, CabbageSettings::getColourFromValueTree (value, CabbageColourIds::consoleText, Colours::grey.darker()));
        textEditor->setColour (TextEditor::backgroundColourId, CabbageSettings::getColourFromValueTree (value, CabbageColourIds::consoleBackground, Colours::grey.darker()));
        textEditor->setColour (ScrollBar::ColourIds::thumbColourId,
            CabbageSettings::getColourFromValueTree (value, CabbageColourIds::consoleBackground, Colours::white).contrasting (.1f));
        repaint();
#else
        textEditor->setColour (TextEditor::textColourId, Colours::green);
        textEditor->setColour (TextEditor::backgroundColourId, Colour (20, 20, 20));
        repaint();
#endif
    }

    void setFontSize (int size)
    {
        textEditor->setFont (Font (fontPtr).withHeight (size));
        fontSize = size;
    }

    int getFontSize()
    {
        return fontSize;
    }

    void zoom (bool in)
    {
        if (in == true)
            fontSize++;
        else
            fontSize = fontSize > 6 ? fontSize - 1 : fontSize;

        String currentText = textEditor->getDocument().getAllContent();
        textEditor->getDocument().replaceAllContent("");
        textEditor->setFont (Font (fontPtr).withHeight (fontSize));
        textEditor->getDocument().replaceAllContent(currentText);
        CodeDocument::Position endPos(textEditor->getDocument(), currentText.length());
        textEditor->moveCaretTo(endPos, false);

    }

    void clearText()
    {
        textEditor->getDocument().replaceAllContent("");
    }

    void resized() override
    {
        Rectangle<int> area (getLocalBounds());
        textEditor->setBounds (area.reduced (2).withY (0));
    }

    bool keyPressed(const juce::KeyPress&, juce::Component*) override
    {
        return false;
    }

    void paint (Graphics& g)  override
    {
#ifndef CabbageLite
        g.fillAll (CabbageSettings::getColourFromValueTree (value, CabbageColourIds::consoleOutline, Colours::grey.darker()));
#endif
    }

private:
    // this is the document that the editor component is showing
    // this is a tokeniser to apply the C++ syntax highlighting
    CsoundTokeniser consoleTokeniser;

    ValueTree value;

};



#endif  // CABBAGEOUTPUCONSOLECOMPONENT_H_INCLUDED
