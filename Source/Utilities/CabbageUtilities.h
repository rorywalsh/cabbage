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
#include "../Application/CabbageIds.h"



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

    static void showMessage(String title, String message, LookAndFeel* feel)
    {
        AlertWindow alert(title, message, AlertWindow::WarningIcon);
        alert.setLookAndFeel(feel);
        alert.addButton("Ok", 1);
#if !defined(AndroidBuild)
        alert.runModalLoop();
#else
        alert.showMessageBoxAsync(AlertWindow::WarningIcon, "Cabbage Message" , message, "Ok");
#endif
    }
	
	
    static int showYesNoMessage(String message, LookAndFeel* feel, int cancel=0)
    {
        AlertWindow alert("Cabbage Message", message, AlertWindow::QuestionIcon, 0);
        alert.setLookAndFeel(feel);
        alert.addButton("Yes", 0);
        alert.addButton("No", 1);
        if(cancel==1)
            alert.addButton("Cancel", 2);
#if !defined(AndroidBuild)
        int result = alert.runModalLoop();
#else
        int result = alert.showYesNoCancelBox(AlertWindow::QuestionIcon, "Warning", message, "Yes", "No", "Cancel", nullptr, nullptr);
#endif
        return result;
    }	
	
	
	static void setImagesForButton(ImageButton* button, const Image image)
	{
	button->setImages(true, true, true, image, 1, Colours::transparentBlack, image,
					 1, Colours::transparentBlack, image, .8, Colours::transparentBlack, 0);
	}
	
	
	Identifier getInterfaceTypeFromFile(File inputFile)
	{
		StringArray stringArray;
		stringArray.addLines(inputFile.loadFileAsString());
		//if(stringArray.contains("<Cabbage>") && stringArray.contains("</Cabbage>"))
			return CabbageInterfaceModes::cabbage;
		//else
		//	stringArray.contains("chn_k")
	}
	
	
    static Font getComponentFont(int style=1)
    {
        Font font = Font(11.5, style);// ("Verdana", 11.5, bold);
        return font;
    }

    static Colour getComponentFontColour()
    {
        Colour cl = Colour::fromRGBA (160, 160, 160, 255);
        return cl;
    }

    static Colour getComponentSkin()
    {
        //Colour skin = Colour::fromRGBA (45, 55, 60, 255);
        Colour skin = Colour::fromRGBA (75, 85, 90, 100); //some transparency
        return skin;
    }
	
    static	String getBoundsString(juce::Rectangle<int> currentBounds)
    {
        String bounds = "bounds(" + String(currentBounds.getX()) + String(", ") + String(currentBounds.getY()) + String(", ") + String(currentBounds.getWidth()) + String(", ")
                        + String(currentBounds.getHeight()) + String(")");
        debug(bounds);
        return bounds;

    }
	

    static bool compDouble(double x, int y)
    {
        if( x > (double)y-0.0001 && x < (double)y+0.0001)
        {
            // They are almost equal
            return true;
        }
        else
        {
            // They aren't equal at all
            return false;
        }
    }

    static double roundToMultiple(double x, double multiple)
    {
        return round(x / multiple) * multiple;
    }

    static int roundIntToMultiple(int x, int multiple)
    {
        return round(x / multiple) * multiple;
    }

    static double roundToPrec(double x, int prec)
    {
        double power = 1.0;
        int i;

        if (prec > 0)
            for (i = 0; i < prec; i++)
                power *= 10.0;
        else if (prec < 0)
            for (i = 0; i < prec; i++)
                power /= 10.0;

        if (x > 0)
            x = floor(x * power + 0.5) / power;
        else if (x < 0)
            x = ceil(x * power - 0.5) / power;

        if (x == -0)
            x = 0;

        return x;
    }



    static int getNumberOfDecimalPlaces(StringArray array)
    {
        int longest=0;
        int index = 0;
        for(int i=0; i<array.size(); i++)
        {
            if(array[i].length()>longest)
            {
                longest = array[i].length();
                index = i;
            }
        }

        if(array[index].indexOf(".")>=0)
        {
            String subTemp = array[index].substring(array[index].indexOf("."), 10);
            return subTemp.length()-1;
        }
        else return 0;


    }
	
	
    static Colour getBackgroundSkin()
    {
        Colour skin = Colour::fromRGB(5, 15, 20);
        return skin;
    }
	
	
    static Colour getDarkerBackgroundSkin()
    {
        Colour skin = getBackgroundSkin().darker(0.4);
        return skin;
    }


    static String setDecimalPlaces(const double& x, const int& numDecimals)
    {
        int y=x;
        double z=x-y;
        double m=pow((double)10.f, (double)numDecimals);
        double q=z*m;
        double r=roundToIntAccurate(q);

        return String(static_cast<double>(y)+(1.0/m)*r);
    }	
	
};


#endif  // CABBAGEUTILITIES_H_INCLUDED
