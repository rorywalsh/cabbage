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

#include "CabbagePopupWindow.h"


CabbagePopupWindow::CabbagePopupWindow (ValueTree valueTree, const String& svgText, bool withEditor)
: showEditor(withEditor)
{
    setOpaque(false);

    buttons[0].setButtonText("Ok");
    buttons[0].setSize(70, 25);
    buttons[1].setButtonText("Cancel");
    buttons[1].setSize(70, 25);
    buttons[0].setAlpha(0.0f);
    buttons[1].setAlpha(0.0f);

    buttons[0].setColour(TextButton::ColourIds::buttonOnColourId, Colour(0xff00ABD1));// Lattice::getColours()[1]);
    buttons[1].setColour(TextButton::ColourIds::buttonOnColourId, Colour(0xff00ABD1));// Lattice::getColours()[1]);

    editor.setColour (Label::outlineColourId, Colours::transparentBlack);
    editor.setColour (TextEditor::outlineColourId, Colours::transparentBlack);
    editor.setColour (Label::outlineWhenEditingColourId, Colours::transparentBlack);
    editor.setColour (TextEditor::ColourIds::focusedOutlineColourId, Colours::transparentBlack);
//    lookAndFeelChanged();
    
    if(showEditor)
    {
         addAndMakeVisible(editor);
         editor.setTextToShowWhenEmpty("", Colour(100, 100, 100));
         addAndMakeVisible(buttons[1]);
    }


    
    buttons[0].onClick = [valueTree, this] mutable
    {
        valueTree.setProperty("NEW_PRESET_NAME", editor.getText(), nullptr);
        juce::ModalComponentManager::getInstance()->cancelAllModalComponents();
    };

    buttons[1].onClick = [valueTree, this] mutable
    {
        valueTree.setProperty("NEW_PRESET_NAME", "", nullptr);
        juce::ModalComponentManager::getInstance()->cancelAllModalComponents();
    };


    editor.onReturnKey = [valueTree, this] mutable
    {
        valueTree.setProperty("NEW_PRESET_NAME", editor.getText(), nullptr);
        juce::ModalComponentManager::getInstance()->cancelAllModalComponents();
    };
    
    addAndMakeVisible(buttons[0]);

    setSVGImage(svgText);

}

void CabbagePopupWindow::setSVGImage(const String svgText)
{
    std::unique_ptr<XmlElement> svg(XmlDocument::parse(svgText));
    if (svg != nullptr)
    {
        drawable = Drawable::createFromSVG(*svg);
    }
}
void CabbagePopupWindow::resized()
{
    if(showEditor)
    {
        int h = 250;
        editor.setBounds(75, h-100, getWidth()-150, 16);
        buttons[0].setBounds(75, 195, 120, 30);
        buttons[1].setBounds(205, 195, 120, 30);
    }
    else
        buttons[0].setCentrePosition(getLocalBounds().removeFromBottom(50).getCentre());
}

void CabbagePopupWindow::paint(Graphics& g)
{
    g.fillAll(Colours::transparentBlack);
    drawable->setTransformToFit(getLocalBounds().toFloat(), RectanglePlacement::stretchToFit);
    drawable->draw(g, 1.f, {});

    //g.setColour(Colour(0xff00ABD1));
}
