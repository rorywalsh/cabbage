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
};


#endif  // CABBAGEUTILITIES_H_INCLUDED
