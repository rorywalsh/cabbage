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


#include "CabbageWidgetData.h"
#define MAX_MATRIX_SIZE 64

//#include "CabbageWidgetDataInitMethods.cpp"
//===============================================================================
// Main Cabbage abstract GUI class
//===============================================================================
constexpr unsigned long long int HashStringToInt (const char* str, unsigned long long int hash = 0)
{
    return (*str == 0) ? hash : 101 * HashStringToInt (str + 1) + *str;
}
//===============================================================================
void CabbageWidgetData::setWidgetState (ValueTree widgetData, String lineFromCsd, int ID)
{
    setProperty (widgetData, "scalex", 1);
    setProperty (widgetData, "scaley", 1);
    setProperty (widgetData, "resize", 0);
    setProperty (widgetData, CabbageIdentifierIds::presetignore, 0);
    setProperty (widgetData, CabbageIdentifierIds::guimode, CabbageIdentifierIds::polling.toString());
    setProperty (widgetData, CabbageIdentifierIds::automation, 0);
    setProperty (widgetData, CabbageIdentifierIds::active, 1);
    setProperty (widgetData, CabbageIdentifierIds::isparent, 0);
    setProperty (widgetData, CabbageIdentifierIds::automatable, 0);
    setProperty (widgetData, CabbageIdentifierIds::parentdir, "");
    setProperty (widgetData, CabbageIdentifierIds::manufacturer, "CabbageAudio");
    setProperty (widgetData, CabbageIdentifierIds::imgdebug, 0);
    setProperty (widgetData, CabbageIdentifierIds::allowboundsupdate, 0);
    setProperty (widgetData, CabbageIdentifierIds::identchannelmessage, "");
    setProperty (widgetData, CabbageIdentifierIds::popuptext, "");
    setProperty (widgetData, CabbageIdentifierIds::alpha, 1);
    setProperty (widgetData, CabbageIdentifierIds::visible, 1);
    setProperty (widgetData, CabbageIdentifierIds::rotate, 0.f);
    setProperty (widgetData, CabbageIdentifierIds::pivotx, 0.f);
    setProperty (widgetData, CabbageIdentifierIds::pivoty, 0.f);
    setProperty (widgetData, CabbageIdentifierIds::linenumber, ID);
    setProperty (widgetData, CabbageIdentifierIds::decimalplaces, 0);
    setProperty (widgetData, CabbageIdentifierIds::update, 0);
    setProperty (widgetData, CabbageIdentifierIds::arraysize, 0);
    setProperty (widgetData, CabbageIdentifierIds::plant, "");
    setProperty (widgetData, CabbageIdentifierIds::basechannel, "");
    setProperty (widgetData, CabbageIdentifierIds::typeface, "");
    setProperty (widgetData, CabbageIdentifierIds::mouseinteraction, 1);
    setProperty (widgetData, CabbageIdentifierIds::surrogatelinenumber, -99);
    setProperty (widgetData, CabbageIdentifierIds::valueprefix, "");
    setProperty (widgetData, CabbageIdentifierIds::valuepostfix, "");
    setProperty(widgetData, CabbageIdentifierIds::filmstripimage, "");
    setProperty(widgetData, CabbageIdentifierIds::filmstripframes, 31);
    setProperty(widgetData, CabbageIdentifierIds::filmStripRemoveFrom1, 0);
    setProperty(widgetData, CabbageIdentifierIds::filmStripRemoveFrom2, 0);
    setProperty(widgetData, CabbageIdentifierIds::opcodedir, "");

    StringArray strTokens;
    strTokens.addTokens (lineFromCsd, " ", "\"");

    if(strTokens.size()>0)
        setProperty (widgetData, CabbageIdentifierIds::type, strTokens[0].trim());

    setProperty (widgetData, CabbageIdentifierIds::widgetarray, "");

    if (strTokens[0].trim() == String(CabbageWidgetTypes::hslider))
        setHSliderProperties (widgetData, ID);

    else if (strTokens[0].trim() == String(CabbageWidgetTypes::vslider))
        setVSliderProperties (widgetData, ID);

    else if (strTokens[0].trim() == String(CabbageWidgetTypes::rslider))
        setRSliderProperties (widgetData, ID);

    else if (strTokens[0].trim() == String(CabbageWidgetTypes::groupbox))
        setGroupBoxProperties (widgetData, ID);

    else if (strTokens[0].trim() == String(CabbageWidgetTypes::csoundoutput))
        setCsoundOutputProperties (widgetData, ID);

    else if (strTokens[0].trim() == String(CabbageWidgetTypes::keyboard) || 
		strTokens[0].trim() == String(CabbageWidgetTypes::keyboarddisplay))
        setKeyboardProperties (widgetData, ID, (strTokens[0].trim() == "keyboard" ? false : true));

    else if (strTokens[0].trim() == String(CabbageWidgetTypes::form))
        setFormProperties (widgetData, ID);

    else if (strTokens[0].trim() == String(CabbageWidgetTypes::textbox))
        setTextBoxProperties (widgetData, ID);

    else if (strTokens[0].trim() == String(CabbageWidgetTypes::checkbox))
        setCheckBoxProperties (widgetData, ID);

    else if (strTokens[0].trim() == String(CabbageWidgetTypes::nslider))
        setNumberSliderProperties (widgetData, ID);

    else if (strTokens[0].trim() == String(CabbageWidgetTypes::combobox))
        setComboBoxProperties (widgetData, ID);

    else if (strTokens[0].trim() == String(CabbageWidgetTypes::label))
        setLabelProperties (widgetData, ID);

    else if (strTokens[0].trim() == String(CabbageWidgetTypes::listbox))
        setListBoxProperties (widgetData, ID);

    else if (strTokens[0].trim() == String(CabbageWidgetTypes::texteditor))
        setTextEditorProperties (widgetData, ID);

    else if (strTokens[0].trim() == String(CabbageWidgetTypes::image))
        setImageProperties (widgetData, ID);

    else if (strTokens[0].trim() == String(CabbageWidgetTypes::encoder))
        setEncoderProperties (widgetData, ID);

    else if (strTokens[0].trim() == String(CabbageWidgetTypes::hmeter))
        setMeterProperties (widgetData, ID, false);

    else if (strTokens[0].trim() == String(CabbageWidgetTypes::vmeter))
        setMeterProperties (widgetData, ID, true);

    else if (strTokens[0].trim() == String(CabbageWidgetTypes::button))
        setButtonProperties (widgetData, ID);

    else if (strTokens[0].trim() == String(CabbageWidgetTypes::optionbutton))
        setOptionButtonProperties (widgetData, ID);
    
    else if (strTokens[0].trim() == String(CabbageWidgetTypes::soundfiler))
        setSoundfilerProperties (widgetData, ID);

    else if (strTokens[0].trim() == String(CabbageWidgetTypes::filebutton))
        setFileButtonProperties (widgetData, ID);

    else if (strTokens[0].trim() == String(CabbageWidgetTypes::infobutton))
        setInfoButtonProperties (widgetData, ID);

    else if (strTokens[0].trim() == String(CabbageWidgetTypes::line))
        setLineProperties (widgetData, ID);
    
    else if (strTokens[0].trim() == String(CabbageWidgetTypes::cvoutput) || strTokens[0].trim() == String(CabbageWidgetTypes::cvinput))
        setPortProperties (widgetData, ID, strTokens[0].trim());
    
    else if (strTokens[0].trim() == String(CabbageWidgetTypes::screw))
        setScrewProperties (widgetData, ID);

    else if (strTokens[0].trim() == String(CabbageWidgetTypes::light))
        setLightProperties (widgetData, ID);

    else if (strTokens[0].trim() == String(CabbageWidgetTypes::sourcebutton) || strTokens[0].trim() == String(CabbageWidgetTypes::loadbutton))
        setLoadButtonProperties (widgetData, ID);

    else if (strTokens[0].trim() == String(CabbageWidgetTypes::signaldisplay) || strTokens[0].trim() == String(CabbageWidgetTypes::fftdisplay))
        setSignalDisplayProperties (widgetData, ID);

    else if (strTokens[0].trim() == String(CabbageWidgetTypes::xypad))
        setXYPadProperties (widgetData, ID);

    else if (strTokens[0].trim() == String(CabbageWidgetTypes::gentable))
        setGenTableProperties (widgetData, ID);

    else if (strTokens[0].trim() == String(CabbageWidgetTypes::hrange))
        setHRangeSliderProperties (widgetData, ID);

    else if (strTokens[0].trim() == String(CabbageWidgetTypes::vrange))
        setVRangeSliderProperties (widgetData, ID);

    else if (strTokens[0].trim() == String(CabbageWidgetTypes::eventsequencer))
        setEventSequencerProperties (widgetData, ID);

    //===============table==================//
    else if (strTokens[0].trim() == String(CabbageWidgetTypes::table))
    {
        setProperty (widgetData, "basetype", "layout");
        var tableColours;
        tableColours.append ("white");
        tableColours.append ("cornflowerblue");
        tableColours.append ("yellow");
        tableColours.append ("lime");
        tableColours.append ("green");
        tableColours.append ("pink");

        setProperty (widgetData, CabbageIdentifierIds::top, 10);
        setProperty (widgetData, CabbageIdentifierIds::left, 10);
        setProperty (widgetData, CabbageIdentifierIds::width, 400);
        setProperty (widgetData, CabbageIdentifierIds::height, 200);
        setProperty (widgetData, CabbageIdentifierIds::tablenumber, 1);
        setProperty (widgetData, CabbageIdentifierIds::drawmode, "");
        setProperty (widgetData, CabbageIdentifierIds::resizemode, 0);
        setProperty (widgetData, CabbageIdentifierIds::readonly, 0);
        setProperty (widgetData, CabbageIdentifierIds::tablecolour, tableColours);
        setProperty (widgetData, CabbageIdentifierIds::amprange, 0);
        setProperty (widgetData, CabbageIdentifierIds::type, "table");
        setProperty (widgetData, CabbageIdentifierIds::stack, 0);
        setProperty (widgetData, CabbageIdentifierIds::name, getProperty (widgetData, "name").toString() + String (ID));
        setProperty (widgetData, CabbageIdentifierIds::identchannel, "");
        setProperty (widgetData, CabbageIdentifierIds::visible, 1);
    }

    //===============stepper example==================//
    else if (strTokens[0].trim() == "stepper")
    {
        setProperty (widgetData, "basetype", "layout");
        setProperty (widgetData, CabbageIdentifierIds::top, 10);
        setProperty (widgetData, CabbageIdentifierIds::left, 10);
        setProperty (widgetData, CabbageIdentifierIds::width, 160);
        setProperty (widgetData, CabbageIdentifierIds::height, 2);
        setProperty (widgetData, CabbageIdentifierIds::colour, Colours::white.toString());
        setProperty (widgetData, CabbageIdentifierIds::type, "stepper");
        setProperty (widgetData, CabbageIdentifierIds::name, "stepper");
        setProperty (widgetData, CabbageIdentifierIds::name, getProperty (widgetData, "name").toString() + String (ID));
        setProperty (widgetData, CabbageIdentifierIds::identchannel, "");
        setProperty (widgetData, CabbageIdentifierIds::visible, 1);
    }

    //===============non-GUI host widgets==================//
    else if (strTokens[0].trim() == "hostbpm")
    {
        setProperty (widgetData, CabbageIdentifierIds::basetype, "layout");
        setProperty (widgetData, CabbageIdentifierIds::name, "hostbpm");
        setProperty (widgetData, CabbageIdentifierIds::type, "hostbpm");
    }

    else if (strTokens[0].trim() == "hostppqpos")
    {
        setProperty (widgetData, CabbageIdentifierIds::basetype, "layout");
        setProperty (widgetData, CabbageIdentifierIds::name, "hostppqpos");
        setProperty (widgetData, CabbageIdentifierIds::type, "hostppqpos");
    }

    else if (strTokens[0].trim() == "hostplaying")
    {
        setProperty (widgetData, CabbageIdentifierIds::basetype, "layout");
        setProperty (widgetData, CabbageIdentifierIds::name, "hostplaying");
        setProperty (widgetData, CabbageIdentifierIds::type, "hostplaying");
    }

    else if (strTokens[0].trim() == "hostrecording")
    {
        setProperty (widgetData, CabbageIdentifierIds::basetype, "layout");
        setProperty (widgetData, CabbageIdentifierIds::name, "hostrecording");
        setProperty (widgetData, CabbageIdentifierIds::type, "hostrecording");
    }

    else if (strTokens[0].trim() == "hosttime")
    {
        setProperty (widgetData, CabbageIdentifierIds::basetype, "layout");
        setProperty (widgetData, CabbageIdentifierIds::name, "hosttime");
        setProperty (widgetData, CabbageIdentifierIds::type, "hosttime");
    }
    else
    {
        setProperty (widgetData, CabbageIdentifierIds::type, strTokens[0]);
    }

    //parse the text now that all default values ahve been assigned
    setCustomWidgetState (widgetData, lineFromCsd, "");
}

