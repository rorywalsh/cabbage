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

#define OK 0

//simple abstract class used to hold information about about displays
class SignalDisplay
{
public:
    float yScale;
    int windid, min ,max, size;
    String caption;

    SignalDisplay(String _caption, int _id, float _scale, int _min, int _max, int _size):
        caption(_caption),
        windid(_id),
        yScale(_scale),
        min(_min),
        max(_max),
        size(_size)
    {}

    ~SignalDisplay()
    {
        points.clear();
    }

    Array<float, CriticalSection> getPoints()
    {
        return points;
    }

    void setPoints(Array <float, CriticalSection > tablePoints)
    {
        points.swapWith(tablePoints);
    }

private:
    Array <float, CriticalSection > points;
};


//not used yet.....
class KeyboardShortcutKeys
{
public:
    KeyboardShortcutKeys(XmlElement* xml):xmlData(xml)
    {

    }

    ~KeyboardShortcutKeys() {}

    String getKeyPress(String name)
    {
        for(int i=0; xmlData->getNumAttributes(); i++)
        {
            if(xmlData->getAttributeName(i)==name)
                return xmlData->getAttributeValue(i);
        }
    }

    int keyCode;
    ModifierKeys mods;
    ScopedPointer<XmlElement> xmlData;
};


//===========================================================================================
//some utility functions used across classes...
//===========================================================================================
class CabbageUtilities
{
public:
    CabbageUtilities() {};
    ~CabbageUtilities() {};


//===========================================================================================
    string juce2Str(juce::String inStr)
    {
        string str(inStr.toUTF8());
        return str;
    }

//===========================================================================================
    String str2Juce(std::string inStr)
    {
        String str(inStr.c_str());
        return str;
    }

    float cabbageABS(float in)
    {
        if(in<0)
            return in*-1;
        else return in;
    }
//
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
//===========================================================================================
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

