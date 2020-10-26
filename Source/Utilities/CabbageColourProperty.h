/*
  ==============================================================================

   This file is part of the JUCE library.
   Copyright (c) 2015 - ROLI Ltd.

   Permission is granted to use this software under the terms of either:
   a) the GPL v2 (or any later version)
   b) the Affero GPL v3

   Details of these licenses can be found at: www.gnu.org/licenses

   JUCE is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
   A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

   ------------------------------------------------------------------------------

   To release a closed-source product which uses JUCE, commercial licenses are
   available: visit www.juce.com for more information.

  ==============================================================================
*/

#ifndef CABBAGECOLOURPROPERTYCOMPONENT_H_INCLUDED
#define CABBAGECOLOURPROPERTYCOMPONENT_H_INCLUDED

#include "../CabbageCommonHeaders.h"

class CabbagePropertiesPanel;

//======= ColourPallete =======
class ColourPallete : public ColourSelector
{
public:
    ColourPallete(): ColourSelector()
    {
        setColour (ColourSelector::backgroundColourId, Colours::black);
        setColour (ColourSelector::labelTextColourId, Colours::white);

    };

    ~ColourPallete()
    {
        this->removeAllChangeListeners();
    };

    static Array<Colour> getColourSwatches()
    {
        Array <Colour> swatchColours;
        //setup swatches for colour selector.
        swatchColours.set (0, Colour (0xFF000000));
        swatchColours.set (1, Colour (0xFFFFFFFF));
        swatchColours.set (2, Colour (0xFFFF0000));
        swatchColours.set (3, Colour (0xFF00FF00));
        swatchColours.set (4, Colour (0xFF0000FF));
        swatchColours.set (5, Colour (0xFFFFFF00));
        swatchColours.set (6, Colour (0xFFFF00FF));
        swatchColours.set (7, Colour (0xFF00FFFF));
        swatchColours.set (8, Colour (0x80000000));
        swatchColours.set (9, Colour (0x80FFFFFF));
        swatchColours.set (10, Colour (0x80FF0000));
        swatchColours.set (11, Colour (0x8000FF00));
        swatchColours.set (12, Colour (0x800000FF));
        swatchColours.set (13, Colour (0x80FFFF00));
        swatchColours.set (14, Colour (0x80FF00FF));
        swatchColours.set (15, Colour (0x8000FFFF));
        return swatchColours;
    }

    int getNumSwatches() const override
    {
        return getColourSwatches().size();
    }

    Colour getSwatchColour (int index) const override
    {
        return getColourSwatches()[index];
    }

    void setSwatchColour (const int index, const Colour& newColour) override
    {
        getColourSwatches().set (index, newColour);
    }

    String getNameOfParent()
    {
        return nameOfParent;
    }
    void setNameOfParent (String parent)
    {
        nameOfParent = parent;
    }

private:
    String nameOfParent;


};

class ColourPropertyComponent : public PropertyComponent, public ChangeListener, public ChangeBroadcaster
{
    bool colourSettings;

public :
    //======= ColourPropertyComponent =======
    ColourPropertyComponent (String name, String colourString, bool colourSettings = false);
    ~ColourPropertyComponent() {}
    void paint (Graphics& g)  override;
    void mouseDown (const MouseEvent& e) override;
    void changeListenerCallback (juce::ChangeBroadcaster* source) override;
    void refresh() override {}
    String getCurrentColourString();
    Colour colour;
    String name;


};

class ColourMultiPropertyComponent : public PropertyComponent, public ChangeListener, public Button::Listener, public ChangeBroadcaster
{
    bool colourSettings;
    TextEditor editor;

    class OverlayComponent : public Component
    {
        Colour overlayColour;
    public:
        explicit OverlayComponent (String name): Component (name) {}
        void setColour (Colour colour) {  overlayColour = colour; repaint();}
        void paint (Graphics& g)  override {    g.fillAll (overlayColour);   }
    };

    OwnedArray<OverlayComponent> overlayComponents;
    LookAndFeel_V2 lookAndFeel;
    Viewport viewport;
    Component overlayComponentContainer;
    TextButton addColour, removeColour;
    Array<Colour> colours;

    void addNewColour (Colour colour);
public :
    //======= ColourPropertyComponent =======
    ColourMultiPropertyComponent (String name, var colours, bool colourSettings = false);
    ~ColourMultiPropertyComponent() {}
    void paint (Graphics& g)  override;
    void mouseDown (const MouseEvent& e) override;
    void changeListenerCallback (juce::ChangeBroadcaster* source) override;
    void refresh() override {}
    void resized() override;
    void buttonClicked (Button* button) override;
    String getCurrentColourString();
    Colour colour;
    String name;
    int currentColourIndex = 0;


};
#endif  // CABBAGECOLOURPROPERTYCOMPONENT_H_INCLUDED
