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
//#include "CabbageWidgetDataInitMethods.cpp"
//===============================================================================
// Main Cabbage abstract GUI class
//===============================================================================


void CabbageWidgetData::setWidgetState(ValueTree widgetData, String lineFromCsd, int ID)
{
    setProperty(widgetData, "scalex", 1);
    setProperty(widgetData, "scaley", 1);
    setProperty(widgetData, "resize", 0);
    setProperty(widgetData, CabbageIdentifierIds::active, 1);
    setProperty(widgetData, CabbageIdentifierIds::parentdir, "");
    setProperty(widgetData, CabbageIdentifierIds::imgdebug, 0);
    setProperty(widgetData, CabbageIdentifierIds::allowboundsupdate, 0);
    setProperty(widgetData, CabbageIdentifierIds::identchannelmessage, "");
    setProperty(widgetData, CabbageIdentifierIds::popuptext, "");
    setProperty(widgetData, CabbageIdentifierIds::alpha, 1);
    setProperty(widgetData, CabbageIdentifierIds::visible, 1);
    setProperty(widgetData, CabbageIdentifierIds::rotate, 0.f);
    setProperty(widgetData, CabbageIdentifierIds::pivotx, 0.f);
    setProperty(widgetData, CabbageIdentifierIds::pivoty, 0.f);
    setProperty(widgetData, CabbageIdentifierIds::linenumber, ID);
    setProperty(widgetData, CabbageIdentifierIds::decimalplaces, 0);
    setProperty(widgetData, CabbageIdentifierIds::update, 0);

    int top, left, width, height;
    StringArray strTokens;
    strTokens.addTokens(lineFromCsd, " ", "\"");
    setProperty(widgetData, CabbageIdentifierIds::channelarray, "");

    if(strTokens[0].trim() == "hslider")
        setHSliderProperties(widgetData, ID);

    else if(strTokens[0].trim() == "vslider")
        setVSliderProperties(widgetData, ID);

    else if(strTokens[0].trim() == "rslider")
        setRSliderProperties(widgetData, ID);

    else if(strTokens[0].trim() == "groupbox")
        setGroupBoxProperties(widgetData, ID);

    else if(strTokens[0].trim() == "csoundoutput")
        setCsoundOutputProperties(widgetData, ID);

    else if(strTokens[0].trim() == "keyboard")
        setKeyboardProperties(widgetData, ID);

    else if(strTokens[0].trim() == "form")
        setFormProperties(widgetData, ID);

    else if(strTokens[0].trim() == "textbox")
        setTextBoxProperties(widgetData, ID);

    else if(strTokens[0].trim() == "checkbox")
        setCheckBoxProperties(widgetData, ID);

    else if(strTokens[0].trim() == "numberbox")
        setNumberBoxProperties(widgetData, ID);

    else if(strTokens[0].trim() == "combobox")
        setComboBoxProperties(widgetData, ID);

    else if(strTokens[0].trim() == "label")
        setLabelProperties(widgetData, ID);

    else if(strTokens[0].trim() == "texteditor")
        setTextEditorProperties(widgetData, ID);

    else if(strTokens[0].trim() == "image")
        setImageProperties(widgetData, ID);

    else if(strTokens[0].trim() == "encoder")
        setEncoderProperties(widgetData, ID);

    else if(strTokens[0].trim() == "button")
        setButtonProperties(widgetData, ID);

    else if(strTokens[0].trim() == "soundfiler")
        setSoundfilerProperties(widgetData, ID);

    else if(strTokens[0].trim() == "filebutton")
        setFileButtonProperties(widgetData, ID);

    else if(strTokens[0].trim() == "infobutton")
        setInfoButtonProperties(widgetData, ID);

    else if((strTokens[0].trim() == "sourcebutton")||(strTokens[0].trim() == "loadbutton"))
        setLoadButtonProperties(widgetData, ID);

    else if(strTokens[0].trim() == "signaldisplay" || strTokens[0].trim() == "fftdisplay")
        setSignalDisplayProperties(widgetData, ID);

    else if(strTokens[0].trim() == "xypad")
        setXYPadProperties(widgetData, ID);

    else if(strTokens[0].trim() == "gentable")
        setGenTableProperties(widgetData, ID);

    else if(strTokens[0].trim() == "hrange")
        setHRangeSliderProperties(widgetData, ID);

    else if(strTokens[0].trim() == "vrange")
        setVRangeSliderProperties(widgetData, ID);

    //===============table==================//
    else if(strTokens[0].trim() == "table")
    {
        setProperty(widgetData, "basetype", "layout");
        top = 10;
        left = 10;
        width = 400;
        height = 200;

        var tableColours;
        tableColours.append("white");
        tableColours.append("cornflowerblue");
        tableColours.append("yellow");
        tableColours.append("lime");
        tableColours.append("green");
        tableColours.append("pink");

        setProperty(widgetData, CabbageIdentifierIds::top, 10);
        setProperty(widgetData, CabbageIdentifierIds::left, 10);
        setProperty(widgetData, CabbageIdentifierIds::width, 400);
        setProperty(widgetData, CabbageIdentifierIds::height, 200);
        setProperty(widgetData, CabbageIdentifierIds::tablenumber, 1);
        setProperty(widgetData, CabbageIdentifierIds::drawmode, "");
        setProperty(widgetData, CabbageIdentifierIds::resizemode, 0);
        setProperty(widgetData, CabbageIdentifierIds::readonly, 0);
        setProperty(widgetData, CabbageIdentifierIds::tablecolour, tableColours);
        setProperty(widgetData, CabbageIdentifierIds::amprange, 0);
        setProperty(widgetData, CabbageIdentifierIds::type, "table");
        setProperty(widgetData, CabbageIdentifierIds::stack, 0);
        setProperty(widgetData, CabbageIdentifierIds::name, getProperty(widgetData, "name").toString()+String(ID));
        setProperty(widgetData, CabbageIdentifierIds::identchannel, "");
        setProperty(widgetData, CabbageIdentifierIds::visible, 1);
    }

    //===============stepper example==================//
    else if(strTokens[0].trim() == "stepper")
    {
        setProperty(widgetData, "basetype", "layout");
        top = 10;
        left = 10;
        width = 180;
        height = 2;

        setProperty(widgetData, CabbageIdentifierIds::top, 10);
        setProperty(widgetData, CabbageIdentifierIds::left, 10);
        setProperty(widgetData, CabbageIdentifierIds::width, 160);
        setProperty(widgetData, CabbageIdentifierIds::height, 2);
        setProperty(widgetData, CabbageIdentifierIds::colour, Colours::white.toString());
        setProperty(widgetData, CabbageIdentifierIds::type, "stepper");
        setProperty(widgetData, CabbageIdentifierIds::name, "stepper");
        setProperty(widgetData, CabbageIdentifierIds::name, getProperty(widgetData, "name").toString()+String(ID));
        setProperty(widgetData, CabbageIdentifierIds::identchannel, "");
        setProperty(widgetData, CabbageIdentifierIds::visible, 1);
    }

    //===============non-GUI host widgets==================//
    else if(strTokens[0].trim() == "hostbpm")
    {
        setProperty(widgetData, CabbageIdentifierIds::basetype, "layout");
        setProperty(widgetData, CabbageIdentifierIds::name, "hostbpm");
        setProperty(widgetData, CabbageIdentifierIds::type, "hostbpm");
    }

    else if(strTokens[0].trim() == "hostppqpos")
    {
        setProperty(widgetData, CabbageIdentifierIds::basetype, "layout");
        setProperty(widgetData, CabbageIdentifierIds::name, "hostppqpos");
        setProperty(widgetData, CabbageIdentifierIds::type, "hostppqpos");
    }

    else if(strTokens[0].trim() == "hostplaying")
    {
        setProperty(widgetData, CabbageIdentifierIds::basetype, "layout");
        setProperty(widgetData, CabbageIdentifierIds::name, "hostplaying");
        setProperty(widgetData, CabbageIdentifierIds::type, "hostplaying");
    }

    else if(strTokens[0].trim() == "hostrecording")
    {
        setProperty(widgetData, CabbageIdentifierIds::basetype, "layout");
        setProperty(widgetData, CabbageIdentifierIds::name, "hostrecording");
        setProperty(widgetData, CabbageIdentifierIds::type, "hostrecording");
    }

    else if(strTokens[0].trim() == "hosttime")
    {
        setProperty(widgetData, CabbageIdentifierIds::basetype, "layout");
        setProperty(widgetData, CabbageIdentifierIds::name, "hosttime");
        setProperty(widgetData, CabbageIdentifierIds::type, "hosttime");
    }

    //parse the text now that all default values ahve been assigned
    setCustomWidgetState(widgetData, lineFromCsd, "");
}

