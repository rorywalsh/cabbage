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


#include "../JuceLibraryCode/JuceHeader.h"

#pragma warning(disable: 4244) // possible loss of data
#pragma warning(disable: 4100) // possible loss of data

using namespace std;

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

//===========================================================================================
//some utility functions used across classes...
//===========================================================================================
class CabbageUtilities
{
public:
    CabbageUtilities() {};
    ~CabbageUtilities() {};

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
        button->setImages(true, true, true, image, 1, Colours::transparentBlack, image,
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
        return round (x / multiple) * multiple;
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

        if (x == -0)
            x = 0;

        return x;
    }

    //===========================================================================================
    static Justification getJustification (String align)
    {
        if (align == "centre")
            return Justification::centred;
        else if (align == "left")
            return Justification::left;
        else
            return Justification::right;

        return Justification::centred;
    }

    //===========================================================================================
    static StringArray getTokens (String code, char breakChar)
    {
        StringArray tokens;

        const char* str = code.toUTF8().getAddress();

        do
        {
            const char* begin = str;

            while (*str != ' ' && *str)
                str++;

            tokens.add (string (begin, str));
        }
        while (0 != *str++);

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
        if (title.length() < 1)title = "Cabbage Message";

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
        AlertWindow alert (title, message, AlertWindow::WarningIcon);
        alert.setLookAndFeel (feel);
        alert.addButton ("Ok", 1);
#if !defined(AndroidBuild)
        alert.runModalLoop();
#else
        alert.showMessageBoxAsync (AlertWindow::WarningIcon, "Cabbage Message" , message, "Ok");
#endif
    }

    static int showYesNoMessage (String message, LookAndFeel* feel, int cancel = 0)
    {
        AlertWindow alert ("Cabbage Message", message, AlertWindow::WarningIcon, 0);
        alert.setLookAndFeel (feel);
        alert.addButton ("Yes", 0);
        alert.addButton ("No", 1);

        if (cancel == 1)
            alert.addButton ("Cancel", 2);

#if !defined(AndroidBuild)
        int result = alert.runModalLoop();
#else
        int result = alert.showYesNoCancelBox (AlertWindow::QuestionIcon, "Warning", message, "Yes", "No", "Cancel", nullptr, nullptr);
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


	static String getFileAndPath(File csdFile, String filename)
	{
		if(filename.isEmpty())
			return String::empty;
			
		return File(csdFile).getParentDirectory().getChildFile(filename).getFullPathName();
	}
    //===================================================================================
    static int getNumberOfDecimalPlaces (String incr)
    {

        if (incr.indexOf (".") >= 0)
        {
            String subTemp = incr.substring (incr.indexOf ("."), 10);
            return subTemp.length() - 1;
        }
        
		return 2;

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
    static void addCustomPlantsToMenu (PopupMenu& m, Array<File>& plantFiles, String userDir)
    {

        PopupMenu menu;
        int fileCnt = 0;

        if (File (userDir).exists())
        {
            FileSearchPath filePaths (userDir);
            //CabbageUtilities::showMessage(appProperties->getUserSettings()->getValue("CabbageFilePaths"))

            //add all files in root of specifed directories
            for (int i = 0; i < filePaths.getNumPaths(); i++)
            {
                File plantDir (filePaths[i]);
                plantDir.findChildFiles (plantFiles, File::findFiles, false, "*.plant");

            }

            for (int i = 0; i < plantFiles.size(); ++i)
                m.addItem (i + 100, plantFiles[i].getFileNameWithoutExtension());


            //fileCnt = cabbageFiles.size();

            //increment menu size and serach recursively through all subfolders in specified dirs
            for (int i = 0; i < filePaths.getNumPaths(); i++)
            {
                Array<File> subFolders;
                File searchDir (filePaths[i]);
                subFolders.add (searchDir);
                searchDir.findChildFiles (subFolders, File::findDirectories, true);

                //remove parent dirs from array
                for (int p = 0; p < filePaths.getNumPaths(); p++)
                    subFolders.removeAllInstancesOf (filePaths[p]);

                PopupMenu subMenu;

                for (int subs = 0; subs < subFolders.size(); subs++)
                {
                    fileCnt = plantFiles.size();
                    subFolders[subs].findChildFiles (plantFiles, File::findFiles, false, "*.plants");
                    subMenu.clear();

                    for (int fileIndex = fileCnt + 1; fileIndex < plantFiles.size(); fileIndex++)
                        subMenu.addItem (fileIndex + 100, plantFiles[fileIndex].getFileNameWithoutExtension());


                    m.addSubMenu (subFolders[subs].getFileNameWithoutExtension(), subMenu);
                }

                subMenu.clear();
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
        int y = x;
        double z = x - y;
        double m = pow ((double)10.f, (double)numDecimals);
        double q = z * m;
        double r = roundToIntAccurate (q);

        return String (static_cast<double> (y) + (1.0 / m) * r);
    }


    //======================================================================================
    static void writeValueTreeToFile (ValueTree valueTree, String filePath)
    {
        ScopedPointer<XmlElement> data (valueTree.createXml());
        // only works when there are no objects in the array...
        //write new xml settings files based on data from user settings file, but using ValueTree
        data->writeToFile (File (filePath), String::empty);
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
    ~RoundButton() {}

    void mouseDown (const MouseEvent& e)
    {
        //Logger::writeToLog("Mouse down on round button:"+String(type));
        sendChangeMessage();
        mode = (mode == 1 ? 0 : mode + 1);
    }

    void paint (Graphics& g)
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
            g.fillRoundedRectangle (getWidth()*.18, getHeight()*.4f, getWidth()*.65, getHeight()*.25, 2);

            if (getName() == "zoomIn")
                g.fillRoundedRectangle (getWidth()*.38f, getHeight()*.20, getWidth()*.25, getHeight()*.65, 2);
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
