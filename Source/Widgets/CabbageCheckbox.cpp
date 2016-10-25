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
CabbageCheckbox::CabbageCheckbox(ValueTree wData) :
name(CabbageWidget::getStringProp(wData, CabbageIdentifierIds::name)),
caption(CabbageWidget::getStringProp(wData, CabbageIdentifierIds::caption)),
buttonText(CabbageWidget::getStringProp(wData, CabbageIdentifierIds::text)),
colour(CabbageWidget::getStringProp(wData, CabbageIdentifierIds::colour)),
fontcolour(CabbageWidget::getStringProp(wData, CabbageIdentifierIds::fontcolour)),
oncolour(CabbageWidget::getStringProp(wData, CabbageIdentifierIds::oncolour)),
isRect(CabbageWidget::getStringProp(wData, CabbageIdentifierIds::shape).equalsIgnoreCase("square")),
rotate(CabbageWidget::getNumProp(wData, CabbageIdentifierIds::rotate)),
pivotx(CabbageWidget::getNumProp(wData, CabbageIdentifierIds::pivotx)),
pivoty(CabbageWidget::getNumProp(wData, CabbageIdentifierIds::pivoty)),
tooltipText(String::empty),
corners(CabbageWidget::getNumProp(wData, CabbageIdentifierIds::corners)),
groupbox(String("groupbox_")+name),
widget(name),
widgetData(wData)
{
	widgetData.addListener(this);
	setName(name);
	offX=offY=offWidth=offHeight=0;

    const float left = CabbageWidget::getNumProp(widgetData, CabbageIdentifierIds::left);
    const float top = CabbageWidget::getNumProp(widgetData, CabbageIdentifierIds::top);
    const float width = CabbageWidget::getNumProp(widgetData, CabbageIdentifierIds::width);
    const float height = CabbageWidget::getNumProp(widgetData, CabbageIdentifierIds::height);


	widget.getProperties().set("svgpath", CabbageWidget::getStringProp(widgetData, CabbageIdentifierIds::svgpath));
	addAndMakeVisible(groupbox);
	addAndMakeVisible(widget);
	groupbox.setVisible(false);
	groupbox.getProperties().set("groupLine", var(1));
	groupbox.setColour(GroupComponent::textColourId, Colour::fromString(fontcolour));
	groupbox.setColour(TextButton::buttonColourId, CabbageUtilities::getComponentSkin());

	widget.setButtonText(buttonText);
	widget.getProperties().set("cornersize", corners);
	
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
		widget.setTooltip(tooltipText);
	}

	widget.getProperties().set("isRect", isRect);

	if(CabbageWidget::getNumProp(widgetData, CabbageIdentifierIds::radiogroup)!=0)
		widget.setRadioGroupId(CabbageWidget::getNumProp(widgetData, CabbageIdentifierIds::radiogroup));

	widget.setColour(ToggleButton::textColourId, Colour::fromString(fontcolour));
	widget.setColour(TextButton::buttonOnColourId, Colour::fromString(oncolour));
	widget.setColour(TextButton::buttonColourId, Colour::fromString(colour));
	widget.setButtonText(buttonText);
	setAlpha(CabbageWidget::getNumProp(widgetData, CabbageIdentifierIds::alpha));

	//set initial value if given
	if(CabbageWidget::getNumProp(widgetData, CabbageIdentifierIds::value)==1)
		widget.setToggleState(true, dontSendNotification);
	else
		widget.setToggleState(false, dontSendNotification);

	this->setWantsKeyboardFocus(false);
		
	setBounds(left, top, width, height);
}

//==============================================================================
void CabbageCheckbox::valueTreePropertyChanged (ValueTree& valueTree, const Identifier&)
{
	const MessageManagerLock mmLock;
	widget.setColour(ToggleButton::textColourId, Colour::fromString(CabbageWidget::getStringProp(valueTree, CabbageIdentifierIds::fontcolour)));
	widget.setColour(TextButton::buttonColourId, Colour::fromString(CabbageWidget::getStringProp(valueTree, CabbageIdentifierIds::colour)));
	setBounds(CabbageWidget::getBounds(valueTree));
	widget.getProperties().set("isRect", CabbageWidget::getStringProp(valueTree, CabbageIdentifierIds::shape).equalsIgnoreCase("square"));
	widget.setButtonText(CabbageWidget::getStringProp(valueTree, CabbageIdentifierIds::text));
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
		widget.setTooltip(tooltipText);
	}
	repaint();		
}

//---------------------------------------------
void CabbageCheckbox::resized()
{
	groupbox.setBounds(0, 0, getWidth(), getHeight());
	widget.setBounds(offX, offY, getWidth()+offWidth, getHeight()+offHeight);
	if(rotate!=0)
		setTransform(AffineTransform::rotation(rotate, getX()+pivotx, pivoty+getY()));
	this->setWantsKeyboardFocus(false);
}