//===========================================================================================
// this method parses the Cabbage text and set each of the Cabbage indentifers
//===========================================================================================
void CabbageWidgetData::setCustomWidgetState (ValueTree widgetData, String lineOfText, String currentIdentifier)
{

    //remove any text after a semicolon and take out tabs..
    lineOfText = lineOfText.replace ("\t", " ");

    if (lineOfText.indexOf (";") > -1)
        lineOfText = lineOfText.substring (0, lineOfText.indexOf (0, ";"));

    if (lineOfText.trim() == "<Cabbage>" || lineOfText.trim() == "</Cabbage>" || lineOfText.trim().isEmpty())
        return;

    lineOfText = lineOfText.trimCharactersAtStart(" ");


    String typeOfWidget = getStringProp(widgetData, CabbageIdentifierIds::type);
    //reduce line ot code to identifiers...
    if(lineOfText.indexOf (typeOfWidget) != -1)
    {
        lineOfText = lineOfText.substring (lineOfText.indexOf (typeOfWidget) + typeOfWidget.length()).trim();
    }

    IdentifiersAndParameters identifierValueSet = getSetofIdentifiersAndParameters (lineOfText);

    CabbageIdentifierStrings identifierArray;


    for ( int indx = 0 ; indx < identifierValueSet.identifier.size() ; indx++)
    {
        //CabbageUtilities::debug(identifierValueSet.identifier[indx]);
        StringArray strTokens;
        String identifier = identifierValueSet.identifier[indx].trimCharactersAtStart (" ");

        if (identifier.indexOf (":") != -1)
            identifier = identifier.substring (0, identifier.indexOf (":") + 1);

        strTokens = CabbageUtilities::getTokens (identifierValueSet.parameter[indx], ',');

        bool isCabbageWidget = (identifier.indexOf("_") != -1 ? false : true);

        switch (HashStringToInt (identifier.toStdString().c_str()))
        {
                //======== strings ===============================
            case HashStringToInt ("kind"):
            case HashStringToInt ("file"):
            case HashStringToInt ("imgPath"):
            case HashStringToInt ("opcodeDir"):
            case HashStringToInt ("pluginId"):
            case HashStringToInt ("popupText"):
            case HashStringToInt ("mode"):
            case HashStringToInt ("channelType"):
            case HashStringToInt ("orientation"):
            case HashStringToInt ("identChannel"):
            case HashStringToInt ("author"):
            case HashStringToInt ("align"):
            case HashStringToInt ("displayType"):
            case HashStringToInt ("radioGroup"):
            case HashStringToInt ("name"):
            case HashStringToInt ("style"):
            case HashStringToInt ("caption"):
            case HashStringToInt ("plant"):
            case HashStringToInt ("show"):
            case HashStringToInt ("child"):
            case HashStringToInt ("typeface"):
            case HashStringToInt ("guiMode"):
            case HashStringToInt ("manufacturer"):
            case HashStringToInt ("logger"):
            case HashStringToInt ("namespace"):
                setProperty (widgetData, identifier, (identifier.contains("fix") ? strTokens[0] : strTokens[0].trim()));
                break;
                
            case HashStringToInt ("valuePrefix"):
            case HashStringToInt ("valuePostfix"):
                setPreAndPostfixes(strTokens, widgetData, identifier, false);
                break;
                
            case HashStringToInt ("popupPrefix"):
            case HashStringToInt ("popupPostfix"):
                setPreAndPostfixes(strTokens, widgetData, identifier, true);
                break;
                
            case HashStringToInt ("channel"):
            case HashStringToInt ("channels"):
                setChannelArrays (strTokens, widgetData, identifier);
                break;
                
            case HashStringToInt ("channelArray"):
            case HashStringToInt ("widgetArray"):
                setChannelArrays (strTokens, widgetData, identifier);
                break;
                
            case HashStringToInt ("items"):
            case HashStringToInt ("text"):
                setTextItemArrays (strTokens, widgetData, getStringProp(widgetData, CabbageIdentifierIds::type));
                break;
            case HashStringToInt ("items:"):
            case HashStringToInt ("text:"):
                setComboItemArrays (strTokens, widgetData, identifierValueSet.identifier[indx]);
                break;
                
            case HashStringToInt ("populate"):
                setPopulateProps (strTokens, widgetData);
                break;
                
            case HashStringToInt ("imgFile"):
                setImageFiles (strTokens, widgetData, getStringProp(widgetData, CabbageIdentifierIds::type));
                break;
                
            case HashStringToInt ("shape"):
                setShapes (strTokens, widgetData);
                break;
                
            case HashStringToInt ("import"):
                addFiles (strTokens, widgetData, "import");
                break;
                
            case HashStringToInt("bundle"):
                addFiles(strTokens, widgetData, "bundle");
                break;
                
            case HashStringToInt("filmstrip"):
                setFilmStrip(strTokens, widgetData);
                break;
                
                
                
                //=========== floats ===============================
            case HashStringToInt ("active"):
            case HashStringToInt ("automation"):
            case HashStringToInt ("automatable"):
            case HashStringToInt ("alpha"):
            case HashStringToInt ("corners"):
            case HashStringToInt ("doubleClickTogglesEdit"):
            case HashStringToInt ("ffttableNumber"):
            case HashStringToInt ("fill"):
            case HashStringToInt ("guiRefresh"):
            case HashStringToInt ("imgdebug"):
            case HashStringToInt ("increment"):
            case HashStringToInt ("keypressBaseOctave"):
            case HashStringToInt ("keyWidth"):
            case HashStringToInt ("latched"):
            case HashStringToInt ("lineThickness"):
            case HashStringToInt ("latency"):
            case HashStringToInt ("markerEnd"):
            case HashStringToInt ("markerStart"):
            case HashStringToInt ("markerThickness"):
            case HashStringToInt ("middleC"):
            case HashStringToInt ("mouseInteraction"):
            case HashStringToInt ("outlineThickness"):
            case HashStringToInt ("pivotX"):
            case HashStringToInt ("pivotY"):
            case HashStringToInt ("presetIgnore"):
            case HashStringToInt ("protectedItems"):
            case HashStringToInt ("readOnly"):
            case HashStringToInt ("scrollbars"):
            case HashStringToInt ("sidechain"):
            case HashStringToInt ("sliderSkew"):
            case HashStringToInt ("surrogatelinenumber"):
            case HashStringToInt ("textBox"):
            case HashStringToInt ("titleBarGradient"):
            case HashStringToInt ("trackerInsideRadius"):
            case HashStringToInt ("trackerOutsideRadius"):
            case HashStringToInt ("trackerThickness"):
            case HashStringToInt ("updateRate"):
            case HashStringToInt ("value"):
            case HashStringToInt ("valueTextBox"):
            case HashStringToInt ("valueX"):
            case HashStringToInt ("scale"):
            case HashStringToInt ("sort"):
            case HashStringToInt ("valueY"):
            case HashStringToInt ("velocitY"):
            case HashStringToInt ("visible"):
            case HashStringToInt ("wrap"):
            case HashStringToInt ("zoom"):
            case HashStringToInt ("showScrubber"):
                if (getStringProp (widgetData, CabbageIdentifierIds::channeltype) == "string")
                    setProperty (widgetData, identifier, strTokens[0].trim());
                else
                    setProperty (widgetData, identifier, strTokens[0].trim().getFloatValue());
                
                break;
            case HashStringToInt ("refreshFiles"):
                setProperty (widgetData, identifier, 1);
            case HashStringToInt ("crop"):
                setProperty (widgetData, CabbageIdentifierIds::cropx, strTokens[0].trim().getFloatValue());
                setProperty (widgetData, CabbageIdentifierIds::cropy, strTokens[1].trim().getFloatValue());
                setProperty (widgetData, CabbageIdentifierIds::cropwidth, strTokens[2].trim().getFloatValue());
                setProperty (widgetData, CabbageIdentifierIds::cropheight, strTokens[3].trim().getFloatValue());
                break;
                
            case HashStringToInt ("max"):
                setProperty (widgetData, CabbageIdentifierIds::maxenabled, 1);
                setProperty (widgetData, CabbageIdentifierIds::max, strTokens[0].trim().getFloatValue());
                break;
                
            case HashStringToInt ("min"):
                setProperty (widgetData, CabbageIdentifierIds::minenabled, 1);
                setProperty (widgetData, CabbageIdentifierIds::min, strTokens[0].trim().getFloatValue());
                break;
                
            case HashStringToInt ("range"):
            case HashStringToInt ("rangeX"):
            case HashStringToInt ("rangeY"):
                setRange (strTokens, widgetData, identifier);
                break;
                
            case HashStringToInt ("rotate"):
                setProperty (widgetData, CabbageIdentifierIds::rotate, strTokens[0].trim().getFloatValue());
                setProperty (widgetData, CabbageIdentifierIds::pivotx, strTokens[1].trim().getFloatValue());
                setProperty (widgetData, CabbageIdentifierIds::pivoty, strTokens[2].trim().getFloatValue());
                break;
                
            case HashStringToInt ("ampRange"):
                setAmpRange (strTokens, widgetData);
                break;
                
            case HashStringToInt ("rescale"):
                
                setProperty (widgetData, CabbageIdentifierIds::scalex, strTokens[0].trim().getFloatValue());
                setProperty (widgetData, CabbageIdentifierIds::scaley, strTokens[1].trim().getFloatValue());
                break;
                
                //============= ints ===========================
            case HashStringToInt ("signalVariable"):
                setProperty (widgetData, CabbageIdentifierIds::signalvariable, getVarArrayFromTokens (strTokens));
                break;
                
            case HashStringToInt ("valueTextBoxBounds"):
            case HashStringToInt ("sliderBounds"):
                setProperty (widgetData, identifier, strTokens);
                break;
            case HashStringToInt ("popup"):
            case HashStringToInt ("numberOfSteps"):
            case HashStringToInt ("showstepnumbers"):
            case HashStringToInt ("bpm"):
            case HashStringToInt ("cellWidth"):
            case HashStringToInt ("cellHeight"):
            case HashStringToInt ("resize"):
            case HashStringToInt ("fontSize"):
            case HashStringToInt ("gapMarkers"):
                setProperty (widgetData, identifier, strTokens[0].trim().getIntValue());
                break;
                break;
                
            case HashStringToInt ("tableNumber"):
            case HashStringToInt ("tableNumbers"):
                setTableNumberArrays (strTokens, widgetData);
                break;
                
            case HashStringToInt ("size"):
                if (strTokens.size() >= 2)
                {
                    setProperty (widgetData, CabbageIdentifierIds::width, strTokens[0].trim().getFloatValue());
                    setProperty (widgetData, CabbageIdentifierIds::height, strTokens[1].trim().getFloatValue());
                    setProperty(widgetData, CabbageIdentifierIds::top, getProperty(widgetData, CabbageIdentifierIds::top));
                    setProperty(widgetData, CabbageIdentifierIds::left, getProperty(widgetData, CabbageIdentifierIds::left));
                }
                
                break;
                
            case HashStringToInt ("bounds"):
                setBounds (strTokens, widgetData);
                break;
                
            case HashStringToInt ("pos"):
                setProperty (widgetData, CabbageIdentifierIds::left, strTokens[0].trim().getFloatValue());
                setProperty (widgetData, CabbageIdentifierIds::top, strTokens[1].trim().getFloatValue());
                setProperty (widgetData, CabbageIdentifierIds::width, getProperty(widgetData, CabbageIdentifierIds::width));
                setProperty (widgetData, CabbageIdentifierIds::height, getProperty(widgetData, CabbageIdentifierIds::height));
                break;
                
            case HashStringToInt ("fontStyle"):
                setFontStyle (strTokens, widgetData);
                break;
                
            case HashStringToInt ("scrubberPosition"):
                setScrubberPosition (strTokens, widgetData);
                break;
                
            case HashStringToInt("keyPressed"):
                setKeyboardDisplayNotes(strTokens, widgetData);
                break;
                
            case HashStringToInt ("sampleRange"):
                setProperty (widgetData, CabbageIdentifierIds::startpos, strTokens[0].trim().getFloatValue());
                
                if (strTokens.size() > 1)
                    setProperty (widgetData, CabbageIdentifierIds::endpos, strTokens[1].trim().getFloatValue());
                else
                    setProperty (widgetData, CabbageIdentifierIds::endpos, -1);
                
                break;
                
                //============ colours ===================
            case HashStringToInt ("fontColour:0"):
            case HashStringToInt ("menuColour"):
            case HashStringToInt ("tableBackgroundColour"):
            case HashStringToInt ("overlayColour"):
            case HashStringToInt ("backgroundColour"):
            case HashStringToInt ("keySeparatorColour"):
            case HashStringToInt ("blackNoteColour"):
            case HashStringToInt ("keyDownColour"):
            case HashStringToInt ("whiteNoteColour"):
            case HashStringToInt ("mouseOverkeyColour"):
            case HashStringToInt ("arrowBackgroundColour"):
            case HashStringToInt ("arrowColour"):
            case HashStringToInt ("ballColour"):
            case HashStringToInt ("caretColour"):
            case HashStringToInt ("fillColour"):
            case HashStringToInt ("tableGridColour"):
            case HashStringToInt ("trackerColour"):
            case HashStringToInt ("trackerBgColour"):
            case HashStringToInt ("highlightColour"):
            case HashStringToInt ("activeCellColour"):
            case HashStringToInt ("titleBarColour"):
            case HashStringToInt ("outlineColour"):
            case HashStringToInt ("textColour"):
            case HashStringToInt ("textBoxColour"):
            case HashStringToInt ("textBoxOutlineColour"):
            case HashStringToInt ("markerColour"):
                setProperty (widgetData, identifier, getColourFromText (strTokens.joinIntoString (",")).toString());
                break;
                
            case HashStringToInt ("colour:"):
            case HashStringToInt ("colour"):
            case HashStringToInt ("fontColour:"):
            case HashStringToInt ("fontColour"):
                setColourByNumber (strTokens.joinIntoString (","), widgetData, identifierValueSet.identifier[indx]);
                break;
                
            case HashStringToInt ("tableColour"):
            case HashStringToInt ("tableColours"):
            case HashStringToInt ("tableColour:"):
                setColourArrays (strTokens, widgetData, identifierValueSet.identifier[indx]);
                break;
                
            case HashStringToInt ("meterColour"):
            case HashStringToInt ("meterColour:"):
                setColourArrays (strTokens, widgetData, identifierValueSet.identifier[indx], false);
                break;
                
                //matrix event props
            case HashStringToInt ("cellData"):
                setCellData(strTokens, identifierValueSet.parameter[indx], widgetData);
                break;
            case HashStringToInt("rowPrefix"):
            case HashStringToInt("colPrefix"):
                setMatrixPrefix(strTokens, identifierValueSet.parameter[indx], widgetData, identifierValueSet.identifier[indx]);
                break;
            case HashStringToInt("matrixSize"):
                setMatrixSize(strTokens, widgetData);
            default:
                if(!isCabbageWidget)
                    setProperty (widgetData, identifier, strTokens);
                break;
                
        }
//        switch (HashStringToInt (identifier.toStdString().c_str()))
//        {
//            //======== strings ===============================
//            case HashStringToInt ("kind"):
//            case HashStringToInt ("file"):
//            case HashStringToInt ("imgpath"):
//            case HashStringToInt ("opcodedir"):
//            case HashStringToInt ("pluginid"):
//            case HashStringToInt ("popuptext"):
//            case HashStringToInt ("mode"):
//            case HashStringToInt ("channeltype"):
//            case HashStringToInt ("orientation"):
//            case HashStringToInt ("identchannel"):
//            case HashStringToInt ("author"):
//            case HashStringToInt ("align"):
//            case HashStringToInt ("displaytype"):
//            case HashStringToInt ("radiogroup"):
//            case HashStringToInt ("name"):
//            case HashStringToInt ("style"):
//            case HashStringToInt ("caption"):
//            case HashStringToInt ("plant"):
//            case HashStringToInt ("show"):
//            case HashStringToInt ("child"):
//            case HashStringToInt ("typeface"):
//            case HashStringToInt ("guimode"):
//            case HashStringToInt ("manufacturer"):
//            case HashStringToInt ("logger"):
//            case HashStringToInt ("namespace"):
//                setProperty (widgetData, identifier, (identifier.contains("fix") ? strTokens[0] : strTokens[0].trim()));
//                break;
//
//            case HashStringToInt ("valueprefix"):
//            case HashStringToInt ("valuepostfix"):
//                setPreAndPostfixes(strTokens, widgetData, identifier, false);
//                break;
//
//            case HashStringToInt ("popupprefix"):
//            case HashStringToInt ("popuppostfix"):
//                setPreAndPostfixes(strTokens, widgetData, identifier, true);
//                break;
//
//            case HashStringToInt ("channel"):
//            case HashStringToInt ("channels"):
//                setChannelArrays (strTokens, widgetData, identifier);
//                break;
//
//            case HashStringToInt ("channelarray"):
//            case HashStringToInt ("widgetarray"):
//                setChannelArrays (strTokens, widgetData, identifier);
//                break;
//
//            case HashStringToInt ("items"):
//            case HashStringToInt ("text"):
//                setTextItemArrays (strTokens, widgetData, getStringProp(widgetData, CabbageIdentifierIds::type));
//                break;
//            case HashStringToInt ("items:"):
//            case HashStringToInt ("text:"):
//                setComboItemArrays (strTokens, widgetData, identifierValueSet.identifier[indx]);
//                break;
//
//            case HashStringToInt ("populate"):
//                setPopulateProps (strTokens, widgetData);
//                break;
//
//            case HashStringToInt ("imgfile"):
//                setImageFiles (strTokens, widgetData, getStringProp(widgetData, CabbageIdentifierIds::type));
//                break;
//
//            case HashStringToInt ("shape"):
//                setShapes (strTokens, widgetData);
//                break;
//
//            case HashStringToInt ("import"):
//                addFiles (strTokens, widgetData, "import");
//                break;
//
//            case HashStringToInt("bundle"):
//                addFiles(strTokens, widgetData, "bundle");
//                break;
//
//            case HashStringToInt("filmstrip"):
//                setFilmStrip(strTokens, widgetData);
//                break;
//
//
//
//            //=========== floats ===============================
//            case HashStringToInt ("active"):
//            case HashStringToInt ("automation"):
//            case HashStringToInt ("automatable"):
//            case HashStringToInt ("alpha"):
//            case HashStringToInt ("corners"):
//            case HashStringToInt ("ffttablenumber"):
//            case HashStringToInt ("fill"):
//            case HashStringToInt ("guirefresh"):
//            case HashStringToInt ("imgdebug"):
//            case HashStringToInt ("increment"):
//            case HashStringToInt ("keypressbaseoctave"):
//            case HashStringToInt ("keywidth"):
//            case HashStringToInt ("latched"):
//            case HashStringToInt ("linethickness"):
//            case HashStringToInt ("latency"):
//            case HashStringToInt ("markerend"):
//            case HashStringToInt ("markerstart"):
//            case HashStringToInt ("markerthickness"):
//            case HashStringToInt ("middlec"):
//            case HashStringToInt ("mouseinteraction"):
//            case HashStringToInt ("outlinethickness"):
//            case HashStringToInt ("pivotx"):
//            case HashStringToInt ("pivoty"):
//            case HashStringToInt ("readonly"):
//            case HashStringToInt ("scrollbars"):
//            case HashStringToInt ("sidechain"):
//            case HashStringToInt ("sliderskew"):
//            case HashStringToInt ("surrogatelinenumber"):
//            case HashStringToInt ("textbox"):
//            case HashStringToInt ("titlebargradient"):
//            case HashStringToInt ("trackerinsideradius"):
//            case HashStringToInt ("trackeroutsideradius"):
//            case HashStringToInt ("trackerthickness"):
//            case HashStringToInt ("updaterate"):
//            case HashStringToInt ("value"):
//            case HashStringToInt ("valuetextbox"):
//            case HashStringToInt ("valuex"):
//            case HashStringToInt ("scale"):
//            case HashStringToInt ("valuey"):
//            case HashStringToInt ("velocity"):
//            case HashStringToInt ("visible"):
//            case HashStringToInt ("wrap"):
//            case HashStringToInt ("zoom"):
//            case HashStringToInt ("showscrubber"):
//                if (getStringProp (widgetData, CabbageIdentifierIds::channeltype) == "string")
//                    setProperty (widgetData, identifier, strTokens[0].trim());
//                else
//                    setProperty (widgetData, identifier, strTokens[0].trim().getFloatValue());
//
//                break;
//            case HashStringToInt ("refreshfiles"):
//                setProperty (widgetData, identifier, 1);
//            case HashStringToInt ("crop"):
//                setProperty (widgetData, CabbageIdentifierIds::cropx, strTokens[0].trim().getFloatValue());
//                setProperty (widgetData, CabbageIdentifierIds::cropy, strTokens[1].trim().getFloatValue());
//                setProperty (widgetData, CabbageIdentifierIds::cropwidth, strTokens[2].trim().getFloatValue());
//                setProperty (widgetData, CabbageIdentifierIds::cropheight, strTokens[3].trim().getFloatValue());
//                break;
//
//            case HashStringToInt ("max"):
//                setProperty (widgetData, CabbageIdentifierIds::maxenabled, 1);
//                setProperty (widgetData, CabbageIdentifierIds::max, strTokens[0].trim().getFloatValue());
//                break;
//
//            case HashStringToInt ("min"):
//                setProperty (widgetData, CabbageIdentifierIds::minenabled, 1);
//                setProperty (widgetData, CabbageIdentifierIds::min, strTokens[0].trim().getFloatValue());
//                break;
//
//            case HashStringToInt ("range"):
//            case HashStringToInt ("rangex"):
//            case HashStringToInt ("rangey"):
//                setRange (strTokens, widgetData, identifier);
//                break;
//
//            case HashStringToInt ("rotate"):
//                setProperty (widgetData, CabbageIdentifierIds::rotate, strTokens[0].trim().getFloatValue());
//                setProperty (widgetData, CabbageIdentifierIds::pivotx, strTokens[1].trim().getFloatValue());
//                setProperty (widgetData, CabbageIdentifierIds::pivoty, strTokens[2].trim().getFloatValue());
//                break;
//
//            case HashStringToInt ("amprange"):
//                setAmpRange (strTokens, widgetData);
//                break;
//
//            case HashStringToInt ("rescale"):
//
//                setProperty (widgetData, CabbageIdentifierIds::scalex, strTokens[0].trim().getFloatValue());
//                setProperty (widgetData, CabbageIdentifierIds::scaley, strTokens[1].trim().getFloatValue());
//                break;
//
//            //============= ints ===========================
//            case HashStringToInt ("signalvariable"):
//                setProperty (widgetData, CabbageIdentifierIds::signalvariable, getVarArrayFromTokens (strTokens));
//                break;
//
//            case HashStringToInt ("popup"):
//            case HashStringToInt ("numberofsteps"):
//            case HashStringToInt ("showstepnumbers"):
//            case HashStringToInt ("bpm"):
//            case HashStringToInt ("cellwidth"):
//            case HashStringToInt ("cellheight"):
//            case HashStringToInt ("resize"):
//            case HashStringToInt ("fontsize"):
//            case HashStringToInt ("gapmarkers"):
//                setProperty (widgetData, identifier, strTokens[0].trim().getIntValue());
//                break;
//                break;
//
//            case HashStringToInt ("tablenumber"):
//            case HashStringToInt ("tablenumbers"):
//                setTableNumberArrays (strTokens, widgetData);
//                break;
//
//            case HashStringToInt ("size"):
//                if (strTokens.size() >= 2)
//                {
//                    setProperty (widgetData, CabbageIdentifierIds::width, strTokens[0].trim().getFloatValue());
//                    setProperty (widgetData, CabbageIdentifierIds::height, strTokens[1].trim().getFloatValue());
//                    setProperty(widgetData, CabbageIdentifierIds::top, getProperty(widgetData, CabbageIdentifierIds::top));
//                    setProperty(widgetData, CabbageIdentifierIds::left, getProperty(widgetData, CabbageIdentifierIds::left));
//                }
//
//                break;
//
//            case HashStringToInt ("bounds"):
//                setBounds (strTokens, widgetData);
//                break;
//
//            case HashStringToInt ("pos"):
//                setProperty (widgetData, CabbageIdentifierIds::left, strTokens[0].trim().getFloatValue());
//                setProperty (widgetData, CabbageIdentifierIds::top, strTokens[1].trim().getFloatValue());
//                setProperty (widgetData, CabbageIdentifierIds::width, getProperty(widgetData, CabbageIdentifierIds::width));
//                setProperty (widgetData, CabbageIdentifierIds::height, getProperty(widgetData, CabbageIdentifierIds::height));
//                break;
//
//            case HashStringToInt ("fontstyle"):
//                setFontStyle (strTokens, widgetData);
//                break;
//
//            case HashStringToInt ("scrubberposition"):
//                setScrubberPosition (strTokens, widgetData);
//                break;
//
//            case HashStringToInt("keypressed"):
//                setKeyboardDisplayNotes(strTokens, widgetData);
//                break;
//
//            case HashStringToInt ("samplerange"):
//                setProperty (widgetData, CabbageIdentifierIds::startpos, strTokens[0].trim().getFloatValue());
//
//                if (strTokens.size() > 1)
//                    setProperty (widgetData, CabbageIdentifierIds::endpos, strTokens[1].trim().getFloatValue());
//                else
//                    setProperty (widgetData, CabbageIdentifierIds::endpos, -1);
//
//                break;
//
//            //============ colours ===================
//            case HashStringToInt ("fontcolour:0"):
//            case HashStringToInt ("menucolour"):
//            case HashStringToInt ("tablebackgroundcolour"):
//            case HashStringToInt ("overlaycolour"):
//            case HashStringToInt ("backgroundcolour"):
//            case HashStringToInt ("keyseparatorcolour"):
//            case HashStringToInt ("blacknotecolour"):
//            case HashStringToInt ("keydowncolour"):
//            case HashStringToInt ("whitenotecolour"):
//            case HashStringToInt ("mouseoverkeycolour"):
//            case HashStringToInt ("arrowbackgroundcolour"):
//            case HashStringToInt ("arrowcolour"):
//            case HashStringToInt ("ballcolour"):
//            case HashStringToInt ("fillcolour"):
//            case HashStringToInt ("tablegridcolour"):
//            case HashStringToInt ("trackercolour"):
//            case HashStringToInt ("trackerbgcolour"):
//            case HashStringToInt ("highlightcolour"):
//            case HashStringToInt ("activecellcolour"):
//            case HashStringToInt ("titlebarcolour"):
//            case HashStringToInt ("outlinecolour"):
//            case HashStringToInt ("textcolour"):
//            case HashStringToInt ("textboxcolour"):
//            case HashStringToInt ("textboxoutlinecolour"):
//            case HashStringToInt ("markercolour"):
//                setProperty (widgetData, identifier, getColourFromText (strTokens.joinIntoString (",")).toString());
//                break;
//
//            case HashStringToInt ("colour:"):
//            case HashStringToInt ("colour"):
//            case HashStringToInt ("fontcolour:"):
//            case HashStringToInt ("fontcolour"):
//                setColourByNumber (strTokens, widgetData, identifierValueSet.identifier[indx]);
//                break;
//
//            case HashStringToInt ("tablecolour"):
//            case HashStringToInt ("tablecolours"):
//            case HashStringToInt ("tablecolour:"):
//                setColourArrays (strTokens, widgetData, identifierValueSet.identifier[indx]);
//                break;
//
//            case HashStringToInt ("metercolour"):
//            case HashStringToInt ("metercolour:"):
//                setColourArrays (strTokens, widgetData, identifierValueSet.identifier[indx], false);
//                break;
//
//            //matrix event props
//            case HashStringToInt ("celldata"):
//                setCellData(strTokens, identifierValueSet.parameter[indx], widgetData);
//                break;
//            case HashStringToInt("rowprefix"):
//            case HashStringToInt("colprefix"):
//                setMatrixPrefix(strTokens, identifierValueSet.parameter[indx], widgetData, identifierValueSet.identifier[indx]);
//                break;
//            case HashStringToInt("matrixsize"):
//                setMatrixSize(strTokens, widgetData);
//            default:
//                if(!isCabbageWidget)
//                    setProperty (widgetData, identifier, strTokens);
//                break;
//
//        }
    }
}
//===========================================================================================
//Methods used by setCustomWidgetState() method
var CabbageWidgetData::getVarArrayFromTokens (StringArray strTokens)
{
    var array;
    array.append (strTokens[0].trim());

    for (int i = 1; i < strTokens.size(); i++)
    {
        array.append (strTokens[i].trim());
    }

    return array;
}

