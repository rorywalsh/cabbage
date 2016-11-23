/*
  ==============================================================================

    CabbageToolbarFactory.cpp
    Created: 23 Nov 2016 5:09:25pm
    Author:  rory

  ==============================================================================
*/

#include "CabbageToolbarFactory.h"
#include "CabbageDocumentWindow.h"

CabbageToolbarFactory::CabbageToolbarFactory(CabbageDocumentWindow* owner)
	: owner(owner)
{
	
}

void CabbageToolbarFactory::getAllToolbarItemIds (Array<int>& ids)
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

void CabbageToolbarFactory::getDefaultItemSet (Array<int>& ids)
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


ToolbarItemComponent* CabbageToolbarFactory::createItem (int itemId)
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

ToolbarButton* CabbageToolbarFactory::createButtonFromSVG (const int itemId, const String& text, const File svgFile)
{
	ScopedPointer<XmlElement> svg (XmlDocument::parse(svgFile.loadFileAsString()));
	if(svg == nullptr)
		jassert(false);

	Drawable* drawable;

	if (svg != nullptr)
	{
		drawable = Drawable::createFromSVG (*svg);
	}

	ToolbarButton* button = new ToolbarButton(itemId, text, drawable, 0);
	button->addListener(owner);
	return button; 
}