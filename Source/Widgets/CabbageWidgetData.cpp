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
    setProperty(widgetData, CabbageIdentifierIds::svgdebug, 0);
	setProperty(widgetData, CabbageIdentifierIds::allowboundsupdate, 0);
	setProperty(widgetData, CabbageIdentifierIds::identchannelmessage, "");
    setProperty(widgetData, CabbageIdentifierIds::popuptext, "");
    setProperty(widgetData, CabbageIdentifierIds::alpha, 1);
    setProperty(widgetData, CabbageIdentifierIds::visible, 1);
    setProperty(widgetData, CabbageIdentifierIds::rotate, 0.f);
    setProperty(widgetData, CabbageIdentifierIds::pivotx, 0.f);
    setProperty(widgetData, CabbageIdentifierIds::pivoty, 0.f);
	setProperty(widgetData, CabbageIdentifierIds::linenumber, ID);


	int top, left, width, height;
    StringArray strTokens;
    strTokens.addTokens(lineFromCsd, " ", "\"");

    setProperty(widgetData, CabbageIdentifierIds::channelarray, "");
    //===============sliders==================//
    if(strTokens[0].trim() == "hslider" || strTokens[0].trim() == "hslider2" || strTokens[0].trim() == "hslider3")
    {
        setProperty(widgetData, CabbageIdentifierIds::basetype, "interactive");
        setProperty(widgetData, CabbageIdentifierIds::top, 10);
        setProperty(widgetData, CabbageIdentifierIds::left, 10);
        setProperty(widgetData, CabbageIdentifierIds::width, 150);
        setProperty(widgetData, CabbageIdentifierIds::height, 50);
        var channels;
        channels.append("hslider");
        setProperty(widgetData, CabbageIdentifierIds::channel, channels);
        setProperty(widgetData, CabbageIdentifierIds::min, 0);
        setProperty(widgetData, CabbageIdentifierIds::max, 1);
        setProperty(widgetData, CabbageIdentifierIds::value, 0);
        setProperty(widgetData, CabbageIdentifierIds::text, "");
        setProperty(widgetData, CabbageIdentifierIds::textbox, 0.f);
        setProperty(widgetData, CabbageIdentifierIds::caption, "");
        setProperty(widgetData, CabbageIdentifierIds::colour, Colours::white.toString());
        setProperty(widgetData, CabbageIdentifierIds::trackercolour, Colour(0, 118, 38).toString());
        setProperty(widgetData, CabbageIdentifierIds::fontcolour, CabbageUtilities::getComponentFontColour().toString());
        setProperty(widgetData, CabbageIdentifierIds::textcolour, CabbageUtilities::getComponentFontColour().toString());
        setProperty(widgetData, CabbageIdentifierIds::sliderskew, 1);
        setProperty(widgetData, CabbageIdentifierIds::sliderincr, .001);
        setProperty(widgetData, CabbageIdentifierIds::velocity, 0);

        setProperty(widgetData, CabbageIdentifierIds::midichan, -99);
        setProperty(widgetData, CabbageIdentifierIds::midictrl, -99);
        //these don't appear in the props dialog
        setProperty(widgetData, CabbageIdentifierIds::name, "hslider");
        setProperty(widgetData, CabbageIdentifierIds::type, getProperty(widgetData, "name").toString());
        setProperty(widgetData, CabbageIdentifierIds::name, getProperty(widgetData, "name").toString()+String(ID));
        if(strTokens[0].trim() == "hslider2")
            setProperty(widgetData, CabbageIdentifierIds::kind, "horizontal2");
        else if(strTokens[0].trim() == "hslider3")
            setProperty(widgetData, CabbageIdentifierIds::kind, "horizontal3");
        else
            setProperty(widgetData, CabbageIdentifierIds::kind, "horizontal");
        setProperty(widgetData, CabbageIdentifierIds::decimalplaces, 1);
        setProperty(widgetData, CabbageIdentifierIds::identchannel, "");
        setProperty(widgetData, CabbageIdentifierIds::trackerthickness, .1);
        setProperty(widgetData, CabbageIdentifierIds::visible, 1);
        setProperty(widgetData, CabbageIdentifierIds::gradient, 1);
        setProperty(widgetData, CabbageIdentifierIds::svgslider, "");
        setProperty(widgetData, CabbageIdentifierIds::svgsliderbg, "");


    }
    else if(strTokens[0].trim() == "socketsend" || strTokens[0].trim() == "socketreceive")
    {
        setProperty(widgetData, CabbageIdentifierIds::basetype, "layout");
        setProperty(widgetData, CabbageIdentifierIds::socketaddress, "");
        setProperty(widgetData, CabbageIdentifierIds::socketport, 0);
    }
    else if(strTokens[0].trim() == "vslider" || strTokens[0].trim() == "vslider2" ||  strTokens[0].trim() == "vslider3")
    {
        setProperty(widgetData, CabbageIdentifierIds::basetype, "interactive");
        setProperty(widgetData, CabbageIdentifierIds::top, 10);
        setProperty(widgetData, CabbageIdentifierIds::left, 10);
        setProperty(widgetData, CabbageIdentifierIds::width, 50);
        setProperty(widgetData, CabbageIdentifierIds::height, 150);
        var channels;
        channels.append("vslider");
        setProperty(widgetData, CabbageIdentifierIds::channel, channels);
        setProperty(widgetData, CabbageIdentifierIds::min, 0);
        setProperty(widgetData, CabbageIdentifierIds::max, 1);
        setProperty(widgetData, CabbageIdentifierIds::value, 0);
        setProperty(widgetData, CabbageIdentifierIds::text, "");
        setProperty(widgetData, CabbageIdentifierIds::textbox, 0.f);
        setProperty(widgetData, CabbageIdentifierIds::caption, "");
        setProperty(widgetData, CabbageIdentifierIds::colour, Colours::white.toString());
        setProperty(widgetData, CabbageIdentifierIds::trackercolour, Colour(0, 118, 38).toString());
        setProperty(widgetData, CabbageIdentifierIds::fontcolour, CabbageUtilities::getComponentFontColour().toString());
        setProperty(widgetData, CabbageIdentifierIds::textcolour, CabbageUtilities::getComponentFontColour().toString());
        setProperty(widgetData, CabbageIdentifierIds::sliderskew, 1);
        setProperty(widgetData, CabbageIdentifierIds::sliderincr, .001);
        setProperty(widgetData, CabbageIdentifierIds::velocity, 0);
        setProperty(widgetData, CabbageIdentifierIds::midichan, -99);
        setProperty(widgetData, CabbageIdentifierIds::midictrl, -99);
        //these don't appear in the props dialog
        setProperty(widgetData, CabbageIdentifierIds::name, "vslider");
        setProperty(widgetData, CabbageIdentifierIds::type, getProperty(widgetData, "name").toString());
        setProperty(widgetData, CabbageIdentifierIds::name, getProperty(widgetData, "name").toString()+String(ID));
        if(strTokens[0].trim() == "vslider2")
            setProperty(widgetData, CabbageIdentifierIds::kind, "vertical2");
        else if(strTokens[0].trim() == "vslider3")
            setProperty(widgetData, CabbageIdentifierIds::kind, "vertical3");
        else
            setProperty(widgetData, CabbageIdentifierIds::kind, "vertical");

        setProperty(widgetData, CabbageIdentifierIds::decimalplaces, 1);
        setProperty(widgetData, CabbageIdentifierIds::trackerthickness, .1);
        setProperty(widgetData, CabbageIdentifierIds::identchannel, "");
        setProperty(widgetData, CabbageIdentifierIds::visible, 1);
        setProperty(widgetData, CabbageIdentifierIds::svgslider, "");
        setProperty(widgetData, CabbageIdentifierIds::svgsliderbg, "");
    }
    else if(strTokens[0].trim() == "rslider")
    {
        setProperty(widgetData, CabbageIdentifierIds::basetype, "interactive");
        setProperty(widgetData, CabbageIdentifierIds::top, 10);
        setProperty(widgetData, CabbageIdentifierIds::left, 10);
        setProperty(widgetData, CabbageIdentifierIds::width, 60);
        setProperty(widgetData, CabbageIdentifierIds::height, 60);
        var channels;
        channels.append("rslider");
        setProperty(widgetData, CabbageIdentifierIds::channel, channels);
        setProperty(widgetData, CabbageIdentifierIds::min, 0);
        setProperty(widgetData, CabbageIdentifierIds::max, 1);
        setProperty(widgetData, CabbageIdentifierIds::value, 0);
        setProperty(widgetData, CabbageIdentifierIds::sliderskew, 1);
        setProperty(widgetData, CabbageIdentifierIds::sliderincr, .001);
        setProperty(widgetData, CabbageIdentifierIds::text, "");
        setProperty(widgetData, CabbageIdentifierIds::textbox, 0.f);
        setProperty(widgetData, CabbageIdentifierIds::caption, "");
        setProperty(widgetData, CabbageIdentifierIds::colour, Colours::whitesmoke.toString());
        setProperty(widgetData, CabbageIdentifierIds::trackercolour, Colour(0, 118, 38).toString());
        setProperty(widgetData, CabbageIdentifierIds::fontcolour, CabbageUtilities::getComponentFontColour().toString());
        setProperty(widgetData, CabbageIdentifierIds::textcolour, CabbageUtilities::getComponentFontColour().toString());
        setProperty(widgetData, CabbageIdentifierIds::outlinecolour, Colours::black.brighter(.3f).toString());
        setProperty(widgetData, CabbageIdentifierIds::midichan, -99);
        setProperty(widgetData, CabbageIdentifierIds::midictrl, -99);
        //these don't appear in the props dialog
        setProperty(widgetData, CabbageIdentifierIds::name, "rslider");
        setProperty(widgetData, CabbageIdentifierIds::type, getProperty(widgetData, "name").toString());
        setProperty(widgetData, CabbageIdentifierIds::name, getProperty(widgetData, "name").toString()+String(ID));
        setProperty(widgetData, CabbageIdentifierIds::kind, "rotary");
        setProperty(widgetData, CabbageIdentifierIds::decimalplaces, 1);
        setProperty(widgetData, CabbageIdentifierIds::velocity, 0);
        setProperty(widgetData, CabbageIdentifierIds::identchannel, "");
        setProperty(widgetData, CabbageIdentifierIds::trackerthickness, .7);
        setProperty(widgetData, CabbageIdentifierIds::visible, 1);
        setProperty(widgetData, CabbageIdentifierIds::svgslider, "");
        setProperty(widgetData, CabbageIdentifierIds::svgsliderbg, "");
    }

    else if((strTokens[0].trim() == "sourcebutton")||(strTokens[0].trim() == "loadbutton"))
    {
        top = 10;
        left = 10;
        width = 600;
        height = 440;
        //name.append(String(ID), 1024);
        setProperty(widgetData, CabbageIdentifierIds::top, 10);
        setProperty(widgetData, CabbageIdentifierIds::left, 10);
        setProperty(widgetData, CabbageIdentifierIds::width, 80);
        setProperty(widgetData, CabbageIdentifierIds::height, 40);
        setProperty(widgetData, CabbageIdentifierIds::text, "");
        setProperty(widgetData, CabbageIdentifierIds::caption, "");
        setProperty(widgetData, CabbageIdentifierIds::colour, Colours::black.toString());
        setProperty(widgetData, CabbageIdentifierIds::fontcolour, Colours::white.toString());
        setProperty(widgetData, CabbageIdentifierIds::oncolour, Colours::black.toString());
        setProperty(widgetData, CabbageIdentifierIds::onfontcolour, Colours::white.toString());
        setProperty(widgetData, CabbageIdentifierIds::type, strTokens[0].trim());
        setProperty(widgetData, CabbageIdentifierIds::name, strTokens[0].trim());
        setProperty(widgetData, CabbageIdentifierIds::name, getProperty(widgetData, "name").toString()+String(ID));
        setProperty(widgetData, CabbageIdentifierIds::identchannel, "");
        setProperty(widgetData, CabbageIdentifierIds::visible, 1);
        setProperty(widgetData, CabbageIdentifierIds::svgbuttonon, "");
        setProperty(widgetData, CabbageIdentifierIds::svgbuttonoff, "");
    }

    //===============info button==================//
    else if(strTokens[0].trim() == "infobutton")
    {
        setProperty(widgetData, "basetype", "layout");
        top = 10;
        left = 10;
        width = 600;
        height = 440;
        //name.append(String(ID), 1024);
        setProperty(widgetData, CabbageIdentifierIds::top, 10);
        setProperty(widgetData, CabbageIdentifierIds::left, 10);
        setProperty(widgetData, CabbageIdentifierIds::width, 80);
        setProperty(widgetData, CabbageIdentifierIds::height, 40);
        setProperty(widgetData, CabbageIdentifierIds::text, "");
        setProperty(widgetData, CabbageIdentifierIds::caption, "");
        setProperty(widgetData, CabbageIdentifierIds::colour, Colours::black.toString());
        setProperty(widgetData, CabbageIdentifierIds::fontcolour, Colours::white.toString());
        setProperty(widgetData, CabbageIdentifierIds::oncolour, Colours::black.toString());
        setProperty(widgetData, CabbageIdentifierIds::onfontcolour, Colours::white.toString());
        setProperty(widgetData, CabbageIdentifierIds::type, "infobutton");
        setProperty(widgetData, CabbageIdentifierIds::name, "infobutton");
        setProperty(widgetData, CabbageIdentifierIds::name, getProperty(widgetData, "name").toString()+String(ID));
        setProperty(widgetData, CabbageIdentifierIds::identchannel, "");
        setProperty(widgetData, CabbageIdentifierIds::visible, 1);
        setProperty(widgetData, CabbageIdentifierIds::svgbuttonon, "");
        setProperty(widgetData, CabbageIdentifierIds::svgbuttonoff, "");
    }


    //===============record button==================//
    else if(strTokens[0].trim() == "recordbutton")
    {
        setProperty(widgetData, "basetype", "layout");
        top = 10;
        left = 10;
        width = 80;
        height = 40;
        var array;
        array.append("Record");
        array.append("Record");


        setProperty(widgetData, CabbageIdentifierIds::top, 10);
        setProperty(widgetData, CabbageIdentifierIds::left, 10);
        setProperty(widgetData, CabbageIdentifierIds::width, 80);
        setProperty(widgetData, CabbageIdentifierIds::height, 40);
        var channels;
        channels.append("recordbutton");
        setProperty(widgetData, CabbageIdentifierIds::channel, channels);
        setProperty(widgetData, CabbageIdentifierIds::value, 1);
        setProperty(widgetData, CabbageIdentifierIds::text, array);
        setProperty(widgetData, CabbageIdentifierIds::caption, "");
        setProperty(widgetData, CabbageIdentifierIds::colour, Colours::black.toString());
        setProperty(widgetData, CabbageIdentifierIds::fontcolour, Colours::white.toString());
        setProperty(widgetData, CabbageIdentifierIds::oncolour, Colours::black.toString());
        setProperty(widgetData, CabbageIdentifierIds::onfontcolour, Colours::white.toString());
        setProperty(widgetData, CabbageIdentifierIds::type, "recordbutton");
        setProperty(widgetData, CabbageIdentifierIds::name, "recordbutton");
        setProperty(widgetData, CabbageIdentifierIds::mode, "file");
        setProperty(widgetData, CabbageIdentifierIds::identchannel, "");
        setProperty(widgetData, "name", getProperty(widgetData, "name").toString()+String(ID));
        var populate;
        populate.append("");
        populate.append("");
        setProperty(widgetData, "populate", populate);
        setProperty(widgetData, CabbageIdentifierIds::visible, 1);
        setProperty(widgetData, CabbageIdentifierIds::svgbuttonon, "");
        setProperty(widgetData, CabbageIdentifierIds::svgbuttonoff, "");
    }
    //===============file button==================//
    else if(strTokens[0].trim() == "filebutton")
    {
        setProperty(widgetData, "basetype", "layout");
        top = 10;
        left = 10;
        width = 80;
        height = 40;
        var array;
        array.append("Open file");
        array.append("Open file");


        setProperty(widgetData, CabbageIdentifierIds::top, 10);
        setProperty(widgetData, CabbageIdentifierIds::left, 10);
        setProperty(widgetData, CabbageIdentifierIds::width, 80);
        setProperty(widgetData, CabbageIdentifierIds::height, 40);
        var channels;
        channels.append("filebutton");
        setProperty(widgetData, CabbageIdentifierIds::channel, channels);
        setProperty(widgetData, CabbageIdentifierIds::value, 1);
        setProperty(widgetData, CabbageIdentifierIds::text, array);
        setProperty(widgetData, CabbageIdentifierIds::caption, "");
        setProperty(widgetData, CabbageIdentifierIds::colour, Colours::black.toString());
        setProperty(widgetData, CabbageIdentifierIds::fontcolour, Colours::white.toString());
        setProperty(widgetData, CabbageIdentifierIds::oncolour, Colours::black.toString());
        setProperty(widgetData, CabbageIdentifierIds::onfontcolour, Colours::white.toString());
        setProperty(widgetData, CabbageIdentifierIds::type, "filebutton");
        setProperty(widgetData, CabbageIdentifierIds::name, "filebutton");
        setProperty(widgetData, CabbageIdentifierIds::mode, "file");
        setProperty(widgetData, CabbageIdentifierIds::identchannel, "");
        setProperty(widgetData, "name", getProperty(widgetData, "name").toString()+String(ID));
        var populate;
        populate.append("");
        populate.append("");
        setProperty(widgetData, "populate", populate);
        setProperty(widgetData, CabbageIdentifierIds::visible, 1);
        setProperty(widgetData, CabbageIdentifierIds::filetype, "*");
        setProperty(widgetData, CabbageIdentifierIds::svgbuttonon, "");
        setProperty(widgetData, CabbageIdentifierIds::svgbuttonoff, "");
    }

    //===============button==================//
    else if(strTokens[0].trim() == "button")
    {
        setProperty(widgetData, "basetype", "interactive");
        top = 10;
        left = 10;
        width = 80;
        height = 40;
        var array;
        array.append("Push me");
        array.append("Push me");

        setProperty(widgetData, CabbageIdentifierIds::top, 10);
        setProperty(widgetData, CabbageIdentifierIds::left, 10);
        setProperty(widgetData, CabbageIdentifierIds::width, 80);
        setProperty(widgetData, CabbageIdentifierIds::height, 40);
        var channels;
        channels.append("buttonchan");
        setProperty(widgetData, CabbageIdentifierIds::channel, channels);
        setProperty(widgetData, CabbageIdentifierIds::value, 0);
        setProperty(widgetData, CabbageIdentifierIds::text, array);
        setProperty(widgetData, CabbageIdentifierIds::caption, "");
        setProperty(widgetData, CabbageIdentifierIds::colour, Colours::black.toString());
        setProperty(widgetData, CabbageIdentifierIds::fontcolour, Colours::white.toString());
        setProperty(widgetData, CabbageIdentifierIds::oncolour, Colours::black.toString());
        setProperty(widgetData, CabbageIdentifierIds::onfontcolour, Colours::white.toString());
        setProperty(widgetData, CabbageIdentifierIds::latched, 1);
        setProperty(widgetData, CabbageIdentifierIds::type, "button");
        setProperty(widgetData, CabbageIdentifierIds::name, "button");
        setProperty(widgetData, CabbageIdentifierIds::name, getProperty(widgetData, "name").toString()+String(ID));
        setProperty(widgetData, CabbageIdentifierIds::identchannel, "");
        setProperty(widgetData, CabbageIdentifierIds::radiogroup, 0);
        setProperty(widgetData, CabbageIdentifierIds::visible, 1);
        setProperty(widgetData, CabbageIdentifierIds::svgbuttonon, "");
        setProperty(widgetData, CabbageIdentifierIds::svgbuttonoff, "");

    }

    //===============checkbox==================//
    else if(strTokens[0].trim() == "checkbox")
    {
		setCheckBoxProperties(widgetData, ID);
    }

    //===============numberbox==================//
    else if(strTokens[0].trim() == "numberbox")
    {
        setProperty(widgetData, "basetype", "interactive");
        setProperty(widgetData, CabbageIdentifierIds::top, 10);
        setProperty(widgetData, CabbageIdentifierIds::left, 10);
        setProperty(widgetData, CabbageIdentifierIds::width, 100);
        setProperty(widgetData, CabbageIdentifierIds::height, 22);
        var channels;
        channels.append("numberbox");
        setProperty(widgetData, CabbageIdentifierIds::channel, channels);
        setProperty(widgetData, CabbageIdentifierIds::min, 0);
        setProperty(widgetData, CabbageIdentifierIds::max, 100);
        setProperty(widgetData, CabbageIdentifierIds::value, 50);
        setProperty(widgetData, CabbageIdentifierIds::sliderskew, 1);
        setProperty(widgetData, CabbageIdentifierIds::sliderincr, .01);
        setProperty(widgetData, CabbageIdentifierIds::text, "");
        setProperty(widgetData, CabbageIdentifierIds::value, 0);
        setProperty(widgetData, CabbageIdentifierIds::caption, "");
        setProperty(widgetData, CabbageIdentifierIds::type, "numberbox");
        setProperty(widgetData, CabbageIdentifierIds::colour, CabbageUtilities::getDarkerBackgroundSkin().toString());
        setProperty(widgetData, CabbageIdentifierIds::fontcolour, Colours::whitesmoke.toString());
        setProperty(widgetData, CabbageIdentifierIds::textcolour, CabbageUtilities::getComponentFontColour().toString());
        setProperty(widgetData, CabbageIdentifierIds::name, "numberbox");
        setProperty(widgetData, CabbageIdentifierIds::name, getProperty(widgetData, "name").toString()+String(ID));
        setProperty(widgetData, CabbageIdentifierIds::identchannel, "");
        setProperty(widgetData, CabbageIdentifierIds::visible, 1);
        setProperty(widgetData, CabbageIdentifierIds::velocity, 80);
    }
    //===============popupmenu==================//
    else if(strTokens[0].trim() == "popupmenu")
    {
        setProperty(widgetData, "basetype", "layout");

        var array;
        array.append("Item 1");
        array.append("Item 2");
        array.append("Item 3");
        array.append("Item 4");

        setProperty(widgetData, CabbageIdentifierIds::top, 10);
        setProperty(widgetData, CabbageIdentifierIds::left, 10);
        setProperty(widgetData, CabbageIdentifierIds::width, 80);
        setProperty(widgetData, CabbageIdentifierIds::height, 20);
        var channels;
        channels.append("popupchan");
        setProperty(widgetData, CabbageIdentifierIds::channel, channels);
        setProperty(widgetData, CabbageIdentifierIds::channeltype, "number");
        setProperty(widgetData, CabbageIdentifierIds::text, array);
        setProperty(widgetData, CabbageIdentifierIds::value, 1);

        setProperty(widgetData, CabbageIdentifierIds::colour, Colours::black.toString());
        setProperty(widgetData, CabbageIdentifierIds::fontcolour, CabbageUtilities::getComponentFontColour().toString());
        setProperty(widgetData, CabbageIdentifierIds::type, "popupmenu");
        setProperty(widgetData, CabbageIdentifierIds::name, "popupmenu");

        var populate;
        populate.append("");
        populate.append("");
        setProperty(widgetData, CabbageIdentifierIds::populate, populate);
        setProperty(widgetData, CabbageIdentifierIds::name, getProperty(widgetData, "name").toString()+String(ID));
        setProperty(widgetData, CabbageIdentifierIds::identchannel, "");
        setProperty(widgetData, CabbageIdentifierIds::visible, 1);
    }
    //===============combobox==================//
    else if(strTokens[0].trim() == "combobox")
    {
		setComboBoxProperties(widgetData, ID);
    }

    //===============label==================//
    else if(strTokens[0].trim() == "label")
    {
        setProperty(widgetData, "basetype", "layout");
        setProperty(widgetData, CabbageIdentifierIds::top, 10);
        setProperty(widgetData, CabbageIdentifierIds::left, 10);
        setProperty(widgetData, CabbageIdentifierIds::width, 80);
        setProperty(widgetData, CabbageIdentifierIds::height, 16);
        setProperty(widgetData, CabbageIdentifierIds::text, "hello");
        setProperty(widgetData, CabbageIdentifierIds::min, 1);
        setProperty(widgetData, CabbageIdentifierIds::name, "label");
        setProperty(widgetData, CabbageIdentifierIds::type, "label");
        setProperty(widgetData, CabbageIdentifierIds::type, getProperty(widgetData, "name").toString());
        setProperty(widgetData, CabbageIdentifierIds::name, getProperty(widgetData, "name").toString()+String(ID));
        setProperty(widgetData, CabbageIdentifierIds::colour, Colours::transparentBlack.toString());
        setProperty(widgetData, CabbageIdentifierIds::fontcolour, CabbageUtilities::getComponentFontColour().toString());
        setProperty(widgetData, CabbageIdentifierIds::align, "centre");
        setProperty(widgetData, CabbageIdentifierIds::fontstyle, 1);
        setProperty(widgetData, CabbageIdentifierIds::channel, "");
        setProperty(widgetData, CabbageIdentifierIds::identchannel, "");
        setProperty(widgetData, CabbageIdentifierIds::corners, 3);
        setProperty(widgetData, CabbageIdentifierIds::visible, 1);
    }

    //===============soundfiler==================//
    else if(strTokens[0].trim() == "soundfiler")
    {
        setProperty(widgetData, "basetype", "layout");
        top = 10;
        left = 10;
        width = 300;
        height = 200;

        setProperty(widgetData, CabbageIdentifierIds::top, 10);
        setProperty(widgetData, CabbageIdentifierIds::left, 10);
        setProperty(widgetData, CabbageIdentifierIds::width, 300);
        setProperty(widgetData, CabbageIdentifierIds::height, 200);
        var channels;
        //channels.append("pos");
        //channels.append("end");
        setProperty(widgetData, CabbageIdentifierIds::channel, channels);
        setProperty(widgetData, CabbageIdentifierIds::colour, CabbageUtilities::getComponentFontColour().toString());
        setProperty(widgetData, CabbageIdentifierIds::fontcolour, CabbageUtilities::getComponentFontColour().toString());
        setProperty(widgetData, CabbageIdentifierIds::type, "soundfiler");
        setProperty(widgetData, CabbageIdentifierIds::name, "soundfiler");
        setProperty(widgetData, CabbageIdentifierIds::file, "");
        setProperty(widgetData, CabbageIdentifierIds::name, getProperty(widgetData, "name").toString()+String(ID));
        setProperty(widgetData, CabbageIdentifierIds::identchannel, "");
        setProperty(widgetData, CabbageIdentifierIds::visible, 1);
        setProperty(widgetData, CabbageIdentifierIds::scrubberposition, 0);
        setProperty(widgetData, CabbageIdentifierIds::zoom, 0);
        setProperty(widgetData, CabbageIdentifierIds::startpos, 0);
        setProperty(widgetData, CabbageIdentifierIds::tablenumber, -1);
    }
    //===============gentable==================//
    else if(strTokens[0].trim() == "gentable")
    {
        setProperty(widgetData, "basetype", "layout");
        top = 10;
        left = 10;
        width = 300;
        height = 200;

        var tableColours;
        tableColours.append("white");
        tableColours.append("cornflowerblue");
        tableColours.append("yellow");
        tableColours.append("lime");
        tableColours.append("green");
        tableColours.append("pink");
        tableColours.append("brown");
        tableColours.append("purple");
        tableColours.append("lightblue");
        tableColours.append("darkgreen");
        tableColours.append("lightgreen");
        tableColours.append("mango");
        setProperty(widgetData, CabbageIdentifierIds::top, 10);
        setProperty(widgetData, CabbageIdentifierIds::left, 10);
        setProperty(widgetData, CabbageIdentifierIds::width, 300);
        setProperty(widgetData, CabbageIdentifierIds::height, 200);
        var channels;
        channels.append("pos");
        channels.append("end");
        setProperty(widgetData, CabbageIdentifierIds::channel, channels);
        setProperty(widgetData, CabbageIdentifierIds::colour, CabbageUtilities::getComponentFontColour().toString());
        setProperty(widgetData, CabbageIdentifierIds::fontcolour, CabbageUtilities::getComponentFontColour().toString());
        setProperty(widgetData, CabbageIdentifierIds::type, "gentable");
        setProperty(widgetData, CabbageIdentifierIds::tablecolour, tableColours);
        setProperty(widgetData, CabbageIdentifierIds::name, "gentable");
        setProperty(widgetData, CabbageIdentifierIds::amprange, 0);
        setProperty(widgetData, CabbageIdentifierIds::drawmode, "");
        setProperty(widgetData, CabbageIdentifierIds::file, "");
        setProperty(widgetData, CabbageIdentifierIds::name, getProperty(widgetData, "name").toString()+String(ID));
        setProperty(widgetData, CabbageIdentifierIds::identchannel, "");
        setProperty(widgetData, CabbageIdentifierIds::visible, 1);
        setProperty(widgetData, CabbageIdentifierIds::active, 0);

        var tables;
        tables.append(0);
        tables.append(0);

        setProperty(widgetData, CabbageIdentifierIds::scrubberposition, tables);
        setProperty(widgetData, CabbageIdentifierIds::zoom, -1);
        setProperty(widgetData, CabbageIdentifierIds::startpos, 0);
        setProperty(widgetData, CabbageIdentifierIds::tablenumber, -1);
        setProperty(widgetData, CabbageIdentifierIds::outlinethickness, 1.f);
        setProperty(widgetData, CabbageIdentifierIds::tablebackgroundcolour, Colour(15, 15, 15).toString());
        setProperty(widgetData, CabbageIdentifierIds::tablegridcolour, Colour(45, 45, 45).toString());
    }

    //===============image==================//
    else if(strTokens[0].trim() == "image")
    {
        setProperty(widgetData, "basetype", "layout");
        top = 10;
        left = 10;
        width = 160;
        height = 120;


        setProperty(widgetData, CabbageIdentifierIds::top, 10);
        setProperty(widgetData, CabbageIdentifierIds::left, 10);
        setProperty(widgetData, CabbageIdentifierIds::width, 160);
        setProperty(widgetData, CabbageIdentifierIds::height, 120);
        setProperty(widgetData, CabbageIdentifierIds::shape, "rounded");
        setProperty(widgetData, CabbageIdentifierIds::corners, 5);
        setProperty(widgetData, CabbageIdentifierIds::colour, Colours::white.toString());
        setProperty(widgetData, CabbageIdentifierIds::outlinecolour, Colours::white.toString());
        setProperty(widgetData, CabbageIdentifierIds::outlinethickness, 0);
        setProperty(widgetData, CabbageIdentifierIds::popup, 0);
        setProperty(widgetData, CabbageIdentifierIds::plant, "");
        setProperty(widgetData, CabbageIdentifierIds::type, "image");
        setProperty(widgetData, CabbageIdentifierIds::file, "");
        setProperty(widgetData, CabbageIdentifierIds::name, "image");
        setProperty(widgetData, CabbageIdentifierIds::name, getProperty(widgetData, "name").toString()+String(ID));
        setProperty(widgetData, CabbageIdentifierIds::identchannel, "");
        setProperty(widgetData, CabbageIdentifierIds::visible, 1);
    }

    //===============image==================//
    else if(strTokens[0].trim() == "encoder")
    {
        setProperty(widgetData, CabbageIdentifierIds::basetype, "interactive");
        setProperty(widgetData, CabbageIdentifierIds::top, 10);
        setProperty(widgetData, CabbageIdentifierIds::left, 10);
        setProperty(widgetData, CabbageIdentifierIds::width, 60);
        setProperty(widgetData, CabbageIdentifierIds::height, 60);
        var channels;
        channels.append("encoder");
        setProperty(widgetData, CabbageIdentifierIds::channel, channels);
        setProperty(widgetData, CabbageIdentifierIds::value, 0);
        setProperty(widgetData, CabbageIdentifierIds::sliderincr, .001);
        setProperty(widgetData, CabbageIdentifierIds::text, "");
        setProperty(widgetData, CabbageIdentifierIds::textbox, 0.f);
        setProperty(widgetData, CabbageIdentifierIds::caption, "");
        setProperty(widgetData, CabbageIdentifierIds::colour, Colours::whitesmoke.toString());
        setProperty(widgetData, CabbageIdentifierIds::trackercolour, Colour(0, 118, 38).toString());
        setProperty(widgetData, CabbageIdentifierIds::fontcolour, CabbageUtilities::getComponentFontColour().toString());
        setProperty(widgetData, CabbageIdentifierIds::textcolour, CabbageUtilities::getComponentFontColour().toString());
        setProperty(widgetData, CabbageIdentifierIds::outlinecolour, Colours::black.brighter(.3f).toString());
        setProperty(widgetData, CabbageIdentifierIds::midichan, -99);
        setProperty(widgetData, CabbageIdentifierIds::midictrl, -99);
        //these don't appear in the props dialog
        setProperty(widgetData, CabbageIdentifierIds::name, "encoder");
        setProperty(widgetData, CabbageIdentifierIds::type, getProperty(widgetData, "name").toString());
        setProperty(widgetData, CabbageIdentifierIds::name, getProperty(widgetData, "name").toString()+String(ID));
        setProperty(widgetData, CabbageIdentifierIds::decimalplaces, 1);
        setProperty(widgetData, CabbageIdentifierIds::identchannel, "");
        setProperty(widgetData, CabbageIdentifierIds::trackerthickness, .05);
        setProperty(widgetData, CabbageIdentifierIds::visible, 1);
        setProperty(widgetData, CabbageIdentifierIds::minenabled, 0);
        setProperty(widgetData, CabbageIdentifierIds::maxenabled, 0);
    }

    //===============groupbox==================//
    else if(strTokens[0].trim() == "groupbox")
    {
        setProperty(widgetData, CabbageIdentifierIds::basetype, "layout");
        setProperty(widgetData, CabbageIdentifierIds::top, 10);
        setProperty(widgetData, CabbageIdentifierIds::left, 10);
        setProperty(widgetData, CabbageIdentifierIds::width, 80);
        setProperty(widgetData, CabbageIdentifierIds::height, 22);
        setProperty(widgetData, CabbageIdentifierIds::corners, 5);
        setProperty(widgetData, CabbageIdentifierIds::colour, Colour(35, 35, 35).toString());
        setProperty(widgetData, CabbageIdentifierIds::fontcolour, CabbageUtilities::getComponentFontColour().toString());
        setProperty(widgetData, CabbageIdentifierIds::popup, 0);
        setProperty(widgetData, CabbageIdentifierIds::plant, "");
        setProperty(widgetData, CabbageIdentifierIds::child, 0);
        setProperty(widgetData, CabbageIdentifierIds::linethickness, 1);
        setProperty(widgetData, CabbageIdentifierIds::type, "groupbox");
        setProperty(widgetData, CabbageIdentifierIds::name, "groupbox");
        setProperty(widgetData, CabbageIdentifierIds::name, getProperty(widgetData, "name").toString()+String(ID));
        setProperty(widgetData, CabbageIdentifierIds::identchannel, "");
        setProperty(widgetData, CabbageIdentifierIds::visible, 1);
        setProperty(widgetData, CabbageIdentifierIds::svggroupbox, "");

    }
    //===============line==================//
    else if(strTokens[0].trim() == "line")
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
        setProperty(widgetData, CabbageIdentifierIds::type, "line");
        setProperty(widgetData, CabbageIdentifierIds::name, "line");
        setProperty(widgetData, CabbageIdentifierIds::name, getProperty(widgetData, "name").toString()+String(ID));
        setProperty(widgetData, CabbageIdentifierIds::identchannel, "");
        setProperty(widgetData, CabbageIdentifierIds::visible, 1);
    }

    //===============csoundoutput==================//
    else if(strTokens[0].trim() == "csoundoutput")
    {
        setProperty(widgetData, "basetype", "layout");
        top = 10;
        left = 10;
        width = 400;
        height = 200;
        setProperty(widgetData, CabbageIdentifierIds::top, 10);
        setProperty(widgetData, CabbageIdentifierIds::left, 10);
        setProperty(widgetData, CabbageIdentifierIds::width, 400);
        setProperty(widgetData, CabbageIdentifierIds::text, "Csound output");
        setProperty(widgetData, CabbageIdentifierIds::height, 200);
        setProperty(widgetData, CabbageIdentifierIds::colour, Colours::black.toString());
        setProperty(widgetData, CabbageIdentifierIds::fontcolour, Colours::cornflowerblue.toString());
        setProperty(widgetData, CabbageIdentifierIds::name, "csoundoutput");
        setProperty(widgetData, CabbageIdentifierIds::wrap, 0);

        setProperty(widgetData, CabbageIdentifierIds::type, "csoundoutput");
        setProperty(widgetData, CabbageIdentifierIds::name, getProperty(widgetData, "name").toString()+String(ID));
        setProperty(widgetData, CabbageIdentifierIds::identchannel, "");
        setProperty(widgetData, CabbageIdentifierIds::visible, 1);
    }
    //===============textbox==================//
    else if(strTokens[0].trim() == "textbox")
    {
        setProperty(widgetData, "basetype", "layout");
        top = 10;
        left = 10;
        width = 400;
        height = 200;
        setProperty(widgetData, CabbageIdentifierIds::top, 10);
        setProperty(widgetData, CabbageIdentifierIds::left, 10);
        setProperty(widgetData, CabbageIdentifierIds::width, 400);
        setProperty(widgetData, CabbageIdentifierIds::text, "");
        setProperty(widgetData, CabbageIdentifierIds::height, 200);
        setProperty(widgetData, CabbageIdentifierIds::colour, Colours::black.toString());
        setProperty(widgetData, CabbageIdentifierIds::fontcolour, Colours::white.toString());
        setProperty(widgetData, CabbageIdentifierIds::name, "textbox");
        setProperty(widgetData, CabbageIdentifierIds::type, "textbox");
        setProperty(widgetData, CabbageIdentifierIds::wrap, 0);
        setProperty(widgetData, CabbageIdentifierIds::file, "");
        setProperty(widgetData, CabbageIdentifierIds::name, getProperty(widgetData, "name").toString()+String(ID));
        setProperty(widgetData, CabbageIdentifierIds::identchannel, "");
        setProperty(widgetData, CabbageIdentifierIds::visible, 1);
    }

    //===============textbox==================//
    else if(strTokens[0].trim() == "texteditor")
    {
        setProperty(widgetData, "basetype", "layout");
        top = 10;
        left = 10;
        width = 400;
        height = 200;
        setProperty(widgetData, CabbageIdentifierIds::top, 10);
        setProperty(widgetData, CabbageIdentifierIds::left, 10);
        setProperty(widgetData, CabbageIdentifierIds::width, 400);
        setProperty(widgetData, CabbageIdentifierIds::text, "");
        setProperty(widgetData, CabbageIdentifierIds::height, 200);
        setProperty(widgetData, CabbageIdentifierIds::channel, "texteditor");
        setProperty(widgetData, CabbageIdentifierIds::colour, Colours::white.toString());
        setProperty(widgetData, CabbageIdentifierIds::fontcolour, Colours::black.toString());
        setProperty(widgetData, CabbageIdentifierIds::name, "texteditor");
        setProperty(widgetData, CabbageIdentifierIds::type, "texteditor");
        setProperty(widgetData, CabbageIdentifierIds::name, getProperty(widgetData, "name").toString()+String(ID));
        setProperty(widgetData, CabbageIdentifierIds::identchannel, "");
        setProperty(widgetData, CabbageIdentifierIds::visible, 1);
    }

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

    //===============xypad==================//
    else if(strTokens[0].trim() == "xypad")
    {
        setProperty(widgetData, "basetype", "interactive");
        top = 10;
        left = 10;
        width = 200;
        height = 200;

        var channels;
        channels.append("xChan");
        channels.append("yChan");
        setProperty(widgetData, CabbageIdentifierIds::xyautoindex, 0);
        setProperty(widgetData, CabbageIdentifierIds::xchannel, channels[0]);
        setProperty(widgetData, CabbageIdentifierIds::ychannel, channels[1]);
        setProperty(widgetData, CabbageIdentifierIds::top, 10);
        setProperty(widgetData, CabbageIdentifierIds::left, 10);
        setProperty(widgetData, CabbageIdentifierIds::caption, "");
        setProperty(widgetData, CabbageIdentifierIds::width, 200);
        setProperty(widgetData, CabbageIdentifierIds::height, 200);
        setProperty(widgetData, CabbageIdentifierIds::minx, 0);
        setProperty(widgetData, CabbageIdentifierIds::maxx, 200);
        setProperty(widgetData, CabbageIdentifierIds::text, "");
        setProperty(widgetData, CabbageIdentifierIds::miny, 0);
        setProperty(widgetData, CabbageIdentifierIds::maxy, 200);
        setProperty(widgetData, CabbageIdentifierIds::valuex, 0);
        setProperty(widgetData, CabbageIdentifierIds::valuey, 0);
        setProperty(widgetData, CabbageIdentifierIds::colour, Colours::lime.toString());
        setProperty(widgetData, CabbageIdentifierIds::fontcolour, Colours::cornflowerblue.toString());
        setProperty(widgetData, CabbageIdentifierIds::textcolour, Colours::cornflowerblue.toString());
        setProperty(widgetData, CabbageIdentifierIds::type, "xypad");
        setProperty(widgetData, CabbageIdentifierIds::name, "xypad");
        setProperty(widgetData, CabbageIdentifierIds::name, getProperty(widgetData, "name").toString()+String(ID));
        setProperty(widgetData, CabbageIdentifierIds::identchannel, "");
        setProperty(widgetData, CabbageIdentifierIds::visible, 1);
    }
    //===============keyboard==================//
    else if(strTokens[0].trim() == "keyboard")
    {
        setProperty(widgetData, "basetype", "layout");
        top = 10;
        left = 10;
        width = 400;
        height = 100;

        setProperty(widgetData, CabbageIdentifierIds::top, 10);
        setProperty(widgetData, CabbageIdentifierIds::left, 10);
        setProperty(widgetData, CabbageIdentifierIds::width, 400);
        setProperty(widgetData, CabbageIdentifierIds::height, 100);
        setProperty(widgetData, CabbageIdentifierIds::value, 60);
        setProperty(widgetData, CabbageIdentifierIds::middlec, 3);
        setProperty(widgetData, CabbageIdentifierIds::type, "keyboard");
        setProperty(widgetData, CabbageIdentifierIds::name, "keyboard");
        setProperty(widgetData, CabbageIdentifierIds::name, getProperty(widgetData, "name").toString()+String(ID));
        setProperty(widgetData, CabbageIdentifierIds::visible, 1);
        setProperty(widgetData, CabbageIdentifierIds::keywidth, 16);
        setProperty(widgetData, CabbageIdentifierIds::scrollbars, 1);

    }
    //===============form==================//
    else if(strTokens[0].trim() == "form")
    {
        setProperty(widgetData, CabbageIdentifierIds::basetype, "layout");
        setProperty(widgetData, CabbageIdentifierIds::top, 10);
        setProperty(widgetData, CabbageIdentifierIds::left, 10);
        setProperty(widgetData, CabbageIdentifierIds::width, 600);
        setProperty(widgetData, CabbageIdentifierIds::height, 300);
        setProperty(widgetData, CabbageIdentifierIds::caption, "");
        setProperty(widgetData, CabbageIdentifierIds::name, "form");
        setProperty(widgetData, CabbageIdentifierIds::type, "form");
        setProperty(widgetData, CabbageIdentifierIds::guirefresh, 100);
        setProperty(widgetData, CabbageIdentifierIds::identchannel, "");
        setProperty(widgetData, CabbageIdentifierIds::visible, 1);
        setProperty(widgetData, CabbageIdentifierIds::scrollbars, 1);
        setProperty(widgetData, CabbageIdentifierIds::titlebarcolour, "");
        setProperty(widgetData, CabbageIdentifierIds::fontcolour, "");
    }

    //===============rangeslider===============//
    else if(strTokens[0].trim() == "hrange" || strTokens[0].trim() == "vrange")
    {
        setProperty(widgetData, CabbageIdentifierIds::basetype, "interactive");
        setProperty(widgetData, CabbageIdentifierIds::top, 10);
        setProperty(widgetData, CabbageIdentifierIds::left, 10);
        setProperty(widgetData, CabbageIdentifierIds::width, 50);
        setProperty(widgetData, CabbageIdentifierIds::height, 150);
        var channels;
        channels.append("rangeslider");
        setProperty(widgetData, CabbageIdentifierIds::channel, channels);
        setProperty(widgetData, CabbageIdentifierIds::min, 0);
        setProperty(widgetData, CabbageIdentifierIds::max, 1);
        setProperty(widgetData, CabbageIdentifierIds::value, 0);
        setProperty(widgetData, CabbageIdentifierIds::text, "");
        setProperty(widgetData, CabbageIdentifierIds::textbox, 0.f);
        setProperty(widgetData, CabbageIdentifierIds::caption, "");
        setProperty(widgetData, CabbageIdentifierIds::colour, Colours::white.toString());
        setProperty(widgetData, CabbageIdentifierIds::trackercolour, Colour(0, 118, 38).toString());
        setProperty(widgetData, CabbageIdentifierIds::fontcolour, CabbageUtilities::getComponentFontColour().toString());
        setProperty(widgetData, CabbageIdentifierIds::textcolour, CabbageUtilities::getComponentFontColour().toString());
        setProperty(widgetData, CabbageIdentifierIds::sliderskew, 1);
        setProperty(widgetData, CabbageIdentifierIds::sliderincr, .001);
        setProperty(widgetData, CabbageIdentifierIds::midichan, -99);
        setProperty(widgetData, CabbageIdentifierIds::midictrl, -99);
        //these don't appear in the props dialog
        setProperty(widgetData, CabbageIdentifierIds::name, strTokens[0].trim());
        setProperty(widgetData, CabbageIdentifierIds::type, getProperty(widgetData, "name").toString());
        setProperty(widgetData, CabbageIdentifierIds::name, getProperty(widgetData, "name").toString()+String(ID));
        if(strTokens[0].trim() == "hrange")
            setProperty(widgetData, CabbageIdentifierIds::kind, "horizontal");
        else
            setProperty(widgetData, CabbageIdentifierIds::kind, "vertical");

        setProperty(widgetData, CabbageIdentifierIds::decimalplaces, 1);
        setProperty(widgetData, CabbageIdentifierIds::trackerthickness, .1);
        setProperty(widgetData, CabbageIdentifierIds::identchannel, "");
        setProperty(widgetData, CabbageIdentifierIds::visible, 1);
        setProperty(widgetData, CabbageIdentifierIds::svgslider, "");
        setProperty(widgetData, CabbageIdentifierIds::svgsliderbg, "");
    }

    //===============signaldisplay==================//
    else if(strTokens[0].trim() == "signaldisplay" || strTokens[0].trim() == "fftdisplay")
    {
        setProperty(widgetData, "basetype", "layout");
        top = 10;
        left = 10;
        width = 180;
        height = 2;

        var signalVariables;
        signalVariables.append("");

        setProperty(widgetData, CabbageIdentifierIds::top, 10);
        setProperty(widgetData, CabbageIdentifierIds::left, 10);
        setProperty(widgetData, CabbageIdentifierIds::width, 260);
        setProperty(widgetData, CabbageIdentifierIds::height, 100);
        setProperty(widgetData, CabbageIdentifierIds::tablecolour, Colours::lime.toString());
        setProperty(widgetData, CabbageIdentifierIds::fontcolour, Colours::white.toString());
        setProperty(widgetData, CabbageIdentifierIds::tablebackgroundcolour, Colour(30, 30, 38).toString());
        setProperty(widgetData, CabbageIdentifierIds::type, "signaldisplay");
        setProperty(widgetData, CabbageIdentifierIds::name, "signaldisplay");
        setProperty(widgetData, CabbageIdentifierIds::min, 0);
        setProperty(widgetData, CabbageIdentifierIds::max, 2048);
        setProperty(widgetData, CabbageIdentifierIds::updaterate, 100);
        setProperty(widgetData, CabbageIdentifierIds::signalvariable, signalVariables);
        setProperty(widgetData, CabbageIdentifierIds::outlinethickness, 1);
        setProperty(widgetData, CabbageIdentifierIds::name, getProperty(widgetData, "name").toString()+String(ID));
        setProperty(widgetData, CabbageIdentifierIds::identchannel, "");
        setProperty(widgetData, CabbageIdentifierIds::displaytype, "spectroscope");
        setProperty(widgetData, CabbageIdentifierIds::zoom, 0);
        setProperty(widgetData, CabbageIdentifierIds::visible, 1);
    }

    //===============stepper==================//
    else if(strTokens[0].trim() == "scope")
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
        setProperty(widgetData, CabbageIdentifierIds::type, "scope");
        setProperty(widgetData, CabbageIdentifierIds::name, "scope");
        setProperty(widgetData, CabbageIdentifierIds::name, getProperty(widgetData, "name").toString()+String(ID));
        setProperty(widgetData, CabbageIdentifierIds::identchannel, "");
        setProperty(widgetData, CabbageIdentifierIds::visible, 1);
    }

    //===============stepper==================//
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
void CabbageWidgetData::setComboBoxProperties(ValueTree widgetData, int ID)
{
	setProperty(widgetData, "basetype", "interactive");
	var array;
	array.append("Item 1");
	array.append("Item 2");
	array.append("Item 3");
	array.append("Item 4");
	setProperty(widgetData, CabbageIdentifierIds::top, 10);
	setProperty(widgetData, CabbageIdentifierIds::left, 10);
	setProperty(widgetData, CabbageIdentifierIds::width, 80);
	setProperty(widgetData, CabbageIdentifierIds::height, 20);
	var channels;
	channels.append("combochan");
	setProperty(widgetData, CabbageIdentifierIds::channel, channels);
	setProperty(widgetData, CabbageIdentifierIds::channeltype, "number");
	setProperty(widgetData, CabbageIdentifierIds::text, array);
	setProperty(widgetData, CabbageIdentifierIds::value, 1);
	setProperty(widgetData, CabbageIdentifierIds::currenttext, "");
	setProperty(widgetData, CabbageIdentifierIds::caption, "");
	setProperty(widgetData, CabbageIdentifierIds::colour, Colours::black.toString());
	setProperty(widgetData, CabbageIdentifierIds::fontcolour, CabbageUtilities::getComponentFontColour().toString());
	setProperty(widgetData, CabbageIdentifierIds::menucolour, Colours::whitesmoke.toString());
	setProperty(widgetData, CabbageIdentifierIds::type, "combobox");
	setProperty(widgetData, CabbageIdentifierIds::name, "combobox");
	setProperty(widgetData, CabbageIdentifierIds::comborange, 4);
	setProperty(widgetData, CabbageIdentifierIds::file, "");
	var populate;
	populate.append("");
	populate.append("");
	setProperty(widgetData, CabbageIdentifierIds::populate, populate);
	setProperty(widgetData, CabbageIdentifierIds::name, getProperty(widgetData, "name").toString()+String(ID));
	setProperty(widgetData, CabbageIdentifierIds::identchannel, "");
	setProperty(widgetData, CabbageIdentifierIds::visible, 1);
	setProperty(widgetData, CabbageIdentifierIds::workingdir, "");
}
		
