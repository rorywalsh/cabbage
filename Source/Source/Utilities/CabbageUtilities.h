/*
  Copyright (C) 2009 Rory Walsh

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

#ifndef __CabbUtilities_h__
#define __CabbUtilities_h__



#include "JuceHeader.h"
#include "../BinaryData/CabbageBinaryData.h"

#include <fstream>

class CabbageIDELookAndFeel;

#ifdef JUCE_MAC
 #include <pwd.h>
 #include <unistd.h>
#endif

#ifdef _MSC_VER
#pragma warning(disable: 4244) // possible loss of data
#pragma warning(disable: 4100) // possible loss of data
#endif


#define QUADBEZIER 999

#define svgRSliderDiameter 100

#define svgVSliderWidth 30
#define svgVSliderHeight 300

#define svgVSliderThumb 100
#define svgHSliderThumb 100


#define svgHSliderWidth 300
#define svgHSliderHeight 30

#define svgButtonWidth 100
#define svgButtonHeight 50

#define svgGroupboxWidth 400
#define svgGroupboxHeight 300


//simple abstract class used to hold information about about displays
class SignalDisplay
{
public:
    String caption;
    int windid;
    float yScale;
    int min , max, size;

    SignalDisplay (String _caption, int _id, float _scale, int _min, int _max, int _size):
        caption (_caption),
        windid (_id),
        yScale (_scale),
        min (_min),
        max (_max),
        size (_size)
    {}

    ~SignalDisplay()
    {
        points.clear();
    }

    Array<float, CriticalSection> getPoints()
    {
        return points;
    }

    void setPoints (Array <float, CriticalSection > tablePoints)
    {
        points.swapWith (tablePoints);
    }

private:
    Array <float, CriticalSection > points;
};

//================================================================================
class CabbageExamplesFolder
{
public:

    CabbageExamplesFolder() {}

    static StringArray getEffects()
    {
        StringArray directories;
        directories.add ("Distortion");
        directories.add ("Dynamics");
        directories.add ("Filters");
        directories.add ("Miscellaneous");
        directories.add ("Modulation");
        directories.add ("Reverbs");
        directories.add ("Spectral");
        directories.add ("Time");
        return directories;
    }

    static StringArray getInstruments()
    {
        StringArray directories;
        directories.add ("DrumMachines");
        directories.add ("Miscellaneous");
        directories.add ("Noise");
        directories.add ("PhysicalModelling");
        directories.add ("Synths");
        return directories;
    }

};
//================================================================================
class CabbageImages
{
public:
    CabbageImages() {}

    static const Image drawBypassIcon (int width, int height, bool isActive)
    {
        Image img = Image (Image::ARGB, width, height, true);
        Graphics g (img);

        const float x = 0;
        const float y = 0;
        const float w = width - 5.f;
        const float h = height;
        const float d = 5;
        g.setColour (isActive ? Colours::cornflowerblue.darker (.8f) : Colours::lime);
        Path p;
        p.startNewSubPath (x + 5, y + h / 2.f + d / 2.f);
        g.drawEllipse (x, y + h / 2.f, d, d, 2);
        g.drawEllipse (x + w, y + h / 2.f, d, d, 2.f);

        if (!isActive)
        {
            p.lineTo (x + w, y + h / 2.f + d / 2.f);
        }
        else
        {
            p.addArc (x + w, y + h / 2.f + d / 2.f, 5, 5, 3.14f, 3.14f);
        }

        p.closeSubPath();
        g.strokePath (p, PathStrokeType (2));

        return img;
    }

    static const Image drawEditGUIIcon (int width, int height)
    {
        Image img = Image (Image::ARGB, width, height, true);
        Graphics g (img);

        Path p, dashedP;
        p.addRoundedRectangle (0, 0, width, height, 2);
        g.setColour (Colour (0x1a1a1a00)/*Colour (30, 30, 30)*/);
        p.closeSubPath();
        g.fillPath (p);
        p.clear();

        g.setColour (Colours::lightgrey);//Colours::cornflowerblue);
        p.addEllipse (4, 4, width - 8, height - 8);
        const float dashLengths[] = { 3.0f, 3.0f };
        PathStrokeType (6.0, PathStrokeType::mitered).createDashedStroke (dashedP, p, dashLengths, 2);
        g.fillPath (dashedP);
        p.clear();

        g.setColour (Colours::grey.brighter(0.3f));//Colours::cornflowerblue.darker());
        //p.addEllipse (3, 3, width - 6, height - 6);
        //g.fillPath (p);
        g.drawEllipse (5, 5, width - 10, height - 10, 5);
        p.clear();

		/*g.setColour(Colour(82, 99, 106));//Colour (30, 30, 30));
        p.addEllipse (6, 6, width - 12, height - 12);
        g.fillPath (p);*/
        return img;
    }

    static const Image drawPlayPauseIcon (int width, int height, Colour colour, bool isPlaying, bool isPressed = false)
    {
        Image img = Image (Image::ARGB, width, height, true);
        Graphics g (img);
        Path p;

        const int newWidth = (isPressed == true ? width - 1 : width);
        const int newHeight = (isPressed == true ? height - 1 : height);

        p.addRoundedRectangle (0, 0, newWidth, newHeight, 2);
        g.setColour(colour);//  /*Colour (30, 30, 30)*/);
        p.closeSubPath();
        g.fillPath (p);
        p.clear();

        int scaleFactor = -3; // <-- in pixel

        if (isPlaying == false)
        {
            g.setColour (Colours::white/*Colours::lime.darker()*/);
            p.addTriangle (newWidth - scaleFactor, 2 - scaleFactor, newWidth - 5 + scaleFactor, newHeight / 2, newWidth - scaleFactor, newHeight - 3 + scaleFactor);
        }
        else
        {
            g.setColour (Colours::white/*Colours::lime.darker (.7f)*/);
            p.addRectangle (newWidth, 4 - scaleFactor, newWidth, newHeight - 11 + scaleFactor);
        }

        p.closeSubPath();
        g.fillPath (p);
        return img;
    }

    static const Image drawPlayStopIcon (int width, int height, Colour colour, bool isPlaying, bool isPressed = false)
    {
        Image img = Image (Image::ARGB, width, height, true);
        Graphics g (img);
        Path p;

        const int newWidth = (isPressed == true ? width - 1 : width);
        const int newHeight = (isPressed == true ? height - 1 : height);
		
        p.addRoundedRectangle (0, 0, newWidth, newHeight, 2);
		g.setColour(colour);//  /*Colour (30, 30, 30)*/);
        p.closeSubPath();
        g.fillPath (p);
        p.clear();

        int scaleFactor = -3; // <-- in pixel

        if (isPlaying == false)
        {
            g.setColour (Colours::white/*Colours::lime.darker()*/);
            p.addTriangle (newWidth * .62f - scaleFactor, 2 - scaleFactor, newWidth - 5 + scaleFactor, newHeight / 2, newWidth * .62f - scaleFactor, newHeight - 3 + scaleFactor);

        }
        else
        {
            g.setColour (Colours::white/*Colours::lime.darker (.7f)*/);
            p.addRectangle (newWidth * .62f - scaleFactor, 4 - scaleFactor, newWidth * .3f + scaleFactor * 2.f, newHeight - 11 + scaleFactor);
        }

        p.closeSubPath();
        g.fillPath (p);
        g.setColour (Colours::whitesmoke.darker());
        g.setColour (Colours::whitesmoke);
        g.drawFittedText (isPlaying == true ? "Stop" : "Play", 0, 0, int(width * .6), height - 1, Justification::centred, 1);
        return img;
    }

    static const Image drawCloseIcon (int width, int height)
    {
        Image img = Image (Image::ARGB, width, height, true);
        Graphics g (img);

        Path p;
        p.addRoundedRectangle (0, 0, width - 5, height - 5, 2);
        g.setColour (Colour (0x1a1a1a00)/*Colour (30, 30, 30)*/);
        p.closeSubPath();
        g.fillPath (p);
        p.clear();
        g.setColour (Colours::lightgrey/*Colours::red*/);
        p.startNewSubPath (5, 5);
        p.lineTo (width - 10, height - 10);
        p.closeSubPath();
        g.strokePath (p, PathStrokeType (3));

        p.startNewSubPath (5, height - 10);
        p.lineTo (width - 10, 5);
        p.closeSubPath();
        g.strokePath (p, PathStrokeType (3));
        return img;
    }

    static const Image drawEditorIcon (int width, int height)
    {
        Image img = Image (Image::ARGB, width, height, true);
        Graphics g (img);
        Path p;


        p.addRoundedRectangle (0, 0, width, height, 2);
        g.setColour (Colour (0x1a1a1a00)/*Colour (30, 30, 30)*/);
        p.closeSubPath();
        g.fillPath (p);
        p.clear();

        g.setColour (Colours::black/*Colours::cornflowerblue.darker()*/);

        for ( int i = 0 ; i < 3; i++)
        {
            p.startNewSubPath (6 + (i * 6), 3);
            p.lineTo (6 + (i * 6), height - 3);
            g.strokePath (p, PathStrokeType (2));
            p.closeSubPath();
            p.clear();
        }

        Random rand;
        g.setColour (Colours::whitesmoke/*Colours::lime.darker()*/);

        for ( int i = 0 ; i < 3; i++)
        {
            const int heightOfTHumb = rand.nextInt (Range<int> (5, height - 5));
            p.startNewSubPath (4 + (i * 6), heightOfTHumb);
            p.lineTo (9 + (i * 6), heightOfTHumb);
            g.strokePath (p, PathStrokeType (2));
            p.closeSubPath();
            p.clear();
        }

        return img;
    }

    static const Image drawMuteIcon (int width, int height, bool muted)
    {
        Image img = Image (Image::ARGB, width, height, true);
        Graphics g (img);

        const float x = 0;
        const float y = 0;
        const float w = width - 5.f;
        const float h = height;
        g.setColour (Colours::whitesmoke);
        Path p;
        p.startNewSubPath (x, y + h * .33f);
        p.lineTo (x + w / 2.f, y + h * .33f);
        p.lineTo (x + w, y + 0.f);
        p.lineTo (x + w, y + h);
        p.lineTo (x + w / 2.f, y + h * .66f);
        p.lineTo (x, y + h * .66f);
        p.lineTo (x, y + h * .33f);
        p.closeSubPath();
        g.strokePath (p, PathStrokeType (1));

        if (muted)
        {
            g.setColour (Colours::lime);
            g.drawLine (x + 0, y + 0, x + w, y + h, 3);
            g.drawLine (x + 0, y + h, x + w, y + 0, 3);
        }
        else
            g.fillPath (p);

        return img;
    }
};

