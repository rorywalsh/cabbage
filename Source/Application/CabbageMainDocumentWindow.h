/*
  ==============================================================================

    CabbageMainDocumentWindow.h
    Created: 11 Oct 2016 12:43:50pm
    Author:  rory

  ==============================================================================
*/
#ifndef CABBAGEMAINWINDOW_H_INCLUDED
#define CABBAGEMAINWINDOW_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "CabbageCodeEditor.h"
#include "CabbageOutputConsole.h"
#include "../BinaryData/CabbageBinaryData.h"

class CabbageSettings;



//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainContentComponent   : public Component
{
public:

	class HorizontalResizerBar : public Component
	{
		public:
			HorizontalResizerBar(MainContentComponent* parent, ValueTree valueTree)
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
			}
			
			
			void paint(Graphics &g)
			{
				if(isActive)
					g.fillAll(CabbageSettings::getColourFromValueTree(valueTree, CabbageColourIds::consoleOutline, Colours::grey).contrasting(.4));
				else
					g.fillAll(CabbageSettings::getColourFromValueTree(valueTree, CabbageColourIds::consoleOutline, Colours::grey));

			}
			
	private:
		MainContentComponent* owner;
		ValueTree valueTree;
		int startingYPos;
		bool isActive = false;
	};

    //==============================================================================
    MainContentComponent(ValueTree settings);
    ~MainContentComponent();

    void paint (Graphics&) override;
    void resized() override;
	Image createBackground();
	
	void openFile(File file);

    ScopedPointer<CabbageCodeEditorComponent> editor;
    ScopedPointer<CabbageOutputConsole> outputConsole;
	HorizontalResizerBar horizontalResizerBar;
    CodeDocument csoundDocument;
    CsoundTokeniser csoundTokeniser;

private:
	Image bgImage;
	
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


//==============================================================================
/*
    This class implements the desktop window that contains an instance of
    our MainContentComponent class.
*/
class CabbageMainDocumentWindow    : public DocumentWindow
{	
public:
    CabbageMainDocumentWindow (String name, CabbageSettings* settings);
	~CabbageMainDocumentWindow()
	{

	}
	
	void closeButtonPressed() override
	{
		JUCEApplicationBase::quit();
	}
	
	void updateEditorColourScheme();
	
	MainContentComponent* getMainContentComponent();
	ScopedPointer<MainContentComponent> mainContentComponent;



private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CabbageMainDocumentWindow)
};

#endif