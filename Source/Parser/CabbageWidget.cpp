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


#include "CabbageWidget.h"
//===============================================================================
// Main Cabbage abstract GUI class
//===============================================================================
CabbageWidget::CabbageWidget(String compStr, int ID):
    width(0),
    height(0),
    left(0),
    top(0),
    refreshFromDisk(false),
    warningMessages(""),
	ValueTree("CabbageWidgetTree")
{
//Default values are assigned to all attributres
//before parsing begins
    setWidgetProperty("scalex", 1);
    setWidgetProperty("scaley", 1);
    setWidgetProperty("resize", 0);
    setWidgetProperty(CabbageIdentifierIds::active, 1);
    setWidgetProperty(CabbageIdentifierIds::parentdir, "");
    setWidgetProperty(CabbageIdentifierIds::svgdebug, 0);

    StringArray strTokens;
    strTokens.addTokens(compStr, " ", "\"");

    setWidgetProperty(CabbageIdentifierIds::channelarray, "");
    //===============sliders==================//
    if(strTokens[0].trim() == "hslider" || strTokens[0].trim() == "hslider2" || strTokens[0].trim() == "hslider3")
    {
        setWidgetProperty(CabbageIdentifierIds::basetype, "interactive");
        setWidgetProperty(CabbageIdentifierIds::top, 10);
        setWidgetProperty(CabbageIdentifierIds::left, 10);
        setWidgetProperty(CabbageIdentifierIds::width, 150);
        setWidgetProperty(CabbageIdentifierIds::height, 50);
        var channels;
        channels.append("hslider");
        setWidgetProperty(CabbageIdentifierIds::channel, channels);
        setWidgetProperty(CabbageIdentifierIds::min, 0);
        setWidgetProperty(CabbageIdentifierIds::max, 1);
        setWidgetProperty(CabbageIdentifierIds::value, 0);
        setWidgetProperty(CabbageIdentifierIds::text, "");
        setWidgetProperty(CabbageIdentifierIds::textbox, 0.f);
        setWidgetProperty(CabbageIdentifierIds::caption, "");
        setWidgetProperty(CabbageIdentifierIds::colour, Colours::white.toString());
        setWidgetProperty(CabbageIdentifierIds::trackercolour, Colour(0, 118, 38).toString());
        setWidgetProperty(CabbageIdentifierIds::fontcolour, CabbageUtilities::getComponentFontColour().toString());
        setWidgetProperty(CabbageIdentifierIds::textcolour, CabbageUtilities::getComponentFontColour().toString());
        setWidgetProperty(CabbageIdentifierIds::sliderskew, 1);
        setWidgetProperty(CabbageIdentifierIds::sliderincr, .001);
        setWidgetProperty(CabbageIdentifierIds::velocity, 0);

        setWidgetProperty(CabbageIdentifierIds::midichan, -99);
        setWidgetProperty(CabbageIdentifierIds::midictrl, -99);
        //these don't appear in the props dialog
        setWidgetProperty(CabbageIdentifierIds::name, "hslider");
        setWidgetProperty(CabbageIdentifierIds::type, getWidgetPropertyWithDefault("name", "").toString());
        setWidgetProperty(CabbageIdentifierIds::name, getWidgetPropertyWithDefault("name", "").toString()+String(ID));
        if(strTokens[0].trim() == "hslider2")
            setWidgetProperty(CabbageIdentifierIds::kind, "horizontal2");
        else if(strTokens[0].trim() == "hslider3")
            setWidgetProperty(CabbageIdentifierIds::kind, "horizontal3");
        else
            setWidgetProperty(CabbageIdentifierIds::kind, "horizontal");
        setWidgetProperty(CabbageIdentifierIds::decimalplaces, 1);
        setWidgetProperty(CabbageIdentifierIds::identchannel, "");
        setWidgetProperty(CabbageIdentifierIds::trackerthickness, .1);
        setWidgetProperty(CabbageIdentifierIds::visible, 1);
        setWidgetProperty(CabbageIdentifierIds::gradient, 1);
        setWidgetProperty(CabbageIdentifierIds::svgslider, "");
        setWidgetProperty(CabbageIdentifierIds::svgsliderbg, "");


    }
    else if(strTokens[0].trim() == "socketsend" || strTokens[0].trim() == "socketreceive")
    {
        setWidgetProperty(CabbageIdentifierIds::basetype, "layout");
        setWidgetProperty(CabbageIdentifierIds::socketaddress, "");
        setWidgetProperty(CabbageIdentifierIds::socketport, 0);
    }
    else if(strTokens[0].trim() == "vslider" || strTokens[0].trim() == "vslider2" ||  strTokens[0].trim() == "vslider3")
    {
        setWidgetProperty(CabbageIdentifierIds::basetype, "interactive");
        setWidgetProperty(CabbageIdentifierIds::top, 10);
        setWidgetProperty(CabbageIdentifierIds::left, 10);
        setWidgetProperty(CabbageIdentifierIds::width, 50);
        setWidgetProperty(CabbageIdentifierIds::height, 150);
        var channels;
        channels.append("vslider");
        setWidgetProperty(CabbageIdentifierIds::channel, channels);
        setWidgetProperty(CabbageIdentifierIds::min, 0);
        setWidgetProperty(CabbageIdentifierIds::max, 1);
        setWidgetProperty(CabbageIdentifierIds::value, 0);
        setWidgetProperty(CabbageIdentifierIds::text, "");
        setWidgetProperty(CabbageIdentifierIds::textbox, 0.f);
        setWidgetProperty(CabbageIdentifierIds::caption, "");
        setWidgetProperty(CabbageIdentifierIds::colour, Colours::white.toString());
        setWidgetProperty(CabbageIdentifierIds::trackercolour, Colour(0, 118, 38).toString());
        setWidgetProperty(CabbageIdentifierIds::fontcolour, CabbageUtilities::getComponentFontColour().toString());
        setWidgetProperty(CabbageIdentifierIds::textcolour, CabbageUtilities::getComponentFontColour().toString());
        setWidgetProperty(CabbageIdentifierIds::sliderskew, 1);
        setWidgetProperty(CabbageIdentifierIds::sliderincr, .001);
        setWidgetProperty(CabbageIdentifierIds::velocity, 0);
        setWidgetProperty(CabbageIdentifierIds::midichan, -99);
        setWidgetProperty(CabbageIdentifierIds::midictrl, -99);
        //these don't appear in the props dialog
        setWidgetProperty(CabbageIdentifierIds::name, "vslider");
        setWidgetProperty(CabbageIdentifierIds::type, getWidgetPropertyWithDefault("name", "").toString());
        setWidgetProperty(CabbageIdentifierIds::name, getWidgetPropertyWithDefault("name", "").toString()+String(ID));
        if(strTokens[0].trim() == "vslider2")
            setWidgetProperty(CabbageIdentifierIds::kind, "vertical2");
        else if(strTokens[0].trim() == "vslider3")
            setWidgetProperty(CabbageIdentifierIds::kind, "vertical3");
        else
            setWidgetProperty(CabbageIdentifierIds::kind, "vertical");

        setWidgetProperty(CabbageIdentifierIds::decimalplaces, 1);
        setWidgetProperty(CabbageIdentifierIds::trackerthickness, .1);
        setWidgetProperty(CabbageIdentifierIds::identchannel, "");
        setWidgetProperty(CabbageIdentifierIds::visible, 1);
        setWidgetProperty(CabbageIdentifierIds::svgslider, "");
        setWidgetProperty(CabbageIdentifierIds::svgsliderbg, "");
    }
    else if(strTokens[0].trim() == "rslider")
    {
        setWidgetProperty(CabbageIdentifierIds::basetype, "interactive");
        setWidgetProperty(CabbageIdentifierIds::top, 10);
        setWidgetProperty(CabbageIdentifierIds::left, 10);
        setWidgetProperty(CabbageIdentifierIds::width, 60);
        setWidgetProperty(CabbageIdentifierIds::height, 60);
        var channels;
        channels.append("rslider");
        setWidgetProperty(CabbageIdentifierIds::channel, channels);
        setWidgetProperty(CabbageIdentifierIds::min, 0);
        setWidgetProperty(CabbageIdentifierIds::max, 1);
        setWidgetProperty(CabbageIdentifierIds::value, 0);
        setWidgetProperty(CabbageIdentifierIds::sliderskew, 1);
        setWidgetProperty(CabbageIdentifierIds::sliderincr, .001);
        setWidgetProperty(CabbageIdentifierIds::text, "");
        setWidgetProperty(CabbageIdentifierIds::textbox, 0.f);
        setWidgetProperty(CabbageIdentifierIds::caption, "");
        setWidgetProperty(CabbageIdentifierIds::colour, Colours::whitesmoke.toString());
        setWidgetProperty(CabbageIdentifierIds::trackercolour, Colour(0, 118, 38).toString());
        setWidgetProperty(CabbageIdentifierIds::fontcolour, CabbageUtilities::getComponentFontColour().toString());
        setWidgetProperty(CabbageIdentifierIds::textcolour, CabbageUtilities::getComponentFontColour().toString());
        setWidgetProperty(CabbageIdentifierIds::outlinecolour, Colours::black.brighter(.3f).toString());
        setWidgetProperty(CabbageIdentifierIds::midichan, -99);
        setWidgetProperty(CabbageIdentifierIds::midictrl, -99);
        //these don't appear in the props dialog
        setWidgetProperty(CabbageIdentifierIds::name, "rslider");
        setWidgetProperty(CabbageIdentifierIds::type, getWidgetPropertyWithDefault("name", "").toString());
        setWidgetProperty(CabbageIdentifierIds::name, getWidgetPropertyWithDefault("name", "").toString()+String(ID));
        setWidgetProperty(CabbageIdentifierIds::kind, "rotary");
        setWidgetProperty(CabbageIdentifierIds::decimalplaces, 1);
        setWidgetProperty(CabbageIdentifierIds::velocity, 0);
        setWidgetProperty(CabbageIdentifierIds::identchannel, "");
        setWidgetProperty(CabbageIdentifierIds::trackerthickness, .7);
        setWidgetProperty(CabbageIdentifierIds::visible, 1);
        setWidgetProperty(CabbageIdentifierIds::svgslider, "");
        setWidgetProperty(CabbageIdentifierIds::svgsliderbg, "");
    }

    else if((strTokens[0].trim() == "sourcebutton")||(strTokens[0].trim() == "loadbutton"))
    {
        top = 10;
        left = 10;
        width = 600;
        height = 440;
        //name.append(String(ID), 1024);
        setWidgetProperty(CabbageIdentifierIds::top, 10);
        setWidgetProperty(CabbageIdentifierIds::left, 10);
        setWidgetProperty(CabbageIdentifierIds::width, 80);
        setWidgetProperty(CabbageIdentifierIds::height, 40);
        setWidgetProperty(CabbageIdentifierIds::text, "");
        setWidgetProperty(CabbageIdentifierIds::caption, "");
        setWidgetProperty(CabbageIdentifierIds::colour, Colours::black.toString());
        setWidgetProperty(CabbageIdentifierIds::fontcolour, Colours::white.toString());
        setWidgetProperty(CabbageIdentifierIds::oncolour, Colours::black.toString());
        setWidgetProperty(CabbageIdentifierIds::onfontcolour, Colours::white.toString());
        setWidgetProperty(CabbageIdentifierIds::type, strTokens[0].trim());
        setWidgetProperty(CabbageIdentifierIds::name, strTokens[0].trim());
        setWidgetProperty(CabbageIdentifierIds::name, getWidgetPropertyWithDefault("name", "").toString()+String(ID));
        setWidgetProperty(CabbageIdentifierIds::identchannel, "");
        setWidgetProperty(CabbageIdentifierIds::visible, 1);
        setWidgetProperty(CabbageIdentifierIds::svgbuttonon, "");
        setWidgetProperty(CabbageIdentifierIds::svgbuttonoff, "");
    }

    //===============info button==================//
    else if(strTokens[0].trim() == "infobutton")
    {
        setWidgetProperty("basetype", "layout");
        top = 10;
        left = 10;
        width = 600;
        height = 440;
        //name.append(String(ID), 1024);
        setWidgetProperty(CabbageIdentifierIds::top, 10);
        setWidgetProperty(CabbageIdentifierIds::left, 10);
        setWidgetProperty(CabbageIdentifierIds::width, 80);
        setWidgetProperty(CabbageIdentifierIds::height, 40);
        setWidgetProperty(CabbageIdentifierIds::text, "");
        setWidgetProperty(CabbageIdentifierIds::caption, "");
        setWidgetProperty(CabbageIdentifierIds::colour, Colours::black.toString());
        setWidgetProperty(CabbageIdentifierIds::fontcolour, Colours::white.toString());
        setWidgetProperty(CabbageIdentifierIds::oncolour, Colours::black.toString());
        setWidgetProperty(CabbageIdentifierIds::onfontcolour, Colours::white.toString());
        setWidgetProperty(CabbageIdentifierIds::type, "infobutton");
        setWidgetProperty(CabbageIdentifierIds::name, "infobutton");
        setWidgetProperty(CabbageIdentifierIds::name, getWidgetPropertyWithDefault("name", "").toString()+String(ID));
        setWidgetProperty(CabbageIdentifierIds::identchannel, "");
        setWidgetProperty(CabbageIdentifierIds::visible, 1);
        setWidgetProperty(CabbageIdentifierIds::svgbuttonon, "");
        setWidgetProperty(CabbageIdentifierIds::svgbuttonoff, "");
    }


    //===============record button==================//
    else if(strTokens[0].trim() == "recordbutton")
    {
        setWidgetProperty("basetype", "layout");
        top = 10;
        left = 10;
        width = 80;
        height = 40;
        var array;
        array.append("Record");
        array.append("Record");


        setWidgetProperty(CabbageIdentifierIds::top, 10);
        setWidgetProperty(CabbageIdentifierIds::left, 10);
        setWidgetProperty(CabbageIdentifierIds::width, 80);
        setWidgetProperty(CabbageIdentifierIds::height, 40);
        var channels;
        channels.append("recordbutton");
        setWidgetProperty(CabbageIdentifierIds::channel, channels);
        setWidgetProperty(CabbageIdentifierIds::value, 1);
        setWidgetProperty(CabbageIdentifierIds::text, array);
        setWidgetProperty(CabbageIdentifierIds::caption, "");
        setWidgetProperty(CabbageIdentifierIds::colour, Colours::black.toString());
        setWidgetProperty(CabbageIdentifierIds::fontcolour, Colours::white.toString());
        setWidgetProperty(CabbageIdentifierIds::oncolour, Colours::black.toString());
        setWidgetProperty(CabbageIdentifierIds::onfontcolour, Colours::white.toString());
        setWidgetProperty(CabbageIdentifierIds::type, "recordbutton");
        setWidgetProperty(CabbageIdentifierIds::name, "recordbutton");
        setWidgetProperty(CabbageIdentifierIds::mode, "file");
        setWidgetProperty(CabbageIdentifierIds::identchannel, "");
        setWidgetProperty("name", getWidgetPropertyWithDefault("name", "").toString()+String(ID));
        var populate;
        populate.append("");
        populate.append("");
        setWidgetProperty("populate", populate);
        setWidgetProperty(CabbageIdentifierIds::visible, 1);
        setWidgetProperty(CabbageIdentifierIds::svgbuttonon, "");
        setWidgetProperty(CabbageIdentifierIds::svgbuttonoff, "");
    }
    //===============file button==================//
    else if(strTokens[0].trim() == "filebutton")
    {
        setWidgetProperty("basetype", "layout");
        top = 10;
        left = 10;
        width = 80;
        height = 40;
        var array;
        array.append("Open file");
        array.append("Open file");


        setWidgetProperty(CabbageIdentifierIds::top, 10);
        setWidgetProperty(CabbageIdentifierIds::left, 10);
        setWidgetProperty(CabbageIdentifierIds::width, 80);
        setWidgetProperty(CabbageIdentifierIds::height, 40);
        var channels;
        channels.append("filebutton");
        setWidgetProperty(CabbageIdentifierIds::channel, channels);
        setWidgetProperty(CabbageIdentifierIds::value, 1);
        setWidgetProperty(CabbageIdentifierIds::text, array);
        setWidgetProperty(CabbageIdentifierIds::caption, "");
        setWidgetProperty(CabbageIdentifierIds::colour, Colours::black.toString());
        setWidgetProperty(CabbageIdentifierIds::fontcolour, Colours::white.toString());
        setWidgetProperty(CabbageIdentifierIds::oncolour, Colours::black.toString());
        setWidgetProperty(CabbageIdentifierIds::onfontcolour, Colours::white.toString());
        setWidgetProperty(CabbageIdentifierIds::type, "filebutton");
        setWidgetProperty(CabbageIdentifierIds::name, "filebutton");
        setWidgetProperty(CabbageIdentifierIds::mode, "file");
        setWidgetProperty(CabbageIdentifierIds::identchannel, "");
        setWidgetProperty("name", getWidgetPropertyWithDefault("name", "").toString()+String(ID));
        var populate;
        populate.append("");
        populate.append("");
        setWidgetProperty("populate", populate);
        setWidgetProperty(CabbageIdentifierIds::visible, 1);
        setWidgetProperty(CabbageIdentifierIds::filetype, "*");
        setWidgetProperty(CabbageIdentifierIds::svgbuttonon, "");
        setWidgetProperty(CabbageIdentifierIds::svgbuttonoff, "");
    }

    //===============button==================//
    else if(strTokens[0].trim() == "button")
    {
        setWidgetProperty("basetype", "interactive");
        top = 10;
        left = 10;
        width = 80;
        height = 40;
        var array;
        array.append("Push me");
        array.append("Push me");

        setWidgetProperty(CabbageIdentifierIds::top, 10);
        setWidgetProperty(CabbageIdentifierIds::left, 10);
        setWidgetProperty(CabbageIdentifierIds::width, 80);
        setWidgetProperty(CabbageIdentifierIds::height, 40);
        var channels;
        channels.append("buttonchan");
        setWidgetProperty(CabbageIdentifierIds::channel, channels);
        setWidgetProperty(CabbageIdentifierIds::value, 0);
        setWidgetProperty(CabbageIdentifierIds::text, array);
        setWidgetProperty(CabbageIdentifierIds::caption, "");
        setWidgetProperty(CabbageIdentifierIds::colour, Colours::black.toString());
        setWidgetProperty(CabbageIdentifierIds::fontcolour, Colours::white.toString());
        setWidgetProperty(CabbageIdentifierIds::oncolour, Colours::black.toString());
        setWidgetProperty(CabbageIdentifierIds::onfontcolour, Colours::white.toString());
        setWidgetProperty(CabbageIdentifierIds::latched, 1);
        setWidgetProperty(CabbageIdentifierIds::type, "button");
        setWidgetProperty(CabbageIdentifierIds::name, "button");
        setWidgetProperty(CabbageIdentifierIds::name, getWidgetPropertyWithDefault("name", "").toString()+String(ID));
        setWidgetProperty(CabbageIdentifierIds::identchannel, "");
        setWidgetProperty(CabbageIdentifierIds::radiogroup, 0);
        setWidgetProperty(CabbageIdentifierIds::visible, 1);
        setWidgetProperty(CabbageIdentifierIds::svgbuttonon, "");
        setWidgetProperty(CabbageIdentifierIds::svgbuttonoff, "");

    }

    //===============checkbox==================//
    else if(strTokens[0].trim() == "checkbox")
    {
        setWidgetProperty("basetype", "interactive");
        setWidgetProperty(CabbageIdentifierIds::top, 10);
        setWidgetProperty(CabbageIdentifierIds::left, 10);
        setWidgetProperty(CabbageIdentifierIds::width, 100);
        setWidgetProperty(CabbageIdentifierIds::height, 22);
        var channels;
        channels.append("checkchan");
        setWidgetProperty(CabbageIdentifierIds::channel, channels);
        setWidgetProperty(CabbageIdentifierIds::text, "");
        setWidgetProperty(CabbageIdentifierIds::value, 0);
        setWidgetProperty(CabbageIdentifierIds::caption, "");
        setWidgetProperty(CabbageIdentifierIds::shape, "square");
        setWidgetProperty(CabbageIdentifierIds::corners, 2);
        setWidgetProperty(CabbageIdentifierIds::type, "checkbox");
        setWidgetProperty(CabbageIdentifierIds::oncolour, Colours::lime.toString());
        setWidgetProperty(CabbageIdentifierIds::colour, Colours::black.toString());
        setWidgetProperty(CabbageIdentifierIds::fontcolour, CabbageUtilities::getComponentFontColour().toString());
        setWidgetProperty(CabbageIdentifierIds::name, "checkbox");
        setWidgetProperty(CabbageIdentifierIds::name, getWidgetPropertyWithDefault("name", "").toString()+String(ID));
        setWidgetProperty(CabbageIdentifierIds::identchannel, "");
        setWidgetProperty(CabbageIdentifierIds::radiogroup, 0);
        setWidgetProperty(CabbageIdentifierIds::visible, 1);
    }

    //===============numberbox==================//
    else if(strTokens[0].trim() == "numberbox")
    {
        setWidgetProperty("basetype", "interactive");
        setWidgetProperty(CabbageIdentifierIds::top, 10);
        setWidgetProperty(CabbageIdentifierIds::left, 10);
        setWidgetProperty(CabbageIdentifierIds::width, 100);
        setWidgetProperty(CabbageIdentifierIds::height, 22);
        var channels;
        channels.append("numberbox");
        setWidgetProperty(CabbageIdentifierIds::channel, channels);
        setWidgetProperty(CabbageIdentifierIds::min, 0);
        setWidgetProperty(CabbageIdentifierIds::max, 100);
        setWidgetProperty(CabbageIdentifierIds::value, 50);
        setWidgetProperty(CabbageIdentifierIds::sliderskew, 1);
        setWidgetProperty(CabbageIdentifierIds::sliderincr, .01);
        setWidgetProperty(CabbageIdentifierIds::text, "");
        setWidgetProperty(CabbageIdentifierIds::value, 0);
        setWidgetProperty(CabbageIdentifierIds::caption, "");
        setWidgetProperty(CabbageIdentifierIds::type, "numberbox");
        setWidgetProperty(CabbageIdentifierIds::colour, CabbageUtilities::getDarkerBackgroundSkin().toString());
        setWidgetProperty(CabbageIdentifierIds::fontcolour, Colours::whitesmoke.toString());
        setWidgetProperty(CabbageIdentifierIds::textcolour, CabbageUtilities::getComponentFontColour().toString());
        setWidgetProperty(CabbageIdentifierIds::name, "numberbox");
        setWidgetProperty(CabbageIdentifierIds::name, getWidgetPropertyWithDefault("name", "").toString()+String(ID));
        setWidgetProperty(CabbageIdentifierIds::identchannel, "");
        setWidgetProperty(CabbageIdentifierIds::visible, 1);
        setWidgetProperty(CabbageIdentifierIds::velocity, 80);
    }
    //===============popupmenu==================//
    else if(strTokens[0].trim() == "popupmenu")
    {
        setWidgetProperty("basetype", "layout");

        var array;
        array.append("Item 1");
        array.append("Item 2");
        array.append("Item 3");
        array.append("Item 4");

        setWidgetProperty(CabbageIdentifierIds::top, 10);
        setWidgetProperty(CabbageIdentifierIds::left, 10);
        setWidgetProperty(CabbageIdentifierIds::width, 80);
        setWidgetProperty(CabbageIdentifierIds::height, 20);
        var channels;
        channels.append("popupchan");
        setWidgetProperty(CabbageIdentifierIds::channel, channels);
        setWidgetProperty(CabbageIdentifierIds::channeltype, "number");
        setWidgetProperty(CabbageIdentifierIds::text, array);
        setWidgetProperty(CabbageIdentifierIds::value, 1);

        setWidgetProperty(CabbageIdentifierIds::colour, Colours::black.toString());
        setWidgetProperty(CabbageIdentifierIds::fontcolour, CabbageUtilities::getComponentFontColour().toString());
        setWidgetProperty(CabbageIdentifierIds::type, "popupmenu");
        setWidgetProperty(CabbageIdentifierIds::name, "popupmenu");

        var populate;
        populate.append("");
        populate.append("");
        setWidgetProperty(CabbageIdentifierIds::populate, populate);
        setWidgetProperty(CabbageIdentifierIds::name, getWidgetPropertyWithDefault("name", "").toString()+String(ID));
        setWidgetProperty(CabbageIdentifierIds::identchannel, "");
        setWidgetProperty(CabbageIdentifierIds::visible, 1);
    }
    //===============combobox==================//
    else if(strTokens[0].trim() == "combobox")
    {
        setWidgetProperty("basetype", "interactive");

        top = 10;
        left = 10;
        width = 80;
        height = 22;
        //add one item by befault, will be overwritten by users values
        var array;
        array.append("Item 1");
        array.append("Item 2");
        array.append("Item 3");
        array.append("Item 4");
        setWidgetProperty(CabbageIdentifierIds::top, 10);
        setWidgetProperty(CabbageIdentifierIds::left, 10);
        setWidgetProperty(CabbageIdentifierIds::width, 80);
        setWidgetProperty(CabbageIdentifierIds::height, 20);
        var channels;
        channels.append("combochan");
        setWidgetProperty(CabbageIdentifierIds::channel, channels);
        setWidgetProperty(CabbageIdentifierIds::channeltype, "number");
        setWidgetProperty(CabbageIdentifierIds::text, array);
        setWidgetProperty(CabbageIdentifierIds::value, 1);
        setWidgetProperty(CabbageIdentifierIds::currenttext, "");
        setWidgetProperty(CabbageIdentifierIds::caption, "");
        setWidgetProperty(CabbageIdentifierIds::colour, Colours::black.toString());
        setWidgetProperty(CabbageIdentifierIds::fontcolour, CabbageUtilities::getComponentFontColour().toString());
        setWidgetProperty(CabbageIdentifierIds::type, "combobox");
        setWidgetProperty(CabbageIdentifierIds::name, "combobox");
        setWidgetProperty(CabbageIdentifierIds::comborange, 4);
        setWidgetProperty(CabbageIdentifierIds::file, "");
        var populate;
        populate.append("");
        populate.append("");
        setWidgetProperty(CabbageIdentifierIds::populate, populate);
        setWidgetProperty(CabbageIdentifierIds::name, getWidgetPropertyWithDefault("name", "").toString()+String(ID));
        setWidgetProperty(CabbageIdentifierIds::identchannel, "");
        setWidgetProperty(CabbageIdentifierIds::visible, 1);
        setWidgetProperty(CabbageIdentifierIds::workingdir, "");

    }

    //===============combobox==================//
    else if(strTokens[0].trim() == "listbox")
    {
        setWidgetProperty("basetype", "layout");

        top = 10;
        left = 10;
        width = 80;
        height = 22;
        //add one item by befault, will be overwritten by users values
        var array;
        array.append("Item 1");
        array.append("Item 2");
        array.append("Item 3");
        array.append("Item 4");
        setWidgetProperty(CabbageIdentifierIds::top, 10);
        setWidgetProperty(CabbageIdentifierIds::left, 10);
        setWidgetProperty(CabbageIdentifierIds::width, 80);
        setWidgetProperty(CabbageIdentifierIds::height, 20);
        var channels;
        channels.append("listbox");
        setWidgetProperty(CabbageIdentifierIds::channel, channels);
        setWidgetProperty(CabbageIdentifierIds::channeltype, "number");
        setWidgetProperty(CabbageIdentifierIds::text, array);
        setWidgetProperty(CabbageIdentifierIds::value, 1);
        setWidgetProperty(CabbageIdentifierIds::caption, "");
        setWidgetProperty(CabbageIdentifierIds::colour, Colours::black.toString());
        setWidgetProperty(CabbageIdentifierIds::highlightcolour, Colours::black.contrasting().toString());
        setWidgetProperty(CabbageIdentifierIds::fontcolour, CabbageUtilities::getComponentFontColour().toString());
        setWidgetProperty(CabbageIdentifierIds::type, "listbox");
        setWidgetProperty(CabbageIdentifierIds::name, "listbox");
        setWidgetProperty(CabbageIdentifierIds::comborange, 4);
        setWidgetProperty(CabbageIdentifierIds::file, "");
        setWidgetProperty(CabbageIdentifierIds::align, "centre");
        var populate;
        populate.append("");
        populate.append("");
        setWidgetProperty(CabbageIdentifierIds::populate, populate);
        setWidgetProperty(CabbageIdentifierIds::name, getWidgetPropertyWithDefault("name", "").toString()+String(ID));
        setWidgetProperty(CabbageIdentifierIds::identchannel, "");
        setWidgetProperty(CabbageIdentifierIds::visible, 1);
        setWidgetProperty(CabbageIdentifierIds::workingdir, "");

    }
    //===============label==================//
    else if(strTokens[0].trim() == "label")
    {
        setWidgetProperty("basetype", "layout");
        setWidgetProperty(CabbageIdentifierIds::top, 10);
        setWidgetProperty(CabbageIdentifierIds::left, 10);
        setWidgetProperty(CabbageIdentifierIds::width, 80);
        setWidgetProperty(CabbageIdentifierIds::height, 16);
        setWidgetProperty(CabbageIdentifierIds::text, "hello");
        setWidgetProperty(CabbageIdentifierIds::min, 1);
        setWidgetProperty(CabbageIdentifierIds::name, "label");
        setWidgetProperty(CabbageIdentifierIds::type, "label");
        setWidgetProperty(CabbageIdentifierIds::type, getWidgetPropertyWithDefault("name", "").toString());
        setWidgetProperty(CabbageIdentifierIds::name, getWidgetPropertyWithDefault("name", "").toString()+String(ID));
        setWidgetProperty(CabbageIdentifierIds::colour, Colours::transparentBlack.toString());
        setWidgetProperty(CabbageIdentifierIds::fontcolour, CabbageUtilities::getComponentFontColour().toString());
        setWidgetProperty(CabbageIdentifierIds::align, "centre");
        setWidgetProperty(CabbageIdentifierIds::fontstyle, 1);
        setWidgetProperty(CabbageIdentifierIds::channel, "");
        setWidgetProperty(CabbageIdentifierIds::identchannel, "");
        setWidgetProperty(CabbageIdentifierIds::corners, 3);
        setWidgetProperty(CabbageIdentifierIds::visible, 1);
    }

    //===============soundfiler==================//
    else if(strTokens[0].trim() == "soundfiler")
    {
        setWidgetProperty("basetype", "layout");
        top = 10;
        left = 10;
        width = 300;
        height = 200;

        setWidgetProperty(CabbageIdentifierIds::top, 10);
        setWidgetProperty(CabbageIdentifierIds::left, 10);
        setWidgetProperty(CabbageIdentifierIds::width, 300);
        setWidgetProperty(CabbageIdentifierIds::height, 200);
        var channels;
        //channels.append("pos");
        //channels.append("end");
        setWidgetProperty(CabbageIdentifierIds::channel, channels);
        setWidgetProperty(CabbageIdentifierIds::colour, CabbageUtilities::getComponentFontColour().toString());
        setWidgetProperty(CabbageIdentifierIds::fontcolour, CabbageUtilities::getComponentFontColour().toString());
        setWidgetProperty(CabbageIdentifierIds::type, "soundfiler");
        setWidgetProperty(CabbageIdentifierIds::name, "soundfiler");
        setWidgetProperty(CabbageIdentifierIds::file, "");
        setWidgetProperty(CabbageIdentifierIds::name, getWidgetPropertyWithDefault("name", "").toString()+String(ID));
        setWidgetProperty(CabbageIdentifierIds::identchannel, "");
        setWidgetProperty(CabbageIdentifierIds::visible, 1);
        setWidgetProperty(CabbageIdentifierIds::scrubberposition, 0);
        setWidgetProperty(CabbageIdentifierIds::zoom, 0);
        setWidgetProperty(CabbageIdentifierIds::startpos, 0);
        setWidgetProperty(CabbageIdentifierIds::tablenumber, -1);
    }
    //===============gentable==================//
    else if(strTokens[0].trim() == "gentable")
    {
        setWidgetProperty("basetype", "layout");
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
        setWidgetProperty(CabbageIdentifierIds::top, 10);
        setWidgetProperty(CabbageIdentifierIds::left, 10);
        setWidgetProperty(CabbageIdentifierIds::width, 300);
        setWidgetProperty(CabbageIdentifierIds::height, 200);
        var channels;
        channels.append("pos");
        channels.append("end");
        setWidgetProperty(CabbageIdentifierIds::channel, channels);
        setWidgetProperty(CabbageIdentifierIds::colour, CabbageUtilities::getComponentFontColour().toString());
        setWidgetProperty(CabbageIdentifierIds::fontcolour, CabbageUtilities::getComponentFontColour().toString());
        setWidgetProperty(CabbageIdentifierIds::type, "gentable");
        setWidgetProperty(CabbageIdentifierIds::tablecolour, tableColours);
        setWidgetProperty(CabbageIdentifierIds::name, "gentable");
        setWidgetProperty(CabbageIdentifierIds::amprange, 0);
        setWidgetProperty(CabbageIdentifierIds::drawmode, "");
        setWidgetProperty(CabbageIdentifierIds::file, "");
        setWidgetProperty(CabbageIdentifierIds::name, getWidgetPropertyWithDefault("name", "").toString()+String(ID));
        setWidgetProperty(CabbageIdentifierIds::identchannel, "");
        setWidgetProperty(CabbageIdentifierIds::visible, 1);
        setWidgetProperty(CabbageIdentifierIds::active, 0);

        var tables;
        tables.append(0);
        tables.append(0);

        setWidgetProperty(CabbageIdentifierIds::scrubberposition, tables);
        setWidgetProperty(CabbageIdentifierIds::zoom, -1);
        setWidgetProperty(CabbageIdentifierIds::startpos, 0);
        setWidgetProperty(CabbageIdentifierIds::tablenumber, -1);
        setWidgetProperty(CabbageIdentifierIds::outlinethickness, 1.f);
        setWidgetProperty(CabbageIdentifierIds::tablebackgroundcolour, Colour(15, 15, 15).toString());
        setWidgetProperty(CabbageIdentifierIds::tablegridcolour, Colour(45, 45, 45).toString());
    }

    //===============image==================//
    else if(strTokens[0].trim() == "image")
    {
        setWidgetProperty("basetype", "layout");
        top = 10;
        left = 10;
        width = 160;
        height = 120;


        setWidgetProperty(CabbageIdentifierIds::top, 10);
        setWidgetProperty(CabbageIdentifierIds::left, 10);
        setWidgetProperty(CabbageIdentifierIds::width, 160);
        setWidgetProperty(CabbageIdentifierIds::height, 120);
        setWidgetProperty(CabbageIdentifierIds::shape, "rounded");
        setWidgetProperty(CabbageIdentifierIds::corners, 5);
        setWidgetProperty(CabbageIdentifierIds::colour, Colours::white.toString());
        setWidgetProperty(CabbageIdentifierIds::outlinecolour, Colours::white.toString());
        setWidgetProperty(CabbageIdentifierIds::outlinethickness, 0);
        setWidgetProperty(CabbageIdentifierIds::popup, 0);
        setWidgetProperty(CabbageIdentifierIds::plant, "");
        setWidgetProperty(CabbageIdentifierIds::type, "image");
        setWidgetProperty(CabbageIdentifierIds::file, "");
        setWidgetProperty(CabbageIdentifierIds::name, "image");
        setWidgetProperty(CabbageIdentifierIds::name, getWidgetPropertyWithDefault("name", "").toString()+String(ID));
        setWidgetProperty(CabbageIdentifierIds::identchannel, "");
        setWidgetProperty(CabbageIdentifierIds::visible, 1);
    }

    //===============image==================//
    else if(strTokens[0].trim() == "encoder")
    {
        setWidgetProperty(CabbageIdentifierIds::basetype, "interactive");
        setWidgetProperty(CabbageIdentifierIds::top, 10);
        setWidgetProperty(CabbageIdentifierIds::left, 10);
        setWidgetProperty(CabbageIdentifierIds::width, 60);
        setWidgetProperty(CabbageIdentifierIds::height, 60);
        var channels;
        channels.append("encoder");
        setWidgetProperty(CabbageIdentifierIds::channel, channels);
        setWidgetProperty(CabbageIdentifierIds::value, 0);
        setWidgetProperty(CabbageIdentifierIds::sliderincr, .001);
        setWidgetProperty(CabbageIdentifierIds::text, "");
        setWidgetProperty(CabbageIdentifierIds::textbox, 0.f);
        setWidgetProperty(CabbageIdentifierIds::caption, "");
        setWidgetProperty(CabbageIdentifierIds::colour, Colours::whitesmoke.toString());
        setWidgetProperty(CabbageIdentifierIds::trackercolour, Colour(0, 118, 38).toString());
        setWidgetProperty(CabbageIdentifierIds::fontcolour, CabbageUtilities::getComponentFontColour().toString());
        setWidgetProperty(CabbageIdentifierIds::textcolour, CabbageUtilities::getComponentFontColour().toString());
        setWidgetProperty(CabbageIdentifierIds::outlinecolour, Colours::black.brighter(.3f).toString());
        setWidgetProperty(CabbageIdentifierIds::midichan, -99);
        setWidgetProperty(CabbageIdentifierIds::midictrl, -99);
        //these don't appear in the props dialog
        setWidgetProperty(CabbageIdentifierIds::name, "encoder");
        setWidgetProperty(CabbageIdentifierIds::type, getWidgetPropertyWithDefault("name", "").toString());
        setWidgetProperty(CabbageIdentifierIds::name, getWidgetPropertyWithDefault("name", "").toString()+String(ID));
        setWidgetProperty(CabbageIdentifierIds::decimalplaces, 1);
        setWidgetProperty(CabbageIdentifierIds::identchannel, "");
        setWidgetProperty(CabbageIdentifierIds::trackerthickness, .05);
        setWidgetProperty(CabbageIdentifierIds::visible, 1);
        setWidgetProperty(CabbageIdentifierIds::minenabled, 0);
        setWidgetProperty(CabbageIdentifierIds::maxenabled, 0);
    }

    //===============groupbox==================//
    else if(strTokens[0].trim() == "groupbox")
    {
        setWidgetProperty(CabbageIdentifierIds::basetype, "layout");
        setWidgetProperty(CabbageIdentifierIds::top, 10);
        setWidgetProperty(CabbageIdentifierIds::left, 10);
        setWidgetProperty(CabbageIdentifierIds::width, 80);
        setWidgetProperty(CabbageIdentifierIds::height, 22);
        setWidgetProperty(CabbageIdentifierIds::corners, 5);
        setWidgetProperty(CabbageIdentifierIds::colour, Colour(35, 35, 35).toString());
        setWidgetProperty(CabbageIdentifierIds::fontcolour, CabbageUtilities::getComponentFontColour().toString());
        setWidgetProperty(CabbageIdentifierIds::popup, 0);
        setWidgetProperty(CabbageIdentifierIds::plant, "");
        setWidgetProperty(CabbageIdentifierIds::child, 0);
        setWidgetProperty(CabbageIdentifierIds::linethickness, 1);
        setWidgetProperty(CabbageIdentifierIds::type, "groupbox");
        setWidgetProperty(CabbageIdentifierIds::name, "groupbox");
        setWidgetProperty(CabbageIdentifierIds::name, getWidgetPropertyWithDefault("name", "").toString()+String(ID));
        setWidgetProperty(CabbageIdentifierIds::identchannel, "");
        setWidgetProperty(CabbageIdentifierIds::visible, 1);
        setWidgetProperty(CabbageIdentifierIds::svggroupbox, "");

    }
    //===============line==================//
    else if(strTokens[0].trim() == "line")
    {
        setWidgetProperty("basetype", "layout");
        top = 10;
        left = 10;
        width = 180;
        height = 2;

        setWidgetProperty(CabbageIdentifierIds::top, 10);
        setWidgetProperty(CabbageIdentifierIds::left, 10);
        setWidgetProperty(CabbageIdentifierIds::width, 160);
        setWidgetProperty(CabbageIdentifierIds::height, 2);
        setWidgetProperty(CabbageIdentifierIds::colour, Colours::white.toString());
        setWidgetProperty(CabbageIdentifierIds::type, "line");
        setWidgetProperty(CabbageIdentifierIds::name, "line");
        setWidgetProperty(CabbageIdentifierIds::name, getWidgetPropertyWithDefault("name", "").toString()+String(ID));
        setWidgetProperty(CabbageIdentifierIds::identchannel, "");
        setWidgetProperty(CabbageIdentifierIds::visible, 1);
    }

    //===============csoundoutput==================//
    else if(strTokens[0].trim() == "csoundoutput")
    {
        setWidgetProperty("basetype", "layout");
        top = 10;
        left = 10;
        width = 400;
        height = 200;
        setWidgetProperty(CabbageIdentifierIds::top, 10);
        setWidgetProperty(CabbageIdentifierIds::left, 10);
        setWidgetProperty(CabbageIdentifierIds::width, 400);
        setWidgetProperty(CabbageIdentifierIds::text, "Csound output");
        setWidgetProperty(CabbageIdentifierIds::height, 200);
        setWidgetProperty(CabbageIdentifierIds::colour, Colours::black.toString());
        setWidgetProperty(CabbageIdentifierIds::fontcolour, Colours::cornflowerblue.toString());
        setWidgetProperty(CabbageIdentifierIds::name, "csoundoutput");
        setWidgetProperty(CabbageIdentifierIds::wrap, 0);

        setWidgetProperty(CabbageIdentifierIds::type, "csoundoutput");
        setWidgetProperty(CabbageIdentifierIds::name, getWidgetPropertyWithDefault("name", "").toString()+String(ID));
        setWidgetProperty(CabbageIdentifierIds::identchannel, "");
        setWidgetProperty(CabbageIdentifierIds::visible, 1);
    }
    //===============textbox==================//
    else if(strTokens[0].trim() == "textbox")
    {
        setWidgetProperty("basetype", "layout");
        top = 10;
        left = 10;
        width = 400;
        height = 200;
        setWidgetProperty(CabbageIdentifierIds::top, 10);
        setWidgetProperty(CabbageIdentifierIds::left, 10);
        setWidgetProperty(CabbageIdentifierIds::width, 400);
        setWidgetProperty(CabbageIdentifierIds::text, "");
        setWidgetProperty(CabbageIdentifierIds::height, 200);
        setWidgetProperty(CabbageIdentifierIds::colour, Colours::black.toString());
        setWidgetProperty(CabbageIdentifierIds::fontcolour, Colours::white.toString());
        setWidgetProperty(CabbageIdentifierIds::name, "textbox");
        setWidgetProperty(CabbageIdentifierIds::type, "textbox");
        setWidgetProperty(CabbageIdentifierIds::wrap, 0);
        setWidgetProperty(CabbageIdentifierIds::file, "");
        setWidgetProperty(CabbageIdentifierIds::name, getWidgetPropertyWithDefault("name", "").toString()+String(ID));
        setWidgetProperty(CabbageIdentifierIds::identchannel, "");
        setWidgetProperty(CabbageIdentifierIds::visible, 1);
    }

    //===============textbox==================//
    else if(strTokens[0].trim() == "texteditor")
    {
        setWidgetProperty("basetype", "layout");
        top = 10;
        left = 10;
        width = 400;
        height = 200;
        setWidgetProperty(CabbageIdentifierIds::top, 10);
        setWidgetProperty(CabbageIdentifierIds::left, 10);
        setWidgetProperty(CabbageIdentifierIds::width, 400);
        setWidgetProperty(CabbageIdentifierIds::text, "");
        setWidgetProperty(CabbageIdentifierIds::height, 200);
        setWidgetProperty(CabbageIdentifierIds::channel, "texteditor");
        setWidgetProperty(CabbageIdentifierIds::colour, Colours::white.toString());
        setWidgetProperty(CabbageIdentifierIds::fontcolour, Colours::black.toString());
        setWidgetProperty(CabbageIdentifierIds::name, "texteditor");
        setWidgetProperty(CabbageIdentifierIds::type, "texteditor");
        setWidgetProperty(CabbageIdentifierIds::name, getWidgetPropertyWithDefault("name", "").toString()+String(ID));
        setWidgetProperty(CabbageIdentifierIds::identchannel, "");
        setWidgetProperty(CabbageIdentifierIds::visible, 1);
    }

    //===============table==================//
    else if(strTokens[0].trim() == "table")
    {
        setWidgetProperty("basetype", "layout");
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

        setWidgetProperty(CabbageIdentifierIds::top, 10);
        setWidgetProperty(CabbageIdentifierIds::left, 10);
        setWidgetProperty(CabbageIdentifierIds::width, 400);
        setWidgetProperty(CabbageIdentifierIds::height, 200);
        setWidgetProperty(CabbageIdentifierIds::tablenumber, 1);
        setWidgetProperty(CabbageIdentifierIds::drawmode, "");
        setWidgetProperty(CabbageIdentifierIds::resizemode, 0);
        setWidgetProperty(CabbageIdentifierIds::readonly, 0);
        setWidgetProperty(CabbageIdentifierIds::tablecolour, tableColours);
        setWidgetProperty(CabbageIdentifierIds::amprange, 0);
        setWidgetProperty(CabbageIdentifierIds::type, "table");
        setWidgetProperty(CabbageIdentifierIds::stack, 0);
        setWidgetProperty(CabbageIdentifierIds::name, getWidgetPropertyWithDefault("name", "").toString()+String(ID));
        setWidgetProperty(CabbageIdentifierIds::identchannel, "");
        setWidgetProperty(CabbageIdentifierIds::visible, 1);
    }

    //===============xypad==================//
    else if(strTokens[0].trim() == "xypad")
    {
        setWidgetProperty("basetype", "interactive");
        top = 10;
        left = 10;
        width = 200;
        height = 200;

        var channels;
        channels.append("xChan");
        channels.append("yChan");
        setWidgetProperty(CabbageIdentifierIds::xyautoindex, 0);
        setWidgetProperty(CabbageIdentifierIds::xchannel, channels[0]);
        setWidgetProperty(CabbageIdentifierIds::ychannel, channels[1]);
        setWidgetProperty(CabbageIdentifierIds::top, 10);
        setWidgetProperty(CabbageIdentifierIds::left, 10);
        setWidgetProperty(CabbageIdentifierIds::caption, "");
        setWidgetProperty(CabbageIdentifierIds::width, 200);
        setWidgetProperty(CabbageIdentifierIds::height, 200);
        setWidgetProperty(CabbageIdentifierIds::minx, 0);
        setWidgetProperty(CabbageIdentifierIds::maxx, 200);
        setWidgetProperty(CabbageIdentifierIds::text, "");
        setWidgetProperty(CabbageIdentifierIds::miny, 0);
        setWidgetProperty(CabbageIdentifierIds::maxy, 200);
        setWidgetProperty(CabbageIdentifierIds::valuex, 0);
        setWidgetProperty(CabbageIdentifierIds::valuey, 0);
        setWidgetProperty(CabbageIdentifierIds::colour, Colours::lime.toString());
        setWidgetProperty(CabbageIdentifierIds::fontcolour, Colours::cornflowerblue.toString());
        setWidgetProperty(CabbageIdentifierIds::textcolour, Colours::cornflowerblue.toString());
        setWidgetProperty(CabbageIdentifierIds::type, "xypad");
        setWidgetProperty(CabbageIdentifierIds::name, "xypad");
        setWidgetProperty(CabbageIdentifierIds::name, getWidgetPropertyWithDefault("name", "").toString()+String(ID));
        setWidgetProperty(CabbageIdentifierIds::identchannel, "");
        setWidgetProperty(CabbageIdentifierIds::visible, 1);
    }
    //===============keyboard==================//
    else if(strTokens[0].trim() == "keyboard")
    {
        setWidgetProperty("basetype", "layout");
        top = 10;
        left = 10;
        width = 400;
        height = 100;

        setWidgetProperty(CabbageIdentifierIds::top, 10);
        setWidgetProperty(CabbageIdentifierIds::left, 10);
        setWidgetProperty(CabbageIdentifierIds::width, 400);
        setWidgetProperty(CabbageIdentifierIds::height, 100);
        setWidgetProperty(CabbageIdentifierIds::value, 60);
        setWidgetProperty(CabbageIdentifierIds::middlec, 3);
        setWidgetProperty(CabbageIdentifierIds::type, "keyboard");
        setWidgetProperty(CabbageIdentifierIds::name, "keyboard");
        setWidgetProperty(CabbageIdentifierIds::name, getWidgetPropertyWithDefault("name", "").toString()+String(ID));
        setWidgetProperty(CabbageIdentifierIds::visible, 1);
        setWidgetProperty(CabbageIdentifierIds::keywidth, 16);
        setWidgetProperty(CabbageIdentifierIds::scrollbars, 1);

    }
    //===============form==================//
    else if(strTokens[0].trim() == "form")
    {
        setWidgetProperty(CabbageIdentifierIds::basetype, "layout");
        setWidgetProperty(CabbageIdentifierIds::top, 10);
        setWidgetProperty(CabbageIdentifierIds::left, 10);
        setWidgetProperty(CabbageIdentifierIds::width, 600);
        setWidgetProperty(CabbageIdentifierIds::height, 300);
        setWidgetProperty(CabbageIdentifierIds::caption, "");
        setWidgetProperty(CabbageIdentifierIds::name, "form");
        setWidgetProperty(CabbageIdentifierIds::type, "form");
        setWidgetProperty(CabbageIdentifierIds::guirefresh, 100);
        setWidgetProperty(CabbageIdentifierIds::identchannel, "");
        setWidgetProperty(CabbageIdentifierIds::visible, 1);
        setWidgetProperty(CabbageIdentifierIds::scrollbars, 1);
        setWidgetProperty(CabbageIdentifierIds::titlebarcolour, "");
        setWidgetProperty(CabbageIdentifierIds::fontcolour, "");
    }

    //===============rangeslider===============//
    else if(strTokens[0].trim() == "hrange" || strTokens[0].trim() == "vrange")
    {
        setWidgetProperty(CabbageIdentifierIds::basetype, "interactive");
        setWidgetProperty(CabbageIdentifierIds::top, 10);
        setWidgetProperty(CabbageIdentifierIds::left, 10);
        setWidgetProperty(CabbageIdentifierIds::width, 50);
        setWidgetProperty(CabbageIdentifierIds::height, 150);
        var channels;
        channels.append("rangeslider");
        setWidgetProperty(CabbageIdentifierIds::channel, channels);
        setWidgetProperty(CabbageIdentifierIds::min, 0);
        setWidgetProperty(CabbageIdentifierIds::max, 1);
        setWidgetProperty(CabbageIdentifierIds::value, 0);
        setWidgetProperty(CabbageIdentifierIds::text, "");
        setWidgetProperty(CabbageIdentifierIds::textbox, 0.f);
        setWidgetProperty(CabbageIdentifierIds::caption, "");
        setWidgetProperty(CabbageIdentifierIds::colour, Colours::white.toString());
        setWidgetProperty(CabbageIdentifierIds::trackercolour, Colour(0, 118, 38).toString());
        setWidgetProperty(CabbageIdentifierIds::fontcolour, CabbageUtilities::getComponentFontColour().toString());
        setWidgetProperty(CabbageIdentifierIds::textcolour, CabbageUtilities::getComponentFontColour().toString());
        setWidgetProperty(CabbageIdentifierIds::sliderskew, 1);
        setWidgetProperty(CabbageIdentifierIds::sliderincr, .001);
        setWidgetProperty(CabbageIdentifierIds::midichan, -99);
        setWidgetProperty(CabbageIdentifierIds::midictrl, -99);
        //these don't appear in the props dialog
        setWidgetProperty(CabbageIdentifierIds::name, strTokens[0].trim());
        setWidgetProperty(CabbageIdentifierIds::type, getWidgetPropertyWithDefault("name", "").toString());
        setWidgetProperty(CabbageIdentifierIds::name, getWidgetPropertyWithDefault("name", "").toString()+String(ID));
        if(strTokens[0].trim() == "hrange")
            setWidgetProperty(CabbageIdentifierIds::kind, "horizontal");
        else
            setWidgetProperty(CabbageIdentifierIds::kind, "vertical");

        setWidgetProperty(CabbageIdentifierIds::decimalplaces, 1);
        setWidgetProperty(CabbageIdentifierIds::trackerthickness, .1);
        setWidgetProperty(CabbageIdentifierIds::identchannel, "");
        setWidgetProperty(CabbageIdentifierIds::visible, 1);
        setWidgetProperty(CabbageIdentifierIds::svgslider, "");
        setWidgetProperty(CabbageIdentifierIds::svgsliderbg, "");
    }

    //===============signaldisplay==================//
    else if(strTokens[0].trim() == "signaldisplay" || strTokens[0].trim() == "fftdisplay")
    {
        setWidgetProperty("basetype", "layout");
        top = 10;
        left = 10;
        width = 180;
        height = 2;

        var signalVariables;
        signalVariables.append("");

        setWidgetProperty(CabbageIdentifierIds::top, 10);
        setWidgetProperty(CabbageIdentifierIds::left, 10);
        setWidgetProperty(CabbageIdentifierIds::width, 260);
        setWidgetProperty(CabbageIdentifierIds::height, 100);
        setWidgetProperty(CabbageIdentifierIds::tablecolour, Colours::lime.toString());
        setWidgetProperty(CabbageIdentifierIds::fontcolour, Colours::white.toString());
        setWidgetProperty(CabbageIdentifierIds::tablebackgroundcolour, Colour(30, 30, 38).toString());
        setWidgetProperty(CabbageIdentifierIds::type, "signaldisplay");
        setWidgetProperty(CabbageIdentifierIds::name, "signaldisplay");
        setWidgetProperty(CabbageIdentifierIds::min, 0);
        setWidgetProperty(CabbageIdentifierIds::max, 2048);
        setWidgetProperty(CabbageIdentifierIds::updaterate, 100);
        setWidgetProperty(CabbageIdentifierIds::signalvariable, signalVariables);
        setWidgetProperty(CabbageIdentifierIds::outlinethickness, 1);
        setWidgetProperty(CabbageIdentifierIds::name, getWidgetPropertyWithDefault("name", "").toString()+String(ID));
        setWidgetProperty(CabbageIdentifierIds::identchannel, "");
        setWidgetProperty(CabbageIdentifierIds::displaytype, "spectroscope");
        setWidgetProperty(CabbageIdentifierIds::zoom, 0);
        setWidgetProperty(CabbageIdentifierIds::visible, 1);
    }

    //===============stepper==================//
    else if(strTokens[0].trim() == "scope")
    {
        setWidgetProperty("basetype", "layout");
        top = 10;
        left = 10;
        width = 180;
        height = 2;

        setWidgetProperty(CabbageIdentifierIds::top, 10);
        setWidgetProperty(CabbageIdentifierIds::left, 10);
        setWidgetProperty(CabbageIdentifierIds::width, 160);
        setWidgetProperty(CabbageIdentifierIds::height, 2);
        setWidgetProperty(CabbageIdentifierIds::colour, Colours::white.toString());
        setWidgetProperty(CabbageIdentifierIds::type, "scope");
        setWidgetProperty(CabbageIdentifierIds::name, "scope");
        setWidgetProperty(CabbageIdentifierIds::name, getWidgetPropertyWithDefault("name", "").toString()+String(ID));
        setWidgetProperty(CabbageIdentifierIds::identchannel, "");
        setWidgetProperty(CabbageIdentifierIds::visible, 1);
    }

    //===============stepper==================//
    else if(strTokens[0].trim() == "stepper")
    {
        setWidgetProperty("basetype", "layout");
        top = 10;
        left = 10;
        width = 180;
        height = 2;

        setWidgetProperty(CabbageIdentifierIds::top, 10);
        setWidgetProperty(CabbageIdentifierIds::left, 10);
        setWidgetProperty(CabbageIdentifierIds::width, 160);
        setWidgetProperty(CabbageIdentifierIds::height, 2);
        setWidgetProperty(CabbageIdentifierIds::colour, Colours::white.toString());
        setWidgetProperty(CabbageIdentifierIds::type, "stepper");
        setWidgetProperty(CabbageIdentifierIds::name, "stepper");
        setWidgetProperty(CabbageIdentifierIds::name, getWidgetPropertyWithDefault("name", "").toString()+String(ID));
        setWidgetProperty(CabbageIdentifierIds::identchannel, "");
        setWidgetProperty(CabbageIdentifierIds::visible, 1);
    }

    //===============non-GUI host widgets==================//
    else if(strTokens[0].trim() == "hostbpm")
    {
        setWidgetProperty(CabbageIdentifierIds::basetype, "layout");
        setWidgetProperty(CabbageIdentifierIds::name, "hostbpm");
        setWidgetProperty(CabbageIdentifierIds::type, "hostbpm");
    }

    else if(strTokens[0].trim() == "hostppqpos")
    {
        setWidgetProperty(CabbageIdentifierIds::basetype, "layout");
        setWidgetProperty(CabbageIdentifierIds::name, "hostppqpos");
        setWidgetProperty(CabbageIdentifierIds::type, "hostppqpos");
    }

    else if(strTokens[0].trim() == "hostplaying")
    {
        setWidgetProperty(CabbageIdentifierIds::basetype, "layout");
        setWidgetProperty(CabbageIdentifierIds::name, "hostplaying");
        setWidgetProperty(CabbageIdentifierIds::type, "hostplaying");
    }

    else if(strTokens[0].trim() == "hostrecording")
    {
        setWidgetProperty(CabbageIdentifierIds::basetype, "layout");
        setWidgetProperty(CabbageIdentifierIds::name, "hostrecording");
        setWidgetProperty(CabbageIdentifierIds::type, "hostrecording");
    }

    else if(strTokens[0].trim() == "hosttime")
    {
        setWidgetProperty(CabbageIdentifierIds::basetype, "layout");
        setWidgetProperty(CabbageIdentifierIds::name, "hosttime");
        setWidgetProperty(CabbageIdentifierIds::type, "hosttime");
    }

    setWidgetProperty(CabbageIdentifierIds::popuptext, "");
    setWidgetProperty(CabbageIdentifierIds::alpha, 1);
    setWidgetProperty(CabbageIdentifierIds::visible, 1);
    setWidgetProperty(CabbageIdentifierIds::rotate, 0.f);
    setWidgetProperty(CabbageIdentifierIds::pivotx, 0.f);
    setWidgetProperty(CabbageIdentifierIds::pivoty, 0.f);


	//parse the text now that all default values ahve been assigned
    parse(compStr, "");
}

