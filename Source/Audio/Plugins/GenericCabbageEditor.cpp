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

#include "GenericCabbageEditor.h"
#include "GenericCabbagePluginProcessor.h"

GenericCabbageEditor::GenericCabbageEditor(AudioProcessor& parent)
    : AudioProcessorEditor (parent), lookAndFeel(),
      noParameterLabel ("noparam", "No Csound channels found in orchestra. If you wish to have real time control of parameters please delcare channels using the chn, or chnexport opecodes.")
{

    setLookAndFeel(&lookAndFeel);
    const OwnedArray<AudioProcessorParameter>& params = parent.getParameters();

    for (int i = 0; i < params.size(); ++i)
    {
        if (const AudioParameterFloat* param = dynamic_cast<AudioParameterFloat*> (params[i]))
        {
            Slider* aSlider;
            paramSliders.add (aSlider = new Slider (param->name));
            aSlider->setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxRight, false, 100, 25);
            aSlider->setRange (param->range.start, param->range.end);
            aSlider->setSliderStyle (Slider::LinearHorizontal);
            aSlider->setValue (*param);
            aSlider->setColour(Slider::ColourIds::rotarySliderFillColourId, Colour(221, 147,0));
            aSlider->addListener (this);
            addAndMakeVisible (aSlider);

            Label* aLabel;
            paramLabels.add (aLabel = new Label (param->name, param->name));
            addAndMakeVisible (aLabel);

            controls.add (aSlider);  // [10]
        }
        else if (const AudioParameterBool* param = dynamic_cast<AudioParameterBool*> (params[i])) // [11]
        {
            ToggleButton* aButton;

            paramToggles.add (aButton = new ToggleButton (param->name));
            aButton->setToggleState (*param, dontSendNotification);

            aButton->addListener (this);
            addAndMakeVisible (aButton);

            controls.add (aButton);
        }
    }

    noParameterLabel.setJustificationType (Justification::horizontallyCentred | Justification::verticallyCentred);
    noParameterLabel.setFont (noParameterLabel.getFont().withStyle (Font::italic));

    setSize (kParamSliderWidth + kParamLabelWidth,
             jmax (100, kParamControlHeight * controls.size()));

    if (paramSliders.size() == 0)
        addAndMakeVisible (noParameterLabel);

}

GenericCabbageEditor::~GenericCabbageEditor()
{

}

void GenericCabbageEditor::resized()
{
    Rectangle<int> r = getLocalBounds();
    noParameterLabel.setBounds (r);

    for (int i = 0; i < controls.size(); ++i)
    {
        Rectangle<int> paramBounds = r.removeFromTop (kParamControlHeight);

        if (Slider* aSlider = dynamic_cast<Slider*> (controls[i]))
        {
            Rectangle<int> labelBounds = paramBounds.removeFromLeft (kParamLabelWidth);

            const int sliderIndex = paramSliders.indexOf (aSlider);
            paramLabels[sliderIndex]->setBounds (labelBounds);
            aSlider->setBounds (paramBounds);
        }
        else if (ToggleButton* aButton = dynamic_cast<ToggleButton*> (controls[i]))
        {
            aButton->setBounds (paramBounds);
        }
    }
}
