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
    warningMessages("")
{
//Default values are assigned to all attributres
//before parsing begins
    cabbageIdentifiers.set("scalex", 1);
    cabbageIdentifiers.set("scaley", 1);
    cabbageIdentifiers.set("resize", 0);
    cabbageIdentifiers.set(CabbageIdentifierIds::active, 1);
    cabbageIdentifiers.set(CabbageIdentifierIds::parentdir, "");
    cabbageIdentifiers.set(CabbageIdentifierIds::svgdebug, 0);

    StringArray strTokens;
    strTokens.addTokens(compStr, " ", "\"");

    cabbageIdentifiers.set(CabbageIdentifierIds::channelarray, "");
    //===============sliders==================//
    if(strTokens[0].trim() == "hslider" || strTokens[0].trim() == "hslider2" || strTokens[0].trim() == "hslider3")
    {
        cabbageIdentifiers.set(CabbageIdentifierIds::basetype, "interactive");
        cabbageIdentifiers.set(CabbageIdentifierIds::top, 10);
        cabbageIdentifiers.set(CabbageIdentifierIds::left, 10);
        cabbageIdentifiers.set(CabbageIdentifierIds::width, 150);
        cabbageIdentifiers.set(CabbageIdentifierIds::height, 50);
        var channels;
        channels.append("hslider");
        cabbageIdentifiers.set(CabbageIdentifierIds::channel, channels);
        cabbageIdentifiers.set(CabbageIdentifierIds::min, 0);
        cabbageIdentifiers.set(CabbageIdentifierIds::max, 1);
        cabbageIdentifiers.set(CabbageIdentifierIds::value, 0);
        cabbageIdentifiers.set(CabbageIdentifierIds::text, "");
        cabbageIdentifiers.set(CabbageIdentifierIds::textbox, 0.f);
        cabbageIdentifiers.set(CabbageIdentifierIds::caption, "");
        cabbageIdentifiers.set(CabbageIdentifierIds::colour, Colours::white.toString());
        cabbageIdentifiers.set(CabbageIdentifierIds::trackercolour, Colour(0, 118, 38).toString());
        cabbageIdentifiers.set(CabbageIdentifierIds::fontcolour, CabbageUtilities::getComponentFontColour().toString());
        cabbageIdentifiers.set(CabbageIdentifierIds::textcolour, CabbageUtilities::getComponentFontColour().toString());
        cabbageIdentifiers.set(CabbageIdentifierIds::sliderskew, 1);
        cabbageIdentifiers.set(CabbageIdentifierIds::sliderincr, .001);
        cabbageIdentifiers.set(CabbageIdentifierIds::velocity, 0);

        cabbageIdentifiers.set(CabbageIdentifierIds::midichan, -99);
        cabbageIdentifiers.set(CabbageIdentifierIds::midictrl, -99);
        //these don't appear in the props dialog
        cabbageIdentifiers.set(CabbageIdentifierIds::name, "hslider");
        cabbageIdentifiers.set(CabbageIdentifierIds::type, cabbageIdentifiers.getWithDefault("name", "").toString());
        cabbageIdentifiers.set(CabbageIdentifierIds::name, cabbageIdentifiers.getWithDefault("name", "").toString()+String(ID));
        if(strTokens[0].trim() == "hslider2")
            cabbageIdentifiers.set(CabbageIdentifierIds::kind, "horizontal2");
        else if(strTokens[0].trim() == "hslider3")
            cabbageIdentifiers.set(CabbageIdentifierIds::kind, "horizontal3");
        else
            cabbageIdentifiers.set(CabbageIdentifierIds::kind, "horizontal");
        cabbageIdentifiers.set(CabbageIdentifierIds::decimalplaces, 1);
        cabbageIdentifiers.set(CabbageIdentifierIds::identchannel, "");
        cabbageIdentifiers.set(CabbageIdentifierIds::trackerthickness, .1);
        cabbageIdentifiers.set(CabbageIdentifierIds::visible, 1);
        cabbageIdentifiers.set(CabbageIdentifierIds::gradient, 1);
        cabbageIdentifiers.set(CabbageIdentifierIds::svgslider, "");
        cabbageIdentifiers.set(CabbageIdentifierIds::svgsliderbg, "");


    }
    else if(strTokens[0].trim() == "socketsend" || strTokens[0].trim() == "socketreceive")
    {
        cabbageIdentifiers.set(CabbageIdentifierIds::basetype, "layout");
        cabbageIdentifiers.set(CabbageIdentifierIds::socketaddress, "");
        cabbageIdentifiers.set(CabbageIdentifierIds::socketport, 0);
    }
    else if(strTokens[0].trim() == "vslider" || strTokens[0].trim() == "vslider2" ||  strTokens[0].trim() == "vslider3")
    {
        cabbageIdentifiers.set(CabbageIdentifierIds::basetype, "interactive");
        cabbageIdentifiers.set(CabbageIdentifierIds::top, 10);
        cabbageIdentifiers.set(CabbageIdentifierIds::left, 10);
        cabbageIdentifiers.set(CabbageIdentifierIds::width, 50);
        cabbageIdentifiers.set(CabbageIdentifierIds::height, 150);
        var channels;
        channels.append("vslider");
        cabbageIdentifiers.set(CabbageIdentifierIds::channel, channels);
        cabbageIdentifiers.set(CabbageIdentifierIds::min, 0);
        cabbageIdentifiers.set(CabbageIdentifierIds::max, 1);
        cabbageIdentifiers.set(CabbageIdentifierIds::value, 0);
        cabbageIdentifiers.set(CabbageIdentifierIds::text, "");
        cabbageIdentifiers.set(CabbageIdentifierIds::textbox, 0.f);
        cabbageIdentifiers.set(CabbageIdentifierIds::caption, "");
        cabbageIdentifiers.set(CabbageIdentifierIds::colour, Colours::white.toString());
        cabbageIdentifiers.set(CabbageIdentifierIds::trackercolour, Colour(0, 118, 38).toString());
        cabbageIdentifiers.set(CabbageIdentifierIds::fontcolour, CabbageUtilities::getComponentFontColour().toString());
        cabbageIdentifiers.set(CabbageIdentifierIds::textcolour, CabbageUtilities::getComponentFontColour().toString());
        cabbageIdentifiers.set(CabbageIdentifierIds::sliderskew, 1);
        cabbageIdentifiers.set(CabbageIdentifierIds::sliderincr, .001);
        cabbageIdentifiers.set(CabbageIdentifierIds::velocity, 0);
        cabbageIdentifiers.set(CabbageIdentifierIds::midichan, -99);
        cabbageIdentifiers.set(CabbageIdentifierIds::midictrl, -99);
        //these don't appear in the props dialog
        cabbageIdentifiers.set(CabbageIdentifierIds::name, "vslider");
        cabbageIdentifiers.set(CabbageIdentifierIds::type, cabbageIdentifiers.getWithDefault("name", "").toString());
        cabbageIdentifiers.set(CabbageIdentifierIds::name, cabbageIdentifiers.getWithDefault("name", "").toString()+String(ID));
        if(strTokens[0].trim() == "vslider2")
            cabbageIdentifiers.set(CabbageIdentifierIds::kind, "vertical2");
        else if(strTokens[0].trim() == "vslider3")
            cabbageIdentifiers.set(CabbageIdentifierIds::kind, "vertical3");
        else
            cabbageIdentifiers.set(CabbageIdentifierIds::kind, "vertical");

        cabbageIdentifiers.set(CabbageIdentifierIds::decimalplaces, 1);
        cabbageIdentifiers.set(CabbageIdentifierIds::trackerthickness, .1);
        cabbageIdentifiers.set(CabbageIdentifierIds::identchannel, "");
        cabbageIdentifiers.set(CabbageIdentifierIds::visible, 1);
        cabbageIdentifiers.set(CabbageIdentifierIds::svgslider, "");
        cabbageIdentifiers.set(CabbageIdentifierIds::svgsliderbg, "");
    }
    else if(strTokens[0].trim() == "rslider")
    {
        cabbageIdentifiers.set(CabbageIdentifierIds::basetype, "interactive");
        cabbageIdentifiers.set(CabbageIdentifierIds::top, 10);
        cabbageIdentifiers.set(CabbageIdentifierIds::left, 10);
        cabbageIdentifiers.set(CabbageIdentifierIds::width, 60);
        cabbageIdentifiers.set(CabbageIdentifierIds::height, 60);
        var channels;
        channels.append("rslider");
        cabbageIdentifiers.set(CabbageIdentifierIds::channel, channels);
        cabbageIdentifiers.set(CabbageIdentifierIds::min, 0);
        cabbageIdentifiers.set(CabbageIdentifierIds::max, 1);
        cabbageIdentifiers.set(CabbageIdentifierIds::value, 0);
        cabbageIdentifiers.set(CabbageIdentifierIds::sliderskew, 1);
        cabbageIdentifiers.set(CabbageIdentifierIds::sliderincr, .001);
        cabbageIdentifiers.set(CabbageIdentifierIds::text, "");
        cabbageIdentifiers.set(CabbageIdentifierIds::textbox, 0.f);
        cabbageIdentifiers.set(CabbageIdentifierIds::caption, "");
        cabbageIdentifiers.set(CabbageIdentifierIds::colour, Colours::whitesmoke.toString());
        cabbageIdentifiers.set(CabbageIdentifierIds::trackercolour, Colour(0, 118, 38).toString());
        cabbageIdentifiers.set(CabbageIdentifierIds::fontcolour, CabbageUtilities::getComponentFontColour().toString());
        cabbageIdentifiers.set(CabbageIdentifierIds::textcolour, CabbageUtilities::getComponentFontColour().toString());
        cabbageIdentifiers.set(CabbageIdentifierIds::outlinecolour, Colours::black.brighter(.3f).toString());
        cabbageIdentifiers.set(CabbageIdentifierIds::midichan, -99);
        cabbageIdentifiers.set(CabbageIdentifierIds::midictrl, -99);
        //these don't appear in the props dialog
        cabbageIdentifiers.set(CabbageIdentifierIds::name, "rslider");
        cabbageIdentifiers.set(CabbageIdentifierIds::type, cabbageIdentifiers.getWithDefault("name", "").toString());
        cabbageIdentifiers.set(CabbageIdentifierIds::name, cabbageIdentifiers.getWithDefault("name", "").toString()+String(ID));
        cabbageIdentifiers.set(CabbageIdentifierIds::kind, "rotary");
        cabbageIdentifiers.set(CabbageIdentifierIds::decimalplaces, 1);
        cabbageIdentifiers.set(CabbageIdentifierIds::velocity, 0);
        cabbageIdentifiers.set(CabbageIdentifierIds::identchannel, "");
        cabbageIdentifiers.set(CabbageIdentifierIds::trackerthickness, .7);
        cabbageIdentifiers.set(CabbageIdentifierIds::visible, 1);
        cabbageIdentifiers.set(CabbageIdentifierIds::svgslider, "");
        cabbageIdentifiers.set(CabbageIdentifierIds::svgsliderbg, "");
    }

    else if((strTokens[0].trim() == "sourcebutton")||(strTokens[0].trim() == "loadbutton"))
    {
        top = 10;
        left = 10;
        width = 600;
        height = 440;
        //name.append(String(ID), 1024);
        cabbageIdentifiers.set(CabbageIdentifierIds::top, 10);
        cabbageIdentifiers.set(CabbageIdentifierIds::left, 10);
        cabbageIdentifiers.set(CabbageIdentifierIds::width, 80);
        cabbageIdentifiers.set(CabbageIdentifierIds::height, 40);
        cabbageIdentifiers.set(CabbageIdentifierIds::text, "");
        cabbageIdentifiers.set(CabbageIdentifierIds::caption, "");
        cabbageIdentifiers.set(CabbageIdentifierIds::colour, Colours::black.toString());
        cabbageIdentifiers.set(CabbageIdentifierIds::fontcolour, Colours::white.toString());
        cabbageIdentifiers.set(CabbageIdentifierIds::oncolour, Colours::black.toString());
        cabbageIdentifiers.set(CabbageIdentifierIds::onfontcolour, Colours::white.toString());
        cabbageIdentifiers.set(CabbageIdentifierIds::type, strTokens[0].trim());
        cabbageIdentifiers.set(CabbageIdentifierIds::name, strTokens[0].trim());
        cabbageIdentifiers.set(CabbageIdentifierIds::name, cabbageIdentifiers.getWithDefault("name", "").toString()+String(ID));
        cabbageIdentifiers.set(CabbageIdentifierIds::identchannel, "");
        cabbageIdentifiers.set(CabbageIdentifierIds::visible, 1);
        cabbageIdentifiers.set(CabbageIdentifierIds::svgbuttonon, "");
        cabbageIdentifiers.set(CabbageIdentifierIds::svgbuttonoff, "");
    }

    //===============info button==================//
    else if(strTokens[0].trim() == "infobutton")
    {
        cabbageIdentifiers.set("basetype", "layout");
        top = 10;
        left = 10;
        width = 600;
        height = 440;
        //name.append(String(ID), 1024);
        cabbageIdentifiers.set(CabbageIdentifierIds::top, 10);
        cabbageIdentifiers.set(CabbageIdentifierIds::left, 10);
        cabbageIdentifiers.set(CabbageIdentifierIds::width, 80);
        cabbageIdentifiers.set(CabbageIdentifierIds::height, 40);
        cabbageIdentifiers.set(CabbageIdentifierIds::text, "");
        cabbageIdentifiers.set(CabbageIdentifierIds::caption, "");
        cabbageIdentifiers.set(CabbageIdentifierIds::colour, Colours::black.toString());
        cabbageIdentifiers.set(CabbageIdentifierIds::fontcolour, Colours::white.toString());
        cabbageIdentifiers.set(CabbageIdentifierIds::oncolour, Colours::black.toString());
        cabbageIdentifiers.set(CabbageIdentifierIds::onfontcolour, Colours::white.toString());
        cabbageIdentifiers.set(CabbageIdentifierIds::type, "infobutton");
        cabbageIdentifiers.set(CabbageIdentifierIds::name, "infobutton");
        cabbageIdentifiers.set(CabbageIdentifierIds::name, cabbageIdentifiers.getWithDefault("name", "").toString()+String(ID));
        cabbageIdentifiers.set(CabbageIdentifierIds::identchannel, "");
        cabbageIdentifiers.set(CabbageIdentifierIds::visible, 1);
        cabbageIdentifiers.set(CabbageIdentifierIds::svgbuttonon, "");
        cabbageIdentifiers.set(CabbageIdentifierIds::svgbuttonoff, "");
    }

    //===============directory list==================//
    else if(strTokens[0].trim() == "directorylist")
    {
        cabbageIdentifiers.set("basetype", "layout");
        /*
                  top = 10;
                  left = 10;
                  width = 600;
                  height = 440;
                  channel = "";
                  name = "directorylist";
        		  caption = "";
        		  type = name;
                  //name.append(String(ID), 1024);

        	 */
    }


    //===============record button==================//
    else if(strTokens[0].trim() == "recordbutton")
    {
        cabbageIdentifiers.set("basetype", "layout");
        top = 10;
        left = 10;
        width = 80;
        height = 40;
        var array;
        array.append("Record");
        array.append("Record");


        cabbageIdentifiers.set(CabbageIdentifierIds::top, 10);
        cabbageIdentifiers.set(CabbageIdentifierIds::left, 10);
        cabbageIdentifiers.set(CabbageIdentifierIds::width, 80);
        cabbageIdentifiers.set(CabbageIdentifierIds::height, 40);
        var channels;
        channels.append("recordbutton");
        cabbageIdentifiers.set(CabbageIdentifierIds::channel, channels);
        cabbageIdentifiers.set(CabbageIdentifierIds::value, 1);
        cabbageIdentifiers.set(CabbageIdentifierIds::text, array);
        cabbageIdentifiers.set(CabbageIdentifierIds::caption, "");
        cabbageIdentifiers.set(CabbageIdentifierIds::colour, Colours::black.toString());
        cabbageIdentifiers.set(CabbageIdentifierIds::fontcolour, Colours::white.toString());
        cabbageIdentifiers.set(CabbageIdentifierIds::oncolour, Colours::black.toString());
        cabbageIdentifiers.set(CabbageIdentifierIds::onfontcolour, Colours::white.toString());
        cabbageIdentifiers.set(CabbageIdentifierIds::type, "recordbutton");
        cabbageIdentifiers.set(CabbageIdentifierIds::name, "recordbutton");
        cabbageIdentifiers.set(CabbageIdentifierIds::mode, "file");
        cabbageIdentifiers.set(CabbageIdentifierIds::identchannel, "");
        cabbageIdentifiers.set("name", cabbageIdentifiers.getWithDefault("name", "").toString()+String(ID));
        var populate;
        populate.append("");
        populate.append("");
        cabbageIdentifiers.set("populate", populate);
        cabbageIdentifiers.set(CabbageIdentifierIds::visible, 1);
        cabbageIdentifiers.set(CabbageIdentifierIds::svgbuttonon, "");
        cabbageIdentifiers.set(CabbageIdentifierIds::svgbuttonoff, "");
    }
    //===============file button==================//
    else if(strTokens[0].trim() == "filebutton")
    {
        cabbageIdentifiers.set("basetype", "layout");
        top = 10;
        left = 10;
        width = 80;
        height = 40;
        var array;
        array.append("Open file");
        array.append("Open file");


        cabbageIdentifiers.set(CabbageIdentifierIds::top, 10);
        cabbageIdentifiers.set(CabbageIdentifierIds::left, 10);
        cabbageIdentifiers.set(CabbageIdentifierIds::width, 80);
        cabbageIdentifiers.set(CabbageIdentifierIds::height, 40);
        var channels;
        channels.append("filebutton");
        cabbageIdentifiers.set(CabbageIdentifierIds::channel, channels);
        cabbageIdentifiers.set(CabbageIdentifierIds::value, 1);
        cabbageIdentifiers.set(CabbageIdentifierIds::text, array);
        cabbageIdentifiers.set(CabbageIdentifierIds::caption, "");
        cabbageIdentifiers.set(CabbageIdentifierIds::colour, Colours::black.toString());
        cabbageIdentifiers.set(CabbageIdentifierIds::fontcolour, Colours::white.toString());
        cabbageIdentifiers.set(CabbageIdentifierIds::oncolour, Colours::black.toString());
        cabbageIdentifiers.set(CabbageIdentifierIds::onfontcolour, Colours::white.toString());
        cabbageIdentifiers.set(CabbageIdentifierIds::type, "filebutton");
        cabbageIdentifiers.set(CabbageIdentifierIds::name, "filebutton");
        cabbageIdentifiers.set(CabbageIdentifierIds::mode, "file");
        cabbageIdentifiers.set(CabbageIdentifierIds::identchannel, "");
        cabbageIdentifiers.set("name", cabbageIdentifiers.getWithDefault("name", "").toString()+String(ID));
        var populate;
        populate.append("");
        populate.append("");
        cabbageIdentifiers.set("populate", populate);
        cabbageIdentifiers.set(CabbageIdentifierIds::visible, 1);
        cabbageIdentifiers.set(CabbageIdentifierIds::filetype, "*");
        cabbageIdentifiers.set(CabbageIdentifierIds::svgbuttonon, "");
        cabbageIdentifiers.set(CabbageIdentifierIds::svgbuttonoff, "");
    }

    //===============button==================//
    else if(strTokens[0].trim() == "button")
    {
        cabbageIdentifiers.set("basetype", "interactive");
        top = 10;
        left = 10;
        width = 80;
        height = 40;
        var array;
        array.append("Push me");
        array.append("Push me");

        cabbageIdentifiers.set(CabbageIdentifierIds::top, 10);
        cabbageIdentifiers.set(CabbageIdentifierIds::left, 10);
        cabbageIdentifiers.set(CabbageIdentifierIds::width, 80);
        cabbageIdentifiers.set(CabbageIdentifierIds::height, 40);
        var channels;
        channels.append("buttonchan");
        cabbageIdentifiers.set(CabbageIdentifierIds::channel, channels);
        cabbageIdentifiers.set(CabbageIdentifierIds::value, 0);
        cabbageIdentifiers.set(CabbageIdentifierIds::text, array);
        cabbageIdentifiers.set(CabbageIdentifierIds::caption, "");
        cabbageIdentifiers.set(CabbageIdentifierIds::colour, Colours::black.toString());
        cabbageIdentifiers.set(CabbageIdentifierIds::fontcolour, Colours::white.toString());
        cabbageIdentifiers.set(CabbageIdentifierIds::oncolour, Colours::black.toString());
        cabbageIdentifiers.set(CabbageIdentifierIds::onfontcolour, Colours::white.toString());
        cabbageIdentifiers.set(CabbageIdentifierIds::latched, 1);
        cabbageIdentifiers.set(CabbageIdentifierIds::type, "button");
        cabbageIdentifiers.set(CabbageIdentifierIds::name, "button");
        cabbageIdentifiers.set(CabbageIdentifierIds::name, cabbageIdentifiers.getWithDefault("name", "").toString()+String(ID));
        cabbageIdentifiers.set(CabbageIdentifierIds::identchannel, "");
        cabbageIdentifiers.set(CabbageIdentifierIds::radiogroup, 0);
        cabbageIdentifiers.set(CabbageIdentifierIds::visible, 1);
        cabbageIdentifiers.set(CabbageIdentifierIds::svgbuttonon, "");
        cabbageIdentifiers.set(CabbageIdentifierIds::svgbuttonoff, "");

    }

    //===============checkbox==================//
    else if(strTokens[0].trim() == "checkbox")
    {
        cabbageIdentifiers.set("basetype", "interactive");
        cabbageIdentifiers.set(CabbageIdentifierIds::top, 10);
        cabbageIdentifiers.set(CabbageIdentifierIds::left, 10);
        cabbageIdentifiers.set(CabbageIdentifierIds::width, 100);
        cabbageIdentifiers.set(CabbageIdentifierIds::height, 22);
        var channels;
        channels.append("checkchan");
        cabbageIdentifiers.set(CabbageIdentifierIds::channel, channels);
        cabbageIdentifiers.set(CabbageIdentifierIds::text, "");
        cabbageIdentifiers.set(CabbageIdentifierIds::value, 0);
        cabbageIdentifiers.set(CabbageIdentifierIds::caption, "");
        cabbageIdentifiers.set(CabbageIdentifierIds::shape, "square");
        cabbageIdentifiers.set(CabbageIdentifierIds::corners, 2);
        cabbageIdentifiers.set(CabbageIdentifierIds::type, "checkbox");
        cabbageIdentifiers.set(CabbageIdentifierIds::oncolour, Colours::lime.toString());
        cabbageIdentifiers.set(CabbageIdentifierIds::colour, Colours::black.toString());
        cabbageIdentifiers.set(CabbageIdentifierIds::fontcolour, CabbageUtilities::getComponentFontColour().toString());
        cabbageIdentifiers.set(CabbageIdentifierIds::name, "checkbox");
        cabbageIdentifiers.set(CabbageIdentifierIds::name, cabbageIdentifiers.getWithDefault("name", "").toString()+String(ID));
        cabbageIdentifiers.set(CabbageIdentifierIds::identchannel, "");
        cabbageIdentifiers.set(CabbageIdentifierIds::radiogroup, 0);
        cabbageIdentifiers.set(CabbageIdentifierIds::visible, 1);
    }

    //===============numberbox==================//
    else if(strTokens[0].trim() == "numberbox")
    {
        cabbageIdentifiers.set("basetype", "interactive");
        cabbageIdentifiers.set(CabbageIdentifierIds::top, 10);
        cabbageIdentifiers.set(CabbageIdentifierIds::left, 10);
        cabbageIdentifiers.set(CabbageIdentifierIds::width, 100);
        cabbageIdentifiers.set(CabbageIdentifierIds::height, 22);
        var channels;
        channels.append("numberbox");
        cabbageIdentifiers.set(CabbageIdentifierIds::channel, channels);
        cabbageIdentifiers.set(CabbageIdentifierIds::min, 0);
        cabbageIdentifiers.set(CabbageIdentifierIds::max, 100);
        cabbageIdentifiers.set(CabbageIdentifierIds::value, 50);
        cabbageIdentifiers.set(CabbageIdentifierIds::sliderskew, 1);
        cabbageIdentifiers.set(CabbageIdentifierIds::sliderincr, .01);
        cabbageIdentifiers.set(CabbageIdentifierIds::text, "");
        cabbageIdentifiers.set(CabbageIdentifierIds::value, 0);
        cabbageIdentifiers.set(CabbageIdentifierIds::caption, "");
        cabbageIdentifiers.set(CabbageIdentifierIds::type, "numberbox");
        cabbageIdentifiers.set(CabbageIdentifierIds::colour, CabbageUtilities::getDarkerBackgroundSkin().toString());
        cabbageIdentifiers.set(CabbageIdentifierIds::fontcolour, Colours::whitesmoke.toString());
        cabbageIdentifiers.set(CabbageIdentifierIds::textcolour, CabbageUtilities::getComponentFontColour().toString());
        cabbageIdentifiers.set(CabbageIdentifierIds::name, "numberbox");
        cabbageIdentifiers.set(CabbageIdentifierIds::name, cabbageIdentifiers.getWithDefault("name", "").toString()+String(ID));
        cabbageIdentifiers.set(CabbageIdentifierIds::identchannel, "");
        cabbageIdentifiers.set(CabbageIdentifierIds::visible, 1);
        cabbageIdentifiers.set(CabbageIdentifierIds::velocity, 80);
    }
    //===============popupmenu==================//
    else if(strTokens[0].trim() == "popupmenu")
    {
        cabbageIdentifiers.set("basetype", "layout");

        var array;
        array.append("Item 1");
        array.append("Item 2");
        array.append("Item 3");
        array.append("Item 4");

        cabbageIdentifiers.set(CabbageIdentifierIds::top, 10);
        cabbageIdentifiers.set(CabbageIdentifierIds::left, 10);
        cabbageIdentifiers.set(CabbageIdentifierIds::width, 80);
        cabbageIdentifiers.set(CabbageIdentifierIds::height, 20);
        var channels;
        channels.append("popupchan");
        cabbageIdentifiers.set(CabbageIdentifierIds::channel, channels);
        cabbageIdentifiers.set(CabbageIdentifierIds::channeltype, "number");
        cabbageIdentifiers.set(CabbageIdentifierIds::text, array);
        cabbageIdentifiers.set(CabbageIdentifierIds::value, 1);

        cabbageIdentifiers.set(CabbageIdentifierIds::colour, Colours::black.toString());
        cabbageIdentifiers.set(CabbageIdentifierIds::fontcolour, CabbageUtilities::getComponentFontColour().toString());
        cabbageIdentifiers.set(CabbageIdentifierIds::type, "popupmenu");
        cabbageIdentifiers.set(CabbageIdentifierIds::name, "popupmenu");

        var populate;
        populate.append("");
        populate.append("");
        cabbageIdentifiers.set(CabbageIdentifierIds::populate, populate);
        cabbageIdentifiers.set(CabbageIdentifierIds::name, cabbageIdentifiers.getWithDefault("name", "").toString()+String(ID));
        cabbageIdentifiers.set(CabbageIdentifierIds::identchannel, "");
        cabbageIdentifiers.set(CabbageIdentifierIds::visible, 1);
    }
    //===============combobox==================//
    else if(strTokens[0].trim() == "combobox")
    {
        cabbageIdentifiers.set("basetype", "interactive");

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
        cabbageIdentifiers.set(CabbageIdentifierIds::top, 10);
        cabbageIdentifiers.set(CabbageIdentifierIds::left, 10);
        cabbageIdentifiers.set(CabbageIdentifierIds::width, 80);
        cabbageIdentifiers.set(CabbageIdentifierIds::height, 20);
        var channels;
        channels.append("combochan");
        cabbageIdentifiers.set(CabbageIdentifierIds::channel, channels);
        cabbageIdentifiers.set(CabbageIdentifierIds::channeltype, "number");
        cabbageIdentifiers.set(CabbageIdentifierIds::text, array);
        cabbageIdentifiers.set(CabbageIdentifierIds::value, 1);
        cabbageIdentifiers.set(CabbageIdentifierIds::currenttext, "");
        cabbageIdentifiers.set(CabbageIdentifierIds::caption, "");
        cabbageIdentifiers.set(CabbageIdentifierIds::colour, Colours::black.toString());
        cabbageIdentifiers.set(CabbageIdentifierIds::fontcolour, CabbageUtilities::getComponentFontColour().toString());
        cabbageIdentifiers.set(CabbageIdentifierIds::type, "combobox");
        cabbageIdentifiers.set(CabbageIdentifierIds::name, "combobox");
        cabbageIdentifiers.set(CabbageIdentifierIds::comborange, 4);
        cabbageIdentifiers.set(CabbageIdentifierIds::file, "");
        var populate;
        populate.append("");
        populate.append("");
        cabbageIdentifiers.set(CabbageIdentifierIds::populate, populate);
        cabbageIdentifiers.set(CabbageIdentifierIds::name, cabbageIdentifiers.getWithDefault("name", "").toString()+String(ID));
        cabbageIdentifiers.set(CabbageIdentifierIds::identchannel, "");
        cabbageIdentifiers.set(CabbageIdentifierIds::visible, 1);
        cabbageIdentifiers.set(CabbageIdentifierIds::workingdir, "");

    }

    //===============combobox==================//
    else if(strTokens[0].trim() == "listbox")
    {
        cabbageIdentifiers.set("basetype", "layout");

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
        cabbageIdentifiers.set(CabbageIdentifierIds::top, 10);
        cabbageIdentifiers.set(CabbageIdentifierIds::left, 10);
        cabbageIdentifiers.set(CabbageIdentifierIds::width, 80);
        cabbageIdentifiers.set(CabbageIdentifierIds::height, 20);
        var channels;
        channels.append("listbox");
        cabbageIdentifiers.set(CabbageIdentifierIds::channel, channels);
        cabbageIdentifiers.set(CabbageIdentifierIds::channeltype, "number");
        cabbageIdentifiers.set(CabbageIdentifierIds::text, array);
        cabbageIdentifiers.set(CabbageIdentifierIds::value, 1);
        cabbageIdentifiers.set(CabbageIdentifierIds::caption, "");
        cabbageIdentifiers.set(CabbageIdentifierIds::colour, Colours::black.toString());
        cabbageIdentifiers.set(CabbageIdentifierIds::highlightcolour, Colours::black.contrasting().toString());
        cabbageIdentifiers.set(CabbageIdentifierIds::fontcolour, CabbageUtilities::getComponentFontColour().toString());
        cabbageIdentifiers.set(CabbageIdentifierIds::type, "listbox");
        cabbageIdentifiers.set(CabbageIdentifierIds::name, "listbox");
        cabbageIdentifiers.set(CabbageIdentifierIds::comborange, 4);
        cabbageIdentifiers.set(CabbageIdentifierIds::file, "");
        cabbageIdentifiers.set(CabbageIdentifierIds::align, "centre");
        var populate;
        populate.append("");
        populate.append("");
        cabbageIdentifiers.set(CabbageIdentifierIds::populate, populate);
        cabbageIdentifiers.set(CabbageIdentifierIds::name, cabbageIdentifiers.getWithDefault("name", "").toString()+String(ID));
        cabbageIdentifiers.set(CabbageIdentifierIds::identchannel, "");
        cabbageIdentifiers.set(CabbageIdentifierIds::visible, 1);
        cabbageIdentifiers.set(CabbageIdentifierIds::workingdir, "");

    }
    //===============label==================//
    else if(strTokens[0].trim() == "label")
    {
        cabbageIdentifiers.set("basetype", "layout");
        cabbageIdentifiers.set(CabbageIdentifierIds::top, 10);
        cabbageIdentifiers.set(CabbageIdentifierIds::left, 10);
        cabbageIdentifiers.set(CabbageIdentifierIds::width, 80);
        cabbageIdentifiers.set(CabbageIdentifierIds::height, 16);
        cabbageIdentifiers.set(CabbageIdentifierIds::text, "hello");
        cabbageIdentifiers.set(CabbageIdentifierIds::min, 1);
        cabbageIdentifiers.set(CabbageIdentifierIds::name, "label");
        cabbageIdentifiers.set(CabbageIdentifierIds::type, "label");
        cabbageIdentifiers.set(CabbageIdentifierIds::type, cabbageIdentifiers.getWithDefault("name", "").toString());
        cabbageIdentifiers.set(CabbageIdentifierIds::name, cabbageIdentifiers.getWithDefault("name", "").toString()+String(ID));
        cabbageIdentifiers.set(CabbageIdentifierIds::colour, Colours::transparentBlack.toString());
        cabbageIdentifiers.set(CabbageIdentifierIds::fontcolour, CabbageUtilities::getComponentFontColour().toString());
        cabbageIdentifiers.set(CabbageIdentifierIds::align, "centre");
        cabbageIdentifiers.set(CabbageIdentifierIds::fontstyle, 1);
        cabbageIdentifiers.set(CabbageIdentifierIds::channel, "");
        cabbageIdentifiers.set(CabbageIdentifierIds::identchannel, "");
        cabbageIdentifiers.set(CabbageIdentifierIds::corners, 3);
        cabbageIdentifiers.set(CabbageIdentifierIds::visible, 1);
    }
    //===============multitab==================//
    else if(strTokens[0].trim() == "multitab")
    {
        /*
        cabbageIdentifiers.set("basetype", "layout");
        top = 10;
        left = 10;
        width = 100;
        height = 16;
        channel = "multitab";
        colour = CabbageUtilities::getComponentFontColour();
        fontcolour = CabbageUtilities::getComponentFontColour();
        items.add("Tab 1");
        name = "multitab";
        type = name;
        name.append(String(ID), 1024);
         */
    }
    //===============soundfiler==================//
    else if(strTokens[0].trim() == "soundfiler")
    {
        cabbageIdentifiers.set("basetype", "layout");
        top = 10;
        left = 10;
        width = 300;
        height = 200;

        cabbageIdentifiers.set(CabbageIdentifierIds::top, 10);
        cabbageIdentifiers.set(CabbageIdentifierIds::left, 10);
        cabbageIdentifiers.set(CabbageIdentifierIds::width, 300);
        cabbageIdentifiers.set(CabbageIdentifierIds::height, 200);
        var channels;
        //channels.append("pos");
        //channels.append("end");
        cabbageIdentifiers.set(CabbageIdentifierIds::channel, channels);
        cabbageIdentifiers.set(CabbageIdentifierIds::colour, CabbageUtilities::getComponentFontColour().toString());
        cabbageIdentifiers.set(CabbageIdentifierIds::fontcolour, CabbageUtilities::getComponentFontColour().toString());
        cabbageIdentifiers.set(CabbageIdentifierIds::type, "soundfiler");
        cabbageIdentifiers.set(CabbageIdentifierIds::name, "soundfiler");
        cabbageIdentifiers.set(CabbageIdentifierIds::file, "");
        cabbageIdentifiers.set(CabbageIdentifierIds::name, cabbageIdentifiers.getWithDefault("name", "").toString()+String(ID));
        cabbageIdentifiers.set(CabbageIdentifierIds::identchannel, "");
        cabbageIdentifiers.set(CabbageIdentifierIds::visible, 1);
        cabbageIdentifiers.set(CabbageIdentifierIds::scrubberposition, 0);
        cabbageIdentifiers.set(CabbageIdentifierIds::zoom, 0);
        cabbageIdentifiers.set(CabbageIdentifierIds::startpos, 0);
        cabbageIdentifiers.set(CabbageIdentifierIds::tablenumber, -1);
    }
    //===============gentable==================//
    else if(strTokens[0].trim() == "gentable")
    {
        cabbageIdentifiers.set("basetype", "layout");
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
        cabbageIdentifiers.set(CabbageIdentifierIds::top, 10);
        cabbageIdentifiers.set(CabbageIdentifierIds::left, 10);
        cabbageIdentifiers.set(CabbageIdentifierIds::width, 300);
        cabbageIdentifiers.set(CabbageIdentifierIds::height, 200);
        var channels;
        channels.append("pos");
        channels.append("end");
        cabbageIdentifiers.set(CabbageIdentifierIds::channel, channels);
        cabbageIdentifiers.set(CabbageIdentifierIds::colour, CabbageUtilities::getComponentFontColour().toString());
        cabbageIdentifiers.set(CabbageIdentifierIds::fontcolour, CabbageUtilities::getComponentFontColour().toString());
        cabbageIdentifiers.set(CabbageIdentifierIds::type, "gentable");
        cabbageIdentifiers.set(CabbageIdentifierIds::tablecolour, tableColours);
        cabbageIdentifiers.set(CabbageIdentifierIds::name, "gentable");
        cabbageIdentifiers.set(CabbageIdentifierIds::amprange, 0);
        cabbageIdentifiers.set(CabbageIdentifierIds::drawmode, "");
        cabbageIdentifiers.set(CabbageIdentifierIds::file, "");
        cabbageIdentifiers.set(CabbageIdentifierIds::name, cabbageIdentifiers.getWithDefault("name", "").toString()+String(ID));
        cabbageIdentifiers.set(CabbageIdentifierIds::identchannel, "");
        cabbageIdentifiers.set(CabbageIdentifierIds::visible, 1);
        cabbageIdentifiers.set(CabbageIdentifierIds::active, 0);

        var tables;
        tables.append(0);
        tables.append(0);

        cabbageIdentifiers.set(CabbageIdentifierIds::scrubberposition, tables);
        cabbageIdentifiers.set(CabbageIdentifierIds::zoom, -1);
        cabbageIdentifiers.set(CabbageIdentifierIds::startpos, 0);
        cabbageIdentifiers.set(CabbageIdentifierIds::tablenumber, -1);
        cabbageIdentifiers.set(CabbageIdentifierIds::outlinethickness, 1.f);
        cabbageIdentifiers.set(CabbageIdentifierIds::tablebackgroundcolour, Colour(15, 15, 15).toString());
        cabbageIdentifiers.set(CabbageIdentifierIds::tablegridcolour, Colour(45, 45, 45).toString());
    }

    //===============image==================//
    else if(strTokens[0].trim() == "image")
    {
        cabbageIdentifiers.set("basetype", "layout");
        top = 10;
        left = 10;
        width = 160;
        height = 120;


        cabbageIdentifiers.set(CabbageIdentifierIds::top, 10);
        cabbageIdentifiers.set(CabbageIdentifierIds::left, 10);
        cabbageIdentifiers.set(CabbageIdentifierIds::width, 160);
        cabbageIdentifiers.set(CabbageIdentifierIds::height, 120);
        cabbageIdentifiers.set(CabbageIdentifierIds::shape, "rounded");
        cabbageIdentifiers.set(CabbageIdentifierIds::corners, 5);
        cabbageIdentifiers.set(CabbageIdentifierIds::colour, Colours::white.toString());
        cabbageIdentifiers.set(CabbageIdentifierIds::outlinecolour, Colours::white.toString());
        cabbageIdentifiers.set(CabbageIdentifierIds::outlinethickness, 0);
        cabbageIdentifiers.set(CabbageIdentifierIds::popup, 0);
        cabbageIdentifiers.set(CabbageIdentifierIds::plant, "");
        cabbageIdentifiers.set(CabbageIdentifierIds::type, "image");
        cabbageIdentifiers.set(CabbageIdentifierIds::file, "");
        cabbageIdentifiers.set(CabbageIdentifierIds::name, "image");
        cabbageIdentifiers.set(CabbageIdentifierIds::name, cabbageIdentifiers.getWithDefault("name", "").toString()+String(ID));
        cabbageIdentifiers.set(CabbageIdentifierIds::identchannel, "");
        cabbageIdentifiers.set(CabbageIdentifierIds::visible, 1);
    }

    //===============image==================//
    else if(strTokens[0].trim() == "encoder")
    {
        cabbageIdentifiers.set(CabbageIdentifierIds::basetype, "interactive");
        cabbageIdentifiers.set(CabbageIdentifierIds::top, 10);
        cabbageIdentifiers.set(CabbageIdentifierIds::left, 10);
        cabbageIdentifiers.set(CabbageIdentifierIds::width, 60);
        cabbageIdentifiers.set(CabbageIdentifierIds::height, 60);
        var channels;
        channels.append("encoder");
        cabbageIdentifiers.set(CabbageIdentifierIds::channel, channels);
        cabbageIdentifiers.set(CabbageIdentifierIds::value, 0);
        cabbageIdentifiers.set(CabbageIdentifierIds::sliderincr, .001);
        cabbageIdentifiers.set(CabbageIdentifierIds::text, "");
        cabbageIdentifiers.set(CabbageIdentifierIds::textbox, 0.f);
        cabbageIdentifiers.set(CabbageIdentifierIds::caption, "");
        cabbageIdentifiers.set(CabbageIdentifierIds::colour, Colours::whitesmoke.toString());
        cabbageIdentifiers.set(CabbageIdentifierIds::trackercolour, Colour(0, 118, 38).toString());
        cabbageIdentifiers.set(CabbageIdentifierIds::fontcolour, CabbageUtilities::getComponentFontColour().toString());
        cabbageIdentifiers.set(CabbageIdentifierIds::textcolour, CabbageUtilities::getComponentFontColour().toString());
        cabbageIdentifiers.set(CabbageIdentifierIds::outlinecolour, Colours::black.brighter(.3f).toString());
        cabbageIdentifiers.set(CabbageIdentifierIds::midichan, -99);
        cabbageIdentifiers.set(CabbageIdentifierIds::midictrl, -99);
        //these don't appear in the props dialog
        cabbageIdentifiers.set(CabbageIdentifierIds::name, "encoder");
        cabbageIdentifiers.set(CabbageIdentifierIds::type, cabbageIdentifiers.getWithDefault("name", "").toString());
        cabbageIdentifiers.set(CabbageIdentifierIds::name, cabbageIdentifiers.getWithDefault("name", "").toString()+String(ID));
        cabbageIdentifiers.set(CabbageIdentifierIds::decimalplaces, 1);
        cabbageIdentifiers.set(CabbageIdentifierIds::identchannel, "");
        cabbageIdentifiers.set(CabbageIdentifierIds::trackerthickness, .05);
        cabbageIdentifiers.set(CabbageIdentifierIds::visible, 1);
        cabbageIdentifiers.set(CabbageIdentifierIds::minenabled, 0);
        cabbageIdentifiers.set(CabbageIdentifierIds::maxenabled, 0);
    }

    //===============groupbox==================//
    else if(strTokens[0].trim() == "groupbox")
    {
        cabbageIdentifiers.set(CabbageIdentifierIds::basetype, "layout");
        cabbageIdentifiers.set(CabbageIdentifierIds::top, 10);
        cabbageIdentifiers.set(CabbageIdentifierIds::left, 10);
        cabbageIdentifiers.set(CabbageIdentifierIds::width, 80);
        cabbageIdentifiers.set(CabbageIdentifierIds::height, 22);
        cabbageIdentifiers.set(CabbageIdentifierIds::corners, 5);
        cabbageIdentifiers.set(CabbageIdentifierIds::colour, Colour(35, 35, 35).toString());
        cabbageIdentifiers.set(CabbageIdentifierIds::fontcolour, CabbageUtilities::getComponentFontColour().toString());
        cabbageIdentifiers.set(CabbageIdentifierIds::popup, 0);
        cabbageIdentifiers.set(CabbageIdentifierIds::plant, "");
        cabbageIdentifiers.set(CabbageIdentifierIds::child, 0);
        cabbageIdentifiers.set(CabbageIdentifierIds::linethickness, 1);
        cabbageIdentifiers.set(CabbageIdentifierIds::type, "groupbox");
        cabbageIdentifiers.set(CabbageIdentifierIds::name, "groupbox");
        cabbageIdentifiers.set(CabbageIdentifierIds::name, cabbageIdentifiers.getWithDefault("name", "").toString()+String(ID));
        cabbageIdentifiers.set(CabbageIdentifierIds::identchannel, "");
        cabbageIdentifiers.set(CabbageIdentifierIds::visible, 1);
        cabbageIdentifiers.set(CabbageIdentifierIds::svggroupbox, "");

    }
    //===============line==================//
    else if(strTokens[0].trim() == "line")
    {
        cabbageIdentifiers.set("basetype", "layout");
        top = 10;
        left = 10;
        width = 180;
        height = 2;

        cabbageIdentifiers.set(CabbageIdentifierIds::top, 10);
        cabbageIdentifiers.set(CabbageIdentifierIds::left, 10);
        cabbageIdentifiers.set(CabbageIdentifierIds::width, 160);
        cabbageIdentifiers.set(CabbageIdentifierIds::height, 2);
        cabbageIdentifiers.set(CabbageIdentifierIds::colour, Colours::white.toString());
        cabbageIdentifiers.set(CabbageIdentifierIds::type, "line");
        cabbageIdentifiers.set(CabbageIdentifierIds::name, "line");
        cabbageIdentifiers.set(CabbageIdentifierIds::name, cabbageIdentifiers.getWithDefault("name", "").toString()+String(ID));
        cabbageIdentifiers.set(CabbageIdentifierIds::identchannel, "");
        cabbageIdentifiers.set(CabbageIdentifierIds::visible, 1);
    }

    //===============csoundoutput==================//
    else if(strTokens[0].trim() == "csoundoutput")
    {
        cabbageIdentifiers.set("basetype", "layout");
        top = 10;
        left = 10;
        width = 400;
        height = 200;
        cabbageIdentifiers.set(CabbageIdentifierIds::top, 10);
        cabbageIdentifiers.set(CabbageIdentifierIds::left, 10);
        cabbageIdentifiers.set(CabbageIdentifierIds::width, 400);
        cabbageIdentifiers.set(CabbageIdentifierIds::text, "Csound output");
        cabbageIdentifiers.set(CabbageIdentifierIds::height, 200);
        cabbageIdentifiers.set(CabbageIdentifierIds::colour, Colours::black.toString());
        cabbageIdentifiers.set(CabbageIdentifierIds::fontcolour, Colours::cornflowerblue.toString());
        cabbageIdentifiers.set(CabbageIdentifierIds::name, "csoundoutput");
        cabbageIdentifiers.set(CabbageIdentifierIds::wrap, 0);

        cabbageIdentifiers.set(CabbageIdentifierIds::type, "csoundoutput");
        cabbageIdentifiers.set(CabbageIdentifierIds::name, cabbageIdentifiers.getWithDefault("name", "").toString()+String(ID));
        cabbageIdentifiers.set(CabbageIdentifierIds::identchannel, "");
        cabbageIdentifiers.set(CabbageIdentifierIds::visible, 1);
    }
    //===============textbox==================//
    else if(strTokens[0].trim() == "textbox")
    {
        cabbageIdentifiers.set("basetype", "layout");
        top = 10;
        left = 10;
        width = 400;
        height = 200;
        cabbageIdentifiers.set(CabbageIdentifierIds::top, 10);
        cabbageIdentifiers.set(CabbageIdentifierIds::left, 10);
        cabbageIdentifiers.set(CabbageIdentifierIds::width, 400);
        cabbageIdentifiers.set(CabbageIdentifierIds::text, "");
        cabbageIdentifiers.set(CabbageIdentifierIds::height, 200);
        cabbageIdentifiers.set(CabbageIdentifierIds::colour, Colours::black.toString());
        cabbageIdentifiers.set(CabbageIdentifierIds::fontcolour, Colours::white.toString());
        cabbageIdentifiers.set(CabbageIdentifierIds::name, "textbox");
        cabbageIdentifiers.set(CabbageIdentifierIds::type, "textbox");
        cabbageIdentifiers.set(CabbageIdentifierIds::wrap, 0);
        cabbageIdentifiers.set(CabbageIdentifierIds::file, "");
        cabbageIdentifiers.set(CabbageIdentifierIds::name, cabbageIdentifiers.getWithDefault("name", "").toString()+String(ID));
        cabbageIdentifiers.set(CabbageIdentifierIds::identchannel, "");
        cabbageIdentifiers.set(CabbageIdentifierIds::visible, 1);
    }

    //===============textbox==================//
    else if(strTokens[0].trim() == "texteditor")
    {
        cabbageIdentifiers.set("basetype", "layout");
        top = 10;
        left = 10;
        width = 400;
        height = 200;
        cabbageIdentifiers.set(CabbageIdentifierIds::top, 10);
        cabbageIdentifiers.set(CabbageIdentifierIds::left, 10);
        cabbageIdentifiers.set(CabbageIdentifierIds::width, 400);
        cabbageIdentifiers.set(CabbageIdentifierIds::text, "");
        cabbageIdentifiers.set(CabbageIdentifierIds::height, 200);
        cabbageIdentifiers.set(CabbageIdentifierIds::channel, "texteditor");
        cabbageIdentifiers.set(CabbageIdentifierIds::colour, Colours::white.toString());
        cabbageIdentifiers.set(CabbageIdentifierIds::fontcolour, Colours::black.toString());
        cabbageIdentifiers.set(CabbageIdentifierIds::name, "texteditor");
        cabbageIdentifiers.set(CabbageIdentifierIds::type, "texteditor");
        cabbageIdentifiers.set(CabbageIdentifierIds::name, cabbageIdentifiers.getWithDefault("name", "").toString()+String(ID));
        cabbageIdentifiers.set(CabbageIdentifierIds::identchannel, "");
        cabbageIdentifiers.set(CabbageIdentifierIds::visible, 1);
    }


    //===============vemeter==================//
    else if(strTokens[0].trim() == "vumeter")
    {
        /*
        cabbageIdentifiers.set("basetype", "layout");
        top = 10;
        left = 10;
        width = 400;
        height = 200;
        vuConfig.clear();
        colour = Colours::white;
        name = "vumeter";
        type = name;
        name.append(String(ID), 1024);
         */
    }
    //===============table==================//
    else if(strTokens[0].trim() == "table")
    {
        cabbageIdentifiers.set("basetype", "layout");
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

        cabbageIdentifiers.set(CabbageIdentifierIds::top, 10);
        cabbageIdentifiers.set(CabbageIdentifierIds::left, 10);
        cabbageIdentifiers.set(CabbageIdentifierIds::width, 400);
        cabbageIdentifiers.set(CabbageIdentifierIds::height, 200);
        cabbageIdentifiers.set(CabbageIdentifierIds::tablenumber, 1);
        cabbageIdentifiers.set(CabbageIdentifierIds::drawmode, "");
        cabbageIdentifiers.set(CabbageIdentifierIds::resizemode, 0);
        cabbageIdentifiers.set(CabbageIdentifierIds::readonly, 0);
        cabbageIdentifiers.set(CabbageIdentifierIds::tablecolour, tableColours);
        cabbageIdentifiers.set(CabbageIdentifierIds::amprange, 0);
        cabbageIdentifiers.set(CabbageIdentifierIds::type, "table");
        cabbageIdentifiers.set(CabbageIdentifierIds::stack, 0);
        cabbageIdentifiers.set(CabbageIdentifierIds::name, cabbageIdentifiers.getWithDefault("name", "").toString()+String(ID));
        cabbageIdentifiers.set(CabbageIdentifierIds::identchannel, "");
        cabbageIdentifiers.set(CabbageIdentifierIds::visible, 1);
    }

    //===============xypad==================//
    else if(strTokens[0].trim() == "xypad")
    {
        cabbageIdentifiers.set("basetype", "interactive");
        top = 10;
        left = 10;
        width = 200;
        height = 200;

        var channels;
        channels.append("xChan");
        channels.append("yChan");
        cabbageIdentifiers.set(CabbageIdentifierIds::xyautoindex, 0);
        cabbageIdentifiers.set(CabbageIdentifierIds::xchannel, channels[0]);
        cabbageIdentifiers.set(CabbageIdentifierIds::ychannel, channels[1]);
        cabbageIdentifiers.set(CabbageIdentifierIds::top, 10);
        cabbageIdentifiers.set(CabbageIdentifierIds::left, 10);
        cabbageIdentifiers.set(CabbageIdentifierIds::caption, "");
        cabbageIdentifiers.set(CabbageIdentifierIds::width, 200);
        cabbageIdentifiers.set(CabbageIdentifierIds::height, 200);
        cabbageIdentifiers.set(CabbageIdentifierIds::minx, 0);
        cabbageIdentifiers.set(CabbageIdentifierIds::maxx, 200);
        cabbageIdentifiers.set(CabbageIdentifierIds::text, "");
        cabbageIdentifiers.set(CabbageIdentifierIds::miny, 0);
        cabbageIdentifiers.set(CabbageIdentifierIds::maxy, 200);
        cabbageIdentifiers.set(CabbageIdentifierIds::valuex, 0);
        cabbageIdentifiers.set(CabbageIdentifierIds::valuey, 0);
        cabbageIdentifiers.set(CabbageIdentifierIds::colour, Colours::lime.toString());
        cabbageIdentifiers.set(CabbageIdentifierIds::fontcolour, Colours::cornflowerblue.toString());
        cabbageIdentifiers.set(CabbageIdentifierIds::textcolour, Colours::cornflowerblue.toString());
        cabbageIdentifiers.set(CabbageIdentifierIds::type, "xypad");
        cabbageIdentifiers.set(CabbageIdentifierIds::name, "xypad");
        cabbageIdentifiers.set(CabbageIdentifierIds::name, cabbageIdentifiers.getWithDefault("name", "").toString()+String(ID));
        cabbageIdentifiers.set(CabbageIdentifierIds::identchannel, "");
        cabbageIdentifiers.set(CabbageIdentifierIds::visible, 1);
    }
    //===============keyboard==================//
    else if(strTokens[0].trim() == "keyboard")
    {
        cabbageIdentifiers.set("basetype", "layout");
        top = 10;
        left = 10;
        width = 400;
        height = 100;

        cabbageIdentifiers.set(CabbageIdentifierIds::top, 10);
        cabbageIdentifiers.set(CabbageIdentifierIds::left, 10);
        cabbageIdentifiers.set(CabbageIdentifierIds::width, 400);
        cabbageIdentifiers.set(CabbageIdentifierIds::height, 100);
        cabbageIdentifiers.set(CabbageIdentifierIds::value, 60);
        cabbageIdentifiers.set(CabbageIdentifierIds::middlec, 3);
        cabbageIdentifiers.set(CabbageIdentifierIds::type, "keyboard");
        cabbageIdentifiers.set(CabbageIdentifierIds::name, "keyboard");
        cabbageIdentifiers.set(CabbageIdentifierIds::name, cabbageIdentifiers.getWithDefault("name", "").toString()+String(ID));
        cabbageIdentifiers.set(CabbageIdentifierIds::visible, 1);
        cabbageIdentifiers.set(CabbageIdentifierIds::keywidth, 16);
        cabbageIdentifiers.set(CabbageIdentifierIds::scrollbars, 1);

    }
    //===============form==================//
    else if(strTokens[0].trim() == "form")
    {
        cabbageIdentifiers.set(CabbageIdentifierIds::basetype, "layout");
        cabbageIdentifiers.set(CabbageIdentifierIds::top, 10);
        cabbageIdentifiers.set(CabbageIdentifierIds::left, 10);
        cabbageIdentifiers.set(CabbageIdentifierIds::width, 600);
        cabbageIdentifiers.set(CabbageIdentifierIds::height, 300);
        cabbageIdentifiers.set(CabbageIdentifierIds::caption, "");
        cabbageIdentifiers.set(CabbageIdentifierIds::name, "form");
        cabbageIdentifiers.set(CabbageIdentifierIds::type, "form");
        cabbageIdentifiers.set(CabbageIdentifierIds::guirefresh, 100);
        cabbageIdentifiers.set(CabbageIdentifierIds::identchannel, "");
        cabbageIdentifiers.set(CabbageIdentifierIds::visible, 1);
        cabbageIdentifiers.set(CabbageIdentifierIds::scrollbars, 1);
        cabbageIdentifiers.set(CabbageIdentifierIds::titlebarcolour, "");
        cabbageIdentifiers.set(CabbageIdentifierIds::fontcolour, "");
    }

    //===============rangeslider===============//
    else if(strTokens[0].trim() == "hrange" || strTokens[0].trim() == "vrange")
    {
        cabbageIdentifiers.set(CabbageIdentifierIds::basetype, "interactive");
        cabbageIdentifiers.set(CabbageIdentifierIds::top, 10);
        cabbageIdentifiers.set(CabbageIdentifierIds::left, 10);
        cabbageIdentifiers.set(CabbageIdentifierIds::width, 50);
        cabbageIdentifiers.set(CabbageIdentifierIds::height, 150);
        var channels;
        channels.append("rangeslider");
        cabbageIdentifiers.set(CabbageIdentifierIds::channel, channels);
        cabbageIdentifiers.set(CabbageIdentifierIds::min, 0);
        cabbageIdentifiers.set(CabbageIdentifierIds::max, 1);
        cabbageIdentifiers.set(CabbageIdentifierIds::value, 0);
        cabbageIdentifiers.set(CabbageIdentifierIds::text, "");
        cabbageIdentifiers.set(CabbageIdentifierIds::textbox, 0.f);
        cabbageIdentifiers.set(CabbageIdentifierIds::caption, "");
        cabbageIdentifiers.set(CabbageIdentifierIds::colour, Colours::white.toString());
        cabbageIdentifiers.set(CabbageIdentifierIds::trackercolour, Colour(0, 118, 38).toString());
        cabbageIdentifiers.set(CabbageIdentifierIds::fontcolour, CabbageUtilities::getComponentFontColour().toString());
        cabbageIdentifiers.set(CabbageIdentifierIds::textcolour, CabbageUtilities::getComponentFontColour().toString());
        cabbageIdentifiers.set(CabbageIdentifierIds::sliderskew, 1);
        cabbageIdentifiers.set(CabbageIdentifierIds::sliderincr, .001);
        cabbageIdentifiers.set(CabbageIdentifierIds::midichan, -99);
        cabbageIdentifiers.set(CabbageIdentifierIds::midictrl, -99);
        //these don't appear in the props dialog
        cabbageIdentifiers.set(CabbageIdentifierIds::name, strTokens[0].trim());
        cabbageIdentifiers.set(CabbageIdentifierIds::type, cabbageIdentifiers.getWithDefault("name", "").toString());
        cabbageIdentifiers.set(CabbageIdentifierIds::name, cabbageIdentifiers.getWithDefault("name", "").toString()+String(ID));
        if(strTokens[0].trim() == "hrange")
            cabbageIdentifiers.set(CabbageIdentifierIds::kind, "horizontal");
        else
            cabbageIdentifiers.set(CabbageIdentifierIds::kind, "vertical");

        cabbageIdentifiers.set(CabbageIdentifierIds::decimalplaces, 1);
        cabbageIdentifiers.set(CabbageIdentifierIds::trackerthickness, .1);
        cabbageIdentifiers.set(CabbageIdentifierIds::identchannel, "");
        cabbageIdentifiers.set(CabbageIdentifierIds::visible, 1);
        cabbageIdentifiers.set(CabbageIdentifierIds::svgslider, "");
        cabbageIdentifiers.set(CabbageIdentifierIds::svgsliderbg, "");
    }

    //===============signaldisplay==================//
    else if(strTokens[0].trim() == "signaldisplay" || strTokens[0].trim() == "fftdisplay")
    {
        cabbageIdentifiers.set("basetype", "layout");
        top = 10;
        left = 10;
        width = 180;
        height = 2;

        var signalVariables;
        signalVariables.append("");

        cabbageIdentifiers.set(CabbageIdentifierIds::top, 10);
        cabbageIdentifiers.set(CabbageIdentifierIds::left, 10);
        cabbageIdentifiers.set(CabbageIdentifierIds::width, 260);
        cabbageIdentifiers.set(CabbageIdentifierIds::height, 100);
        cabbageIdentifiers.set(CabbageIdentifierIds::tablecolour, Colours::lime.toString());
        cabbageIdentifiers.set(CabbageIdentifierIds::fontcolour, Colours::white.toString());
        cabbageIdentifiers.set(CabbageIdentifierIds::tablebackgroundcolour, Colour(30, 30, 38).toString());
        cabbageIdentifiers.set(CabbageIdentifierIds::type, "signaldisplay");
        cabbageIdentifiers.set(CabbageIdentifierIds::name, "signaldisplay");
        cabbageIdentifiers.set(CabbageIdentifierIds::min, 0);
        cabbageIdentifiers.set(CabbageIdentifierIds::max, 2048);
        cabbageIdentifiers.set(CabbageIdentifierIds::updaterate, 100);
        cabbageIdentifiers.set(CabbageIdentifierIds::signalvariable, signalVariables);
        cabbageIdentifiers.set(CabbageIdentifierIds::outlinethickness, 1);
        cabbageIdentifiers.set(CabbageIdentifierIds::name, cabbageIdentifiers.getWithDefault("name", "").toString()+String(ID));
        cabbageIdentifiers.set(CabbageIdentifierIds::identchannel, "");
        cabbageIdentifiers.set(CabbageIdentifierIds::displaytype, "spectroscope");
        cabbageIdentifiers.set(CabbageIdentifierIds::zoom, 0);
        cabbageIdentifiers.set(CabbageIdentifierIds::visible, 1);
    }

    //===============stepper==================//
    else if(strTokens[0].trim() == "scope")
    {
        cabbageIdentifiers.set("basetype", "layout");
        top = 10;
        left = 10;
        width = 180;
        height = 2;

        cabbageIdentifiers.set(CabbageIdentifierIds::top, 10);
        cabbageIdentifiers.set(CabbageIdentifierIds::left, 10);
        cabbageIdentifiers.set(CabbageIdentifierIds::width, 160);
        cabbageIdentifiers.set(CabbageIdentifierIds::height, 2);
        cabbageIdentifiers.set(CabbageIdentifierIds::colour, Colours::white.toString());
        cabbageIdentifiers.set(CabbageIdentifierIds::type, "scope");
        cabbageIdentifiers.set(CabbageIdentifierIds::name, "scope");
        cabbageIdentifiers.set(CabbageIdentifierIds::name, cabbageIdentifiers.getWithDefault("name", "").toString()+String(ID));
        cabbageIdentifiers.set(CabbageIdentifierIds::identchannel, "");
        cabbageIdentifiers.set(CabbageIdentifierIds::visible, 1);
    }

    //===============stepper==================//
    else if(strTokens[0].trim() == "stepper")
    {
        cabbageIdentifiers.set("basetype", "layout");
        top = 10;
        left = 10;
        width = 180;
        height = 2;

        cabbageIdentifiers.set(CabbageIdentifierIds::top, 10);
        cabbageIdentifiers.set(CabbageIdentifierIds::left, 10);
        cabbageIdentifiers.set(CabbageIdentifierIds::width, 160);
        cabbageIdentifiers.set(CabbageIdentifierIds::height, 2);
        cabbageIdentifiers.set(CabbageIdentifierIds::colour, Colours::white.toString());
        cabbageIdentifiers.set(CabbageIdentifierIds::type, "stepper");
        cabbageIdentifiers.set(CabbageIdentifierIds::name, "stepper");
        cabbageIdentifiers.set(CabbageIdentifierIds::name, cabbageIdentifiers.getWithDefault("name", "").toString()+String(ID));
        cabbageIdentifiers.set(CabbageIdentifierIds::identchannel, "");
        cabbageIdentifiers.set(CabbageIdentifierIds::visible, 1);
    }

    //===============non-GUI host widgets==================//
    else if(strTokens[0].trim() == "hostbpm")
    {
        cabbageIdentifiers.set(CabbageIdentifierIds::basetype, "layout");
        cabbageIdentifiers.set(CabbageIdentifierIds::name, "hostbpm");
        cabbageIdentifiers.set(CabbageIdentifierIds::type, "hostbpm");
    }

    else if(strTokens[0].trim() == "hostppqpos")
    {
        cabbageIdentifiers.set(CabbageIdentifierIds::basetype, "layout");
        cabbageIdentifiers.set(CabbageIdentifierIds::name, "hostppqpos");
        cabbageIdentifiers.set(CabbageIdentifierIds::type, "hostppqpos");
    }

    else if(strTokens[0].trim() == "hostplaying")
    {
        cabbageIdentifiers.set(CabbageIdentifierIds::basetype, "layout");
        cabbageIdentifiers.set(CabbageIdentifierIds::name, "hostplaying");
        cabbageIdentifiers.set(CabbageIdentifierIds::type, "hostplaying");
    }

    else if(strTokens[0].trim() == "hostrecording")
    {
        cabbageIdentifiers.set(CabbageIdentifierIds::basetype, "layout");
        cabbageIdentifiers.set(CabbageIdentifierIds::name, "hostrecording");
        cabbageIdentifiers.set(CabbageIdentifierIds::type, "hostrecording");
    }

    else if(strTokens[0].trim() == "hosttime")
    {
        cabbageIdentifiers.set(CabbageIdentifierIds::basetype, "layout");
        cabbageIdentifiers.set(CabbageIdentifierIds::name, "hosttime");
        cabbageIdentifiers.set(CabbageIdentifierIds::type, "hosttime");
    }

    cabbageIdentifiers.set(CabbageIdentifierIds::popuptext, "");
    cabbageIdentifiers.set(CabbageIdentifierIds::alpha, 1);
    cabbageIdentifiers.set(CabbageIdentifierIds::visible, 1);
    cabbageIdentifiers.set(CabbageIdentifierIds::rotate, 0.f);
    cabbageIdentifiers.set(CabbageIdentifierIds::pivotx, 0.f);
    cabbageIdentifiers.set(CabbageIdentifierIds::pivoty, 0.f);


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
                cabbageIdentifiers.set(CabbageIdentifierIds::name, strTokens[0].trim());
            }
            else if(identArray[indx].equalsIgnoreCase("plant"))
            {
                cabbageIdentifiers.set(CabbageIdentifierIds::plant, strTokens[0].trim());
            }
            else if(identArray[indx].equalsIgnoreCase("caption"))
            {
                cabbageIdentifiers.set(CabbageIdentifierIds::caption, strTokens[0].trim());
            }

            else if(identArray[indx].equalsIgnoreCase("signalvariable"))
            {
                var array;
                array.append(strTokens[0].trim());
                for(int i=1; i<strTokens.size(); i++)
                {
                    array.append(strTokens[i].trim());
                }
                cabbageIdentifiers.set(CabbageIdentifierIds::signalvariable, array);
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
                    cabbageIdentifiers.set(CabbageIdentifierIds::xchannel, strTokens[0].trim());
                    cabbageIdentifiers.set(CabbageIdentifierIds::ychannel, strTokens[1].trim());
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
                    cabbageIdentifiers.set(CabbageIdentifierIds::channel, array);
                    //identifiers that appear more than once need to use indx-- so we can check for another instance
                    indx--;
                }

                cabbageIdentifiers.set(CabbageIdentifierIds::channel, array);
                if(identArray[indx].equalsIgnoreCase("widgetarray"))
                {
                    var channelArray, identChannelArray;
                    int size = strTokens[1].getIntValue();
                    for(int i=0; i<size; i++)
                    {
                        channelArray.append(String(strTokens[0])+String(i+1));
                        identChannelArray.append(String(strTokens[0])+String("_ident")+String(i+1));

                    }
                    cabbageIdentifiers.set(CabbageIdentifierIds::channelarray, channelArray);
                    cabbageIdentifiers.set(CabbageIdentifierIds::identchannelarray, identChannelArray);
                }
            }

            else if(identArray[indx].equalsIgnoreCase("colour:0")||
                    identArray[indx].equalsIgnoreCase("colours("))
            {
                cabbageIdentifiers.set(CabbageIdentifierIds::colour, getColourFromText(strTokens.joinIntoString(",")).toString());
            }

            else if(identArray[indx].equalsIgnoreCase("colour:1") ||
                    identArray[indx].equalsIgnoreCase("colour"))
            {
                if(typeOfWidget.contains("checkbox") || typeOfWidget.contains("button"))
                {
                    cabbageIdentifiers.set(CabbageIdentifierIds::oncolour, getColourFromText(strTokens.joinIntoString(",")).toString());
                }
                else
                    cabbageIdentifiers.set(CabbageIdentifierIds::colour, getColourFromText(strTokens.joinIntoString(",")).toString());
            }

            else if(identArray[indx].equalsIgnoreCase("fontcolour")||
                    identArray[indx].equalsIgnoreCase("fontcolour:1"))
            {
                if(typeOfWidget.contains("button"))
                    cabbageIdentifiers.set(CabbageIdentifierIds::onfontcolour, getColourFromText(strTokens.joinIntoString(",")).toString());
                else
                    cabbageIdentifiers.set(CabbageIdentifierIds::fontcolour, getColourFromText(strTokens.joinIntoString(",")).toString());

            }
            else if(identArray[indx].equalsIgnoreCase("fontcolour:0"))
            {
                cabbageIdentifiers.set(CabbageIdentifierIds::fontcolour, getColourFromText(strTokens.joinIntoString(",")).toString());
            }

            else if(identArray[indx].equalsIgnoreCase("tablebackgroundcolour"))
            {
                cabbageIdentifiers.set(CabbageIdentifierIds::tablebackgroundcolour, getColourFromText(strTokens.joinIntoString(",")).toString());
            }

            else if(identArray[indx].equalsIgnoreCase("backgroundcolour"))
            {
                cabbageIdentifiers.set(CabbageIdentifierIds::backgroundcolour, getColourFromText(strTokens.joinIntoString(",")).toString());
            }

            else if(identArray[indx].equalsIgnoreCase("tablegridcolour"))
            {
                cabbageIdentifiers.set(CabbageIdentifierIds::tablegridcolour, getColourFromText(strTokens.joinIntoString(",")).toString());
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
                cabbageIdentifiers.set(CabbageIdentifierIds::tablecolour, colours);
            }

            else if(identArray[indx].equalsIgnoreCase("trackercolour"))
            {
                cabbageIdentifiers.set(CabbageIdentifierIds::trackercolour, getColourFromText(strTokens.joinIntoString(",")).toString());
            }

            else if(identArray[indx].equalsIgnoreCase("highlightcolour"))
            {
                cabbageIdentifiers.set(CabbageIdentifierIds::highlightcolour, getColourFromText(strTokens.joinIntoString(",")).toString());
            }

            else if(identArray[indx].equalsIgnoreCase("titlebarcolour"))
            {
                cabbageIdentifiers.set(CabbageIdentifierIds::titlebarcolour, getColourFromText(strTokens.joinIntoString(",")).toString());
            }

            else if(identArray[indx].equalsIgnoreCase("kind"))
            {
                cabbageIdentifiers.set(CabbageIdentifierIds::kind, strTokens[0].trim());
            }

            else if(identArray[indx].equalsIgnoreCase("file"))
            {
                cabbageIdentifiers.set(CabbageIdentifierIds::file, strTokens[0].trim());
            }

            else if(identArray[indx].equalsIgnoreCase("svgpath"))
            {
                cabbageIdentifiers.set(CabbageIdentifierIds::svgpath, strTokens[0].trim());
            }

            else if(identArray[indx].equalsIgnoreCase("svgfile"))
            {
                if(typeOfWidget=="groupbox")
                    cabbageIdentifiers.set(CabbageIdentifierIds::svggroupbox, strTokens[0].trim());
                else if(typeOfWidget.contains("slider"))
                {
                    if(strTokens[0].trim().contains("background"))
                        cabbageIdentifiers.set(CabbageIdentifierIds::svgsliderbg, strTokens[1].trim());
                    else
                        cabbageIdentifiers.set(CabbageIdentifierIds::svgslider, strTokens[1].trim());
                }
                else if(typeOfWidget.contains("button"))
                {
                    if(strTokens[0].trim().contains("off"))
                        cabbageIdentifiers.set(CabbageIdentifierIds::svgbuttonoff, strTokens[1].trim());
                    else
                        cabbageIdentifiers.set(CabbageIdentifierIds::svgbuttonon, strTokens[1].trim());
                }
                //identifiers that appear more than once need to use indx-- so we can check for another instance
                indx--;
            }

            else if(identArray[indx].equalsIgnoreCase("fillcolour"))
            {
                cabbageIdentifiers.set(CabbageIdentifierIds::fillcolour, getColourFromText(strTokens.joinIntoString(",")).toString());
            }

            else if(identArray[indx].equalsIgnoreCase("shape"))
            {
                cabbageIdentifiers.set(CabbageIdentifierIds::shape, strTokens[0].trim());
            }
            else if(identArray[indx].equalsIgnoreCase("outlinecolour"))
            {
                cabbageIdentifiers.set(CabbageIdentifierIds::outlinecolour, getColourFromText(strTokens.joinIntoString(",")).toString());
            }
            else if(identArray[indx].equalsIgnoreCase("textcolour"))
            {
                cabbageIdentifiers.set(CabbageIdentifierIds::textcolour, getColourFromText(strTokens.joinIntoString(",")).toString());
            }

            else if(identArray[indx].equalsIgnoreCase("pluginid"))
            {
                cabbageIdentifiers.set(CabbageIdentifierIds::pluginid, strTokens[0].trim());
            }

            else if(identArray[indx].equalsIgnoreCase("popuptext"))
            {
                cabbageIdentifiers.set(CabbageIdentifierIds::popuptext, strTokens[0]);
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

                cabbageIdentifiers.set(CabbageIdentifierIds::text, value);
                cabbageIdentifiers.set(CabbageIdentifierIds::comborange, comboRange);
            }


            else if(identArray[indx].equalsIgnoreCase("preset"))
            {
                //CabbageUtilities::showMessage("preset is no longer supported, see docs");
            }

            else if(identArray[indx].equalsIgnoreCase("mode") ||
                    identArray[indx].equalsIgnoreCase(",mode"))
            {
                cabbageIdentifiers.set(CabbageIdentifierIds::mode, strTokens[0].trim());
            }

            else if(identArray[indx].equalsIgnoreCase("native"))
            {
                cabbageIdentifiers.set("native", strTokens[0].trim());
            }


            else if(identArray[indx].equalsIgnoreCase("channeltype"))
            {
                cabbageIdentifiers.set(CabbageIdentifierIds::channeltype, strTokens[0].trim());
            }

            else if(identArray[indx].equalsIgnoreCase("identchannel"))
            {
                cabbageIdentifiers.set(CabbageIdentifierIds::identchannel, strTokens[0].trim());
            }

            else if(identArray[indx].equalsIgnoreCase("populate"))
            {
                cabbageIdentifiers.set(CabbageIdentifierIds::filetype, strTokens[0].trim());

                if(strTokens.size()>1)
                {
                    cabbageIdentifiers.set(CabbageIdentifierIds::workingdir, strTokens[1].trim());
                }
            }

            else if(identArray[indx].equalsIgnoreCase("author"))
            {
                cabbageIdentifiers.set(CabbageIdentifierIds::author, strTokens[0].trim());
            }

            else if(identArray[indx].equalsIgnoreCase("align"))
            {
                cabbageIdentifiers.set(CabbageIdentifierIds::align, strTokens[0].trim());
            }


            else if(identArray[indx].equalsIgnoreCase("tabs"))
            {
                var value;
                for(int i= 0; i<(int)strTokens.size(); i++)
                {
                    value.append(strTokens[i].trim());
                }
                cabbageIdentifiers.set(CabbageIdentifierIds::tabs, strTokens[0].trim());
            }

            else if(identArray[indx].equalsIgnoreCase("tabpage"))
            {
                cabbageIdentifiers.set(CabbageIdentifierIds::tabpage, strTokens[0].trim());
            }

            else if(identArray[indx].equalsIgnoreCase("displaytype"))
            {
                cabbageIdentifiers.set(CabbageIdentifierIds::displaytype, strTokens[0].trim());
            }

            else if(identArray[indx].equalsIgnoreCase("include"))
            {
                var array;
                for(int i=0; i<strTokens.size(); i++)
                {
                    array.append(strTokens[i].trim());
                }

                cabbageIdentifiers.set(CabbageIdentifierIds::include, array);
            }

            else if(identArray[indx].equalsIgnoreCase("drawmode"))
            {
                var value;
                if(strTokens.size()>0)
                    for(int i=0; i<strTokens.size(); i++)
                    {
                        value.append(strTokens[i].trim());
                    }
                cabbageIdentifiers.set(CabbageIdentifierIds::drawmode, value);
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
                    cabbageIdentifiers.set(CabbageIdentifierIds::socketaddress, address);
                    cabbageIdentifiers.set(CabbageIdentifierIds::socketport, port);
                }
            }

            else if(identArray[indx].equalsIgnoreCase("size"))
            {
                if(strTokens.size()>=2)
                {
                    //sizeText = identArray.getReference(indx)+tstr+")";
                    width = strTokens[0].trim().getFloatValue();
                    height = strTokens[1].trim().getFloatValue();
                    cabbageIdentifiers.set(CabbageIdentifierIds::width, strTokens[0].trim().getFloatValue());
                    cabbageIdentifiers.set(CabbageIdentifierIds::height, strTokens[1].trim().getFloatValue());
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
                        cabbageIdentifiers.set(CabbageIdentifierIds::height, width);
                    }
                    else
                    {
                        height = strTokens[3].trim().getFloatValue();
                        cabbageIdentifiers.set(CabbageIdentifierIds::left, strTokens[0].trim().getFloatValue());
                        cabbageIdentifiers.set(CabbageIdentifierIds::top, strTokens[1].trim().getFloatValue());
                        cabbageIdentifiers.set(CabbageIdentifierIds::width, strTokens[2].trim().getFloatValue());
                        cabbageIdentifiers.set(CabbageIdentifierIds::height, strTokens[3].trim().getFloatValue());
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
                    cabbageIdentifiers.set(CabbageIdentifierIds::left, strTokens[0].trim().getFloatValue());
                    cabbageIdentifiers.set(CabbageIdentifierIds::top, strTokens[1].trim().getFloatValue());
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
                    cabbageIdentifiers.set(CabbageIdentifierIds::min, strTokens[0].trim().getDoubleValue());
                    cabbageIdentifiers.set(CabbageIdentifierIds::max, strTokens[1].trim().getDoubleValue());
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
                                cabbageIdentifiers.set(CabbageIdentifierIds::minvalue, values[0].trim().getFloatValue());
                                cabbageIdentifiers.set(CabbageIdentifierIds::maxvalue, values[1].trim().getFloatValue());
                            }
                        }
                        else
                        {
                            cabbageIdentifiers.set(CabbageIdentifierIds::value, strTokens[2].trim().getDoubleValue() < min ? min : strTokens[2].trim().getDoubleValue());
                            tempArray.add(strTokens[2].trim());
                        }
                    }


                    if(strTokens.size()>3)
                    {
                        cabbageIdentifiers.set(CabbageIdentifierIds::sliderskew, strTokens[3].trim().getDoubleValue());
                        tempArray.add(strTokens[3].trim());
                    }

                    if(strTokens.size()>4)
                    {
                        tempArray.add(strTokens[4].trim());
                        cabbageIdentifiers.set(CabbageIdentifierIds::sliderincr, strTokens[4].trim().getDoubleValue());
                    }

                    double sliderRange = max-min;
                    int decimalPlaces = getNumberOfDecimalPlaces(tempArray);
                    if(decimalPlaces<1 && max<=1)
                        decimalPlaces = 2;
                    else
                        decimalPlaces = getNumberOfDecimalPlaces(tempArray);
                    cabbageIdentifiers.set(CabbageIdentifierIds::decimalplaces, decimalPlaces);
                    cabbageIdentifiers.set(CabbageIdentifierIds::range, sliderRange);

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

                    cabbageIdentifiers.set(CabbageIdentifierIds::minx, strTokens[0].trim().getDoubleValue());
                    cabbageIdentifiers.set(CabbageIdentifierIds::maxx, strTokens[1].getDoubleValue());
                    cabbageIdentifiers.set(CabbageIdentifierIds::valuex, strTokens[2].getDoubleValue());
                    cabbageIdentifiers.set(CabbageIdentifierIds::rangex, maxX-minX);

                    cabbageIdentifiers.set(CabbageIdentifierIds::decimalplaces, strTokens[3].trim().getDoubleValue());
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

                    cabbageIdentifiers.set(CabbageIdentifierIds::miny, strTokens[0].trim().getDoubleValue());
                    cabbageIdentifiers.set(CabbageIdentifierIds::maxy, strTokens[1].trim().getDoubleValue());
                    cabbageIdentifiers.set(CabbageIdentifierIds::valuey, strTokens[2].trim().getDoubleValue());
                    cabbageIdentifiers.set(CabbageIdentifierIds::rangey, maxY-minY);
                    cabbageIdentifiers.set(CabbageIdentifierIds::decimalplaces, strTokens[3].trim().getDoubleValue());
                }
            }
            else if(identArray[indx].equalsIgnoreCase("min"))
            {
                cabbageIdentifiers.set(CabbageIdentifierIds::minenabled, 1);
                cabbageIdentifiers.set(CabbageIdentifierIds::minvalue, strTokens[0].trim().getFloatValue());
            }

            else if(identArray[indx].equalsIgnoreCase("svgdebug"))
            {
                cabbageIdentifiers.set(CabbageIdentifierIds::svgdebug, strTokens[0].trim().getFloatValue());
            }

            else if(identArray[indx].equalsIgnoreCase("middlec"))
            {
                cabbageIdentifiers.set(CabbageIdentifierIds::middlec, strTokens[0].trim().getFloatValue());
            }

            else if(identArray[indx].equalsIgnoreCase("fill"))
            {
                cabbageIdentifiers.set(CabbageIdentifierIds::fill, strTokens[0].trim().getIntValue());
            }

            else if(identArray[indx].equalsIgnoreCase("updaterate"))
            {
                cabbageIdentifiers.set(CabbageIdentifierIds::updaterate, strTokens[0].trim().getIntValue());
            }

            else if(identArray[indx].equalsIgnoreCase("keywidth"))
            {
                cabbageIdentifiers.set(CabbageIdentifierIds::keywidth, strTokens[0].trim().getFloatValue());
            }

            else if(identArray[indx].equalsIgnoreCase("rotate"))
            {
                cabbageIdentifiers.set(CabbageIdentifierIds::rotate, strTokens[0].trim().getFloatValue());
                if(strTokens.size()==3)
                {
                    cabbageIdentifiers.set(CabbageIdentifierIds::pivotx, strTokens[1].trim().getFloatValue());
                    cabbageIdentifiers.set(CabbageIdentifierIds::pivoty, strTokens[2].trim().getFloatValue());
                }
            }

            else if(identArray[indx].equalsIgnoreCase("pivoty"))
            {
                cabbageIdentifiers.set(CabbageIdentifierIds::pivoty, strTokens[0].trim().getFloatValue());
            }

            else if(identArray[indx].equalsIgnoreCase("ffttablenumber"))
            {
                cabbageIdentifiers.set(CabbageIdentifierIds::ffttablenumber, strTokens[0].trim().getFloatValue()-1);
            }

            else if(identArray[indx].equalsIgnoreCase("pivotx"))
            {
                cabbageIdentifiers.set(CabbageIdentifierIds::pivotx, strTokens[0].trim().getFloatValue());
            }

            else if(identArray[indx].equalsIgnoreCase("midictrl"))
            {
                if(strTokens.size()<2)
                {
                    warningMessages+="Not enough paramters passed to midiCtrl(): usage midiCtrl(midiChan, midiCtrl\")\n";
                }
                else
                {
                    cabbageIdentifiers.set(CabbageIdentifierIds::midichan, strTokens[0].trim().getFloatValue());
                    cabbageIdentifiers.set(CabbageIdentifierIds::midictrl, strTokens[0].trim().getFloatValue());
                }
            }
            else if(identArray[indx].equalsIgnoreCase("max"))
            {
                cabbageIdentifiers.set(CabbageIdentifierIds::maxenabled, 1);
                cabbageIdentifiers.set(CabbageIdentifierIds::maxvalue, strTokens[0].trim().getFloatValue());
            }

            else if(identArray[indx].equalsIgnoreCase("sliderincr"))
            {
                cabbageIdentifiers.set(CabbageIdentifierIds::sliderincr, strTokens[0].trim().getFloatValue());
            }

            else if(identArray[indx].equalsIgnoreCase("sliderskew"))
            {
                cabbageIdentifiers.set(CabbageIdentifierIds::sliderskew, strTokens[0].trim().getFloatValue());
            }

            else if(identArray[indx].equalsIgnoreCase("visible"))
            {
                cabbageIdentifiers.set(CabbageIdentifierIds::visible, strTokens[0].trim().getFloatValue());
            }

            else if(identArray[indx].equalsIgnoreCase("active"))
            {
                cabbageIdentifiers.set(CabbageIdentifierIds::active, strTokens[0].trim().getFloatValue());
            }

            else if(identArray[indx].equalsIgnoreCase("stack"))
            {
                cabbageIdentifiers.set(CabbageIdentifierIds::stack, strTokens[0].trim().getFloatValue());
            }

            else if(identArray[indx].equalsIgnoreCase("tab"))
            {
                cabbageIdentifiers.set(CabbageIdentifierIds::tabbed, strTokens[0].trim().getFloatValue());
            }

            else if(identArray[indx].equalsIgnoreCase("latched"))
            {
                cabbageIdentifiers.set(CabbageIdentifierIds::latched, strTokens[0].trim().getFloatValue());
            }

            else if(identArray[indx].equalsIgnoreCase("alpha"))
            {
                cabbageIdentifiers.set(CabbageIdentifierIds::alpha, strTokens[0].trim().getFloatValue());
            }

            else if(identArray[indx].equalsIgnoreCase("corners"))
            {
                cabbageIdentifiers.set(CabbageIdentifierIds::corners, strTokens[0].trim().getFloatValue());
            }

            else if(identArray[indx].equalsIgnoreCase("radiogroup"))
            {
                cabbageIdentifiers.set(CabbageIdentifierIds::radiogroup, strTokens[0].trim().getFloatValue());
            }

            else if(identArray[indx].equalsIgnoreCase("guirefresh"))
            {
                cabbageIdentifiers.set(CabbageIdentifierIds::guirefresh, strTokens[0].trim().getFloatValue());
            }

            else if(identArray[indx].equalsIgnoreCase("fontstyle"))
            {
                if(strTokens[0].trim()=="bold")
                    cabbageIdentifiers.set(CabbageIdentifierIds::fontstyle, 1);
                else if(strTokens[0].trim()=="italic")
                    cabbageIdentifiers.set(CabbageIdentifierIds::fontstyle, 2);
                else if(strTokens[0].trim()=="bold italic")
                    cabbageIdentifiers.set(CabbageIdentifierIds::fontstyle, 3);
                else if(strTokens[0].trim()=="underlined")
                    cabbageIdentifiers.set(CabbageIdentifierIds::fontstyle, 4);
                else if(strTokens[0].trim()=="bold underlined")
                    cabbageIdentifiers.set(CabbageIdentifierIds::fontstyle, 5);
                else if(strTokens[0].trim()=="italic underlined")
                    cabbageIdentifiers.set(CabbageIdentifierIds::fontstyle, 6);
                else if(strTokens[0].trim()=="bold italic underlined")
                    cabbageIdentifiers.set(CabbageIdentifierIds::fontstyle, 7);
                else
                    cabbageIdentifiers.set(CabbageIdentifierIds::fontstyle, 0);
            }

            else if(identArray[indx].equalsIgnoreCase("textbox"))
            {
                cabbageIdentifiers.set(CabbageIdentifierIds::textbox, strTokens[0].trim().getFloatValue());
            }

            else if(identArray[indx].equalsIgnoreCase("velocity"))
            {
                cabbageIdentifiers.set(CabbageIdentifierIds::velocity, strTokens[0].trim().getFloatValue());
            }

            else if(identArray[indx].equalsIgnoreCase("gradient"))
            {
                cabbageIdentifiers.set(CabbageIdentifierIds::gradient, strTokens[0].trim().getFloatValue());
            }

            else if(identArray[indx].equalsIgnoreCase("scrubberposition"))
            {
                var scrubberInfo;
                scrubberInfo.append(strTokens[0].trim().getIntValue());
                for(int i=1; i<strTokens.size(); i++)
                {
                    scrubberInfo.append(strTokens[i].trim().getDoubleValue());
                    cabbageIdentifiers.set(CabbageIdentifierIds::scrubberposition, scrubberInfo);
                }

                cabbageIdentifiers.set(CabbageIdentifierIds::scrubberposition, scrubberInfo);

            }

            else if(identArray[indx].equalsIgnoreCase("logger"))
            {
                cabbageIdentifiers.set(CabbageIdentifierIds::logger, strTokens[0].trim().getIntValue());
            }

            else if(identArray[indx].equalsIgnoreCase("child"))
            {
                cabbageIdentifiers.set(CabbageIdentifierIds::child, strTokens[0].trim().getIntValue());
            }

            else if(identArray[indx].equalsIgnoreCase("show"))
            {
                cabbageIdentifiers.set(CabbageIdentifierIds::show, strTokens[0].trim().getIntValue());
            }

            else if(identArray[indx].equalsIgnoreCase("zoom"))
            {
                cabbageIdentifiers.set(CabbageIdentifierIds::zoom, strTokens[0].trim().getFloatValue());
                if(strTokens.size()>1)
                    cabbageIdentifiers.set(CabbageIdentifierIds::startpos, strTokens[1].trim().getFloatValue());
                else
                    cabbageIdentifiers.set(CabbageIdentifierIds::startpos, -1);
            }

            else if(identArray[indx].equalsIgnoreCase("samplerange"))
            {
                cabbageIdentifiers.set(CabbageIdentifierIds::startpos, strTokens[0].trim().getFloatValue());
                if(strTokens.size()>1)
                    cabbageIdentifiers.set(CabbageIdentifierIds::endpos, strTokens[1].trim().getFloatValue());
                else
                    cabbageIdentifiers.set(CabbageIdentifierIds::endpos, -1);
            }

            else if(identArray[indx].equalsIgnoreCase("wrap"))
            {
                cabbageIdentifiers.set(CabbageIdentifierIds::wrap, strTokens[0].trim().getFloatValue());
            }

            else if(identArray[indx].equalsIgnoreCase("readonly"))
            {
                cabbageIdentifiers.set(CabbageIdentifierIds::readonly, strTokens[0].trim().getFloatValue());
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

                cabbageIdentifiers.set(CabbageIdentifierIds::tableconfig, tableConfig);
                cabbageIdentifiers.set(CabbageIdentifierIds::tablenumber, value);
            }
            else if(identArray[indx].equalsIgnoreCase("popup"))
            {
                cabbageIdentifiers.set(CabbageIdentifierIds::popup, strTokens[0].trim().getIntValue());
            }

            else if(identArray[indx].equalsIgnoreCase("resize"))
            {
                cabbageIdentifiers.set(CabbageIdentifierIds::popup, strTokens[0].trim().getIntValue());
            }

            else if(identArray[indx].equalsIgnoreCase("resize"))
            {
                cabbageIdentifiers.set(CabbageIdentifierIds::popup, strTokens[0].trim().getIntValue());
            }

            else if(identArray[indx].equalsIgnoreCase("rescale"))
            {
                cabbageIdentifiers.set(CabbageIdentifierIds::scalex, strTokens[0].trim().getFloatValue());
                cabbageIdentifiers.set(CabbageIdentifierIds::scaley, strTokens[1].trim().getFloatValue());
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
                cabbageIdentifiers.set(CabbageIdentifierIds::resizemode, value);
            }

            else if(identArray[indx].equalsIgnoreCase("amprange"))
            {
                if(strTokens.size()<3)
                    warningMessages+="Not enough paramters passed to amprange(): usage amprange(min, max, tablenumber, quantise\")\n";

                var value;
                var temp = cabbageIdentifiers.getWithDefault(CabbageIdentifierIds::amprange, "");

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

                cabbageIdentifiers.set(CabbageIdentifierIds::amprange, value);
                //identifiers that appear more than once need to use indx-- so we can check for another instance
                indx--;
            }

            else if(identArray[indx].equalsIgnoreCase("outlinethickness"))
            {
                //cabbageIdentifiers.set("line", strTokens[0].trim().getFloatValue());
                cabbageIdentifiers.set(CabbageIdentifierIds::outlinethickness, strTokens[0].trim().getFloatValue());
            }

            else if(identArray[indx].equalsIgnoreCase("linethickness"))
            {
                //cabbageIdentifiers.set("line", strTokens[0].trim().getFloatValue());
                cabbageIdentifiers.set(CabbageIdentifierIds::linethickness, strTokens[0].trim().getFloatValue());
            }

            else if(identArray[indx].equalsIgnoreCase("trackerthickness"))
            {
                //cabbageIdentifiers.set("line", strTokens[0].trim().getFloatValue());
                cabbageIdentifiers.set(CabbageIdentifierIds::trackerthickness, strTokens[0].trim().getFloatValue());
            }

            else if(identArray[indx].equalsIgnoreCase("value"))
            {
                cabbageIdentifiers.set(CabbageIdentifierIds::value, strTokens[0].trim().getFloatValue());
            }

            else if(identArray[indx].equalsIgnoreCase("scrollbars"))
            {
                cabbageIdentifiers.set(CabbageIdentifierIds::scrollbars, strTokens[0].trim().getIntValue());
            }

            //============================================
            //sample identifiers for stepper class
            //============================================
            else if(identArray[indx].equalsIgnoreCase("numberofsteps"))
            {
                cabbageIdentifiers.set("numberofsteps", strTokens[0].trim().getFloatValue());
            }

            else if(identArray[indx].equalsIgnoreCase("stepbpm"))
            {
                cabbageIdentifiers.set("stepbpm", strTokens[0].trim().getIntValue());
            }


            strTokens.clear();
        }
    }
}
//=========================================================================
//retrieve numerical attributes
float CabbageWidget::getNumProp(Identifier prop)
{
    var props = cabbageIdentifiers.getWithDefault(prop, -9999);
    if(props.size()>0)
        return props[0];
    else
        return cabbageIdentifiers.getWithDefault(prop, -9999);
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
    cabbageIdentifiers.set(prop, val);
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
    var strings = cabbageIdentifiers.getWithDefault(prop, "");


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
    var strings = cabbageIdentifiers.getWithDefault(prop, "");
    for(int i=0; i<strings.size(); i++)
        returnArray.add(strings[i].toString());
    return returnArray;
}
//===================================================================
String CabbageWidget::getStringArrayPropValue(Identifier prop, int index)
{
    StringArray returnArray;
    var strings = cabbageIdentifiers.getWithDefault(prop, "");
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
    cabbageIdentifiers.remove(prop);
    cabbageIdentifiers.set(prop, value);
    //cabbageIdentifiers.set(prop, value);
}
//===================================================================
void CabbageWidget::setStringArrayPropValue(Identifier prop, int index, String value)
{
    var strings = cabbageIdentifiers.getWithDefault(prop, "");
    if(strings.size()>0)
        if(isPositiveAndBelow(index,strings.size()))
        {
            strings.getArray()->set(index, value);
            cabbageIdentifiers.set(prop, strings);
        }

}
//===================================================================
Array<int> CabbageWidget::getIntArrayProp(Identifier prop)
{
    Array<int> returnArray;
    var ints = cabbageIdentifiers.getWithDefault(prop, "");
    for(int i=0; i<ints.size(); i++)
        returnArray.add(ints[i]);
    return returnArray;
}
//===================================================================
var CabbageWidget::getVarArrayProp(Identifier prop)
{
    return cabbageIdentifiers.getWithDefault(prop, "");
}
//===================================================================
int CabbageWidget::getIntArrayPropValue(Identifier prop, int index)
{
    Array<int> returnArray;
    var ints = cabbageIdentifiers.getWithDefault(prop, "");
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
    var ints = cabbageIdentifiers.getWithDefault(prop, "");
    for(int i=0; i<ints.size(); i++)
        returnArray.add(ints[i]);
    return returnArray;
}
//===================================================================
float CabbageWidget::getFloatArrayPropValue(Identifier prop, int index)
{
    Array<float> returnArray;
    var ints = cabbageIdentifiers.getWithDefault(prop, "");
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
    Rectangle<int> rect(cabbageIdentifiers.getWithDefault("left", 0),
                        cabbageIdentifiers.getWithDefault("top", 0),
                        cabbageIdentifiers.getWithDefault("width", 10),
                        cabbageIdentifiers.getWithDefault("height", 10));

    cabbageIdentifiers.set(CabbageIdentifierIds::left, rect.getX()*scale.x);
    cabbageIdentifiers.set(CabbageIdentifierIds::top, rect.getY()*scale.y);
    cabbageIdentifiers.set(CabbageIdentifierIds::width, rect.getWidth()*scale.x);
    cabbageIdentifiers.set(CabbageIdentifierIds::height, rect.getHeight()*scale.y);	;
}
//===================================================================
void CabbageWidget::setStringProp(Identifier prop, String val)
{
//	CabbageUtilities::debug(prop.toString());
//	CabbageUtilities::debug(val);

    cabbageIdentifiers.set(prop, val);
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