CabbageWidgetData::IdentifiersAndParameters CabbageWidgetData::getSetofIdentifiersAndParameters (String lineOfText)
{
    StringArray identifiersInLine = CabbageUtilities::getTokens (lineOfText.substring (0, lineOfText.lastIndexOf (")")+1).trimCharactersAtStart ("), "), ')');
//    CabbageUtilities::debug(identifiersInLine.joinIntoString(" - "));
    StringArray parameters;

    for ( int i = 0 ; i < identifiersInLine.size() ; i++)
        identifiersInLine.set (i, identifiersInLine[i].trim().trimCharactersAtStart (" ,") + ")");

    for ( int i = 0 ; i < identifiersInLine.size() ; i++)
    {
        String parameter = identifiersInLine[i];
        parameter = parameter.substring (parameter.indexOf ("(") + 1, parameter.lastIndexOf (")")).trimCharactersAtStart ("\"").trimCharactersAtEnd ("\"");
        parameters.add (parameter);
    }

    for ( int i = 0 ; i < identifiersInLine.size() ; i++)
    {
        const String newToken = identifiersInLine[i];
        identifiersInLine.set (i, newToken.substring (0, newToken.indexOf ("(")));
    }

    IdentifiersAndParameters valueSet;
    identifiersInLine.removeEmptyStrings();

    for ( int i = 0 ; i < identifiersInLine.size() ; i++)
    {
        valueSet.identifier.add (identifiersInLine[i].removeCharacters(" "));
        valueSet.parameter.add (parameters[i].removeCharacters ("\""));
    }


    return valueSet;

}

