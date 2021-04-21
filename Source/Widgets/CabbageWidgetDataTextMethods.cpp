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
        case HashStringToInt ("lineThickness"):
        case HashStringToInt ("max"):
        case HashStringToInt ("min"):
        case HashStringToInt ("outlineThickness"):
        case HashStringToInt ("trackerThickness"):
        case HashStringToInt ("trackerOutsideRadius"):
        case HashStringToInt ("trackerInsideRadius"):
        case HashStringToInt ("sampleRange"):
        case HashStringToInt ("scrubberPosition"):
        case HashStringToInt ("surrogatelinenumber"):
        case HashStringToInt ("value"):
        case HashStringToInt ("valueTextBox"):
        case HashStringToInt ("velocity"):
        case HashStringToInt ("visible"):
        case HashStringToInt ("zoom"):
        case HashStringToInt ("sort"):
        case HashStringToInt ("protectedItems"):
        case HashStringToInt ("range"):
        case HashStringToInt ("latched"):
        case HashStringToInt ("fontSize"):
        case HashStringToInt ("mouseInteraction"):
        case HashStringToInt ("automatable"):
            return getNumericalValueTextAsCabbageCode (widgetData, identifier, "").trim();
            
        case HashStringToInt ("align"):
        case HashStringToInt ("channelType"):
        case HashStringToInt ("file"):
        case HashStringToInt ("mode"):
		case HashStringToInt ("style"):
        case HashStringToInt ("plant"):
        case HashStringToInt ("popupPostfix"):
        case HashStringToInt ("popupPrefix"):
        case HashStringToInt ("popupText"):
        case HashStringToInt ("shape"):
        case HashStringToInt ("radioGroup"):
            return getSimpleTextAsCabbageCode(widgetData, identifier, "");
            
            
        case HashStringToInt ("channel"):
        case HashStringToInt ("identChannel"):
        case HashStringToInt ("populate"):
        case HashStringToInt ("tableNumber"):
        case HashStringToInt ("text"):
        case HashStringToInt ("valuePrefix"):
        case HashStringToInt ("valuePostfix"):
            return getMultiItemTextAsCabbageCode(widgetData, identifier, "");

        case HashStringToInt ("bounds"):
            return getBoundsTextAsCabbageCode (getBounds (widgetData));

        case HashStringToInt("filmstrip"):
        case HashStringToInt("filmstripFrames"):
            return getFilmStripTextAsCabbageCode(widgetData, "");

        case HashStringToInt ("type"):
            return getStringProp (widgetData, CabbageIdentifierIds::type);
            

        case HashStringToInt ("ampRange"):
            return getMultiItemNumbersAsCabbageCode (widgetData, identifier, "");
            
        case HashStringToInt ("activeCellColour"):
        case HashStringToInt ("arrowBackGroundcolour"):
        case HashStringToInt ("arrowColour"):
        case HashStringToInt ("backGroundColour"):
        case HashStringToInt ("ballColour"):
        case HashStringToInt ("blackNoteColour"):
        case HashStringToInt ("fillColour"):
        case HashStringToInt ("highlightColour"):
        case HashStringToInt ("keySeparatorColour"):
        case HashStringToInt ("keyDownColour"):
        case HashStringToInt ("menuColour"):
        case HashStringToInt ("mouseOverKeyColour"):
        case HashStringToInt ("outlineColour"):
        case HashStringToInt ("overlayColour"):
        case HashStringToInt ("tableBackGroundcolour"):
        case HashStringToInt ("tableGridColour"):
        case HashStringToInt ("textboxColour"):
        case HashStringToInt ("textboxOutlineColour"):
        case HashStringToInt ("textColour"):
        case HashStringToInt ("titleBarColour"):
        case HashStringToInt ("trackerColour"):
        case HashStringToInt ("markerColour"):
        case HashStringToInt ("whiteNoteColour"):
        case HashStringToInt ("colour"):
        case HashStringToInt ("fontColour"):
        case HashStringToInt ("colour:0"):
        case HashStringToInt ("colour:1"):
        case HashStringToInt ("fontColour:0"):
        case HashStringToInt ("fontColour:1"):
        case HashStringToInt ("tableColour"):
        case HashStringToInt ("tableColours"):
        case HashStringToInt ("tableColour:"):
        case HashStringToInt ("meterColour"):
        case HashStringToInt ("meterColour:"):
            return getColoursTextAsCabbageCode (widgetData, identifier, "");
            
        case HashStringToInt ("channelarray"):
        case HashStringToInt ("widgetArray"):
            return getWidgetArrayAsCabbageCode (widgetData, "");
            
        case HashStringToInt ("rotate"):
            return getRotateTextAsCabbageCode (widgetData, "");
            
        case HashStringToInt ("imgFile"):
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
            identifiersInLine.set(i, identifiersInLine[i].replace(macro, ""));
            identifiersInLine.insert(i, macro);
            i++;
        }
    }
    

    for (const juce::String currentIdentifier : identifiersInLine)
    {

        const String currentIdentName = currentIdentifier.substring(0, currentIdentifier.indexOf(
                                                                                                 "(")).trim().removeCharacters(", ");
            
        //I need to check that the current identifiers are not the same as the existing ones, if so don't replace anything
        if (currentIdentName.isNotEmpty())
        {
            //getCabbageCodeForIdentifier will return multiple imgFile() identifiers, this will create a problem...
            String stringToReplace = currentIdentifier.trimCharactersAtStart(", ") + ")";
            const String newText = getCabbageCodeForIdentifier(widgetData, currentIdentName).trimCharactersAtEnd(", ");
            
            if(CabbageUtilities::getNumberOfOccurances(newText, "imgFile")>1)
            {
                StringArray newImgFileTokens = CabbageUtilities::getTokens(newText, ')');
                for(int i = 0 ; i < newImgFileTokens.size() ; i++)
                {
                    DBG(newImgFileTokens[i]);
                    String testToken = newImgFileTokens[i].trimCharactersAtStart(", ") + ")";
                    replacedIdentifiers.add(testToken.substring(0, testToken.indexOf("(")));

                    if(stringToReplace.isEmpty() && !returnString.contains(testToken))
                    {
                        returnString = returnString + " " + testToken;
                    }
                    else if(testToken!=stringToReplace && macroStrings.indexOf(testToken) == -1 && !returnString.contains(testToken))
                        returnString = " " + returnString.replace(stringToReplace, testToken);
                    else
                        stringToReplace = "";
                }
            }
            else
            {
                replacedIdentifiers.add(newText.substring(0, newText.indexOf("(")));
                if(newText!=stringToReplace && macroStrings.indexOf(newText) == -1)
                    returnString = returnString.replace(stringToReplace, newText);
            }

//            replacedIdentifiers.add(newText.substring(0, newText.indexOf("(")));
//            if(newText!=stringToReplace && macroStrings.indexOf(newText) == -1)
//                returnString = returnString.replace(stringToReplace, newText);
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


String CabbageWidgetData::getFilmStripTextAsCabbageCode(ValueTree widgetData, const String macroText)
{
    ValueTree tempData("tempTree");
    const String type = getStringProp(widgetData, CabbageIdentifierIds::type);
    setWidgetState(tempData, type + " " + macroText, -99);

    const String filmImage = getStringProp(widgetData, CabbageIdentifierIds::filmstripimage);
    const int numberOfFrames = getNumProp(widgetData, CabbageIdentifierIds::filmstripframes);
    const float remove1 = getNumProp(widgetData, CabbageIdentifierIds::filmStripRemoveFrom1);
    const float remove2 = getNumProp(widgetData, CabbageIdentifierIds::filmStripRemoveFrom2);

    if (getStringProp(tempData, CabbageIdentifierIds::filmstripimage) != filmImage)
    {
        if (getNumProp(tempData, CabbageIdentifierIds::filmStripRemoveFrom1) != remove1)
        {
            return "filmstrip(\"" + filmImage + "\", " + String(numberOfFrames) + ", " + String(remove1) + ", " + String(remove2) + ")";
        }
        else
            return "filmstrip(\"" + filmImage + "\", " + String(numberOfFrames) + ")";
    }

    return String();
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
        return "rangeX(" + String (getNumProp (widgetData, CabbageIdentifierIds::minx))
        + ", "
        + String (getNumProp (widgetData, CabbageIdentifierIds::maxx))
        + ", "
        + String (getNumProp (widgetData, CabbageIdentifierIds::valuex))
        + ") rangeY("
        + String (getNumProp (widgetData, CabbageIdentifierIds::miny))
        + ", "
        + String (getNumProp (widgetData, CabbageIdentifierIds::maxy))
        + ", "
        + String (getNumProp (widgetData, CabbageIdentifierIds::valuey))
        + ")";
    }
    
    else if (type == "gentable" && identifier == "sampleRange")
    {
        if (getProperty (widgetData, CabbageIdentifierIds::startpos) != getProperty (tempData, CabbageIdentifierIds::startpos)
            || getProperty (widgetData, CabbageIdentifierIds::endpos) != getProperty (tempData, CabbageIdentifierIds::endpos) )
        {
            return "sampleRange(" + String (getNumProp (widgetData, CabbageIdentifierIds::startpos))
            + ", "
            + String (getNumProp (widgetData, CabbageIdentifierIds::endpos))
            + ")";
        }
    }
    
    else if (type == "gentable" && identifier == "scrubberPosition")
    {
        if (getProperty (widgetData, CabbageIdentifierIds::scrubberposition) != getProperty (tempData, CabbageIdentifierIds::scrubberposition))
        {
            return "scrubberPosition(" + String (getNumProp (widgetData, CabbageIdentifierIds::scrubberposition_sample))
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
        returnText = returnText + "imgFile(\"On\", \"" + text + "\") ";
    }

    if (getStringProp(widgetData, CabbageIdentifierIds::imgbuttonoff)
        != getStringProp(tempData, CabbageIdentifierIds::imgbuttonoff))
    {
        const String text = getStringProp(widgetData, CabbageIdentifierIds::imgbuttonoff);
        returnText = returnText +"imgFile(\"Off\", \"" + text + "\") ";
    }

    if (getStringProp(widgetData, CabbageIdentifierIds::imgslider)
        != getStringProp(tempData, CabbageIdentifierIds::imgslider))
    {
        const String text = getStringProp(widgetData, CabbageIdentifierIds::imgslider);
        returnText = returnText +"imgFile(\"Slider\", \"" + text + "\") ";
    }

    if (getStringProp(widgetData, CabbageIdentifierIds::imgsliderbg)
        != getStringProp(tempData, CabbageIdentifierIds::imgsliderbg))
    {
        const String text = getStringProp(widgetData, CabbageIdentifierIds::imgsliderbg);
        returnText = returnText +"imgFile(\"Background\", \"" + text + "\") ";
    }

    if (getStringProp(widgetData, CabbageIdentifierIds::imggroupbox)
        != getStringProp(tempData, CabbageIdentifierIds::imggroupbox))
    {
        const String text = getStringProp(widgetData, CabbageIdentifierIds::imggroupbox);
        returnText = returnText + "imgFile(\"" + text + "\")";
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



  /* if (identifier == "pos")
    {
        const int left = getNumProp(widgetData, CabbageIdentifierIds::left);
        const int top = getNumProp(widgetData, CabbageIdentifierIds::top);
        const int w = getNumProp(widgetData, CabbageIdentifierIds::width);
        const int h = getNumProp(widgetData, CabbageIdentifierIds::height);
        const String boundsText = getBoundsTextAsCabbageCode(getBounds(widgetData));
        const String boundsTextTemp = getBoundsTextAsCabbageCode(getBounds(tempData));

        if (boundsTextTemp != boundsText)
            return boundsTextTemp;

    }
    else if (identifier == "size")
    {
        const int left = getNumProp(widgetData, CabbageIdentifierIds::left);
        const int top = getNumProp(widgetData, CabbageIdentifierIds::top);
        const int w = getNumProp(widgetData, CabbageIdentifierIds::width);
        const int h = getNumProp(widgetData, CabbageIdentifierIds::height);
        const String boundsText = getBoundsTextAsCabbageCode(getBounds(widgetData));
        const String boundsTextTemp = getBoundsTextAsCabbageCode(getBounds(tempData));
        if (boundsTextTemp != boundsText)
            return boundsTextTemp;
    }*/

    if (array)
    {
        if (identifier == "ampRange")
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
        
        if (identifier == "tableNumber")
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
    
    if (identifier == "tableNumber")
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
        return String ("widgetArray(\"" + baseChannel + "\", " + String (arraySize) + ")");
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
    
    else if (identifier == "outlineColour")
    {
        if (getStringProp(widgetData, CabbageIdentifierIds::outlinecolour) !=
            getStringProp(tempData, CabbageIdentifierIds::outlinecolour))
        {
            const Colour col = Colour::fromString(getStringProp(widgetData, CabbageIdentifierIds::outlinecolour));
            colourString = colourString << "outlineColour(" << (float) col.getRed() << ", " << (float) col.getGreen() << ", "
            << (float) col.getBlue() << ", " << (float) col.getAlpha() << ")";
        }
    }
    
    else if (identifier.contains("tableColour:"))
    {
        if (getProperty(widgetData, CabbageIdentifierIds::tablecolour) !=
            getProperty(tempData, CabbageIdentifierIds::tablecolour))
        {
            var colours = getProperty(widgetData, CabbageIdentifierIds::tablecolour);
            
            for (int i = 0; i < colours.size(); i++)
            {
                const Colour col = Colour::fromString(colours[i].toString());
                colourString = colourString << "tableColour:" + String(i) + "(" << (float) col.getRed() << ", "
                << (float) col.getGreen() << ", " << (float) col.getBlue() << ", "
                << (float) col.getAlpha() << ") ";
            }
        }
    }
    
    else if (identifier == "fontColour:1")
    {
        if (getStringProp(widgetData, CabbageIdentifierIds::onfontcolour) !=
            getStringProp(tempData, CabbageIdentifierIds::onfontcolour))
        {
            const Colour col = Colour::fromString(getStringProp(widgetData, CabbageIdentifierIds::onfontcolour));
            String newIdent;
            if(type.contains("button") || type.contains("checkbox") && (identifier == "fontColour" || identifier == "fontColour:1"))
                newIdent = "fontColour:1(";
            else if(type.contains("slider") || type == "label" || type == "csoundoutput" || type == "combobox" || type == "listbox" || type == "eventsequencer" || type == "xypad" || type == "encoder")
                newIdent = "fontColour(";
            else
                jassertfalse;
            
            colourString = colourString << newIdent << (float) col.getRed()
            << ", " << (float) col.getGreen() << ", " << (float) col.getBlue() << ", "
            << (float) col.getAlpha() << ")";
        }
    }


    else if(identifier=="fontColour:0" && type != "combobox" && type != "encoder" && type != "label" && type != "listbox" && type != "groupbox" && !type.contains("slider") && type!="textbox" && type!="xypad")
    {
        if (getStringProp(widgetData, CabbageIdentifierIds::fontcolour) !=
            getStringProp(tempData, CabbageIdentifierIds::fontcolour))
        {
            const Colour col = Colour::fromString(getStringProp(widgetData, CabbageIdentifierIds::fontcolour));
            colourString =
            colourString << "fontColour:0(" << (float) col.getRed() << ", " << (float) col.getGreen() << ", "
            << (float) col.getBlue() << ", " << (float) col.getAlpha() << ")";
        }
    }

    else if(identifier.contains("meterColour:"))
    {
        if (getProperty(widgetData, CabbageIdentifierIds::metercolour) !=
            getProperty(tempData, CabbageIdentifierIds::metercolour))
        {
            var colours = getProperty(widgetData, CabbageIdentifierIds::metercolour);
            
            for (int i = 0; i < colours.size(); i++)
            {
                const Colour col = Colour::fromString(colours[i].toString());
                colourString = colourString << "meterColour:" + String(i) + "(" << (float) col.getRed() << ", "
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
            && !identifier.contains("tableColour")
            && !identifier.contains("meterColour"))
        {
            const Colour col = Colour::fromString (getStringProp (widgetData, identifier));
            colourString << identifier.trim() << "(" << (float)col.getRed() << ", " << (float)col.getGreen() << ", " << (float)col.getBlue() << ", " << (float)col.getAlpha() << ")";
        }
    }

    return colourString;
    
}

