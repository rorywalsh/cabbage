/*
  ==============================================================================

    EditorAndConsoleContentComponent.h
    Created: 30 Oct 2016 1:42:59pm
    Author:  rory

  ==============================================================================
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
				setText(initString);
			}
			
			void paint(Graphics &g)
			{
				const Colour background = CabbageSettings::getColourFromValueTree(valueTree, CabbageColourIds::statusBar, Colours::black);
				const Colour text = CabbageSettings::getColourFromValueTree(valueTree, CabbageColourIds::statusBarText, Colours::black);
				g.setColour(background);
				g.fillAll();

				g.setColour(background.darker(.7));
				g.drawRect(0, 0, getWidth(), getHeight(), 2);
				g.setColour(text);
				g.drawFittedText (statusText, getLocalBounds().withX(10), Justification::left, 2);
			}
			
			void setText(String text)
			{
				statusText = text;
				repaint();
			}
			
		private:
			ValueTree valueTree;
			String statusText;
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
					setBounds(getPosition().getX(), startingYPos+e.getDistanceFromDragStartY(), getWidth(), getHeight());
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
