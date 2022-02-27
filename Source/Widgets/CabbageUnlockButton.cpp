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

#include "CabbageUnlockButton.h"
#include "../Audio/Plugins/CabbagePluginEditor.h"

class UnlockForm : public Component
{

public:
	UnlockForm() :Component() {};
	~UnlockForm() override { setLookAndFeel(nullptr); }
	/*void paint(Graphics& g) {
			g.fillAll(Colours::red);
	}*/

};

CabbageUnlockButton::CabbageUnlockButton(ValueTree wData, CabbagePluginEditor* _owner)
	: TextButton(),
	owner(_owner),
	widgetData(wData),
	CabbageWidgetBase(_owner)
{
	widgetData.addListener(this);              //add listener to valueTree so it gets notified when a widget's property changes
	initialiseCommonAttributes(this, wData);   //initialise common attributes such as bounds, name, rotation, etc..
	setButtonText(getTextArray()[getValue()]);
	addListener(this);
	tooltipText = CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::popuptext);
	if (tooltipText.isNotEmpty())
		setTooltip(tooltipText);

	setClickingTogglesState(true);

	setToggleState((bool)getValue(), dontSendNotification);


	setImgProperties(*this, wData, "buttonon");
	setImgProperties(*this, wData, "buttonoff");
	setImgProperties(*this, wData, "buttonover");

	const String imgOff = CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::imgbuttonoff);
	const String imgOver = CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::imgbuttonover);
	const String imgOn = CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::imgbuttonon);

	setLookAndFeelColours(wData);

	getProperties().set("outlinecolour", CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::outlinecolour));
	getProperties().set("outlinethickness", CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::outlinethickness));
	getProperties().set("corners", CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::corners));

	if (owner->globalStyle == "legacy")
	{
		return;
	}



	//if users are passing custom images, use old style look and feel
	if (CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::style) == "flat" &&
		imgOff.isEmpty() && imgOn.isEmpty() && imgOver.isEmpty())
	{
		int fontstyle = CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::fontstyle);
		owner->customFont.setStyleFlags(fontstyle);
		flatLookAndFeel.customFont = owner->customFont;
		setLookAndFeel(&flatLookAndFeel);
		lookAndFeelChanged();
	}
	else {
		lookAndFeel.customFont = owner->customFont;
		setLookAndFeel(&lookAndFeel);
		lookAndFeelChanged();
	}


}

void CabbageUnlockButton::buttonClicked(Button*)
{
	DialogWindow::LaunchOptions o;
	auto content = std::make_unique<UnlockForm>();
	content->setSize(500, 550);
	o.content.setOwned(content.release());
	o.dialogTitle = TRANS("Unlock / Register");
	o.dialogBackgroundColour = o.content->getLookAndFeel().findColour(ResizableWindow::backgroundColourId);
	o.escapeKeyTriggersCloseButton = true;
	o.useNativeTitleBar = true;
	o.resizable = false;
	o.launchAsync();
}

void CabbageUnlockButton::setLookAndFeelColours(ValueTree wData)
{
	setColour(TextButton::textColourOffId, Colour::fromString(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::fontcolour)));
	setColour(TextButton::buttonColourId, Colour::fromString(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::colour)));
	setColour(TextButton::textColourOnId, Colour::fromString(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::onfontcolour)));
	setColour(TextButton::buttonOnColourId, Colour::fromString(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::oncolour)));
	getProperties().set("outlinecolour", CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::outlinecolour));
	getProperties().set("outlinethickness", CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::outlinethickness));
	getProperties().set("corners", CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::corners));
	repaint();
}

void CabbageUnlockButton::valueTreePropertyChanged(ValueTree& valueTree, const Identifier& prop)
{

		setLookAndFeelColours(valueTree);
		handleCommonUpdates(this, valueTree, false, prop);      //handle comon updates such as bounds, alpha, rotation, visible, etc
		populateTextArrays(valueTree);
		setButtonText(getTextArray()[getValue()]);
		setTooltip(getCurrentPopupText(valueTree));
}



//===============================================================================


