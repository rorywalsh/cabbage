/*
  ==============================================================================

    CabbageColourProperty.cpp
    Created: 27 Oct 2016 12:34:46pm
    Author:  rory

  ==============================================================================
*/

#include "CabbageColourProperty.h"


//======= ColourPropertyComponent =======
ColourPropertyComponent::ColourPropertyComponent (String name, String colourString, bool colourSettings):
    PropertyComponent (name, 25), colourSettings (colourSettings)
{
    this->setName (name);
    colour = Colour::fromString (colourString);
    setSize (100, 30);
}


void ColourPropertyComponent::paint (Graphics& g)
{
    g.setColour (getLookAndFeel().findColour (PropertyComponent::ColourIds::backgroundColourId));
    g.fillRect (getLocalBounds().reduced (0));


    const Colour textColour = isEnabled() == true ? getLookAndFeel().findColour (PropertyComponent::ColourIds::labelTextColourId) :
                              getLookAndFeel().findColour (PropertyComponent::ColourIds::labelTextColourId).withAlpha (.5f);

    g.setColour (textColour);
    g.setFont (getHeight()*.6);


    const int textW = colourSettings == true ? 300 : jmin (200, getWidth() / 3);
    const Rectangle<int> r (textW, 1, getWidth() - textW - 1, getHeight() - 3);

    g.drawFittedText (getName(),
                      3, r.getY(), r.getX() - 5, r.getHeight(),
                      Justification::centredLeft, 2);

    g.setColour (colour);
    g.fillRect ( textW, r.getY(), getWidth(), r.getHeight());

}

void ColourPropertyComponent::mouseDown (const MouseEvent& e)
{
    if (isEnabled())
    {
        ColourPallete* colourSelector = new ColourPallete();
        colourSelector->setBounds (0, 0, 300, 300);
        colourSelector->addChangeListener (this);
        colourSelector->setNameOfParent (name);
        colourSelector->setCurrentColour (colour);
        CallOutBox::launchAsynchronously (colourSelector, getScreenBounds(), nullptr);
        colour = colourSelector->getCurrentColour();

    }
}

void ColourPropertyComponent::changeListenerCallback (juce::ChangeBroadcaster* source)
{
    if (ColourPallete* cs = dynamic_cast <ColourPallete*> (source))
    {
        if (cs->getNameOfParent() == name)
            colour = cs->getCurrentColour();

        repaint();
        sendChangeMessage();
    }
}

String ColourPropertyComponent::getCurrentColourString()
{
    return colour.toString();
}

//===============================================================================================================
ColourMultiPropertyComponent::ColourMultiPropertyComponent (String name, var colours, bool colourSettings):
    PropertyComponent (name, 60), 
	colourSettings (colourSettings), 
	editor("Colours"), 
	overlayComponentContainer("container"),
	addColour("+"),
	removeColour("-"),
	lookAndFeel()
{
    this->setName (name);
	lookAndFeel.setColour(ScrollBar::ColourIds::thumbColourId, Colours::whitesmoke);
	viewport.setLookAndFeel(&lookAndFeel);

	for( int i = 0 ; i < colours.size() ; i++)
		addNewColour(Colour::fromString(colours[i].toString()));
	
	addAndMakeVisible(viewport);
	addAndMakeVisible(addColour);
	addColour.addListener(this);
	removeColour.addListener(this);
	addAndMakeVisible(removeColour);
	viewport.setViewedComponent(&overlayComponentContainer);
	viewport.setScrollBarsShown(true, false);
	//addAndMakeVisible(editor);

    setSize (100, 30);
}

