/*
  ==============================================================================

    CabbageColourProperty.cpp
    Created: 27 Oct 2016 12:34:46pm
    Author:  rory

  ==============================================================================
*/

#include "CabbageColourProperty.h"


//======= ColourPropertyComponent =======
ColourPropertyComponent::ColourPropertyComponent(String name, String colourString, bool colourSettings):
    PropertyComponent(name, 25), colourSettings(colourSettings)
{
    this->setName(name);
    colour = Colour::fromString(colourString);
    setSize(100, 30);
}


void ColourPropertyComponent::paint(Graphics &g)
{
    g.setColour(getLookAndFeel().findColour(PropertyComponent::ColourIds::backgroundColourId));
    g.fillRect(getLocalBounds().reduced(0));


    const Colour textColour = isEnabled() == true ? getLookAndFeel().findColour(PropertyComponent::ColourIds::labelTextColourId) :
                              getLookAndFeel().findColour(PropertyComponent::ColourIds::labelTextColourId).withAlpha(.5f);

    g.setColour (textColour);
    g.setFont (getHeight()*.6);

	
    const int textW = colourSettings == true ? 300 : jmin (200, getWidth() / 3);
    const Rectangle<int> r (textW, 1, getWidth() - textW - 1, getHeight() - 3);

    g.drawFittedText (getName(),
                      3, r.getY(), r.getX() - 5, r.getHeight(),
                      Justification::centredLeft, 2);

    g.setColour(colour);
    g.fillRect( textW, r.getY(), getWidth(), r.getHeight());

}

void ColourPropertyComponent::mouseDown(const MouseEvent& e)
{
    if(isEnabled())
    {
        ColourPallete* colourSelector = new ColourPallete();
        colourSelector->setBounds(0, 0, 300, 300);
        colourSelector->addChangeListener(this);
        colourSelector->setNameOfParent(name);
        colourSelector->setCurrentColour(colour);
        CallOutBox::launchAsynchronously(colourSelector, getScreenBounds(), nullptr);
        colour = colourSelector->getCurrentColour();
		
    }
}

void ColourPropertyComponent::changeListenerCallback(juce::ChangeBroadcaster *source)
{
    if(ColourPallete* cs = dynamic_cast <ColourPallete*> (source))
	{
		if(cs->getNameOfParent()==name)
			colour = cs->getCurrentColour();
		repaint();
		sendChangeMessage();
	}
}

String ColourPropertyComponent::getCurrentColourString()
{
    return colour.toString();
}