/*
  ==============================================================================

    GenericCabbagePluginProcessor.cpp
    Created: 21 Oct 2016 10:50:44am
    Author:  rory

  ==============================================================================
*/

#include "GenericCabbagePluginProcessor.h"
#include "GenericCabbageEditor.h"

GenericCabbagePluginProcessor::GenericCabbagePluginProcessor (File inputFile, const int ins, const int outs)
    : CsoundPluginProcessor (inputFile, ins, outs)
{
    setupAndCompileCsound (inputFile, inputFile.getParentDirectory());
    this->setPluginName (inputFile.getFileNameWithoutExtension());
    csoundChanList = NULL;
    int numberOfChannels = csoundListChannels (getCsoundStruct(), &csoundChanList);

    for (int i = 0; i < numberOfChannels; i++ )
    {
        const float min = csoundChanList[i].hints.min;
        const float max = (csoundChanList[i].hints.max ==  0 ? 1 : csoundChanList[i].hints.max);
        const float defaultValue = csoundChanList[i].hints.dflt;
        const String channel = csoundChanList[i].name;
        NormalisableRange<float> range (min, max);
        addParameter (new AudioParameterFloat (channel, channel, range, defaultValue));
    }

}

GenericCabbagePluginProcessor::~GenericCabbagePluginProcessor()
{
    getCsound()->DeleteChannelList (csoundChanList);
}

//==============================================================================
bool GenericCabbagePluginProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* GenericCabbagePluginProcessor::createEditor()
{
    return new GenericCabbageEditor (*this);
}

void GenericCabbagePluginProcessor::sendChannelDataToCsound()
{
    const OwnedArray<AudioProcessorParameter>& params = getParameters();

    for (int i = 0; i < params.size(); i++)
    {
        if (AudioParameterFloat* param = dynamic_cast<AudioParameterFloat*> (params[i]))
            getCsound()->SetChannel (param->name.toUTF8(), *param);
    }
}

