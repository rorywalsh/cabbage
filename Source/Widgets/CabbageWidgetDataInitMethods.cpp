/*
  Copyright (C) 2016 Rory Walsh

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

//===========================================================================================
void CabbageWidgetData::setFormProperties (ValueTree widgetData, int ID)
{
    setProperty (widgetData, CabbageIdentifierIds::basetype, "layout");
    setProperty (widgetData, CabbageIdentifierIds::top, 10);
    setProperty (widgetData, CabbageIdentifierIds::left, 10);
    setProperty (widgetData, CabbageIdentifierIds::width, 600);
    setProperty (widgetData, CabbageIdentifierIds::height, 300);
    setProperty (widgetData, CabbageIdentifierIds::caption, "");
    setProperty (widgetData, CabbageIdentifierIds::name, "form");
    setProperty (widgetData, CabbageIdentifierIds::type, "form");
    setProperty (widgetData, CabbageIdentifierIds::guirefresh, 128);
    setProperty (widgetData, CabbageIdentifierIds::identchannel, "");
    setProperty (widgetData, CabbageIdentifierIds::visible, 1);
    setProperty (widgetData, CabbageIdentifierIds::scrollbars, 0);
    setProperty (widgetData, CabbageIdentifierIds::titlebarcolour, Colour(57, 70, 76).toString());
    setProperty (widgetData, CabbageIdentifierIds::titlebargradient, 0.15f);
    setProperty (widgetData, CabbageIdentifierIds::style, "");
    setProperty (widgetData, CabbageIdentifierIds::channeltype, "number");
	setProperty(widgetData, CabbageIdentifierIds::sidechain, 0);
    setProperty (widgetData, CabbageIdentifierIds::fontcolour, "");
    setProperty (widgetData, CabbageIdentifierIds::colour, CabbageUtilities::getBackgroundSkin().toString());
}

void CabbageWidgetData::setHRangeSliderProperties (ValueTree widgetData, int ID)
{
    setProperty (widgetData, CabbageIdentifierIds::basetype, "interactive");
    setProperty (widgetData, CabbageIdentifierIds::top, 10);
    setProperty (widgetData, CabbageIdentifierIds::left, 10);
    setProperty (widgetData, CabbageIdentifierIds::width, 250);
    setProperty (widgetData, CabbageIdentifierIds::height, 40);
    setProperty (widgetData, CabbageIdentifierIds::textboxoutlinecolour, Colour (0xb2808080).toString());
    setProperty (widgetData, CabbageIdentifierIds::channeltype, "number");
    var channels;
    channels.append ("rangesliderMin");
    channels.append ("rangesliderMax");
    setProperty (widgetData, CabbageIdentifierIds::channel, channels);
    setProperty (widgetData, CabbageIdentifierIds::min, 0);
    setProperty (widgetData, CabbageIdentifierIds::max, 1);
    setProperty (widgetData, CabbageIdentifierIds::value, 0);
    setProperty (widgetData, CabbageIdentifierIds::text, "");
    setProperty (widgetData, CabbageWidgetTypes::textbox, 0.f);
    setProperty (widgetData, CabbageIdentifierIds::textboxcolour, Colour (0.f, 0.f, 0.f, 0.f).toString());
    setProperty (widgetData, CabbageIdentifierIds::caption, "");
    setProperty (widgetData, CabbageIdentifierIds::colour, Colours::white.toString());
    setProperty (widgetData, CabbageIdentifierIds::trackercolour, Colour (0, 118, 38).toString());
    setProperty (widgetData, CabbageIdentifierIds::fontcolour, CabbageUtilities::getComponentFontColour().toString());
    setProperty (widgetData, CabbageIdentifierIds::textcolour, CabbageUtilities::getComponentFontColour().toString());
    setProperty (widgetData, CabbageIdentifierIds::sliderskew, 1);
    setProperty (widgetData, CabbageIdentifierIds::increment, .001);
    setProperty (widgetData, CabbageIdentifierIds::midichan, -99);
    setProperty (widgetData, CabbageIdentifierIds::midictrl, -99);
    setProperty (widgetData, CabbageIdentifierIds::name, "hrange");
    setProperty (widgetData, CabbageIdentifierIds::type, getProperty (widgetData, "name").toString());
    setProperty (widgetData, CabbageIdentifierIds::name, getProperty (widgetData, "name").toString() + String (ID));
    setProperty (widgetData, CabbageIdentifierIds::kind, "horizontal");
    setProperty (widgetData, CabbageIdentifierIds::decimalplaces, 1);
    setProperty (widgetData, CabbageIdentifierIds::trackerthickness, .1);
    setProperty (widgetData, CabbageIdentifierIds::identchannel, "");
    setProperty (widgetData, CabbageIdentifierIds::visible, 1);
    setProperty (widgetData, CabbageIdentifierIds::imgslider, "");
    setProperty (widgetData, CabbageIdentifierIds::imgsliderbg, "");
    setProperty (widgetData, CabbageIdentifierIds::style, "flat");
    setProperty (widgetData, CabbageIdentifierIds::automatable, 1.0f);
}

void CabbageWidgetData::setVRangeSliderProperties (ValueTree widgetData, int ID)
{
    setProperty (widgetData, CabbageIdentifierIds::basetype, "interactive");
    setProperty (widgetData, CabbageIdentifierIds::top, 10);
    setProperty (widgetData, CabbageIdentifierIds::left, 10);
    setProperty (widgetData, CabbageIdentifierIds::width, 50);
    setProperty (widgetData, CabbageIdentifierIds::height, 150);
    setProperty (widgetData, CabbageIdentifierIds::textboxoutlinecolour, Colour (0xb2808080).toString());
    var channels;
    channels.append ("rangeslider");
    setProperty (widgetData, CabbageIdentifierIds::channel, channels);
    setProperty (widgetData, CabbageIdentifierIds::min, 0);
    setProperty (widgetData, CabbageIdentifierIds::channeltype, "number");
    setProperty (widgetData, CabbageIdentifierIds::max, 1);
    setProperty (widgetData, CabbageIdentifierIds::value, 0);
    setProperty (widgetData, CabbageIdentifierIds::text, "");
    setProperty (widgetData, CabbageWidgetTypes::textbox, 0.f);
    setProperty (widgetData, CabbageIdentifierIds::caption, "");
    setProperty (widgetData, CabbageIdentifierIds::colour, Colours::white.toString());
    setProperty (widgetData, CabbageIdentifierIds::textboxcolour, Colour (0.f, 0.f, 0.f, 0.f).toString());
    setProperty (widgetData, CabbageIdentifierIds::trackercolour, Colour (0, 118, 38).toString());
    setProperty (widgetData, CabbageIdentifierIds::fontcolour, CabbageUtilities::getComponentFontColour().toString());
    setProperty (widgetData, CabbageIdentifierIds::textcolour, CabbageUtilities::getComponentFontColour().toString());
    setProperty (widgetData, CabbageIdentifierIds::sliderskew, 1);
    setProperty (widgetData, CabbageIdentifierIds::increment, .001);
    setProperty (widgetData, CabbageIdentifierIds::midichan, -99);
    setProperty (widgetData, CabbageIdentifierIds::midictrl, -99);
    setProperty (widgetData, CabbageIdentifierIds::name, "vrange");
    setProperty (widgetData, CabbageIdentifierIds::type, getProperty (widgetData, "name").toString());
    setProperty (widgetData, CabbageIdentifierIds::name, getProperty (widgetData, "name").toString() + String (ID));
    setProperty (widgetData, CabbageIdentifierIds::kind, "vertical");
    setProperty (widgetData, CabbageIdentifierIds::decimalplaces, 1);
    setProperty (widgetData, CabbageIdentifierIds::trackerthickness, .1);
    setProperty (widgetData, CabbageIdentifierIds::identchannel, "");
    setProperty (widgetData, CabbageIdentifierIds::visible, 1);
    setProperty (widgetData, CabbageIdentifierIds::imgslider, "");
    setProperty (widgetData, CabbageIdentifierIds::imgsliderbg, "");
    setProperty (widgetData, CabbageIdentifierIds::style, "flat");
    setProperty (widgetData, CabbageIdentifierIds::automatable, 1.0f);
}

void CabbageWidgetData::setRSliderProperties (ValueTree widgetData, int ID)
{
    setProperty (widgetData, CabbageIdentifierIds::basetype, "interactive");
    setProperty (widgetData, CabbageIdentifierIds::top, 10);
    setProperty (widgetData, CabbageIdentifierIds::left, 10);
    setProperty (widgetData, CabbageIdentifierIds::width, 60);
    setProperty (widgetData, CabbageIdentifierIds::height, 60);
    setProperty (widgetData, CabbageIdentifierIds::textboxoutlinecolour, Colour (0xb2808080).toString());
    var channels;
    channels.append ("rslider");
    setProperty (widgetData, CabbageIdentifierIds::channel, channels);
    setProperty (widgetData, CabbageIdentifierIds::min, 0);
    setProperty (widgetData, CabbageIdentifierIds::channeltype, "number");
    setProperty (widgetData, CabbageIdentifierIds::max, 1);
    setProperty (widgetData, CabbageIdentifierIds::value, 0);
    setProperty (widgetData, CabbageIdentifierIds::sliderskew, 1);
    setProperty (widgetData, CabbageIdentifierIds::increment, .001);
    setProperty (widgetData, CabbageIdentifierIds::text, "");
    setProperty (widgetData, CabbageIdentifierIds::valuetextbox, 0.f);
    setProperty (widgetData, CabbageIdentifierIds::textboxcolour, Colour (0.f, 0.f, 0.f, 0.f).toString());
    setProperty (widgetData, CabbageIdentifierIds::caption, "");
    setProperty (widgetData, CabbageIdentifierIds::colour, Colours::whitesmoke.toString());
    setProperty (widgetData, CabbageIdentifierIds::trackercolour, Colour (0, 118, 38).toString());
    setProperty (widgetData, CabbageIdentifierIds::trackerbgcolour, Colour (0, 0, 0).toString());
    setProperty (widgetData, CabbageIdentifierIds::markercolour, Colour(80, 80, 80).toString());
    setProperty (widgetData, CabbageIdentifierIds::markerthickness, 1.0f);
    setProperty (widgetData, CabbageIdentifierIds::markerstart, 0.5f);
    setProperty (widgetData, CabbageIdentifierIds::markerend, 0.9f);
    setProperty (widgetData, CabbageIdentifierIds::fontcolour, CabbageUtilities::getComponentFontColour().toString());
    setProperty (widgetData, CabbageIdentifierIds::textcolour, CabbageUtilities::getComponentFontColour().toString());
    setProperty (widgetData, CabbageIdentifierIds::outlinecolour, Colours::black.brighter (.3f).toString());
    setProperty (widgetData, CabbageIdentifierIds::midichan, -99);
    setProperty (widgetData, CabbageIdentifierIds::midictrl, -99);
    setProperty (widgetData, CabbageIdentifierIds::name, "rslider");
    setProperty (widgetData, CabbageIdentifierIds::type, getProperty (widgetData, "name").toString());
    setProperty (widgetData, CabbageIdentifierIds::name, getProperty (widgetData, "name").toString() + String (ID));
    setProperty (widgetData, CabbageIdentifierIds::kind, "rotary");
    setProperty (widgetData, CabbageIdentifierIds::decimalplaces, 1);
    setProperty (widgetData, CabbageIdentifierIds::velocity, 0);
    setProperty (widgetData, CabbageIdentifierIds::identchannel, "");
    setProperty (widgetData, CabbageIdentifierIds::trackerthickness, 1.f);
    setProperty (widgetData, CabbageIdentifierIds::trackerinsideradius, .7f);
    setProperty (widgetData, CabbageIdentifierIds::trackeroutsideradius, 1.f);
    setProperty (widgetData, CabbageIdentifierIds::visible, 1);
    setProperty (widgetData, CabbageIdentifierIds::imgslider, "");
    setProperty (widgetData, CabbageIdentifierIds::imgsliderbg, "");
	setProperty (widgetData, CabbageIdentifierIds::style, "flat");
    setProperty (widgetData, CabbageIdentifierIds::automatable, 1.0f);
}

void CabbageWidgetData::setLineProperties (ValueTree widgetData, int ID)
{
    setProperty (widgetData, CabbageIdentifierIds::basetype, "layout");
    setProperty (widgetData, CabbageIdentifierIds::top, 10);
    setProperty (widgetData, CabbageIdentifierIds::left, 10);
    setProperty (widgetData, CabbageIdentifierIds::width, 160);
    setProperty (widgetData, CabbageIdentifierIds::height, 2);
    setProperty (widgetData, CabbageIdentifierIds::channeltype, "number");
    setProperty (widgetData, CabbageIdentifierIds::colour, Colours::white.toString());
    setProperty (widgetData, CabbageIdentifierIds::type, "line");
    setProperty (widgetData, CabbageIdentifierIds::name, getProperty (widgetData, "name").toString() + String (ID));
    setProperty (widgetData, CabbageIdentifierIds::identchannel, "");
    setProperty (widgetData, CabbageIdentifierIds::visible, 1);
}

void CabbageWidgetData::setScrewProperties (ValueTree widgetData, int ID)
{
    setProperty (widgetData, CabbageIdentifierIds::basetype, "layout");
    setProperty (widgetData, CabbageIdentifierIds::top, 10);
    setProperty (widgetData, CabbageIdentifierIds::left, 10);
    setProperty (widgetData, CabbageIdentifierIds::width, 30);
    setProperty (widgetData, CabbageIdentifierIds::height, 30);
    setProperty (widgetData, CabbageIdentifierIds::type, "screw");
    setProperty (widgetData, CabbageIdentifierIds::name, getProperty (widgetData, "name").toString() + String (ID));
    setProperty (widgetData, CabbageIdentifierIds::visible, 1);
}

void CabbageWidgetData::setPortProperties (ValueTree widgetData, int ID, const String type)
{
    setProperty (widgetData, CabbageIdentifierIds::basetype, "layout");
    setProperty (widgetData, CabbageIdentifierIds::top, 10);
    setProperty (widgetData, CabbageIdentifierIds::left, 10);
    setProperty (widgetData, CabbageIdentifierIds::width, 30);
    setProperty (widgetData, CabbageIdentifierIds::height, 30);
    setProperty (widgetData, CabbageIdentifierIds::type, type);
    setProperty (widgetData, CabbageIdentifierIds::name, getProperty (widgetData, "name").toString() + String (ID));
    setProperty (widgetData, CabbageIdentifierIds::visible, 1);
}

void CabbageWidgetData::setLightProperties (ValueTree widgetData, int ID)
{
    setProperty (widgetData, CabbageIdentifierIds::basetype, "layout");
    setProperty (widgetData, CabbageIdentifierIds::top, 10);
    setProperty (widgetData, CabbageIdentifierIds::left, 10);
    setProperty (widgetData, CabbageIdentifierIds::width, 30);
    setProperty (widgetData, CabbageIdentifierIds::height, 30);
    setProperty (widgetData, CabbageIdentifierIds::colour, Colours::lime.toString());
    setProperty (widgetData, CabbageIdentifierIds::type, "light");
    setProperty (widgetData, CabbageIdentifierIds::name, getProperty (widgetData, "name").toString() + String (ID));
    setProperty (widgetData, CabbageIdentifierIds::visible, 1);
}

void CabbageWidgetData::setEventSequencerProperties (ValueTree widgetData, int ID)
{
    setProperty (widgetData, CabbageIdentifierIds::basetype, "layout");
    setProperty (widgetData, CabbageIdentifierIds::top, 10);
    setProperty (widgetData, CabbageIdentifierIds::left, 10);
    setProperty (widgetData, CabbageIdentifierIds::width, 160);
    setProperty (widgetData, CabbageIdentifierIds::height, 2);
    setProperty (widgetData, CabbageIdentifierIds::channeltype, "number");
    setProperty (widgetData, CabbageIdentifierIds::colour, Colours::white.toString());
    setProperty (widgetData, CabbageIdentifierIds::type, "eventsequencer");
    setProperty (widgetData, CabbageIdentifierIds::backgroundcolour, Colours::white.toString());
    setProperty (widgetData, CabbageIdentifierIds::activecellcolour, Colours::white.toString());
    setProperty (widgetData, CabbageIdentifierIds::highlightcolour, Colours::cornflowerblue.toString());
    setProperty (widgetData, CabbageIdentifierIds::fontcolour, Colours::black.toString());
    setProperty (widgetData, CabbageIdentifierIds::textcolour, Colours::black.toString());
    setProperty (widgetData, CabbageIdentifierIds::name, getProperty (widgetData, "name").toString() + String (ID));
    setProperty (widgetData, CabbageIdentifierIds::identchannel, "");
    setProperty (widgetData, CabbageIdentifierIds::orientation, "vertical");
    setProperty (widgetData, CabbageIdentifierIds::matrixcols, 2);
    setProperty (widgetData, CabbageIdentifierIds::matrixrows, 8);
    setProperty (widgetData, CabbageIdentifierIds::visible, 1);
    setProperty (widgetData, CabbageIdentifierIds::value, 1);
    setProperty (widgetData, CabbageIdentifierIds::numberofsteps, 16);
    setProperty (widgetData, CabbageIdentifierIds::bpm, 60);
    setProperty (widgetData, CabbageIdentifierIds::cellwidth, 0);
    setProperty (widgetData, CabbageIdentifierIds::cellheight, 0);
}

void CabbageWidgetData::setXYPadProperties (ValueTree widgetData, int ID)
{
    setProperty (widgetData, "basetype", "interactive");
    var channels;
    channels.append ("xChan");
    channels.append ("yChan");
    setProperty (widgetData, CabbageIdentifierIds::xyautoindex, 0);
    setProperty (widgetData, CabbageIdentifierIds::channel, channels);
    setProperty (widgetData, CabbageIdentifierIds::top, 10);
    setProperty (widgetData, CabbageIdentifierIds::left, 10);
    setProperty (widgetData, CabbageIdentifierIds::caption, "");
    setProperty (widgetData, CabbageIdentifierIds::width, 200);
    setProperty (widgetData, CabbageIdentifierIds::height, 200);
    setProperty (widgetData, CabbageIdentifierIds::minx, 0);
    setProperty (widgetData, CabbageIdentifierIds::maxx, 1);
    setProperty (widgetData, CabbageIdentifierIds::text, "");
    setProperty (widgetData, CabbageIdentifierIds::miny, 0);
    setProperty (widgetData, CabbageIdentifierIds::maxy, 1);
    setProperty (widgetData, CabbageIdentifierIds::valuex, 0);
    setProperty (widgetData, CabbageIdentifierIds::channeltype, "number");
    setProperty (widgetData, CabbageIdentifierIds::valuey, 0);
    setProperty (widgetData, CabbageIdentifierIds::ballcolour, Colours::lime.toString());
    setProperty (widgetData, CabbageIdentifierIds::fontcolour, Colours::cornflowerblue.toString());
    setProperty (widgetData, CabbageIdentifierIds::textcolour, Colours::cornflowerblue.toString());
    setProperty (widgetData, CabbageIdentifierIds::backgroundcolour, CabbageUtilities::getComponentSkin().toString());
    setProperty (widgetData, CabbageIdentifierIds::colour, CabbageUtilities::getComponentSkin().toString());
    setProperty (widgetData, CabbageIdentifierIds::type, "xypad");
    setProperty (widgetData, CabbageIdentifierIds::name, "xypad");
    setProperty (widgetData, CabbageIdentifierIds::name, getProperty (widgetData, "name").toString() + String (ID));
    setProperty (widgetData, CabbageIdentifierIds::identchannel, "");
    setProperty (widgetData, CabbageIdentifierIds::visible, 1);
    setProperty (widgetData, CabbageIdentifierIds::automatable, 1.0f);
}

void CabbageWidgetData::setSignalDisplayProperties (ValueTree widgetData, int ID)
{
    setProperty (widgetData, "basetype", "layout");
    var signalVariables;
    signalVariables.append ("");
    setProperty (widgetData, CabbageIdentifierIds::top, 10);
    setProperty (widgetData, CabbageIdentifierIds::left, 10);
    setProperty (widgetData, CabbageIdentifierIds::width, 260);
    setProperty (widgetData, CabbageIdentifierIds::height, 100);
    setProperty (widgetData, CabbageIdentifierIds::tablecolour, Colours::lime.toString());
    setProperty (widgetData, CabbageIdentifierIds::fontcolour, Colours::white.toString());
    setProperty (widgetData, CabbageIdentifierIds::tablebackgroundcolour, Colour (30, 30, 38).toString());
    setProperty (widgetData, CabbageIdentifierIds::type, "signaldisplay");
    setProperty (widgetData, CabbageIdentifierIds::channeltype, "number");
    setProperty (widgetData, CabbageIdentifierIds::name, "signaldisplay");
    setProperty (widgetData, CabbageIdentifierIds::min, 0);
    setProperty (widgetData, CabbageIdentifierIds::max, 2048);
    setProperty (widgetData, CabbageIdentifierIds::updaterate, 100);
    setProperty (widgetData, CabbageIdentifierIds::signalvariable, signalVariables);
    setProperty (widgetData, CabbageIdentifierIds::outlinethickness, 1);
    setProperty (widgetData, CabbageIdentifierIds::name, getProperty (widgetData, "name").toString() + String (ID));
    setProperty (widgetData, CabbageIdentifierIds::identchannel, "");
    setProperty (widgetData, CabbageIdentifierIds::displaytype, "spectroscope");
    setProperty (widgetData, CabbageIdentifierIds::zoom, 0);
    setProperty (widgetData, CabbageIdentifierIds::visible, 1);
    setProperty (widgetData, CabbageIdentifierIds::updaterate, 100);
}

void CabbageWidgetData::setLoadButtonProperties (ValueTree widgetData, int ID)
{
    setProperty (widgetData, CabbageIdentifierIds::top, 10);
    setProperty (widgetData, CabbageIdentifierIds::left, 10);
    setProperty (widgetData, CabbageIdentifierIds::width, 80);
    setProperty (widgetData, CabbageIdentifierIds::height, 40);
    setProperty (widgetData, CabbageIdentifierIds::text, "");
    setProperty (widgetData, CabbageIdentifierIds::caption, "");
    setProperty (widgetData, CabbageIdentifierIds::colour, Colours::black.toString());
    setProperty (widgetData, CabbageIdentifierIds::fontcolour, Colours::white.toString());
    setProperty (widgetData, CabbageIdentifierIds::oncolour, Colours::black.toString());
    setProperty (widgetData, CabbageIdentifierIds::onfontcolour, Colours::white.toString());
    setProperty (widgetData, CabbageIdentifierIds::type, "loadbutton");
    setProperty (widgetData, CabbageIdentifierIds::channeltype, "number");
    setProperty (widgetData, CabbageIdentifierIds::name, "loadbutton");
    setProperty (widgetData, CabbageIdentifierIds::name, getProperty (widgetData, "name").toString() + String (ID));
    setProperty (widgetData, CabbageIdentifierIds::identchannel, "");
    setProperty (widgetData, CabbageIdentifierIds::visible, 1);
    setProperty (widgetData, CabbageIdentifierIds::imgbuttonon, "");
    setProperty (widgetData, CabbageIdentifierIds::imgbuttonoff, "");
}

void CabbageWidgetData::setKeyboardProperties (ValueTree widgetData, int ID, bool displayOnly)
{
    setProperty (widgetData, "basetype", "layout");
    setProperty (widgetData, CabbageIdentifierIds::top, 10);
    setProperty (widgetData, CabbageIdentifierIds::left, 10);
    setProperty (widgetData, CabbageIdentifierIds::width, 400);
    setProperty (widgetData, CabbageIdentifierIds::height, 100);
    setProperty (widgetData, CabbageIdentifierIds::value, 60);
    setProperty (widgetData, CabbageIdentifierIds::middlec, 3);
    setProperty (widgetData, CabbageIdentifierIds::keypressbaseoctave, 3);
    setProperty (widgetData, CabbageIdentifierIds::type, displayOnly == true ? "keyboarddisplay" : "keyboard");
    setProperty (widgetData, CabbageIdentifierIds::name, displayOnly == true ? "keyboarddisplay" : "keyboard");
    if(displayOnly)
    {
        var array;
        setProperty (widgetData, CabbageIdentifierIds::keypressed, array);
        setProperty (widgetData, CabbageIdentifierIds::keyreleased, array);
    }
    setProperty (widgetData, CabbageIdentifierIds::kind, "horizontal");
    setProperty (widgetData, CabbageIdentifierIds::channeltype, "number");
    setProperty (widgetData, CabbageIdentifierIds::whitenotecolour, Colours::whitesmoke.toString());
    setProperty (widgetData, CabbageIdentifierIds::blacknotecolour, Colours::black.toString());
    setProperty (widgetData, CabbageIdentifierIds::keyseparatorcolour, Colour (0x66000000).toString());
    setProperty (widgetData, CabbageIdentifierIds::arrowbackgroundcolour, Colour (0xffd3d3d3).toString());
    setProperty (widgetData, CabbageIdentifierIds::mouseoverkeycolour, Colour (0x80ffff00).toString());
    setProperty (widgetData, CabbageIdentifierIds::keydowncolour, Colour (0x80ffff00).toString());
    setProperty (widgetData, CabbageIdentifierIds::arrowcolour, Colour (0xff000000).toString());
    setProperty (widgetData, CabbageIdentifierIds::keydowncolour, Colour (0x80ffff00).toString());
    setProperty (widgetData, CabbageIdentifierIds::name, getProperty (widgetData, "name").toString() + String (ID));
    setProperty (widgetData, CabbageIdentifierIds::visible, 1);
    setProperty (widgetData, CabbageIdentifierIds::keywidth, 16);
    setProperty (widgetData, CabbageIdentifierIds::scrollbars, 1);
}

void CabbageWidgetData::setCsoundOutputProperties (ValueTree widgetData, int ID)
{
    setProperty (widgetData, "basetype", "layout");
    setProperty (widgetData, CabbageIdentifierIds::top, 10);
    setProperty (widgetData, CabbageIdentifierIds::left, 10);
    setProperty (widgetData, CabbageIdentifierIds::width, 400);
    setProperty (widgetData, CabbageIdentifierIds::text, "Csound output");
    setProperty (widgetData, CabbageIdentifierIds::height, 200);
    setProperty (widgetData, CabbageIdentifierIds::colour, Colours::black.toString());
    setProperty (widgetData, CabbageIdentifierIds::fontcolour, Colours::cornflowerblue.toString());
    setProperty (widgetData, CabbageIdentifierIds::name, "csoundoutput");
    setProperty (widgetData, CabbageIdentifierIds::channeltype, "number");
    setProperty (widgetData, CabbageIdentifierIds::wrap, 0);
    setProperty (widgetData, CabbageIdentifierIds::type, "csoundoutput");
    setProperty (widgetData, CabbageIdentifierIds::name, getProperty (widgetData, "name").toString() + String (ID));
    setProperty (widgetData, CabbageIdentifierIds::identchannel, "");
    setProperty (widgetData, CabbageIdentifierIds::visible, 1);
    setProperty (widgetData, CabbageIdentifierIds::monospaced, 0);
}

void CabbageWidgetData::setButtonProperties (ValueTree widgetData, int ID)
{
    setProperty (widgetData, "basetype", "interactive");
    var array;
    array.append ("Push me");
    array.append ("Push me");
    setProperty (widgetData, CabbageIdentifierIds::top, 10);
    setProperty (widgetData, CabbageIdentifierIds::left, 10);
    setProperty (widgetData, CabbageIdentifierIds::width, 80);
    setProperty (widgetData, CabbageIdentifierIds::height, 40);
    var channels;
    channels.append ("buttonchan");
    setProperty (widgetData, CabbageIdentifierIds::channel, channels);
    setProperty (widgetData, CabbageIdentifierIds::value, 0);
    setProperty (widgetData, CabbageIdentifierIds::min, 0);
    setProperty (widgetData, CabbageIdentifierIds::max, 1);
    setProperty (widgetData, CabbageIdentifierIds::text, array);
    setProperty (widgetData, CabbageIdentifierIds::caption, "");
    setProperty (widgetData, CabbageIdentifierIds::colour, Colours::black.toString());
    setProperty (widgetData, CabbageIdentifierIds::fontcolour, Colours::white.toString());
    setProperty (widgetData, CabbageIdentifierIds::oncolour, Colours::black.toString());
    setProperty (widgetData, CabbageIdentifierIds::onfontcolour, Colours::white.toString());
    setProperty (widgetData, CabbageIdentifierIds::latched, 1);
    setProperty (widgetData, CabbageIdentifierIds::type, "button");
	setProperty(widgetData, CabbageIdentifierIds::popuptext, "");
    setProperty (widgetData, CabbageIdentifierIds::name, "button");
    setProperty (widgetData, CabbageIdentifierIds::channeltype, "number");
    setProperty (widgetData, CabbageIdentifierIds::name, getProperty (widgetData, "name").toString() + String (ID));
    setProperty (widgetData, CabbageIdentifierIds::identchannel, "");
    setProperty (widgetData, CabbageIdentifierIds::radiogroup, "");
    setProperty (widgetData, CabbageIdentifierIds::visible, 1);
    setProperty (widgetData, CabbageIdentifierIds::imgbuttonon, "");
    setProperty (widgetData, CabbageIdentifierIds::imgbuttonoff, "");
	setProperty(widgetData, CabbageIdentifierIds::style, "flat");
    setProperty (widgetData, CabbageIdentifierIds::automatable, 1.0f);
}

void CabbageWidgetData::setFileButtonProperties (ValueTree widgetData, int ID)
{
    setProperty (widgetData, "basetype", "layout");
    var array;
    array.append ("Open file");
    array.append ("Open file");
    setProperty (widgetData, CabbageIdentifierIds::top, 10);
    setProperty (widgetData, CabbageIdentifierIds::left, 10);
    setProperty (widgetData, CabbageIdentifierIds::width, 80);
    setProperty (widgetData, CabbageIdentifierIds::height, 40);
    var channels;
    channels.append ("filebutton");
    setProperty (widgetData, CabbageIdentifierIds::channel, channels);
    setProperty (widgetData, CabbageIdentifierIds::value, 1);
    setProperty (widgetData, CabbageIdentifierIds::text, array);
    setProperty (widgetData, CabbageIdentifierIds::caption, "");
	setProperty(widgetData, CabbageIdentifierIds::popuptext, "");
    setProperty (widgetData, CabbageIdentifierIds::colour, Colours::black.toString());
    setProperty (widgetData, CabbageIdentifierIds::fontcolour, Colours::white.toString());
    setProperty (widgetData, CabbageIdentifierIds::oncolour, Colours::black.toString());
    setProperty (widgetData, CabbageIdentifierIds::onfontcolour, Colours::white.toString());
    setProperty (widgetData, CabbageIdentifierIds::type, "filebutton");
    setProperty (widgetData, CabbageIdentifierIds::name, "filebutton");
    setProperty (widgetData, CabbageIdentifierIds::channeltype, "string");
    setProperty (widgetData, CabbageIdentifierIds::mode, "file");
    setProperty (widgetData, CabbageIdentifierIds::identchannel, "");
    setProperty (widgetData, "name", getProperty (widgetData, "name").toString() + String (ID));
    var populate;
    populate.append ("");
    populate.append ("");
    setProperty (widgetData, "populate", populate);
    setProperty (widgetData, CabbageIdentifierIds::visible, 1);
    setProperty (widgetData, CabbageIdentifierIds::filetype, "*");
    setProperty (widgetData, CabbageIdentifierIds::imgbuttonon, "");
    setProperty (widgetData, CabbageIdentifierIds::imgbuttonoff, "");
	setProperty(widgetData, CabbageIdentifierIds::style, "flat");
}

void CabbageWidgetData::setInfoButtonProperties (ValueTree widgetData, int ID)
{
    setProperty (widgetData, "basetype", "layout");
    setProperty (widgetData, CabbageIdentifierIds::top, 10);
    setProperty (widgetData, CabbageIdentifierIds::left, 10);
    setProperty (widgetData, CabbageIdentifierIds::width, 80);
    setProperty (widgetData, CabbageIdentifierIds::height, 40);
    setProperty (widgetData, CabbageIdentifierIds::text, "");
    setProperty (widgetData, CabbageIdentifierIds::caption, "");
	setProperty(widgetData, CabbageIdentifierIds::popuptext, "");
    setProperty (widgetData, CabbageIdentifierIds::channeltype, "number");
    setProperty (widgetData, CabbageIdentifierIds::colour, Colours::black.toString());
    setProperty (widgetData, CabbageIdentifierIds::fontcolour, Colours::white.toString());
    setProperty (widgetData, CabbageIdentifierIds::oncolour, Colours::black.toString());
    setProperty (widgetData, CabbageIdentifierIds::onfontcolour, Colours::white.toString());
    setProperty (widgetData, CabbageIdentifierIds::type, "infobutton");
    setProperty (widgetData, CabbageIdentifierIds::name, "infobutton");
    setProperty (widgetData, CabbageIdentifierIds::channeltype, "number");
    setProperty (widgetData, CabbageIdentifierIds::name, getProperty (widgetData, "name").toString() + String (ID));
    setProperty (widgetData, CabbageIdentifierIds::identchannel, "");
    setProperty (widgetData, CabbageIdentifierIds::visible, 1);
    setProperty (widgetData, CabbageIdentifierIds::imgbuttonon, "");
    setProperty (widgetData, CabbageIdentifierIds::imgbuttonoff, "");
	setProperty(widgetData, CabbageIdentifierIds::style, "flat");
}
void CabbageWidgetData::setTextBoxProperties (ValueTree widgetData, int ID)
{
    setProperty (widgetData, "basetype", "layout");
    setProperty (widgetData, CabbageIdentifierIds::top, 10);
    setProperty (widgetData, CabbageIdentifierIds::left, 10);
    setProperty (widgetData, CabbageIdentifierIds::width, 400);
    setProperty (widgetData, CabbageIdentifierIds::text, "");
    setProperty (widgetData, CabbageIdentifierIds::height, 200);
    setProperty (widgetData, CabbageIdentifierIds::colour, Colours::black.toString());
    setProperty (widgetData, CabbageIdentifierIds::fontcolour, Colours::white.toString());
    setProperty (widgetData, CabbageIdentifierIds::name, "textbox");
    setProperty (widgetData, CabbageIdentifierIds::type, "textbox");
    setProperty (widgetData, CabbageIdentifierIds::wrap, 0);
    setProperty (widgetData, CabbageIdentifierIds::file, "");
    setProperty (widgetData, CabbageIdentifierIds::channeltype, "number");
    setProperty (widgetData, CabbageIdentifierIds::name, getProperty (widgetData, "name").toString() + String (ID));
    setProperty (widgetData, CabbageIdentifierIds::identchannel, "");
    setProperty (widgetData, CabbageIdentifierIds::visible, 1);
}

void CabbageWidgetData::setLabelProperties (ValueTree widgetData, int ID)
{
    setProperty (widgetData, "basetype", "layout");
    setProperty (widgetData, CabbageIdentifierIds::top, 10);
    setProperty (widgetData, CabbageIdentifierIds::left, 10);
    setProperty (widgetData, CabbageIdentifierIds::width, 80);
    setProperty (widgetData, CabbageIdentifierIds::height, 16);
    setProperty (widgetData, CabbageIdentifierIds::text, "hello");
    setProperty (widgetData, CabbageIdentifierIds::min, 1);
    setProperty (widgetData, CabbageIdentifierIds::name, "label");
    setProperty (widgetData, CabbageIdentifierIds::type, "label");
    setProperty (widgetData, CabbageIdentifierIds::channeltype, "number");
    setProperty (widgetData, CabbageIdentifierIds::type, getProperty (widgetData, "name").toString());
    setProperty (widgetData, CabbageIdentifierIds::name, getProperty (widgetData, "name").toString() + String (ID));
    setProperty (widgetData, CabbageIdentifierIds::colour, Colours::transparentBlack.toString());
    setProperty (widgetData, CabbageIdentifierIds::fontcolour, CabbageUtilities::getComponentFontColour().toString());
    setProperty (widgetData, CabbageIdentifierIds::align, "centre");
    setProperty (widgetData, CabbageIdentifierIds::fontstyle, 1);
    setProperty (widgetData, CabbageIdentifierIds::channel, "");
    setProperty (widgetData, CabbageIdentifierIds::identchannel, "");
    setProperty (widgetData, CabbageIdentifierIds::corners, 3);
    setProperty (widgetData, CabbageIdentifierIds::visible, 1);
}

void CabbageWidgetData::setListBoxProperties (ValueTree widgetData, int ID)
{
    setProperty (widgetData, "basetype", "interactive");
    var array;
    array.append ("Item 1");
    array.append ("Item 2");
    array.append ("Item 3");
    array.append ("Item 4");
    setProperty (widgetData, CabbageIdentifierIds::top, 10);
    setProperty (widgetData, CabbageIdentifierIds::left, 10);
    setProperty (widgetData, CabbageIdentifierIds::width, 180);
    setProperty (widgetData, CabbageIdentifierIds::height, 60);
    var channels;
    channels.append ("listboxchan");
    setProperty (widgetData, CabbageIdentifierIds::channel, channels);
    setProperty (widgetData, CabbageIdentifierIds::channeltype, "number");
    setProperty (widgetData, CabbageIdentifierIds::text, array);
    setProperty (widgetData, CabbageIdentifierIds::align, "left");
    setProperty (widgetData, CabbageIdentifierIds::value, 1);
    setProperty (widgetData, CabbageIdentifierIds::update, 1);
    setProperty (widgetData, CabbageIdentifierIds::caption, "");
    setProperty (widgetData, CabbageIdentifierIds::colour, Colours::black.toString());
    setProperty (widgetData, CabbageIdentifierIds::fontcolour, Colours::white.toString());
    setProperty (widgetData, CabbageIdentifierIds::highlightcolour, Colour(100, 100, 100).toString());
    setProperty (widgetData, CabbageIdentifierIds::type, "listbox");
    setProperty (widgetData, CabbageIdentifierIds::comborange, 4);
    setProperty (widgetData, CabbageIdentifierIds::file, "");
    var populate;
    setProperty (widgetData, CabbageIdentifierIds::populate, populate);
    setProperty (widgetData, CabbageIdentifierIds::name, "listbox");
    setProperty (widgetData, CabbageIdentifierIds::name, getProperty (widgetData, "name").toString() + String (ID));
    setProperty (widgetData, CabbageIdentifierIds::identchannel, "");
    setProperty (widgetData, CabbageIdentifierIds::visible, 1);
    setProperty (widgetData, CabbageIdentifierIds::workingdir, "");
    setProperty (widgetData, CabbageIdentifierIds::automatable, 1.0f);
}

void CabbageWidgetData::setMeterProperties (ValueTree widgetData, int ID, bool isVertical)
{
    var meterColours;
    meterColours.append (Colours::lime.toString());
    setProperty (widgetData, "basetype", "layout");
    setProperty (widgetData, CabbageIdentifierIds::top, 10);
    setProperty (widgetData, CabbageIdentifierIds::left, 10);
    setProperty (widgetData, CabbageIdentifierIds::width, 80);
    setProperty (widgetData, CabbageIdentifierIds::height, 16);
    setProperty (widgetData, CabbageIdentifierIds::channeltype, "number");
    setProperty (widgetData, CabbageIdentifierIds::name, isVertical == true ? "vmeter" : "hmeter");
    setProperty (widgetData, CabbageIdentifierIds::name, isVertical == true ? "vmeter" : "hmeter");
    setProperty (widgetData, CabbageIdentifierIds::orientation, isVertical == true ? "vertical" : "horizontal");

    setProperty (widgetData, CabbageIdentifierIds::type, getProperty (widgetData, "name").toString());
    setProperty (widgetData, CabbageIdentifierIds::name, getProperty (widgetData, "name").toString() + String (ID));
    setProperty (widgetData, CabbageIdentifierIds::overlaycolour, Colours::transparentBlack.toString());
    setProperty (widgetData, CabbageIdentifierIds::metercolour, meterColours);
    setProperty (widgetData, CabbageIdentifierIds::outlinecolour, Colours::white.toString());
    setProperty (widgetData, CabbageIdentifierIds::outlinethickness, 1);
    setProperty (widgetData, CabbageIdentifierIds::value, 1);
    setProperty (widgetData, CabbageIdentifierIds::corners, 3);
    setProperty (widgetData, CabbageIdentifierIds::channel, "");
    setProperty (widgetData, CabbageIdentifierIds::identchannel, "");
    setProperty (widgetData, CabbageIdentifierIds::visible, 1);
}

void CabbageWidgetData::setTextEditorProperties (ValueTree widgetData, int ID)
{
    setProperty (widgetData, "basetype", "layout");
    setProperty (widgetData, CabbageIdentifierIds::top, 10);
    setProperty (widgetData, CabbageIdentifierIds::left, 10);
    setProperty (widgetData, CabbageIdentifierIds::width, 100);
    setProperty (widgetData, CabbageIdentifierIds::text, "");
    setProperty (widgetData, CabbageIdentifierIds::height, 30);
    setProperty (widgetData, CabbageIdentifierIds::channel, "texteditor");
    setProperty (widgetData, CabbageIdentifierIds::colour, Colours::white.toString());
    setProperty (widgetData, CabbageIdentifierIds::fontcolour, Colours::black.toString());
    setProperty (widgetData, CabbageIdentifierIds::outlinecolour, Colours::transparentBlack.toString());
    setProperty (widgetData, CabbageIdentifierIds::name, "texteditor");
    setProperty (widgetData, CabbageIdentifierIds::type, "texteditor");
    setProperty (widgetData, CabbageIdentifierIds::channeltype, "string");
    setProperty (widgetData, CabbageIdentifierIds::name, getProperty (widgetData, "name").toString() + String (ID));
    setProperty (widgetData, CabbageIdentifierIds::identchannel, "");
    setProperty (widgetData, CabbageIdentifierIds::visible, 1);
}

void CabbageWidgetData::setSoundfilerProperties (ValueTree widgetData, int ID)
{
    setProperty (widgetData, "basetype", "layout");
    setProperty (widgetData, CabbageIdentifierIds::top, 10);
    setProperty (widgetData, CabbageIdentifierIds::left, 10);
    setProperty (widgetData, CabbageIdentifierIds::width, 300);
    setProperty (widgetData, CabbageIdentifierIds::height, 200);
    var channels;
    setProperty (widgetData, CabbageIdentifierIds::channel, channels);
    setProperty (widgetData, CabbageIdentifierIds::colour, CabbageUtilities::getComponentFontColour().toString());
    setProperty (widgetData, CabbageIdentifierIds::tablebackgroundcolour, Colours::black.toString());
    setProperty (widgetData, CabbageIdentifierIds::type, "soundfiler");
    setProperty (widgetData, CabbageIdentifierIds::name, "soundfiler");
    setProperty (widgetData, CabbageIdentifierIds::file, "");
    setProperty (widgetData, CabbageIdentifierIds::name, getProperty (widgetData, "name").toString() + String (ID));
    setProperty (widgetData, CabbageIdentifierIds::identchannel, "");
    setProperty (widgetData, CabbageIdentifierIds::visible, 1);
    setProperty (widgetData, CabbageIdentifierIds::scrubberposition, 0);
    setProperty (widgetData, CabbageIdentifierIds::channeltype, "number");
    setProperty (widgetData, CabbageIdentifierIds::zoom, -1);
    setProperty (widgetData, CabbageIdentifierIds::startpos, 0);
    setProperty (widgetData, CabbageIdentifierIds::tablenumber, -1);
}

void CabbageWidgetData::setEncoderProperties (ValueTree widgetData, int ID)
{
    setProperty (widgetData, CabbageIdentifierIds::basetype, "interactive");
    setProperty (widgetData, CabbageIdentifierIds::top, 10);
    setProperty (widgetData, CabbageIdentifierIds::left, 10);
    setProperty (widgetData, CabbageIdentifierIds::width, 60);
    setProperty (widgetData, CabbageIdentifierIds::height, 60);
    var channels;
    channels.append ("encoder");
    setProperty (widgetData, CabbageIdentifierIds::channel, channels);
    setProperty (widgetData, CabbageIdentifierIds::value, 0);
    setProperty (widgetData, CabbageIdentifierIds::increment, 1);
    setProperty (widgetData, CabbageIdentifierIds::channeltype, "number");
    setProperty (widgetData, CabbageIdentifierIds::text, "");
    setProperty (widgetData, CabbageWidgetTypes::textbox, 0.f);
    setProperty (widgetData, CabbageIdentifierIds::caption, "");
    setProperty (widgetData, CabbageIdentifierIds::colour, Colours::whitesmoke.toString());
    setProperty (widgetData, CabbageIdentifierIds::trackercolour, Colour (0, 118, 38).toString());
    setProperty (widgetData, CabbageIdentifierIds::fontcolour, CabbageUtilities::getComponentFontColour().toString());
    setProperty (widgetData, CabbageIdentifierIds::textcolour, CabbageUtilities::getComponentFontColour().toString());
    setProperty (widgetData, CabbageIdentifierIds::outlinecolour, Colours::black.brighter (.3f).toString());
    setProperty (widgetData, CabbageIdentifierIds::midichan, -99);
    setProperty (widgetData, CabbageIdentifierIds::midictrl, -99);
    //these don't appear in the props dialog
    setProperty (widgetData, CabbageIdentifierIds::name, "encoder");
    setProperty (widgetData, CabbageIdentifierIds::type, getProperty (widgetData, "name").toString());
    setProperty (widgetData, CabbageIdentifierIds::name, getProperty (widgetData, "name").toString() + String (ID));
    setProperty (widgetData, CabbageIdentifierIds::decimalplaces, 1);
    setProperty (widgetData, CabbageIdentifierIds::identchannel, "");
    setProperty (widgetData, CabbageIdentifierIds::trackerthickness, .05);
    setProperty (widgetData, CabbageIdentifierIds::visible, 1);
    setProperty (widgetData, CabbageIdentifierIds::minenabled, 0);
    setProperty (widgetData, CabbageIdentifierIds::maxenabled, 0);
    setProperty (widgetData, CabbageIdentifierIds::automatable, 1.0f);
}

void CabbageWidgetData::setGroupBoxProperties (ValueTree widgetData, int ID)
{
    setProperty (widgetData, CabbageIdentifierIds::basetype, "layout");
    setProperty (widgetData, CabbageIdentifierIds::top, 10);
    setProperty (widgetData, CabbageIdentifierIds::left, 10);
    setProperty (widgetData, CabbageIdentifierIds::width, 180);
    setProperty (widgetData, CabbageIdentifierIds::height, 122);
    setProperty (widgetData, CabbageIdentifierIds::corners, 5);
    setProperty (widgetData, CabbageIdentifierIds::colour, Colour (35, 35, 35).toString());
    setProperty (widgetData, CabbageIdentifierIds::fontcolour, CabbageUtilities::getComponentFontColour().toString());
    setProperty (widgetData, CabbageIdentifierIds::outlinecolour, CabbageUtilities::getComponentFontColour().toString());
    setProperty (widgetData, CabbageIdentifierIds::popup, 0);
    setProperty (widgetData, CabbageIdentifierIds::plant, "");
    setProperty (widgetData, CabbageIdentifierIds::child, 0);
    setProperty (widgetData, CabbageIdentifierIds::outlinethickness, 1);
    setProperty (widgetData, CabbageIdentifierIds::linethickness, 1);
    setProperty (widgetData, CabbageIdentifierIds::channeltype, "number");
    setProperty (widgetData, CabbageIdentifierIds::align, "centre");
    setProperty (widgetData, CabbageIdentifierIds::type, "groupbox");
    setProperty (widgetData, CabbageIdentifierIds::name, "groupbox");
    setProperty (widgetData, CabbageIdentifierIds::name, getProperty (widgetData, "name").toString() + String (ID));
    setProperty (widgetData, CabbageIdentifierIds::identchannel, "");
    setProperty (widgetData, CabbageIdentifierIds::visible, 1);
    setProperty (widgetData, CabbageIdentifierIds::imggroupbox, "");
}

void CabbageWidgetData::setGenTableProperties (ValueTree widgetData, int ID)
{
    setProperty (widgetData, "basetype", "layout");
    var tableColours;
    tableColours.append (Colours::lime.toString());

    setProperty (widgetData, CabbageIdentifierIds::top, 10);
    setProperty (widgetData, CabbageIdentifierIds::left, 10);
    setProperty (widgetData, CabbageIdentifierIds::width, 300);
    setProperty (widgetData, CabbageIdentifierIds::height, 200);

    var amprange;
    amprange.append (-1);
    amprange.append (1);
    amprange.append (-1);
    amprange.append (0.01);

    setProperty (widgetData, CabbageIdentifierIds::colour, CabbageUtilities::getComponentFontColour().toString());
    setProperty (widgetData, CabbageIdentifierIds::fontcolour, CabbageUtilities::getComponentFontColour().toString());
    setProperty (widgetData, CabbageIdentifierIds::type, "gentable");
    setProperty (widgetData, CabbageIdentifierIds::tablecolour, tableColours);
    setProperty (widgetData, CabbageIdentifierIds::name, "gentable");
    setProperty (widgetData, CabbageIdentifierIds::amprange, amprange);
    setProperty (widgetData, CabbageIdentifierIds::tablenumber, 0);
    setProperty (widgetData, CabbageIdentifierIds::amprange_max, 1);
    setProperty (widgetData, CabbageIdentifierIds::amprange_min, -1);
    setProperty (widgetData, CabbageIdentifierIds::amprange_tablenumber, -1);
    setProperty (widgetData, CabbageIdentifierIds::amprange_quantise, 0.01);
    setProperty (widgetData, CabbageIdentifierIds::amprange_quantise, 0.01);
    setProperty (widgetData, CabbageIdentifierIds::channeltype, "number");
    setProperty (widgetData, CabbageIdentifierIds::startpos, 0);
    setProperty (widgetData, CabbageIdentifierIds::endpos, -1);
    setProperty (widgetData, CabbageIdentifierIds::fill, 1);
    setProperty (widgetData, CabbageIdentifierIds::drawmode, "");
    setProperty (widgetData, CabbageIdentifierIds::file, "");
    setProperty (widgetData, CabbageIdentifierIds::name, getProperty (widgetData, "name").toString() + String (ID));
    setProperty (widgetData, CabbageIdentifierIds::identchannel, "");
    setProperty (widgetData, CabbageIdentifierIds::visible, 1);
    setProperty (widgetData, CabbageIdentifierIds::active, 0);

    var scrubInfo;
    scrubInfo.append (0);
    scrubInfo.append (-1);

    setProperty (widgetData, CabbageIdentifierIds::scrubberposition, scrubInfo);
    setProperty (widgetData, CabbageIdentifierIds::zoom, -1);
    setProperty (widgetData, CabbageIdentifierIds::startpos, 0);
    setProperty (widgetData, CabbageIdentifierIds::outlinethickness, 1.f);
    setProperty (widgetData, CabbageIdentifierIds::tablebackgroundcolour, Colour (15, 15, 15).toString());
    setProperty (widgetData, CabbageIdentifierIds::tablegridcolour, Colour (45, 45, 45).toString());
}
void CabbageWidgetData::setHSliderProperties (ValueTree widgetData, int ID)
{
    setProperty (widgetData, CabbageIdentifierIds::basetype, "interactive");
    setProperty (widgetData, CabbageIdentifierIds::top, 10);
    setProperty (widgetData, CabbageIdentifierIds::left, 10);
    setProperty (widgetData, CabbageIdentifierIds::width, 150);
    setProperty (widgetData, CabbageIdentifierIds::height, 50);
    var channels;
    channels.append ("hslider");
    setProperty (widgetData, CabbageIdentifierIds::channel, channels);
    setProperty (widgetData, CabbageIdentifierIds::min, 0);
    setProperty (widgetData, CabbageIdentifierIds::max, 1);
    setProperty (widgetData, CabbageIdentifierIds::value, 0);
    setProperty (widgetData, CabbageIdentifierIds::text, "");
    setProperty (widgetData, CabbageIdentifierIds::valuetextbox, 0.f);
    setProperty (widgetData, CabbageIdentifierIds::caption, "");
    setProperty (widgetData, CabbageIdentifierIds::channeltype, "number");
    setProperty (widgetData, CabbageIdentifierIds::colour, Colours::white.toString());
    setProperty (widgetData, CabbageIdentifierIds::gapmarkers, 0);
    setProperty (widgetData, CabbageIdentifierIds::trackercolour, Colour (0, 118, 38).toString());
    setProperty (widgetData, CabbageIdentifierIds::trackerbgcolour, Colour (0, 0, 0).toString());
    setProperty (widgetData, CabbageIdentifierIds::fontcolour, CabbageUtilities::getComponentFontColour().toString());
    setProperty (widgetData, CabbageIdentifierIds::textcolour, CabbageUtilities::getComponentFontColour().toString());
    setProperty (widgetData, CabbageIdentifierIds::sliderskew, 1);
    setProperty (widgetData, CabbageIdentifierIds::increment, .001);
    setProperty (widgetData, CabbageIdentifierIds::velocity, 0);

    setProperty (widgetData, CabbageIdentifierIds::midichan, -99);
    setProperty (widgetData, CabbageIdentifierIds::midictrl, -99);
    //these don't appear in the props dialog
    setProperty (widgetData, CabbageIdentifierIds::name, "hslider");
    setProperty (widgetData, CabbageIdentifierIds::type, getProperty (widgetData, "name").toString());
    setProperty (widgetData, CabbageIdentifierIds::name, getProperty (widgetData, "name").toString() + String (ID));
    setProperty (widgetData, CabbageIdentifierIds::kind, "horizontal");
    setProperty (widgetData, CabbageIdentifierIds::decimalplaces, 1);
    setProperty (widgetData, CabbageIdentifierIds::identchannel, "");
    setProperty (widgetData, CabbageIdentifierIds::trackerthickness, .1);
    setProperty (widgetData, CabbageIdentifierIds::visible, 1);
    setProperty (widgetData, CabbageIdentifierIds::gradient, 1);
    setProperty (widgetData, CabbageIdentifierIds::imgslider, "");
    setProperty (widgetData, CabbageIdentifierIds::imgsliderbg, "");
    setProperty (widgetData, CabbageIdentifierIds::style, "flat");
    setProperty (widgetData, CabbageIdentifierIds::automatable, 1.0f);
}

void CabbageWidgetData::setVSliderProperties (ValueTree widgetData, int ID)
{
    setProperty (widgetData, CabbageIdentifierIds::basetype, "interactive");
    setProperty (widgetData, CabbageIdentifierIds::top, 10);
    setProperty (widgetData, CabbageIdentifierIds::left, 10);
    setProperty (widgetData, CabbageIdentifierIds::width, 50);
    setProperty (widgetData, CabbageIdentifierIds::height, 150);
    var channels;
    channels.append ("vslider");
    setProperty (widgetData, CabbageIdentifierIds::channel, channels);
    setProperty (widgetData, CabbageIdentifierIds::min, 0);
    setProperty (widgetData, CabbageIdentifierIds::max, 1);
    setProperty (widgetData, CabbageIdentifierIds::value, 0);
    setProperty (widgetData, CabbageIdentifierIds::text, "");
    setProperty (widgetData, CabbageIdentifierIds::valuetextbox, 0.f);
    setProperty (widgetData, CabbageIdentifierIds::caption, "");
    setProperty (widgetData, CabbageIdentifierIds::channeltype, "number");
    setProperty (widgetData, CabbageIdentifierIds::colour, Colours::white.toString());
    setProperty (widgetData, CabbageIdentifierIds::gapmarkers, 0);
    setProperty (widgetData, CabbageIdentifierIds::trackercolour, Colour (0, 118, 38).toString());
    setProperty (widgetData, CabbageIdentifierIds::trackerbgcolour, Colour (0, 0, 0).toString());
    setProperty (widgetData, CabbageIdentifierIds::fontcolour, CabbageUtilities::getComponentFontColour().toString());
    setProperty (widgetData, CabbageIdentifierIds::textcolour, CabbageUtilities::getComponentFontColour().toString());
    setProperty (widgetData, CabbageIdentifierIds::sliderskew, 1);
    setProperty (widgetData, CabbageIdentifierIds::increment, .001);
    setProperty (widgetData, CabbageIdentifierIds::velocity, 0);
    setProperty (widgetData, CabbageIdentifierIds::midichan, -99);
    setProperty (widgetData, CabbageIdentifierIds::midictrl, -99);
    //these don't appear in the props dialog
    setProperty (widgetData, CabbageIdentifierIds::name, "vslider");
    setProperty (widgetData, CabbageIdentifierIds::type, getProperty (widgetData, "name").toString());
    setProperty (widgetData, CabbageIdentifierIds::name, getProperty (widgetData, "name").toString() + String (ID));
    setProperty (widgetData, CabbageIdentifierIds::kind, "vertical");

    setProperty (widgetData, CabbageIdentifierIds::decimalplaces, 1);
    setProperty (widgetData, CabbageIdentifierIds::trackerthickness, .1);
    setProperty (widgetData, CabbageIdentifierIds::identchannel, "");
    setProperty (widgetData, CabbageIdentifierIds::visible, 1);
    setProperty (widgetData, CabbageIdentifierIds::imgslider, "");
    setProperty (widgetData, CabbageIdentifierIds::imgsliderbg, "");
    setProperty (widgetData, CabbageIdentifierIds::style, "flat");
    setProperty (widgetData, CabbageIdentifierIds::automatable, 1.0f);
}

void CabbageWidgetData::setImageProperties (ValueTree widgetData, int ID)
{
    setProperty (widgetData, "basetype", "layout");
    setProperty (widgetData, CabbageIdentifierIds::top, 10);
    setProperty (widgetData, CabbageIdentifierIds::left, 10);
    setProperty (widgetData, CabbageIdentifierIds::width, 160);
    setProperty (widgetData, CabbageIdentifierIds::height, 120);
    setProperty (widgetData, CabbageIdentifierIds::cropx, 0);
    setProperty (widgetData, CabbageIdentifierIds::cropy, 0);
    setProperty (widgetData, CabbageIdentifierIds::cropwidth, 0);
    setProperty (widgetData, CabbageIdentifierIds::cropheight, 0);
    setProperty (widgetData, CabbageIdentifierIds::shape, "square");
    setProperty (widgetData, CabbageIdentifierIds::corners, 0);
    setProperty (widgetData, CabbageIdentifierIds::channeltype, "number");
    setProperty (widgetData, CabbageIdentifierIds::colour, Colours::white.toString());
    setProperty (widgetData, CabbageIdentifierIds::outlinecolour, Colours::white.toString());
    setProperty (widgetData, CabbageIdentifierIds::outlinethickness, 0);
    setProperty (widgetData, CabbageIdentifierIds::popup, 0);
    setProperty (widgetData, CabbageIdentifierIds::plant, "");
    setProperty (widgetData, CabbageIdentifierIds::type, "image");
    setProperty (widgetData, CabbageIdentifierIds::file, "");
    setProperty (widgetData, CabbageIdentifierIds::name, "image");
    setProperty (widgetData, CabbageIdentifierIds::name, getProperty (widgetData, "name").toString() + String (ID));
    setProperty (widgetData, CabbageIdentifierIds::identchannel, "");
    setProperty (widgetData, CabbageIdentifierIds::visible, 1);
}

void CabbageWidgetData::setComboBoxProperties (ValueTree widgetData, int ID)
{
    setProperty (widgetData, "basetype", "interactive");
    var array;
    array.append ("Item 1");
    array.append ("Item 2");
    array.append ("Item 3");
    array.append ("Item 4");
    setProperty (widgetData, CabbageIdentifierIds::top, 10);
    setProperty (widgetData, CabbageIdentifierIds::left, 10);
    setProperty (widgetData, CabbageIdentifierIds::width, 80);
    setProperty (widgetData, CabbageIdentifierIds::height, 20);
    var channels;
    channels.append ("combochan");
    setProperty (widgetData, CabbageIdentifierIds::channel, channels);
    setProperty (widgetData, CabbageIdentifierIds::channeltype, "number");
    setProperty (widgetData, CabbageIdentifierIds::text, array);
    setProperty (widgetData, CabbageIdentifierIds::align, "left");
    setProperty (widgetData, CabbageIdentifierIds::value, 1);
    setProperty (widgetData, CabbageIdentifierIds::update, 1);
    setProperty (widgetData, CabbageIdentifierIds::channeltype, "number");
    setProperty (widgetData, CabbageIdentifierIds::currenttext, "");
    setProperty (widgetData, CabbageIdentifierIds::caption, "");
    setProperty (widgetData, CabbageIdentifierIds::colour, Colours::black.toString());
    setProperty (widgetData, CabbageIdentifierIds::fontcolour, Colours::white.toString());
    setProperty (widgetData, CabbageIdentifierIds::menucolour, Colours::whitesmoke.toString());
    setProperty (widgetData, CabbageIdentifierIds::type, "combobox");
    setProperty (widgetData, CabbageIdentifierIds::comborange, 4);
    setProperty (widgetData, CabbageIdentifierIds::file, "");
    var populate;
    //populate.append (String::empty);
    //populate.append (String::empty);
    setProperty (widgetData, CabbageIdentifierIds::populate, populate);
    setProperty (widgetData, CabbageIdentifierIds::name, "combobox");
    setProperty (widgetData, CabbageIdentifierIds::name, getProperty (widgetData, "name").toString() + String (ID));
    setProperty (widgetData, CabbageIdentifierIds::identchannel, "");
    setProperty (widgetData, CabbageIdentifierIds::visible, 1);
    setProperty (widgetData, CabbageIdentifierIds::workingdir, "");
    setProperty (widgetData, CabbageIdentifierIds::refreshfiles, 0);
    setProperty (widgetData, CabbageIdentifierIds::automatable, 1.0f);
}

void CabbageWidgetData::setNumberSliderProperties (ValueTree widgetData, int ID)
{
    setProperty (widgetData, "basetype", "interactive");
    setProperty (widgetData, CabbageIdentifierIds::top, 10);
    setProperty (widgetData, CabbageIdentifierIds::left, 10);
    setProperty (widgetData, CabbageIdentifierIds::width, 100);
    setProperty (widgetData, CabbageIdentifierIds::height, 22);
    var channels;
    channels.append ("numberbox");
    setProperty (widgetData, CabbageIdentifierIds::channel, channels);
    setProperty (widgetData, CabbageIdentifierIds::min, 0);
    setProperty (widgetData, CabbageIdentifierIds::max, 100);
    setProperty (widgetData, CabbageIdentifierIds::value, 50);
    setProperty (widgetData, CabbageIdentifierIds::sliderskew, 1);
    setProperty (widgetData, CabbageIdentifierIds::increment, .01);
    setProperty (widgetData, CabbageIdentifierIds::text, "");
	setProperty(widgetData, CabbageIdentifierIds::fontsize, -1);
    setProperty (widgetData, CabbageIdentifierIds::value, 0);
    setProperty (widgetData, CabbageIdentifierIds::caption, "");
    setProperty (widgetData, CabbageIdentifierIds::type, "nslider");
    setProperty (widgetData, CabbageIdentifierIds::channeltype, "number");
    setProperty (widgetData, CabbageIdentifierIds::colour, CabbageUtilities::getDarkerBackgroundSkin().toString());
    setProperty (widgetData, CabbageIdentifierIds::fontcolour, Colours::whitesmoke.toString());
    setProperty (widgetData, CabbageIdentifierIds::textcolour, CabbageUtilities::getComponentFontColour().toString());
    setProperty (widgetData, CabbageIdentifierIds::name, "nslider");
    setProperty (widgetData, CabbageIdentifierIds::align, "above");
    setProperty (widgetData, CabbageIdentifierIds::name, getProperty (widgetData, "name").toString() + String (ID));
    setProperty (widgetData, CabbageIdentifierIds::identchannel, "");
    setProperty (widgetData, CabbageIdentifierIds::visible, 1);
    setProperty (widgetData, CabbageIdentifierIds::velocity, 80);
    setProperty (widgetData, CabbageIdentifierIds::automatable, 1.0f);
}
void CabbageWidgetData::setCheckBoxProperties (ValueTree widgetData, int ID)
{
    setProperty (widgetData, "basetype", "interactive");
    setProperty (widgetData, CabbageIdentifierIds::top, 10);
    setProperty (widgetData, CabbageIdentifierIds::left, 10);
    setProperty (widgetData, CabbageIdentifierIds::width, 100);
    setProperty (widgetData, CabbageIdentifierIds::height, 30);

    var channels;
    channels.append ("checkchan");
    setProperty (widgetData, CabbageIdentifierIds::channel, channels);
    setProperty (widgetData, CabbageIdentifierIds::text, "");
    setProperty (widgetData, CabbageIdentifierIds::value, 0);
    setProperty (widgetData, CabbageIdentifierIds::min, 0);
    setProperty (widgetData, CabbageIdentifierIds::max, 1);
    setProperty (widgetData, CabbageIdentifierIds::caption, "");
    setProperty (widgetData, CabbageIdentifierIds::shape, "square");
    setProperty (widgetData, CabbageIdentifierIds::channeltype, "number");
    setProperty (widgetData, CabbageIdentifierIds::corners, float (getProperty (widgetData, CabbageIdentifierIds::height))*.1f);
    setProperty (widgetData, CabbageIdentifierIds::type, "checkbox");
    setProperty (widgetData, CabbageIdentifierIds::oncolour, Colours::lime.toString());
    setProperty (widgetData, CabbageIdentifierIds::colour, Colours::black.toString());
    setProperty (widgetData, CabbageIdentifierIds::onfontcolour, CabbageUtilities::getComponentFontColour().toString());
    setProperty (widgetData, CabbageIdentifierIds::fontcolour, CabbageUtilities::getComponentFontColour().toString());
    setProperty (widgetData, CabbageIdentifierIds::name, "checkbox");
    setProperty (widgetData, CabbageIdentifierIds::name, getProperty (widgetData, "name").toString() + String (ID));
    setProperty (widgetData, CabbageIdentifierIds::identchannel, "");
    setProperty (widgetData, CabbageIdentifierIds::radiogroup, "");
    setProperty (widgetData, CabbageIdentifierIds::visible, 1);
    setProperty (widgetData, CabbageIdentifierIds::imgbuttonon, "");
    setProperty (widgetData, CabbageIdentifierIds::imgbuttonoff, "");
    setProperty (widgetData, CabbageIdentifierIds::automatable, 1.0f);
}

