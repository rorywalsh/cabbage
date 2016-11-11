/*
  ==============================================================================

    CabbageWidgetBase.h
    Created: 11 Nov 2016 6:58:12pm
    Author:  rory

  ==============================================================================
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
	CabbageWidgetBase(){}
	~CabbageWidgetBase(){}

	int pivotx, pivoty, visible, active, value;
    float rotate, alpha, currentValue;
	String tooltipText, text, channel;
	File imgButtonOn, imgButtonOff, imgPath;
	
	void initialiseCommonAttributes(ValueTree valueTree);	
	void handleCommonUpdates(Component* child, ValueTree data);
	
	//see below file for implementation
	extern template< typename Type >
    void setImgProperties(Type& comp, ValueTree widgetData, String type);
	
	String getString(ValueTree data, String identifier);
	void setChannel(ValueTree value);
	float getValue(ValueTree data);
	static int getImgHeight(File imgFile);
	static int getImgWidth(File imgFile);
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
				comp.getProperties().set(CabbageIdentifierIds::imggroupbox, CabbageUtilities::getImgHeight(imgFile.loadFileAsString()));
				comp.getProperties().set("imggroupboxheight", CabbageUtilities::getImgHeight(imgFile.loadFileAsString()));
				comp.getProperties().set("imggroupboxwidth", CabbageUtilities::getImgWidth(imgFile.loadFileAsString()));
			}
			else
				comp.getProperties().set(CabbageIdentifierIds::imggroupbox, imgFile.getFullPathName());
		}
		else if(imgPath.exists())
		{
			File filename(imgPath.getFullPathName()+"/groupbox."+fileType);
			CabbageUtilities::debug(filename.getFullPathName());
			if(filename.existsAsFile())
			{
				if(imgFile.getFileExtension().contains("svg"))
				{
					comp.getProperties().set(CabbageIdentifierIds::imggroupbox, imgFile.loadFileAsString());
					comp.getProperties().set("imggroupboxheight", CabbageUtilities::getImgHeight(filename.loadFileAsString()));
					comp.getProperties().set("imggroupboxwidth", CabbageUtilities::getImgWidth(filename.loadFileAsString()));
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
				comp.getProperties().set(CabbageIdentifierIds::imgbuttonon, imgFile.loadFileAsString());
				comp.getProperties().set("imgbuttonheight", CabbageUtilities::getImgHeight(imgFile.loadFileAsString()));
				comp.getProperties().set("imgbuttonwidth", CabbageUtilities::getImgWidth(imgFile.loadFileAsString()));
			}
			else
				comp.getProperties().set(CabbageIdentifierIds::imgbuttonon, imgFile.getFullPathName());
		}
		else if(imgPath.exists())
		{
			File filename(imgPath.getFullPathName()+"/buttonon."+fileType);
			CabbageUtilities::debug(filename.getFullPathName());
			if(filename.existsAsFile())
			{
				if(imgFile.getFileExtension().contains("svg"))
				{
					comp.getProperties().set(CabbageIdentifierIds::imgbuttonon, imgFile.loadFileAsString());
					comp.getProperties().set("imgbuttonheight", CabbageUtilities::getImgHeight(filename.loadFileAsString()));
					comp.getProperties().set("imgbuttonwidth", CabbageUtilities::getImgWidth(filename.loadFileAsString()));
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
				comp.getProperties().set(CabbageIdentifierIds::imgbuttonoff, imgFile.loadFileAsString());
				comp.getProperties().set("imgbuttonheight", CabbageUtilities::getImgHeight(imgFile.loadFileAsString()));
				comp.getProperties().set("imgbuttonwidth", CabbageUtilities::getImgWidth(imgFile.loadFileAsString()));
			}
			else
				comp.getProperties().set(CabbageIdentifierIds::imgbuttonoff, imgFile.getFullPathName());
		}
		else if(imgPath.exists())
		{
			File filename(imgPath.getFullPathName()+"/buttonoff."+fileType);
			CabbageUtilities::debug(filename.getFullPathName());
			if(filename.existsAsFile())
			{
				
				if(imgFile.getFileExtension().contains("svg"))
				{
				comp.getProperties().set(CabbageIdentifierIds::imgbuttonoff, filename.loadFileAsString());
				comp.getProperties().set("imgbuttonheight", CabbageUtilities::getImgHeight(filename.loadFileAsString()));
				comp.getProperties().set("imgbuttonwidth", CabbageUtilities::getImgWidth(filename.loadFileAsString()));
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
				comp.getProperties().set("imgsliderbg", imgFile.loadFileAsString());
				comp.getProperties().set("imgsliderbgheight", CabbageUtilities::getImgHeight(imgFile.loadFileAsString()));
				comp.getProperties().set("imgsliderbgwidth", CabbageUtilities::getImgWidth(imgFile.loadFileAsString()));
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
					comp.getProperties().set("imgsliderbg", filename.loadFileAsString());
					comp.getProperties().set("imgsliderbgheight", CabbageUtilities::getImgHeight(filename.loadFileAsString()));
					comp.getProperties().set("imgsliderbgwidth", CabbageUtilities::getImgWidth(filename.loadFileAsString()));
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
				comp.getProperties().set("imgslider", imgFile.loadFileAsString());
				comp.getProperties().set("imgsliderheight", CabbageUtilities::getImgHeight(imgFile.loadFileAsString()));
				comp.getProperties().set("imgsliderwidth", CabbageUtilities::getImgWidth(imgFile.loadFileAsString()));
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
					comp.getProperties().set("imgslider", filename.loadFileAsString());
					comp.getProperties().set("imgsliderheight", CabbageUtilities::getImgHeight(filename.loadFileAsString()));
					comp.getProperties().set("imgsliderwidth", CabbageUtilities::getImgWidth(filename.loadFileAsString()));
				}
				else
					comp.getProperties().set("imgslider", filename.getFullPathName());
			}
		}
	}
}



#endif  // CABBAGEWIDGETBASE_H_INCLUDED
