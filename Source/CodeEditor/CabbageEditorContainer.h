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

#ifndef CabbageEditorContainer_H_INCLUDED
#define CabbageEditorContainer_H_INCLUDED

#include "../CabbageCommonHeaders.h"
#include "CabbageCodeEditor.h"
#include "CabbageOutputConsole.h"
#include "JavascriptCodeTokeniser.h"

class CabbageMainComponent;

class CabbageEditorContainer : public Component
{
public:
	//-------------------------------------------------------------
	class StatusBar : public Component
	{
	public:
		StatusBar(ValueTree valueTree, CabbageEditorContainer* parent)
			: Component("StatusBar"),
			valueTree(valueTree),
		{
            ignoreUnused(parent);
			String initString = (SystemStats::getOperatingSystemName() +
				"CPU: " + String(SystemStats::getCpuSpeedInMegaherz())
				+ "MHz  Cores: " + String(SystemStats::getNumCpus())
				+ "  " + String(SystemStats::getMemorySizeInMegabytes()) + "MB");
			setText(StringArray(initString));
		}

		void paint(Graphics& g)  override;

		void setText(StringArray text)
		{
			statusText = text;
			repaint();
		}

		int getCurrentYPos() { return currentYPos; }

	private:
		ValueTree valueTree;
		StringArray statusText;
	};

	CabbageMainComponent* getContentComponent();
	//=============================================================================
	CabbageEditorContainer(CabbageSettings* settings, bool isCsdFile = true);
	~CabbageEditorContainer() override;
	void updateLookAndFeel();
	void openFile(File file);
	File getFile() { return currentFile; }
	void setFile(File file) { currentFile = file; }
    void resized() override;

    void updateEditorColourScheme();
    void mouseDown (const MouseEvent& e) override;
    void mouseUp (const MouseEvent& e) override;
    void mouseExit (const MouseEvent& e) override;
    void mouseEnter (const MouseEvent& e) override;
    void mouseDrag (const MouseEvent& e) override;

    int getStatusBarPosition();
    void hideOutputConsole();
    std::unique_ptr<CabbageCodeEditorComponent> editor;
    std::unique_ptr<CabbageOutputConsole> outputConsole;
    StatusBar statusBar;
    CodeDocument csoundDocument;
	CodeDocument consoleDocument;
    CsoundTokeniser csoundTokeniser;
	JavascriptTokeniser javaTokeniser;
    CabbageSettings* settings;

private:
	File currentFile;
    bool isCsdFile = true;
    int startingDragPos;

};


#endif  // CabbageEditorContainer_H_INCLUDED
