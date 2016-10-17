/*
  ==============================================================================

    CabbageUtilities.h
    Created: 14 Oct 2016 11:40:29am
    Author:  rory

  ==============================================================================
*/

#ifndef CABBAGEUTILITIES_H_INCLUDED
#define CABBAGEUTILITIES_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "../BinaryData/CabbageBinaryData.h"



class CabbageUtilities
{
public:
    CabbageUtilities() {};
    ~CabbageUtilities() {};

    static void debug(String message)
    {
#ifdef DEBUG
        Logger::writeToLog(message);
#endif
    }

    static void debug(float value)
    {
#ifdef DEBUG
        Logger::writeToLog(String(value));
#endif
    }

    static void debug(String message, double value)
    {
#ifdef DEBUG
        Logger::writeToLog(message+":"+String(value));
#endif
    }

    static void debug(float val, String value)
    {
#ifdef DEBUG
        Logger::writeToLog(String(val)+":"+value);
#endif
    }

    static void debug(float val, float value)
    {
#ifdef DEBUG
        Logger::writeToLog(String(val)+":"+String(value));
#endif
    }

    static void debug(String message, String value)
    {
#ifdef DEBUG
        Logger::writeToLog(message+":"+value);
#endif
    }

    static void showMessage(String message)
    {
        AlertWindow::showMessageBoxAsync (AlertWindow::WarningIcon,
                                          "Cabbage Message",
                                          message,
                                          "Ok");

    }

    static void showMessage(String title, String message)
    {
        AlertWindow::showMessageBoxAsync (AlertWindow::WarningIcon,
                                          title,
                                          message,
                                          "Ok");

    }

    static Image drawSVGImageFromFilePath(String path, String type, AffineTransform affine)
    {
        String svgFileName = File(path).existsAsFile()? path : path+"/"+String(type)+".svg";
        Image svgImg;
        File svgFile(svgFileName);
        ScopedPointer<XmlElement> svg (XmlDocument::parse(svgFile.loadFileAsString()));
		svgImg = Image(Image::ARGB, 80, 80, true);
		ScopedPointer<Drawable> drawable;
		
        if (svgFile.exists())
        {
			Graphics graph(svgImg);
            if (svg != nullptr)
            {
                drawable = Drawable::createFromSVG (*svg);
                drawable->draw(graph, 1.f, affine);
                return svgImg;
            }
		}
		else return Image::null;
	}
};


#endif  // CABBAGEUTILITIES_H_INCLUDED
