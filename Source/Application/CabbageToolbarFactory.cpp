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
#include "CabbageMainComponent.h"

CabbageToolbarFactory::CabbageToolbarFactory (CabbageMainComponent* owner)
    : owner (owner), togglePlayButton (nullptr), combo (nullptr)
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
    ids.add (doc_save_graph);
    ids.add (doc_saveAs_graph);
    ids.add (edit_copy);
    ids.add (edit_cut);
    ids.add (edit_paste);
    ids.add (system_prefs);
    ids.add (toggle_play);
    ids.add (custom_comboBox);
    ids.add (record_to_disk);
}

void CabbageToolbarFactory::timerCallback()
{
    toggleRecordButton->setToggleState((flashing % 2 == 0), dontSendNotification);
    flashing++;
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
    ids.add (doc_save_graph);
    ids.add (doc_saveAs_graph);
    ids.add (edit_copy);
    ids.add (edit_cut);
    ids.add (edit_paste);
    ids.add (system_prefs);
    ids.add (toggle_play);
    ids.add (custom_comboBox);
    ids.add (record_to_disk);

}

static const String getSVGTextFromMemory (const void* svg, size_t size)
{
    MemoryInputStream svgStream (svg, size, false);
    return svgStream.readString();
}

const String getSVGTextFromFile (const String filename)
{
    File svgFile (filename);
    if (!svgFile.existsAsFile())
        return "";
    return svgFile.loadFileAsString();
}

ToolbarItemComponent* CabbageToolbarFactory::createItem (int itemId)
{

    ToolbarButton *button = nullptr;

    switch (itemId)
    {
    case doc_new:
        button = createButtonFromSVG (itemId, "new", getSVGTextFromFile (iconsPath + "/document-new.svg"));
        if (!button)
            button = createButtonFromPNG (itemId, "new", CabbageBinaryData::documentnew_png, CabbageBinaryData::documentnew_pngSize);
        //return createButtonFromSVG (itemId, "new", getSVGTextFromMemory (CabbageBinaryData::documentnew_svg, CabbageBinaryData::documentnew_svgSize));
        break;
    case doc_open:
        button = createButtonFromSVG (itemId, "open", getSVGTextFromFile (iconsPath + "/document-open.svg"));
        if (!button)
            button = createButtonFromPNG (itemId, "open", CabbageBinaryData::documentopen_png, CabbageBinaryData::documentopen_pngSize);
        //return createButtonFromSVG (itemId, "open", getSVGTextFromMemory (CabbageBinaryData::documentopen_svg, CabbageBinaryData::documentopen_svgSize));
        break;
    case doc_save:
        button = createButtonFromSVG (itemId, "save", getSVGTextFromFile (iconsPath + "/document-save.svg"));
        if (!button)
            button = createButtonFromPNG (itemId, "save", CabbageBinaryData::documentsave_png, CabbageBinaryData::documentsave_pngSize);
        //return createButtonFromSVG (itemId, "save", getSVGTextFromMemory (CabbageBinaryData::documentsave_svg, CabbageBinaryData::documentsave_svgSize));
        break;
    case record_to_disk:
            button = createToggleButtonFromSVG (itemId, "toggleRecord", svgRecordTextOff, svgRecordTextOn);
            break;
    case doc_saveAs:
        button = createButtonFromSVG (itemId, "save as", getSVGTextFromFile (iconsPath + "/document-save-as.svg"));
        if (!button)
            button = createButtonFromPNG (itemId, "save as", CabbageBinaryData::documentsaveas_png, CabbageBinaryData::documentsaveas_pngSize);
        //return createButtonFromSVG (itemId, "save as", getSVGTextFromMemory (CabbageBinaryData::documentsaveas_svg, CabbageBinaryData::documentsaveas_svgSize));
        break;
    case edit_copy:
        button = createButtonFromSVG (itemId, "copy", getSVGTextFromFile (iconsPath + "/edit-copy.svg"));
        if (!button)
            button = createButtonFromSVG (itemId, "copy", getSVGTextFromMemory (CabbageBinaryData::editcopy_svg, CabbageBinaryData::editcopy_svgSize));
        break;
    case edit_cut:
        button = createButtonFromSVG (itemId, "cut", getSVGTextFromFile (iconsPath + "/edit-cut.svg"));
        if (!button)
            button = createButtonFromPNG (itemId, "cut", CabbageBinaryData::editcut_png, CabbageBinaryData::editcut_pngSize);
        //return createButtonFromSVG (itemId, "cut", getSVGTextFromMemory (CabbageBinaryData::editcut_svg, CabbageBinaryData::editcut_svgSize));
        break;
    case edit_paste:
        button = createButtonFromSVG (itemId, "paste", getSVGTextFromFile (iconsPath + "/edit-paste.svg"));
        if (!button)
            button = createButtonFromPNG (itemId, "paste", CabbageBinaryData::editpaste_png, CabbageBinaryData::editpaste_pngSize);
        //return createButtonFromSVG (itemId, "paste", getSVGTextFromMemory (CabbageBinaryData::editpaste_svg, CabbageBinaryData::editpaste_svgSize));
        break;
    case system_prefs:
        button = createButtonFromSVG (itemId, "settings", getSVGTextFromFile (iconsPath + "/preferences-system.svg"));
        if (!button)
            button = createButtonFromPNG (itemId, "settings", CabbageBinaryData::emblemsystem_png, CabbageBinaryData::emblemsystem_pngSize);
        //return createButtonFromSVG (itemId, "settings", getSVGTextFromMemory (CabbageBinaryData::emblemsystem_svg, CabbageBinaryData::emblemsystem_svgSize));
        break;
    case custom_comboBox:
        return (combo = new ToolbarComboBox (itemId));

    case toggle_play:
        button = createButtonFromSVG (itemId, "togglePlay", getSVGTextFromFile (iconsPath + "/audio-muted.svg"), getSVGTextFromFile (iconsPath + "/audio-on.svg"));
        if (!button)
            button = createToggleButtonFromPNG (itemId, "togglePlay", CabbageBinaryData::audiovolumemuted_png, CabbageBinaryData::audiovolumemuted_pngSize,
                                              CabbageBinaryData::audiovolumehigh_png, CabbageBinaryData::audiovolumehigh_pngSize);
        break;
        //return createButtonFromSVG (itemId, "togglePlay", getSVGTextFromMemory (CabbageBinaryData::Audiovolumemuted_svg, CabbageBinaryData::Audiovolumemuted_svgSize),
        //                            getSVGTextFromMemory (CabbageBinaryData::Audiovolumehigh_svg, CabbageBinaryData::Audiovolumehigh_svgSize));

    default:
        break;
    }

    return button;// nullptr;
}


