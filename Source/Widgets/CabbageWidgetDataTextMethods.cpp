/*
 Copyright (C) 2017 Rory Walsh
 
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

#include "../Utilities/CabbageUtilities.h"
#include "../CabbageIds.h"
#include "CabbageWidgetData.h"

constexpr unsigned long long int HashStringToInt (const char* str, unsigned long long int hash = 0)
{
    return (*str == 0) ? hash : 101 * HashStringToInt (str + 1) + *str;
}

//===========================================================================
// these methods will return Cabbage code based on data stored in widget tree
String CabbageWidgetData::getCabbageCodeForIdentifier(ValueTree widgetData, String identifier)
{
    switch (HashStringToInt (identifier.toStdString().c_str()))
    {
        case HashStringToInt ("active"):
        case HashStringToInt ("alpha"):
        case HashStringToInt ("corners"):
        case HashStringToInt ("fill"):
        case HashStringToInt ("increment"):
        case HashStringToInt ("linethickness"):
        case HashStringToInt ("max"):
        case HashStringToInt ("min"):
        case HashStringToInt ("outlinethickness"):
        case HashStringToInt ("trackerthickness"):
        case HashStringToInt ("trackeroutsideradius"):
        case HashStringToInt ("trackerinsideradius"):
        case HashStringToInt ("samplerange"):
        case HashStringToInt ("scrubberposition"):
        case HashStringToInt ("surrogatelinenumber"):
        case HashStringToInt ("value"):
        case HashStringToInt ("valuetextbox"):
        case HashStringToInt ("velocity"):
        case HashStringToInt ("visible"):
        case HashStringToInt ("zoom"):
        case HashStringToInt ("range"):
        case HashStringToInt ("latched"):
        case HashStringToInt ("fontsize"):
        case HashStringToInt ("mouseinteraction"):
            return getNumericalValueTextAsCabbageCode (widgetData, identifier, "").trim();
            
        case HashStringToInt ("align"):
        case HashStringToInt ("channeltype"):
        case HashStringToInt ("file"):
        case HashStringToInt ("mode"):
		case HashStringToInt ("style"):
        case HashStringToInt ("plant"):
        case HashStringToInt ("popuppostfix"):
        case HashStringToInt ("popupprefix"):
        case HashStringToInt("postfix"):
        case HashStringToInt("prefix"):
        case HashStringToInt ("popuptext"):
        case HashStringToInt ("shape"):
        case HashStringToInt ("radiogroup"):
            return getSimpleTextAsCabbageCode(widgetData, identifier, "");
            
            
        case HashStringToInt ("channel"):
        case HashStringToInt ("identchannel"):
        case HashStringToInt ("populate"):
        case HashStringToInt ("tablenumber"):
        case HashStringToInt ("text"):
            return getMultiItemTextAsCabbageCode(widgetData, identifier, "");
            
        

        case HashStringToInt ("bounds"):
            return getBoundsTextAsCabbageCode (getBounds (widgetData));

            
        case HashStringToInt ("type"):
            return getStringProp (widgetData, CabbageIdentifierIds::type);
            
        case HashStringToInt ("pos"):
        case HashStringToInt ("size"):
        case HashStringToInt ("amprange"):
            return getMultiItemNumbersAsCabbageCode (widgetData, identifier, "");
            
        case HashStringToInt ("activecellcolour"):
        case HashStringToInt ("arrowbackgroundcolour"):
        case HashStringToInt ("arrowcolour"):
        case HashStringToInt ("backgroundcolour"):
        case HashStringToInt ("ballcolour"):
        case HashStringToInt ("blacknotecolour"):
        case HashStringToInt ("fillcolour"):
        case HashStringToInt ("highlightcolour"):
        case HashStringToInt ("keyseparatorcolour"):
        case HashStringToInt ("keydowncolour"):
        case HashStringToInt ("menucolour"):
        case HashStringToInt ("mouseoeverkeycolour"):
        case HashStringToInt ("outlinecolour"):
        case HashStringToInt ("overlaycolour"):
        case HashStringToInt ("tablebackgroundcolour"):
        case HashStringToInt ("tablegridcolour"):
        case HashStringToInt ("textboxcolour"):
        case HashStringToInt ("textboxoutlinecolour"):
        case HashStringToInt ("textcolour"):
        case HashStringToInt ("titlebarcolour"):
        case HashStringToInt ("trackercolour"):
        case HashStringToInt ("markercolour"):
        case HashStringToInt ("whitenotecolour"):
        case HashStringToInt ("colour"):
        case HashStringToInt ("fontcolour"):
        case HashStringToInt ("colour:0"):
        case HashStringToInt ("colour:1"):
        case HashStringToInt ("fontcolour:0"):
        case HashStringToInt ("fontcolour:1"):
        case HashStringToInt ("tablecolour"):
        case HashStringToInt ("tablecolours"):
        case HashStringToInt ("tablecolour:"):
        case HashStringToInt ("metercolour"):
        case HashStringToInt ("metercolour:"):
            return getColoursTextAsCabbageCode (widgetData, identifier, "");
            
        case HashStringToInt ("channelarray"):
        case HashStringToInt ("widgetarray"):
            return getWidgetArrayAsCabbageCode (widgetData, "");
            
        case HashStringToInt ("rotate"):
            return getRotateTextAsCabbageCode (widgetData, "");
            
        case HashStringToInt ("imgfile"):
            return getImagesTextAsCabbageCode (widgetData, "");
            
        default:
            return String();
            break;
            
    }
}

String CabbageWidgetData::getCabbageCodeFromIdentifiers (ValueTree widgetData, const String currentLineText)
{
    String returnString = currentLineText;
    StringArray replacedIdentifiers;
    StringArray identifiersInLine = CabbageUtilities::getTokens(currentLineText, ')');
    
    //remove widget type
    const String widgetType = CabbageWidgetData::getStringProp (widgetData, CabbageIdentifierIds::type);
    identifiersInLine.set(0, identifiersInLine[0].substring(identifiersInLine[0].indexOf(" ") + 1));
    CabbageIdentifierStrings fullListOfIdentifierStrings;
    fullListOfIdentifierStrings.sort(true);
    
    var macroNames = CabbageWidgetData::getProperty (widgetData, CabbageIdentifierIds::macronames);
    var macroStrings = CabbageWidgetData::getProperty (widgetData, CabbageIdentifierIds::macrostrings);
    
    
    //deal with macros
    for ( int i = 0 ; i < identifiersInLine.size() ; i++)
    {
        if(identifiersInLine[i].contains("$"))
        {
            const String macroWithIdentifier = identifiersInLine[i].trimCharactersAtStart(", ");
            const String macro = macroWithIdentifier.substring(0, macroWithIdentifier.indexOf(" "));
            const int indexOfmacro = macroNames.indexOf(macro);
            identifiersInLine.set(i, identifiersInLine[i].replace(macro, ""));
            identifiersInLine.insert(i, macro);
            i++;
        }
    }
    

    for (const auto currentIdentifier : identifiersInLine)
    {
        
        const String currentIdentName = currentIdentifier.substring(0, currentIdentifier.indexOf(
                                                                                                 "(")).trim().removeCharacters(", ");
        
        //I need to check that the current identifiers and not the same as the existing ones, if so don't replace anything
        if (currentIdentName.isNotEmpty())
        {
            const String newText = getCabbageCodeForIdentifier(widgetData, currentIdentName).trimCharactersAtEnd(", ");
            const String stringToReplace = currentIdentifier.trimCharactersAtStart(", ") + ")";
            
            replacedIdentifiers.add(newText.substring(0, newText.indexOf("(")));
            if(newText!=stringToReplace && macroStrings.indexOf(newText) == -1)
                returnString = returnString.replace(stringToReplace, newText);
        }
        
    }
    
    identifiersInLine.clear();
    identifiersInLine = CabbageUtilities::getTokens(returnString, ')');
    
    for (auto str : identifiersInLine)
        identifiersInLine.set(identifiersInLine.indexOf(str), str.substring(0, str.indexOf("(")).trimCharactersAtStart(", "));
    
    //remove widget type from first string in array
    identifiersInLine.set(0, identifiersInLine[0].substring(identifiersInLine[0].indexOf(" ") + 1));

    for (auto &ident : fullListOfIdentifierStrings)
    {



        if (replacedIdentifiers.indexOf(ident) == -1)
        {

            const String newIdent = getCabbageCodeForIdentifier(widgetData, ident).trimCharactersAtEnd(
                    ", ").trimCharactersAtStart(",");

            if (newIdent.isNotEmpty() && macroStrings.indexOf(newIdent) == -1)
                returnString = returnString.trimEnd() + " " + newIdent;

        }
    }
    
    if(widgetType == returnString.substring(0, returnString.indexOf(" ")))
        return returnString;
    else
        return widgetType + returnString.trimEnd();
}

String CabbageWidgetData::getBoundsTextAsCabbageCode (Rectangle<int> rect)
{
    const String boundsText = "bounds(" + String (rect.getX()) + ", " + String (rect.getY()) + ", " + String (rect.getWidth()) + ", " + String (rect.getHeight()) + ")";
    return boundsText;
}

String CabbageWidgetData::getNumericalValueTextAsCabbageCode (ValueTree widgetData, String identifier, const String macroText)
{
    ValueTree tempData ("tempTree");
    const String type = getStringProp (widgetData, CabbageIdentifierIds::type);
    setWidgetState (tempData, type + " " + macroText, -99);
    
    if (type.contains ("slider") && identifier == "range")
    {
        return "range(" + String (getNumProp (widgetData, CabbageIdentifierIds::min))
        + ", "
        + String (getNumProp (widgetData, CabbageIdentifierIds::max))
        + ", "
        + String (getNumProp (widgetData, CabbageIdentifierIds::value))
        + ", "
        + String (getNumProp (widgetData, CabbageIdentifierIds::sliderskew))
        + ", "
        + String (getNumProp (widgetData, CabbageIdentifierIds::increment))
        + ")";
    }
    
    if (type.contains ("range") && identifier == "range")
    {
        return "range(" + String (getNumProp (widgetData, CabbageIdentifierIds::min))
        + ", "
        + String (getNumProp (widgetData, CabbageIdentifierIds::max))
        + ", "
        + String (getNumProp (widgetData, CabbageIdentifierIds::minvalue))
        + ":"
        + String (getNumProp (widgetData, CabbageIdentifierIds::maxvalue))
        + ", "
        + String (getNumProp (widgetData, CabbageIdentifierIds::sliderskew))
        + ", "
        + String (getNumProp (widgetData, CabbageIdentifierIds::increment))
        + ")";
    }
    
    else if (identifier == "max" || identifier == "min")
    {
        if (type.contains ("range") || type == CabbageWidgetTypes::encoder)
            return identifier + "(" + String (getNumProp (widgetData, identifier)) + ")";
    }
    else if (type == "xypad" && identifier == "range")
    {
        return "rangex(" + String (getNumProp (widgetData, CabbageIdentifierIds::minx))
        + ", "
        + String (getNumProp (widgetData, CabbageIdentifierIds::maxx))
        + ", "
        + String (getNumProp (widgetData, CabbageIdentifierIds::valuex))
        + ") rangey("
        + String (getNumProp (widgetData, CabbageIdentifierIds::miny))
        + ", "
        + String (getNumProp (widgetData, CabbageIdentifierIds::maxy))
        + ", "
        + String (getNumProp (widgetData, CabbageIdentifierIds::valuey))
        + ")";
    }
    
    else if (type == "gentable" && identifier == "samplerange")
    {
        if (getProperty (widgetData, CabbageIdentifierIds::startpos) != getProperty (tempData, CabbageIdentifierIds::startpos)
            || getProperty (widgetData, CabbageIdentifierIds::endpos) != getProperty (tempData, CabbageIdentifierIds::endpos) )
        {
            return "samplerange(" + String (getNumProp (widgetData, CabbageIdentifierIds::startpos))
            + ", "
            + String (getNumProp (widgetData, CabbageIdentifierIds::endpos))
            + ")";
        }
    }
    
    else if (type == "gentable" && identifier == "scrubberposition")
    {
        if (getProperty (widgetData, CabbageIdentifierIds::scrubberposition) != getProperty (tempData, CabbageIdentifierIds::scrubberposition))
        {
            return "scrubberposition(" + String (getNumProp (widgetData, CabbageIdentifierIds::scrubberposition_sample))
            + ", "
            + String (getNumProp (widgetData, CabbageIdentifierIds::scrubberposition_table))
            + ")";
            
        }
    }
    
    else if ((type == "combobox" || type == "listbox")
             && getProperty (widgetData, CabbageIdentifierIds::channeltype).toString() == "string"
             && identifier == "value")
    {
        return identifier + "(\"" + getProperty (widgetData, identifier).toString() + "\")";
    }
    
    else
    {
//        CabbageUtilities::debug(identifier);
        if (getNumProp (widgetData, identifier) != getNumProp (tempData, identifier))
        {
            if(type.contains ("slider") && identifier != "value" && identifier != "increment")
                return identifier + "(" + String (getNumProp (widgetData, identifier)) + ")";
            else if((type.contains ("slider") && identifier == "increment") || (type.contains ("slider") && identifier == "value"))
                return "";
            else
                return identifier + "(" + String (getNumProp (widgetData, identifier)) + ")";
        }
    }
    
    return String();
}

String CabbageWidgetData::getRotateTextAsCabbageCode (ValueTree widgetData, const String macroText)
{
    ValueTree tempData ("tempTree");
    const String type = getStringProp (widgetData, CabbageIdentifierIds::type);
    setWidgetState (tempData, type + " " + macroText, -99);

if (getNumProp(widgetData, CabbageIdentifierIds::rotate) != getNumProp(tempData, CabbageIdentifierIds::rotate)
    || getNumProp(widgetData, CabbageIdentifierIds::pivotx) != getNumProp(tempData, CabbageIdentifierIds::pivotx)
    || getNumProp(widgetData, CabbageIdentifierIds::pivoty) != getNumProp(tempData, CabbageIdentifierIds::pivoty))
{
    const float rotate = getNumProp(widgetData, CabbageIdentifierIds::rotate);
    const float pivotx = getNumProp(widgetData, CabbageIdentifierIds::pivotx);
    const float pivoty = getNumProp(widgetData, CabbageIdentifierIds::pivoty);

    return "rotate(" + String(rotate) + ", " + String(pivotx) + ", " + String(pivoty) + ")";
}

return String();
}

String CabbageWidgetData::getSimpleTextAsCabbageCode(ValueTree widgetData, String identifier, const String macroText)
{
    ValueTree tempData("tempTree");
    const String type = getStringProp(widgetData, CabbageIdentifierIds::type);
    setWidgetState(tempData, type + " " + macroText, -99);


    if (getStringProp(widgetData, identifier) != getStringProp(tempData, identifier))
    {
        const String text = getStringProp(widgetData, identifier);
        return identifier + "(\"" + text + "\")";
    }

    return String();
}

String CabbageWidgetData::getImagesTextAsCabbageCode(ValueTree widgetData, const String macroText)
{
    ValueTree tempData("tempTree");
    const String type = getStringProp(widgetData, CabbageIdentifierIds::type);
    setWidgetState(tempData, type + " " + macroText, -99);
    String returnText = "";

    if (getStringProp(widgetData, CabbageIdentifierIds::imgbuttonon)
        != getStringProp(tempData, CabbageIdentifierIds::imgbuttonon))
    {
        const String text = getStringProp(widgetData, CabbageIdentifierIds::imgbuttonon);
        returnText = "imgfile(\"On\", \"" + text + "\")";
    }

    if (getStringProp(widgetData, CabbageIdentifierIds::imgbuttonoff)
        != getStringProp(tempData, CabbageIdentifierIds::imgbuttonoff))
    {
        const String text = getStringProp(widgetData, CabbageIdentifierIds::imgbuttonoff);
        returnText = returnText + "imgfile(\"Off\", \"" + text + "\")";
    }

    if (getStringProp(widgetData, CabbageIdentifierIds::imgslider)
        != getStringProp(tempData, CabbageIdentifierIds::imgslider))
    {
        const String text = getStringProp(widgetData, CabbageIdentifierIds::imgslider);
        returnText = returnText + "imgfile(\"Slider\", \"" + text + "\")";
    }

    if (getStringProp(widgetData, CabbageIdentifierIds::imgsliderbg)
        != getStringProp(tempData, CabbageIdentifierIds::imgsliderbg))
    {
        const String text = getStringProp(widgetData, CabbageIdentifierIds::imgsliderbg);
        returnText = returnText + "imgfile(\"Background\", \"" + text + "\")";
    }

    if (getStringProp(widgetData, CabbageIdentifierIds::imggroupbox)
        != getStringProp(tempData, CabbageIdentifierIds::imggroupbox))
    {
        const String text = getStringProp(widgetData, CabbageIdentifierIds::imggroupbox);
        returnText = returnText + "imgfile(\"" + text + "\")";
    }

    return returnText;
}

String CabbageWidgetData::getMultiItemNumbersAsCabbageCode(ValueTree widgetData, String identifier, const String macroText)
{
    var items = getProperty(widgetData, identifier);
    const Array<var>* array = items.getArray();
    ValueTree tempData("tempTree");
    const String type = getStringProp(widgetData, CabbageIdentifierIds::type);
    setWidgetState(tempData, type + " " + macroText, -99);
    var tempItems = getProperty(tempData, identifier);



    if (identifier == "pos")
    {
        const int left = getNumProp(widgetData, CabbageIdentifierIds::left);
        const int top = getNumProp(widgetData, CabbageIdentifierIds::top);
        return identifier + "(" + String(left) + ", "
            + String(top) + ")";
    }
    else if (identifier == "size")
    {
        const int w = getNumProp(widgetData, CabbageIdentifierIds::width);
        const int h = getNumProp(widgetData, CabbageIdentifierIds::height);
        return identifier + "(" + String(w) + ", "
            + String(h) + ")";
    }

    if (array)
    {
        if (identifier == "amprange")
        {
            if (getProperty(widgetData, CabbageIdentifierIds::amprange) != getProperty(tempData, CabbageIdentifierIds::amprange))
            {
                return identifier + "(" + array->getReference(0).toString() + ", "
                    + array->getReference(1).toString() + ", "
                    + array->getReference(2).toString() + ", "
                    + String(float(array->getReference(3)), 4) + ")";
            }

            return String();
        }
    }
    
    return String();
}

String CabbageWidgetData::getMultiItemTextAsCabbageCode (ValueTree widgetData, String identifier, const String macroText)
{

    var items = getProperty(widgetData, identifier);
    const Array<var>* array = items.getArray();
    ValueTree tempData ("tempTree");
    
    const String typeOfWidget = getProperty (widgetData, CabbageIdentifierIds::type);
    setWidgetState (tempData, typeOfWidget + " " + macroText, -99);
    var tempItems = getProperty (tempData, identifier);
    
    if(tempItems.equalsWithSameType(items))
        return String();
    
    identifier = (identifier == CabbageIdentifierIds::importfiles.toString() ? "import" : identifier);
    
    if (typeOfWidget == "gentable" && identifier == "channel")
        return String();
    
    String itemString = "";
    
    if (array)
    {
        
        if (identifier == "tablenumber")
        {
            for (int i = 0 ; i < array->size() - 1 ; i++)
            {
                const String text = array->getReference (array->size() - 1).toString();
                itemString = itemString + array->getReference (i).toString() + ", ";
            }
            
            itemString = itemString + array->getReference (array->size() - 1).toString();
            
            if ( array->size() == 1)
                return identifier + "(" + itemString + "), ";
            else
                return identifier + "(" + itemString + "), ";
        }
        else
        {
            for (int i = 0 ; i < array->size() - 1 ; i++)
            {
                itemString = itemString + "\"" + array->getReference (i).toString() + "\", ";
            }
            
            if ( array->size() == 1)
                return identifier + "(\"" + itemString + array->getReference (array->size() - 1).toString() + "\")";
            else
                return identifier + "(" + itemString + "\"" + array->getReference (array->size() - 1).toString() + "\")";
            
        }
    }
    
    StringArray stringArray;
    stringArray.addLines (items.toString());
    
    if (identifier == "tablenumber")
    {
        for ( int i = 0 ; i < stringArray.size(); i++)
            stringArray.set (i, stringArray[i]);
    }
    else
        for ( int i = 0 ; i < stringArray.size(); i++)
            stringArray.set (i, "\"" + stringArray[i] + "\"");
    
    if (stringArray.joinIntoString ("\n").length() > 0)
        return identifier + "(" + stringArray.joinIntoString (", ") + ")";
    else
        return String();
}

String CabbageWidgetData::getWidgetArrayAsCabbageCode (ValueTree widgetData, const String macroText)
{
    ValueTree tempData ("tempTree");
    const String baseChannel = getStringProp (widgetData, CabbageIdentifierIds::basechannel);
    const int arraySize = getNumProp (widgetData, CabbageIdentifierIds::arraysize);
    
    if (getStringProp (widgetData, CabbageIdentifierIds::basechannel) != getStringProp (tempData, CabbageIdentifierIds::basechannel))
    {
        return String ("widgetarray(\"" + baseChannel + "\", " + String (arraySize) + ")");
    }
    
    return String();
    
}

String CabbageWidgetData::getColoursTextAsCabbageCode (ValueTree widgetData, const String identifier, const String macroText)
{
    ValueTree tempData("tempTree");
    //tempData = widgetData.createCopy();
    const String type = getStringProp(widgetData, CabbageIdentifierIds::type);
    setWidgetState(tempData, type + " " + macroText, -99);
    String colourString;
    
    if (identifier == "colour:0" && type.contains("slider") == false && type != "combobox" && type != "listbox" && type != "image" && type != "gentable" && type != "soundfiler" && type != "encoder" && type != "label" && type!="textbox" && type!="xypad" && type!="groupbox")
    {
        if (getStringProp(widgetData, CabbageIdentifierIds::colour) !=
            getStringProp(tempData, CabbageIdentifierIds::colour))
        {
            const Colour col = Colour::fromString(getStringProp(widgetData, CabbageIdentifierIds::colour));
            colourString = colourString << "colour:0(" << (float) col.getRed() << ", " << (float) col.getGreen() << ", "
            << (float) col.getBlue() << ", " << (float) col.getAlpha() << ")";
        }
    }
    
    else if (identifier == "colour:1")
    {
        if (getStringProp(widgetData, CabbageIdentifierIds::oncolour) !=
            getStringProp(tempData, CabbageIdentifierIds::oncolour))
        {
            const Colour col = Colour::fromString(getStringProp(widgetData, CabbageIdentifierIds::oncolour));
            colourString = colourString << "colour:1(" << (float) col.getRed() << ", " << (float) col.getGreen() << ", "
            << (float) col.getBlue() << ", " << (float) col.getAlpha() << ")";
        }
    }
    
    else if (identifier == "outlinecolour")
    {
        if (getStringProp(widgetData, CabbageIdentifierIds::outlinecolour) !=
            getStringProp(tempData, CabbageIdentifierIds::outlinecolour))
        {
            const Colour col = Colour::fromString(getStringProp(widgetData, CabbageIdentifierIds::outlinecolour));
            colourString = colourString << "outlinecolour(" << (float) col.getRed() << ", " << (float) col.getGreen() << ", "
            << (float) col.getBlue() << ", " << (float) col.getAlpha() << ")";
        }
    }
    
    else if (identifier.contains("tablecolour:"))
    {
        if (getProperty(widgetData, CabbageIdentifierIds::tablecolour) !=
            getProperty(tempData, CabbageIdentifierIds::tablecolour))
        {
            var colours = getProperty(widgetData, CabbageIdentifierIds::tablecolour);
            
            for (int i = 0; i < colours.size(); i++)
            {
                const Colour col = Colour::fromString(colours[i].toString());
                colourString = colourString << "tablecolour:" + String(i) + "(" << (float) col.getRed() << ", "
                << (float) col.getGreen() << ", " << (float) col.getBlue() << ", "
                << (float) col.getAlpha() << ") ";
            }
        }
    }
    
    
    else if (identifier == "fontcolour:1")
    {
        if (getStringProp(widgetData, CabbageIdentifierIds::onfontcolour) !=
            getStringProp(tempData, CabbageIdentifierIds::onfontcolour))
        {
            const Colour col = Colour::fromString(getStringProp(widgetData, CabbageIdentifierIds::onfontcolour));
            String newIdent;
            if(type.contains("button") || type.contains("checkbox") && (identifier == "fontcolour" || identifier == "fontcolour:1"))
                newIdent = "fontcolour:1(";
            else if(type.contains("slider") || type == "label" || type == "csoundoutput" || type == "combobox" || type == "listbox" || type == "eventsequencer" || type == "xypad" || type == "encoder")
                newIdent = "fontcolour(";
            else
                jassertfalse;
            
            colourString = colourString << newIdent << (float) col.getRed()
            << ", " << (float) col.getGreen() << ", " << (float) col.getBlue() << ", "
            << (float) col.getAlpha() << ")";
        }
    }


    else if(identifier=="fontcolour:0" && type != "combobox" && type != "encoder" && type != "label" && type != "listbox" && type != "groupbox" && !type.contains("slider") && type!="textbox" && type!="xypad")
    {
        if (getStringProp(widgetData, CabbageIdentifierIds::fontcolour) !=
            getStringProp(tempData, CabbageIdentifierIds::fontcolour))
        {
            const Colour col = Colour::fromString(getStringProp(widgetData, CabbageIdentifierIds::fontcolour));
            colourString =
            colourString << "fontcolour:0(" << (float) col.getRed() << ", " << (float) col.getGreen() << ", "
            << (float) col.getBlue() << ", " << (float) col.getAlpha() << ")";
        }
    }

    else if(identifier.contains("metercolour:"))
    {
        if (getProperty(widgetData, CabbageIdentifierIds::metercolour) !=
            getProperty(tempData, CabbageIdentifierIds::metercolour))
        {
            var colours = getProperty(widgetData, CabbageIdentifierIds::metercolour);
            
            for (int i = 0; i < colours.size(); i++)
            {
                const Colour col = Colour::fromString(colours[i].toString());
                colourString = colourString << "metercolour:" + String(i) + "(" << (float) col.getRed() << ", "
                << (float) col.getGreen() << ", " << (float) col.getBlue() << ", "
                << (float) col.getAlpha() << ") ";
            }
        }
    }
    else
    {
        if (getStringProp (widgetData, identifier) != getStringProp (tempData, identifier)
            && !type.contains("button")
            && !type.contains("checkbox")
            && !identifier.contains("tablecolour")
            && !type.contains("meter"))
        {
            const Colour col = Colour::fromString (getStringProp (widgetData, identifier));
            colourString << identifier.trim() << "(" << (float)col.getRed() << ", " << (float)col.getGreen() << ", " << (float)col.getBlue() << ", " << (float)col.getAlpha() << ")";
        }
    }

    return colourString;
    
}