//===========================================================================================
//some utility functions used across classes...
//===========================================================================================
class CabbageUtilities
{
public:
    CabbageUtilities() {}
    ~CabbageUtilities() {}

    enum TargetTypes
    {
        IDE = 0,
        PluginSynth,
        PluginEffect
    };

    enum TargetPlatformTypes
    {
        Win32 = 0,
        Linux,
        OSX
    };

    //==============================================================
    static int getTarget()
    {
#ifdef Cabbage_IDE_Build
        return TargetTypes::IDE;
#elif Cabbage_Plugin_Synth
        return TargetTypes::PluginSynth;
#else
        return TargetTypes::PluginEffect;
#endif
    }

    static int getTargetPlatform()
    {
#ifdef WIN32
        return TargetPlatformTypes::Win32;
#elif LINUX
        return TargetPlatformTypes::Linux;
#else
        return TargetPlatformTypes::OSX;
#endif
    }

    static bool shouldUseNativeBrowser()
    {
#ifdef Use_Native_File_Browser
        return true;
#else
        return false;
#endif
    }
	//==============================================================
	static int getHeaderInfo(String csdText, String headerString)
	{
        
        const int start = csdText.indexOf("/*");
        const int end = csdText.indexOf("*/");
        
        //special case where users has misplaced a closing comment */
        if(end < start)
            csdText = csdText.replaceFirstOccurrenceOf("*/", "");
        
		while (csdText.indexOf("/*") != -1 && csdText.indexOf("*/") != -1)
		{
			const String comments = csdText.substring(csdText.indexOf("/*"), csdText.indexOf("*/") + 2);
			csdText = csdText.replace(comments, "");
		}

		StringArray array;
		array.addLines(csdText);

		bool inCsoundInstrumentsSection = false;

		for (int i = 0; i < array.size(); i++)
		{
			if (array[i] == "<CsInstruments>")
				inCsoundInstrumentsSection = true;

			if (inCsoundInstrumentsSection)
			{
               
				if (array[i].indexOf(";") != -1)
					array.set(i, array[i].substring(0, array[i].indexOf(";")));

				array.set(i, array[i].removeCharacters("\t").trimStart());

				if (array[i].contains(headerString) && array[i].contains("=") && array[i].indexOf(headerString) < array[i].indexOf("="))
				{
					String channels = array[i].substring(array[i].indexOf("=") + 1, (array[i].contains(";") ? array[i].indexOf(";") : 100));
					return channels.trim().getIntValue();
				}
			}
		}


		if(headerString=="nchnls")
            return 2;
        else if(headerString == "nchnls_i")
            return -1;
        else
            return -1;
	}
    //==============================================================
    static const String getSVGTextFromMemory (const void* svg, size_t size)
    {
        MemoryInputStream svgStream (svg, size, false);
        return svgStream.readString();
    }
    //==============================================================
    static void debug (String message)
    {
#ifdef DEBUG
        Logger::writeToLog (message);
#endif
    }

