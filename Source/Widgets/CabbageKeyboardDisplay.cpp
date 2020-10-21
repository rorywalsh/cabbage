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

#include "CabbageKeyboardDisplay.h"

CabbageKeyboardDisplay::CabbageKeyboardDisplay(ValueTree wData)
	:
	MidiKeyboardDisplay(MidiKeyboardDisplay::horizontalKeyboard),
    scrollbars(CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::scrollbars)),
    keyWidth(CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::keywidth)),
    widgetData(wData)
{
	setOrientation(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::kind) == "horizontal" ? MidiKeyboardDisplay::horizontalKeyboard : MidiKeyboardDisplay::verticalKeyboardFacingRight);
	setName(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::name));
	widgetData.addListener(this);              //add listener to valueTree so it gets notified when a widget's property changes
	initialiseCommonAttributes(this, wData);   //initialise common attributes such as bounds, name, rotation, etc..

	setLowestVisibleKey(CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::value));
	setOctaveForMiddleC(CabbageWidgetData::getNumProp(wData, CabbageIdentifierIds::middlec));
	setKeyWidth(keyWidth);
	setScrollButtonsVisible(scrollbars == 1 ? true : false);
    
	updateColours(wData);
    colourPressedNotes(wData);

}

void CabbageKeyboardDisplay::colourPressedNotes(ValueTree wData)
{
    var notesPressed = CabbageWidgetData::getProperty(wData, CabbageIdentifierIds::keypressed);
    Array<var>* array = notesPressed.getArray();
    Array<int> keys;
    if (array)
    {
        if (array->size()>0)
        {
            for( int i = 0 ; i < array->size() ; i++)
            {
                if(!keys.contains(int(array->getReference(i))))
                   keys.add(int(array->getReference(i)));
            }
            notesDown = keys;
        }
    }
    else
    {
        const int note = CabbageWidgetData::getProperty(wData, CabbageIdentifierIds::keypressed);
        notesDown = Array<int>(note);
    }
    
    
    updateKeys();
}
void CabbageKeyboardDisplay::valueTreePropertyChanged(ValueTree& valueTree, const Identifier& prop)
{
	setOrientation(CabbageWidgetData::getStringProp(valueTree, CabbageIdentifierIds::kind) == "horizontal" ? MidiKeyboardDisplay::horizontalKeyboard : MidiKeyboardDisplay::verticalKeyboardFacingRight);


    
	updateColours(valueTree);
	handleCommonUpdates(this, valueTree);      //handle comon updates such as bounds, alpha, rotation, visible, etc
    
    colourPressedNotes(valueTree);

}


void CabbageKeyboardDisplay::updateColours(ValueTree& wData)
{
	setColour(MidiKeyboardComponent::whiteNoteColourId, Colour::fromString(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::whitenotecolour)));
	setColour(MidiKeyboardComponent::blackNoteColourId, Colour::fromString(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::blacknotecolour)));
	setColour(MidiKeyboardComponent::upDownButtonArrowColourId, Colour::fromString(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::arrowcolour)));
	setColour(MidiKeyboardComponent::upDownButtonBackgroundColourId, Colour::fromString(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::arrowbackgroundcolour)));
	setColour(MidiKeyboardComponent::keySeparatorLineColourId, Colour::fromString(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::keyseparatorcolour)));
	setColour(MidiKeyboardComponent::mouseOverKeyOverlayColourId, Colour::fromString(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::mouseoverkeycolour)));
	setColour(MidiKeyboardComponent::keyDownOverlayColourId, Colour::fromString(CabbageWidgetData::getStringProp(wData, CabbageIdentifierIds::keydowncolour)));
}

static const uint8 whiteNotes[] = { 0, 2, 4, 5, 7, 9, 11 };
static const uint8 blackNotes[] = { 1, 3, 6, 8, 10 };


struct MidiKeyboardDisplay::UpDownButton : public Button
{
	UpDownButton(MidiKeyboardDisplay& c, int d)
		: Button({}), owner(c), delta(d)
	{
	}

	void clicked() override
	{
		auto note = owner.getLowestVisibleKey();

		if (delta < 0)
			note = (note - 1) / 12;
		else
			note = note / 12 + 1;

		owner.setLowestVisibleKey(note * 12);
	}

