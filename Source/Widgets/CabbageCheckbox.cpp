/*
  ==============================================================================

    CabbageCheckbox.cpp
    Created: 25 Oct 2016 8:01:43pm
    Author:  rory

  ==============================================================================
*/

#include "CabbageCheckbox.h"

//==============================================================================
// custom checkbox component with optional surrounding groupbox
//==============================================================================
CabbageCheckbox::CabbageCheckbox(ValueTree widgetData) :
name(CabbageWidget::getStringProp(widgetData, CabbageIdentifierIds::name)),
caption(CabbageWidget::getStringProp(widgetData, CabbageIdentifierIds::caption)),
buttonText(CabbageWidget::getStringProp(widgetData, CabbageIdentifierIds::text)),
colour(CabbageWidget::getStringProp(widgetData, CabbageIdentifierIds::colour)),
fontcolour(CabbageWidget::getStringProp(widgetData, CabbageIdentifierIds::fontcolour)),
oncolour(CabbageWidget::getStringProp(widgetData, CabbageIdentifierIds::oncolour)),
isRect(CabbageWidget::getStringProp(widgetData, CabbageIdentifierIds::shape).equalsIgnoreCase("square")),
rotate(CabbageWidget::getNumProp(widgetData, CabbageIdentifierIds::rotate)),
pivotx(CabbageWidget::getNumProp(widgetData, CabbageIdentifierIds::pivotx)),
pivoty(CabbageWidget::getNumProp(widgetData, CabbageIdentifierIds::pivoty)),
tooltipText(String::empty),
corners(CabbageWidget::getNumProp(widgetData, CabbageIdentifierIds::corners)),
groupbox(String("groupbox_")+name),
button(name)
{
	widgetData.addListener(this);
	setName(name);
	offX=offY=offWidth=offHeight=0;

    const float left = CabbageWidget::getNumProp(widgetData, CabbageIdentifierIds::left);
    const float top = CabbageWidget::getNumProp(widgetData, CabbageIdentifierIds::top);
    const float width = CabbageWidget::getNumProp(widgetData, CabbageIdentifierIds::width);
    const float height = CabbageWidget::getNumProp(widgetData, CabbageIdentifierIds::height);


	button.getProperties().set("svgpath", CabbageWidget::getStringProp(widgetData, CabbageIdentifierIds::svgpath));
	addAndMakeVisible(groupbox);
	addAndMakeVisible(button);
	groupbox.setVisible(false);
	groupbox.getProperties().set("groupLine", var(1));
	groupbox.setColour(GroupComponent::textColourId, Colour::fromString(fontcolour));
	groupbox.setColour(TextButton::buttonColourId, CabbageUtilities::getComponentSkin());

	button.setButtonText(buttonText);
	button.getProperties().set("cornersize", corners);
	
	if(!CabbageWidget::getNumProp(widgetData, CabbageIdentifierIds::visible))
        this->setVisible(false);
	
	if(caption.length()>0)
	{
		offX=10;
		offY=20;
		offWidth=-20;
		offHeight=-30;
		groupbox.setVisible(true);
		groupbox.setText(caption);
	}

	if(CabbageWidget::getStringProp(widgetData, CabbageIdentifierIds::popuptext).isNotEmpty())
	{
		tooltipText = CabbageWidget::getStringProp(widgetData, CabbageIdentifierIds::popuptext);
		button.setTooltip(tooltipText);
	}

	button.getProperties().set("isRect", isRect);

	if(CabbageWidget::getNumProp(widgetData, CabbageIdentifierIds::radiogroup)!=0)
		button.setRadioGroupId(CabbageWidget::getNumProp(widgetData, CabbageIdentifierIds::radiogroup));

	button.setColour(ToggleButton::textColourId, Colour::fromString(fontcolour));
	button.setColour(TextButton::buttonOnColourId, Colour::fromString(oncolour));
	button.setColour(TextButton::buttonColourId, Colour::fromString(colour));
	button.setButtonText(buttonText);
	setAlpha(CabbageWidget::getNumProp(widgetData, CabbageIdentifierIds::alpha));

	//set initial value if given
	if(CabbageWidget::getNumProp(widgetData, CabbageIdentifierIds::value)==1)
		button.setToggleState(true, dontSendNotification);
	else
		button.setToggleState(false, dontSendNotification);

	this->setWantsKeyboardFocus(false);
		
	setBounds(left, top, width, height);
}

//==============================================================================
void CabbageCheckbox::valueTreePropertyChanged (ValueTree& valueTree, const Identifier&)
{
	const MessageManagerLock mmLock;
	button.setColour(ToggleButton::textColourId, Colour::fromString(CabbageWidget::getStringProp(valueTree, CabbageIdentifierIds::fontcolour)));
	button.setColour(TextButton::buttonColourId, Colour::fromString(CabbageWidget::getStringProp(valueTree, CabbageIdentifierIds::colour)));
	setBounds(CabbageWidget::getBounds(valueTree));
	button.getProperties().set("isRect", CabbageWidget::getStringProp(valueTree, CabbageIdentifierIds::shape).equalsIgnoreCase("square"));
	button.setButtonText(CabbageWidget::getStringProp(valueTree, CabbageIdentifierIds::text));
	setAlpha(CabbageWidget::getNumProp(valueTree, CabbageIdentifierIds::alpha));
	if(rotate!=CabbageWidget::getNumProp(valueTree, CabbageIdentifierIds::rotate))
	{
		rotate = CabbageWidget::getNumProp(valueTree, CabbageIdentifierIds::rotate);
		setTransform(AffineTransform::rotation(rotate, getX()+CabbageWidget::getNumProp(valueTree, CabbageIdentifierIds::pivotx), getY()+CabbageWidget::getNumProp(valueTree, CabbageIdentifierIds::pivoty)));
	}
	if(!CabbageWidget::getNumProp(valueTree, CabbageIdentifierIds::visible))
	{
		setVisible(false);
		setEnabled(false);
	}
	else
	{
		setVisible(true);
		setEnabled(true);
	}
	if(!CabbageWidget::getNumProp(valueTree, CabbageIdentifierIds::active))
	{
		setEnabled(false);
	}
	else
	{
		setEnabled(true);
	}
	if(tooltipText!=CabbageWidget::getStringProp(valueTree, CabbageIdentifierIds::popuptext))
	{
		tooltipText = CabbageWidget::getStringProp(valueTree, CabbageIdentifierIds::popuptext);
		button.setTooltip(tooltipText);
	}
	repaint();		
}

//---------------------------------------------
void CabbageCheckbox::resized()
{
	groupbox.setBounds(0, 0, getWidth(), getHeight());
	button.setBounds(offX, offY, getWidth()+offWidth, getHeight()+offHeight);
	if(rotate!=0)
		setTransform(AffineTransform::rotation(rotate, getX()+pivotx, pivoty+getY()));
	this->setWantsKeyboardFocus(false);
}