//===========================================================================================
// this method parsing the Cabbage text and set each of the Cabbage indentifers
//===========================================================================================
void CabbageWidgetData::setCustomWidgetState(ValueTree widgetData, String inStr, String identifier)
{
    //remove any text after a semicolon and take out tabs..
    String str = inStr.replace("\t", " ");
    if(str.indexOf(0, ";")!=-1)
        str = str.substring(0, str.indexOf(0, ";"));

    StringArray identArray;

    String parameters;
    //if identifier string is not empty, use that, otherwise
    //search through identArray list of known identifiers
    if(identifier.isNotEmpty())
    {
        while(identifier.indexOf("(")>0)
        {
            parameters = identifier.substring(identifier.indexOf("("), identifier.indexOf(")")+1);
            identifier = identifier.replace(parameters, "").replace(",", " ");
        }
        identArray.addTokens(identifier, " ");
    }
    else
    {
        CabbageIdentifierStrings idententifiers;
        identArray = idententifiers;
    }

    String warningMessages;

    String typeOfWidget="";
    StringArray strTokens, tempTokens;
    tempTokens.addTokens(str, ", \t", "\"");
    typeOfWidget = tempTokens[0];

    for(int indx=0; indx<identArray.size(); indx++)
    {

        int identPos = str.toLowerCase().indexOf(" "+identArray[indx]);

        // int identPos = str.toLowerCase().indexOf(" "+identArray[indx]+"(");
        // if(identPos<0)
        //     identPos = str.toLowerCase().indexOf(","+identArray[indx]+"(");


        if(identPos>-1)
        {
            String newString = str.substring(identPos+identArray[indx].length());
            String tstr = newString.substring(newString.indexOf("(")+1, newString.indexOf(0, ")"));

            if(tstr.length()==0)
                return;

            //find current identifier and remove it from main string so that we can
            //check for multiple instances of an identifier such as amprange
            String deleteStr = str.substring(identPos+1);
            deleteStr = deleteStr.substring(0, deleteStr.indexOf(")")+1);
            str = str.replace(deleteStr, "");

            strTokens.addTokens(tstr.removeCharacters("\t)\""), ",", "\"");


            //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
            //string paramters
            //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

            if(identArray[indx].equalsIgnoreCase("name"))
            {
                setProperty(widgetData, CabbageIdentifierIds::name, strTokens[0].trim());
            }
            else if(identArray[indx].equalsIgnoreCase("plant"))
            {
                setProperty(widgetData, CabbageIdentifierIds::plant, strTokens[0].trim());
            }
            else if(identArray[indx].equalsIgnoreCase("caption"))
            {
                setProperty(widgetData, CabbageIdentifierIds::caption, strTokens[0].trim());
            }

            else if(identArray[indx].equalsIgnoreCase("signalvariable"))
            {
                var array;
                array.append(strTokens[0].trim());
                for(int i=1; i<strTokens.size(); i++)
                {
                    array.append(strTokens[i].trim());
                }
                setProperty(widgetData, CabbageIdentifierIds::signalvariable, array);
            }


            else if(identArray[indx].equalsIgnoreCase("channel")||
                    identArray[indx].equalsIgnoreCase("channels")||
                    identArray[indx].equalsIgnoreCase("channelarray")||
                    identArray[indx].equalsIgnoreCase("widgetarray"))
            {
                var array;
                array.append(strTokens[0].trim());
                //if(str.containsIgnoreCase("xypad"))
                // {
                //    setProperty(widgetData, CabbageIdentifierIds::xchannel, strTokens[0].trim());
                //    setProperty(widgetData, CabbageIdentifierIds::ychannel, strTokens[1].trim());
                //}

                for(int i=1; i<strTokens.size(); i++)
                {
                    array.append(strTokens[i].trim());
                }

                if(identArray[indx].equalsIgnoreCase("channelarray"))
                {
                    //if there are already channels declared we want to keep them
                    array.resize(0);

                    const Array<var>* valueArray = widgetData.getProperty(CabbageIdentifierIds::channel).getArray();

                    if(valueArray)
                    {
                        for(int i=0; i<valueArray->size(); i++)
                        {
                            array.append(valueArray[i]);
                        }
                    }
                    var channelArray, identChannelArray;
                    int size = strTokens[1].getIntValue();
                    for(int i=0; i<size; i++)
                    {
                        array.append(strTokens[0]+String(i+1));
                    }
                    setProperty(widgetData, CabbageIdentifierIds::channel, array);
                    //identifiers that appear more than once need to use indx-- so we can check for another instance
                    indx--;
                }

                setProperty(widgetData, CabbageIdentifierIds::channel, array);

                if(identArray[indx].equalsIgnoreCase("widgetarray"))
                {
                    var channelArray, identChannelArray;
                    int size = strTokens[1].getIntValue();
                    for(int i=0; i<size; i++)
                    {
                        channelArray.append(String(strTokens[0])+String(i+1));
                        identChannelArray.append(String(strTokens[0])+String("_ident")+String(i+1));

                    }
                    setProperty(widgetData, CabbageIdentifierIds::channelarray, channelArray);
                    setProperty(widgetData, CabbageIdentifierIds::identchannelarray, identChannelArray);
                }
            }

            else if(identArray[indx].equalsIgnoreCase("colour:0")||
                    identArray[indx].equalsIgnoreCase("colour"))
            {
                setProperty(widgetData, CabbageIdentifierIds::colour, getColourFromText(strTokens.joinIntoString(",")).toString());
            }

            else if(identArray[indx].equalsIgnoreCase("colour:1") ||
                    identArray[indx].equalsIgnoreCase("colour"))
            {
                if(typeOfWidget.contains("checkbox") || typeOfWidget.contains("button"))
                {
                    setProperty(widgetData, CabbageIdentifierIds::oncolour, getColourFromText(strTokens.joinIntoString(",")).toString());
                }
                else
                    setProperty(widgetData, CabbageIdentifierIds::colour, getColourFromText(strTokens.joinIntoString(",")).toString());
            }

            else if(identArray[indx].equalsIgnoreCase("fontcolour")||
                    identArray[indx].equalsIgnoreCase("fontcolour:1"))
            {
                if(typeOfWidget.contains("button"))
                    setProperty(widgetData, CabbageIdentifierIds::onfontcolour, getColourFromText(strTokens.joinIntoString(",")).toString());
                else
                    setProperty(widgetData, CabbageIdentifierIds::fontcolour, getColourFromText(strTokens.joinIntoString(",")).toString());

            }
            else if(identArray[indx].equalsIgnoreCase("fontcolour:0"))
            {
                setProperty(widgetData, CabbageIdentifierIds::fontcolour, getColourFromText(strTokens.joinIntoString(",")).toString());
            }

            else if(identArray[indx].equalsIgnoreCase("menucolour"))
            {
                setProperty(widgetData, CabbageIdentifierIds::menucolour, getColourFromText(strTokens.joinIntoString(",")).toString());
            }

            else if(identArray[indx].equalsIgnoreCase("tablebackgroundcolour"))
            {
                setProperty(widgetData, CabbageIdentifierIds::tablebackgroundcolour, getColourFromText(strTokens.joinIntoString(",")).toString());
            }

            else if(identArray[indx].equalsIgnoreCase("backgroundcolour"))
            {
                setProperty(widgetData, CabbageIdentifierIds::backgroundcolour, getColourFromText(strTokens.joinIntoString(",")).toString());
            }

            else if(identArray[indx].equalsIgnoreCase("ballcolour"))
            {
                setProperty(widgetData, CabbageIdentifierIds::ballcolour, getColourFromText(strTokens.joinIntoString(",")).toString());
            }

            else if(identArray[indx].equalsIgnoreCase("tablegridcolour"))
            {
                setProperty(widgetData, CabbageIdentifierIds::tablegridcolour, getColourFromText(strTokens.joinIntoString(",")).toString());
            }

            else if(identArray[indx].equalsIgnoreCase("tablecolour") ||
                    identArray[indx].equalsIgnoreCase("tablecolours"))
            {
                var colours;
                for(int i=0; i<strTokens.size(); i++)
                {
                    colours.append(strTokens[i].trim());
                    //Logger::writeToLog(strTokens[i].trim());
                }
                setProperty(widgetData, CabbageIdentifierIds::tablecolour, colours);
            }

            else if(identArray[indx].equalsIgnoreCase("trackercolour"))
            {
                setProperty(widgetData, CabbageIdentifierIds::trackercolour, getColourFromText(strTokens.joinIntoString(",")).toString());
            }

            else if(identArray[indx].equalsIgnoreCase("highlightcolour"))
            {
                setProperty(widgetData, CabbageIdentifierIds::highlightcolour, getColourFromText(strTokens.joinIntoString(",")).toString());
            }

            else if(identArray[indx].equalsIgnoreCase("titlebarcolour"))
            {
                setProperty(widgetData, CabbageIdentifierIds::titlebarcolour, getColourFromText(strTokens.joinIntoString(",")).toString());
            }

            else if(identArray[indx].equalsIgnoreCase("kind"))
            {
                setProperty(widgetData, CabbageIdentifierIds::kind, strTokens[0].trim());
            }

            else if(identArray[indx].equalsIgnoreCase("file"))
            {
                setProperty(widgetData, CabbageIdentifierIds::file, strTokens[0].trim());
            }

            else if(identArray[indx].equalsIgnoreCase("imgpath"))
            {
                setProperty(widgetData, CabbageIdentifierIds::imgpath, strTokens[0].trim());
            }

            else if(identArray[indx].equalsIgnoreCase("imgfile"))
            {
                if(typeOfWidget=="groupbox")
                    setProperty(widgetData, CabbageIdentifierIds::imggroupbox, strTokens[0].trim());
                else if(typeOfWidget.contains("slider"))
                {
                    if(strTokens[0].trim().containsIgnoreCase("background"))
                        setProperty(widgetData, CabbageIdentifierIds::imgsliderbg, strTokens[1].trim());
                    else
                        setProperty(widgetData, CabbageIdentifierIds::imgslider, strTokens[1].trim());
                }
                else if(typeOfWidget.contains("button") || typeOfWidget.contains("checkbox"))
                {
                    if(strTokens[0].trim().containsIgnoreCase("off"))
                        setProperty(widgetData, CabbageIdentifierIds::imgbuttonoff, strTokens[1].trim());
                    else
                        setProperty(widgetData, CabbageIdentifierIds::imgbuttonon, strTokens[1].trim());
                }
                //identifiers that appear more than once need to use indx-- so we can check for another instance
                indx--;
            }

            else if(identArray[indx].equalsIgnoreCase("fillcolour"))
            {
                setProperty(widgetData, CabbageIdentifierIds::fillcolour, getColourFromText(strTokens.joinIntoString(",")).toString());
            }

            else if(identArray[indx].equalsIgnoreCase("shape"))
            {
                if(strTokens[0].trim()=="sharp")
                    setProperty(widgetData, CabbageIdentifierIds::shape, "square");
                else if(strTokens[0].trim()=="rounded")
				{
					setProperty(widgetData, CabbageIdentifierIds::shape, "square");
					setProperty(widgetData, CabbageIdentifierIds::corners, 5);
				}
				else
                    setProperty(widgetData, CabbageIdentifierIds::shape, strTokens[0].trim());
            }
            else if(identArray[indx].equalsIgnoreCase("outlinecolour"))
            {
                setProperty(widgetData, CabbageIdentifierIds::outlinecolour, getColourFromText(strTokens.joinIntoString(",")).toString());
            }
            else if(identArray[indx].equalsIgnoreCase("textcolour"))
            {
                setProperty(widgetData, CabbageIdentifierIds::textcolour, getColourFromText(strTokens.joinIntoString(",")).toString());
            }

            else if(identArray[indx].equalsIgnoreCase("pluginid"))
            {
                setProperty(widgetData, CabbageIdentifierIds::pluginid, strTokens[0].trim());
            }

            else if(identArray[indx].equalsIgnoreCase("popuptext"))
            {
                setProperty(widgetData, CabbageIdentifierIds::popuptext, strTokens[0]);
            }

            else if(identArray[indx].equalsIgnoreCase("items")||
                    identArray[indx].equalsIgnoreCase("text"))
            {
                //CabbageUtilities::showMessage(typeOfWidget+":"+text);
                var value;
                int comboRange = 0;
                for(int i= 0; i<(int)strTokens.size(); i++)
                {
                    //Logger::writeToLog(strTokens[i]);
                    String test = strTokens[i];
                    value.append(strTokens[i].trim());
                    //if((int)strTokens.size()<2) {
                    //	items.add(strTokens[i]);
                    //	value.append(strTokens[i].trim());
                    //	i++;
                    //	}
                    comboRange = i+1;
                }
                //Logger::writeToLog(String(value.size())+typeOfWidget);
                if((value.size()==1) && ((typeOfWidget=="button") || (typeOfWidget=="filebutton")))
                {
                    value.append(strTokens[0]);
                    //Logger::writeToLog(strTokens[0].trim());
                }

                setProperty(widgetData, CabbageIdentifierIds::text, value);
                setProperty(widgetData, CabbageIdentifierIds::comborange, comboRange);
            }


            else if(identArray[indx].equalsIgnoreCase("preset"))
            {
                //CabbageUtilities::showMessage("preset is no longer supported, see docs");
            }

            else if(identArray[indx].equalsIgnoreCase("mode") ||
                    identArray[indx].equalsIgnoreCase(",mode"))
            {
                setProperty(widgetData, CabbageIdentifierIds::mode, strTokens[0].trim());
            }

            else if(identArray[indx].equalsIgnoreCase("native"))
            {
                setProperty(widgetData, "native", strTokens[0].trim());
            }


            else if(identArray[indx].equalsIgnoreCase("channeltype"))
            {
                setProperty(widgetData, CabbageIdentifierIds::channeltype, strTokens[0].trim());
            }

            else if(identArray[indx].equalsIgnoreCase("identchannel"))
            {
                setProperty(widgetData, CabbageIdentifierIds::identchannel, strTokens[0].trim());
            }

            else if(identArray[indx].equalsIgnoreCase("populate"))
            {
                setProperty(widgetData, CabbageIdentifierIds::filetype, strTokens[0].trim());

                if(strTokens.size()>1)
                {
                    setProperty(widgetData, CabbageIdentifierIds::workingdir, strTokens[1].trim());
                }
            }

            else if(identArray[indx].equalsIgnoreCase("author"))
            {
                setProperty(widgetData, CabbageIdentifierIds::author, strTokens[0].trim());
            }

            else if(identArray[indx].equalsIgnoreCase("align"))
            {
                setProperty(widgetData, CabbageIdentifierIds::align, strTokens[0].trim());
            }


            else if(identArray[indx].equalsIgnoreCase("tabs"))
            {
                var value;
                for(int i= 0; i<(int)strTokens.size(); i++)
                {
                    value.append(strTokens[i].trim());
                }
                setProperty(widgetData, CabbageIdentifierIds::tabs, strTokens[0].trim());
            }

            else if(identArray[indx].equalsIgnoreCase("tabpage"))
            {
                setProperty(widgetData, CabbageIdentifierIds::tabpage, strTokens[0].trim());
            }

            else if(identArray[indx].equalsIgnoreCase("displaytype"))
            {
                setProperty(widgetData, CabbageIdentifierIds::displaytype, strTokens[0].trim());
            }

            else if(identArray[indx].equalsIgnoreCase("include"))
            {
                var array;
                for(int i=0; i<strTokens.size(); i++)
                {
                    array.append(strTokens[i].trim());
                }

                setProperty(widgetData, CabbageIdentifierIds::include, array);
            }

            else if(identArray[indx].equalsIgnoreCase("drawmode"))
            {
                var value;
                if(strTokens.size()>0)
                    for(int i=0; i<strTokens.size(); i++)
                    {
                        value.append(strTokens[i].trim());
                    }
                setProperty(widgetData, CabbageIdentifierIds::drawmode, value);
            }
            //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
            //numeric paramters
            //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
            else if(identArray[indx].equalsIgnoreCase("address"))
            {
                if(strTokens.size()>=2)
                {
                    //sizeText = identArray.getReference(indx)+tstr+")";
                    String address = strTokens[0].trim();
                    int port = strTokens[1].trim().getIntValue();
                    setProperty(widgetData, CabbageIdentifierIds::socketaddress, address);
                    setProperty(widgetData, CabbageIdentifierIds::socketport, port);
                }
            }

            else if(identArray[indx].equalsIgnoreCase("size"))
            {
                if(strTokens.size()>=2)
                {
                    //sizeText = identArray.getReference(indx)+tstr+")";
                    setProperty(widgetData, CabbageIdentifierIds::width, strTokens[0].trim().getFloatValue());
                    setProperty(widgetData, CabbageIdentifierIds::height, strTokens[1].trim().getFloatValue());
                }
                else
                    warningMessages+="size() does not have enough parameters\n";
            }

            else if(identArray[indx].equalsIgnoreCase("bounds"))
            {
                //CabbageUtilities::debug(strTokens.size());
                if(strTokens.size()<4)
                {
                    warningMessages+="Not enough paramters passed to bounds(): usage pos(top, left width, height\")\n";
                }
                else
                {
                    if(strTokens.size()==3)
                    {
                        setProperty(widgetData, CabbageIdentifierIds::height, getProperty(widgetData, CabbageIdentifierIds::width));
                    }
                    else
                    {
                        setProperty(widgetData, CabbageIdentifierIds::left, strTokens[0].trim().getFloatValue());
                        setProperty(widgetData, CabbageIdentifierIds::top, strTokens[1].trim().getFloatValue());
                        setProperty(widgetData, CabbageIdentifierIds::width, strTokens[2].trim().getFloatValue());
                        setProperty(widgetData, CabbageIdentifierIds::height, strTokens[3].trim().getFloatValue());
                    }

                }
            }

            else if(identArray[indx].equalsIgnoreCase("pos"))
            {
                if(strTokens.size()<2)
                {
                    warningMessages+="Not enough paramters passed to pos(): usage pos(top, left\")\n";
                }
                else
                {
                    setProperty(widgetData, CabbageIdentifierIds::left, strTokens[0].trim().getFloatValue());
                    setProperty(widgetData, CabbageIdentifierIds::top, strTokens[1].trim().getFloatValue());
                }
            }

            else if(identArray[indx].equalsIgnoreCase("range"))
            {
                if(strTokens.size()<3)
                {
                    warningMessages+="Not enough paramters passed to range(): usage range(minx, max, value, skew, incr\")\n";
                }
                else
                {
                    StringArray tempArray;
                    double min = strTokens[0].trim().getDoubleValue();// getFloatValue();
                    double max = strTokens[1].trim().getDoubleValue();//.getFloatValue();
                    setProperty(widgetData, CabbageIdentifierIds::min, strTokens[0].trim().getDoubleValue());
                    setProperty(widgetData, CabbageIdentifierIds::max, strTokens[1].trim().getDoubleValue());
                    tempArray.add(strTokens[0].trim());
                    tempArray.add(strTokens[1].trim());
                    if(strTokens.size()>2)
                    {
                        if(strTokens[2].contains(":") && strTokens.size()>0)
                        {
                            //split string into two
                            StringArray values;
                            values.addTokens(strTokens[2], ":", "");
                            if(values.size()==2)
                            {
                                setProperty(widgetData, CabbageIdentifierIds::minvalue, values[0].trim().getFloatValue());
                                setProperty(widgetData, CabbageIdentifierIds::maxvalue, values[1].trim().getFloatValue());
                            }
                        }
                        else
                        {
                            CabbageUtilities::debug(strTokens[2].trim());
                            setProperty(widgetData, CabbageIdentifierIds::value, strTokens[2].trim().getFloatValue());
                            tempArray.add(strTokens[2].trim());
                        }
                    }


                    if(strTokens.size()>3)
                    {
                        setProperty(widgetData, CabbageIdentifierIds::sliderskew, strTokens[3].trim().getFloatValue());
                        tempArray.add(strTokens[3].trim());
                    }

                    if(strTokens.size()>4)
                    {
                        tempArray.add(strTokens[4].trim());
                        setProperty(widgetData, CabbageIdentifierIds::sliderincr, strTokens[4].trim().getFloatValue());
                    }

                    double sliderRange = max-min;
                    int decimalPlaces = getNumberOfDecimalPlaces(tempArray);
                    if(decimalPlaces<1 && max<=1)
                        decimalPlaces = 2;
                    else
                        decimalPlaces = getNumberOfDecimalPlaces(tempArray);
                    setProperty(widgetData, CabbageIdentifierIds::decimalplaces, decimalPlaces);
                    setProperty(widgetData, CabbageIdentifierIds::range, sliderRange);

                }
            }
            else if(identArray[indx].equalsIgnoreCase("rangex"))
            {
                if(strTokens.size()<3)
                {
                    warningMessages+="Not enough paramters passed to range(): usage rangex(minx, max, value\")\n";
                }
                else
                {
                    float minX = strTokens[0].removeCharacters("()").trim().getFloatValue();
                    float maxX = strTokens[1].removeCharacters("()").trim().getFloatValue();

                    setProperty(widgetData, CabbageIdentifierIds::minx, strTokens[0].trim().getDoubleValue());
                    setProperty(widgetData, CabbageIdentifierIds::maxx, strTokens[1].getDoubleValue());
                    setProperty(widgetData, CabbageIdentifierIds::valuex, strTokens[2].getDoubleValue());
                    setProperty(widgetData, CabbageIdentifierIds::rangex, maxX-minX);

                    setProperty(widgetData, CabbageIdentifierIds::decimalplaces, strTokens[3].trim().getDoubleValue());
                }
            }
            else if(identArray[indx].equalsIgnoreCase("rangey"))
            {
                if(strTokens.size()<3)
                {
                    warningMessages+="Not enough paramters passed to range(): usage rangey(minx, max, value\")\n";
                }
                else
                {
                    float minY = strTokens[0].trim().getFloatValue();
                    float maxY = strTokens[1].trim().getFloatValue();

                    setProperty(widgetData, CabbageIdentifierIds::miny, strTokens[0].trim().getDoubleValue());
                    setProperty(widgetData, CabbageIdentifierIds::maxy, strTokens[1].trim().getDoubleValue());
                    setProperty(widgetData, CabbageIdentifierIds::valuey, strTokens[2].trim().getDoubleValue());
                    setProperty(widgetData, CabbageIdentifierIds::rangey, maxY-minY);
                    setProperty(widgetData, CabbageIdentifierIds::decimalplaces, strTokens[3].trim().getDoubleValue());
                }
            }
            else if(identArray[indx].equalsIgnoreCase("min"))
            {
                setProperty(widgetData, CabbageIdentifierIds::minenabled, 1);
                setProperty(widgetData, CabbageIdentifierIds::minvalue, strTokens[0].trim().getFloatValue());
            }

            else if(identArray[indx].equalsIgnoreCase("imgdebug"))
            {
                setProperty(widgetData, CabbageIdentifierIds::imgdebug, strTokens[0].trim().getFloatValue());
            }

            else if(identArray[indx].equalsIgnoreCase("middlec"))
            {
                setProperty(widgetData, CabbageIdentifierIds::middlec, strTokens[0].trim().getFloatValue());
            }

            else if(identArray[indx].equalsIgnoreCase("fill"))
            {
                setProperty(widgetData, CabbageIdentifierIds::fill, strTokens[0].trim().getIntValue());
            }

            else if(identArray[indx].equalsIgnoreCase("updaterate"))
            {
                setProperty(widgetData, CabbageIdentifierIds::updaterate, strTokens[0].trim().getIntValue());
            }

            else if(identArray[indx].equalsIgnoreCase("keywidth"))
            {
                setProperty(widgetData, CabbageIdentifierIds::keywidth, strTokens[0].trim().getFloatValue());
            }

            else if(identArray[indx].equalsIgnoreCase("rotate"))
            {
                setProperty(widgetData, CabbageIdentifierIds::rotate, strTokens[0].trim().getFloatValue());
                if(strTokens.size()==3)
                {
                    setProperty(widgetData, CabbageIdentifierIds::pivotx, strTokens[1].trim().getFloatValue());
                    setProperty(widgetData, CabbageIdentifierIds::pivoty, strTokens[2].trim().getFloatValue());
                }
            }

            else if(identArray[indx].equalsIgnoreCase("pivoty"))
            {
                setProperty(widgetData, CabbageIdentifierIds::pivoty, strTokens[0].trim().getFloatValue());
            }

            else if(identArray[indx].equalsIgnoreCase("ffttablenumber"))
            {
                setProperty(widgetData, CabbageIdentifierIds::ffttablenumber, strTokens[0].trim().getFloatValue()-1);
            }

            else if(identArray[indx].equalsIgnoreCase("pivotx"))
            {
                setProperty(widgetData, CabbageIdentifierIds::pivotx, strTokens[0].trim().getFloatValue());
            }

            else if(identArray[indx].equalsIgnoreCase("midictrl"))
            {
                if(strTokens.size()<2)
                {
                    warningMessages+="Not enough paramters passed to midiCtrl(): usage midiCtrl(midiChan, midiCtrl\")\n";
                }
                else
                {
                    setProperty(widgetData, CabbageIdentifierIds::midichan, strTokens[0].trim().getFloatValue());
                    setProperty(widgetData, CabbageIdentifierIds::midictrl, strTokens[0].trim().getFloatValue());
                }
            }
            else if(identArray[indx].equalsIgnoreCase("max"))
            {
                setProperty(widgetData, CabbageIdentifierIds::maxenabled, 1);
                setProperty(widgetData, CabbageIdentifierIds::maxvalue, strTokens[0].trim().getFloatValue());
            }

            else if(identArray[indx].equalsIgnoreCase("sliderincr"))
            {
                setProperty(widgetData, CabbageIdentifierIds::sliderincr, strTokens[0].trim().getFloatValue());
            }

            else if(identArray[indx].equalsIgnoreCase("sliderskew"))
            {
                setProperty(widgetData, CabbageIdentifierIds::sliderskew, strTokens[0].trim().getFloatValue());
            }

            else if(identArray[indx].equalsIgnoreCase("visible"))
            {
                setProperty(widgetData, CabbageIdentifierIds::visible, strTokens[0].trim().getFloatValue());
            }

            else if(identArray[indx].equalsIgnoreCase("active"))
            {
                setProperty(widgetData, CabbageIdentifierIds::active, strTokens[0].trim().getFloatValue());
            }

            else if(identArray[indx].equalsIgnoreCase("stack"))
            {
                setProperty(widgetData, CabbageIdentifierIds::stack, strTokens[0].trim().getFloatValue());
            }

            else if(identArray[indx].equalsIgnoreCase("tab"))
            {
                setProperty(widgetData, CabbageIdentifierIds::tabbed, strTokens[0].trim().getFloatValue());
            }

            else if(identArray[indx].equalsIgnoreCase("latched"))
            {
                setProperty(widgetData, CabbageIdentifierIds::latched, strTokens[0].trim().getFloatValue());
            }

            else if(identArray[indx].equalsIgnoreCase("alpha"))
            {
                setProperty(widgetData, CabbageIdentifierIds::alpha, strTokens[0].trim().getFloatValue());
            }

            else if(identArray[indx].equalsIgnoreCase("corners"))
            {
                setProperty(widgetData, CabbageIdentifierIds::corners, strTokens[0].trim().getFloatValue());
            }

            else if(identArray[indx].equalsIgnoreCase("radiogroup"))
            {
                setProperty(widgetData, CabbageIdentifierIds::radiogroup, strTokens[0].trim().getFloatValue());
            }

            else if(identArray[indx].equalsIgnoreCase("guirefresh"))
            {
                setProperty(widgetData, CabbageIdentifierIds::guirefresh, strTokens[0].trim().getFloatValue());
            }

            else if(identArray[indx].equalsIgnoreCase("fontstyle"))
            {
                if(strTokens[0].trim()=="bold")
                    setProperty(widgetData, CabbageIdentifierIds::fontstyle, 1);
                else if(strTokens[0].trim()=="italic")
                    setProperty(widgetData, CabbageIdentifierIds::fontstyle, 2);
                else if(strTokens[0].trim()=="bold italic")
                    setProperty(widgetData, CabbageIdentifierIds::fontstyle, 3);
                else if(strTokens[0].trim()=="underlined")
                    setProperty(widgetData, CabbageIdentifierIds::fontstyle, 4);
                else if(strTokens[0].trim()=="bold underlined")
                    setProperty(widgetData, CabbageIdentifierIds::fontstyle, 5);
                else if(strTokens[0].trim()=="italic underlined")
                    setProperty(widgetData, CabbageIdentifierIds::fontstyle, 6);
                else if(strTokens[0].trim()=="bold italic underlined")
                    setProperty(widgetData, CabbageIdentifierIds::fontstyle, 7);
                else
                    setProperty(widgetData, CabbageIdentifierIds::fontstyle, 0);
            }

            else if(identArray[indx].equalsIgnoreCase("textbox") || identArray[indx].equalsIgnoreCase("valuetextbox"))
            {
                setProperty(widgetData, CabbageIdentifierIds::valuetextbox, strTokens[0].trim().getFloatValue());
            }

            else if(identArray[indx].equalsIgnoreCase("velocity"))
            {
                setProperty(widgetData, CabbageIdentifierIds::velocity, strTokens[0].trim().getFloatValue());
            }

            else if(identArray[indx].equalsIgnoreCase("gradient"))
            {
                setProperty(widgetData, CabbageIdentifierIds::gradient, strTokens[0].trim().getFloatValue());
            }

            else if(identArray[indx].equalsIgnoreCase("scrubberposition"))
            {
                var scrubberInfo;
                scrubberInfo.append(strTokens[0].trim().getIntValue());
                for(int i=1; i<strTokens.size(); i++)
                {
                    scrubberInfo.append(strTokens[i].trim().getDoubleValue());
                    setProperty(widgetData, CabbageIdentifierIds::scrubberposition, scrubberInfo);
                }

                setProperty(widgetData, CabbageIdentifierIds::scrubberposition, scrubberInfo);

            }

            else if(identArray[indx].equalsIgnoreCase("logger"))
            {
                setProperty(widgetData, CabbageIdentifierIds::logger, strTokens[0].trim().getIntValue());
            }

            else if(identArray[indx].equalsIgnoreCase("child"))
            {
                setProperty(widgetData, CabbageIdentifierIds::child, strTokens[0].trim().getIntValue());
            }

            else if(identArray[indx].equalsIgnoreCase("show"))
            {
                setProperty(widgetData, CabbageIdentifierIds::show, strTokens[0].trim().getIntValue());
            }

            else if(identArray[indx].equalsIgnoreCase("zoom"))
            {
                setProperty(widgetData, CabbageIdentifierIds::zoom, strTokens[0].trim().getFloatValue());
//                if(strTokens.size()>1)
//                    setProperty(widgetData, CabbageIdentifierIds::startpos, strTokens[1].trim().getFloatValue());
//                else
//                    setProperty(widgetData, CabbageIdentifierIds::startpos, -1);
            }

            else if(identArray[indx].equalsIgnoreCase("samplerange"))
            {
                setProperty(widgetData, CabbageIdentifierIds::startpos, strTokens[0].trim().getFloatValue());
                if(strTokens.size()>1)
                    setProperty(widgetData, CabbageIdentifierIds::endpos, strTokens[1].trim().getFloatValue());
                else
                    setProperty(widgetData, CabbageIdentifierIds::endpos, -1);
            }

            else if(identArray[indx].equalsIgnoreCase("wrap"))
            {
                setProperty(widgetData, CabbageIdentifierIds::wrap, strTokens[0].trim().getFloatValue());
            }

            else if(identArray[indx].equalsIgnoreCase("readonly"))
            {
                setProperty(widgetData, CabbageIdentifierIds::readonly, strTokens[0].trim().getFloatValue());
            }

            else if(identArray[indx].equalsIgnoreCase("tablenum")||
                    (identArray[indx].equalsIgnoreCase("tablenumber")||
                     identArray[indx].equalsIgnoreCase("tablenumbers")||
                     identArray[indx].equalsIgnoreCase("tablenumbs")))
            {
                var value;
                var tableConfig;
                //value.append(tableNum);
                //tableNumbers.add(tableNum);

                for(int i=0; i<strTokens.size(); i++)
                {
                    if(strTokens[i].contains(":") && strTokens.size()>0)
                    {
                        //split string into two
                        StringArray tablesData;
                        tablesData.addTokens(strTokens[i], ":", "");
                        var tables;
                        for(int w=0; w<tablesData.size(); w++)
                        {
                            tables.append(tablesData[w]);
                            value.append(tablesData[w]);
                            //Logger::writeToLog(tablesData[w]);

                        }
                        //value.append(tables);
                        tableConfig.append(tables);
                    }
                    else
                    {
                        //tableNumbers.add(strTokens[i].trim().getFloatValue());
                        value.append(strTokens[i].trim().getFloatValue());
                        tableConfig.append(strTokens[i].trim().getFloatValue());
                        //Logger::writeToLog(strTokens[i].trim());
                    }
                }

                setProperty(widgetData, CabbageIdentifierIds::tableconfig, tableConfig);
                setProperty(widgetData, CabbageIdentifierIds::tablenumber, value);
                //setProperty(widgetData, CabbageIdentifierIds::updatetable, 1);

            }
            else if(identArray[indx].equalsIgnoreCase("popup"))
            {
                setProperty(widgetData, CabbageIdentifierIds::popup, strTokens[0].trim().getIntValue());
            }

            else if(identArray[indx].equalsIgnoreCase("resize"))
            {
                setProperty(widgetData, CabbageIdentifierIds::popup, strTokens[0].trim().getIntValue());
            }

            else if(identArray[indx].equalsIgnoreCase("resize"))
            {
                setProperty(widgetData, CabbageIdentifierIds::popup, strTokens[0].trim().getIntValue());
            }

            else if(identArray[indx].equalsIgnoreCase("rescale"))
            {
                setProperty(widgetData, CabbageIdentifierIds::scalex, strTokens[0].trim().getFloatValue());
                setProperty(widgetData, CabbageIdentifierIds::scaley, strTokens[1].trim().getFloatValue());
            }

            else if(identArray[indx].equalsIgnoreCase("resizemode"))
            {
                var value;
                if(strTokens.size()>0)
                    for(int i=0; i<strTokens.size(); i++)
                    {
                        //add table modes to val array
                        value.append(strTokens[i].trim().getFloatValue());
                    }
                setProperty(widgetData, CabbageIdentifierIds::resizemode, value);
            }

            else if(identArray[indx].equalsIgnoreCase("amprange"))
            {
                if(strTokens.size()<3)
                    warningMessages+="Not enough paramters passed to amprange(): usage amprange(min, max, tablenumber, quantise\")\n";

                var value;
                var temp = getProperty(widgetData, CabbageIdentifierIds::amprange);

                for(int i=0; i<temp.size(); i++)
                    value.append(temp[i]);

                for(int i=0; i<strTokens.size(); i++)
                {
                    //add table modes to val array
                    value.append(strTokens[i].trim().getFloatValue());
                }
                //set default quantise space..
                if(strTokens.size()==3)
                    value.append(0.01);

                setProperty(widgetData, CabbageIdentifierIds::amprange, value);
                //identifiers that appear more than once need to use indx-- so we can check for another instance
                indx--;
            }

            else if(identArray[indx].equalsIgnoreCase("outlinethickness"))
            {
                //setProperty(widgetData, "line", strTokens[0].trim().getFloatValue());
                setProperty(widgetData, CabbageIdentifierIds::outlinethickness, strTokens[0].trim().getFloatValue());
            }

            else if(identArray[indx].equalsIgnoreCase("linethickness"))
            {
                //setProperty(widgetData, "line", strTokens[0].trim().getFloatValue());
                setProperty(widgetData, CabbageIdentifierIds::linethickness, strTokens[0].trim().getFloatValue());
            }

            else if(identArray[indx].equalsIgnoreCase("trackerthickness"))
            {
                //setProperty(widgetData, "line", strTokens[0].trim().getFloatValue());
                setProperty(widgetData, CabbageIdentifierIds::trackerthickness, strTokens[0].trim().getFloatValue());
            }

            else if(identArray[indx].equalsIgnoreCase("value"))
            {
                setProperty(widgetData, CabbageIdentifierIds::value, strTokens[0].trim().getFloatValue());
            }

            else if(identArray[indx].equalsIgnoreCase("scrollbars"))
            {
                setProperty(widgetData, CabbageIdentifierIds::scrollbars, strTokens[0].trim().getIntValue());
            }

            //============================================
            //sample identifiers for stepper class
            //============================================
            else if(identArray[indx].equalsIgnoreCase("numberofsteps"))
            {
                setProperty(widgetData, "numberofsteps", strTokens[0].trim().getFloatValue());
            }

            else if(identArray[indx].equalsIgnoreCase("stepbpm"))
            {
                setProperty(widgetData, "stepbpm", strTokens[0].trim().getIntValue());
            }


            strTokens.clear();
        }
    }
}
//=========================================================================
//retrieve numerical attributes
float CabbageWidgetData::getNumProp(ValueTree widgetData, Identifier prop)
{
    var props = getProperty(widgetData, prop);
    if(props.size()>0)
        return props[0];
    else
        return getProperty(widgetData, prop);
}