String CabbageWidgetData::replaceIdentifier (String line, String identifier, String updatedIdentifier)
{
    if(identifier == CabbageIdentifierIds::importfiles.toString())
        identifier = "import";

    if (identifier.length() < 2)
        return line;

    if (updatedIdentifier.length() < 2)
        return line;

    int startPos = line.indexOf (identifier);

    if (startPos == -1)
        return line + " " + updatedIdentifier;

    String firstSection = line.substring (0, line.indexOf (identifier));
    line = line.substring (line.indexOf (identifier));
    String secondSection = line.substring (line.indexOf (")") + 1);

    return firstSection + updatedIdentifier + secondSection;
}
//=========================================================================================================
void CabbageWidgetData::setChannelArrays (StringArray strTokens, ValueTree widgetData, String identifier)
{


    var array;
    array.append (strTokens[0].trim());

    if (identifier != CabbageIdentifierIds::widgetarray.toString())
    {
        for (int i = 1; i < strTokens.size(); i++)
        {
            array.append (strTokens[i].trim());
        }

        setProperty (widgetData, CabbageIdentifierIds::channel, array);

        if (array.size() == 2)
        {
            setProperty (widgetData, CabbageIdentifierIds::xchannel, array[0]);
            setProperty (widgetData, CabbageIdentifierIds::ychannel, array[1]);
        }
    }
    else
    {
        var channelArray, identChannelArray;
        int size = strTokens[1].getIntValue();

        for (int i = 0; i < size; i++)
        {
           String chnny = (String (strTokens[0]) + String (i + 1));
            channelArray.append (String (strTokens[0]) + String (i + 1));
            identChannelArray.append (String (strTokens[0]) + String ("_ident") + String (i + 1));
        }

        setProperty (widgetData, CabbageIdentifierIds::widgetarray, channelArray);
        setProperty (widgetData, CabbageIdentifierIds::arraysize, size);
        setProperty (widgetData, CabbageIdentifierIds::basechannel, strTokens[0]);
        setProperty (widgetData, CabbageIdentifierIds::identchannelarray, identChannelArray);

    }
}

