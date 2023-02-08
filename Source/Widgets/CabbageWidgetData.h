/*
  Copyright (C) 2007 Rory Walsh

  Cabbage is free software; you can redistribute it
  and/or modify it under the terms of the GNU General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  Cabbage is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

   You should have received a copy of the GNU General Public
  License along with Csound; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
  02111-1307 USA

*/
#ifndef CABBPARSE_H
#define CABBPARSE_H


#include "../Utilities/CabbageUtilities.h"
#include "../CabbageIds.h"


class CabbageWidgetData : public CabbageUtilities
{
    //    Array<int> tableNumbers;
    //    Array<float> tableChannelValues;
    //    String warningMessages;
    //    bool refreshFromDisk;

public:

    struct IdentifiersAndParameters
    {
        StringArray identifier;
        StringArray parameter;
    };

    CabbageWidgetData() {}
    ~CabbageWidgetData() {}
    //============================================================================
    static void setWidgetState (ValueTree widgetData, const String lineFromCsd, int ID);
    static void setCustomWidgetState (ValueTree widgetData, const String lineFromCsd);
    //============================================================================
    // these methods are implemented in CabbageWidgetDataInitMethods.h
    static void setCheckBoxProperties (ValueTree widgetData, int ID);
    static void setComboBoxProperties (ValueTree widgetData, int ID);
    static void setImageProperties (ValueTree widgetData, int ID);
    static void setFormProperties (ValueTree widgetData, int ID);
    static void setGroupBoxProperties (ValueTree widgetData, int ID);
    static void setRSliderProperties (ValueTree widgetData, int ID);
    static void setHSliderProperties (ValueTree widgetData, int ID);
    static void setVSliderProperties (ValueTree widgetData, int ID);
    static void setNumberSliderProperties (ValueTree widgetData, int ID);
    static void setCsoundOutputProperties (ValueTree widgetData, int ID);
    static void setKeyboardProperties (ValueTree widgetData, int ID, bool displayOnly);
    static void setTextBoxProperties (ValueTree widgetData, int ID);
    static void setOptionBoxProperties (ValueTree widgetData, int ID);
    static void setLabelProperties (ValueTree widgetData, int ID);
    static void setListBoxProperties (ValueTree widgetData, int ID);
    static void setTextEditorProperties (ValueTree widgetData, int ID);
    static void setEncoderProperties (ValueTree widgetData, int ID);
    static void setSoundfilerProperties (ValueTree widgetData, int ID);
    static void setOptionButtonProperties (ValueTree widgetData, int ID);
    static void setButtonProperties (ValueTree widgetData, int ID);
	static void setUnlockButtonProperties(ValueTree widgetData, int ID);
    static void setFileButtonProperties (ValueTree widgetData, int ID);
    static void setPresetButtonProperties (ValueTree widgetData, int ID);
    static void setInfoButtonProperties (ValueTree widgetData, int ID);
    static void setLoadButtonProperties (ValueTree widgetDatta, int ID);
    static void setSignalDisplayProperties (ValueTree widgetData, int ID);
    static void setGenTableProperties (ValueTree widgetData, int ID);
    static void setXYPadProperties (ValueTree widgetData, int ID);
    static void setHRangeSliderProperties (ValueTree widgetData, int ID);
    static void setVRangeSliderProperties (ValueTree widgetData, int ID);
    static void setLineProperties (ValueTree widgetData, int ID);
    static void setScrewProperties (ValueTree widgetData, int ID);
    static void setPortProperties (ValueTree widgetData, int ID, const String type);
    static void setLightProperties (ValueTree widgetData, int ID);
    static void setMeterProperties (ValueTree widgetData, int ID, bool isVertical);
    static void setEventSequencerProperties (ValueTree widgetData, int ID);
    static void setPathProperties (ValueTree widgetData, int ID);
    //============================================================================
    static float getNumProp (ValueTree widgetData, Identifier prop);
    static void setNumProp (ValueTree widgetData, Identifier prop, float val);
    static void setStringProp (ValueTree widgetData, Identifier prop, const String val);
    static String getStringProp (ValueTree widgetData, Identifier prop);
    static Rectangle<int> getBounds (ValueTree widgetData);
    static void setBounds (ValueTree widgetData, Rectangle<int> rect);
    static void setProperty (ValueTree widgetData, Identifier name, const var& value, ValueTree::Listener *listenerToExclude = nullptr);
    static var getProperty (ValueTree widgetData, Identifier name);
    //============================================================================
    static IdentifiersAndParameters getSetofIdentifiersAndParameters (String lineOfText);
    static var getVarArrayFromTokens (StringArray strTokens);
    static void setPointsFromTokens (ValueTree widgetData, StringArray strTokens);
    static void addFiles (StringArray strToken, ValueTree widgetData, String identifier);
    static void setChannelArrays (StringArray strTokens, ValueTree widgetData, String identifier);
    static void setTextItemArrays (StringArray strTokens, ValueTree widgetData, String typeOfWidget);
    static void setColourArrays (StringArray strTokens, ValueTree widgetData, String identifier, bool isTable = true);
    static void setComboItemArrays (StringArray strTokens, ValueTree widgetData, String identifier);
    static void setImageFiles (StringArray strTokens, ValueTree widgetData, String typeOfWidget);
    static void setShapes (StringArray strTokens, ValueTree widgetData);
    static void setFilmStrip(StringArray strTokens, ValueTree widgetData);
    static void setBounds (StringArray strTokens, ValueTree widgetData);
    static void setRange (StringArray strTokens, ValueTree widgetData, String identifier);
    static void setFontStyle (StringArray strTokens, ValueTree widgetData);
    static void setScrubberPosition (StringArray strTokens, ValueTree widgetData);
	static void setKeyboardDisplayNotes(StringArray strTokens, ValueTree widgetData);
    static void setAmpRange (StringArray strTokens, ValueTree widgetData);
	static void setSideChainChannels(StringArray strTokens, ValueTree widgetData);
    static void setTableNumberArrays (StringArray strTokens, ValueTree widgetData);
    static void setColourByNumber (String string, ValueTree widgetData, String identifier);
    static void setPopulateProps ( StringArray strTokens, ValueTree widgetData);
    static void setCellData(StringArray strTokens, String str, ValueTree widgetData);
	static void setMatrixPrefix(StringArray strTokens, String str, ValueTree widgetData, String identifier);
    static void setMatrixSize(StringArray strTokens, ValueTree widgetData);
    static void setPreAndPostfixes(StringArray strTokens, ValueTree widgetData, String identifier, bool isPopup);
    static String replaceIdentifier (String line, String identifier, String updatedIdentifier);
    //============================================================================
    static Colour getColourFromText (String text);
    static String getCabbageCodeForIdentifier(ValueTree widgetData, const String);
    static String getCabbageCodeFromIdentifiers (ValueTree props, const String);
    //============================================================================
    static void setSVGText(ValueTree widgetData, StringArray tokens);
    static String getBoundsTextAsCabbageCode (Rectangle<int> rect);
    static String getFilmStripTextAsCabbageCode(ValueTree widgetData, const String macroText);
    static String getMultiItemTextAsCabbageCode (ValueTree widgetData, String identifier, const String macroText);
    static String getColoursTextAsCabbageCode (ValueTree widgetData, const String identifier, const String macroText);
    static String getNumericalValueTextAsCabbageCode (ValueTree widgetData, String identifier, const String macroText);
    static String getRotateTextAsCabbageCode (ValueTree widgetData, const String macroText);
    static String getSimpleTextAsCabbageCode (ValueTree widgetData, String identfier, const String macroText);
    static String getImagesTextAsCabbageCode (ValueTree widgetData, const String macroText);
    static String getWidgetArrayAsCabbageCode (ValueTree widgetData, const String macroText);
    static String getMultiItemNumbersAsCabbageCode (ValueTree widgetData, String identifier, const String macroText);
    //============================================================================
    static String removeWidgetFromValueTree (ValueTree widgetData, int lineNumber);
    //============================================================================
    static ValueTree getValueTreeForComponent (ValueTree widgetData, String name, bool byChannel = false);
    static Rectangle<int> getBoundsFromText (String text);
    //const CabbageIdentifierStrings identifierArray;
};

#endif
