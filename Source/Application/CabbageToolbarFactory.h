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

class CabbageContentComponent;

class CabbageToolbarFactory   : public ToolbarItemFactory
{
public:
	CabbageToolbarFactory(CabbageContentComponent* owner);
	~CabbageToolbarFactory()
	{
	}

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
		custom_comboBox  = 8,
		toggle_play		= 9
	};

	void getAllToolbarItemIds (Array<int>& ids) override;
	void getDefaultItemSet (Array<int>& ids) override;
	ToolbarItemComponent* createItem (int itemId) override;
	
	void togglePlay(bool enabled)
	{
		togglePlayButton->setToggleState(enabled, dontSendNotification);
	}
	
	ToolbarButton* togglePlayButton;
	
private:

	ToolbarButton* createButtonFromSVG (const int itemId, const String& text, const File svgFile, const String onFile="");


	class ToolbarComboBox : public ToolbarItemComponent
	{
		public:
			ToolbarComboBox (const int toolbarItemId)
				: ToolbarItemComponent (toolbarItemId, "Custom Toolbar Item", false),
				  comboBox ("toolbar combo box")
			{
				addAndMakeVisible (comboBox);
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
			
			void addItemsToComboBox(const String text, const int itemId)
			{
				comboBox.addItem(text, itemId);
				comboBox.setSelectedId (1);
			}

			void clearItemsFromComboBox()
			{
				comboBox.clear(dontSendNotification);
			}
			
			ComboBox& getCombo()
			{
				return comboBox;
			}

		private:
			ComboBox comboBox;
	};
	
	CabbageContentComponent* owner;

public:
	ToolbarComboBox* combo;	//owner by toolbar factory...
};



#endif  // CODEEDITORTOOLBARFACTORY_H_INCLUDED