    static void debug (float value)
    {
#ifdef DEBUG
        Logger::writeToLog (String (value));
#endif
    }

    static void debug (String message, double value)
    {
#ifdef DEBUG
        Logger::writeToLog (message + ":" + String (value));
#endif
    }

    static void debug (float val, String value)
    {
#ifdef DEBUG
        Logger::writeToLog (String (val) + ":" + value);
#endif
    }

    static void debug (float val, float value)
    {
#ifdef DEBUG
        Logger::writeToLog (String (val) + ":" + String (value));
#endif
    }

    static void debug (String message, String value)
    {
#ifdef DEBUG
        Logger::writeToLog (message + ":" + value);
#endif
    }
    //===========================================================================================
    static void addExamples (PopupMenu& m, const String menuName, String dir, Array<File>& filesArray, StringArray folders, int indexOffset)
    {
        PopupMenu subMenu1, subMenu2;
        int noOfFiles = filesArray.size();

        if (folders.size() > 0)
        {
            for ( int i = 0 ; i < folders.size() ; i++ )
            {
                subMenu2.clear();
                File searchDir (dir + "/" + menuName + "/" + folders[i]);
                Array<File> exampleFilesArray;
                searchDir.findChildFiles (exampleFilesArray, File::findFiles, false, "*.csd");
                exampleFilesArray.sort();
                filesArray.addArray (exampleFilesArray);

                for (int fileCnt = noOfFiles; fileCnt < filesArray.size(); fileCnt++)
                {
                    subMenu2.addItem (fileCnt + indexOffset, filesArray[fileCnt].getFileNameWithoutExtension());
                }

                subMenu1.addSubMenu (folders[i], subMenu2);
                noOfFiles = filesArray.size();
            }

            m.addSubMenu (menuName, subMenu1);
        }
        else
        {
            subMenu2.clear();
            File searchDir (dir + "/" + menuName);
            Array<File> exampleFilesArray;
            searchDir.findChildFiles (exampleFilesArray, File::findFiles, false, "*.csd");
            exampleFilesArray.sort();
            filesArray.addArray (exampleFilesArray);

            for (int fileCnt = noOfFiles; fileCnt < filesArray.size(); fileCnt++)
            {
                subMenu2.addItem (fileCnt + indexOffset, filesArray[fileCnt].getFileNameWithoutExtension());
            }

            m.addSubMenu (menuName, subMenu2);

        }


    }

