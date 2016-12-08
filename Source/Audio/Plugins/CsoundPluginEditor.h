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

#ifndef PLUGINEDITOR_H_INCLUDED
#define PLUGINEDITOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "CsoundPluginProcessor.h"


//==============================================================================
/**
*/
class CsoundPluginEditor  : public AudioProcessorEditor
{
public:
    CsoundPluginEditor (CsoundPluginProcessor&);
    ~CsoundPluginEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:

    CsoundPluginProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CsoundPluginEditor)
};


#endif  // PLUGINEDITOR_H_INCLUDED