void CabbageWidgetData::setMatrixPrefix(StringArray strTokens, String parameters, ValueTree widgetData, String identifier)
{
	var currentPrefixes = getProperty(widgetData, identifier);
	StringArray prefixes;
	StringArray tokens;

	if (currentPrefixes.size() < MAX_MATRIX_SIZE)
		for (int i = 0; i < MAX_MATRIX_SIZE; i++)
			prefixes.add("");
	else
		for (int i = 0 ; i < currentPrefixes.size() ; i++)
			prefixes.add(currentPrefixes[i].toString());

	var nums;
	if (strTokens[0].contains(":") && strTokens.size() > 0)
		tokens.addTokens(strTokens[0], ":", "");
	else
		tokens.add(strTokens[0]);

	while (parameters.indexOf(",") != -1)
		parameters = parameters.substring(parameters.indexOf(",") + 1);


	for (auto num : tokens)
		prefixes.set(num.getIntValue(), parameters.replace("\\", "\""));

	//if (identifier == identifier)
		setProperty(widgetData, identifier, prefixes);
	//else //rowprefix
	//	setProperty(widgetData, "rowprefix", prefixes);

}



void CabbageWidgetData::setMatrixSize(StringArray strTokens, ValueTree widgetData)
{
    setNumProp(widgetData, CabbageIdentifierIds::matrixrows, strTokens[0].getIntValue());
    setNumProp(widgetData, CabbageIdentifierIds::matrixcols, strTokens[1].getIntValue());
}

