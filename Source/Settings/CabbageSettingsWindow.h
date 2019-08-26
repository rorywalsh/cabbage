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

#ifndef CABBAGESETTINGSWINDOW_H_INCLUDED
#define CABBAGESETTINGSWINDOW_H_INCLUDED

#include "../CabbageIds.h"
#include "CabbageSettings.h"
#include "../Utilities/CabbageColourProperty.h"
#include "../Utilities/CabbageFilePropertyComponent.h"
#include "../Utilities/CabbageUtilities.h"
#include "../BinaryData/CabbageBinaryData.h"
#include "../CodeEditor/CsoundTokeniser.h"
#include "../LookAndFeel/PropertyPanelLookAndFeel.h"


class CabbageSettingsWindow :
    public Component,
    public Button::Listener,
    public Value::Listener,
    public FilenameComponentListener,
    public ChangeListener,
    public TextPropertyComponent::Listener
{

public:
    CabbageSettingsWindow (CabbageSettings& settings, AudioDeviceSelectorComponent* audioDevice);
    ~CabbageSettingsWindow()
    {
        codeEditor = nullptr;
        audioDeviceSelector = nullptr;
        colourPanel.clear();
        miscPanel.clear();
        setLookAndFeel (nullptr);
        miscPanel.setLookAndFeel (nullptr);
        colourPanel.setLookAndFeel (nullptr);
    };

    CabbageSettings& settings;
    void changeListenerCallback (ChangeBroadcaster* source) override;
    void addColourProperties();
    void addMiscProperties();
    void resized() override;
    void buttonClicked (Button* button) override;
    void paint (Graphics& g)  override;
    void valueChanged (Value& value) override;
    void updateColourScheme();
    void mouseEnter (const MouseEvent& e) override;
    void selectPanel (String button);
    void filenameComponentChanged (FilenameComponent*) override;
    void textPropertyComponentChanged (TextPropertyComponent* comp) override;
    void loadRepoCode (String codeSnippetName);

    class RepoListBox   :   public Component, ListBoxModel
    {
        int currentRow;
        Colour bgColour;
        int refresh;
        CabbageSettingsWindow* owner;
        int currentIndex;

    public:
        RepoListBox (CabbageSettingsWindow* _owner);
        ~RepoListBox();
        void paint (Graphics& g) override
        {
            listBox.setBounds (0, 0, getWidth(), getHeight());
        };
        void resized() override {};
        int getNumRows() override { return items.size();};
        void setDefaultItem();
        void listBoxItemClicked (int row, const MouseEvent&) override;
        void paintListBoxItem (int rowNumber, Graphics& g,
                               int width, int height, bool rowIsSelected) override;
        void selectedRowsChanged (int /*lastRowselected*/) override {};
        void update();
        ListBox listBox;
        StringArray items;
        StringArray codeSnippets;
        int getCurrentRow()
        {
            return currentRow;
        }

        void updateEntry (String updatedCode);
        void removeEntry();

    };

    RepoListBox listBox;
    ScopedPointer<CodeEditorComponent> codeEditor;
    CodeDocument document;
    CsoundTokeniser csoundTokeniser;


	class ButtonProperty : public ButtonPropertyComponent
	{
	public:
		ButtonProperty(String text, CabbageSettings& _settings) : ButtonPropertyComponent(text, true),
		settings(_settings), text(text)
		{}
		~ButtonProperty() {}

		String getButtonText() const override
		{
			return text;
		}

		void buttonClicked() override
		{
			settings.getUserSettings()->setValue("AudioDriversWarning_dismiss", "0");
		}

	private:
		CabbageSettings& settings;
		String text;
	};

private:
    PropertyPanel colourPanel, miscPanel;
    ScopedPointer<PropertyPanelLookAndFeel> propertyPanelLook;

    Colour bgColour, labelBgColour, labelTextColour;

    ScopedPointer<AudioDeviceSelectorComponent> audioDeviceSelector;
    ValueTree valueTree;
    TextButton deleteRepoButton, saveRepoButton;
    ImageButton audioSettingsButton, colourSettingsButton, miscSettingsButton, codeRepoButton;

    Value alwaysOnTopPluginValue, resetNotifications, alwaysOnTopGraphValue, showLastOpenedFileValue, compileOnSaveValue, breakLinesValue, autoCompleteValue;
    Viewport viewport;

};



#endif  // CABBAGESETTINGSWINDOW_H_INCLUDED
