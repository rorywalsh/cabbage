/*
  Copyright (C) 2016 Rory Walsh

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
  
#ifndef CABBAGEWIDGETBASE_H_INCLUDED
#define CABBAGEWIDGETBASE_H_INCLUDED

#include "../CabbageCommonHeaders.h"

// Simple base class for taking care of some widget housekeeping. This class looks after
// common memeber variables such as alpha values, tooltiptext, bounds, etc
// Each cabbage widget should inherit from this class so initialiseCommonAttributes()
// can be called in its valueTreePropertyChanged() method.
class CabbageWidgetBase
{
public:
    CabbageWidgetBase() {}
    ~CabbageWidgetBase() {}

    int pivotx, pivoty, visible, active, value;
    float rotate, alpha, currentValue;
    String tooltipText, text, channel;
    File imgButtonOn, imgButtonOff, imgPath;

    void initialiseCommonAttributes(Component* child, ValueTree valueTree);		//handles simple attributes
    void handleCommonUpdates(Component* child, ValueTree data);

    //see below file for implementation
    template< typename Type >
    void setImgProperties(Type& comp, ValueTree widgetData, String type);

    String getCurrentString(ValueTree data, String identifier);
    void setChannel(ValueTree value);
    float getCurrentValue(ValueTree data);
    static int getSVGHeight(File svgFile);
    static int getSVGWidth(File svgFile);
};
//================== setImgProperties ==========================
template< typename Type >
void CabbageWidgetBase::setImgProperties(Type& comp, ValueTree data, String imgType)
{

    File imgFile;
    const File imgPath(CabbageWidgetData::getStringProp(data, CabbageIdentifierIds::imgpath));
    const String fileType(CabbageWidgetData::getStringProp(data, CabbageIdentifierIds::filetype));

    if(imgType=="groupbox")
    {
        imgFile = File(CabbageWidgetData::getStringProp(data, CabbageIdentifierIds::imggroupbox));
        if(imgFile.existsAsFile())
        {
            if(imgFile.getFileExtension().contains("svg"))
            {
                comp.getProperties().set(CabbageIdentifierIds::imggroupbox, imgFile.getFullPathName());
                comp.getProperties().set("imggroupboxheight", getSVGHeight(imgFile.loadFileAsString()));
                comp.getProperties().set("imggroupboxwidth", getSVGWidth(imgFile.loadFileAsString()));
            }
            else
                comp.getProperties().set(CabbageIdentifierIds::imggroupbox, imgFile.getFullPathName());
        }
        else if(imgPath.exists())
        {
            File filename(imgPath.getFullPathName()+"/groupbox."+fileType);

            if(filename.existsAsFile())
            {
                if(imgFile.getFileExtension().contains("svg"))
                {
                    comp.getProperties().set(CabbageIdentifierIds::imggroupbox, imgFile.getFullPathName());
                    comp.getProperties().set("imggroupboxheight", getSVGHeight(filename.loadFileAsString()));
                    comp.getProperties().set("imggroupboxwidth", getSVGWidth(filename.loadFileAsString()));
                }
                else
                    comp.getProperties().set(CabbageIdentifierIds::imggroupbox, imgFile.getFullPathName());
            }
        }
    }
    else if(imgType=="buttonon")
    {
        imgFile = File(CabbageWidgetData::getStringProp(data, CabbageIdentifierIds::imgbuttonon));
        if(imgFile.existsAsFile())
        {

            if(imgFile.getFileExtension().contains("svg"))
            {
                comp.getProperties().set(CabbageIdentifierIds::imgbuttonon, imgFile.getFullPathName());
                comp.getProperties().set("imgbuttonheight", getSVGHeight(imgFile.loadFileAsString()));
                comp.getProperties().set("imgbuttonwidth", getSVGWidth(imgFile.loadFileAsString()));
            }
            else
                comp.getProperties().set(CabbageIdentifierIds::imgbuttonon, imgFile.getFullPathName());
        }
        else if(imgPath.exists())
        {
            File filename(imgPath.getFullPathName()+"/buttonon."+fileType);

            if(filename.existsAsFile())
            {
                if(imgFile.getFileExtension().contains("svg"))
                {
                    comp.getProperties().set(CabbageIdentifierIds::imgbuttonon, imgFile.getFullPathName());
                    comp.getProperties().set("imgbuttonheight", getSVGHeight(filename.loadFileAsString()));
                    comp.getProperties().set("imgbuttonwidth", getSVGWidth(filename.loadFileAsString()));
                }
                else
                    comp.getProperties().set(CabbageIdentifierIds::imgbuttonon, imgFile.getFullPathName());
            }
        }
    }
    else if(imgType=="buttonoff")
    {
        imgFile = File(CabbageWidgetData::getStringProp(data, CabbageIdentifierIds::imgbuttonoff));
        if(imgFile.existsAsFile())
        {
            if(imgFile.getFileExtension().contains("svg"))
            {
                comp.getProperties().set(CabbageIdentifierIds::imgbuttonoff, imgFile.getFullPathName());
                comp.getProperties().set("imgbuttoffheight", getSVGHeight(imgFile.loadFileAsString()));
                comp.getProperties().set("imgbuttoffwidth", getSVGWidth(imgFile.loadFileAsString()));
            }
            else
                comp.getProperties().set(CabbageIdentifierIds::imgbuttonoff, imgFile.getFullPathName());
        }
        else if(imgPath.exists())
        {
            File filename(imgPath.getFullPathName()+"/buttonoff."+fileType);

            if(filename.existsAsFile())
            {

                if(imgFile.getFileExtension().contains("svg"))
                {
                    comp.getProperties().set(CabbageIdentifierIds::imgbuttonoff, filename.getFullPathName());
                    comp.getProperties().set("imgbuttoffheight", getSVGHeight(filename.loadFileAsString()));
                    comp.getProperties().set("imgbuttoffwidth", getSVGWidth(filename.loadFileAsString()));
                }
                else
                    comp.getProperties().set(CabbageIdentifierIds::imgbuttonoff, filename.getFullPathName());
            }
        }
    }
    else if(imgType.contains("sliderbg"))
    {
        imgFile = File(CabbageWidgetData::getStringProp(data, CabbageIdentifierIds::imgsliderbg));
        if(imgFile.existsAsFile())
        {

            if(imgFile.getFileExtension().contains("svg"))
            {
                comp.getProperties().set("imgsliderbg", imgFile.getFullPathName());
                comp.getProperties().set("imgsliderbgheight", getSVGHeight(imgFile.loadFileAsString()));
                comp.getProperties().set("imgsliderbgwidth", getSVGWidth(imgFile.loadFileAsString()));
            }
            else
                comp.getProperties().set("imgsliderbg", imgFile.getFullPathName());

        }
        else if(imgPath.exists())
        {
            File filename;
            if(imgType=="rsliderbg")
                filename = File(imgPath.getFullPathName()+"/rslider_background."+fileType);
            else if(imgType=="hsliderbg")
                filename = File(imgPath.getFullPathName()+"/hslider_background."+fileType);
            else
                filename = File(imgPath.getFullPathName()+"/vslider_background."+fileType);

            if(filename.existsAsFile())
            {

                if(imgFile.getFileExtension().contains("svg"))
                {
                    comp.getProperties().set("imgsliderbg", filename.getFullPathName());
                    comp.getProperties().set("imgsliderbgheight", getSVGHeight(filename.loadFileAsString()));
                    comp.getProperties().set("imgsliderbgwidth", getSVGWidth(filename.loadFileAsString()));
                }
                else
                    comp.getProperties().set("imgsliderbg", imgFile.getFullPathName());
            }
        }
    }
    else if(imgType.contains("slider"))
    {
        imgFile = File(CabbageWidgetData::getStringProp(data, CabbageIdentifierIds::imgslider));
        if(imgFile.existsAsFile())
        {

            if(imgFile.getFileExtension().contains("svg"))
            {
                comp.getProperties().set("imgslider", imgFile.getFullPathName());
                comp.getProperties().set("imgsliderheight", getSVGHeight(imgFile.loadFileAsString()));
                comp.getProperties().set("imgsliderwidth", getSVGWidth(imgFile.loadFileAsString()));
            }
            else
                comp.getProperties().set("imgslider", imgFile.getFullPathName());
        }
        else if(imgPath.exists())
        {
            File filename;
            if(imgType=="rslider")
                filename = File(imgPath.getFullPathName()+"/rslider."+fileType);
            else if(imgType=="hslider")
                filename = File(imgPath.getFullPathName()+"/hslider."+fileType);
            else
                filename = File(imgPath.getFullPathName()+"/vslider."+fileType);

            if(filename.existsAsFile())
            {

                if(imgFile.getFileExtension().contains("svg"))
                {
                    comp.getProperties().set("imgslider", filename.getFullPathName());
                    comp.getProperties().set("imgsliderheight", getSVGHeight(filename.loadFileAsString()));
                    comp.getProperties().set("imgsliderwidth", getSVGWidth(filename.loadFileAsString()));
                }
                else
                    comp.getProperties().set("imgslider", filename.getFullPathName());
            }
        }
    }
}



#endif  // CABBAGEWIDGETBASE_H_INCLUDED