void CabbageWidgetData::setPreAndPostfixes(StringArray strTokens, ValueTree widgetData, String identifier, bool isPopup)
{
    var array;
    
    if (isPopup)
    {
        for (const auto& str : strTokens)
        {
            array.append(CabbageUtilities::convertWhitespaceEscapeChars(str));
        }
    }
    else
    {
        for (const auto& str : strTokens)
        {
            array.append(CabbageUtilities::removeWhitespaceEscapeChars(str));
        }
    }
    
    setProperty(widgetData, identifier, array);
}

void CabbageWidgetData::setCellData(StringArray strTokens, String parameters, ValueTree widgetData)
{
    var props;
    props.append(strTokens[0].getIntValue());
    props.append(strTokens[1].getIntValue());

    while(parameters.indexOf(",")!=-1)
        parameters =  parameters.substring(parameters.indexOf(",")+1);

    props.append(parameters.replace("\\", "\""));
    setProperty (widgetData, CabbageIdentifierIds::celldata, props);
}

void CabbageWidgetData::addFiles (StringArray strTokens, ValueTree widgetData, String identifier)
{
    var files;

    for (int i = 0; i < strTokens.size(); i++)
    {
        files.append (strTokens[i].trim());
    }
    if(identifier == "import")
        setProperty (widgetData, CabbageIdentifierIds::importfiles, files);
    else if(identifier == "bundle")
        setProperty (widgetData, CabbageIdentifierIds::bundle, files);
}


void CabbageWidgetData::setTextItemArrays (StringArray strTokens, ValueTree widgetData, String typeOfWidget)
{
    var value;
    int comboRange = 0;

    for (int i = 0; i < strTokens.size(); i++)
    {
        value.append (strTokens[i].trim().trimCharactersAtEnd ("\"").trimCharactersAtStart ("\""));
        comboRange = i + 1;
    }

    if ((value.size() == 1) && ((typeOfWidget == "button") || (typeOfWidget == "filebutton")))
    {
        value.append (strTokens[0].trim().trimCharactersAtEnd ("\"").trimCharactersAtStart ("\""));
    }

    setProperty (widgetData, CabbageIdentifierIds::text, value);
    setProperty (widgetData, CabbageIdentifierIds::comborange, comboRange);
}


void CabbageWidgetData::setColourByNumber (String strTokens, ValueTree widgetData, String identifier)
{
    const String typeOfWidget = getStringProp (widgetData, CabbageIdentifierIds::type);
    
    if (identifier == "colour:0" || identifier == "colours(")
    {
        setProperty (widgetData, CabbageIdentifierIds::colour, getColourFromText (strTokens).toString());
    }

    else if (identifier == "colour:1" || identifier == "colour")
    {
        if (typeOfWidget.contains ("checkbox") || typeOfWidget.contains ("button"))
        {
            setProperty (widgetData, CabbageIdentifierIds::oncolour, getColourFromText (strTokens).toString());
        }
		else
		{
			setProperty(widgetData, CabbageIdentifierIds::colour, getColourFromText (strTokens).toString());
		}
            
    }

    else if (identifier == "fontColour")
    {
        //setProperty (widgetData, CabbageIdentifierIds::onfontcolour, getColourFromText (strTokens.joinIntoString (",")).toString());
        setProperty (widgetData, CabbageIdentifierIds::fontcolour, getColourFromText (strTokens).toString());

    }

    else if (identifier == "fontColour:1")
    {
        setProperty (widgetData, CabbageIdentifierIds::onfontcolour, getColourFromText (strTokens).toString());
    }

    else if (identifier == "fontColour:0")
    {
        setProperty (widgetData, CabbageIdentifierIds::fontcolour, getColourFromText (strTokens).toString());
    }
}


void CabbageWidgetData::setComboItemArrays (StringArray strTokens, ValueTree widgetData, String identifier)
{
    var items = getProperty (widgetData, CabbageIdentifierIds::text );
    bool deleteItems = true;
    for (int i = 0; i < items.size(); i++)
    {
        if(items[i].toString().contains("subM"))
            deleteItems = false;
    }
    
    if(deleteItems)
        items.resize(0);

    items.append("subM:"+strTokens[0].trim());
    int comboRange = 0;
    
    for (int i = 1; i < strTokens.size(); i++)
    {
        items.append (strTokens[i].trim().trimCharactersAtEnd ("\"").trimCharactersAtStart ("\""));
    }
    
    comboRange = items.size();

    setProperty (widgetData, CabbageIdentifierIds::text, items);
    setProperty (widgetData, CabbageIdentifierIds::comborange, comboRange);
    
}

void CabbageWidgetData::setColourArrays (StringArray strTokens, ValueTree widgetData, String identifier, bool isTable)
{
    const int colourIndex = identifier.substring (identifier.indexOf (":") + 1).getIntValue();
    const var colours = getProperty (widgetData, isTable == true ? CabbageIdentifierIds::tablecolour : CabbageIdentifierIds::metercolour );
    var newColours = colours.clone();

    for (int i = newColours.size(); i < colourIndex + 1; i++){
        newColours.append ("0");
    }    

    newColours[colourIndex] = getColourFromText (strTokens.joinIntoString (",")).toString();
    setProperty (widgetData, isTable == true ? CabbageIdentifierIds::tablecolour : CabbageIdentifierIds::metercolour, newColours);

}