String CabbageWidgetData::getStringProp(ValueTree widgetData, Identifier prop)
{
    var string = getProperty(widgetData, prop);
    const Array<var>* array = string.getArray();
    if(array)
    {
        return array->getReference(0).toString();
    }
    else
        return string.toString();

}

void CabbageWidgetData::setNumProp(ValueTree widgetData, Identifier prop, float val)
{
    setProperty(widgetData, prop, val);
}

void CabbageWidgetData::setStringProp(ValueTree widgetData, Identifier name, const String value)
{
    widgetData.setProperty(name, value, 0);
}

void CabbageWidgetData::setProperty(ValueTree widgetData, Identifier name, const var &value)
{
    widgetData.setProperty(name, value, 0);
}

var CabbageWidgetData::getProperty(ValueTree widgetData, Identifier name)
{
    return widgetData.getProperty(name);
}

ValueTree CabbageWidgetData::getValueTreeForComponent(ValueTree widgetData, String name)
{
    for(int i=0; i<widgetData.getNumChildren(); i++)
    {
        if(name==widgetData.getChild(i).getProperty(CabbageIdentifierIds::name).toString())
        {
            return widgetData.getChild(i);
        }
    }

    return ValueTree("empty");
}

//================================================================================================
// these mthods can be used to find the values of indentifiers
// by passing a single line of text to them.
//================================================================================================
Rectangle<int> CabbageWidgetData::getBoundsFromText(String text)
{
    text = " "+text;
    String subString = text.substring(text.indexOfWholeWord("bounds"));
    subString = subString.substring(7, subString.indexOf(")"));
    StringArray strTokens;
    strTokens.addTokens(subString, ",()", "");
    return Rectangle<int>(strTokens[0].getIntValue(), strTokens[1].getIntValue(), strTokens[2].getIntValue(), strTokens[3].getIntValue());
}

