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

#include "CabbageEditorContainer.h"
#include "../Application/CabbageMainComponent.h"

CabbageEditorContainer::CabbageEditorContainer (CabbageSettings* settings, bool isCsdFile)
    : settings (settings),
      statusBar (settings->valueTree, this)
{
    addAndMakeVisible (statusBar);
    addAndMakeVisible (editor = new CabbageCodeEditorComponent (this, &statusBar, settings->valueTree, csoundDocument, &csoundTokeniser));
    addAndMakeVisible (outputConsole = new CabbageOutputConsole (settings->valueTree));
	
    editor->setLineNumbersShown (true);
    editor->addMouseListener (this, true);
    Typeface::Ptr fontPtr = Typeface::createSystemTypefaceFor (CabbageBinaryData::DejaVuSansMonoBold_ttf,  CabbageBinaryData::DejaVuSansMonoBold_ttfSize);
    const int fontSize = settings->getUserSettings()->getIntValue ("FontSize", 17);
	const int fontSizeConsole = settings->getUserSettings()->getIntValue ("FontSizeConsole", 14);
    editor->setFont (Font (fontPtr).withHeight (fontSize));
    editor->setFontSize (fontSize);
    editor->setVisible (true);
    outputConsole->setVisible (true);
	outputConsole->setFontSize(fontSizeConsole);	
    statusBar.addMouseListener (this, true);

    const int width = settings->getUserSettings()->getIntValue ("IDE_LastKnownWidth");
    const int height = settings->getUserSettings()->getIntValue ("IDE_LastKnownHeight");
    const int x = settings->getUserSettings()->getIntValue ("IDE_LastKnownX");
    const int y = settings->getUserSettings()->getIntValue ("IDE_LastKnownY");
    const int statusBarPosition = settings->getUserSettings()->getIntValue ("IDE_StatusBarPos");

    setSize (width, height);
    setTopLeftPosition (x, y);
    statusBar.setBounds (0, statusBarPosition, width, 28);
    editor->setBounds (0, 0, width, height);
    updateLookAndFeel();
}

CabbageEditorContainer::~CabbageEditorContainer()
{
    settings->getUserSettings()->setValue ("FontSize", editor->getFontSize());
	settings->getUserSettings()->setValue ("FontSizeConsole", outputConsole->getFontSize());
    editor = nullptr;
    outputConsole = nullptr;
}

void CabbageEditorContainer::hideOutputConsole()
{
    statusBar.setBounds (0, getHeight() - statusBar.getHeight(), getWidth(), statusBar.getHeight());
    outputConsole->setBounds (0, getHeight(), getWidth(), 0);
}

CabbageMainComponent* CabbageEditorContainer::getContentComponent()
{
    return this->findParentComponentOfClass<CabbageMainComponent>();
}

void CabbageEditorContainer::openFile (File file)
{
    editor->setVisible (true);
    outputConsole->setVisible (true);
    editor->loadContent (file.loadFileAsString());
}

void CabbageEditorContainer::updateLookAndFeel()
{
    editor->updateColourScheme();
    outputConsole->updateColourScheme();
}

int CabbageEditorContainer::getStatusBarPosition()
{
    return statusBar.getY();
}

void CabbageEditorContainer::mouseExit (const MouseEvent& e)
{
    if (e.eventComponent->getName() == "StatusBar")
        statusBar.setMouseCursor (MouseCursor::NormalCursor);
}

void CabbageEditorContainer::mouseDown (const MouseEvent& e)
{
    editor->updateCurrenLineMarker();
}

void CabbageEditorContainer::mouseUp (const MouseEvent& e)
{
    editor->updateCurrenLineMarker();
}

void CabbageEditorContainer:: mouseEnter (const MouseEvent& e)
{
    startingDragPos = statusBar.getPosition().getY();

    if (e.eventComponent->getName() == "StatusBar")
        statusBar.setMouseCursor (MouseCursor::UpDownResizeCursor);
}

void CabbageEditorContainer::mouseDrag (const MouseEvent& e)
{
    editor->updateCurrenLineMarker();

    if (e.eventComponent->getName() == "StatusBar")
    {
        statusBar.setBounds (0, jlimit (statusBar.getHeight(), getHeight() - statusBar.getHeight(), startingDragPos + e.getDistanceFromDragStartY()), getWidth(), statusBar.getHeight());
        resized();
    }
}

void CabbageEditorContainer::resized()
{
    Rectangle<int> rect = getLocalBounds();
    editor->setBounds (rect.removeFromTop (statusBar.getY()));
    rect.removeFromTop (statusBar.getHeight());
    outputConsole->setBounds (rect.withHeight (rect.getHeight() - statusBar.getHeight() * 2));

}

void CabbageEditorContainer::updateEditorColourScheme()
{
    editor->updateColourScheme();
    outputConsole->updateColourScheme();
    statusBar.repaint();
}

void CabbageEditorContainer::StatusBar::paint (Graphics& g)
{
    const Colour background = CabbageSettings::getColourFromValueTree (valueTree, CabbageColourIds::statusBar, Colours::black);
    const Colour text = CabbageSettings::getColourFromValueTree (valueTree, CabbageColourIds::statusBarText, Colours::black);
    const Colour outline = CabbageSettings::getColourFromValueTree (valueTree, CabbageColourIds::consoleOutline, Colours::black);
    const Colour opcodeColour = CabbageSettings::getColourFromValueTree (valueTree, CabbageColourIds::keyword, Colours::black);
    const Colour syntaxColour = CabbageSettings::getColourFromValueTree (valueTree, CabbageColourIds::numbers, Colours::black);
    const Colour commmentColour = CabbageSettings::getColourFromValueTree (valueTree, CabbageColourIds::comment, Colours::black);

    g.fillAll (outline);

    g.setColour (background.withAlpha (1.f));
    g.setColour (background.darker (.7));
    g.fillRect (2, 2, getWidth() - 4, getHeight() - 4);
    g.setColour (text);
    g.setFont (Font (14));
    Rectangle<int> area (getLocalBounds());

    if (statusText.size() == 1)
    {
        g.setColour (syntaxColour);
        g.drawFittedText (statusText[0], getLocalBounds().withLeft (25), Justification::left, 2);
    }
    else
    {
        g.setColour (syntaxColour);
        const int opcodeTextWidth = 10;//Font(14).getStringWidth(opcodeText)+10;
        const String opcodeSyntaxText = statusText[3].replaceSection (0, 1, "").replaceSection (statusText[3].length() - 2, 1, "");
        g.drawFittedText (opcodeSyntaxText, area.withLeft (opcodeTextWidth + 30), Justification::left, 2);
        const int opcodeSyntaxWidth = Font (14).getStringWidth (opcodeSyntaxText) + 10;
        g.setFont (Font (14, Font::italic));
        const String descriptionText = "; " + statusText[2];
        g.setColour (commmentColour);
        g.drawFittedText (descriptionText, getLocalBounds().withLeft (opcodeTextWidth + opcodeSyntaxWidth + 50), Justification::left, 2);
    }
}