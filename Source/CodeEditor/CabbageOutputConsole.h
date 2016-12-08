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
public:
    CabbageOutputConsole(ValueTree valueTree): Component(), value(valueTree)
    {
        addAndMakeVisible(textEditor = new TextEditor(), true);
        textEditor->setColour(Label::outlineColourId, Colours::white);
        textEditor->setColour(TextEditor::textColourId, CabbageSettings::getColourFromValueTree(valueTree, CabbageColourIds::consoleText, Colours::grey.darker()));
        textEditor->setColour(TextEditor::backgroundColourId, CabbageSettings::getColourFromValueTree(valueTree, CabbageColourIds::consoleBackground, Colours::grey.darker()));
        textEditor->setMultiLine(true);
        textEditor->setReadOnly(true);
        textEditor->setFont(Font("Arial", 12, 1));

        setText("Cabbage Csound IDE\n");
    };

    ~CabbageOutputConsole() {};


    void setText(String text)
    {
        textEditor->insertTextAtCaret(text);
        textEditor->setCaretPosition(textEditor->getText().length());
    }

    String getText()
    {
        const MessageManagerLock lock;
        return textEditor->getText();
    }

    void updateColourScheme()
    {
        textEditor->setColour(TextEditor::textColourId, CabbageSettings::getColourFromValueTree(value, CabbageColourIds::consoleText, Colours::grey.darker()));
        textEditor->setColour(TextEditor::backgroundColourId, CabbageSettings::getColourFromValueTree(value, CabbageColourIds::consoleBackground, Colours::grey.darker()));
        repaint();
    }

    void setFontSize(int size)
    {
        textEditor->setFont(Font("Arial", size, 0));
    }

    void resized()
    {
        Rectangle<int> area (getLocalBounds());
        textEditor->setBounds(area.reduced(2).withY(0));
    }

    void paint(Graphics& g)
    {
        g.fillAll(CabbageSettings::getColourFromValueTree(value, CabbageColourIds::consoleOutline, Colours::grey.darker()));
//        g.setColour(Colours::white);
//        g.drawRoundedRectangle(getLocalBounds().toFloat(), 2, 2);
//        g.drawFittedText("Csound output", getLocalBounds().withHeight(18), Justification::centred, 1, 1.f);
    }

private:
    ValueTree value;

};



#endif  // CABBAGEOUTPUCONSOLECOMPONENT_H_INCLUDED