	static void setImagesForButton(ImageButton* button, const Image image)
	{
	button->setImages(true, true, true, image, 1, Colours::transparentBlack, image,
					 1, Colours::transparentBlack, image, .8, Colours::transparentBlack, 0);
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
//===========================================================================================
    static const Colour getRandomColour()
    {
        const Colour colour = Colour(Random::getSystemRandom().nextInt(255),
                                     Random::getSystemRandom().nextInt(255),
                                     Random::getSystemRandom().nextInt(255));
        return colour;
    }

//===========================================================================================
    static void showMessageWithLocation(String message)
    {
        File thisFile(File::getSpecialLocation(File::currentApplicationFile));
        AlertWindow alert(thisFile.getFullPathName() , message, AlertWindow::WarningIcon);
        alert.showMessageBoxAsync(AlertWindow::WarningIcon, thisFile.getFullPathName() , message, "Ok");
    }
//===========================================================================================
    static void showMessage(double num)
    {
        String str(num);
        File thisFile(File::getSpecialLocation(File::currentApplicationFile));
        AlertWindow alert(thisFile.getFullPathName(), str, AlertWindow::WarningIcon);
        alert.showMessageBoxAsync(AlertWindow::WarningIcon, thisFile.getFullPathName(), str, "Ok");
    }
//===========================================================================================
    static void showMessage(String title, String message, LookAndFeel* feel, Component* mainWindow)
    {
        if(title.length()<1)title="Cabbage Message";
        mainWindow->setAlwaysOnTop(false);
//	mainWindow->toBack();
        AlertWindow alert(title, message, AlertWindow::WarningIcon);
        alert.setLookAndFeel(feel);
        alert.setAlwaysOnTop(true);
        alert.addButton("Ok", 1);
#if !defined(AndroidBuild)
        alert.runModalLoop();
#else
        alert.showMessageBoxAsync(AlertWindow::WarningIcon, "Cabbage Message" , message, "Ok");
#endif
        mainWindow->setAlwaysOnTop(true);
    }

//===========================================================================================
    static void showMessage(String message, LookAndFeel* feel)
    {
        AlertWindow alert("Cabbage Message" , message, AlertWindow::WarningIcon);
        alert.setLookAndFeel(feel);
        alert.addButton("Ok", 1);
#if !defined(AndroidBuild)
        alert.runModalLoop();
#else
        alert.showMessageBoxAsync(AlertWindow::WarningIcon, "Cabbage Message" , message, "Ok");
#endif
    }

    static void showMessage(String title, String message, LookAndFeel* feel)
    {
        AlertWindow alert(title, message, AlertWindow::WarningIcon);
        alert.setLookAndFeel(feel);
        //alert.showMessageBoxAsync(AlertWindow::WarningIcon, "Cabbage Message" , message, "Ok");
        alert.addButton("Ok", 1);
#if !defined(AndroidBuild)
        alert.runModalLoop();
#else
        alert.showMessageBoxAsync(AlertWindow::WarningIcon, "Cabbage Message" , message, "Ok");
#endif
    }

//===========================================================================================
    static int showYesNoMessage(String message, LookAndFeel* feel, int cancel=0)
    {
        AlertWindow alert("Cabbage Message", message, AlertWindow::WarningIcon, 0);
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
//===========================================================================================
    StringArray CreateStringArray(std::string str)
    {
        StringArray strArray;
        //std::string str = sourceEditorComponent->textEditor->getText().toUTF8();
        int cnt=0;
        for(int i=0; i<(int)str.length(); i++)
        {
            long pos = (int)str.find("\n", 0);
            if(pos!=std::string::npos)
            {
                strArray.add(str2Juce(str.substr(0, pos+1)));
                str.erase(0, pos+1);
                cnt++;
            }
        }
        //add the very last line which won't have a "\n"...
        strArray.add(str2Juce(str.substr(0, 100)));
        return strArray;
    }
	

//==========================================================================================
	static void addFilesToPluginBundle(File csdFile, File exportDir, LookAndFeel *look)
    {
		Array<File> invalidFiles;
        bool invalidFilename = false;
		StringArray csdArray;
		File parentDirectory = csdFile.getParentDirectory();
		csdArray.addLines(csdFile.loadFileAsString());
		for(int i=0; i<csdArray.size(); i++)
		{
			if(csdArray[i].contains("<CabbageIncludes>"))
			{
				int lineIndex = 1;
				while(!csdArray[i+lineIndex].contains("</CabbageIncludes>") && i+lineIndex<csdArray.size())
				{
					File includeFile(parentDirectory.getFullPathName()+"/"+csdArray[i+lineIndex]);					
					File newFile(exportDir.getParentDirectory().getFullPathName()+"/"+csdArray[i+lineIndex]);
					if(includeFile.exists())
					{
                        invalidFilename = true;
						includeFile.copyFileTo(newFile);
					}
					else{
						invalidFiles.add(includeFile);
					}
					lineIndex++;
				}
                
                if(invalidFilename)
                {
                    StringArray invalidFileNames;
                    for(int y=0;y<invalidFiles.size();y++)
                        invalidFileNames.add(invalidFiles[y].getFullPathName());
                    showMessage("Cabbage could not bundle the following files\n"+invalidFileNames.joinIntoString("\n")+"\nPlease make sure they are located in the same folder as your .csd file.", look);
                }
                
				
//					CabbageGUIType cAttr(csdArray[i], -99);
//					this->getProperties().set("colour", cAttr.getStringProp(CabbageIDs::colour));
//					this->getProperties().set("fontcolour", cAttr.getStringProp(CabbageIDs::fontcolour));
//					this->getProperties().set("titlebarcolour", cAttr.getStringProp(CabbageIDs::titlebarcolour));
//					this->lookAndFeelChanged();
//					setName(cAttr.getStringProp(CabbageIDs::caption));
			}
		}
	}
//==========================================================================================
    static Array<File> launchFileBrowser(String title, WildcardFileFilter filter, String fileType, int mode, File initialDir, bool useNative, LookAndFeel *look)
    {
        const bool warnAboutOverwrite = true;
        Array<File> results;
#if !defined(AndroidBuild)
        //if set to open or browse for files
        if(mode==1)
        {
#if !defined(CABBAGE_AU) && !defined(Cabbage_Build_Standalone)
            //in plugin mode it's best to use Cabbage file browser instead of a system one

            useNative=false;
            //CabbageUtilities::debug("I shuldn't be here");
#endif

            if(useNative==false)
            {
                FileBrowserComponent browserComponent ( FileBrowserComponent::openMode|
                                                        FileBrowserComponent::canSelectFiles,
                                                        initialDir,
                                                        &filter,
                                                        nullptr);

                FileChooserDialogBox box (title, String::empty,
                                          browserComponent, warnAboutOverwrite,
                                          CabbageUtilities::getDarkerBackgroundSkin());

                if(look!=nullptr)
                    box.setLookAndFeel(look);

                box.setAlwaysOnTop(true);

                if (box.show())
                {
                    for (int i = 0; i < browserComponent.getNumSelectedFiles(); ++i)
                        results.add (browserComponent.getSelectedFile (i));
                }
            }
            else
            {
                FileChooser openFC(title, initialDir, fileType, true);
                if(openFC.browseForFileToOpen())
                    results = openFC.getResults();
            }
        }

        //set to save files
        if(mode==0)
        {
            if(useNative==false)
            {
                FileBrowserComponent browserComponent ( FileBrowserComponent::saveMode|
                                                        FileBrowserComponent::canSelectFiles,
                                                        initialDir,
                                                        &filter,
                                                        nullptr);

                FileChooserDialogBox box (title, String::empty,
                                          browserComponent, warnAboutOverwrite,
                                          CabbageUtilities::getDarkerBackgroundSkin());

                if(look!=nullptr)
                    box.setLookAndFeel(look);

                box.setAlwaysOnTop(true);

                if (box.show())
                {
                    for (int i = 0; i < browserComponent.getNumSelectedFiles(); ++i)
                        results.add (browserComponent.getSelectedFile (i));
                }
            }
            else
            {
                FileChooser saveFC(String("Save as..."), File::nonexistent, String(""), true);
                if(saveFC.browseForFileToSave(true))
                    results = saveFC.getResults();
            }
        }

        //browse for directory
        if(mode==2)
        {
            if(useNative==false)
            {
                FileBrowserComponent browserComponent ( FileBrowserComponent::openMode,
                                                        initialDir,
                                                        &filter,
                                                        nullptr);

                FileChooserDialogBox box (title, String::empty,
                                          browserComponent, warnAboutOverwrite,
                                          CabbageUtilities::getDarkerBackgroundSkin());

                if(look!=nullptr)
                    box.setLookAndFeel(look);

                box.setAlwaysOnTop(true);

                if (box.show())
                {
                    for (int i = 0; i < browserComponent.getNumSelectedFiles(); ++i)
                        results.add (browserComponent.getSelectedFile (i));
                }
            }
            else
            {
                FileChooser fc(title, File::nonexistent, String(""), true);
                if(fc.browseForDirectory())
                    results.add(fc.getResult());
            }
        }
#endif
        return results;
    }
//==========================================================================================
    static String cabbageString (String input, Font font, float availableWidth)
    {
        //This method returns a truncated string if it is too big to fit into its available
        //space.  Dots are added at the end to signify that it has been truncated.

        String newStr;
        float stringWidth = font.getStringWidthFloat (input);
        int numChars = input.length();
        float charWidth = stringWidth / numChars;

        if (stringWidth > availableWidth)
        {
            int numCharsToInclude = ((availableWidth / charWidth)+0.5) - 2;
            newStr = input.substring (0, numCharsToInclude);
            newStr << "..";
            return newStr;
        }
        else
            return input;
    }


    static int getNchnlsFromFile(String csdText)
    {
        StringArray array;
        array.addLines(csdText);
        //CabbageUtilities::showMessage(file.loadFileAsString());

        for(int i=0; i<array.size(); i++)
        {
            //CabbageUtilities::debug(array.joinIntoString(" "));
            if(array[i].contains("nchnls") && array[i].contains("="))
            {
                String channels = array[i].substring(array[i].indexOf("=")+1, (array[i].contains(";") ? array[i].indexOf(";") : 100));
                return channels.trim().getIntValue();
            }
        }
        return 2;
    }

    static float getKrFromFile(String csdFile, float sr)
    {
        StringArray array;
        array.addLines(File(csdFile).loadFileAsString());

        for(int i=0; i<array.size(); i++)
        {
            if(array[i].contains("kr") && array[i].contains("="))
            {
                String kr = array[i].substring(array[i].indexOf("=")+1, (array[i].contains(";") ? array[i].indexOf(";") : 100));
                return kr.trim().getIntValue();
            }
        }

        for(int i=0; i<array.size(); i++)
        {
            if(array[i].contains("ksmps") && array[i].contains("="))
            {
                String ksmps = array[i].substring(array[i].indexOf("=")+1, (array[i].contains(";") ? array[i].indexOf(";") : 100));
                return sr/(ksmps.trim().getDoubleValue());
            }
        }

        return sr/64.f;
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

//========= Normal font for components ===============================================
    static Font getComponentFont(int style=1)
    {
        Font font = Font(11.5, style);// ("Verdana", 11.5, bold);
        return font;
    }

//======= For slider values etc ======================================================
    static Font getValueFont()
    {
        Font font = Font ("Helvetica", 11.5, 1);
        return font;
    }
//======= get minMax numerical value from stringarray ================================
    static Range<float> getMinMax(StringArray array)
    {
        if(array.size()==0)
            jassert(1);
        float min = array[0].getFloatValue();
        float max = array[0].getFloatValue();
        for(int i=1; i<array.size(); i++)
        {
            if(array[i].getFloatValue()>max)
                max = array[i].getFloatValue();
            if(array[i].getFloatValue()<min)
                min = array[i].getFloatValue();
        }

        return Range<float>(min, max);

    }
//======= For spectrograms and tables etc ============================================
    static Font getSmallerValueFont()
    {
        Font font = Font ("Helvetica", 10, 0);
        return font;
    }

//======= For titles etc ============================================================
    static Font getTitleFont()
    {
        Font font = Font ("Helvetica", 12.5, 1);
        return font;
    }

//======= Title font colour =========================================================
    static Colour getTitleFontColour()
    {
        Colour cl = Colour::fromRGBA (160, 160, 160, 255);
        return cl;
    }

//====== Font Colour ================================================================
    static Colour getComponentFontColour()
    {
        Colour cl = Colour::fromRGBA (160, 160, 160, 255);
        return cl;
    }

//====== for group components etc =================================================
    static Colour getComponentSkin()
    {
        //Colour skin = Colour::fromRGBA (45, 55, 60, 255);
        Colour skin = Colour::fromRGBA (75, 85, 90, 100); //some transparency
        return skin;
    }

//====== border colour ============================================================
    static const Colour getBorderColour()
    {
        return getComponentFontColour().withMultipliedAlpha(0.2);
    }

//====== border width ============================================================
    static const float getBorderWidth()
    {
        return 1.0f;
    }

//======== for the main background =========================================================
    static Colour getBackgroundSkin()
    {
        Colour skin = Colour::fromRGB(5, 15, 20);
        return skin;
    }

//======= method for retrieve the string values of rectangles..
    static	String getBoundsString(juce::Rectangle<int> currentBounds)
    {
        String bounds = "bounds(" + String(currentBounds.getX()) + String(", ") + String(currentBounds.getY()) + String(", ") + String(currentBounds.getWidth()) + String(", ")
                        + String(currentBounds.getHeight()) + String(")");
        debug(bounds);
        return bounds;

    }


//======= method for replacing the contents of an identifier with new values..
    static String replaceIdentifier(String line, String identifier, String updatedIdentifier)
    {
        if(identifier.length()<2)
            return line;
        if(updatedIdentifier.length()<2)
            return line;

        int startPos = line.indexOf(identifier);
        if(startPos==-1)
            return "";

        String firstSection = line.substring(0, line.indexOf(identifier));
        line = line.substring(line.indexOf(identifier));
        String secondSection = line.substring(line.indexOf(")")+1);

        return firstSection+updatedIdentifier+secondSection;
    }

//======= for darker backgrounds ==========================================================
    static Colour getDarkerBackgroundSkin()
    {
        Colour skin = getBackgroundSkin().darker(0.4);
        return skin;
    }

//==========================================================================================
    void cabbageSleep(unsigned int mseconds)
    {
        clock_t goal = mseconds + clock();
        while (goal > clock());
    }

//==========================================================================================
    Array<float> getAmpRangeArray(Array<float> ranges, int tableNumber)
    {
        Array<float> ampRange;

        for(int i=2; i<ranges.size(); i+=4)
            if(ranges[i]==tableNumber || ranges[i]==-1)
            {
                for(int y = i-2; y<=i+1; y++)
                {
                    ampRange.add(ranges[y]);
                }
            }

        return ampRange;

    }

//======== Check if NaN ====================================================================
    static bool isNumber(double x)
    {
        // This looks like it should always be true,
        // but it's false if x is a NaN.
        return (x == x);
    }

//==========================================================================================
    static void addCustomPlantsToMenu (PopupMenu& m, Array<File> &plantFiles, String userDir)
    {
        int menuSize = m.getNumItems();

        PopupMenu menu;
        PopupMenu subMenu;
        int fileCnt=0;

        if(File(userDir).exists())
        {
            FileSearchPath filePaths(userDir);
            //CabbageUtilities::showMessage(appProperties->getUserSettings()->getValue("CabbageFilePaths"))

            //add all files in root of specifed directories
            for(int i=0; i<filePaths.getNumPaths(); i++)
            {
                File plantDir(filePaths[i]);
                plantDir.findChildFiles(plantFiles, File::findFiles, false, "*.plant");

            }

            for (int i = 0; i < plantFiles.size(); ++i)
                m.addItem (i+100, plantFiles[i].getFileNameWithoutExtension());


            //fileCnt = cabbageFiles.size();

            //increment menu size and serach recursively through all subfolders in specified dirs
            for(int i=0; i<filePaths.getNumPaths(); i++)
            {
                Array<File> subFolders;
                File searchDir(filePaths[i]);
                subFolders.add(searchDir);
                searchDir.findChildFiles(subFolders, File::findDirectories, true);

                //remove parent dirs from array
                for(int p=0; p<filePaths.getNumPaths(); p++)
                    subFolders.removeAllInstancesOf(filePaths[p]);

                PopupMenu subMenu;
                for (int subs = 0; subs < subFolders.size(); subs++)
                {
                    fileCnt = plantFiles.size();
                    subFolders[subs].findChildFiles(plantFiles, File::findFiles, false, "*.plants");
                    subMenu.clear();

                    for (int fileIndex=fileCnt+1; fileIndex < plantFiles.size(); fileIndex++)
                        subMenu.addItem (fileIndex+100, plantFiles[fileIndex].getFileNameWithoutExtension());


                    m.addSubMenu(subFolders[subs].getFileNameWithoutExtension(), subMenu);
                }

                subMenu.clear();
            }


            m.addSeparator();
            //m.setLookAndFeel(&this->getLookAndFeel());
        }
    }


    static void addFilesToPopupMenu(PopupMenu &m, Array<File> &filesArray, String dir, String ext, int indexOffset)
    {
        File searchDir(dir);
        Array<File> subFolders;
        subFolders.add(searchDir);
        int noOfFiles=0, fileCnt;
        searchDir.findChildFiles(subFolders, File::findDirectories, true);
        subFolders.sort();
        String pathSlash;
#if defined(LINUX) || defined(MACOSX)
        pathSlash = "/";
        dir = dir+ "/";
#else
        pathSlash = "\\";
        dir = dir+ "\\";
#endif

        PopupMenu subMenu;
//grab all file in sub-folders
        for (int i = 1; i < subFolders.size(); i++)
        {
            if(!subFolders[i].containsSubDirectories())
            {
                subFolders[i].findChildFiles(filesArray, File::findFiles, false, ext);
                filesArray.sort();

                subMenu.clear();
                for (fileCnt = noOfFiles; fileCnt < filesArray.size(); fileCnt++)
                {
                    subMenu.addItem (fileCnt + indexOffset, filesArray[fileCnt].getFileNameWithoutExtension());
                }
                noOfFiles = fileCnt;
                if(noOfFiles>0)
                    m.addSubMenu(subFolders[i].getFullPathName().replace(dir, "").replace(pathSlash, "-"), subMenu);
            }
        }
        subMenu.clear();
    }

//======================================================================================
    static String setDecimalPlaces(const double& x, const int& numDecimals)
    {
        int y=x;
        double z=x-y;
        double m=pow((double)10.f, (double)numDecimals);
        double q=z*m;
        double r=roundToIntAccurate(q);

        return String(static_cast<double>(y)+(1.0/m)*r);
    }


//======================================================================================
	static void writeValueTreeToFile(ValueTree valueTree, String filePath)
	{
		ScopedPointer<XmlElement> data (valueTree.createXml());
		// only works when there are no objects in the array...
		//write new xml settings files based on data from user settings file, but using ValueTree
		data->writeToFile(File(filePath), String::empty);
	}

//======================================================================================
    static int getPreference(ApplicationProperties* appPrefs, String pref)
    {
        return appPrefs->getUserSettings()->getValue(pref, var(0)).getFloatValue();
    }

    static String getPreference(ApplicationProperties* appPrefs, String pref, String value)
    {
        return appPrefs->getUserSettings()->getValue(pref, "");
    }

//================================================================================
    static void toggleOnOffPreference(ApplicationProperties* appPrefs, String pref)
    {
        if(getPreference(appPrefs, pref)==0)
            appPrefs->getUserSettings()->setValue(pref, var(1));
        else
            appPrefs->getUserSettings()->setValue(pref, var(0));
    }
//====================================================================================================
    static void setPreference(ApplicationProperties* appPrefs, String pref, int value)
    {
        appPrefs->getUserSettings()->setValue(pref, var(value));
    }
//====================================================================================================
    static bool hasCabbageTags(File inputFile)
    {
		const String csdFile = inputFile.loadFileAsString();
		if(csdFile.contains("<Cabbage>") && csdFile.contains("</Cabbage>"))
			return true;
		else 
			return false;
    }
//====================================================================================================	
    static Image drawSVGImageFromFilePath(String path, String type, AffineTransform affine)
    {
        String svgFileName = File(path).existsAsFile()? path : path+"/"+String(type)+".svg";
        Image svgImg;
        File svgFile(svgFileName);
        ScopedPointer<XmlElement> svg (XmlDocument::parse(svgFile.loadFileAsString()));

        if (svgFile.exists())
        {
            int width, height;
            for(int i=0; i<svg->getNumAttributes(); i++)
            {
                if(svg->getAttributeName(i)=="width")
                    width = svg->getAttributeValue(i).getIntValue();
                else if(svg->getAttributeName(i)=="height")
                    height = svg->getAttributeValue(i).getIntValue();
            }


            if(type.contains("button"))
                svgImg = Image(Image::ARGB, svgButtonWidth, svgButtonHeight, true);

            else if(type.contains("rslider"))
                svgImg = Image(Image::ARGB, svgRSliderDiameter, svgRSliderDiameter, true);

            else if(type=="hslider_background")
                svgImg = Image(Image::ARGB, svgHSliderWidth, svgHSliderHeight, true);

            else if(type=="hslider")
                svgImg = Image(Image::ARGB, svgHSliderThumb, svgHSliderThumb, true);

            else if(type=="vslider")
                svgImg = Image(Image::ARGB, svgVSliderThumb, svgVSliderThumb, true);

            else if(type=="vslider_background")
                svgImg = Image(Image::ARGB, svgVSliderWidth, svgVSliderHeight, true);

            else if(type.contains("groupbox"))
                svgImg = Image(Image::ARGB, svgGroupboxWidth, svgGroupboxHeight, true);


            if(svg == nullptr)
                Logger::writeToLog("couldn't parse svg, might not exist");

            ScopedPointer<Drawable> drawable;

            Graphics graph(svgImg);
            if (svg != nullptr)
            {
                drawable = Drawable::createFromSVG (*svg);
                drawable->draw(graph, 1.f, affine);
                return svgImg;
            }
        }

        return Image::null;
    }

//====================================================================================
    bool isPluginCsd(File csdfile)
    {
#if defined(MACOSX)
        {
            File binary(csdfile.withFileExtension("").getFullPathName());
            if(binary.existsAsFile())
                return true;
        }
        {
            File binary(csdfile.getFullPathName()+"/Contents/MacOS/"+csdfile.getFileNameWithoutExtension());
            if(binary.existsAsFile())
              return true;
        }
        {
            File binary(csdfile.getParentDirectory().getFullPathName()+"/MacOS/"+csdfile.getFileNameWithoutExtension());
            if(binary.existsAsFile())
                return true;
        }
        
        return false;
#elif defined(WIN32)
        if(File(csdfile.withFileExtension("dll")).existsAsFile())
            return true;
        else
            return false;
#else
        if(File(csdfile.withFileExtension("so")).existsAsFile())
            return true;
        else
            return false;        
#endif
    }
//========= Text button image ========================================================

//====================================================================================================
    static void setPreference(ApplicationProperties* appPrefs, String pref, String value)
    {
        appPrefs->getUserSettings()->setValue(pref, var(value));
    }
//====================================================================================================
    static void drawSphericalThumb (Graphics& g, const float x, const float y,
                                    const float w, const float h, const Colour& colour,
                                    const float outlineThickness)
    {
        ColourGradient cg = ColourGradient (Colours::white, 0, 0, colour, w/2, h/2, false);
        cg.addColour (0.4, Colours::white.overlaidWith (colour));
        g.setGradientFill (cg);
        g.fillEllipse (x, y, w, h);
        g.setOpacity(.4);
        g.fillEllipse (x+1, y+1, w, h);
    }
//====================================================================================================
    static void drawGlassPointer (Graphics& g, float x, float y, float diameter,
                                  const Colour& colour, float outlineThickness, int direction)
    {
        if (diameter <= outlineThickness)
            return;

        Path p;

        p.startNewSubPath (x + diameter * 0.5f, y);
        p.lineTo (x + diameter*.9f, y + diameter * 0.6f);
        //    p.lineTo (x + diameter, y + diameter);
        p.lineTo (diameter*.1f+x, y + diameter*0.6f);
        //    p.lineTo (x, y + diameter * 0.6f);
        p.closeSubPath();

        p.applyTransform(AffineTransform::rotation (direction * (float_Pi * 0.5f), x + diameter * 0.5f, y + diameter * 0.5f));

        {
            ColourGradient cg (Colours::white.overlaidWith (colour.withMultipliedAlpha (0.7f)), 0, y,
                               Colours::white.overlaidWith (colour.withMultipliedAlpha (0.3f)), 0, y + diameter, false);

            cg.addColour (0.4, Colours::white.overlaidWith (colour));

            g.setGradientFill (cg);
            g.fillPath (p);
        }

        ColourGradient cg (Colours::transparentBlack,
                           x + diameter * 0.5f, y + diameter * 0.5f,
                           Colours::black.withAlpha (0.5f * outlineThickness * colour.getFloatAlpha()),
                           x - diameter * 0.2f, y + diameter * 0.5f, true);

        cg.addColour (0.5, Colours::transparentBlack);
        cg.addColour (0.7, Colours::black.withAlpha (0.07f * outlineThickness));

        g.setGradientFill (cg);
        g.fillPath (p);

        g.setColour (Colours::black.withAlpha (0.5f * colour.getFloatAlpha()));
        g.strokePath (p, PathStrokeType (outlineThickness));
    }
//=======================================================================================
//draw buttons for transport controls
    static Image drawSoundfilerButton(String type, String colour)
    {

        Image img = Image(Image::ARGB, 100, 100, true);
        Graphics g (img);
        g.fillAll(Colour(50, 50, 50));
        if (type == "play_normal")
        {
            Path p;
            p.addTriangle(img.getWidth()*0.3f, img.getHeight()*0.2f, img.getWidth()*0.7f, img.getHeight()*0.5f,
                          img.getWidth()*0.3f, img.getHeight()*0.8f);
            g.setColour(Colour::fromString(colour));
            g.fillPath(p);

            return img;
        }
        else if (type == "play_hover")
        {
            Path p;
            p.addTriangle(img.getWidth()*0.3f, img.getHeight()*0.2f, img.getWidth()*0.7f, img.getHeight()*0.5f,
                          img.getWidth()*0.3f, img.getHeight()*0.8f);
            g.setColour(Colour::fromString(colour));
            g.fillPath(p);

            ColourGradient shadow = ColourGradient (Colour::fromRGBA(50, 50, 50, 50), 0, 0,
                                                    Colour::fromRGBA(10, 10, 10, 100), img.getWidth(), img.getHeight(), false);
            g.setGradientFill(shadow);
            g.drawRoundedRectangle(0.5f, 0.5f, img.getWidth()-2.5f, img.getHeight()-2.5f, jmin(img.getWidth()/20.0f,
                                   img.getHeight()/20.0f), 2.5f);

            return img;
        }
        else if (type == "play_down")
        {
            //g.setColour(CabbageUtilities::getDarkerBackgroundSkin().darker(0.9f));
            g.setColour(Colours::black.withAlpha(0.4f));
            g.fillRoundedRectangle(0, 0, img.getWidth(), img.getHeight(), jmin(img.getWidth()/20.0f,
                                   img.getHeight()/20.0f));

            g.setColour(Colour::fromString(colour).withBrightness(5.0f));
            g.fillRoundedRectangle(img.getWidth()*0.3f, img.getHeight()*0.2f,
                                   img.getWidth()*0.2f, img.getHeight()*0.6f, img.getWidth()/20.0f);
            g.fillRoundedRectangle(img.getWidth()*0.6f, img.getHeight()*0.2f,
                                   img.getWidth()*0.2f, img.getHeight()*0.6f, img.getWidth()/20.0f);

            //3D outline
            ColourGradient shadow = ColourGradient (Colours::black, 0, 0,
                                                    Colour::fromRGBA(50, 50, 50, 50), img.getWidth(), img.getHeight(), false);
            g.setGradientFill(shadow);
            g.drawRoundedRectangle(1.5f, 1.5f, img.getWidth()-2.0f, img.getHeight()-2.0f, jmin(img.getWidth()/20.0f,
                                   img.getHeight()/20.0f), 2.0f);

            return img;
        }
        else if (type == "skip_end_normal")
        {
            Path p;
            p.addTriangle(img.getWidth()*0.2f, img.getHeight()*0.2f, img.getWidth()*0.6f, img.getHeight()*0.5f,
                          img.getWidth()*0.2f, img.getHeight()*0.8f);
            g.setColour(CabbageUtilities::getComponentFontColour());
            g.fillPath(p);
            g.fillRoundedRectangle(img.getWidth()*0.7f, img.getHeight()*0.2f,
                                   img.getWidth()*0.1f, img.getHeight()*0.6f, img.getWidth()/20.0f);
            return img;
        }
        else if (type == "skip_end_hover")
        {
            Path p;
            p.addTriangle(img.getWidth()*0.2f, img.getHeight()*0.2f, img.getWidth()*0.6f, img.getHeight()*0.5f,
                          img.getWidth()*0.2f, img.getHeight()*0.8f);
            g.setColour(CabbageUtilities::getComponentFontColour());
            g.fillPath(p);
            g.fillRoundedRectangle(img.getWidth()*0.7f, img.getHeight()*0.2f,
                                   img.getWidth()*0.1f, img.getHeight()*0.6f, img.getWidth()/20.0f);

            //3D outline
            ColourGradient shadow = ColourGradient (Colour::fromRGBA(50, 50, 50, 50), 0, 0,
                                                    Colour::fromRGBA(10, 10, 10, 100), img.getWidth(), img.getHeight(), false);
            g.setGradientFill(shadow);
            g.drawRoundedRectangle(0.5f, 0.5f, img.getWidth()-2.5f, img.getHeight()-2.5f, jmin(img.getWidth()/20.0f,
                                   img.getHeight()/20.0f), 2.5f);

            return img;
        }
        else if (type == "skip_end_down")
        {
            //g.setColour(CabbageUtilities::getDarkerBackgroundSkin().darker(0.9f));
            g.setColour(Colours::black.withAlpha(0.4f));
            g.fillRoundedRectangle(0, 0, img.getWidth(), img.getHeight(), jmin(img.getWidth()/20.0f,
                                   img.getHeight()/20.0f));

            Path p;
            p.addTriangle(img.getWidth()*0.2f, img.getHeight()*0.2f, img.getWidth()*0.6f, img.getHeight()*0.5f,
                          img.getWidth()*0.2f, img.getHeight()*0.8f);
            g.setColour(Colours::cornflowerblue.withBrightness(5.0f));
            g.fillPath(p);
            g.fillRoundedRectangle(img.getWidth()*0.7f, img.getHeight()*0.2f,
                                   img.getWidth()*0.1f, img.getHeight()*0.6f, img.getWidth()/20.0f);

            //3D outline
            ColourGradient shadow = ColourGradient (Colours::black, 0, 0,
                                                    Colour::fromRGBA(50, 50, 50, 50), img.getWidth(), img.getHeight(), false);
            g.setGradientFill(shadow);
            g.drawRoundedRectangle(1.5f, 1.5f, img.getWidth()-2.0f, img.getHeight()-2.0f, jmin(img.getWidth()/20.0f,
                                   img.getHeight()/20.0f), 2.0f);

            return img;
        }
        else if (type == "skip_start_normal")
        {
            Path p;
            p.addTriangle(img.getWidth()*0.4f, img.getHeight()*0.5f, img.getWidth()*0.8f, img.getHeight()*0.2f,
                          img.getWidth()*0.8f, img.getHeight()*0.8f);
            g.setColour(Colour::fromString(colour));
            g.fillPath(p);
            g.fillRoundedRectangle(img.getWidth()*0.2f, img.getHeight()*0.2f,
                                   img.getWidth()*0.1f, img.getHeight()*0.6f, img.getWidth()/20.0f);
            return img;
        }
        else if (type == "skip_start_hover")
        {
            Path p;
            p.addTriangle(img.getWidth()*0.4f, img.getHeight()*0.5f, img.getWidth()*0.8f, img.getHeight()*0.2f,
                          img.getWidth()*0.8f, img.getHeight()*0.8f);
            g.setColour(Colour::fromString(colour).withBrightness(.5));
            g.fillPath(p);
            g.fillRoundedRectangle(img.getWidth()*0.2f, img.getHeight()*0.2f,
                                   img.getWidth()*0.1f, img.getHeight()*0.6f, img.getWidth()/20.0f);

            //3D outline
            ColourGradient shadow = ColourGradient (Colour::fromRGBA(50, 50, 50, 50), 0, 0,
                                                    Colour::fromRGBA(10, 10, 10, 100), img.getWidth(), img.getHeight(), false);
            g.setGradientFill(shadow);
            g.drawRoundedRectangle(0.5f, 0.5f, img.getWidth()-2.5f, img.getHeight()-2.5f, jmin(img.getWidth()/20.0f,
                                   img.getHeight()/20.0f), 2.5f);
            return img;
        }
        else if (type == "skip_start_down")
        {
            //g.setColour(CabbageUtilities::getDarkerBackgroundSkin().darker(0.9f));
            g.setColour(Colours::black.withAlpha(0.4f));
            g.fillRoundedRectangle(0, 0, img.getWidth(), img.getHeight(), jmin(img.getWidth()/20.0f,
                                   img.getHeight()/20.0f));

            Path p;
            p.addTriangle(img.getWidth()*0.4f, img.getHeight()*0.5f, img.getWidth()*0.8f, img.getHeight()*0.2f,
                          img.getWidth()*0.8f, img.getHeight()*0.8f);
            g.setColour(Colour::fromString(colour).withBrightness(5.0f));
            g.fillPath(p);
            g.fillRoundedRectangle(img.getWidth()*0.2f, img.getHeight()*0.2f,
                                   img.getWidth()*0.1f, img.getHeight()*0.6f, img.getWidth()/20.0f);

            //3D outline
            ColourGradient shadow = ColourGradient (Colours::black, 0, 0,
                                                    Colour::fromRGBA(50, 50, 50, 50), img.getWidth(), img.getHeight(), false);
            g.setGradientFill(shadow);
            g.drawRoundedRectangle(1.5f, 1.5f, img.getWidth()-2.0f, img.getHeight()-2.0f, jmin(img.getWidth()/20.0f,
                                   img.getHeight()/20.0f), 2.0f);

            return img;
        }

        else return img;
    }
//====================================================================================================

    static const DrawablePath* createPlayButtonPath(int buttonSize, Colour col)
    {
        Path playPath;
        playPath.addTriangle(0, 0, buttonSize, buttonSize/2, 0, buttonSize);
        DrawablePath* playImage = new DrawablePath();
        playImage->setFill(col);
        playImage->setPath(playPath);
        return playImage;
    }

    static const DrawablePath* createPauseButtonPath(int buttonSize)
    {
        Path pausePath;
        pausePath.addRectangle(0, 0, buttonSize*.4, buttonSize);
        pausePath.addRectangle(buttonSize*.5, 0, buttonSize*.4, buttonSize);
        DrawablePath* pauseImage = new DrawablePath();;
        pauseImage->setPath(pausePath);
        return pauseImage;
    }

    static const DrawablePath* createZoomInButtonPath(int buttonSize)
    {
        Path zoomInPath;
        //zoomInPath.addEllipse(-100, -100, buttonSize+100, buttonSize+100);
        zoomInPath.addRectangle(0, buttonSize/3, buttonSize, buttonSize/3);
        zoomInPath.addRectangle(buttonSize/3, 0, buttonSize/3, buttonSize);
        DrawablePath* zoomImage = new DrawablePath();
        zoomImage->setFill(Colours::green.darker(.9f));
        //zoomImage->setStrokeFill(Colours::green.darker(.9f));
        //zoomImage->setStrokeThickness(4);
        zoomImage->setPath(zoomInPath);
        return zoomImage;
    }

    static const DrawablePath* createZoomOutButtonPath(int buttonSize)
    {
        Path zoomInPath;
        //zoomInPath.addEllipse(-100, -100, buttonSize+100, buttonSize+100);
        zoomInPath.addRectangle(0, buttonSize/3, buttonSize, buttonSize/3);
        DrawablePath* zoomImage = new DrawablePath();
        zoomImage->setFill(Colours::green.darker(.9f));
        //zoomImage->setStrokeFill(Colours::green.darker(.9f));
        //zoomImage->setStrokeThickness(4);
        zoomImage->setPath(zoomInPath);
        return zoomImage;
    }

    static const DrawablePath* createLoopButtonPath()
    {
        Path path;
        path.startNewSubPath(60, 0);
        path.lineTo(100, 0);
        path.closeSubPath();
        path.startNewSubPath(100, 0);
        path.lineTo(100, 80);
        path.closeSubPath();
        path.startNewSubPath(100, 80);
        path.lineTo(120, 80);
        path.lineTo(100, 100);
        path.lineTo(80, 80);
        path.closeSubPath();

        path.startNewSubPath(60, 100);
        path.lineTo(20, 100);
        path.closeSubPath();
        path.startNewSubPath(20, 100);
        path.lineTo(20, 20);
        path.closeSubPath();
        path.startNewSubPath(20, 20);
        path.lineTo(0, 20);
        path.lineTo(20, 0);
        path.lineTo(40, 20);
        path.closeSubPath();
        AffineTransform transform(AffineTransform::identity);
        path.applyTransform(transform.rotated(float_Pi/2.f));
        DrawablePath* image = new DrawablePath();
        image->setFill(Colours::white);
        image->setStrokeThickness(25.f);
        image->setStrokeFill(Colours::green.darker(.9f));
        image->setPath(path);
        return image;
    }

    static const DrawablePath* createStopButtonPath(int buttonSize, Colour col)
    {
        Path stopPath;
        stopPath.addRectangle(0, 0, buttonSize, buttonSize);
        DrawablePath* stopImage = new DrawablePath();
        stopImage->setPath(stopPath);
        stopImage->setFill(col);
        return stopImage;
    }

    static const DrawablePath* createRecordButtonPath(int buttonSize, Colour col)
    {
        Path stopPath;
        stopPath.addEllipse(0, 0, buttonSize, buttonSize);
        DrawablePath* stopImage = new DrawablePath();
        stopImage->setPath(stopPath);
        stopImage->setFill(col);
        return stopImage;
    }

    static const DrawablePath* createDisabledStopButtonPath(float buttonSize, Colour col1, Colour col2)
    {
        Path stopPath;
        stopPath.addRectangle(0, 0, buttonSize, buttonSize);
        stopPath.addLineSegment(Line<float>(0.f, 0.f, buttonSize, buttonSize), 1.f);
        stopPath.addLineSegment(Line<float>(buttonSize, 0.f, 0.f, buttonSize), 1.f);
        DrawablePath* stopImage = new DrawablePath();
        stopImage->setFill(Colours::red);
        stopImage->setStrokeFill(col2);
        stopImage->setPath(stopPath);
        return stopImage;
    }

    static const DrawablePath* createEnvelopeButtonPath(Colour col)
    {
        Path path;
        path.addEllipse(-10, 70, 25, 25);
        path.startNewSubPath(10, 70);
        path.lineTo(45, 10);
        path.closeSubPath();
        path.addEllipse(35, 25, 25, 25);
        path.startNewSubPath(40, 10);
        path.lineTo(70, 80);
        path.closeSubPath();
        path.addEllipse(65, 75, 25, 25);
        path.startNewSubPath(70, 80);
        path.lineTo(120, 10);
        path.closeSubPath();
        path.addEllipse(115, 5, 25, 25);

        DrawablePath* image = new DrawablePath();
        image->setFill(Colours::white);
        image->setStrokeThickness(25.f);
        image->setStrokeFill(col);
        image->setPath(path);
        return image;
    }

    static const DrawablePath* createOpenButtonPath(int buttonSize)
    {
        Path openPath;
        openPath.startNewSubPath(4, 2);
        openPath.lineTo(18, 2);
        openPath.lineTo(18, 5);
        openPath.lineTo(22, 5);
        openPath.lineTo(22, buttonSize-5);
        openPath.lineTo(4, buttonSize-5);
        openPath.lineTo(4, 2);
        //openPath.startNewSubPath(8, 7);
        //openPath.lineTo(24, 7);
        openPath.closeSubPath();
        DrawablePath* openImage = new DrawablePath();;
        openImage->setPath(openPath);
        openImage->setFill(Colours::white);
        openImage->setStrokeFill(Colours::green.darker(.9f));
        openImage->setStrokeThickness(4);
        return openImage;
    }

    static void drawBypassIcon(Graphics& g, Rectangle<float> rect, bool isActive)
    {
        const float x = rect.getX();
        const float y = rect.getY();
        const float w = rect.getWidth()-5.f;
        const float h = rect.getHeight();
        const float d = 5;
        g.setColour(isActive ? Colours::cornflowerblue : Colours::lime);
        Path p;
        p.startNewSubPath(x+5, y+h/2.f+d/2.f);
        g.drawEllipse(x, y+h/2.f, d, d, 1);
        g.drawEllipse(x+w, y+h/2.f, d, d, 1.f);

        if(!isActive)
        {
            p.lineTo(x+w, y+h/2.f+d/2.f);
        }
        else
        {
            p.addArc(x+w, y+h/2.f+d/2.f, 5, 5, 3.14, 3.14);
        }

        p.closeSubPath();
        g.strokePath(p, PathStrokeType(1));
    }
//====================================================================================================
    static String returnFullPathForFile(String file, String fullPath)
    {
        String filePath;
        if(file.isNotEmpty())
        {
            if(file==".")
                file="";

#ifdef MACOSX
#ifndef Cabbage_Build_Standalone
            filePath.append(String("/Contents/")+file, 1024);
#else
            filePath = fullPath+String("//")+file;
#endif
#endif
#ifdef LINUX
            filePath = fullPath+String("/")+file;;
#endif
#ifdef WIN32
            filePath = fullPath+String("\\")+file;;
#endif
#ifdef AndroidBuild
            String homeDir = String(getenv("EXTERNAL_STORAGE"))+String("/Cabbage/");
            filePath = homeDir+String("/")+file;;
#endif
            return filePath;
        }
        else return "";
    }

    //===========================================================================================

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
    RoundButton(String _type, Colour _colour):Component(), visible("bacground")
    {
        setName(_type);
        type = _type;
        colour = _colour;
        mode = 0;
    }
    ~RoundButton() {}

    void mouseDown(const MouseEvent& e)
    {
        //Logger::writeToLog("Mouse down on round button:"+String(type));
        sendChangeMessage();
        mode = (mode==1 ? 0 : mode+1);
    }

    void paint(Graphics& g)
    {
        //Logger::writeToLog(type);
        if(type.contains("zoom"))
        {
            g.fillAll(Colours::transparentBlack);
            g.setColour(CabbageUtilities::getBackgroundSkin());
            g.fillEllipse(0, 0, getWidth(), getHeight());
            g.setColour(Colours::white.withAlpha(.8f));
            g.fillEllipse(1, 1, getWidth()-2, getHeight()-2);
            g.setColour(Colours::black);
            g.fillRoundedRectangle(getWidth()*.18, getHeight()*.4f, getWidth()*.65, getHeight()*.25, 2);
            if(getName()=="zoomIn")
                g.fillRoundedRectangle(getWidth()*.38f, getHeight()*.20, getWidth()*.25, getHeight()*.65, 2);
        }
        else
        {
            g.fillAll(Colours::transparentBlack);
            g.setColour(CabbageUtilities::getBackgroundSkin());
            g.fillEllipse(0, 0, getWidth(), getHeight());
            g.setColour((visible=="foreground" ? colour : colour.withAlpha(.3f)));
            g.fillEllipse(1, 1, getWidth()-2, getHeight()-2);
            g.setColour(colour.contrasting());
            g.drawFittedText(type, 0, 0, getWidth(), getHeight(), Justification::centred, 1);
            if(visible=="off")
            {
                g.setColour(colour.contrasting());
                g.drawLine(0, 0, getWidth(), getHeight(), 2);
                g.drawLine(0, getHeight(), getWidth(), 0, 2);
            }
        }
    }

    void setVisibilityStatus(String status)
    {
        visible = status;
    }

    int getMode()
    {
        return mode;
    }

    int setMode(int mod)
    {
        mode=mod;
    }
};

class FontsArray : public StringArray
{
public:
    FontsArray()
    {
        add("Andale Mono");
        add("Consolas");
        add("Courier	Courier");
        add("DejaVu Sans Mono");
        add("Droid Sans Mono");
        add("Everson Mono");
        add("Fixedsys");
        add("HyperFont");
        add("Inconsolata");
        add("Letter Gothic");
        add("Lucida Console");
        add("Monaco");
        add("Monofur");
        add("PragmataPro");
        add("Prestige Elite");
        add("ProFont");
        add("Source Code Pro");
        add("Terminus");
        add("Tex Gyre Cursor");
        add("Trixie	Trixie");
        add("UM Typewriter");
    }

    ~FontsArray() {}

};
#endif
