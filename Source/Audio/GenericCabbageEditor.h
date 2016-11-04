/*
 ==============================================================================

 This file is part of the JUCE library.
 Copyright (c) 2015 - ROLI Ltd.

 Permission is granted to use this software under the terms of either:
 a) the GPL v2 (or any later version)
 b) the Affero GPL v3

 Details of these licenses can be found at: www.gnu.org/licenses

 JUCE is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

 ------------------------------------------------------------------------------

 To release a closed-source product which uses JUCE, commercial licenses are
 available: visit www.juce.com for more information.

 Reappropriated for Cabbage, RW.
 ==============================================================================
 */

#include "../JuceLibraryCode/JuceHeader.h"
#include "../CabbageGenericPluginLookAndFeel.h"

class GenericCabbagePluginProcessor;

class GenericCabbageEditor : public AudioProcessorEditor,
                      public Slider::Listener,
                      public Button::Listener//, // [11]
                      //private Timer
{
public:
    enum
    {
        kParamControlHeight = 30,
        kParamLabelWidth = 60,
        kParamSliderWidth = 300
    };


    GenericCabbageEditor (AudioProcessor& parent);
    ~GenericCabbageEditor();

    void resized() override;

    void paint (Graphics& g) override
    {
        //g.setColour (Colour(58, 110, 182));
        g.setColour(Colours::black.brighter(.4f));
		g.fillRect (getLocalBounds());
    }

    //==============================================================================
    void sliderValueChanged (Slider* slider) override
    {
        if (AudioParameterFloat* param = getParameterForSlider (slider))
            *param = (float) slider->getValue();
    }

    void sliderDragStarted (Slider* slider) override
    {
        if (AudioParameterFloat* param = getParameterForSlider (slider))
            param->beginChangeGesture();
    }

    void sliderDragEnded (Slider* slider) override
    {
        if (AudioParameterFloat* param = getParameterForSlider (slider))
            param->endChangeGesture();
    }
    
    //==============================================================================
    
    void buttonClicked (Button* button) override
    {
        if (AudioParameterBool* param = getParameterForButton (button))
        {
            param->beginChangeGesture();
            *param = button->getToggleState();
            param->endChangeGesture();
        }
    }

private:
//    void timerCallback() override
//    {
//        const OwnedArray<AudioProcessorParameter>& params = getAudioProcessor()->getParameters();
//        
//        for (int i = 0; i < controls.size(); ++i)
//        {
//            if (Slider* slider = dynamic_cast<Slider*> (controls[i]))
//            {
//                AudioParameterFloat* param = static_cast<AudioParameterFloat*> (params[i]);
//                slider->setValue ((double) *param, dontSendNotification);
//            }
//            else if (Button* button = dynamic_cast<Button*> (controls[i]))
//            {
//                AudioParameterBool* param = static_cast<AudioParameterBool*> (params[i]);
//                button->setToggleState (*param, dontSendNotification);
//            }
//        }
//    }

    AudioParameterFloat* getParameterForSlider (Slider* slider)
    {
        const OwnedArray<AudioProcessorParameter>& params = getAudioProcessor()->getParameters();
        return dynamic_cast<AudioParameterFloat*> (params[controls.indexOf (slider)]); // [12]
    }
    
    AudioParameterBool* getParameterForButton (Button* button)
    {
        const OwnedArray<AudioProcessorParameter>& params = getAudioProcessor()->getParameters();
        return dynamic_cast<AudioParameterBool*> (params[controls.indexOf (button)]);
    }
    
    
    Label noParameterLabel;
    OwnedArray<Slider> paramSliders;
    OwnedArray<Label> paramLabels;
    OwnedArray<Button> paramToggles;  // [8]
    Array<Component*> controls;       // [9]
	CabbageGenericPluginLookAndFeel lookAndFeel;
};
