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

#ifndef CABBAGESETTINGS_H_INCLUDED
#define CABBAGESETTINGS_H_INCLUDED


#include "../CabbageIds.h"

class CabbageSettings : public ApplicationProperties, public ValueTree::Listener, public ChangeBroadcaster
{
public:
    CabbageSettings();
    ~CabbageSettings() override
    {
        defaultPropSet = nullptr;
    }

    void set (String child, Identifier identifier, var value);
    String get (String child, String identifier);
    static Colour getColourFromValueTree (ValueTree valueTree, Identifier identifier, Colour defaultColour);
    static String getColourPropertyName (ValueTree valueTree, int index);
    int getIntProperty (String child);
    static Colour getColourFromValueTree (ValueTree valueTree, int index, Colour defaultColour);
    static void set (ValueTree tree, String child, Identifier identifier, var value);
    static String get (ValueTree tree, String child, String identifier);
    void setProperty (String child, var value);
    void updateRecentFilesList();
    void updateRecentFilesList (File file);
    int getIndexOfProperty (String child, String identifier);
    ValueTree getValueTree();
    XmlElement* getXML (String identifier);
    void setDefaultSettings();
    void setDefaultColourSettings();
    std::unique_ptr<PropertySet> defaultPropSet;
    ValueTree valueTree;
    RecentlyOpenedFilesList recentFiles;
    File getMostRecentFile (int index);

    String getAudioSettingsXml()
    {
        return audioSettingsXml;
    }

    static CabbageSettings& instance()
    {
        static CabbageSettings x;
        return x;
    }


private:


    String audioSettingsXml;
    void changed()
    {
		std::unique_ptr<XmlElement> data(valueTree.createXml());
        getUserSettings()->setValue ("PROJECT_DEFAULT_SETTINGS", data.get());
        sendChangeMessage();
        //XmlElement * el = valueTree.createXml();
        //el->writeToFile(File("/home/rory/Desktop/Example1.xml"), String::empty);
        //delete el;
    }

    //==============================================================================
    void valueTreePropertyChanged (ValueTree&, const Identifier&) override;

    void valueTreeChildAdded (ValueTree&, ValueTree&) override
    {
        changed();
    }
    void valueTreeChildRemoved (ValueTree&, ValueTree&, int) override
    {
        changed();
    }
    void valueTreeChildOrderChanged (ValueTree&, int, int) override
    {
        changed();
    }
    void valueTreeParentChanged (ValueTree&) override
    {
        changed();
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CabbageSettings)
};



#endif  // CABBAGESETTINGS_H_INCLUDED
