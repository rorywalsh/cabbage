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

class CabbageMainComponent;

class CabbageToolbarFactory   : public ToolbarItemFactory, public Timer
{
public:
    CabbageToolbarFactory (CabbageMainComponent* owner);
    ~CabbageToolbarFactory() override {}

    //==============================================================================
    // Each type of item a toolbar can contain must be given a unique ID. These
    // are the ones we'll use in this demo.
    enum ToolbarItemIds
    {
        doc_new          = 1,
        doc_open         = 2,
        doc_save         = 3,
        doc_saveAs       = 4,
        edit_copy        = 5,
        edit_cut         = 6,
        edit_paste       = 7,
        system_prefs     = 8,
        custom_comboBox  = 9,
        toggle_play      = 10,
        doc_save_graph   = 11,
        doc_saveAs_graph = 12,
        record_to_disk   = 13,
    };

    void startRecordingTimer(bool start)
    {
        flashing = 1;
        if(start)
            startTimer(300);
        else
        {
            stopTimer();
            toggleRecordButton->setToggleState (true, dontSendNotification);                
        }
    }
    
    void timerCallback() override;
    
    void getAllToolbarItemIds (Array<int>& ids) override;
    void getDefaultItemSet (Array<int>& ids) override;
    ToolbarItemComponent* createItem (int itemId) override;

    void togglePlay (bool enabled)
    {
        togglePlayButton->setToggleState (enabled, dontSendNotification);
    }
    
    void setRecordButtonState (const String state)
    {
        if(state == "off")
            toggleRecordButton->setToggleState (false, dontSendNotification);
        else if(state == "disabled")
            toggleRecordButton->setEnabled(false);
        else if(state == "enabled")
        {
            toggleRecordButton->setEnabled(true);
            toggleRecordButton->setToggleState (false, dontSendNotification);
        }
            
    }

    ToolbarButton* togglePlayButton;
    ToolbarButton* toggleRecordButton;

	void setIconsPath(String path)
	{
		iconsPath = path;
	}

private:
	
	String iconsPath;
    ToolbarButton* createButtonFromSVG (const int itemId, const String& text, const String svgFile, const String onFile = "");
    ToolbarButton* createButtonFromPNG (const int itemId, const String& text, const void* svg, size_t size, const String onFile = "");
    ToolbarButton* createToggleButtonFromPNG (const int itemId, const String& text, const void* png1, size_t size1, const void* png2, size_t size2);
    ToolbarButton* createToggleButtonFromSVG(const int itemId, const String &text, String svgTextOn, String svgTextOff);
    int flashing = 1;