Rectangle<int> CabbageWidgetData::getBounds(ValueTree widgetData)
{
    Rectangle<int> rect(getProperty(widgetData, CabbageIdentifierIds::left),
                        getProperty(widgetData, CabbageIdentifierIds::top),
                        getProperty(widgetData, CabbageIdentifierIds::width),
                        getProperty(widgetData, CabbageIdentifierIds::height));
    return rect;
}

//===================================================================
Colour CabbageWidgetData::getColourFromText(String text)
{
    StringArray strTokens;
    strTokens.addTokens(text, ",", "");
    Colour colour;
    if(strTokens.size()<2)
    {
        if(strTokens[0].trim()=="0")
            colour = Colours::white.withAlpha(1.f);
        else
            colour = Colours::findColourForName(strTokens[0].trim(), Colours::white);

    }
    else if(strTokens.size()==4)
        colour = Colour::fromRGBA (strTokens[0].getIntValue(),
                                   strTokens[1].getIntValue(),
                                   strTokens[2].getIntValue(),
                                   strTokens[3].getIntValue());
    else if(strTokens.size()==3)
        colour = Colour::fromRGB (strTokens[0].getIntValue(),
                                  strTokens[1].getIntValue(),
                                  strTokens[2].getIntValue());
    return colour;
}