void CabbageWidgetData::setCheckBoxProperties(ValueTree widgetData, int ID)
{
	setProperty(widgetData, "basetype", "interactive");
	setProperty(widgetData, CabbageIdentifierIds::top, 10);
	setProperty(widgetData, CabbageIdentifierIds::left, 10);
	setProperty(widgetData, CabbageIdentifierIds::width, 100);
	setProperty(widgetData, CabbageIdentifierIds::height, 22);
	var channels;
	channels.append("checkchan");
	setProperty(widgetData, CabbageIdentifierIds::channel, channels);
	setProperty(widgetData, CabbageIdentifierIds::text, "");
	setProperty(widgetData, CabbageIdentifierIds::value, 0);
	setProperty(widgetData, CabbageIdentifierIds::caption, "");
	setProperty(widgetData, CabbageIdentifierIds::shape, "square");
	setProperty(widgetData, CabbageIdentifierIds::corners, 2);
	setProperty(widgetData, CabbageIdentifierIds::type, "checkbox");
	setProperty(widgetData, CabbageIdentifierIds::oncolour, Colours::lime.toString());
	setProperty(widgetData, CabbageIdentifierIds::colour, Colours::black.toString());
	setProperty(widgetData, CabbageIdentifierIds::onfontcolour, CabbageUtilities::getComponentFontColour().toString());
	setProperty(widgetData, CabbageIdentifierIds::fontcolour, CabbageUtilities::getComponentFontColour().toString());
	setProperty(widgetData, CabbageIdentifierIds::name, "checkbox");
	setProperty(widgetData, CabbageIdentifierIds::name, getProperty(widgetData, "name").toString()+String(ID));
	setProperty(widgetData, CabbageIdentifierIds::identchannel, "");
	setProperty(widgetData, CabbageIdentifierIds::radiogroup, 0);
	setProperty(widgetData, CabbageIdentifierIds::visible, 1);		
}
//===========================================================================================
// this method parsing the Cabbage text and set each of the Cabbage indentifers
//===========================================================================================
void CabbageWidgetData::setCustomWidgetState(ValueTree widgetData, String inStr, String identifier)
{
    //Logger::writeToLog(str);
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
                if(str.containsIgnoreCase("xypad"))
                {
                    setProperty(widgetData, CabbageIdentifierIds::xchannel, strTokens[0].trim());
                    setProperty(widgetData, CabbageIdentifierIds::ychannel, strTokens[1].trim());
                }
                else if(str.containsIgnoreCase("vumeter"))
                {
                    array.resize(0);
                    for(int u=0; u<strTokens.size(); u++)
                    {
                        array.append(strTokens[u].trim());
                    }
                }
                else
                {
                    for(int i=1; i<strTokens.size(); i++)
                    {
                        array.append(strTokens[i].trim());
                    }
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
                    identArray[indx].equalsIgnoreCase("colours("))
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

            else if(identArray[indx].equalsIgnoreCase("svgpath"))
            {
                setProperty(widgetData, CabbageIdentifierIds::svgpath, strTokens[0].trim());
            }

            else if(identArray[indx].equalsIgnoreCase("svgfile"))
            {
                if(typeOfWidget=="groupbox")
                    setProperty(widgetData, CabbageIdentifierIds::svggroupbox, strTokens[0].trim());
                else if(typeOfWidget.contains("slider"))
                {
                    if(strTokens[0].trim().contains("background"))
                        setProperty(widgetData, CabbageIdentifierIds::svgsliderbg, strTokens[1].trim());
                    else
                        setProperty(widgetData, CabbageIdentifierIds::svgslider, strTokens[1].trim());
                }
                else if(typeOfWidget.contains("button"))
                {
                    if(strTokens[0].trim().contains("off"))
                        setProperty(widgetData, CabbageIdentifierIds::svgbuttonoff, strTokens[1].trim());
                    else
                        setProperty(widgetData, CabbageIdentifierIds::svgbuttonon, strTokens[1].trim());
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
                            setProperty(widgetData, CabbageIdentifierIds::value, strTokens[2].trim().getDoubleValue() < min ? min : strTokens[2].trim().getDoubleValue());
                            tempArray.add(strTokens[2].trim());
                        }
                    }


                    if(strTokens.size()>3)
                    {
                        setProperty(widgetData, CabbageIdentifierIds::sliderskew, strTokens[3].trim().getDoubleValue());
                        tempArray.add(strTokens[3].trim());
                    }

                    if(strTokens.size()>4)
                    {
                        tempArray.add(strTokens[4].trim());
                        setProperty(widgetData, CabbageIdentifierIds::sliderincr, strTokens[4].trim().getDoubleValue());
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
                    warningMessages+="Not enough paramters passed to range(): usage range(minx, max, value\")\n";
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
                    warningMessages+="Not enough paramters passed to range(): usage range(minx, max, value\")\n";
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

            else if(identArray[indx].equalsIgnoreCase("svgdebug"))
            {
                setProperty(widgetData, CabbageIdentifierIds::svgdebug, strTokens[0].trim().getFloatValue());
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

            else if(identArray[indx].equalsIgnoreCase("textbox"))
            {
                setProperty(widgetData, CabbageIdentifierIds::textbox, strTokens[0].trim().getFloatValue());
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
                if(strTokens.size()>1)
                    setProperty(widgetData, CabbageIdentifierIds::startpos, strTokens[1].trim().getFloatValue());
                else
                    setProperty(widgetData, CabbageIdentifierIds::startpos, -1);
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
//                var value;
//                var tableConfig;
//                //value.append(tableNum);
//                //tableNumbers.add(tableNum);
//
//                for(int i=0; i<strTokens.size(); i++)
//                {
//                    if(strTokens[i].contains(":") && strTokens.size()>0)
//                    {
//                        //split string into two
//                        StringArray tablesData;
//                        tablesData.addTokens(strTokens[i], ":", "");
//                        var tables;
//                        for(int w=0; w<tablesData.size(); w++)
//                        {
//                            tables.append(tablesData[w]);
//                            value.append(tablesData[w]);
//                            //Logger::writeToLog(tablesData[w]);
//
//                        }
//                        //value.append(tables);
//                        tableConfig.append(tables);
//                    }
//                    else
//                    {
//                        tableNumbers.add(strTokens[i].trim().getFloatValue());
//                        value.append(strTokens[i].trim().getFloatValue());
//                        tableConfig.append(strTokens[i].trim().getFloatValue());
//                        //Logger::writeToLog(strTokens[i].trim());
//                    }
//                }

//                setProperty(widgetData, CabbageIdentifierIds::tableconfig, tableConfig);
//                setProperty(widgetData, CabbageIdentifierIds::tablenumber, value);
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


String CabbageWidgetData::getStringProp(ValueTree widgetData, Identifier prop, int index)
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

void CabbageWidgetData::setProperty(ValueTree widgetData, Identifier name, const var &value)
{		
	widgetData.setProperty(name, value, 0);		
}

var CabbageWidgetData::getWidgetPropertyWithDefault(ValueTree widgetData, Identifier name, const var &value)
{
	return widgetData.getProperty(name, value);
}	

var CabbageWidgetData::getProperty(ValueTree widgetData, Identifier name)
{
	return widgetData.getProperty(name);
}

ValueTree CabbageWidgetData::getValueTreeForComponent(ValueTree widgetData, String name)
{
	for(int i=0;i<widgetData.getNumChildren();i++)
	{
		if(name==widgetData.getChild(i).getProperty(CabbageIdentifierIds::name).toString())
		{
			return widgetData.getChild(i);
		}
	}
	 
	return ValueTree(""); 
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

    //Logger::writeToLog(text);
    //Logger::writeToLog(colour.toString());
    return colour;
}
//==================================================================
String CabbageWidgetData::getTextFromText(String text)
{
    text = " "+text;
    String subString = text.substring(text.indexOfWholeWord("text"));
    subString = subString.substring(5, subString.indexOf(")"));
    return subString;
}
//==================================================================
var CabbageWidgetData::getVarArrayFromText(String text)
{
    text = " "+text.replace("\"", "");
    String subString = text.substring(text.indexOfWholeWord("text"));
    subString = subString.substring(5, subString.indexOf(")"));
    StringArray array;
    var varArray;
    array.addTokens(subString, ",", "");
    for(int i=0; i<array.size(); i++)
        varArray.append(array[i].trim());
    return varArray;
}
//==================================================================
Point<int> CabbageWidgetData::getPosFromText(String text)
{
    text = " "+text;
    String subString = text.substring(text.indexOfWholeWord("pos"));
    subString = subString.substring(4, subString.indexOf(")"));
    StringArray strTokens;
    strTokens.addTokens(subString, ",()", "");
    return Point<int>(strTokens[0].getIntValue(), strTokens[1].getIntValue());
}
//==================================================================
Point<int> CabbageWidgetData::getSizeFromText(String text)
{
    text = " "+text;
    String subString = text.substring(text.indexOfWholeWord("size"));
    subString = subString.substring(5, subString.indexOf(")"));
    StringArray strTokens;
    strTokens.addTokens(subString, ",()", "");
    return Point<int>(strTokens[0].getIntValue(), strTokens[1].getIntValue());
}
//==================================================================
float CabbageWidgetData::getSkewFromText(String text)
{
    text = " "+text;
    String subString = text.substring(text.indexOfWholeWord("sliderskew"));
    subString = subString.substring(11, subString.indexOf(")"));
    return subString.getFloatValue();
}


//===================================================================
float CabbageWidgetData::getTableChannelValues(int index)
{
		jassert(false);
//    if(index<tableChannelValues.size())
//        return tableChannelValues.getReference(index);
//    else
//        return 0.f;
}
//===================================================================
void CabbageWidgetData::addTableChannelValues()
{
		jassert(false);
//    tableChannelValues.add(-1.f);
}
//===================================================================
void CabbageWidgetData::setTableChannelValues(int index, float val)
{
		jassert(false);
//    if(index<tableChannelValues.size())
//        tableChannelValues.set(index, val);
}


//===================================================================
void CabbageWidgetData::scaleWidget(Point<float> scale)
{
		jassert(false);
//    Rectangle<int> rect(getWidgetPropertyWithDefault("left", 0),
//                        getWidgetPropertyWithDefault("top", 0),
//                        getWidgetPropertyWithDefault("width", 10),
//                        getWidgetPropertyWithDefault("height", 10));
//
//    setProperty(CabbageIdentifierIds::left, rect.getX()*scale.x);
//    setProperty(CabbageIdentifierIds::top, rect.getY()*scale.y);
//    setProperty(CabbageIdentifierIds::width, rect.getWidth()*scale.x);
//    setProperty(CabbageIdentifierIds::height, rect.getHeight()*scale.y);	;
}

//===================================================================
String CabbageWidgetData::getStringForIdentifier(var propsArray, String identifier, String type)
{
    String str;
    if(type=="number")
    {
        if(propsArray.size()>0)
        {
            for(int p=0; p<propsArray.size(); p++)
                str += propsArray[p].toString()+",";
            str= identifier+str.dropLastCharacters(1)+"), ";
            //Logger::writeToLog(str);
        }
        else
            str = identifier+propsArray.toString()+"), ";

        return str;
    }

    else if(type=="string")
    {
        if(propsArray.size()>0)
        {
            for(int p=0; p<propsArray.size(); p++)
                str += "\""+propsArray[p].toString()+"\",";
            str = identifier+str.dropLastCharacters(1)+"), ";
            //Logger::writeToLog(str);
        }
        else
            str = identifier+String("\"")+propsArray.toString()+"\"), ";

        return str;
    }
    else
        return "";
}
//===================================================================
String CabbageWidgetData::getBoundsText(Rectangle<int> rect)
{
	const String boundsText = "bounds(" + String(rect.getX()) + ", " + String(rect.getY()) + ", " + String(rect.getWidth()) + ", " + String(rect.getHeight()) + "), ";
	return boundsText;
}

//===================================================================
String CabbageWidgetData::getValueText(ValueTree widgetData, String identifier)
{
	ValueTree tempData("tempTree");
	const String type = CabbageWidgetData::getStringProp(widgetData, CabbageIdentifierIds::type);
	CabbageWidgetData::setWidgetState(tempData, type, -99);
	
	if(CabbageWidgetData::getNumProp(widgetData, CabbageIdentifierIds::value)!=CabbageWidgetData::getNumProp(tempData, CabbageIdentifierIds::value))
	{
		return "value(" + String(CabbageWidgetData::getNumProp(widgetData, CabbageIdentifierIds::value)) + "), ";
	}
	
	return String::empty;
}

//===================================================================
String CabbageWidgetData::getMultiItemText(ValueTree widgetData, String identifier)
{
	var items = CabbageWidgetData::getProperty(widgetData, identifier);
	CabbageUtilities::debug(items.toString());
	String channelString = "";

	const Array<var>* array = items.getArray();
	if(array)
	{
		for(int i = 0 ; i < array->size()-1 ; i++)
		{
			CabbageUtilities::debug(array->getReference(i).toString());
			const String text = array->getReference(array->size()-1).toString();
			channelString = channelString+ "\"" + array->getReference(i).toString() + "\", ";
		}
		
		CabbageUtilities::debug(channelString);
		
		if( array->size()==1)
			return identifier+"(\""+channelString+array->getReference(array->size()-1).toString()+"\"), ";
		else
			return identifier+"("+channelString+ "\"" + array->getReference(array->size()-1).toString()+"\"), ";
	}	
 
 
	StringArray stringArray;
	stringArray.addLines(items.toString());
	for( int i = 0 ; i < stringArray.size(); i++)
		stringArray.set(i, "\"" + stringArray[i] + "\"");
	CabbageUtilities::debug(stringArray.joinIntoString(", "));
 
	return identifier+"("+stringArray.joinIntoString(", ")+"), ";
}

//===================================================================
String CabbageWidgetData::getColoursText(ValueTree widgetData)
{
	ValueTree tempData("tempTree");
	//tempData = widgetData.createCopy();
	const String type = CabbageWidgetData::getStringProp(widgetData, CabbageIdentifierIds::type);
	CabbageWidgetData::setWidgetState(tempData, type, -99);
	String colourString;
	
	if(CabbageWidgetData::getStringProp(widgetData, CabbageIdentifierIds::colour)!=CabbageWidgetData::getStringProp(tempData, CabbageIdentifierIds::colour))
	{
		const Colour col = Colour::fromString(CabbageWidgetData::getStringProp(widgetData, CabbageIdentifierIds::colour));
		colourString = colourString << "colour:0(" << (float)col.getRed() << ", " << (float)col.getGreen() << ", " << (float)col.getBlue() << ", " << (float)col.getAlpha() << "), ";
	}
	
	if(CabbageWidgetData::getStringProp(widgetData, CabbageIdentifierIds::oncolour)!=CabbageWidgetData::getStringProp(tempData, CabbageIdentifierIds::oncolour))
	{
		const Colour col = Colour::fromString(CabbageWidgetData::getStringProp(widgetData, CabbageIdentifierIds::oncolour));
		colourString = colourString << "colour:1(" << (float)col.getRed() << ", " << (float)col.getGreen() << ", " << (float)col.getBlue() << ", " << (float)col.getAlpha() << "), ";		
	}

	if(CabbageWidgetData::getStringProp(widgetData, CabbageIdentifierIds::fontcolour)!=CabbageWidgetData::getStringProp(tempData, CabbageIdentifierIds::fontcolour))
	{
		const Colour col = Colour::fromString(CabbageWidgetData::getStringProp(widgetData, CabbageIdentifierIds::fontcolour));
		colourString = colourString << "fontcolour:0(" << (float)col.getRed() << ", " << (float)col.getGreen() << ", " << (float)col.getBlue() << ", " << (float)col.getAlpha() << "), ";		
	}
	
	if(CabbageWidgetData::getStringProp(widgetData, CabbageIdentifierIds::onfontcolour)!=CabbageWidgetData::getStringProp(tempData, CabbageIdentifierIds::onfontcolour))
	{
		const Colour col = Colour::fromString(CabbageWidgetData::getStringProp(widgetData, CabbageIdentifierIds::onfontcolour));
		colourString = colourString << "fontcolour:1(" << (float)col.getRed() << ", " << (float)col.getGreen() << ", " << (float)col.getBlue() << ", " << (float)col.getAlpha() << "), ";		
	}
	
	return colourString;
}
//===================================================================
String CabbageWidgetData::getCabbageCodeFromIdentifiers(ValueTree widgetData)
{
	//CabbageUtilities::debug(listOfChannels.joinIntoString("\n"));
	String cabbageCode = CabbageWidgetData::getStringProp(widgetData, CabbageIdentifierIds::type) + " " 
						 + CabbageWidgetData::getBoundsText(CabbageWidgetData::getBounds(widgetData)) 
						 + getMultiItemText(widgetData, "channel")
						 + getValueText(widgetData, "value")
						 + getMultiItemText(widgetData, "text")
						 + getColoursText(widgetData);
	return cabbageCode;
}