ToolbarButton* CabbageToolbarFactory::createButtonFromPNG (const int itemId, const String& text, const void* png, size_t size, const String onFile)
{
    std::unique_ptr< Drawable>  drawableNormal ( Drawable::createFromImageData (png, size));
    ToolbarButton* button = new ToolbarButton(itemId, text, std::move(drawableNormal), 0);
    button->setTooltip (text);
    button->addListener (owner);
    return button;
}

ToolbarButton* CabbageToolbarFactory::createToggleButtonFromSVG(const int itemId, const String &text, String svgTextOn, String svgTextOff)
{
    std::unique_ptr<XmlElement> svgOn(XmlDocument::parse(svgTextOn));
    std::unique_ptr<Drawable> drawable_armForRecordOn(Drawable::createFromSVG (*svgOn));
    std::unique_ptr<XmlElement> svgOff(XmlDocument::parse(svgTextOff));
    std::unique_ptr<Drawable> drawable_armForRecordOff(Drawable::createFromSVG (*svgOff));
    toggleRecordButton = new ToolbarButton (itemId, text, std::move(drawable_armForRecordOn), std::move(drawable_armForRecordOff));
    toggleRecordButton->setClickingTogglesState (true);
    toggleRecordButton->addListener (owner);
    toggleRecordButton->setToggleState (false, false);
    toggleRecordButton->setTooltip ("Arm for record");
    return toggleRecordButton;
}

ToolbarButton* CabbageToolbarFactory::createToggleButtonFromPNG (const int itemId, const String& text, const void* png1, size_t size1, const void* png2, size_t size2)
{

    std::unique_ptr<Drawable> drawableNormal(Drawable::createFromImageData (png1, size1));
	std::unique_ptr<Drawable> drawableOn(Drawable::createFromImageData (png2, size2));
    togglePlayButton = new ToolbarButton (itemId, text, std::move(drawableNormal), std::move(drawableOn));
    togglePlayButton->setClickingTogglesState (true);
    togglePlayButton->addListener (owner);
    togglePlayButton->setToggleState (true, false);
    togglePlayButton->setTooltip ("Start/Stop Audio Graph");
    return togglePlayButton;
}

ToolbarButton* CabbageToolbarFactory::createButtonFromSVG (const int itemId, const String& text, const String svgFile, const String onFile)
{
    std::unique_ptr<XmlElement> svgNormal (XmlDocument::parse (svgFile));

    if (svgNormal == nullptr)
        return nullptr;//jassert (false);

    std::unique_ptr<Drawable> drawableNormal;

    if (svgNormal != nullptr)
    {
        drawableNormal = Drawable::createFromSVG (*svgNormal);
    }

	std::unique_ptr<XmlElement> svgOn (XmlDocument::parse (onFile));

    if (svgOn != nullptr)
    {
        std::unique_ptr<Drawable> drawableOn;

        if (svgOn != nullptr)
        {
            drawableOn = Drawable::createFromSVG (*svgOn);
        }

        togglePlayButton = new ToolbarButton (itemId, text, std::move(drawableNormal), std::move(drawableOn));
        togglePlayButton->setClickingTogglesState (true);
        togglePlayButton->addListener (owner);
        togglePlayButton->setToggleState (true, false);
        togglePlayButton->setTooltip ("Start/Stop Audio Graph");
        return togglePlayButton;
    }

    ToolbarButton* button = new ToolbarButton (itemId, text, std::move(drawableNormal), 0);
    button->setTooltip (text);
    button->addListener (owner);
    return button;

}