void ColourMultiPropertyComponent::resized()
{
    const int textW = colourSettings == true ? 300 : jmin (200, getWidth() / 3);
    const Rectangle<int> r (textW, 1, getWidth() - textW - 1, getHeight() - 3);
	
	viewport.setBounds(r);
	addColour.setBounds(4, 20, (textW/2)-4, 20);
	removeColour.setBounds((textW/2), 20, (textW/2)-4, 20);
	
	overlayComponentContainer.setBounds(r.withHeight(overlayComponents.size()*25));
	
	for(int i = 0 ; i < overlayComponents.size(); i++)
	{
		overlayComponents[i]->setBounds(0, (i*27), overlayComponentContainer.getWidth(), 25);
	}
	
}

void ColourMultiPropertyComponent::addNewColour(Colour colour)
{
	OverlayComponent* comp;
	overlayComponents.add(comp = new OverlayComponent("overlay"+String(overlayComponents.size()+1)));
	comp->addMouseListener(this, false);
	
	comp->setColour(colour);
	overlayComponentContainer.addAndMakeVisible(comp);	
	colours.add(colour);	
}

void ColourMultiPropertyComponent::buttonClicked(Button* button)
{
	if(button->getName()=="+")
	{
		Colour colour = Colour(Random().nextInt()*255, Random().nextInt()*255, Random().nextInt()*255);
		addNewColour(colour);
	}
	else 
	{
		if(overlayComponents.size()>1)
		{
			overlayComponents.remove(overlayComponents.size()-1);
			colours.remove(colours.size()-1);			
		}
	}
	
	resized();
}
void ColourMultiPropertyComponent::paint (Graphics& g)
{
    g.setColour (getLookAndFeel().findColour (PropertyComponent::ColourIds::backgroundColourId));
    g.fillRect (getLocalBounds().reduced (0));


    const Colour textColour = isEnabled() == true ? getLookAndFeel().findColour (PropertyComponent::ColourIds::labelTextColourId) :
                              getLookAndFeel().findColour (PropertyComponent::ColourIds::labelTextColourId).withAlpha (.5f);

    g.setColour (textColour);
    g.setFont (25*.6);


    const int textW = colourSettings == true ? 300 : jmin (200, getWidth() / 3);
    const Rectangle<int> r (textW, 1, getWidth() - textW - 1, 25 - 3);

    g.drawFittedText (getName(),
                      3, r.getY(), r.getX() - 5, r.getHeight(),
                      Justification::centredLeft, 2);

	if(overlayComponents.size()==1)
	{
		g.setColour (textColour);
		g.drawFittedText ("Hit '+' to add a colour..", textW, 25, getWidth()-textW, r.getHeight(), Justification::centred, 1);		
	}
}

void ColourMultiPropertyComponent::mouseDown (const MouseEvent& e)
{
	const String overlayComponentName = e.eventComponent->getName();
	
	if(overlayComponentName.contains("overlay"))
	{
		if (isEnabled())
		{
			const int colourIndex = overlayComponentName.replace("overlay", "").getIntValue()-1;
			ColourPallete* colourSelector = new ColourPallete();
			colourSelector->setBounds (0, 0, 300, 300);
			colourSelector->addChangeListener (this);
			colourSelector->setNameOfParent (overlayComponentName);
			colourSelector->setCurrentColour (colours[colourIndex]);
			CallOutBox::launchAsynchronously (colourSelector, getScreenBounds(), nullptr);
			colour = colourSelector->getCurrentColour();
		}		
	}
}

void ColourMultiPropertyComponent::changeListenerCallback (juce::ChangeBroadcaster* source)
{
    if (ColourPallete* cs = dynamic_cast <ColourPallete*> (source))
    { 
		const String overlayComponentName = cs->getNameOfParent();
		const int colourIndex = overlayComponentName.replace("overlay", "").getIntValue()-1;
		colours.getReference(colourIndex) = cs->getCurrentColour();
		overlayComponents[colourIndex]->setColour(cs->getCurrentColour());    
		currentColourIndex = colourIndex;  
		colour = cs->getCurrentColour();
        sendChangeMessage();
    }
}

String ColourMultiPropertyComponent::getCurrentColourString()
{
    return colour.toString();
}