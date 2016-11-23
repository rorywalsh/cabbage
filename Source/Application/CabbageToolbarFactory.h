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


class CabbageToolbarFactory   : public ToolbarItemFactory
{
public:
	CabbageToolbarFactory() {}

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

	void getAllToolbarItemIds (Array<int>& ids) override
	{
		// This returns the complete list of all item IDs that are allowed to
		// go in our toolbar. Any items you might want to add must be listed here. The
		// order in which they are listed will be used by the toolbar customisation panel.

		ids.add (doc_new);
		ids.add (doc_open);
		ids.add (doc_save);
		ids.add (doc_saveAs);
		ids.add (edit_copy);
		ids.add (edit_cut);
		ids.add (edit_paste);
		ids.add (customComboBox);
	}

	void getDefaultItemSet (Array<int>& ids) override
	{
		// This returns an ordered list of the set of items that make up a
		// toolbar's default set. Not all items need to be on this list, and
		// items can appear multiple times (e.g. the separators used here).
		ids.add (doc_new);
		ids.add (doc_open);
		ids.add (doc_save);
		ids.add (doc_saveAs);
		ids.add (edit_copy);
		ids.add (edit_cut);
		ids.add (edit_paste);
		ids.add (customComboBox);
	}

	ToolbarItemComponent* createItem (int itemId) override
	{
		switch (itemId)
		{
			case doc_new:           
				return createButtonFromSVG(itemId, "new", File("/home/rory/sourcecode/cabaiste/Icons/document-new.svg"));
			case doc_open:          
				return createButtonFromSVG(itemId, "open", File("/home/rory/sourcecode/cabaiste/Icons/document-open.svg"));
			case doc_save:          
				return createButtonFromSVG(itemId, "save", File("/home/rory/sourcecode/cabaiste/Icons/document-save.svg"));
			case doc_saveAs:        
				return createButtonFromSVG(itemId, "save as", File("/home/rory/sourcecode/cabaiste/Icons/document-save-as.svg"));
			case edit_copy:         
				return createButtonFromSVG(itemId, "copy", File("/home/rory/sourcecode/cabaiste/Icons/edit-copy.svg"));
			case edit_cut:          
				return createButtonFromSVG(itemId, "cut", File("/home/rory/sourcecode/cabaiste/Icons/edit-cut.svg"));
			case edit_paste:        
				return createButtonFromSVG(itemId, "paste", File("/home/rory/sourcecode/cabaiste/Icons/edit-paste.svg"));
			case customComboBox:    
				return new ToolbarComboBox (itemId);
			default:                break;
		}

		return nullptr;
	}

private:

	ToolbarButton* createButtonFromSVG (const int itemId, const String& text, const File svgFile)
	{
		ScopedPointer<XmlElement> svg (XmlDocument::parse(svgFile.loadFileAsString()));
		if(svg == nullptr)
			jassert(false);

		Drawable* drawable;

		if (svg != nullptr)
		{
			drawable = Drawable::createFromSVG (*svg);
		}

		return new ToolbarButton(itemId, text, drawable, 0);
	}


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
};



#endif  // CODEEDITORTOOLBARFACTORY_H_INCLUDED
