/*
  ==============================================================================

    CabbageColourProperty.cpp
    Created: 27 Oct 2016 12:34:46pm
    Author:  rory

  ==============================================================================
*/

#include "CabbageColourProperty.h"


//======= ColourPropertyComponent =======
ColourPropertyComponent::ColourPropertyComponent(String name, int index):
PropertyComponent(name, 18)
{
	this->setName(name);
	//colour = Colour::fromString(colourString);
	setSize(100, 30);
}


void ColourPropertyComponent::paint(Graphics &g)
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
   
};