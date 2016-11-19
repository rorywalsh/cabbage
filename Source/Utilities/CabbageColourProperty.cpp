/*
  ==============================================================================

    CabbageColourProperty.cpp
    Created: 27 Oct 2016 12:34:46pm
    Author:  rory

  ==============================================================================
*/

#include "CabbageColourProperty.h"


//======= ColourPropertyComponent =======
ColourPropertyComponent::ColourPropertyComponent(String name, String colourString):
    PropertyComponent(name, 25)
{
    this->setName(name);
    colour = Colour::fromString(colourString);
    setSize(100, 30);
}


void ColourPropertyComponent::paint(Graphics &g)
{
    g.setColour(getLookAndFeel().findColour(PropertyComponent::ColourIds::backgroundColourId));
    g.fillRect(getLocalBounds().reduced(0));

    g.setColour (getLookAndFeel().findColour(PropertyComponent::ColourIds::labelTextColourId));
    g.setFont (getHeight()*.6);


    const int textW = jmin (200, getWidth() / 3);
    const Rectangle<int> r (textW, 1, getWidth() - textW - 1, getHeight() - 3);

    g.drawFittedText (getName(),
                      3, r.getY(), r.getX() - 5, r.getHeight(),
                      Justification::centredLeft, 2);

    g.setColour(colour);
    g.fillRect( getWidth()/3, r.getY(), getWidth(), r.getHeight());

}

void ColourPropertyComponent::mouseDown(const MouseEvent& e)
{
    ColourPallete* colourSelector = new ColourPallete();
    colourSelector->setBounds(0, 0, 300, 300);
    colourSelector->addChangeListener(this);
    colourSelector->setNameOfParent(name);
    colourSelector->setCurrentColour(colour);
    CallOutBox &callOut = CallOutBox::launchAsynchronously(colourSelector, getScreenBounds(), nullptr);
    colour = colourSelector->getCurrentColour();
}

void ColourPropertyComponent::changeListenerCallback(juce::ChangeBroadcaster *source)
{
    ColourPallete* cs = dynamic_cast <ColourPallete*> (source);
    if(cs->getNameOfParent()==name)
        colour = cs->getCurrentColour();
    repaint();
    sendChangeMessage();
}

String ColourPropertyComponent::getCurrentColourString()
{
    return colour.toString();
}