//===================================================================
String CabbageWidgetData::getBoundsTextAsCabbageCode(Rectangle<int> rect)
{
    const String boundsText = "bounds(" + String(rect.getX()) + ", " + String(rect.getY()) + ", " + String(rect.getWidth()) + ", " + String(rect.getHeight()) + "), ";
    return boundsText;
}

//===================================================================
String CabbageWidgetData::getNumericalValueTextAsCabbageCode(ValueTree widgetData, String identifier)
{
    ValueTree tempData("tempTree");
    const String type = CabbageWidgetData::getStringProp(widgetData, CabbageIdentifierIds::type);
    CabbageWidgetData::setWidgetState(tempData, type, -99);

    if(type.contains("slider") && identifier=="value")
    {
        return "range(" + String(CabbageWidgetData::getNumProp(widgetData, CabbageIdentifierIds::min))
               + ", "
               + String(CabbageWidgetData::getNumProp(widgetData, CabbageIdentifierIds::max))
               + ", "
               + String(CabbageWidgetData::getNumProp(widgetData, CabbageIdentifierIds::value))
               + ", "
               + String(CabbageWidgetData::getNumProp(widgetData, CabbageIdentifierIds::sliderskew))
               + ", "
               + String(CabbageWidgetData::getNumProp(widgetData, CabbageIdentifierIds::sliderincr))
               + "), ";
    }
    if(type.contains("range") && identifier=="value")
    {
        return "range(" + String(CabbageWidgetData::getNumProp(widgetData, CabbageIdentifierIds::min))
               + ", "
               + String(CabbageWidgetData::getNumProp(widgetData, CabbageIdentifierIds::max))
               + ", "
               + String(CabbageWidgetData::getNumProp(widgetData, CabbageIdentifierIds::minvalue))
               + ":"
               + String(CabbageWidgetData::getNumProp(widgetData, CabbageIdentifierIds::maxvalue))
               + ", "
               + String(CabbageWidgetData::getNumProp(widgetData, CabbageIdentifierIds::sliderskew))
               + ", "
               + String(CabbageWidgetData::getNumProp(widgetData, CabbageIdentifierIds::sliderincr))
               + "), ";
    }
    else if(type == "xypad" && identifier=="value")
    {
        return "rangex(" + String(CabbageWidgetData::getNumProp(widgetData, CabbageIdentifierIds::minx))
               + ", "
               + String(CabbageWidgetData::getNumProp(widgetData, CabbageIdentifierIds::maxx))
               + ", "
               + String(CabbageWidgetData::getNumProp(widgetData, CabbageIdentifierIds::valuex))
               +"), rangey("
               + String(CabbageWidgetData::getNumProp(widgetData, CabbageIdentifierIds::miny))
               + ", "
               + String(CabbageWidgetData::getNumProp(widgetData, CabbageIdentifierIds::maxy))
               + ", "
               + String(CabbageWidgetData::getNumProp(widgetData, CabbageIdentifierIds::valuey))
               +"), ";
    }
    else
    {
        if(CabbageWidgetData::getNumProp(widgetData, identifier)!=CabbageWidgetData::getNumProp(tempData, identifier))
        {
            return identifier + "(" + String(CabbageWidgetData::getNumProp(widgetData, identifier)) + "), ";
        }
    }
    return String::empty;
}