//===========================================================================================
// this method parsing the Cabbage text and set each of the Cabbage indentifers
//===========================================================================================
void CabbageWidget::parse(String inStr, String identifier)
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

    String typeOfWidget="";
    StringArray strTokens, tempTokens;
    tempTokens.addTokens(str, ", \t", "\"");
    typeOfWidget = tempTokens[0];
    //Logger::writeToLog(typeOfWidget);

    for(int indx=0; indx<identArray.size(); indx++)
    {
        int identPos = str.toLowerCase().indexOf(" "+identArray[indx]+"(");
        if(identPos<0)
            identPos = str.toLowerCase().indexOf(","+identArray[indx]+"(");
		if(identPos<0)
			identPos = str.toLowerCase().indexOf(" "+identArray[indx]+" (");
		if(identPos<0)
			identPos = str.toLowerCase().indexOf(","+identArray[indx]+" (");
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
                setWidgetProperty(CabbageIdentifierIds::name, strTokens[0].trim());
            }
            else if(identArray[indx].equalsIgnoreCase("plant"))
            {
                setWidgetProperty(CabbageIdentifierIds::plant, strTokens[0].trim());
            }
            else if(identArray[indx].equalsIgnoreCase("caption"))
            {
                setWidgetProperty(CabbageIdentifierIds::caption, strTokens[0].trim());
            }

            else if(identArray[indx].equalsIgnoreCase("signalvariable"))
            {
                var array;
                array.append(strTokens[0].trim());
                for(int i=1; i<strTokens.size(); i++)
                {
                    array.append(strTokens[i].trim());
                }
                setWidgetProperty(CabbageIdentifierIds::signalvariable, array);
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
                    setWidgetProperty(CabbageIdentifierIds::xchannel, strTokens[0].trim());
                    setWidgetProperty(CabbageIdentifierIds::ychannel, strTokens[1].trim());
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
                    for(int i=0; i<getStringArrayProp(CabbageIdentifierIds::channel).size(); i++)
                        array.append(getStringArrayPropValue(CabbageIdentifierIds::channel, i));

                    var channelArray, identChannelArray;
                    int size = strTokens[1].getIntValue();
                    for(int i=0; i<size; i++)
                    {
                        array.append(strTokens[0]+String(i+1));
                    }
                    setWidgetProperty(CabbageIdentifierIds::channel, array);
                    //identifiers that appear more than once need to use indx-- so we can check for another instance
                    indx--;
                }

                setWidgetProperty(CabbageIdentifierIds::channel, array);
                if(identArray[indx].equalsIgnoreCase("widgetarray"))
                {
                    var channelArray, identChannelArray;
                    int size = strTokens[1].getIntValue();
                    for(int i=0; i<size; i++)
                    {
                        channelArray.append(String(strTokens[0])+String(i+1));
                        identChannelArray.append(String(strTokens[0])+String("_ident")+String(i+1));

                    }
                    setWidgetProperty(CabbageIdentifierIds::channelarray, channelArray);
                    setWidgetProperty(CabbageIdentifierIds::identchannelarray, identChannelArray);
                }
            }

            else if(identArray[indx].equalsIgnoreCase("colour:0")||
                    identArray[indx].equalsIgnoreCase("colours("))
            {
                setWidgetProperty(CabbageIdentifierIds::colour, getColourFromText(strTokens.joinIntoString(",")).toString());
            }

            else if(identArray[indx].equalsIgnoreCase("colour:1") ||
                    identArray[indx].equalsIgnoreCase("colour"))
            {
                if(typeOfWidget.contains("checkbox") || typeOfWidget.contains("button"))
                {
                    setWidgetProperty(CabbageIdentifierIds::oncolour, getColourFromText(strTokens.joinIntoString(",")).toString());
                }
                else
                    setWidgetProperty(CabbageIdentifierIds::colour, getColourFromText(strTokens.joinIntoString(",")).toString());
            }

            else if(identArray[indx].equalsIgnoreCase("fontcolour")||
                    identArray[indx].equalsIgnoreCase("fontcolour:1"))
            {
                if(typeOfWidget.contains("button"))
                    setWidgetProperty(CabbageIdentifierIds::onfontcolour, getColourFromText(strTokens.joinIntoString(",")).toString());
                else
                    setWidgetProperty(CabbageIdentifierIds::fontcolour, getColourFromText(strTokens.joinIntoString(",")).toString());

            }
            else if(identArray[indx].equalsIgnoreCase("fontcolour:0"))
            {
                setWidgetProperty(CabbageIdentifierIds::fontcolour, getColourFromText(strTokens.joinIntoString(",")).toString());
            }

            else if(identArray[indx].equalsIgnoreCase("tablebackgroundcolour"))
            {
                setWidgetProperty(CabbageIdentifierIds::tablebackgroundcolour, getColourFromText(strTokens.joinIntoString(",")).toString());
            }

            else if(identArray[indx].equalsIgnoreCase("backgroundcolour"))
            {
                setWidgetProperty(CabbageIdentifierIds::backgroundcolour, getColourFromText(strTokens.joinIntoString(",")).toString());
            }

            else if(identArray[indx].equalsIgnoreCase("tablegridcolour"))
            {
                setWidgetProperty(CabbageIdentifierIds::tablegridcolour, getColourFromText(strTokens.joinIntoString(",")).toString());
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
                setWidgetProperty(CabbageIdentifierIds::tablecolour, colours);
            }

            else if(identArray[indx].equalsIgnoreCase("trackercolour"))
            {
                setWidgetProperty(CabbageIdentifierIds::trackercolour, getColourFromText(strTokens.joinIntoString(",")).toString());
            }

            else if(identArray[indx].equalsIgnoreCase("highlightcolour"))
            {
                setWidgetProperty(CabbageIdentifierIds::highlightcolour, getColourFromText(strTokens.joinIntoString(",")).toString());
            }

            else if(identArray[indx].equalsIgnoreCase("titlebarcolour"))
            {
                setWidgetProperty(CabbageIdentifierIds::titlebarcolour, getColourFromText(strTokens.joinIntoString(",")).toString());
            }

            else if(identArray[indx].equalsIgnoreCase("kind"))
            {
                setWidgetProperty(CabbageIdentifierIds::kind, strTokens[0].trim());
            }

            else if(identArray[indx].equalsIgnoreCase("file"))
            {
                setWidgetProperty(CabbageIdentifierIds::file, strTokens[0].trim());
            }

            else if(identArray[indx].equalsIgnoreCase("svgpath"))
            {
                setWidgetProperty(CabbageIdentifierIds::svgpath, strTokens[0].trim());
            }

            else if(identArray[indx].equalsIgnoreCase("svgfile"))
            {
                if(typeOfWidget=="groupbox")
                    setWidgetProperty(CabbageIdentifierIds::svggroupbox, strTokens[0].trim());
                else if(typeOfWidget.contains("slider"))
                {
                    if(strTokens[0].trim().contains("background"))
                        setWidgetProperty(CabbageIdentifierIds::svgsliderbg, strTokens[1].trim());
                    else
                        setWidgetProperty(CabbageIdentifierIds::svgslider, strTokens[1].trim());
                }
                else if(typeOfWidget.contains("button"))
                {
                    if(strTokens[0].trim().contains("off"))
                        setWidgetProperty(CabbageIdentifierIds::svgbuttonoff, strTokens[1].trim());
                    else
                        setWidgetProperty(CabbageIdentifierIds::svgbuttonon, strTokens[1].trim());
                }
                //identifiers that appear more than once need to use indx-- so we can check for another instance
                indx--;
            }

            else if(identArray[indx].equalsIgnoreCase("fillcolour"))
            {
                setWidgetProperty(CabbageIdentifierIds::fillcolour, getColourFromText(strTokens.joinIntoString(",")).toString());
            }

            else if(identArray[indx].equalsIgnoreCase("shape"))
            {
                setWidgetProperty(CabbageIdentifierIds::shape, strTokens[0].trim());
            }
            else if(identArray[indx].equalsIgnoreCase("outlinecolour"))
            {
                setWidgetProperty(CabbageIdentifierIds::outlinecolour, getColourFromText(strTokens.joinIntoString(",")).toString());
            }
            else if(identArray[indx].equalsIgnoreCase("textcolour"))
            {
                setWidgetProperty(CabbageIdentifierIds::textcolour, getColourFromText(strTokens.joinIntoString(",")).toString());
            }

            else if(identArray[indx].equalsIgnoreCase("pluginid"))
            {
                setWidgetProperty(CabbageIdentifierIds::pluginid, strTokens[0].trim());
            }

            else if(identArray[indx].equalsIgnoreCase("popuptext"))
            {
                setWidgetProperty(CabbageIdentifierIds::popuptext, strTokens[0]);
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

                setWidgetProperty(CabbageIdentifierIds::text, value);
                setWidgetProperty(CabbageIdentifierIds::comborange, comboRange);
            }


            else if(identArray[indx].equalsIgnoreCase("preset"))
            {
                //CabbageUtilities::showMessage("preset is no longer supported, see docs");
            }

            else if(identArray[indx].equalsIgnoreCase("mode") ||
                    identArray[indx].equalsIgnoreCase(",mode"))
            {
                setWidgetProperty(CabbageIdentifierIds::mode, strTokens[0].trim());
            }

            else if(identArray[indx].equalsIgnoreCase("native"))
            {
                setWidgetProperty("native", strTokens[0].trim());
            }


            else if(identArray[indx].equalsIgnoreCase("channeltype"))
            {
                setWidgetProperty(CabbageIdentifierIds::channeltype, strTokens[0].trim());
            }

            else if(identArray[indx].equalsIgnoreCase("identchannel"))
            {
                setWidgetProperty(CabbageIdentifierIds::identchannel, strTokens[0].trim());
            }

            else if(identArray[indx].equalsIgnoreCase("populate"))
            {
                setWidgetProperty(CabbageIdentifierIds::filetype, strTokens[0].trim());

                if(strTokens.size()>1)
                {
                    setWidgetProperty(CabbageIdentifierIds::workingdir, strTokens[1].trim());
                }
            }

            else if(identArray[indx].equalsIgnoreCase("author"))
            {
                setWidgetProperty(CabbageIdentifierIds::author, strTokens[0].trim());
            }

            else if(identArray[indx].equalsIgnoreCase("align"))
            {
                setWidgetProperty(CabbageIdentifierIds::align, strTokens[0].trim());
            }


            else if(identArray[indx].equalsIgnoreCase("tabs"))
            {
                var value;
                for(int i= 0; i<(int)strTokens.size(); i++)
                {
                    value.append(strTokens[i].trim());
                }
                setWidgetProperty(CabbageIdentifierIds::tabs, strTokens[0].trim());
            }

            else if(identArray[indx].equalsIgnoreCase("tabpage"))
            {
                setWidgetProperty(CabbageIdentifierIds::tabpage, strTokens[0].trim());
            }

            else if(identArray[indx].equalsIgnoreCase("displaytype"))
            {
                setWidgetProperty(CabbageIdentifierIds::displaytype, strTokens[0].trim());
            }

            else if(identArray[indx].equalsIgnoreCase("include"))
            {
                var array;
                for(int i=0; i<strTokens.size(); i++)
                {
                    array.append(strTokens[i].trim());
                }

                setWidgetProperty(CabbageIdentifierIds::include, array);
            }

            else if(identArray[indx].equalsIgnoreCase("drawmode"))
            {
                var value;
                if(strTokens.size()>0)
                    for(int i=0; i<strTokens.size(); i++)
                    {
                        value.append(strTokens[i].trim());
                    }
                setWidgetProperty(CabbageIdentifierIds::drawmode, value);
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
                    setWidgetProperty(CabbageIdentifierIds::socketaddress, address);
                    setWidgetProperty(CabbageIdentifierIds::socketport, port);
                }
            }

            else if(identArray[indx].equalsIgnoreCase("size"))
            {
                if(strTokens.size()>=2)
                {
                    //sizeText = identArray.getReference(indx)+tstr+")";
                    width = strTokens[0].trim().getFloatValue();
                    height = strTokens[1].trim().getFloatValue();
                    setWidgetProperty(CabbageIdentifierIds::width, strTokens[0].trim().getFloatValue());
                    setWidgetProperty(CabbageIdentifierIds::height, strTokens[1].trim().getFloatValue());
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
                    left = strTokens[0].trim().getFloatValue();
                    top = strTokens[1].trim().getFloatValue();
                    width = strTokens[2].trim().getFloatValue();
                    if(strTokens.size()==3)
                    {
                        height = width;
                        setWidgetProperty(CabbageIdentifierIds::height, width);
                    }
                    else
                    {
                        height = strTokens[3].trim().getFloatValue();
                        setWidgetProperty(CabbageIdentifierIds::left, strTokens[0].trim().getFloatValue());
                        setWidgetProperty(CabbageIdentifierIds::top, strTokens[1].trim().getFloatValue());
                        setWidgetProperty(CabbageIdentifierIds::width, strTokens[2].trim().getFloatValue());
                        setWidgetProperty(CabbageIdentifierIds::height, strTokens[3].trim().getFloatValue());
                    }

                }
            }
            //!!! This haven't been set for the generic data system
            else if(identArray[indx].equalsIgnoreCase("config"))
            {
                vuConfig.clear();
                for(int i= 0; i<(int)strTokens.size(); i++)
                {
                    vuConfig.add(strTokens[i].trim().getFloatValue());
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
                    left = strTokens[0].trim().getFloatValue();
                    top = strTokens[1].trim().getFloatValue();
                    setWidgetProperty(CabbageIdentifierIds::left, strTokens[0].trim().getFloatValue());
                    setWidgetProperty(CabbageIdentifierIds::top, strTokens[1].trim().getFloatValue());
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
                    setWidgetProperty(CabbageIdentifierIds::min, strTokens[0].trim().getDoubleValue());
                    setWidgetProperty(CabbageIdentifierIds::max, strTokens[1].trim().getDoubleValue());
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
                                setWidgetProperty(CabbageIdentifierIds::minvalue, values[0].trim().getFloatValue());
                                setWidgetProperty(CabbageIdentifierIds::maxvalue, values[1].trim().getFloatValue());
                            }
                        }
                        else
                        {
                            setWidgetProperty(CabbageIdentifierIds::value, strTokens[2].trim().getDoubleValue() < min ? min : strTokens[2].trim().getDoubleValue());
                            tempArray.add(strTokens[2].trim());
                        }
                    }


                    if(strTokens.size()>3)
                    {
                        setWidgetProperty(CabbageIdentifierIds::sliderskew, strTokens[3].trim().getDoubleValue());
                        tempArray.add(strTokens[3].trim());
                    }

                    if(strTokens.size()>4)
                    {
                        tempArray.add(strTokens[4].trim());
                        setWidgetProperty(CabbageIdentifierIds::sliderincr, strTokens[4].trim().getDoubleValue());
                    }

                    double sliderRange = max-min;
                    int decimalPlaces = getNumberOfDecimalPlaces(tempArray);
                    if(decimalPlaces<1 && max<=1)
                        decimalPlaces = 2;
                    else
                        decimalPlaces = getNumberOfDecimalPlaces(tempArray);
                    setWidgetProperty(CabbageIdentifierIds::decimalplaces, decimalPlaces);
                    setWidgetProperty(CabbageIdentifierIds::range, sliderRange);

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

                    setWidgetProperty(CabbageIdentifierIds::minx, strTokens[0].trim().getDoubleValue());
                    setWidgetProperty(CabbageIdentifierIds::maxx, strTokens[1].getDoubleValue());
                    setWidgetProperty(CabbageIdentifierIds::valuex, strTokens[2].getDoubleValue());
                    setWidgetProperty(CabbageIdentifierIds::rangex, maxX-minX);

                    setWidgetProperty(CabbageIdentifierIds::decimalplaces, strTokens[3].trim().getDoubleValue());
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

                    setWidgetProperty(CabbageIdentifierIds::miny, strTokens[0].trim().getDoubleValue());
                    setWidgetProperty(CabbageIdentifierIds::maxy, strTokens[1].trim().getDoubleValue());
                    setWidgetProperty(CabbageIdentifierIds::valuey, strTokens[2].trim().getDoubleValue());
                    setWidgetProperty(CabbageIdentifierIds::rangey, maxY-minY);
                    setWidgetProperty(CabbageIdentifierIds::decimalplaces, strTokens[3].trim().getDoubleValue());
                }
            }
            else if(identArray[indx].equalsIgnoreCase("min"))
            {
                setWidgetProperty(CabbageIdentifierIds::minenabled, 1);
                setWidgetProperty(CabbageIdentifierIds::minvalue, strTokens[0].trim().getFloatValue());
            }

            else if(identArray[indx].equalsIgnoreCase("svgdebug"))
            {
                setWidgetProperty(CabbageIdentifierIds::svgdebug, strTokens[0].trim().getFloatValue());
            }

            else if(identArray[indx].equalsIgnoreCase("middlec"))
            {
                setWidgetProperty(CabbageIdentifierIds::middlec, strTokens[0].trim().getFloatValue());
            }

            else if(identArray[indx].equalsIgnoreCase("fill"))
            {
                setWidgetProperty(CabbageIdentifierIds::fill, strTokens[0].trim().getIntValue());
            }

            else if(identArray[indx].equalsIgnoreCase("updaterate"))
            {
                setWidgetProperty(CabbageIdentifierIds::updaterate, strTokens[0].trim().getIntValue());
            }

            else if(identArray[indx].equalsIgnoreCase("keywidth"))
            {
                setWidgetProperty(CabbageIdentifierIds::keywidth, strTokens[0].trim().getFloatValue());
            }

            else if(identArray[indx].equalsIgnoreCase("rotate"))
            {
                setWidgetProperty(CabbageIdentifierIds::rotate, strTokens[0].trim().getFloatValue());
                if(strTokens.size()==3)
                {
                    setWidgetProperty(CabbageIdentifierIds::pivotx, strTokens[1].trim().getFloatValue());
                    setWidgetProperty(CabbageIdentifierIds::pivoty, strTokens[2].trim().getFloatValue());
                }
            }

            else if(identArray[indx].equalsIgnoreCase("pivoty"))
            {
                setWidgetProperty(CabbageIdentifierIds::pivoty, strTokens[0].trim().getFloatValue());
            }

            else if(identArray[indx].equalsIgnoreCase("ffttablenumber"))
            {
                setWidgetProperty(CabbageIdentifierIds::ffttablenumber, strTokens[0].trim().getFloatValue()-1);
            }

            else if(identArray[indx].equalsIgnoreCase("pivotx"))
            {
                setWidgetProperty(CabbageIdentifierIds::pivotx, strTokens[0].trim().getFloatValue());
            }

            else if(identArray[indx].equalsIgnoreCase("midictrl"))
            {
                if(strTokens.size()<2)
                {
                    warningMessages+="Not enough paramters passed to midiCtrl(): usage midiCtrl(midiChan, midiCtrl\")\n";
                }
                else
                {
                    setWidgetProperty(CabbageIdentifierIds::midichan, strTokens[0].trim().getFloatValue());
                    setWidgetProperty(CabbageIdentifierIds::midictrl, strTokens[0].trim().getFloatValue());
                }
            }
            else if(identArray[indx].equalsIgnoreCase("max"))
            {
                setWidgetProperty(CabbageIdentifierIds::maxenabled, 1);
                setWidgetProperty(CabbageIdentifierIds::maxvalue, strTokens[0].trim().getFloatValue());
            }

            else if(identArray[indx].equalsIgnoreCase("sliderincr"))
            {
                setWidgetProperty(CabbageIdentifierIds::sliderincr, strTokens[0].trim().getFloatValue());
            }

            else if(identArray[indx].equalsIgnoreCase("sliderskew"))
            {
                setWidgetProperty(CabbageIdentifierIds::sliderskew, strTokens[0].trim().getFloatValue());
            }

            else if(identArray[indx].equalsIgnoreCase("visible"))
            {
                setWidgetProperty(CabbageIdentifierIds::visible, strTokens[0].trim().getFloatValue());
            }

            else if(identArray[indx].equalsIgnoreCase("active"))
            {
                setWidgetProperty(CabbageIdentifierIds::active, strTokens[0].trim().getFloatValue());
            }

            else if(identArray[indx].equalsIgnoreCase("stack"))
            {
                setWidgetProperty(CabbageIdentifierIds::stack, strTokens[0].trim().getFloatValue());
            }

            else if(identArray[indx].equalsIgnoreCase("tab"))
            {
                setWidgetProperty(CabbageIdentifierIds::tabbed, strTokens[0].trim().getFloatValue());
            }

            else if(identArray[indx].equalsIgnoreCase("latched"))
            {
                setWidgetProperty(CabbageIdentifierIds::latched, strTokens[0].trim().getFloatValue());
            }

            else if(identArray[indx].equalsIgnoreCase("alpha"))
            {
                setWidgetProperty(CabbageIdentifierIds::alpha, strTokens[0].trim().getFloatValue());
            }

            else if(identArray[indx].equalsIgnoreCase("corners"))
            {
                setWidgetProperty(CabbageIdentifierIds::corners, strTokens[0].trim().getFloatValue());
            }

            else if(identArray[indx].equalsIgnoreCase("radiogroup"))
            {
                setWidgetProperty(CabbageIdentifierIds::radiogroup, strTokens[0].trim().getFloatValue());
            }

            else if(identArray[indx].equalsIgnoreCase("guirefresh"))
            {
                setWidgetProperty(CabbageIdentifierIds::guirefresh, strTokens[0].trim().getFloatValue());
            }

            else if(identArray[indx].equalsIgnoreCase("fontstyle"))
            {
                if(strTokens[0].trim()=="bold")
                    setWidgetProperty(CabbageIdentifierIds::fontstyle, 1);
                else if(strTokens[0].trim()=="italic")
                    setWidgetProperty(CabbageIdentifierIds::fontstyle, 2);
                else if(strTokens[0].trim()=="bold italic")
                    setWidgetProperty(CabbageIdentifierIds::fontstyle, 3);
                else if(strTokens[0].trim()=="underlined")
                    setWidgetProperty(CabbageIdentifierIds::fontstyle, 4);
                else if(strTokens[0].trim()=="bold underlined")
                    setWidgetProperty(CabbageIdentifierIds::fontstyle, 5);
                else if(strTokens[0].trim()=="italic underlined")
                    setWidgetProperty(CabbageIdentifierIds::fontstyle, 6);
                else if(strTokens[0].trim()=="bold italic underlined")
                    setWidgetProperty(CabbageIdentifierIds::fontstyle, 7);
                else
                    setWidgetProperty(CabbageIdentifierIds::fontstyle, 0);
            }

            else if(identArray[indx].equalsIgnoreCase("textbox"))
            {
                setWidgetProperty(CabbageIdentifierIds::textbox, strTokens[0].trim().getFloatValue());
            }

            else if(identArray[indx].equalsIgnoreCase("velocity"))
            {
                setWidgetProperty(CabbageIdentifierIds::velocity, strTokens[0].trim().getFloatValue());
            }

            else if(identArray[indx].equalsIgnoreCase("gradient"))
            {
                setWidgetProperty(CabbageIdentifierIds::gradient, strTokens[0].trim().getFloatValue());
            }

            else if(identArray[indx].equalsIgnoreCase("scrubberposition"))
            {
                var scrubberInfo;
                scrubberInfo.append(strTokens[0].trim().getIntValue());
                for(int i=1; i<strTokens.size(); i++)
                {
                    scrubberInfo.append(strTokens[i].trim().getDoubleValue());
                    setWidgetProperty(CabbageIdentifierIds::scrubberposition, scrubberInfo);
                }

                setWidgetProperty(CabbageIdentifierIds::scrubberposition, scrubberInfo);

            }

            else if(identArray[indx].equalsIgnoreCase("logger"))
            {
                setWidgetProperty(CabbageIdentifierIds::logger, strTokens[0].trim().getIntValue());
            }

            else if(identArray[indx].equalsIgnoreCase("child"))
            {
                setWidgetProperty(CabbageIdentifierIds::child, strTokens[0].trim().getIntValue());
            }

            else if(identArray[indx].equalsIgnoreCase("show"))
            {
                setWidgetProperty(CabbageIdentifierIds::show, strTokens[0].trim().getIntValue());
            }

            else if(identArray[indx].equalsIgnoreCase("zoom"))
            {
                setWidgetProperty(CabbageIdentifierIds::zoom, strTokens[0].trim().getFloatValue());
                if(strTokens.size()>1)
                    setWidgetProperty(CabbageIdentifierIds::startpos, strTokens[1].trim().getFloatValue());
                else
                    setWidgetProperty(CabbageIdentifierIds::startpos, -1);
            }

            else if(identArray[indx].equalsIgnoreCase("samplerange"))
            {
                setWidgetProperty(CabbageIdentifierIds::startpos, strTokens[0].trim().getFloatValue());
                if(strTokens.size()>1)
                    setWidgetProperty(CabbageIdentifierIds::endpos, strTokens[1].trim().getFloatValue());
                else
                    setWidgetProperty(CabbageIdentifierIds::endpos, -1);
            }

            else if(identArray[indx].equalsIgnoreCase("wrap"))
            {
                setWidgetProperty(CabbageIdentifierIds::wrap, strTokens[0].trim().getFloatValue());
            }

            else if(identArray[indx].equalsIgnoreCase("readonly"))
            {
                setWidgetProperty(CabbageIdentifierIds::readonly, strTokens[0].trim().getFloatValue());
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
                        tableNumbers.add(strTokens[i].trim().getFloatValue());
                        value.append(strTokens[i].trim().getFloatValue());
                        tableConfig.append(strTokens[i].trim().getFloatValue());
                        //Logger::writeToLog(strTokens[i].trim());
                    }
                }

                setWidgetProperty(CabbageIdentifierIds::tableconfig, tableConfig);
                setWidgetProperty(CabbageIdentifierIds::tablenumber, value);
            }
            else if(identArray[indx].equalsIgnoreCase("popup"))
            {
                setWidgetProperty(CabbageIdentifierIds::popup, strTokens[0].trim().getIntValue());
            }

            else if(identArray[indx].equalsIgnoreCase("resize"))
            {
                setWidgetProperty(CabbageIdentifierIds::popup, strTokens[0].trim().getIntValue());
            }

            else if(identArray[indx].equalsIgnoreCase("resize"))
            {
                setWidgetProperty(CabbageIdentifierIds::popup, strTokens[0].trim().getIntValue());
            }

            else if(identArray[indx].equalsIgnoreCase("rescale"))
            {
                setWidgetProperty(CabbageIdentifierIds::scalex, strTokens[0].trim().getFloatValue());
                setWidgetProperty(CabbageIdentifierIds::scaley, strTokens[1].trim().getFloatValue());
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
                setWidgetProperty(CabbageIdentifierIds::resizemode, value);
            }

            else if(identArray[indx].equalsIgnoreCase("amprange"))
            {
                if(strTokens.size()<3)
                    warningMessages+="Not enough paramters passed to amprange(): usage amprange(min, max, tablenumber, quantise\")\n";

                var value;
                var temp = getWidgetPropertyWithDefault(CabbageIdentifierIds::amprange, "");

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

                setWidgetProperty(CabbageIdentifierIds::amprange, value);
                //identifiers that appear more than once need to use indx-- so we can check for another instance
                indx--;
            }

            else if(identArray[indx].equalsIgnoreCase("outlinethickness"))
            {
                //setWidgetProperty("line", strTokens[0].trim().getFloatValue());
                setWidgetProperty(CabbageIdentifierIds::outlinethickness, strTokens[0].trim().getFloatValue());
            }

            else if(identArray[indx].equalsIgnoreCase("linethickness"))
            {
                //setWidgetProperty("line", strTokens[0].trim().getFloatValue());
                setWidgetProperty(CabbageIdentifierIds::linethickness, strTokens[0].trim().getFloatValue());
            }

            else if(identArray[indx].equalsIgnoreCase("trackerthickness"))
            {
                //setWidgetProperty("line", strTokens[0].trim().getFloatValue());
                setWidgetProperty(CabbageIdentifierIds::trackerthickness, strTokens[0].trim().getFloatValue());
            }

            else if(identArray[indx].equalsIgnoreCase("value"))
            {
                setWidgetProperty(CabbageIdentifierIds::value, strTokens[0].trim().getFloatValue());
            }

            else if(identArray[indx].equalsIgnoreCase("scrollbars"))
            {
                setWidgetProperty(CabbageIdentifierIds::scrollbars, strTokens[0].trim().getIntValue());
            }

            //============================================
            //sample identifiers for stepper class
            //============================================
            else if(identArray[indx].equalsIgnoreCase("numberofsteps"))
            {
                setWidgetProperty("numberofsteps", strTokens[0].trim().getFloatValue());
            }

            else if(identArray[indx].equalsIgnoreCase("stepbpm"))
            {
                setWidgetProperty("stepbpm", strTokens[0].trim().getIntValue());
            }


            strTokens.clear();
        }
    }
}
//=========================================================================
//retrieve numerical attributes
float CabbageWidget::getNumProp(Identifier prop)
{
    var props = getWidgetPropertyWithDefault(prop, -9999);
    if(props.size()>0)
        return props[0];
    else
        return getWidgetPropertyWithDefault(prop, -9999);
}

