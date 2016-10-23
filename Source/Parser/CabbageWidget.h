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

//   You should have received a copy of the GNU Lesser General Public
  License along with Csound; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
  02111-1307 USA

*/
#ifndef CABBPARSE_H
#define CABBPARSE_H


#include "../Utilities/CabbageUtilities.h"
#include "../Application/CabbageIds.h"

class CabbageWidget : public ValueTree, public CabbageUtilities
{
    double width, height, top, left;
    Array<int> vuConfig;
    Array<int> tableNumbers;
    Array<float> tableChannelValues;
    String warningMessages;
    bool refreshFromDisk;

public:
    String getWarningMessages()
    {
        return warningMessages;
    };

    CabbageWidget(String str, int ID);
    CabbageWidget() {};
    ~CabbageWidget(){};
    void parse(String str, String identifier);
    float getNumProp(Identifier prop);
    void setNumProp(Identifier prop, float val);
    void setTableChannelValues(int index, float val);
    float getTableChannelValues(int index);
    void addTableChannelValues();
    void setStringProp(Identifier prop, String val);
    void setStringProp(Identifier prop, int index, String value);
    String getStringProp(Identifier prop);
    String getStringProp(Identifier prop, int index);
    String getPropsString();
    String getColourProp(Identifier prop);
    float getNumPropVal(Identifier prop);
    void scaleWidget(Point<float> scale);
    void setNumPropVal(Identifier prop, float val);
    static String getCabbageCodeFromIdentifiers(NamedValueSet props);
    static String getStringForIdentifier(var props, String identifier, String type);

	void setWidgetProperty(Identifier name, const var &value)
	{
		setProperty(name, value, 0);
	}
	
	var getWidgetPropertyWithDefault(Identifier name, const var &value)
	{
		return getProperty(name, value);
	}	

    Rectangle<int> getBounds()
    {
        Rectangle<int> bounds(left, top, width, height);
        return bounds;
    }

    //static methods used for updating look and pos of GUI controls
    static Rectangle<int> getBoundsFromText(String text);
    static Colour getColourFromText(String text);
    static String getTextFromText(String text);
    static Point<int> getSizeFromText(String text);
    static Point<int> getPosFromText(String text);
    static float getSkewFromText(String text);
    static var getVarArrayFromText(String text);

    void setBounds(Rectangle<int> bounds)
    {
        left = bounds.getX();
        top = bounds.getY();
        width = bounds.getWidth();
        height = bounds.getHeight();
    }

    static StringArray getIdentifiers()
    {
        StringArray test;
        return test;
    }

    Rectangle<int> getComponentBounds();
    StringArray getStringArrayProp(Identifier prop);
    String getStringArrayPropValue(Identifier prop, int index);
    int getIntArrayPropValue(Identifier prop, int index);
    Array<int> getIntArrayProp(Identifier prop);
    var getVarArrayProp(Identifier prop);
    float getFloatArrayPropValue(Identifier prop, int index);
    Array<float> getFloatArrayProp(Identifier prop);
    void setStringArrayPropValue(Identifier prop, int index, String value);
    void setStringArrayProp(Identifier prop, var value);

};

#endif