//===================================================================
String CabbageWidgetData::getRotateTextAsCabbageCode(ValueTree widgetData)
{
    ValueTree tempData("tempTree");
    const String type = CabbageWidgetData::getStringProp(widgetData, CabbageIdentifierIds::type);
    CabbageWidgetData::setWidgetState(tempData, type, -99);



    if(CabbageWidgetData::getNumProp(widgetData, CabbageIdentifierIds::rotate)!=CabbageWidgetData::getNumProp(tempData, CabbageIdentifierIds::rotate)
            || CabbageWidgetData::getNumProp(widgetData, CabbageIdentifierIds::pivotx)!=CabbageWidgetData::getNumProp(tempData, CabbageIdentifierIds::pivotx)
            || CabbageWidgetData::getNumProp(widgetData, CabbageIdentifierIds::pivoty)!=CabbageWidgetData::getNumProp(tempData, CabbageIdentifierIds::pivoty))
    {
        const float rotate = CabbageWidgetData::getNumProp(widgetData, CabbageIdentifierIds::rotate);
        const float pivotx = CabbageWidgetData::getNumProp(widgetData, CabbageIdentifierIds::pivotx);
        const float pivoty = CabbageWidgetData::getNumProp(widgetData, CabbageIdentifierIds::pivoty);

        return "rotate(" + String(rotate) + ", " + String(pivotx) + ", " + String(pivoty) + "), ";
    }

    return String::empty;
}
//===================================================================
String CabbageWidgetData::getSimpleTextAsCabbageCode(ValueTree widgetData, String identifier)
{
    ValueTree tempData("tempTree");
    const String type = CabbageWidgetData::getStringProp(widgetData, CabbageIdentifierIds::type);
    CabbageWidgetData::setWidgetState(tempData, type, -99);

    if(CabbageWidgetData::getStringProp(widgetData, identifier)!=CabbageWidgetData::getStringProp(tempData, identifier))
    {
        const String text = CabbageWidgetData::getStringProp(widgetData, identifier);
        return identifier+"(\"" + text + "\"), ";
    }

    return String::empty;
}
//===================================================================
String CabbageWidgetData::getImagesTextAsCabbageCode(ValueTree widgetData)
{
    ValueTree tempData("tempTree");
    const String type = CabbageWidgetData::getStringProp(widgetData, CabbageIdentifierIds::type);
    CabbageWidgetData::setWidgetState(tempData, type, -99);
    String returnText = "";

    if(CabbageWidgetData::getStringProp(widgetData, CabbageIdentifierIds::imgbuttonon)
            != CabbageWidgetData::getStringProp(tempData, CabbageIdentifierIds::imgbuttonon))
    {
        const String text = CabbageWidgetData::getStringProp(widgetData, CabbageIdentifierIds::imgbuttonon);
        returnText = "imgfile(\"On\", \"" + text + "\"), ";
    }

    if(CabbageWidgetData::getStringProp(widgetData, CabbageIdentifierIds::imgbuttonoff)
            != CabbageWidgetData::getStringProp(tempData, CabbageIdentifierIds::imgbuttonoff))
    {
        const String text = CabbageWidgetData::getStringProp(widgetData, CabbageIdentifierIds::imgbuttonoff);
        returnText = returnText+"imgfile(\"Off\", \"" + text + "\"), ";
    }

    return returnText;
}
//===================================================================
String CabbageWidgetData::getMultiItemTextAsCabbageCode(ValueTree widgetData, String identifier)
{
    var items = CabbageWidgetData::getProperty(widgetData, identifier);
    String channelString = "";

    const Array<var>* array = items.getArray();
    if(array)
    {
        for(int i = 0 ; i < array->size()-1 ; i++)
        {
            const String text = array->getReference(array->size()-1).toString();
            channelString = channelString+ "\"" + array->getReference(i).toString() + "\", ";
        }

        if( array->size()==1)
            return identifier+"(\""+channelString+array->getReference(array->size()-1).toString()+"\"), ";
        else
            return identifier+"("+channelString+ "\"" + array->getReference(array->size()-1).toString()+"\"), ";
    }

    StringArray stringArray;
    stringArray.addLines(items.toString());
    for( int i = 0 ; i < stringArray.size(); i++)
        stringArray.set(i, "\"" + stringArray[i] + "\"");
    //CabbageUtilities::debug(stringArray.joinIntoString(", "));

    if(stringArray.joinIntoString("\n").length()>0)
        return identifier+"("+stringArray.joinIntoString(", ")+"), ";
    else
        return String::empty;
}