//================================================================================================
// these mthods can be used to find the values of indentifiers
// by passing a single line of text to them.
//================================================================================================
Rectangle<int> CabbageWidget::getBoundsFromText(String text)
{
    text = " "+text;
    String subString = text.substring(text.indexOfWholeWord("bounds"));
    subString = subString.substring(7, subString.indexOf(")"));
    StringArray strTokens;
    strTokens.addTokens(subString, ",()", "");
    return Rectangle<int>(strTokens[0].getIntValue(), strTokens[1].getIntValue(), strTokens[2].getIntValue(), strTokens[3].getIntValue());
}

//===================================================================
Colour CabbageWidget::getColourFromText(String text)
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
String CabbageWidget::getTextFromText(String text)
{
    text = " "+text;
    String subString = text.substring(text.indexOfWholeWord("text"));
    subString = subString.substring(5, subString.indexOf(")"));
    return subString;
}
//==================================================================
var CabbageWidget::getVarArrayFromText(String text)
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
Point<int> CabbageWidget::getPosFromText(String text)
{
    text = " "+text;
    String subString = text.substring(text.indexOfWholeWord("pos"));
    subString = subString.substring(4, subString.indexOf(")"));
    StringArray strTokens;
    strTokens.addTokens(subString, ",()", "");
    return Point<int>(strTokens[0].getIntValue(), strTokens[1].getIntValue());
}
//==================================================================
Point<int> CabbageWidget::getSizeFromText(String text)
{
    text = " "+text;
    String subString = text.substring(text.indexOfWholeWord("size"));
    subString = subString.substring(5, subString.indexOf(")"));
    StringArray strTokens;
    strTokens.addTokens(subString, ",()", "");
    return Point<int>(strTokens[0].getIntValue(), strTokens[1].getIntValue());
}
//==================================================================
float CabbageWidget::getSkewFromText(String text)
{
    text = " "+text;
    String subString = text.substring(text.indexOfWholeWord("sliderskew"));
    subString = subString.substring(11, subString.indexOf(")"));
    return subString.getFloatValue();
}