    static PropertiesFile::Options getStorageProps()
    {
        PropertiesFile::Options options;
        options.applicationName = "Cabbage2";
        options.filenameSuffix = "settings";
        options.osxLibrarySubFolder = "Preferences";
#if JUCE_LINUX
        options.folderName = "~/.config/Cabbage2";
#else
        options.folderName          = "Cabbage2";
#endif
        return options;
    }

    static void addExampleFilesToPopupMenu (PopupMenu& m, Array<File>& filesArray, String dir, String ext, int indexOffset)
    {
        ignoreUnused(ext);
        filesArray.clear();
        addExamples (m, "Effects", dir, filesArray, CabbageExamplesFolder::getEffects(), indexOffset);
        addExamples (m, "FilePlayers", dir, filesArray, StringArray(), indexOffset);
        addExamples (m, "FunAndGames", dir, filesArray, StringArray(), indexOffset);
        addExamples (m, "GEN", dir, filesArray, StringArray(), indexOffset);
        addExamples (m, "Instruments", dir, filesArray, CabbageExamplesFolder::getInstruments(), indexOffset);
        addExamples (m, "LiveSampling", dir, filesArray, StringArray(), indexOffset);
        addExamples (m, "MIDI", dir, filesArray, StringArray(), indexOffset);
        addExamples (m, "Miscellaneous", dir, filesArray, StringArray(), indexOffset);
        addExamples (m, "Utilities", dir, filesArray, StringArray(), indexOffset);
        addExamples (m, "Widgets", dir, filesArray, StringArray(), indexOffset);
    }

    static void addFilesToPopupMenu (PopupMenu& m, Array<File>& filesArray, String dir, int indexOffset)
    {

        File searchDir (dir);
        Array<File> searchFilesArray;
        PopupMenu subMenu;
        searchDir.findChildFiles (searchFilesArray, File::findFiles, false, "*.csd");
        searchFilesArray.sort();
        filesArray.addArray (searchFilesArray);

        for (int fileCnt = 0; fileCnt < filesArray.size(); fileCnt++)
        {
            m.addItem (fileCnt + indexOffset, filesArray[fileCnt].getFileNameWithoutExtension());
        }

    }

    //======================================================================================
    static void showMessage (String message)
    {
        AlertWindow::showMessageBoxAsync (AlertWindow::WarningIcon,
                                          "Cabbage Message",
                                          message,
                                          "Ok");

    }

    static void showMessage (String title, String message)
    {
        AlertWindow::showMessageBoxAsync (AlertWindow::WarningIcon,
                                          title,
                                          message,
                                          "Ok");

    }

    static void setImagesForButton (ImageButton* button, const Image image)
    {
        button->setImages (true, true, true, image, 1, Colours::transparentBlack, image,
                           1, Colours::transparentBlack, image, .6f, Colours::transparentBlack, 0.f);
    }

