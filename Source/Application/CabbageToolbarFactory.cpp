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

#include "CabbageToolbarFactory.h"
#include "CabbageContentComponent.h"

CabbageToolbarFactory::CabbageToolbarFactory(CabbageContentComponent* owner)
    : owner(owner), togglePlayButton(nullptr), combo(nullptr)
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
	ids.add (system_prefs);
    ids.add (toggle_play);
    ids.add (custom_comboBox);
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
	ids.add (system_prefs);
    ids.add (toggle_play);
    ids.add (custom_comboBox);

}

static const String getSVGTextFromMemory(const void* svg, size_t size)
{
	MemoryInputStream svgStream(svg, size, false);
    return svgStream.readString();
}

ToolbarItemComponent* CabbageToolbarFactory::createItem (int itemId)
{
    switch (itemId)
    {
	case doc_new:
		return createButtonFromSVG(itemId, "new", getSVGTextFromMemory(CabbageBinaryData::documentnew_svg, CabbageBinaryData::documentnew_svgSize));
    case doc_open:
        return createButtonFromSVG(itemId, "open", getSVGTextFromMemory(CabbageBinaryData::documentopen_svg, CabbageBinaryData::documentopen_svgSize));
    case doc_save:
        return createButtonFromSVG(itemId, "save", getSVGTextFromMemory(CabbageBinaryData::documentsave_svg, CabbageBinaryData::documentsave_svgSize));
    case doc_saveAs:
        return createButtonFromSVG(itemId, "save as", getSVGTextFromMemory(CabbageBinaryData::documentsaveas_svg, CabbageBinaryData::documentsaveas_svgSize));
    case edit_copy:
        return createButtonFromSVG(itemId, "copy", getSVGTextFromMemory(CabbageBinaryData::editcopy_svg, CabbageBinaryData::editcopy_svgSize));
    case edit_cut:
        return createButtonFromSVG(itemId, "cut", getSVGTextFromMemory(CabbageBinaryData::editcut_svg, CabbageBinaryData::editcut_svgSize));
    case edit_paste:
        return createButtonFromSVG(itemId, "paste", getSVGTextFromMemory(CabbageBinaryData::editpaste_svg, CabbageBinaryData::editpaste_svgSize));
    case system_prefs:
        return createButtonFromSVG(itemId, "settings", getSVGTextFromMemory(CabbageBinaryData::emblemsystem_svg, CabbageBinaryData::emblemsystem_svgSize));
    case custom_comboBox:
        return(combo = new ToolbarComboBox (itemId));
    case toggle_play:
        return createButtonFromSVG(itemId, "togglePlay", getSVGTextFromMemory(CabbageBinaryData::mediaplaybackstart_svg, CabbageBinaryData::mediaplaybackstart_svgSize),
														getSVGTextFromMemory(CabbageBinaryData::mediaplaybackstop_svg, CabbageBinaryData::mediaplaybackstop_svgSize));
    default:
        break;
    }

    return nullptr;
}

ToolbarButton* CabbageToolbarFactory::createButtonFromSVG (const int itemId, const String& text, const String svgFile, const String onFile)
{
    ScopedPointer<XmlElement> svgNormal (XmlDocument::parse(svgFile));
    if(svgNormal == nullptr)
        jassert(false);

    Drawable* drawableNormal = nullptr;

    if (svgNormal != nullptr)
    {
        drawableNormal = Drawable::createFromSVG (*svgNormal);
    }

	ScopedPointer<XmlElement> svgOn (XmlDocument::parse(onFile));
	if(svgOn != nullptr)
	{			
        Drawable* drawableOn = nullptr;

        if (svgOn != nullptr)
        {
            drawableOn = Drawable::createFromSVG (*svgOn);
        }

        togglePlayButton = new ToolbarButton(itemId, text, drawableNormal, drawableOn);
        togglePlayButton->setClickingTogglesState(true);
        togglePlayButton->addListener(owner);
        return togglePlayButton;
    }

    ToolbarButton* button = new ToolbarButton(itemId, text, drawableNormal, 0);
    button->addListener(owner);
    return button;

}