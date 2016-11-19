/*
  Copyright (C) 2016 Rory Walsh

  Cabbage is free software; you can redistribute it
  and/or modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  Cabbage is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with Csound; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
  02111-1307 USA
*/

#ifndef EDITORANDCONSOLECONTENTCOMPONENT_H_INCLUDED
#define EDITORANDCONSOLECONTENTCOMPONENT_H_INCLUDED

#include "../CabbageCommonHeaders.h"
#include "CabbageCodeEditor.h"
#include "CabbageOutputConsole.h"

class EditorAndConsoleContentComponent   : public Component
{
public:
    //-------------------------------------------------------------
    class StatusBar : public Component
    {
    public:
        StatusBar(ValueTree valueTree):Component("StatusBar"), valueTree(valueTree)
        {
            String initString = (SystemStats::getOperatingSystemName() +
                                 "CPU: " + String (SystemStats::getCpuSpeedInMegaherz())
                                 + "MHz  Cores: " + String (SystemStats::getNumCpus())
                                 + "  " + String (SystemStats::getMemorySizeInMegabytes()) + "MB");
            setText(StringArray(initString));
        }

        void paint(Graphics &g)
        {
            const Colour background = CabbageSettings::getColourFromValueTree(valueTree, CabbageColourIds::statusBar, Colours::black);
            const Colour text = CabbageSettings::getColourFromValueTree(valueTree, CabbageColourIds::statusBarText, Colours::black);
            const Colour outline = CabbageSettings::getColourFromValueTree(valueTree, CabbageColourIds::consoleOutline, Colours::black);
            const Colour opcodeColour = CabbageSettings::getColourFromValueTree(valueTree, CabbageColourIds::keyword, Colours::black);
            const Colour syntaxColour = CabbageSettings::getColourFromValueTree(valueTree, CabbageColourIds::numbers, Colours::black);
            const Colour commmentColour = CabbageSettings::getColourFromValueTree(valueTree, CabbageColourIds::comment, Colours::black);

            g.fillAll(outline);

            g.setColour(background.withAlpha(1.f));
            g.setColour(background.darker(.7));
            g.fillRect(2, 2, getWidth()-4, getHeight()-4);
            g.setColour(text);
            g.setFont(Font(14));
            Rectangle<int> area (getLocalBounds());

            if(statusText.size()==1)
            {
                g.setColour(syntaxColour);
                g.drawFittedText (statusText[0], getLocalBounds().withLeft(25), Justification::left, 2);
            }
            else
            {
                g.setColour(opcodeColour);
                g.setFont(Font(14, Font::bold));
                const String opcodeText = statusText[0].replace("\"", "");
                g.drawFittedText (opcodeText, getLocalBounds().withLeft(25), Justification::left, 2);
                g.setColour(syntaxColour);
                const int opcodeTextWidth = Font(14).getStringWidth(opcodeText)+10;
                const String opcodeSyntaxText = statusText[3].replaceSection(0, 1, "").replaceSection(statusText[3].length()-2, 1, "");
                g.drawFittedText(opcodeSyntaxText, area.withLeft(opcodeTextWidth+30), Justification::left, 2);
                const int opcodeSyntaxWidth = Font(14).getStringWidth(opcodeSyntaxText)+10;
                g.setFont(Font(14, Font::italic));
                const String descriptionText = "; "+statusText[2];
                g.setColour(commmentColour);
                g.drawFittedText (descriptionText, getLocalBounds().withLeft(opcodeTextWidth+opcodeSyntaxWidth+50), Justification::left, 2);
            }
        }

        void setText(StringArray text)
        {
            statusText = text;
            repaint();
        }

    private:
        ValueTree valueTree;
        StringArray statusText;
    };
    //-------------------------------------------------------------
    class HorizontalResizerBar : public Component
    {
    public:
        HorizontalResizerBar(EditorAndConsoleContentComponent* parent, ValueTree valueTree)
            :Component("HorizontalResizerBar"),
             owner(parent),
             valueTree(valueTree)
        {
            setSize(owner->getWidth(), 25);
        }

        void mouseExit(const MouseEvent& e)
        {
            isActive = false;
            setMouseCursor(MouseCursor::NormalCursor);
            repaint();
        }

        void mouseEnter(const MouseEvent& e)
        {
            isActive = true;
            setMouseCursor(MouseCursor::UpDownResizeCursor);
            startingYPos = getPosition().getY();
            repaint();
        }

        void mouseDrag(const MouseEvent& e)
        {
            setBounds(getPosition().getX(), jmin(2, startingYPos+e.getDistanceFromDragStartY()), owner->getWidth(), getHeight());
            owner->resized();
            repaint();
        }


        void paint(Graphics &g)
        {
            if(isActive)
                g.fillAll(CabbageSettings::getColourFromValueTree(valueTree, CabbageColourIds::consoleOutline, Colours::grey).contrasting(.4));
            else
                g.fillAll(CabbageSettings::getColourFromValueTree(valueTree, CabbageColourIds::consoleOutline, Colours::grey));

        }

    private:
        EditorAndConsoleContentComponent* owner;
        ValueTree valueTree;
        int startingYPos;
        bool isActive = false;
    };

    //=============================================================================
    EditorAndConsoleContentComponent(ValueTree settings);
    ~EditorAndConsoleContentComponent();
    void updateLookAndFeel();
    void openFile(File file);
    void resized();
    void updateEditorColourScheme();
    ScopedPointer<CabbageCodeEditorComponent> editor;
    ScopedPointer<CabbageOutputConsole> outputConsole;
    HorizontalResizerBar horizontalResizerBar;
    StatusBar statusBar;
    CodeDocument csoundDocument;
    CsoundTokeniser csoundTokeniser;
    ValueTree settings;

private:
    int horizontalBarPosition = 0;
    const int statusBarHeight = 25;

};


#endif  // EDITORANDCONSOLECONTENTCOMPONENT_H_INCLUDED