	void paintButton(Graphics& g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override
	{
		owner.drawUpDownButton(g, getWidth(), getHeight(),
			shouldDrawButtonAsHighlighted, shouldDrawButtonAsDown,
			delta > 0);
	}

private:
	MidiKeyboardDisplay & owner;
	const int delta;

	JUCE_DECLARE_NON_COPYABLE(UpDownButton)
};

//==============================================================================
MidiKeyboardDisplay::MidiKeyboardDisplay(Orientation o)
	: orientation(o)
{
	scrollDown.reset(new UpDownButton(*this, -1));
	scrollUp.reset(new UpDownButton(*this, 1));

	addChildComponent(scrollDown.get());
	addChildComponent(scrollUp.get());


	mouseOverNotes.insertMultiple(0, -1, 32);
	mouseDownNotes.insertMultiple(0, -1, 32);

	colourChanged();
	setWantsKeyboardFocus(true);

}

MidiKeyboardDisplay::~MidiKeyboardDisplay()
{
	//state.removeListener(this);
}

//==============================================================================
void MidiKeyboardDisplay::setKeyWidth(float widthInPixels)
{
	jassert(widthInPixels > 0);

	if (keyWidth != widthInPixels) // Prevent infinite recursion if the width is being computed in a 'resized()' call-back
	{
		keyWidth = widthInPixels;
		resized();
	}
}

void MidiKeyboardDisplay::setScrollButtonWidth(int widthInPixels)
{
	jassert(widthInPixels > 0);

	if (scrollButtonWidth != widthInPixels)
	{
		scrollButtonWidth = widthInPixels;
		resized();
	}
}

void MidiKeyboardDisplay::setOrientation(Orientation newOrientation)
{
	if (orientation != newOrientation)
	{
		orientation = newOrientation;
		resized();
	}
}

void MidiKeyboardDisplay::setAvailableRange(int lowestNote, int highestNote)
{
	jassert(lowestNote >= 0 && lowestNote <= 127);
	jassert(highestNote >= 0 && highestNote <= 127);
	jassert(lowestNote <= highestNote);

	if (rangeStart != lowestNote || rangeEnd != highestNote)
	{
		rangeStart = jlimit(0, 127, lowestNote);
		rangeEnd = jlimit(0, 127, highestNote);
		firstKey = jlimit((float)rangeStart, (float)rangeEnd, firstKey);
		resized();
	}
}

void MidiKeyboardDisplay::setLowestVisibleKey(int noteNumber)
{
	setLowestVisibleKeyFloat((float)noteNumber);
}

void MidiKeyboardDisplay::setLowestVisibleKeyFloat(float noteNumber)
{
	noteNumber = jlimit((float)rangeStart, (float)rangeEnd, noteNumber);

	if (noteNumber != firstKey)
	{
		bool hasMoved = (((int)firstKey) != (int)noteNumber);
		firstKey = noteNumber;

		if (hasMoved)
			sendChangeMessage();

		resized();
	}
}

void MidiKeyboardDisplay::setScrollButtonsVisible(bool newCanScroll)
{
	if (canScroll != newCanScroll)
	{
		canScroll = newCanScroll;
		resized();
	}
}

void MidiKeyboardDisplay::colourChanged()
{
	setOpaque(findColour(whiteNoteColourId).isOpaque());
	repaint();
}

//==============================================================================
void MidiKeyboardDisplay::setMidiChannel(int midiChannelNumber)
{
	jassert(midiChannelNumber > 0 && midiChannelNumber <= 16);

	if (midiChannel != midiChannelNumber)
	{
		resetAnyKeysInUse();
		midiChannel = jlimit(1, 16, midiChannelNumber);
	}
}

void MidiKeyboardDisplay::setMidiChannelsToDisplay(int midiChannelMask)
{
	midiInChannelMask = midiChannelMask;
	shouldCheckState = true;
}

void MidiKeyboardDisplay::setVelocity(float v, bool useMousePosition)
{
	velocity = jlimit(0.0f, 1.0f, v);
	useMousePositionForVelocity = useMousePosition;
}

//==============================================================================
Range<float> MidiKeyboardDisplay::getKeyPosition(int midiNoteNumber, float targetKeyWidth) const
{
	jassert(midiNoteNumber >= 0 && midiNoteNumber < 128);

	static const float notePos[] = { 0.0f, 1 - blackNoteWidthRatio * 0.6f,
		1.0f, 2 - blackNoteWidthRatio * 0.4f,
		2.0f,
		3.0f, 4 - blackNoteWidthRatio * 0.7f,
		4.0f, 5 - blackNoteWidthRatio * 0.5f,
		5.0f, 6 - blackNoteWidthRatio * 0.3f,
		6.0f };

	auto octave = midiNoteNumber / 12;
	auto note = midiNoteNumber % 12;

	auto start = octave * 7.0f * targetKeyWidth + notePos[note] * targetKeyWidth;
	auto width = MidiMessage::isMidiNoteBlack(note) ? blackNoteWidthRatio * targetKeyWidth : targetKeyWidth;

	return { start, start + width };
}

Range<float> MidiKeyboardDisplay::getKeyPos(int midiNoteNumber) const
{
	return getKeyPosition(midiNoteNumber, keyWidth)
		- xOffset
		- getKeyPosition(rangeStart, keyWidth).getStart();
}

Rectangle<float> MidiKeyboardDisplay::getRectangleForKey(int note) const
{
	jassert(note >= rangeStart && note <= rangeEnd);

	auto pos = getKeyPos(note);
	auto x = pos.getStart();
	auto w = pos.getLength();

	if (MidiMessage::isMidiNoteBlack(note))
	{
		auto blackNoteLength = getBlackNoteLength();

		switch (orientation)
		{
		case horizontalKeyboard:            return { x, 0, w, blackNoteLength };
		case verticalKeyboardFacingLeft:    return { getWidth() - blackNoteLength, x, blackNoteLength, w };
		case verticalKeyboardFacingRight:   return { 0, getHeight() - x - w, blackNoteLength, w };
		default:                            jassertfalse; break;
		}
	}
	else
	{
		switch (orientation)
		{
		case horizontalKeyboard:            return { x, 0, w, (float)getHeight() };
		case verticalKeyboardFacingLeft:    return { 0, x, (float)getWidth(), w };
		case verticalKeyboardFacingRight:   return { 0, getHeight() - x - w, (float)getWidth(), w };
		default:                            jassertfalse; break;
		}
	}

	return {};
}

float MidiKeyboardDisplay::getKeyStartPosition(int midiNoteNumber) const
{
	return getKeyPos(midiNoteNumber).getStart();
}

float MidiKeyboardDisplay::getTotalKeyboardWidth() const noexcept
{
	return getKeyPos(rangeEnd).getEnd();
}

int MidiKeyboardDisplay::getNoteAtPosition(juce::Point<float> p)
{
	float v;
	return xyToNote(p, v);
}

int MidiKeyboardDisplay::xyToNote(juce::Point<float> pos, float& mousePositionVelocity)
{
	if (!reallyContains(pos.toInt(), false))
		return -1;

	auto p = pos;

	if (orientation != horizontalKeyboard)
	{
		p = { p.y, p.x };

		if (orientation == verticalKeyboardFacingLeft)
			p = { p.x, getWidth() - p.y };
		else
			p = { getHeight() - p.x, p.y };
	}

	return remappedXYToNote(p + juce::Point<float>(xOffset, 0), mousePositionVelocity);
}

int MidiKeyboardDisplay::remappedXYToNote(juce::Point<float> pos, float& mousePositionVelocity) const
{
	auto blackNoteLength = getBlackNoteLength();

	if (pos.getY() < blackNoteLength)
	{
		for (int octaveStart = 12 * (rangeStart / 12); octaveStart <= rangeEnd; octaveStart += 12)
		{
			for (int i = 0; i < 5; ++i)
			{
				auto note = octaveStart + blackNotes[i];

				if (note >= rangeStart && note <= rangeEnd)
				{
					if (getKeyPos(note).contains(pos.x - xOffset))
					{
						mousePositionVelocity = jmax(0.0f, pos.y / blackNoteLength);
						return note;
					}
				}
			}
		}
	}

	for (int octaveStart = 12 * (rangeStart / 12); octaveStart <= rangeEnd; octaveStart += 12)
	{
		for (int i = 0; i < 7; ++i)
		{
			auto note = octaveStart + whiteNotes[i];

			if (note >= rangeStart && note <= rangeEnd)
			{
				if (getKeyPos(note).contains(pos.x - xOffset))
				{
					auto whiteNoteLength = (orientation == horizontalKeyboard) ? getHeight() : getWidth();
					mousePositionVelocity = jmax(0.0f, pos.y / (float)whiteNoteLength);
					return note;
				}
			}
		}
	}

	mousePositionVelocity = 0;
	return -1;
}

//==============================================================================
void MidiKeyboardDisplay::repaintNote(int noteNum)
{
	if (noteNum >= rangeStart && noteNum <= rangeEnd)
		repaint(getRectangleForKey(noteNum).getSmallestIntegerContainer());
}

void MidiKeyboardDisplay::paint(Graphics& g)
{
	g.fillAll(findColour(whiteNoteColourId));

	auto lineColour = findColour(keySeparatorLineColourId);
	auto textColour = findColour(textLabelColourId);

	for (int octave = 0; octave < 128; octave += 12)
	{
		for (int white = 0; white < 7; ++white)
		{
			auto noteNum = octave + whiteNotes[white];
			if (noteNum >= rangeStart && noteNum <= rangeEnd)
				drawWhiteNote(noteNum, g, getRectangleForKey(noteNum),
					false,
					notesDown.contains(noteNum), lineColour, textColour);
		}
	}

	float x1 = 0.0f, y1 = 0.0f, x2 = 0.0f, y2 = 0.0f;
	auto width = getWidth();
	auto height = getHeight();

	if (orientation == verticalKeyboardFacingLeft)
	{
		x1 = width - 1.0f;
		x2 = width - 5.0f;
	}
	else if (orientation == verticalKeyboardFacingRight)
		x2 = 5.0f;
	else
		y2 = 5.0f;

	auto x = getKeyPos(rangeEnd).getEnd();
	auto shadowCol = findColour(shadowColourId);

	if (!shadowCol.isTransparent())
	{
		g.setGradientFill(ColourGradient(shadowCol, x1, y1, shadowCol.withAlpha(0.0f), x2, y2, false));

		switch (orientation)
		{
		case horizontalKeyboard:            g.fillRect(0.0f, 0.0f, x, 5.0f); break;
		case verticalKeyboardFacingLeft:    g.fillRect(width - 5.0f, 0.0f, 5.0f, x); break;
		case verticalKeyboardFacingRight:   g.fillRect(0.0f, 0.0f, 5.0f, x); break;
		default: break;
		}
	}

	if (!lineColour.isTransparent())
	{
		g.setColour(lineColour);

		switch (orientation)
		{
		case horizontalKeyboard:            g.fillRect(0.0f, height - 1.0f, x, 1.0f); break;
		case verticalKeyboardFacingLeft:    g.fillRect(0.0f, 0.0f, 1.0f, x); break;
		case verticalKeyboardFacingRight:   g.fillRect(width - 1.0f, 0.0f, 1.0f, x); break;
		default: break;
		}
	}

	auto blackNoteColour = findColour(blackNoteColourId);

	for (int octave = 0; octave < 128; octave += 12)
	{
		for (int black = 0; black < 5; ++black)
		{
			auto noteNum = octave + blackNotes[black];

			if (noteNum >= rangeStart && noteNum <= rangeEnd)
				drawBlackNote(noteNum, g, getRectangleForKey(noteNum),
					false,
					notesDown.contains(noteNum), blackNoteColour);
		}
	}
}

void MidiKeyboardDisplay::drawWhiteNote(int midiNoteNumber, Graphics& g, Rectangle<float> area,
	bool isDown, bool isOver, Colour lineColour, Colour textColour)
{
	auto c = Colours::transparentWhite;

	if (isDown)  c = findColour(keyDownOverlayColourId);
	if (isOver)  c = c.overlaidWith(findColour(mouseOverKeyOverlayColourId));

	g.setColour(c);
	g.fillRect(area);

	auto text = getWhiteNoteText(midiNoteNumber);

	if (text.isNotEmpty())
	{
		auto fontHeight = jmin(12.0f, keyWidth * 0.9f);

		g.setColour(textColour);
		g.setFont(Font(fontHeight).withHorizontalScale(0.8f));

		switch (orientation)
		{
		case horizontalKeyboard:            g.drawText(text, area.withTrimmedLeft(1.0f).withTrimmedBottom(2.0f), Justification::centredBottom, false); break;
		case verticalKeyboardFacingLeft:    g.drawText(text, area.reduced(2.0f), Justification::centredLeft, false); break;
		case verticalKeyboardFacingRight:   g.drawText(text, area.reduced(2.0f), Justification::centredRight, false); break;
		default: break;
		}
	}

	if (!lineColour.isTransparent())
	{
		g.setColour(lineColour);

		switch (orientation)
		{
		case horizontalKeyboard:            g.fillRect(area.withWidth(1.0f)); break;
		case verticalKeyboardFacingLeft:    g.fillRect(area.withHeight(1.0f)); break;
		case verticalKeyboardFacingRight:   g.fillRect(area.removeFromBottom(1.0f)); break;
		default: break;
		}

		if (midiNoteNumber == rangeEnd)
		{
			switch (orientation)
			{
			case horizontalKeyboard:            g.fillRect(area.expanded(1.0f, 0).removeFromRight(1.0f)); break;
			case verticalKeyboardFacingLeft:    g.fillRect(area.expanded(0, 1.0f).removeFromBottom(1.0f)); break;
			case verticalKeyboardFacingRight:   g.fillRect(area.expanded(0, 1.0f).removeFromTop(1.0f)); break;
			default: break;
			}
		}
	}
}

void MidiKeyboardDisplay::drawBlackNote(int /*midiNoteNumber*/, Graphics& g, Rectangle<float> area,
	bool isDown, bool isOver, Colour noteFillColour)
{
	auto c = noteFillColour;

	if (isDown)  c = c.overlaidWith(findColour(keyDownOverlayColourId));
	if (isOver)  c = c.overlaidWith(findColour(mouseOverKeyOverlayColourId));

	g.setColour(c);
	g.fillRect(area);

	if (isDown)
	{
		g.setColour(noteFillColour);
		g.drawRect(area);
	}
	else
	{
		g.setColour(c.brighter());
		auto sideIndent = 1.0f / 8.0f;
		auto topIndent = 7.0f / 8.0f;
		auto w = area.getWidth();
		auto h = area.getHeight();

		switch (orientation)
		{
		case horizontalKeyboard:            g.fillRect(area.reduced(w * sideIndent, 0).removeFromTop(h * topIndent)); break;
		case verticalKeyboardFacingLeft:    g.fillRect(area.reduced(0, h * sideIndent).removeFromRight(w * topIndent)); break;
		case verticalKeyboardFacingRight:   g.fillRect(area.reduced(0, h * sideIndent).removeFromLeft(w * topIndent)); break;
		default: break;
		}
	}
}

void MidiKeyboardDisplay::setOctaveForMiddleC(int octaveNum)
{
	octaveNumForMiddleC = octaveNum;
	repaint();
}

String MidiKeyboardDisplay::getWhiteNoteText(int midiNoteNumber)
{
	if (midiNoteNumber % 12 == 0)
		return MidiMessage::getMidiNoteName(midiNoteNumber, true, true, octaveNumForMiddleC);

	return {};
}

void MidiKeyboardDisplay::drawUpDownButton(Graphics& g, int w, int h,
	bool mouseOver,
	bool buttonDown,
	bool movesOctavesUp)
{
	g.fillAll(findColour(upDownButtonBackgroundColourId));

	float angle = 0;

	switch (orientation)
	{
	case horizontalKeyboard:            angle = movesOctavesUp ? 0.0f : 0.5f;  break;
	case verticalKeyboardFacingLeft:    angle = movesOctavesUp ? 0.25f : 0.75f; break;
	case verticalKeyboardFacingRight:   angle = movesOctavesUp ? 0.75f : 0.25f; break;
	default:                            jassertfalse; break;
	}

	Path path;
	path.addTriangle(0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.5f);
	path.applyTransform(AffineTransform::rotation(MathConstants<float>::twoPi * angle, 0.5f, 0.5f));

	g.setColour(findColour(upDownButtonArrowColourId)
		.withAlpha(buttonDown ? 1.0f : (mouseOver ? 0.6f : 0.4f)));

	g.fillPath(path, path.getTransformToScaleToFit(1.0f, 1.0f, w - 2.0f, h - 2.0f, true));
}

void MidiKeyboardDisplay::setBlackNoteLengthProportion(float ratio) noexcept
{
	jassert(ratio >= 0.0f && ratio <= 1.0f);

	if (blackNoteLengthRatio != ratio)
	{
		blackNoteLengthRatio = ratio;
		resized();
	}
}

float MidiKeyboardDisplay::getBlackNoteLength() const noexcept
{
	auto whiteNoteLength = orientation == horizontalKeyboard ? getHeight() : getWidth();
	return whiteNoteLength * blackNoteLengthRatio;
}

void MidiKeyboardDisplay::setBlackNoteWidthProportion(float ratio) noexcept
{
	jassert(ratio >= 0.0f && ratio <= 1.0f);

	if (blackNoteWidthRatio != ratio)
	{
		blackNoteWidthRatio = ratio;
		resized();
	}
}

void MidiKeyboardDisplay::resized()
{
	auto w = getWidth();
	auto h = getHeight();

	if (w > 0 && h > 0)
	{
		if (orientation != horizontalKeyboard)
			std::swap(w, h);

		auto kx2 = getKeyPos(rangeEnd).getEnd();

		if ((int)firstKey != rangeStart)
		{
			auto kx1 = getKeyPos(rangeStart).getStart();

			if (kx2 - kx1 <= w)
			{
				firstKey = (float)rangeStart;
				sendChangeMessage();
				repaint();
			}
		}

		scrollDown->setVisible(canScroll && firstKey > (float)rangeStart);

		xOffset = 0;

		if (canScroll)
		{
			auto scrollButtonW = jmin(scrollButtonWidth, w / 2);
			auto r = getLocalBounds();

			if (orientation == horizontalKeyboard)
			{
				scrollDown->setBounds(r.removeFromLeft(scrollButtonW));
				scrollUp->setBounds(r.removeFromRight(scrollButtonW));
			}
			else if (orientation == verticalKeyboardFacingLeft)
			{
				scrollDown->setBounds(r.removeFromTop(scrollButtonW));
				scrollUp->setBounds(r.removeFromBottom(scrollButtonW));
			}
			else
			{
				scrollDown->setBounds(r.removeFromBottom(scrollButtonW));
				scrollUp->setBounds(r.removeFromTop(scrollButtonW));
			}

			auto endOfLastKey = getKeyPos(rangeEnd).getEnd();

			float mousePositionVelocity;
			auto spaceAvailable = w;
			auto lastStartKey = remappedXYToNote({ endOfLastKey - spaceAvailable, 0 }, mousePositionVelocity) + 1;

			if (lastStartKey >= 0 && ((int)firstKey) > lastStartKey)
			{
				firstKey = (float)jlimit(rangeStart, rangeEnd, lastStartKey);
				sendChangeMessage();
			}

			xOffset = getKeyPos((int)firstKey).getStart();
		}
		else
		{
			firstKey = (float)rangeStart;
		}

		scrollUp->setVisible(canScroll && getKeyPos(rangeEnd).getStart() > w);
		repaint();
	}
}

//==============================================================================
void MidiKeyboardDisplay::handleNoteOn(MidiKeyboardState*, int /*midiChannel*/, int /*midiNoteNumber*/, float /*velocity*/)
{
	shouldCheckState = true; // (probably being called from the audio thread, so avoid blocking in here)
}

void MidiKeyboardDisplay::handleNoteOff(MidiKeyboardState*, int /*midiChannel*/, int /*midiNoteNumber*/, float /*velocity*/)
{
	shouldCheckState = true; // (probably being called from the audio thread, so avoid blocking in here)
}

//==============================================================================
void MidiKeyboardDisplay::resetAnyKeysInUse()
{
	if (!keysPressed.isZero())
	{
		//for (int i = 128; --i >= 0;)
		//	if (keysPressed[i])
		//		state.noteOff(midiChannel, i, 0.0f);

		keysPressed.clear();
	}

	for (int i = mouseDownNotes.size(); --i >= 0;)
	{
		auto noteDown = mouseDownNotes.getUnchecked(i);

		if (noteDown >= 0)
		{
			/*state.noteOff(midiChannel, noteDown, 0.0f);*/
			mouseDownNotes.set(i, -1);
		}

		mouseOverNotes.set(i, -1);
	}
}

void MidiKeyboardDisplay::updateNoteUnderMouse(const MouseEvent& e, bool isDown)
{
	updateNoteUnderMouse(e.getEventRelativeTo(this).position, isDown, e.source.getIndex());
}

void MidiKeyboardDisplay::updateNoteUnderMouse(juce::Point<float> pos, bool isDown, int fingerNum)
{
	float mousePositionVelocity = 0.0f;
	auto newNote = xyToNote(pos, mousePositionVelocity);
	auto oldNote = mouseOverNotes.getUnchecked(fingerNum);
	auto oldNoteDown = mouseDownNotes.getUnchecked(fingerNum);


	if (oldNote != newNote)
	{
		repaintNote(oldNote);
		repaintNote(newNote);
		mouseOverNotes.set(fingerNum, newNote);
	}

	if (isDown)
	{
		if (newNote != oldNoteDown)
		{
			if (oldNoteDown >= 0)
			{
				mouseDownNotes.set(fingerNum, -1);

				//if (!mouseDownNotes.contains(oldNoteDown))
				//	state.noteOff(midiChannel, oldNoteDown, eventVelocity);
			}

			if (newNote >= 0 && !mouseDownNotes.contains(newNote))
			{
				//state.noteOn(midiChannel, newNote, eventVelocity);
				mouseDownNotes.set(fingerNum, newNote);
			}
		}
	}
	else if (oldNoteDown >= 0)
	{
		mouseDownNotes.set(fingerNum, -1);

		//if (!mouseDownNotes.contains(oldNoteDown))
		//	state.noteOff(midiChannel, oldNoteDown, eventVelocity);
	}
}

void MidiKeyboardDisplay::mouseMove(const MouseEvent& e)
{
	updateNoteUnderMouse(e, false);
	shouldCheckMousePos = false;
}

void MidiKeyboardDisplay::mouseDrag(const MouseEvent& e)
{
	float mousePositionVelocity;
	auto newNote = xyToNote(e.position, mousePositionVelocity);

	if (newNote >= 0)
		mouseDraggedToKey(newNote, e);

	updateNoteUnderMouse(e, true);
}

bool MidiKeyboardDisplay::mouseDownOnKey(int, const MouseEvent&) { return true; }
void MidiKeyboardDisplay::mouseDraggedToKey(int, const MouseEvent&) {}
void MidiKeyboardDisplay::mouseUpOnKey(int, const MouseEvent&) {}

void MidiKeyboardDisplay::mouseDown(const MouseEvent& e)
{
	float mousePositionVelocity;
	auto newNote = xyToNote(e.position, mousePositionVelocity);

	if (newNote >= 0 && mouseDownOnKey(newNote, e))
	{
		updateNoteUnderMouse(e, true);
		shouldCheckMousePos = true;
	}
}

void MidiKeyboardDisplay::mouseUp(const MouseEvent& e)
{
	updateNoteUnderMouse(e, false);
	shouldCheckMousePos = false;

	float mousePositionVelocity;
	auto note = xyToNote(e.position, mousePositionVelocity);

	if (note >= 0)
		mouseUpOnKey(note, e);
}

void MidiKeyboardDisplay::mouseEnter(const MouseEvent& e)
{
	updateNoteUnderMouse(e, false);
}

void MidiKeyboardDisplay::mouseExit(const MouseEvent& e)
{
	updateNoteUnderMouse(e, false);
}

void MidiKeyboardDisplay::mouseWheelMove(const MouseEvent&, const MouseWheelDetails& wheel)
{
	auto amount = (orientation == horizontalKeyboard && wheel.deltaX != 0)
		? wheel.deltaX : (orientation == verticalKeyboardFacingLeft ? wheel.deltaY
			: -wheel.deltaY);

	setLowestVisibleKeyFloat(firstKey - amount * keyWidth);
}


void MidiKeyboardDisplay::updateKeys()
{
    bool isOn = false;
    for (int i = rangeStart; i <= rangeEnd; ++i)
    {
        
        isOn = notesDown.contains(i);

        if (keysCurrentlyDrawnDown[i] != isOn)
        {
            keysCurrentlyDrawnDown.setBit(i, isOn);
            repaintNote(i);
        }
    }
}


void MidiKeyboardDisplay::focusLost(FocusChangeType)
{
	resetAnyKeysInUse();
}