    //==========================================================
    static bool compDouble (double x, int y)
    {
        if ( x > (double)y - 0.0001 && x < (double)y + 0.0001)
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

    static double roundToMultiple (double x, double multiple)
    {
        return round (x / multiple) * multiple;
    }

    static int roundIntToMultiple (int x, int multiple)
    {
        return int(round (x / multiple) * multiple);
    }

    static double roundToPrec (double x, int prec)
    {
        double power = 1.0;
        int i;

        if (prec > 0)
            for (i = 0; i < prec; i++)
                power *= 10.0;


        if (x > 0)
            x = floor (x * power + 0.5) / power;
        else if (x < 0)
            x = ceil (x * power - 0.5) / power;

        return x;
    }

    //===========================================================================================
    static Justification getJustification (String align)
    {
        if (align == "centre")
            return Justification::centred;
        else if (align == "left")
            return Justification::left;
		else if (align == "top")
			return Justification::centredTop;
        else
            return Justification::right;
    }  

    //===========================================================================================
    static Range<int> getCabbageSectionRange(String csdText)
    {
        Range<int> range;
        StringArray lines;
        lines.addLines(csdText);
        for(int i = 0 ; i < lines.size() ; i++)
        {
            if (lines[i] == "<Cabbage>")
                range.setStart(i);
            else if (lines[i].contains("</Cabbage>"))
                range.setEnd(i);
        }

        return range;
    }

	static StringArray getTokens(String code, char breakChar)
	{
        StringArray tokens;
        int linesize = code.length();
        int from = 0; // index of the first char of the current token in this line of code
        
        for (int i = 0; i < linesize; i++) // let's find all the tokens in this line of code...
        {
            while (i < linesize && (char)code[i] != breakChar) // let's find the end of a token...
            {
                if (code[i] == '\"')   // excuse anything in quotes..
                {
                    i++; // so, skip the first quote char

                    while (i < linesize && code[i] != '\"') // continue to skip until endline or end quote char
                        i++;
                }
                i++; // move to the next char
            }

            tokens.add(code.substring(from, i)); // let's add the token to the tokens array
            from = i + 1; // set new start position for the next token
        }
        
        return tokens;
	}

 
    //===========================================================================================
    static void showMessage (double num)
    {
        String str (num);
        File thisFile (File::getSpecialLocation (File::currentApplicationFile));
        AlertWindow alert (thisFile.getFullPathName(), str, AlertWindow::WarningIcon);
        alert.showMessageBoxAsync (AlertWindow::WarningIcon, thisFile.getFullPathName(), str, "Ok");
    }

    static void showMessage (String title, String message, LookAndFeel* feel, Component* mainWindow)
    {
        if (title.length() < 1)
			title = "Cabbage Message";

        mainWindow->setAlwaysOnTop (false);
        AlertWindow alert (title, message, AlertWindow::WarningIcon);
        alert.setLookAndFeel (feel);
        alert.setAlwaysOnTop (true);
        alert.addButton ("Ok", 1);
#if !defined(AndroidBuild)
        alert.runModalLoop();
#else
        alert.showMessageBoxAsync (AlertWindow::WarningIcon, "Cabbage Message" , message, "Ok");
#endif
        mainWindow->setAlwaysOnTop (true);
    }

    static void showMessage (String message, LookAndFeel* feel)
    {
        AlertWindow alert ("Cabbage Message" , message, AlertWindow::WarningIcon);
        alert.setLookAndFeel (feel);
        alert.addButton ("Ok", 1);
#if !defined(AndroidBuild)
        alert.runModalLoop();
#else
        alert.showMessageBoxAsync (AlertWindow::WarningIcon, "Cabbage Message" , message, "Ok");
#endif
    }

    static void showMessage (String title, String message, LookAndFeel* feel)
    {
        if(!feel)
        {
//            NativeMessageBox::showMessageBox(AlertWindow::AlertIconType::WarningIcon, title, message);
            jassertfalse;
            return;
        }
        AlertWindow alert (title, message, AlertWindow::WarningIcon);
        alert.setLookAndFeel (feel);
        alert.addButton ("Ok", 1);
#if !defined(AndroidBuild)
        alert.runModalLoop();
#else
        alert.showMessageBoxAsync (AlertWindow::WarningIcon, "Cabbage Message" , message, "Ok");
#endif
    }

	static bool showMessageWithHideOption(String title, String message, LookAndFeel* feel)
	{		
		AlertWindow alert(title, message, AlertWindow::WarningIcon);
		alert.setLookAndFeel(feel);
		alert.addButton("Ok", 1);
		ToggleButton tglBox;
		tglBox.setBounds(10, alert.getHeight() - 20, 200, 30);
		LookAndFeel_V4 lAndf;
		tglBox.setLookAndFeel(&lAndf);
		tglBox.setButtonText("Don't show again");
		alert.addCustomComponent(&tglBox);
#if !defined(AndroidBuild)
		alert.runModalLoop();
#else
		alert.showMessageBoxAsync(AlertWindow::WarningIcon, "Cabbage Message", message, "Ok");
#endif
		
		alert.setLookAndFeel(nullptr);
		tglBox.setLookAndFeel(nullptr);

		return tglBox.getToggleState();
	}

    static int showYesNoMessage (String message, LookAndFeel* feel, int cancel = 0)
    {
#if defined(MACOS)
        int messageBoxResult = NativeMessageBox::showYesNoCancelBox (AlertWindow::AlertIconType::WarningIcon,
                                                                     "Warning", message, nullptr, nullptr);
        return messageBoxResult;
#endif

        AlertWindow alert ("Cabbage Message", message, AlertWindow::WarningIcon, nullptr);
        alert.setLookAndFeel (feel);
        alert.addButton ("Yes", 1);
        alert.addButton ("No", 2);
		alert.setAlwaysOnTop(true);
        if (cancel == 1)
            alert.addButton ("Cancel", 0);

        int result;
#if !defined(AndroidBuild)
        result = alert.runModalLoop();
#else
        result = alert.showYesNoCancelBox (AlertWindow::QuestionIcon, "Warning", message, "Yes", "No", "Cancel", nullptr, nullptr);
#endif
        return result;

    }
    
    static int showAlertMessageWithTextEditor (String title, String message, LookAndFeel* feel, String* presetName)
    {

        AlertWindow w (title,
                       message,
                       AlertWindow::WarningIcon);
        w.setLookAndFeel(feel);
        //w.getLookAndFeel().setColour(AlertWindow::ColourIds::backgroundColourId, Colour(43, 43, 43));
        //w.setSize(8500, 300);
        w.addTextEditor ("text", "enter name here", "");
        w.addButton ("OK",     1, KeyPress (KeyPress::returnKey, 0, 0));
        w.addButton ("Cancel", 0, KeyPress (KeyPress::escapeKey, 0, 0));
        
//        if (w.runModalLoop() != 0) // if they picked 'ok'
//        {
//
//        }
        
//        AlertWindow alert ("Cabbage Message", message, AlertWindow::WarningIcon, 0);
//        alert.setLookAndFeel (feel);
//        alert.addButton ("Yes", 1);
//        alert.addButton ("No", 2);
//        alert.setAlwaysOnTop(true);
//        if (cancel == 1)
//            alert.addButton ("Cancel", 0);
//
        int result;
#if !defined(AndroidBuild)
        result = w.runModalLoop();
        *presetName = w.getTextEditorContents ("text");
#else
        result = w.showYesNoCancelBox (AlertWindow::QuestionIcon, "Warning", message, "Yes", "No", "Cancel", nullptr, nullptr);
#endif
        return result;
        
    }
    


    //==========================================================================================
    static String cabbageString (String input, Font font, float availableWidth)
    {
        //This method returns a truncated string if it is too big to fit into its available
        //space.  Dots are added at the end to signify that it has been truncated.

        String newStr;
        float stringWidth = jmax (font.getStringWidthFloat (input), 1.f);
        int numChars = jmax (1, input.length());
        float charWidth = stringWidth / numChars;

        if (stringWidth > availableWidth)
        {
            int numCharsToInclude = int (((availableWidth / charWidth) + 0.5) - 2);
            newStr = input.substring (0, numCharsToInclude);
            newStr << "..";
            return newStr;
        }
        else
            return input;
    }


    static String getFileAndPath (File csdFile, String filename)
    {
        if (filename.isEmpty())
            return String();

        return File (csdFile).getParentDirectory().getChildFile (filename).getFullPathName();
    }
    //===================================================================================
    static int getNumberOfDecimalPlaces (String incr)
    {

        int count = 0;
        float num = abs ((float)incr.getDoubleValue());
        num = num - int (num);

        while (abs (num) >= 0.0000001)
        {
            num = num * 10;
            count = count + 1;
            num = num - int (num);
        }


        if (incr.indexOf (".") >= 0)
        {
            String subTemp = incr.substring (incr.indexOf ("."), 10);

            if (count < subTemp.length() - 1)
                return count;
            else
                return subTemp.length() - 1;
        }

        return count;

    }

    //===========================================================================
    static Font getComponentFont (int style = 1)
    {
        Font font = Font (11.5, style); // ("Verdana", 11.5, bold);
        return font;
    }

    static Font getValueFont()
    {
        Font font = Font ("Helvetica", 11.5, 1);
        return font;
    }

    static Font getEmbeddedFont()
    {
        MemoryInputStream is (CabbageBinaryData::DroidSansMono_ttf, CabbageBinaryData::DroidSansMono_ttfSize, false);
        CustomTypeface* newTypeface = new CustomTypeface (is);
        Font myFont (newTypeface);
        return myFont;
    }
    //===================================================================
    static Font getTitleFont()
    {
        Font font = Font ("Helvetica", 12.5, 1);
        return font;
    }

    static Colour getTitleFontColour()
    {
        Colour cl = Colour::fromRGBA (160, 160, 160, 255);
        return cl;
    }

    static Colour getComponentFontColour()
    {
        Colour cl = Colour::fromRGBA (160, 160, 160, 255);
        return cl;
    }

    static Colour getDarkerBackgroundSkin()
    {
        Colour skin = getBackgroundSkin().darker (0.4f);
        return skin;
    }

    static Colour getComponentSkin()
    {
        Colour skin = Colour::fromRGBA (75, 85, 90, 100); //some transparency
        return skin;
    }

    static const Colour getBorderColour()
    {
        return getComponentFontColour().withMultipliedAlpha (0.2f);
    }

    static const float getBorderWidth()
    {
        return 1.0f;
    }

    static Colour getBackgroundSkin()
    {
        Colour skin = Colour::fromRGB (5, 15, 20);
        return skin;
    }


    static File getRealUserHomeDirectory ()
    {
#ifdef JUCE_MAC
        struct passwd *pw = getpwuid (getuid ());
        if (pw == nullptr)
        {
            jassertfalse;   // unable to read the user info
            return File::getSpecialLocation (File::userHomeDirectory);
        }
        
        return File (String (pw->pw_dir));
#endif
        return File::getSpecialLocation(File::userHomeDirectory);
    }

    //==========================================================================================
    static bool hasCabbageTags (File inputFile)
    {
        const String csdFile = inputFile.loadFileAsString();

        if (csdFile.contains ("<Cabbage>") && csdFile.contains ("</Cabbage>"))
            return true;
        else
            return false;
    }

    static String correctPathSlashes (String path)
    {
        if (SystemStats::getOperatingSystemType() == SystemStats::OperatingSystemType::Linux
            || SystemStats::getOperatingSystemType() == SystemStats::OperatingSystemType::MacOSX)
        {
            path = path.replace ("\\", "/");
        }
        else
        {
            path = path.replace ("/", "\\");
        }

        return path;

    }

    //==========================================================================================
    const static String getPlantFileAsXmlString(File xmlFile)
    {
        StringArray linesFromXmlFile;
        linesFromXmlFile.addLines(xmlFile.loadFileAsString());
        bool shouldReplaceChars = false;
        for( int i = 0 ; i < linesFromXmlFile.size()-1; i++)
        {
            if(shouldReplaceChars)
            {
                linesFromXmlFile.set(i, linesFromXmlFile[i].replace ("&", "&amp;")
                        .replace ("<", "&lt;")
                        .replace(">", "&gt;")
                        .replace ("\"", "$quote;")
                        .replace ("'", "&apos;"));

            }

            if(linesFromXmlFile[i] == "<cabbagecodescript>" || linesFromXmlFile[i] == "<csoundcode>")
                shouldReplaceChars = true;
            else if(linesFromXmlFile[i+1] == "</cabbagecodescript>" || linesFromXmlFile[i+1] == "</csoundcode>")
                shouldReplaceChars = false;


        }
        return linesFromXmlFile.joinIntoString("\n");
    }


    //==========================================================================================
    static void addCustomPlantsToMenu (PopupMenu& m, Array<File>& plantFiles, String userDir)
    {

        PopupMenu menu;
        // int fileCnt = 0;

        if (File (userDir).exists())
        {
            FileSearchPath filePaths (userDir);
            //CabbageUtilities::showMessage(appProperties->getUserSettings()->getValue("CabbageFilePaths"))

            //add all files in root of specifed directories
            for (int i = 0; i < filePaths.getNumPaths(); i++)
            {
                File plantDir (filePaths[i]);
                plantDir.findChildFiles (plantFiles, File::findFiles, false, "*.plant;*.xml");
            }

            for (int i = 0; i < plantFiles.size(); ++i)
            {
				std::unique_ptr<XmlElement> xml(XmlDocument::parse (getPlantFileAsXmlString(plantFiles[i])));

                if (xml) //if valid xml
                {
                    if (xml->hasTagName ("plant"))
                    {
                        forEachXmlChildElement (*xml, e)
                        {
                            if (e->getTagName() == "name")
                                m.addItem(i + 100, e->getAllSubText());
                        }
                    }
                }
            }

            m.addSeparator();
            //m.setLookAndFeel(&this->getLookAndFeel());
        }
    }

    static void addFilesToPluginBundle (File csdFile, File exportDir, LookAndFeel* look)
    {
        Array<File> invalidFiles;
        bool invalidFilename = false;
        StringArray csdArray;
        File parentDirectory = csdFile.getParentDirectory();
        csdArray.addLines (csdFile.loadFileAsString());

        for (int i = 0; i < csdArray.size(); i++)
        {
            if (csdArray[i].contains ("<CabbageIncludes>"))
            {
                int lineIndex = 1;

                while (!csdArray[i + lineIndex].contains ("</CabbageIncludes>") && i + lineIndex < csdArray.size())
                {
                    File includeFile (parentDirectory.getFullPathName() + "/" + csdArray[i + lineIndex]);
                    File newFile (exportDir.getParentDirectory().getFullPathName() + "/" + csdArray[i + lineIndex]);

                    if (includeFile.exists())
                    {
                        invalidFilename = true;
                        includeFile.copyFileTo (newFile);
                    }
                    else
                    {
                        invalidFiles.add (includeFile);
                    }

                    lineIndex++;
                }

                if (invalidFilename)
                {
                    StringArray invalidFileNames;

                    for (int y = 0; y < invalidFiles.size(); y++)
                        invalidFileNames.add (invalidFiles[y].getFullPathName());

                    showMessage ("Cabbage could not bundle the following files\n" + invalidFileNames.joinIntoString ("\n") + "\nPlease make sure they are located in the same folder as your .csd file.", look);
                }
            }
        }
    }




    //======================================================================================
    static String setDecimalPlaces (const double& x, const int& numDecimals)
    {
        int y = int(x);
        double z = x - y;
        double m = pow ((double)10.f, (double)numDecimals);
        double q = z * m;
        double r = roundToIntAccurate (q);

        return String (static_cast<double> (y) + (1.0 / m) * r);
    }
    //======================================================================================
    static int getNumberOfOccurances(String text, String word)
    {
        int wordCount = 0;
        
        if(text.indexOf(word) == -1)
            return wordCount;
        
        int index = text.indexOf(0, word);

        while(index != -1)
        {
            wordCount++;
            index = text.indexOf(index+word.length(), word);
        }
        
        return wordCount;

    }
    //======================================================================================
    static void writeValueTreeToFile (ValueTree valueTree)
    {
		std::unique_ptr<XmlElement> data(valueTree.createXml());
        // only works when there are no objects in the array...
        //write new xml settings files based on data from user settings file, but using ValueTree
        //data->writeTo (File (filePath));
    }

    //======= method for replacing the contents of an identifier with new values..
    static String replaceIdentifier (String line, String identifier, String updatedIdentifier)
    {
        if (identifier.length() < 2)
            return line;

        if (updatedIdentifier.length() < 2)
            return line;

        int startPos = line.indexOf (identifier);

        if (startPos == -1)
            return "";

        String firstSection = line.substring (0, line.indexOf (identifier));
        line = line.substring (line.indexOf (identifier));
        String secondSection = line.substring (line.indexOf (")") + 1);

        return firstSection + updatedIdentifier + secondSection;
    }

	static void searchDirectoryForFiles(String workingDir, String fileType, Array<File> & folderFiles, StringArray &comboItems, int& numberOfFiles)
	{
		Array<File> dirFiles;
		File pluginDir;

		if (workingDir.isNotEmpty())
			pluginDir = File::getCurrentWorkingDirectory().getChildFile(workingDir);
		else
			pluginDir = File::getCurrentWorkingDirectory();

		pluginDir.findChildFiles(dirFiles, 2, false, fileType);

		for (int i = 0; i < dirFiles.size(); ++i)
			folderFiles.add(dirFiles[i]);

		folderFiles.sort();

		for (int i = 0; i < folderFiles.size(); i++)
		{
			CabbageUtilities::debug(folderFiles[i].getFullPathName());
		}

		for (int i = 0; i < folderFiles.size(); i++)
		{
			comboItems.add(folderFiles[i].getFileNameWithoutExtension());
		}

		numberOfFiles = folderFiles.size();

	}
    
    static String convertWhitespaceEscapeChars(const String& str)
    {
        auto newStr = str.replace("\\n", "\n");
        newStr = newStr.replace("\\t", "\t");
        return newStr;
    }
    
    static String removeWhitespaceEscapeChars(const String& str)
    {
        return convertWhitespaceEscapeChars(str).removeCharacters("\n\r\t");
    }
    
    static Typeface::Ptr getFontFromFile(File fontFile)
    {
        if(fontFile.existsAsFile())
        {
            std::unique_ptr<InputStream> inStream (fontFile.createInputStream());
            MemoryBlock mb;
            inStream->readIntoMemoryBlock(mb);
            Typeface::Ptr fontPtr = Typeface::createSystemTypefaceFor (mb.getData(), mb.getSize());
            return fontPtr;
        }
        else
        {
            return nullptr;
        }
        
    }

};

//==============================================================================
// round button
//==============================================================================
class RoundButton : public Component,
    public ChangeBroadcaster

{
    String type;
    String visible;
    Colour colour;
    int mode;
public:
    RoundButton (String _type, Colour _colour): Component(), visible ("bacground")
    {
        setName (_type);
        type = _type;
        colour = _colour;
        mode = 0;
    }
    ~RoundButton() override {}

    void mouseDown (const MouseEvent& e) override
    {
        ignoreUnused(e);
        //Logger::writeToLog("Mouse down on round button:"+String(type));
        sendChangeMessage();
        mode = (mode == 1 ? 0 : mode + 1);
    }

    void paint (Graphics& g)  override
    {
        //Logger::writeToLog(type);
        if (type.contains ("zoom"))
        {
            g.fillAll (Colours::transparentBlack);
            g.setColour (CabbageUtilities::getBackgroundSkin());
            g.fillEllipse (0.f, 0.f, getWidth(), getHeight());
            g.setColour (Colours::white.withAlpha (.8f));
            g.fillEllipse (1, 1, getWidth() - 2, getHeight() - 2);
            g.setColour (Colours::black);
            g.fillRoundedRectangle (getWidth()*.18f, getHeight()*.4f, getWidth()*.65f, getHeight()*.25f, 2);

            if (getName() == "zoomIn")
                g.fillRoundedRectangle (getWidth()*.38f, getHeight()*.20f, getWidth()*.25f, getHeight()*.65f, 2);
        }
        else
        {
            g.fillAll (Colours::transparentBlack);
            g.setColour (CabbageUtilities::getBackgroundSkin());
            g.fillEllipse (0, 0, getWidth(), getHeight());
            g.setColour ((visible == "foreground" ? colour : colour.withAlpha (.3f)));
            g.fillEllipse (1, 1, getWidth() - 2, getHeight() - 2);
            g.setColour (colour.contrasting());
            g.drawFittedText (type, 0, 0, getWidth(), getHeight(), Justification::centred, 1);

            if (visible == "off")
            {
                g.setColour (colour.contrasting());
                g.drawLine (0, 0, getWidth(), getHeight(), 2);
                g.drawLine (0, getHeight(), getWidth(), 0, 2);
            }
        }
    }

    void setVisibilityStatus (String status)
    {
        visible = status;
    }

    int getMode()
    {
        return mode;
    }

    void setMode (int mod)
    {
        mode = mod;
    }
};

class FontsArray : public StringArray
{
public:
    FontsArray()
    {
        add ("Andale Mono");
        add ("Consolas");
        add ("Courier	Courier");
        add ("DejaVu Sans Mono");
        add ("Droid Sans Mono");
        add ("Everson Mono");
        add ("Fixedsys");
        add ("HyperFont");
        add ("Inconsolata");
        add ("Letter Gothic");
        add ("Lucida Console");
        add ("Monaco");
        add ("Monofur");
        add ("PragmataPro");
        add ("Prestige Elite");
        add ("ProFont");
        add ("Source Code Pro");
        add ("Terminus");
        add ("Tex Gyre Cursor");
        add ("Trixie	Trixie");
        add ("UM Typewriter");
    }

    ~FontsArray() {}

};
#endif
