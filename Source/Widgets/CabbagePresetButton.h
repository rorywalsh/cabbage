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

#ifndef CabbagePresetButton_H_INCLUDED
#define CabbagePresetButton_H_INCLUDED

#include "../CabbageCommonHeaders.h"
#include "CabbageWidgetBase.h"
#include "../LookAndFeel/FlatButtonLookAndFeel.h"
#include "../LookAndFeel/CabbageIDELookAndFeel.h"
class CabbagePluginEditor;

class CabbagePresetButton : public TextButton, public ValueTree::Listener, public CabbageWidgetBase, public Button::Listener
{
    CabbagePluginEditor* owner;
    String mode, filetype, tooltipText;
    StringArray fullPresetList;
    String currentPresetDir;
    bool replaceTextWithPreset = false;
    File currentPreset;
	int readonly = 0;


	struct DirAndExt
	{
		String folder;
		String extension; // C++11 allows the extra comma
	};

	DirAndExt user;
	DirAndExt factory;

public:

    CabbagePresetButton (ValueTree wData, CabbagePluginEditor* owner);
    ~CabbagePresetButton() override {
        setLookAndFeel(nullptr); 
        widgetData.removeListener(this);
    }

    
    
    //ValueTree::Listener virtual methods....
    void valueTreePropertyChanged (ValueTree& valueTree, const Identifier&) override;
    void valueTreeChildAdded (ValueTree&, ValueTree&)override {}
    void valueTreeChildRemoved (ValueTree&, ValueTree&, int) override {}
    void valueTreeChildOrderChanged (ValueTree&, int, int) override {}
    void valueTreeParentChanged (ValueTree&) override {}
    String returnValidPath (File path);
    void setLookAndFeelColours (ValueTree wData);
    PopupMenu addPresetsToMenu(String custom);

    void buttonClicked (Button* button)  override;
    ValueTree widgetData;


    FlatButtonLookAndFeel flatLookAndFeel;


    struct PopupMenuLookAndFeel : public LookAndFeel_V4
        {
            void drawPopupMenuColumnSeparatorWithOptions (Graphics& g,
                                                          const Rectangle<int>& bounds,
                                                          const PopupMenu::Options& opt)
            {
                if (auto* target = opt.getTargetComponent())
                {
                    const auto baseColour = Colours::green;//target->findColour (TextButton::buttonColourId);
                    g.setColour (baseColour.brighter (0.4f));

                    const float dashes[] { 5.0f, 5.0f };
                    const auto centre = bounds.toFloat().getCentre();

                    g.drawDashedLine ({ centre.withY ((float) bounds.getY()),
                                        centre.withY ((float) bounds.getBottom()) },
                                      dashes,
                                      numElementsInArray (dashes),
                                      3.0f);
                }
            }

            void drawPopupMenuBackgroundWithOptions (Graphics& g, int, int, const PopupMenu::Options& opt)
            {
                g.fillAll (findColour (PopupMenu::ColourIds::backgroundColourId));
            }

            // Return the amount of space that should be left between popup menu columns.
            int getPopupMenuColumnSeparatorWidthWithOptions (const PopupMenu::Options&)
            {
                return 10;
            }
            
            void drawPopupMenuItem (Graphics& g, const Rectangle<int>& area,
                                                           const bool isSeparator, const bool isActive,
                                                           const bool isHighlighted, const bool isTicked,
                                                           const bool hasSubMenu, const String& text,
                                                           const String& shortcutKeyText,
                                                           const Drawable* icon, const Colour* const textColourToUse)
            {
                if (isSeparator)
                {
                    Rectangle<int> r (area.reduced (5, 0));
                    r.removeFromTop (r.getHeight() / 2 - 1);

                    g.setColour (Colour (0x33000000));
                    g.fillRect (r.removeFromTop (1));

                    g.setColour (Colour (0x66ffffff));
                    g.fillRect (r.removeFromTop (1));
                }
                else
                {
                    Colour textColour (findColour (PopupMenu::ColourIds::textColourId));

                    if (textColourToUse != nullptr)
                        textColour = *textColourToUse;

                    Rectangle<int> r (area.reduced (1));

                    if (isHighlighted)
                    {
                        g.setColour (findColour (PopupMenu::ColourIds::highlightedBackgroundColourId));
                        g.fillRect (r);

                        g.setColour (findColour (PopupMenu::ColourIds::highlightedTextColourId));
                    }
                    else
                    {
                        g.setColour (textColour);
                    }

                    if (! isActive)
                        g.setOpacity (0.3f);

                    Font font (getPopupMenuFont());

                    const float maxFontHeight = area.getHeight() / 1.4f;

                    if (font.getHeight() > maxFontHeight)
                        font.setHeight (maxFontHeight);

                    g.setFont (font);

                    Rectangle<float> iconArea (r.removeFromLeft ((r.getHeight() * 3/*5*/) / 4).reduced (3).toFloat());

                    if (icon != nullptr)
                    {
                        icon->drawWithin (g, iconArea, RectanglePlacement::centred | RectanglePlacement::onlyReduceInSize, 1.0f);
                    }
                    else if (isTicked)
                    {
                        const Path tick (getTickShape (1.0f));
                        g.fillPath (tick, tick.getTransformToScaleToFit (iconArea, true));
                    }

                    if (hasSubMenu)
                    {
                        const float arrowH = 0.6f * getPopupMenuFont().getAscent();

                        const float x = (float) r.removeFromRight ((int) arrowH).getX();
                        const float halfH = (float) r.getCentreY();

                        Path p;
                        p.addTriangle (x, halfH - arrowH * 0.5f,
                                       x, halfH + arrowH * 0.5f,
                                       x + arrowH * 0.6f, halfH);

                        g.fillPath (p);
                    }

                    r.removeFromRight (3);
                    g.drawFittedText (text, r, Justification::centredLeft, 1);

                    if (shortcutKeyText.isNotEmpty())
                    {
                        Font f2 (font);
                        f2.setHeight (f2.getHeight() * 0.75f);
                        f2.setHorizontalScale (0.95f);
                        g.setFont (f2);

                        g.drawText (shortcutKeyText, r, Justification::centredRight, true);
                    }
                }
            }
        };
    
    PopupMenuLookAndFeel laf;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CabbagePresetButton)
};



#endif  // CabbagePresetButton_H_INCLUDED