    String svgRecordTextOff = R"xxx(
    <svg width="56" height="16" viewBox="0 0 56 16" fill="none" xmlns="http://www.w3.org/2000/svg">
    <circle cx="7" cy="8" r="7" fill="#D9D9D9"/>
    <path d="M17.2642 15V0.454545H23.0028C24.1013 0.454545 25.0388 0.651041 25.8153 1.04403C26.5966 1.43229 27.1908 1.9839 27.598 2.69886C28.0099 3.40909 28.2159 4.24479 28.2159 5.20597C28.2159 6.17187 28.0076 7.00284 27.5909 7.69886C27.1742 8.39015 26.5705 8.92045 25.7798 9.28977C24.9938 9.65909 24.0421 9.84375 22.9247 9.84375H19.0824V7.37216H22.4276C23.0147 7.37216 23.5024 7.29167 23.8906 7.13068C24.2789 6.9697 24.5677 6.72822 24.7571 6.40625C24.9512 6.08428 25.0483 5.68419 25.0483 5.20597C25.0483 4.72301 24.9512 4.31581 24.7571 3.98438C24.5677 3.65294 24.2765 3.40199 23.8835 3.23153C23.4953 3.05634 23.0052 2.96875 22.4134 2.96875H20.3395V15H17.2642ZM25.1193 8.38068L28.7344 15H25.3395L21.8026 8.38068H25.1193ZM30.3892 15V0.454545H40.1903V2.99006H33.4645V6.45597H39.6861V8.99148H33.4645V12.4645H40.2188V15H30.3892ZM55.4691 5.54688H52.3583C52.3015 5.14441 52.1855 4.78693 52.0103 4.47443C51.8351 4.1572 51.6102 3.88731 51.3356 3.66477C51.061 3.44223 50.7437 3.27178 50.3839 3.15341C50.0288 3.03504 49.6429 2.97585 49.2262 2.97585C48.4734 2.97585 47.8176 3.16288 47.2589 3.53693C46.7002 3.90625 46.2669 4.44602 45.9592 5.15625C45.6514 5.86174 45.4975 6.71875 45.4975 7.72727C45.4975 8.7642 45.6514 9.63542 45.9592 10.3409C46.2717 11.0464 46.7073 11.5791 47.266 11.9389C47.8247 12.2988 48.471 12.4787 49.2049 12.4787C49.6168 12.4787 49.998 12.4242 50.3484 12.3153C50.7035 12.2064 51.0183 12.0478 51.293 11.8395C51.5676 11.6264 51.7949 11.3684 51.9748 11.0653C52.1594 10.7623 52.2873 10.4167 52.3583 10.0284L55.4691 10.0426C55.3886 10.7102 55.1874 11.3542 54.8654 11.9744C54.5482 12.59 54.1197 13.1416 53.5799 13.6293C53.0449 14.1122 52.4057 14.4957 51.6623 14.7798C50.9237 15.0592 50.0879 15.1989 49.1552 15.1989C47.8578 15.1989 46.6978 14.9053 45.6751 14.3182C44.6571 13.7311 43.8522 12.8812 43.2603 11.7685C42.6732 10.6558 42.3796 9.30871 42.3796 7.72727C42.3796 6.1411 42.6779 4.79167 43.2745 3.67898C43.8711 2.56629 44.6808 1.71875 45.7035 1.13636C46.7262 0.549242 47.8768 0.255681 49.1552 0.255681C49.998 0.255681 50.7792 0.374053 51.4989 0.610795C52.2234 0.847538 52.8649 1.19318 53.4237 1.64773C53.9824 2.09754 54.4369 2.64915 54.7873 3.30256C55.1424 3.95597 55.3697 4.70407 55.4691 5.54688Z" fill="#CECECE"/>
    </svg>

    )xxx";

    String svgRecordTextOn =R"xxx(
    <svg width="56" height="16" viewBox="0 0 56 16" fill="none" xmlns="http://www.w3.org/2000/svg">
    <circle cx="7" cy="8" r="7" fill="#FF0000"/>
    <path d="M17.2642 15V0.454545H23.0028C24.1013 0.454545 25.0388 0.651041 25.8153 1.04403C26.5966 1.43229 27.1908 1.9839 27.598 2.69886C28.0099 3.40909 28.2159 4.24479 28.2159 5.20597C28.2159 6.17187 28.0076 7.00284 27.5909 7.69886C27.1742 8.39015 26.5705 8.92045 25.7798 9.28977C24.9938 9.65909 24.0421 9.84375 22.9247 9.84375H19.0824V7.37216H22.4276C23.0147 7.37216 23.5024 7.29167 23.8906 7.13068C24.2789 6.9697 24.5677 6.72822 24.7571 6.40625C24.9512 6.08428 25.0483 5.68419 25.0483 5.20597C25.0483 4.72301 24.9512 4.31581 24.7571 3.98438C24.5677 3.65294 24.2765 3.40199 23.8835 3.23153C23.4953 3.05634 23.0052 2.96875 22.4134 2.96875H20.3395V15H17.2642ZM25.1193 8.38068L28.7344 15H25.3395L21.8026 8.38068H25.1193ZM30.3892 15V0.454545H40.1903V2.99006H33.4645V6.45597H39.6861V8.99148H33.4645V12.4645H40.2188V15H30.3892ZM55.4691 5.54688H52.3583C52.3015 5.14441 52.1855 4.78693 52.0103 4.47443C51.8351 4.1572 51.6102 3.88731 51.3356 3.66477C51.061 3.44223 50.7437 3.27178 50.3839 3.15341C50.0288 3.03504 49.6429 2.97585 49.2262 2.97585C48.4734 2.97585 47.8176 3.16288 47.2589 3.53693C46.7002 3.90625 46.2669 4.44602 45.9592 5.15625C45.6514 5.86174 45.4975 6.71875 45.4975 7.72727C45.4975 8.7642 45.6514 9.63542 45.9592 10.3409C46.2717 11.0464 46.7073 11.5791 47.266 11.9389C47.8247 12.2988 48.471 12.4787 49.2049 12.4787C49.6168 12.4787 49.998 12.4242 50.3484 12.3153C50.7035 12.2064 51.0183 12.0478 51.293 11.8395C51.5676 11.6264 51.7949 11.3684 51.9748 11.0653C52.1594 10.7623 52.2873 10.4167 52.3583 10.0284L55.4691 10.0426C55.3886 10.7102 55.1874 11.3542 54.8654 11.9744C54.5482 12.59 54.1197 13.1416 53.5799 13.6293C53.0449 14.1122 52.4057 14.4957 51.6623 14.7798C50.9237 15.0592 50.0879 15.1989 49.1552 15.1989C47.8578 15.1989 46.6978 14.9053 45.6751 14.3182C44.6571 13.7311 43.8522 12.8812 43.2603 11.7685C42.6732 10.6558 42.3796 9.30871 42.3796 7.72727C42.3796 6.1411 42.6779 4.79167 43.2745 3.67898C43.8711 2.56629 44.6808 1.71875 45.7035 1.13636C46.7262 0.549242 47.8768 0.255681 49.1552 0.255681C49.998 0.255681 50.7792 0.374053 51.4989 0.610795C52.2234 0.847538 52.8649 1.19318 53.4237 1.64773C53.9824 2.09754 54.4369 2.64915 54.7873 3.30256C55.1424 3.95597 55.3697 4.70407 55.4691 5.54688Z" fill="#CB1313"/>
    </svg>
    )xxx";

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

        void contentAreaChanged (const juce::Rectangle<int>& newArea) override
        {
            comboBox.setSize (newArea.getWidth() - 2,
                              jmin (newArea.getHeight() - 2, 22));

            comboBox.setCentrePosition (newArea.getCentreX(), newArea.getCentreY());
        }

        void addItemsToComboBox (const String newText, const int id)
        {
            comboBox.addItem (newText, id);
            comboBox.setSelectedId (1, dontSendNotification);
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

    CabbageMainComponent* owner;

public:
    ToolbarComboBox* combo; //owner by toolbar factory...
};



#endif  // CODEEDITORTOOLBARFACTORY_H_INCLUDED