//===================================================================
String CabbageWidgetData::getColoursTextAsCabbageCode(ValueTree widgetData)
{
    ValueTree tempData("tempTree");
    //tempData = widgetData.createCopy();
    const String type = CabbageWidgetData::getStringProp(widgetData, CabbageIdentifierIds::type);
    CabbageWidgetData::setWidgetState(tempData, type, -99);
    String colourString;

    if(CabbageWidgetData::getStringProp(widgetData, CabbageIdentifierIds::colour)!=CabbageWidgetData::getStringProp(tempData, CabbageIdentifierIds::colour))
    {
        const String identifier = (type=="image"
                                   || type.contains("slider")
                                   || type=="label"
                                   || type=="groupbox"
                                   || type=="xypad"
                                   || type=="soundfiler"
                                   ? "colour(" : "colour:0(");
        const Colour col = Colour::fromString(CabbageWidgetData::getStringProp(widgetData, CabbageIdentifierIds::colour));
        colourString = colourString << identifier << (float)col.getRed() << ", " << (float)col.getGreen() << ", " << (float)col.getBlue() << ", " << (float)col.getAlpha() << "), ";
    }

    if(CabbageWidgetData::getStringProp(widgetData, CabbageIdentifierIds::oncolour)!=CabbageWidgetData::getStringProp(tempData, CabbageIdentifierIds::oncolour))
    {
        const Colour col = Colour::fromString(CabbageWidgetData::getStringProp(widgetData, CabbageIdentifierIds::oncolour));
        colourString = colourString << "colour:1(" << (float)col.getRed() << ", " << (float)col.getGreen() << ", " << (float)col.getBlue() << ", " << (float)col.getAlpha() << "), ";
    }

    if(CabbageWidgetData::getStringProp(widgetData, CabbageIdentifierIds::fontcolour)!=CabbageWidgetData::getStringProp(tempData, CabbageIdentifierIds::fontcolour))
    {
        const Colour col = Colour::fromString(CabbageWidgetData::getStringProp(widgetData, CabbageIdentifierIds::fontcolour));
        colourString = colourString << (type.contains("slider")
                                        || type=="csoundoutput"
                                        || type=="xypad"
                                        || type=="encoder" ? "fontcolour(" : "fontcolour:0(") << (float)col.getRed() << ", " << (float)col.getGreen() << ", " << (float)col.getBlue() << ", " << (float)col.getAlpha() << "), ";
    }

    if(CabbageWidgetData::getStringProp(widgetData, CabbageIdentifierIds::onfontcolour)!=CabbageWidgetData::getStringProp(tempData, CabbageIdentifierIds::onfontcolour))
    {
        const Colour col = Colour::fromString(CabbageWidgetData::getStringProp(widgetData, CabbageIdentifierIds::onfontcolour));
        colourString = colourString << "fontcolour:1(" << (float)col.getRed() << ", " << (float)col.getGreen() << ", " << (float)col.getBlue() << ", " << (float)col.getAlpha() << "), ";
    }

    if(CabbageWidgetData::getStringProp(widgetData, CabbageIdentifierIds::outlinecolour)!=CabbageWidgetData::getStringProp(tempData, CabbageIdentifierIds::outlinecolour))
    {
        const Colour col = Colour::fromString(CabbageWidgetData::getStringProp(widgetData, CabbageIdentifierIds::outlinecolour));
        colourString = colourString << "outlinecolour(" << (float)col.getRed() << ", " << (float)col.getGreen() << ", " << (float)col.getBlue() << ", " << (float)col.getAlpha() << "), ";
    }

    if(CabbageWidgetData::getStringProp(widgetData, CabbageIdentifierIds::whitenotecolour)!=CabbageWidgetData::getStringProp(tempData, CabbageIdentifierIds::whitenotecolour))
    {
        const Colour col = Colour::fromString(CabbageWidgetData::getStringProp(widgetData, CabbageIdentifierIds::whitenotecolour));
        colourString = colourString << "whitenotecolour(" << (float)col.getRed() << ", " << (float)col.getGreen() << ", " << (float)col.getBlue() << ", " << (float)col.getAlpha() << "), ";
    }

    if(CabbageWidgetData::getStringProp(widgetData, CabbageIdentifierIds::blacknotecolour)!=CabbageWidgetData::getStringProp(tempData, CabbageIdentifierIds::blacknotecolour))
    {
        const Colour col = Colour::fromString(CabbageWidgetData::getStringProp(widgetData, CabbageIdentifierIds::blacknotecolour));
        colourString = colourString << "blacknotecolour(" << (float)col.getRed() << ", " << (float)col.getGreen() << ", " << (float)col.getBlue() << ", " << (float)col.getAlpha() << "), ";
    }

    if(CabbageWidgetData::getStringProp(widgetData, CabbageIdentifierIds::noteseparatorcolour)!=CabbageWidgetData::getStringProp(tempData, CabbageIdentifierIds::noteseparatorcolour))
    {
        const Colour col = Colour::fromString(CabbageWidgetData::getStringProp(widgetData, CabbageIdentifierIds::noteseparatorcolour));
        colourString = colourString << "noteseparatorcolour(" << (float)col.getRed() << ", " << (float)col.getGreen() << ", " << (float)col.getBlue() << ", " << (float)col.getAlpha() << "), ";
    }

    if(CabbageWidgetData::getStringProp(widgetData, CabbageIdentifierIds::arrowbackgroundcolour)!=CabbageWidgetData::getStringProp(tempData, CabbageIdentifierIds::arrowbackgroundcolour))
    {
        const Colour col = Colour::fromString(CabbageWidgetData::getStringProp(widgetData, CabbageIdentifierIds::arrowbackgroundcolour));
        colourString = colourString << "arrowbackgroundcolour(" << (float)col.getRed() << ", " << (float)col.getGreen() << ", " << (float)col.getBlue() << ", " << (float)col.getAlpha() << "), ";
    }

    if(CabbageWidgetData::getStringProp(widgetData, CabbageIdentifierIds::arrowcolour)!=CabbageWidgetData::getStringProp(tempData, CabbageIdentifierIds::arrowcolour))
    {
        const Colour col = Colour::fromString(CabbageWidgetData::getStringProp(widgetData, CabbageIdentifierIds::arrowcolour));
        colourString = colourString << "arrowcolour(" << (float)col.getRed() << ", " << (float)col.getGreen() << ", " << (float)col.getBlue() << ", " << (float)col.getAlpha() << "), ";
    }

    if(CabbageWidgetData::getStringProp(widgetData, CabbageIdentifierIds::ballcolour)!=CabbageWidgetData::getStringProp(tempData, CabbageIdentifierIds::ballcolour))
    {
        const Colour col = Colour::fromString(CabbageWidgetData::getStringProp(widgetData, CabbageIdentifierIds::ballcolour));
        colourString = colourString << "ballcolour(" << (float)col.getRed() << ", " << (float)col.getGreen() << ", " << (float)col.getBlue() << ", " << (float)col.getAlpha() << "), ";
    }

    if(CabbageWidgetData::getStringProp(widgetData, CabbageIdentifierIds::backgroundcolour)!=CabbageWidgetData::getStringProp(tempData, CabbageIdentifierIds::backgroundcolour))
    {
        const Colour col = Colour::fromString(CabbageWidgetData::getStringProp(widgetData, CabbageIdentifierIds::backgroundcolour));
        colourString = colourString << "backgroundcolour(" << (float)col.getRed() << ", " << (float)col.getGreen() << ", " << (float)col.getBlue() << ", " << (float)col.getAlpha() << "), ";
    }


    if(CabbageWidgetData::getStringProp(widgetData, CabbageIdentifierIds::tablebackgroundcolour)!=CabbageWidgetData::getStringProp(tempData, CabbageIdentifierIds::tablebackgroundcolour))
    {
        const Colour col = Colour::fromString(CabbageWidgetData::getStringProp(widgetData, CabbageIdentifierIds::tablebackgroundcolour));
        colourString = colourString << "tablebackgroundcolour(" << (float)col.getRed() << ", " << (float)col.getGreen() << ", " << (float)col.getBlue() << ", " << (float)col.getAlpha() << "), ";
    }

    if(CabbageWidgetData::getStringProp(widgetData, CabbageIdentifierIds::textcolour)!=CabbageWidgetData::getStringProp(tempData, CabbageIdentifierIds::textcolour))
    {
        const Colour col = Colour::fromString(CabbageWidgetData::getStringProp(widgetData, CabbageIdentifierIds::textcolour));
        colourString = colourString << "textcolour(" << (float)col.getRed() << ", " << (float)col.getGreen() << ", " << (float)col.getBlue() << ", " << (float)col.getAlpha() << "), ";
    }

    if(CabbageWidgetData::getStringProp(widgetData, CabbageIdentifierIds::trackercolour)!=CabbageWidgetData::getStringProp(tempData, CabbageIdentifierIds::trackercolour))
    {
        const Colour col = Colour::fromString(CabbageWidgetData::getStringProp(widgetData, CabbageIdentifierIds::trackercolour));
        colourString = colourString << "trackercolour(" << (float)col.getRed() << ", " << (float)col.getGreen() << ", " << (float)col.getBlue() << ", " << (float)col.getAlpha() << "), ";
    }


    return colourString;
}
//===================================================================
String CabbageWidgetData::getCabbageCodeFromIdentifiers(ValueTree widgetData, const String currentLineText)
{

    String cabbageCode = getStringProp(widgetData, CabbageIdentifierIds::type) + " "
                         + getBoundsTextAsCabbageCode(getBounds(widgetData))
                         + getMultiItemTextAsCabbageCode(widgetData, "channel")
                         + getMultiItemTextAsCabbageCode(widgetData, "identchannel")
                         + getNumericalValueTextAsCabbageCode(widgetData, "value")
                         + getMultiItemTextAsCabbageCode(widgetData, "text")
                         + getColoursTextAsCabbageCode(widgetData)
                         + getRotateTextAsCabbageCode(widgetData)
                         + getNumericalValueTextAsCabbageCode(widgetData, "alpha")
                         + getNumericalValueTextAsCabbageCode(widgetData, "corners")
                         + getNumericalValueTextAsCabbageCode(widgetData, "active")
                         + getNumericalValueTextAsCabbageCode(widgetData, "visible")
                         + getNumericalValueTextAsCabbageCode(widgetData, "valuetextbox")
                         + getNumericalValueTextAsCabbageCode(widgetData, "zoom")
                         + getNumericalValueTextAsCabbageCode(widgetData, "outlinethickness")
                         + getNumericalValueTextAsCabbageCode(widgetData, "velocity")
                         + getSimpleTextAsCabbageCode(widgetData, "popuptext")
                         + getSimpleTextAsCabbageCode(widgetData, "align")
                         + getSimpleTextAsCabbageCode(widgetData, "file")
                         + getSimpleTextAsCabbageCode(widgetData, "shape")
                         + getImagesTextAsCabbageCode(widgetData)

                         //lastly, add a bracket in cases of plants that open on a line of widget code rather than on a new line
                         + (getNumProp(widgetData, "containsOpeningCurlyBracket")==1 ? "{" : String::empty);

    return cabbageCode;
}


String CabbageWidgetData::removeWidgetFromValueTree(ValueTree wData, int lineNumber)
{

    for( int  i = 0 ; i < wData.getNumChildren() ; i++ )
    {
        if(int(wData.getChild(i).getProperty(CabbageIdentifierIds::linenumber))==lineNumber)
        {
            const String childName = wData.getChild(i).getProperty(CabbageIdentifierIds::name).toString();
            wData.removeChild(i, 0);
            return childName;
        }
    }

    return String::empty;
}


//===================================================================
float CabbageWidgetData::getTableChannelValues(int index)
{
    jassert(false);
}
//===================================================================
void CabbageWidgetData::addTableChannelValues()
{
    jassert(false);
}
//===================================================================
void CabbageWidgetData::setTableChannelValues(int index, float val)
{
    jassert(false);
}
//===================================================================
void CabbageWidgetData::scaleWidget(Point<float> scale)
{
    jassert(false);
}
//===================================================================
