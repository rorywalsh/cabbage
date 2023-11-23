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
: showEditor(withEditor), vt(valueTree)
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
    editor.setColour (TextEditor::backgroundColourId, Colours::transparentBlack);
    editor.setColour (TextEditor::textColourId, Colour(165, 165, 165));
    editor.setCaretVisible(false);
    editor.setColour (Label::outlineWhenEditingColourId, Colours::transparentBlack);
    editor.setColour (TextEditor::ColourIds::focusedOutlineColourId, Colours::transparentBlack);
//    lookAndFeelChanged();
    
    if(showEditor)
    {
         addAndMakeVisible(editor);
         editor.setTextToShowWhenEmpty("", Colour(100, 100, 100));
         addAndMakeVisible(buttons[1]);
    }


    
    buttons[0].onClick = [this]
    {
        setValueTreeProperty("NEW_PRESET_NAME", editor.getText());
        juce::ModalComponentManager::getInstance()->cancelAllModalComponents();
    };

    buttons[1].onClick = [this]
    {
        setValueTreeProperty("NEW_PRESET_NAME", "");
        juce::ModalComponentManager::getInstance()->cancelAllModalComponents();
    };


    editor.onReturnKey = [this]
    {
        setValueTreeProperty("NEW_PRESET_NAME", editor.getText());
        juce::ModalComponentManager::getInstance()->cancelAllModalComponents();
    };
    
    addAndMakeVisible(buttons[0]);

    setSVGImage(svgText);

}

void CabbagePopupWindow::setValueTreeProperty(String prop, String val)
{
    vt.setProperty(prop, val, nullptr);
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
        editor.setBounds(30, getHeight()*.47, getWidth()-60, 18);
        buttons[0].setBounds(86, 105, 90, 20);
        buttons[1].setBounds(154, 105, 90, 20);
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
