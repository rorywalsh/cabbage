/*
  ==============================================================================

    CabbageFilePropertyComponent.h
    Created: 28 Oct 2016 11:12:58am
    Author:  rory

  ==============================================================================
*/

#ifndef CABBAGEFILEPROPERTYCOMPONENT_H_INCLUDED
#define CABBAGEFILEPROPERTYCOMPONENT_H_INCLUDED


class CabbageFilePropertyComponent  : public PropertyComponent
{
public:
    CabbageFilePropertyComponent (const String& name,
                                  const bool isDirectory,
                                  const bool allowEditingOfFilename,
                                  const String& fileBrowserWildcard = "*",
                                  const String currentFile = "")
        : PropertyComponent (name),
          filenameComp (name, File(), allowEditingOfFilename,
                        isDirectory, false, fileBrowserWildcard,
                        String(), String())
    {
        addAndMakeVisible (filenameComp);
        filenameComp.setCurrentFile (File (currentFile), true, dontSendNotification);
        filenameComp.setTooltip (filenameComp.getCurrentFileText());
        filenameComp.setFilenameIsEditable(true);
    }

    void refresh() {}

    FilenameComponent filenameComp;

private:

};


#endif  // CABBAGEFILEPROPERTYCOMPONENT_H_INCLUDED
