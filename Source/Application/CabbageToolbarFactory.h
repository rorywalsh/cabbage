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

#include "../CabbageIds.h"
#include "../Settings/CabbageSettings.h"
#include "../BinaryData/CabbageBinaryData.h"

#ifndef CODEEDITORTOOLBARFACTORY_H_INCLUDED
#define CODEEDITORTOOLBARFACTORY_H_INCLUDED

class CabbageContentComponent;

class CabbageToolbarFactory   : public ToolbarItemFactory
{
public:
    CabbageToolbarFactory (CabbageContentComponent* owner);
    ~CabbageToolbarFactory()
    {
    }

    //==============================================================================
    // Each type of item a toolbar can contain must be given a unique ID. These
    // are the ones we'll use in this demo.
    enum ToolbarItemIds
    {
        doc_new         = 1,
        doc_open        = 2,
        doc_save        = 3,
        doc_saveAs      = 4,
        edit_copy       = 5,
        edit_cut        = 6,
        edit_paste      = 7,
        system_prefs    = 8,
        custom_comboBox  = 9,
        toggle_play     = 10
    };

    void getAllToolbarItemIds (Array<int>& ids) override;
    void getDefaultItemSet (Array<int>& ids) override;
    ToolbarItemComponent* createItem (int itemId) override;

    void togglePlay (bool enabled)
    {
        togglePlayButton->setToggleState (enabled, dontSendNotification);
    }

    ToolbarButton* togglePlayButton;

private:

    ToolbarButton* createButtonFromSVG (const int itemId, const String& text, const String svgFile, const String onFile = "");


    class ToolbarComboBox : public ToolbarItemComponent
    {
    public:
        ToolbarComboBox (const int toolbarItemId)
            : ToolbarItemComponent (toolbarItemId, "Custom Toolbar Item", false),
              comboBox ("toolbar combo box")
        {
            addAndMakeVisible (comboBox);
            comboBox.setEditableText (false);
        }

        bool getToolbarItemSizes (int /*toolbarDepth*/, bool isVertical,
                                  int& preferredSize, int& minSize, int& maxSize) override
        {
            if (isVertical)
                return false;

            preferredSize = 250;
            minSize = 80;
            maxSize = 300;
            return true;
        }

        void paintButtonArea (Graphics&, int, int, bool, bool) override
        {
        }

        void contentAreaChanged (const Rectangle<int>& newArea) override
        {
            comboBox.setSize (newArea.getWidth() - 2,
                              jmin (newArea.getHeight() - 2, 22));

            comboBox.setCentrePosition (newArea.getCentreX(), newArea.getCentreY());
        }

        void addItemsToComboBox (const String text, const int itemId)
        {
            comboBox.addItem (text, itemId);
            comboBox.setSelectedId (1);
        }

        void clearItemsFromComboBox()
        {
            comboBox.clear (dontSendNotification);
        }

        ComboBox& getCombo()
        {
            return comboBox;
        }

    private:
        ComboBox comboBox;
    };

    CabbageContentComponent* owner;

public:
    ToolbarComboBox* combo; //owner by toolbar factory...
};



#endif  // CODEEDITORTOOLBARFACTORY_H_INCLUDED
