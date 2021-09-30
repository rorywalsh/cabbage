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

#ifndef CABBAGEPROPERTIESCOMPONENT_H_INCLUDED
#define CABBAGEPROPERTIESCOMPONENT_H_INCLUDED

#include "../CabbageCommonHeaders.h"
#include "../Utilities/CabbageColourProperty.h"
#include "../Utilities/CabbageFilePropertyComponent.h"
#include "../LookAndFeel/FlatButtonLookAndFeel.h"
#include "../LookAndFeel/PropertyPanelLookAndFeel.h"

//==============================================================================
class CabbagePropertiesPanel :
    public Component,
    public ChangeBroadcaster,
	public Button::Listener,
    public Value::Listener,
    public TextPropertyComponent::Listener,
    public ChangeListener,
    public FilenameComponentListener
{
public:
    CabbagePropertiesPanel (ValueTree widgetData);
    ~CabbagePropertiesPanel() override;
    void paint (Graphics& g) override;
    void resized() override;
    void getAmpRangeForTable (String identifier, var value);
    void getScrubberPositionForTable (String identifier, var value);
    void setPropertyByName (String name, var value);
    void textPropertyComponentChanged (TextPropertyComponent* comp) override;
    void changeListenerCallback (juce::ChangeBroadcaster* source) override;
    void updateProperties (ValueTree widgetData);
    void valueChanged (Value& value) override;
    void filenameComponentChanged (FilenameComponent* fileComponent) override;
    void saveOpenessState();

	void buttonClicked(Button *) override;
    Array<PropertyComponent*> createPositionEditors (ValueTree valueTree);
    Array<PropertyComponent*> createTextEditors (ValueTree valueTree);
    Array<PropertyComponent*> createNumberEditors (ValueTree valueTree);
    Array<PropertyComponent*> createColourChoosers (ValueTree valueTree);
    Array<PropertyComponent*> createMiscEditors (ValueTree valueTree);
    Array<PropertyComponent*> createFileEditors (ValueTree valueTree);
    Array<PropertyComponent*> createChannelEditors (ValueTree valueTree);
    Array<PropertyComponent*> createValueEditors (CabbagePropertiesPanel* owner, ValueTree valueTree);
    Array<PropertyComponent*> createWidgetArrayEditors (CabbagePropertiesPanel* owner, ValueTree valueTree);
    Array<PropertyComponent*> createAmpRangeEditors (ValueTree valueTree);
    Array<PropertyComponent*> createTwoValueEditors (ValueTree valueTree, Identifier identifier);
    Value isActiveValue, isVisibleValue, alphaValue, shapeValue, filmstripFrames,
          sliderNumberBoxValue, alignValue, velocityValue, fileModeValue,
          fillTableWaveformValue, zoomValue, channelTypeValue, innerRadius, outerRadius;
    Colour backgroundColour, borderColour;
	bool hide = false;

    void setColours (Colour panelBG, Colour labelBG, Colour labelText)
    {
        backgroundColour = panelBG;
        propertyPanelLook->setColours (panelBG, labelBG, labelText);
        repaint();
    }

    void setBorderColour (Colour colour)
    {
        borderColour = colour;
        repaint();
    }

private:

    PropertyPanel propertyPanel;
    String previousWidgetName = "";
	

    struct SectionState
    {
        SectionState (String name, XmlElement* xml): name (name), xmlElement (xml)
        {}
        String name;
        std::unique_ptr<XmlElement> xmlElement;
    };

    OwnedArray<SectionState> sectionStates;

    SectionState* getSectionState (String name)
    {
        for ( auto section : sectionStates)
        {
            if ( section->name == name)
                return section;
        }

        return nullptr;
    }

    ValueTree widgetData;
	TextButton hideButton;
    std::unique_ptr<FlatButtonLookAndFeel> flatLook;
    std::unique_ptr<PropertyPanelLookAndFeel> propertyPanelLook;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CabbagePropertiesPanel)
};

#endif  // CABBAGEPROPERTIESCOMPONENT_H_INCLUDED
