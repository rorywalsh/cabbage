/*
  ==============================================================================

    CodeEditorToolbarFactory.h
    Created: 23 Nov 2016 9:44:28am
    Author:  rory

  ==============================================================================
*/

#include "../CabbageIds.h"
#include "../Settings/CabbageSettings.h"

#ifndef CODEEDITORTOOLBARFACTORY_H_INCLUDED
#define CODEEDITORTOOLBARFACTORY_H_INCLUDED

class CabbageDocumentWindow;

class CabbageToolbarFactory   : public ToolbarItemFactory
{
public:
	CabbageToolbarFactory(CabbageDocumentWindow* owner);

	//==============================================================================
	// Each type of item a toolbar can contain must be given a unique ID. These
	// are the ones we'll use in this demo.
	enum ToolbarItemIds
	{
		doc_new         = 1,
		doc_open        = 2,
		doc_save        = 3,
		doc_saveAs      = 4,
		edit_copy       = 5,
		edit_cut        = 6,
		edit_paste      = 7,
		customComboBox  = 8
	};

	void getAllToolbarItemIds (Array<int>& ids) override;
	void getDefaultItemSet (Array<int>& ids) override;
	ToolbarItemComponent* createItem (int itemId) override;

private:

	ToolbarButton* createButtonFromSVG (const int itemId, const String& text, const File svgFile);


	class ToolbarComboBox : public ToolbarItemComponent
	{
		public:
			ToolbarComboBox (const int toolbarItemId)
				: ToolbarItemComponent (toolbarItemId, "Custom Toolbar Item", false),
				  comboBox ("toolbar combo box")
			{
				addAndMakeVisible (comboBox);

				for (int i = 1; i < 20; ++i)
					comboBox.addItem ("instr " + String (i), i);

				comboBox.setSelectedId (1);
				comboBox.setEditableText (false);
			}

			bool getToolbarItemSizes (int /*toolbarDepth*/, bool isVertical,
									  int& preferredSize, int& minSize, int& maxSize) override
			{
				if (isVertical)
					return false;

				preferredSize = 250;
				minSize = 80;
				maxSize = 300;
				return true;
			}

			void paintButtonArea (Graphics&, int, int, bool, bool) override
			{
			}

			void contentAreaChanged (const Rectangle<int>& newArea) override
			{
				comboBox.setSize (newArea.getWidth() - 2,
								  jmin (newArea.getHeight() - 2, 22));

				comboBox.setCentrePosition (newArea.getCentreX(), newArea.getCentreY());
			}

		private:
			ComboBox comboBox;
	};
	
	CabbageDocumentWindow* owner;
};



#endif  // CODEEDITORTOOLBARFACTORY_H_INCLUDED
