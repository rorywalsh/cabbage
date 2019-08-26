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

#ifndef CABBAGECOMBOBOX_H_INCLUDED
#define CABBAGECOMBOBOX_H_INCLUDED

#include "../CabbageCommonHeaders.h"
#include "CabbageWidgetBase.h"

class CabbagePluginEditor;
class CabbageComboBox
    : public ComboBox,
      public ValueTree::Listener,
      public CabbageWidgetBase,
      public ComboBox::Listener
{
    int offX, offY, offWidth, offHeight, pivotx, pivoty, refresh;
    String name, tooltipText, caption, text, filetype, workingDir;
    float rotate;
    CabbagePluginEditor* owner;
    ValueTree widgetData;
    File pluginDir;
    bool isStringCombo = false;
    bool isPresetCombo = false;
    String currentValueAsText = "";
    Array<File> folderFiles;
    StringArray stringItems;
    StringArray presets;

public:

    CabbageComboBox (ValueTree cAttr, CabbagePluginEditor* _owner);
    ~CabbageComboBox();

    void valueTreePropertyChanged (ValueTree& valueTree, const Identifier&)  override;
    void addItemsToCombobox (ValueTree wData);

    void comboBoxChanged (ComboBox* combo)  override; //is preset combo use this listener

    void valueTreeChildAdded (ValueTree&, ValueTree&)override {};
    void valueTreeChildRemoved (ValueTree&, ValueTree&, int) override {}
    void valueTreeChildOrderChanged (ValueTree&, int, int) override {}
    void valueTreeParentChanged (ValueTree&) override {};

    String getTooltip() override
    {
        return tooltipText;
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CabbageComboBox);
};

#endif  // CABBAGECOMBOBOX_H_INCLUDED
