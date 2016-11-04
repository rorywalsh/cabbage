/*
  Copyright (C) 2007 Rory Walsh

  Cabbage is free software; you can redistribute it
  and/or modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  Cabbage is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
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
    String warningMessages;
    bool refreshFromDisk;

public:
    CabbageWidgetData(){};
    ~CabbageWidgetData(){};
	static void setWidgetState(ValueTree widgetData, String lineFromCsd, int ID);
	static void setCustomWidgetState(ValueTree widgetData, String lineFromCsd, String identifier=String::empty);
	//============================================================================
	static void setCheckBoxProperties(ValueTree widgetData, int ID);
	static void setComboBoxProperties(ValueTree widgetData, int ID);
	//============================================================================
    static float getNumProp(ValueTree widgetData, Identifier prop);
    static void setNumProp(ValueTree widgetData, Identifier prop, float val);
    static void setStringProp(ValueTree widgetData, Identifier prop, String val, int index=0);
    static String getStringProp(ValueTree widgetData, Identifier prop, int index=0);
	static Rectangle<int> getBounds(ValueTree widgetData);
	static void setProperty(ValueTree widgetData, Identifier name, const var &value);
	static var getProperty(ValueTree widgetData, Identifier name);
	//============================================================================
	static String getBoundsText(Rectangle<int> rect);
	static String getMultiItemText(ValueTree widgetData, String identifier);
	static String getColoursText(ValueTree widgetData);
	static String getValueText(ValueTree widgetData, String identifier);
	//============================================================================

	static ValueTree getValueTreeForComponent(ValueTree widgetData, String name);


    void setTableChannelValues(int index, float val);
    float getTableChannelValues(int index);
    void addTableChannelValues();

   // static String getPropsString();
    static String getColourProp(ValueTree widgetData, Identifier prop);
  //  static float getNumPropVal(ValueTree widgetData, Identifier prop);
    void scaleWidget(Point<float> scale);
  //  void setNumPropVal(Identifier prop, float val);
    static String getCabbageCodeFromIdentifiers(ValueTree props);
    static String getStringForIdentifier(var props, String identifier, String type);

	static var getWidgetPropertyWithDefault(ValueTree widgetData, Identifier name, const var &value);
    static Rectangle<int> getBoundsFromText(String text);
    static Colour getColourFromText(String text);
    static String getTextFromText(String text);
    static Point<int> getSizeFromText(String text);
    static Point<int> getPosFromText(String text);
    static float getSkewFromText(String text);
    static var getVarArrayFromText(String text);
    

};

#endif