void CabbageWidgetData::setFilmStrip(StringArray strTokens, ValueTree widgetData)
{
    const String type = getStringProp(widgetData, CabbageIdentifierIds::type);

    setProperty(widgetData, CabbageIdentifierIds::filmstripimage, strTokens[0].trim());
    setProperty(widgetData, CabbageIdentifierIds::filmstripframes, strTokens[1].trim());
    setProperty(widgetData, CabbageIdentifierIds::filmStripRemoveFrom1, strTokens[2].trim());
    setProperty(widgetData, CabbageIdentifierIds::filmStripRemoveFrom2, strTokens[3].trim());
}

void CabbageWidgetData::setShapes (StringArray strTokens, ValueTree widgetData)
{
    if (strTokens[0].trim() == "sharp")
        setProperty (widgetData, CabbageIdentifierIds::shape, "square");
    else if (strTokens[0].trim() == "rounded")
    {
        setProperty (widgetData, CabbageIdentifierIds::shape, "square");
        setProperty (widgetData, CabbageIdentifierIds::corners, 5);
    }
    else
        setProperty (widgetData, CabbageIdentifierIds::shape, strTokens[0].trim());
}

void CabbageWidgetData::setImageFiles (StringArray strTokens, ValueTree widgetData, String typeOfWidget)
{
    if (typeOfWidget == "groupbox")
        setProperty (widgetData, CabbageIdentifierIds::imggroupbox, strTokens[0].trim());
    else if (typeOfWidget.contains ("slider"))
    {
        if (strTokens[0].trim().containsIgnoreCase ("background"))
            setProperty (widgetData, CabbageIdentifierIds::imgsliderbg, strTokens[1].trim());
        else
            setProperty (widgetData, CabbageIdentifierIds::imgslider, strTokens[1].trim());
    }
    else if (typeOfWidget.contains ("button") || typeOfWidget.contains ("checkbox"))
    {
        if (strTokens[0].trim().containsIgnoreCase ("off"))
            setProperty (widgetData, CabbageIdentifierIds::imgbuttonoff, strTokens[1].trim());
        else if (strTokens[0].trim().containsIgnoreCase ("on"))
            setProperty (widgetData, CabbageIdentifierIds::imgbuttonon, strTokens[1].trim());
        else
            setProperty (widgetData, CabbageIdentifierIds::imgbuttonover, strTokens[1].trim());
    }
}

void CabbageWidgetData::setBounds (ValueTree widgetData, Rectangle<int> rect)
{
    setProperty (widgetData, CabbageIdentifierIds::left, rect.getX());
    setProperty (widgetData, CabbageIdentifierIds::top, rect.getY());
    setProperty (widgetData, CabbageIdentifierIds::width, rect.getWidth());
    setProperty (widgetData, CabbageIdentifierIds::height, rect.getHeight());
}

void CabbageWidgetData::setBounds (StringArray strTokens, ValueTree widgetData)
{
    if (strTokens.size() < 4)
    {
        //CabbageUtilities::debug ("Not enough paramters passed to bounds(): usage pos(top, left width, height\")\n");
    }
    else
    {
        if (strTokens.size() == 3)
        {
            setProperty (widgetData, CabbageIdentifierIds::height, getProperty (widgetData, CabbageIdentifierIds::width));
        }
        else
        {
            setProperty (widgetData, CabbageIdentifierIds::left, strTokens[0].trim().getFloatValue());
            setProperty (widgetData, CabbageIdentifierIds::top, strTokens[1].trim().getFloatValue());
            setProperty (widgetData, CabbageIdentifierIds::width, strTokens[2].trim().getFloatValue());
            setProperty (widgetData, CabbageIdentifierIds::height, strTokens[3].trim().getFloatValue());
        }

    }
}
void CabbageWidgetData::setTableNumberArrays (StringArray strTokens, ValueTree widgetData)
{
    var value;
    var tableConfig;

    for (int i = 0; i < strTokens.size(); i++)
    {
        if (strTokens[i].contains (":") && strTokens.size() > 0)
        {
            StringArray tablesData;
            tablesData.addTokens (strTokens[i], ":", "");
            var tables;

            for (int w = 0; w < tablesData.size(); w++)
            {
                tables.append (tablesData[w]);
                value.append (tablesData[w]);
            }

            tableConfig.append (tables);
        }
        else
        {
            value.append (strTokens[i].trim().getFloatValue());
            tableConfig.append (strTokens[i].trim().getFloatValue());
        }
    }

    setProperty (widgetData, CabbageIdentifierIds::tableconfig, tableConfig);
    setProperty (widgetData, CabbageIdentifierIds::tablenumber, value);

}

void CabbageWidgetData::setFontStyle (StringArray strTokens, ValueTree widgetData)
{
    if (strTokens[0].trim() == "bold")
        setProperty (widgetData, CabbageIdentifierIds::fontstyle, 1);
    else if (strTokens[0].trim() == "italic")
        setProperty (widgetData, CabbageIdentifierIds::fontstyle, 2);
    else if (strTokens[0].trim() == "bold italic")
        setProperty (widgetData, CabbageIdentifierIds::fontstyle, 3);
    else if (strTokens[0].trim() == "underlined")
        setProperty (widgetData, CabbageIdentifierIds::fontstyle, 4);
    else if (strTokens[0].trim() == "bold underlined")
        setProperty (widgetData, CabbageIdentifierIds::fontstyle, 5);
    else if (strTokens[0].trim() == "italic underlined")
        setProperty (widgetData, CabbageIdentifierIds::fontstyle, 6);
    else if (strTokens[0].trim() == "bold italic underlined")
        setProperty (widgetData, CabbageIdentifierIds::fontstyle, 7);
    else
        setProperty (widgetData, CabbageIdentifierIds::fontstyle, 0);
}

void CabbageWidgetData::setAmpRange (StringArray strTokens, ValueTree widgetData)
{
    if (strTokens.size() < 3)
        CabbageUtilities::debug ("Not enough paramters passed to amprange(): usage ampRange(min, max, tablenumber, quantise\")\n");

    var array = getProperty (widgetData, CabbageIdentifierIds::amprange);

    if(array.size()>=3)
    {
        for (int i = 0; i < strTokens.size(); i++)
            array[i] = strTokens[i].trim().getFloatValue();
    }
    else
    {
        for (int i = 0; i < strTokens.size(); i++)
            array.append(strTokens[i].trim().getFloatValue());
    }


    setProperty (widgetData, CabbageIdentifierIds::amprange, array);
    setProperty (widgetData, CabbageIdentifierIds::amprange_min, array[0]);
    setProperty (widgetData, CabbageIdentifierIds::amprange_max, array[1]);
    setProperty (widgetData, CabbageIdentifierIds::amprange_tablenumber, array[2]);
    if(array.size()==4)
        setProperty (widgetData, CabbageIdentifierIds::amprange_quantise, array[3]);
}

void CabbageWidgetData::setKeyboardDisplayNotes(StringArray strTokens, ValueTree widgetData)
{
	var notes;
	notes.append(strTokens[0].trim().getIntValue());


	for (int i = 1; i < strTokens.size(); i++)
	{
		notes.append(strTokens[i].trim().getIntValue());
	}
    
    setProperty(widgetData, CabbageIdentifierIds::keypressed, notes);
}

void CabbageWidgetData::setScrubberPosition (StringArray strTokens, ValueTree widgetData)
{
    var scrubberInfo;
    scrubberInfo.append (strTokens[0].trim().getIntValue());

    for (int i = 1; i < strTokens.size(); i++)
    {
        scrubberInfo.append (strTokens[i].trim().getDoubleValue());
        setProperty (widgetData, CabbageIdentifierIds::scrubberposition, scrubberInfo);
    }

    setProperty (widgetData, CabbageIdentifierIds::scrubberposition, scrubberInfo);


    const String typeOfWidget = getProperty (widgetData, CabbageIdentifierIds::type);

    if (typeOfWidget == CabbageWidgetTypes::gentable)
    {
        setProperty (widgetData, CabbageIdentifierIds::scrubberposition_sample, scrubberInfo[0]);
        setProperty (widgetData, CabbageIdentifierIds::scrubberposition_table, scrubberInfo[1]);
    }
}