//===================================================================
//set numerical attributes
void CabbageWidget::setNumProp(Identifier prop, float val)
{
    setWidgetProperty(prop, val);
}
//===================================================================
float CabbageWidget::getTableChannelValues(int index)
{
    if(index<tableChannelValues.size())
        return tableChannelValues.getReference(index);
    else
        return 0.f;
}
//===================================================================
void CabbageWidget::addTableChannelValues()
{
    tableChannelValues.add(-1.f);
}
//===================================================================
void CabbageWidget::setTableChannelValues(int index, float val)
{
    if(index<tableChannelValues.size())
        tableChannelValues.set(index, val);
}
//===================================================================
String CabbageWidget::getStringProp(Identifier prop, int index)
{
    jassert(1);
    /*
    		if(prop.equalsIgnoreCase("channel"))
    			return channels[index].trim();
    		else if(prop.equalsIgnoreCase("snapshotData"))
    			return snapshotData[index].trim();
    		else*/
    return String::empty;
}
//===================================================================
Rectangle<int> CabbageWidget::getComponentBounds()
{
    Rectangle<int> rect;
    rect.setBounds(getNumProp("left"), getNumProp("top"), getNumProp("width"), getNumProp("height"));
    return rect;
}
//===================================================================
String CabbageWidget::getStringProp(Identifier prop)
{
    var strings = getWidgetPropertyWithDefault(prop, "");


    if(strings.size()>0)
    {
        //Logger::writeToLog(prop+":"+strings[0].toString());
        return strings[0].toString();
    }
    else
    {
        //Logger::writeToLog(prop+":"+strings.toString());
        //Logger::writeToLog(prop+":"+strings.toString());
        return strings.toString();
    }
    return String::empty;
}
//===================================================================
StringArray CabbageWidget::getStringArrayProp(Identifier prop)
{
    StringArray returnArray;
    var strings = getWidgetPropertyWithDefault(prop, "");
    for(int i=0; i<strings.size(); i++)
        returnArray.add(strings[i].toString());
    return returnArray;
}
//===================================================================
String CabbageWidget::getStringArrayPropValue(Identifier prop, int index)
{
    StringArray returnArray;
    var strings = getWidgetPropertyWithDefault(prop, "");
    //Logger::writeToLog(String(strings.size()));
    for(int i=0; i<strings.size(); i++)
        returnArray.add(strings[i].toString());
    if(isPositiveAndBelow(index,strings.size()))
        return returnArray[index];
    else
        return returnArray[strings.size()-1];
}
//===================================================================
void CabbageWidget::setStringArrayProp(Identifier prop, var value)
{
    this->removeProperty(prop, 0);
    setWidgetProperty(prop, value);
}
//===================================================================
void CabbageWidget::setStringArrayPropValue(Identifier prop, int index, String value)
{
    var strings = getWidgetPropertyWithDefault(prop, "");
    if(strings.size()>0)
        if(isPositiveAndBelow(index,strings.size()))
        {
            strings.getArray()->set(index, value);
            setWidgetProperty(prop, strings);
        }

}
//===================================================================
Array<int> CabbageWidget::getIntArrayProp(Identifier prop)
{
    Array<int> returnArray;
    var ints = getWidgetPropertyWithDefault(prop, "");
    for(int i=0; i<ints.size(); i++)
        returnArray.add(ints[i]);
    return returnArray;
}
//===================================================================
var CabbageWidget::getVarArrayProp(Identifier prop)
{
    return getWidgetPropertyWithDefault(prop, "");
}
//===================================================================
int CabbageWidget::getIntArrayPropValue(Identifier prop, int index)
{
    Array<int> returnArray;
    var ints = getWidgetPropertyWithDefault(prop, "");
    for(int i=0; i<ints.size(); i++)
        returnArray.add(ints[i]);
    if(isPositiveAndBelow(index,ints.size()))
        return returnArray[index];
    else
        return returnArray[ints.size()-1];

    return 0;
}
//===================================================================
Array<float> CabbageWidget::getFloatArrayProp(Identifier prop)
{
    Array<float> returnArray;
    var ints = getWidgetPropertyWithDefault(prop, "");
    for(int i=0; i<ints.size(); i++)
        returnArray.add(ints[i]);
    return returnArray;
}
//===================================================================
float CabbageWidget::getFloatArrayPropValue(Identifier prop, int index)
{
    Array<float> returnArray;
    var ints = getWidgetPropertyWithDefault(prop, "");
    for(int i=0; i<ints.size(); i++)
        returnArray.add(ints[i]);
    if(isPositiveAndBelow(index,ints.size()))
        return returnArray[index];
    else
        return returnArray[ints.size()-1];
}
//===================================================================
void CabbageWidget::setStringProp(Identifier prop, int index, String value)
{

}
//===================================================================
void CabbageWidget::scaleWidget(Point<float> scale)
{
    Rectangle<int> rect(getWidgetPropertyWithDefault("left", 0),
                        getWidgetPropertyWithDefault("top", 0),
                        getWidgetPropertyWithDefault("width", 10),
                        getWidgetPropertyWithDefault("height", 10));

    setWidgetProperty(CabbageIdentifierIds::left, rect.getX()*scale.x);
    setWidgetProperty(CabbageIdentifierIds::top, rect.getY()*scale.y);
    setWidgetProperty(CabbageIdentifierIds::width, rect.getWidth()*scale.x);
    setWidgetProperty(CabbageIdentifierIds::height, rect.getHeight()*scale.y);	;
}
//===================================================================
void CabbageWidget::setStringProp(Identifier prop, String val)
{
//	CabbageUtilities::debug(prop.toString());
//	CabbageUtilities::debug(val);

    setWidgetProperty(prop, val);
}
//===================================================================
String CabbageWidget::getColourProp(Identifier prop)
{

    return String::empty;
}
//===================================================================
String CabbageWidget::getStringForIdentifier(var propsArray, String identifier, String type)
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
String CabbageWidget::getCabbageCodeFromIdentifiers(NamedValueSet props)
{
    //Logger::writeToLog("::::getCabbageCodeFromIdentifiers::::");
    String line, widgetType;
    var propsArray;
    String temp="";
    String type;
    String colour, fontcolour;
    String left("0"), top("0"), width("100"), height("50"), colR, colG, colB, colA, min("0"), max("1"), skew("1"), drawmode, tablenumber,resizemode,
           incr("0.001"), slidervalue, value, maxx, maxy, minx, miny, valuex, valuey, channel, xchannel, ychannel,
           amprange;
    var rgbVals;
    //run through the complete list of identifiers and find type
    for(int i=0; i<props.size(); i++)
        if(props.getName(i).toString().equalsIgnoreCase(CabbageIdentifierIds::type))
            widgetType = props.getValueAt(i).toString();

    //get default values for this type of widget
    CabbageWidget cAttr(widgetType, -99);

    //run through the complete list of identifiersand create code
    for(int i=0; i<props.size(); i++)
    {
        String identifier = props.getName(i).toString();
        propsArray = props.getValueAt(i);



        if((props.getValueAt(i).toString()!=""))
        {
            //first check to make sure no default values used
            if(props.getValueAt(i).isString() &&
                    props.getValueAt(i).toString() == cAttr.getStringProp(identifier))
            {
                //CabbageUtilities::debug(identifier, cAttr.getStringProp(identifier));
            }
            else if(props.getValueAt(i).isDouble() &&
                    double(props.getValueAt(i)) == cAttr.getNumProp(identifier))
            {
                //CabbageUtilities::debug(identifier, cAttr.getNumProp(identifier));
            }
            else if(props.getValueAt(i).isInt() &&
                    int(props.getValueAt(i)) == cAttr.getNumProp(identifier))
            {
                //CabbageUtilities::debug(identifier, cAttr.getNumProp(identifier));
            }
            else if(props.getValueAt(i).isUndefined())
            {
                //CabbageUtilities::debug("Undefined value");
            }
            else if(props.getName(i).toString()=="top")
                top = props.getValueAt(i).toString();
            else if(props.getName(i).toString()=="left")
                left = props.getValueAt(i).toString();
            else if(props.getName(i).toString()=="width")
                width = props.getValueAt(i).toString();
            else if(props.getName(i).toString()=="height")
                height = props.getValueAt(i).toString();
            else if(identifier=="decimalplaces" ||
                    identifier=="range" ||
                    identifier=="name" ||
                    identifier=="scalex" ||
                    identifier=="scaley" ||
                    identifier=="mode" ||
                    identifier=="basetype" ||
                    identifier=="kind" ||
                    identifier=="visible" ||
                    identifier=="trackerthickness" ||
                    identifier=="xyautoindex" ||
                    identifier=="comborange")
            {
                //none of these identifiers need to be seen...
            }
            else if(props.getName(i).toString()=="colour" || props.getName(i).toString()=="fontcolour"
                    || props.getName(i).toString()=="trackercolour" || props.getName(i).toString()=="textcolour" ||
                    props.getName(i).toString()=="outlinecolour")
            {
                if(propsArray.size()>0)
                {
                    for(int y=0; y<propsArray.size(); y++)
                    {
                        rgbVals.append(propsArray[y]);
                    }
                    colour = colour << props.getName(i).toString() <<"("<<rgbVals[0].toString() << ", " << rgbVals[1].toString() << ", " << rgbVals[2].toString() << ", " << rgbVals[3].toString() << "), ";
                }
                else
                {
                    Colour col = Colour::fromString(props.getValueAt(i).toString());
                    colour = colour << props.getName(i).toString() << "(" << (float)col.getRed() << ", " << (float)col.getGreen() << ", " << (float)col.getBlue() << ", " << (float)col.getAlpha() << "), ";
                }
                rgbVals.resize(0);
            }

            else if((identifier=="popup") && (int(props.getValueAt(i))==0))
            {
                //only show if not set to default
            }
            else if((identifier=="midictrl") && (int(props.getValueAt(i))==-99))
            {
                //only show if not set to default
            }
            else if((identifier=="midichan") && (int(props.getValueAt(i))==-99))
            {
                //only show if not set to default
            }
            else if((identifier=="channel"))
            {
                channel = getStringForIdentifier(propsArray, "channel(", "string");
            }

            else if(identifier=="drawmode")
            {
                drawmode = getStringForIdentifier(propsArray, "drawmode(", "number");
            }

            else if((identifier=="amprange"))
            {
                amprange = getStringForIdentifier(propsArray, "amprange(", "number");
            }

            else if((identifier=="tablenumber"))
            {
                tablenumber = getStringForIdentifier(propsArray, "tablenumber(", "number");
            }

            else if((identifier=="resizemode"))
            {
                resizemode = getStringForIdentifier(propsArray, "resizemode(", "number");
            }

            else if((identifier=="xchannel"))
            {
                xchannel = props.getValueAt(i).toString();
            }
            else if((identifier=="ychannel"))
            {
                ychannel = props.getValueAt(i).toString();
            }


            //grab slider settings..
            else if(identifier=="min")
                min = props.getValueAt(i).toString();
            else if(identifier=="max")
                max = props.getValueAt(i).toString();
            else if(identifier=="value")
            {
                slidervalue = props.getValueAt(i).toString();
                value = "value("+slidervalue+"), ";
            }
            else if(identifier=="sliderskew")
                skew = props.getValueAt(i).toString();
            else if(identifier=="sliderincr")
                incr = String(CabbageUtilities::roundToPrec(props.getValueAt(i), 5));

            //grab xypad x-ranges...
            else if(identifier=="minx")
                minx = props.getValueAt(i).toString();
            else if(identifier=="maxx")
                maxx = props.getValueAt(i).toString();
            else if(identifier=="valuex")
                valuex = props.getValueAt(i).toString();

            //grab xypad y-ranges...
            else if(identifier=="miny")
                miny = props.getValueAt(i).toString();
            else if(identifier=="maxy")
                maxy = props.getValueAt(i).toString();
            else if(identifier=="valuey")
                valuey = props.getValueAt(i).toString();

            else if((identifier=="textbox") && (int(props.getValueAt(i))==0))
            {
                //only show if not set to default
            }
            else
            {
                //catch all other identifiers....
                line = line + identifier+String("(");

                if(propsArray.size()>0)
                    for(int y=0; y<propsArray.size(); y++)
                    {
                        if(propsArray[y].isString())
                            temp = temp+"\""+propsArray[y].toString()+"\", ";
                        else
                            temp = temp+propsArray[y].toString()+", ";
                    }
                else if(props.getValueAt(i).isString())
                    temp = temp+"\""+props.getValueAt(i).toString()+"\"";
                else
                    temp = temp+props.getValueAt(i).toString();

                line = line + temp + "), ";
                temp = "";
            }
        }
    }

    String completeLine;
    String range = "range("+min+", "+max+", "+slidervalue+", "+skew+", "+incr+"), ";
    String rangex = "rangex("+minx+", "+maxx+", "+valuex+"), ";
    String rangey = "rangey("+miny+", "+maxy+", "+valuey+"), ";
    String xypadChannels = "channel(\""+xchannel+"\", \""+ychannel+"\"), ";
    if(widgetType.contains("slider") || widgetType=="numberbox")
        completeLine = widgetType+" bounds("+left+", "+top+", "+width+", "+height+"), "+channel+range+line.replace(", )", ")")+ " "+colour;
    else if(widgetType=="xypad")
        completeLine = widgetType+" bounds("+left+", "+top+", "+width+", "+height+"), "+xypadChannels+rangex+rangey+line.replace(", )", ")")+ " "+colour;
    else
        completeLine = widgetType+" bounds("+left+", "+top+", "+width+", "+height+"), "+channel+line.replace(", )", ")")+ " "+tablenumber+drawmode+amprange+colour+resizemode+value;
    return completeLine;

}
