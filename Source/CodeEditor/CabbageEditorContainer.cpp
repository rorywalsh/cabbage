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

CabbageEditorContainer::CabbageEditorContainer(ValueTree settings)
    : horizontalResizerBar(this, settings), settings(settings),
      statusBar(settings)
{
    addAndMakeVisible(horizontalResizerBar);
    addAndMakeVisible(statusBar);
    addAndMakeVisible(editor = new CabbageCodeEditorComponent(this, &statusBar, settings, csoundDocument, &csoundTokeniser));
    addAndMakeVisible(outputConsole = new CabbageOutputConsole(settings));
    editor->setLineNumbersShown(true);
    editor->setFont(Font(String("DejaVu Sans Mono"), 17, 0));
    editor->setVisible(true);
    outputConsole->setVisible(true);
    horizontalResizerBar.setVisible(true);
    setSize(500, 500);
    const int width = settings.getChildWithName("General").getProperty("LastKnownWidth");
    const int height = settings.getChildWithName("General").getProperty("LastKnownHeight");
    horizontalBarPosition = settings.getChildWithName("General").getProperty("HorizontalBarLastPosition");
    editor->setBounds(0, 0, getWidth(), 500);
    horizontalResizerBar.setBounds(0, 500, 4000, getHeight()*.01);
    updateLookAndFeel();
}

CabbageEditorContainer::~CabbageEditorContainer()
{
    editor = nullptr;
    outputConsole = nullptr;
}
void CabbageEditorContainer::openFile(File file)
{
    editor->setVisible(true);
    outputConsole->setVisible(true);
    horizontalResizerBar.setVisible(true);
    editor->loadContent(file.loadFileAsString());
}

void CabbageEditorContainer::updateLookAndFeel()
{
    editor->updateColourScheme();
    outputConsole->updateColourScheme();
    horizontalResizerBar.repaint();
}

void CabbageEditorContainer::resized()
{
	Rectangle<int> rect = getLocalBounds();
    editor->setBounds(rect.removeFromTop(horizontalResizerBar.getY()));
    //const int consoleY = horizontalResizerBar.getY()+horizontalResizerBar.getHeight();
    //const int consoleHeight = getHeight()-(consoleY+statusBarHeight+5);
    statusBar.setBounds(rect.removeFromTop(28));
    outputConsole->setBounds(rect);
}

void CabbageEditorContainer::updateEditorColourScheme()
{
    editor->updateColourScheme();
    outputConsole->updateColourScheme();
    horizontalResizerBar.repaint();
    statusBar.repaint();
}