void CabbageWidgetData::setPopulateProps (StringArray strTokens, ValueTree widgetData)
{
    var array;

    for ( auto str : strTokens)
        array.append (str.trim());

    setProperty (widgetData, CabbageIdentifierIds::populate, array);

    setProperty (widgetData, CabbageIdentifierIds::filetype, strTokens[0].trim());

    if (strTokens.size() > 1)
        setProperty (widgetData, CabbageIdentifierIds::workingdir, strTokens[1].trim());

    //remove default items for text array if filetype is known ...
    if (CabbageWidgetData::getStringProp (widgetData, CabbageIdentifierIds::type) == CabbageWidgetTypes::combobox
        && strTokens[0].trim().isNotEmpty())
        CabbageWidgetData::setProperty (widgetData, CabbageIdentifierIds::text, "");
    
    //if populate is called, it's a fair assumption that the widget is using string channels..
    //.. however, if it's a preset combo, set the type to number so we can automate it..
//    if(!strTokens[0].contains("snaps"))
//        CabbageWidgetData::setProperty (widgetData, CabbageIdentifierIds::channeltype, "string");

}

void CabbageWidgetData::setRange (StringArray strTokens, ValueTree widgetData, String identifier)
{
    if (identifier == "range")
    {
        if (strTokens.size() < 3)
        {
            CabbageUtilities::debug ("Not enough paramters passed to range(): usage range(minx, max, value, skew, incr\")\n");
        }
        else
        {
            double min = strTokens[0].trim().getDoubleValue();// getFloatValue();
            double max = strTokens[1].trim().getDoubleValue();//.getFloatValue();
            setProperty (widgetData, CabbageIdentifierIds::min, strTokens[0].trim().getDoubleValue());
            setProperty (widgetData, CabbageIdentifierIds::max, strTokens[1].trim().getDoubleValue());

            int decimalPlaces = 2;

            if (strTokens.size() > 2)
            {
                if (strTokens[2].contains (":") && strTokens.size() > 0)
                {
                    //split string into two
                    StringArray values;
                    values.addTokens (strTokens[2], ":", "");

                    if (values.size() == 2)
                    {
                        setProperty (widgetData, CabbageIdentifierIds::minvalue, values[0].trim().getFloatValue());
                        setProperty (widgetData, CabbageIdentifierIds::maxvalue, values[1].trim().getFloatValue());
                    }
                }
                else
                    setProperty (widgetData, CabbageIdentifierIds::value, strTokens[2].trim().getFloatValue());
            }


            if (strTokens.size() > 3)
                setProperty (widgetData, CabbageIdentifierIds::sliderskew, strTokens[3].trim().getFloatValue());


            if (strTokens.size() > 4)
            {
                decimalPlaces = getNumberOfDecimalPlaces (strTokens[4].trim());
                setProperty (widgetData, CabbageIdentifierIds::increment, strTokens[4].trim().getFloatValue());
            }

            double sliderRange = max - min;
            setProperty (widgetData, CabbageIdentifierIds::decimalplaces, decimalPlaces);
            setProperty (widgetData, CabbageIdentifierIds::range, sliderRange);

        }
    }
    else if (identifier == "rangeX")
    {
        if (strTokens.size() < 3)
        {
            CabbageUtilities::debug ("Not enough paramters passed to range(): usage rangex(minx, max, value\")\n");
        }
        else
        {
            float minX = strTokens[0].removeCharacters ("()").trim().getFloatValue();
            float maxX = strTokens[1].removeCharacters ("()").trim().getFloatValue();

            setProperty (widgetData, CabbageIdentifierIds::minx, strTokens[0].trim().getDoubleValue());
            setProperty (widgetData, CabbageIdentifierIds::maxx, strTokens[1].getDoubleValue());
            setProperty (widgetData, CabbageIdentifierIds::valuex, strTokens[2].getDoubleValue());
            setProperty (widgetData, CabbageIdentifierIds::rangex, maxX - minX);

            setProperty (widgetData, CabbageIdentifierIds::decimalplaces, strTokens[3].trim().getDoubleValue());
        }
    }
    else if (identifier == "rangeY")
    {
        if (strTokens.size() < 3)
        {
            CabbageUtilities::debug ("Not enough paramters passed to range(): usage rangey(minx, max, value\")\n");
        }
        else
        {
            float minY = strTokens[0].trim().getFloatValue();
            float maxY = strTokens[1].trim().getFloatValue();

            setProperty (widgetData, CabbageIdentifierIds::miny, strTokens[0].trim().getDoubleValue());
            setProperty (widgetData, CabbageIdentifierIds::maxy, strTokens[1].trim().getDoubleValue());
            setProperty (widgetData, CabbageIdentifierIds::valuey, strTokens[2].trim().getDoubleValue());
            setProperty (widgetData, CabbageIdentifierIds::rangey, maxY - minY);
            setProperty (widgetData, CabbageIdentifierIds::decimalplaces, strTokens[3].trim().getDoubleValue());
        }
    }
}

//=========================================================================
// retrieve/set widget tree data
float CabbageWidgetData::getNumProp (ValueTree widgetData, Identifier prop)
{
    var props = getProperty (widgetData, prop);

    if (props.size() > 0)
        return props[0];
    else
        return props;
}


String CabbageWidgetData::getStringProp (ValueTree widgetData, Identifier prop)
{
    var strings = getProperty (widgetData, prop);

    if (strings.size() > 0)
    {
        return strings[0].toString ();
    }
    else
        return strings.toString();

}

void CabbageWidgetData::setNumProp (ValueTree widgetData, Identifier prop, float val)
{
    setProperty (widgetData, prop, val);
}

void CabbageWidgetData::setStringProp (ValueTree widgetData, Identifier name, const String value)
{
    widgetData.setProperty (name, value, 0);
}

void CabbageWidgetData::setProperty (ValueTree widgetData, Identifier name, const var& value)
{
    Array<var>* array = value.getArray();

    if (array)
    {
        var elements;

        for ( int i = 0 ; i < array->size() ; i++)
            elements.append (array->getReference (i));

        widgetData.setProperty (name, elements, 0);
        return;
    }

    widgetData.setProperty (name, value, 0);
}

var CabbageWidgetData::getProperty (ValueTree widgetData, Identifier name)
{
    return widgetData.getProperty (name);
}

//================================================================================================
ValueTree CabbageWidgetData::getValueTreeForComponent (ValueTree widgetData, String name, bool searchByChannel)
{
    for (int i = 0; i < widgetData.getNumChildren(); i++)
    {

        if (searchByChannel == true)
        {
            //only need first channel from multichannel widgets..see CabbagePluginProcessor::restorePluginState()
            var channels = widgetData.getChild (i).getProperty (CabbageIdentifierIds::channel);
			const String channel = CabbageWidgetData::getStringProp(widgetData.getChild(i), CabbageIdentifierIds::channel);

            if (channels.size() > 0)
                if (name == channels[0].toString())
                {
                    return widgetData.getChild (i);
                }

			if(name == CabbageWidgetData::getStringProp(widgetData.getChild(i), CabbageIdentifierIds::channel))
				return  widgetData.getChild(i).createCopy();

        }
        else
        {
            if(name == "form")
            {
                return widgetData.getChild(0);
            }
            else if (name == widgetData.getChild (i).getProperty (CabbageIdentifierIds::name).toString())
            {
                return widgetData.getChild (i);
            }
        }
    }

    return ValueTree ("empty");
}

//================================================================================================
// these methods are be used to get non-primitive data from text identifiers
Rectangle<int> CabbageWidgetData::getBoundsFromText (String text)
{
    text = " " + text;
    String subString = text.substring (text.indexOfWholeWord ("bounds"));
    subString = subString.substring (7, subString.indexOf (")"));
    StringArray strTokens;
    strTokens.addTokens (subString, ",()", "");
    return Rectangle<int> (strTokens[0].getIntValue(), strTokens[1].getIntValue(), strTokens[2].getIntValue(), strTokens[3].getIntValue());
}

Rectangle<int> CabbageWidgetData::getBounds (ValueTree widgetData)
{
    Rectangle<int> rect (getProperty (widgetData, CabbageIdentifierIds::left),
                         getProperty (widgetData, CabbageIdentifierIds::top),
                         getProperty (widgetData, CabbageIdentifierIds::width),
                         getProperty (widgetData, CabbageIdentifierIds::height));
    return rect;
}

Colour CabbageWidgetData::getColourFromText (String text)
{
    DBG(text);
    StringArray strTokens;
    strTokens.addTokens (text, ",", "");
    Colour colour;

    if (strTokens.size() < 2)
    {
        if (strTokens[0].trim() == "0")
            colour = Colours::white.withAlpha (1.f);
        else
            colour = Colours::findColourForName (strTokens[0].trim(), Colours::white);

    }
    else if (strTokens.size() == 4)
        colour = Colour::fromRGBA (strTokens[0].getIntValue(),
                                   strTokens[1].getIntValue(),
                                   strTokens[2].getIntValue(),
                                   strTokens[3].getIntValue());
    else if (strTokens.size() == 3)
        colour = Colour::fromRGB (strTokens[0].getIntValue(),
                                  strTokens[1].getIntValue(),
                                  strTokens[2].getIntValue());

    return colour;
}
