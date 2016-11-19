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
                                  const String& fileBrowserWildcard = "*")
        : PropertyComponent (name),
          filenameComp (name, File(), allowEditingOfFilename,
                        isDirectory, false, fileBrowserWildcard,
                        String(), String())
    {
        addAndMakeVisible (filenameComp);

    }

    //virtual void setFile (const File& newFile) = 0;
    //virtual File getFile() const = 0;

    void refresh()
    {
        int i=0;//filenameComp.setCurrentFile (getFile(), false);
    }

    FilenameComponent filenameComp;

private:

};


#endif  // CABBAGEFILEPROPERTYCOMPONENT_H_INCLUDED
