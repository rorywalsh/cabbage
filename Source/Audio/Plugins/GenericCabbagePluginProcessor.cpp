/*
  ==============================================================================

    GenericCabbagePluginProcessor.cpp
    Created: 21 Oct 2016 10:50:44am
    Author:  rory

  ==============================================================================
*/

#include "GenericCabbagePluginProcessor.h"
#include "GenericCabbageEditor.h"

GenericCabbagePluginProcessor::GenericCabbagePluginProcessor (File inputFile, const BusesProperties ioBuses)
    : CsoundPluginProcessor (inputFile, ioBuses)
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
    if(csoundChanList)
        getEngine()->DeleteChannelList (csoundChanList);
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
    auto& params = getParameters();

    for (int i = 0; i < params.size(); i++)
    {
        if (AudioParameterFloat* param = dynamic_cast<AudioParameterFloat*> (params[i]))
            getEngine()->SetChannel (param->name.toUTF8(), *param);
    }
}

