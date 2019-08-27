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

class CabbageOutputConsole : public Component
{
    ScopedPointer<TextEditor> textEditor;
    int fontSize;
    Typeface::Ptr fontPtr;
public:
    CabbageOutputConsole (ValueTree valueTree): Component(), value (valueTree)
    {
        addAndMakeVisible (textEditor = new TextEditor(), true);
        textEditor->setColour (Label::outlineColourId, Colours::white);
        textEditor->setColour (TextEditor::textColourId, CabbageSettings::getColourFromValueTree (valueTree, CabbageColourIds::consoleText, Colours::grey.darker()));
        textEditor->setColour (TextEditor::backgroundColourId, CabbageSettings::getColourFromValueTree (valueTree, CabbageColourIds::consoleBackground, Colours::grey.darker()));
        textEditor->setColour (TextEditor::highlightColourId, CabbageSettings::getColourFromValueTree (valueTree, CabbageColourIds::selectTextBackground, Colours::grey));
        textEditor->setColour (TextEditor::highlightedTextColourId, CabbageSettings::getColourFromValueTree (valueTree, CabbageColourIds::selectTextBackground, Colours::grey).contrasting(0.8f));
        textEditor->setMultiLine (true);
        textEditor->setReadOnly (true);

        fontPtr = Typeface::createSystemTypefaceFor (CabbageBinaryData::DejaVuSansMonoBold_ttf,  CabbageBinaryData::DejaVuSansMonoBold_ttfSize);
        textEditor->setFont (Font (fontPtr).withHeight (12));

        setText ("Csound output message console\n");
    };

    ~CabbageOutputConsole() {};


    void setText (String text)
    {
        const MessageManagerLock lock;
        textEditor->insertTextAtCaret (text);
        textEditor->setCaretPosition (textEditor->getText().length());
    }

    String getText()
    {
        const MessageManagerLock lock;
        return textEditor->getText();
    }

    void updateColourScheme()
    {
#ifndef CabbageLite
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

        String currentText = textEditor->getText();
        textEditor->setText ("");
        textEditor->setFont (Font (fontPtr).withHeight (fontSize));
        textEditor->setText (currentText);
        textEditor->setCaretPosition (textEditor->getText().length());

    }

    void clearText()
    {
        textEditor->setText ("");
    }

    void resized() override
    {
        Rectangle<int> area (getLocalBounds());
        textEditor->setBounds (area.reduced (2).withY (0));
    }

    void paint (Graphics& g)  override
    {
#ifndef CabbageLite
        g.fillAll (CabbageSettings::getColourFromValueTree (value, CabbageColourIds::consoleOutline, Colours::grey.darker()));
#endif
    }

private:
    ValueTree value;

};



#endif  // CABBAGEOUTPUCONSOLECOMPONENT_H_INCLUDED
