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

#include "../CabbageIds.h"

class ColourSettingsPropertyComponent : public PropertyComponent, public ChangeListener, public ActionBroadcaster
{

public :

    //======= ColourPallete =======
    class ColourPallete : public ColourSelector
    {
    public:
        ColourPallete(): ColourSelector()
        {
            setColour(ColourSelector::backgroundColourId, Colours::black);
            setColour(ColourSelector::labelTextColourId, Colours::white);
        };

        ~ColourPallete()
        {
            this->removeAllChangeListeners();
        };

        static Array<Colour> getColourSwatches()
        {
            Array <Colour> swatchColours;
            //setup swatches for colour selector.
            swatchColours.set(0, Colour(0xFF000000));
            swatchColours.set(1, Colour(0xFFFFFFFF));
            swatchColours.set(2, Colour(0xFFFF0000));
            swatchColours.set(3, Colour(0xFF00FF00));
            swatchColours.set(4, Colour(0xFF0000FF));
            swatchColours.set(5, Colour(0xFFFFFF00));
            swatchColours.set(6, Colour(0xFFFF00FF));
            swatchColours.set(7, Colour(0xFF00FFFF));
            swatchColours.set(8, Colour(0x80000000));
            swatchColours.set(9, Colour(0x80FFFFFF));
            swatchColours.set(10, Colour(0x80FF0000));
            swatchColours.set(11, Colour(0x8000FF00));
            swatchColours.set(12, Colour(0x800000FF));
            swatchColours.set(13, Colour(0x80FFFF00));
            swatchColours.set(14, Colour(0x80FF00FF));
            swatchColours.set(15, Colour(0x8000FFFF));
            return swatchColours;
        }

        int getNumSwatches() const
        {
            return getColourSwatches().size();
        }

        Colour getSwatchColour(int index) const
        {
            return getColourSwatches()[index];
        }

        void setSwatchColour (const int index, const Colour &newColour) const
        {
            getColourSwatches().set(index, newColour);
        }

        String getNameOfParent()
        {
            return nameOfParent;
        }

        void setNameOfParent(String parent)
        {
            nameOfParent = parent;
        }

    private:
        String nameOfParent;


    };

    //======= ColourPropertyComponent =======
    ColourSettingsPropertyComponent(ValueTree value, int index): valueTree(value), PropertyComponent("name", 18)
    {
        name = CabbageSettings::getColourPropertyName(valueTree, index);
        colour = CabbageSettings::getColourFromValueTree(valueTree, index, Colours::red);
        this->setName(name);
        //colour = Colour::fromString(colourString);
        setSize(100, 30);
    }

    ~ColourSettingsPropertyComponent() {}

    void paint(Graphics &g)
    {
        g.fillAll(colour);
        g.setColour(Colours::white);
        //float borderWidth = 10;
        //g.fillRoundedRectangle(0.f, 0.f, getWidth()-borderWidth, getHeight()-borderWidth,  getHeight()*0.1);
        g.setColour(colour);
        //g.fillRoundedRectangle(2.f, 2.f, getWidth()-borderWidth-2, getHeight()-borderWidth-2,  getHeight()*0.1);
        g.setColour (colour.contrasting());
        g.setFont (Font(18));
        const juce::Rectangle<int> r (5, 0, getWidth()-1, getHeight()+1);
        g.drawFittedText(name, r, Justification::centred, 2);
    }

    void mouseDown(const MouseEvent& e)
    {
        ColourPallete* colourSelector = new ColourPallete();
        colourSelector->setBounds(0, 0, 300, 300);
        colourSelector->addChangeListener(this);
        colourSelector->setNameOfParent(name);
        colourSelector->setCurrentColour(colour);
        CallOutBox &callOut = CallOutBox::launchAsynchronously(colourSelector, getScreenBounds(), nullptr);
        colour = colourSelector->getCurrentColour();
    }

    void changeListenerCallback(juce::ChangeBroadcaster *source)
    {
        ColourPallete* cs = dynamic_cast <ColourPallete*> (source);
        if(cs->getNameOfParent()==name)
            colour = cs->getCurrentColour();
        repaint();
        CabbageSettings::set(valueTree, "Colours", name, colour.toString());
    };

    void refresh() override {}


    Colour colour;
    String name;
    //ColourPallete colourSelector;
    ValueTree valueTree;

};

#endif  // CABBAGECOLOURPROPERTYCOMPONENT_H_INCLUDED
