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


//==============================================================================
class CabbagePropertiesPanel :
    public Component,
    public ChangeBroadcaster,
    public Value::Listener,
    public TextPropertyComponent::Listener,
    public ChangeListener,
    public FilenameComponentListener
{
public:
    CabbagePropertiesPanel(ValueTree widgetData);
    void paint (Graphics& g) override;
    void resized() override;
    void setPropertyByName(String name, var value);
    void textPropertyComponentChanged (TextPropertyComponent * comp);
    void changeListenerCallback(juce::ChangeBroadcaster *source);
    void updateProperties(ValueTree widgetData);
    void valueChanged(Value& value);
    void filenameComponentChanged (FilenameComponent* fileComponent);

    Array<PropertyComponent*> createPositionEditors(ValueTree valueTree);
    Array<PropertyComponent*> createTextEditors(ValueTree valueTree);
    Array<PropertyComponent*> createColourChoosers (ValueTree valueTree);
    Array<PropertyComponent*> createMiscEditors(ValueTree valueTree);
    Array<PropertyComponent*> createFileEditors(ValueTree valueTree);
    Array<PropertyComponent*> createValueEditors(CabbagePropertiesPanel* owner, ValueTree valueTree);
	Array<PropertyComponent*> createWidgetArrayEditors(CabbagePropertiesPanel* owner, ValueTree valueTree);
    Value isActiveValue, isVisibleValue, alphaValue, shapeValue, sliderNumberBoxValue, alignValue, velocityValue, fileModeValue;
    Colour backgroundColour, borderColour;


    void setBackgroundColour(Colour colour)
    {
        backgroundColour = colour;
        repaint();
    }

    void setBorderColour(Colour colour)
    {
        borderColour = colour;
        repaint();
    }
private:
    PropertyPanel propertyPanel;
    ValueTree widgetData;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CabbagePropertiesPanel)
};

#endif  // CABBAGEPROPERTIESCOMPONENT_H_INCLUDED
