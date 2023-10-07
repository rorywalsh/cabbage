//
//  CabbageMIDIOpcode.cpp
//  Cabbage
//
//  Created by Rory on 09/02/2023.
//

#include "CabbageDiskinOpcode.h"


int CabbageDiskin::init()
{
    if(!File::getCurrentWorkingDirectory().getChildFile(inargs.str_data(0).data).existsAsFile()){
        csound->init_error("Could not open audio file. Please make sure you provide a full path\n");
        return NOTOK;
    }

    resample = std::make_unique<LagrangeInterpolator>();
    loopMode = static_cast<int>(inargs[3]);
    skipTime = static_cast<int>(inargs[2]);
    
    juce::AudioFormatManager formatManager;
    formatManager.registerBasicFormats();
    auto* reader = formatManager.createReaderFor(File::getCurrentWorkingDirectory().getChildFile(inargs.str_data(0).data));

    if (reader != nullptr)//don't bother trying to read file unless it's valid...
    {
        audioBuffer.setSize((int) reader->numChannels, (int) reader->lengthInSamples);
        reader->read(&audioBuffer, 0, (int) reader->lengthInSamples, 0, true, true);
        resampleBuffer((double)reader->sampleRate/(double)csound->sr(), audioBuffer, audioBuffer.getNumChannels());
        delete reader;
    }

    
    for(int i = 0 ; i < 16 ; i++)
        sampleIndex[i] = skipTime*csound->sr();

    return OK;
}


void CabbageDiskin::resampleBuffer(double ratio, AudioBuffer<float>& buffer, int numChannels)
{
    
    AudioBuffer<float> temp;
    temp.clear();//zero contents
    temp.setSize(numChannels, (int)(buffer.getNumSamples() / ratio));
    const float** inputs = buffer.getArrayOfReadPointers();
    float** outputs = temp.getArrayOfWritePointers();
    resample->process(ratio, inputs[0], outputs[0], temp.getNumSamples());
    if(numChannels == 2)
        resample->process(ratio, inputs[1], outputs[1], temp.getNumSamples());
    buffer = temp;
}

int CabbageDiskin::aperf()
{
    playbackRate = inargs[1];
    for (int channel = 0; channel < audioBuffer.getNumChannels(); ++channel)
    {
        auto* channelSamples = audioBuffer.getReadPointer (channel);
        // auto* channelData = buffer.getWritePointer (channel);
        csnd::AudioSig out(this, outargs(channel));
        for( int i = 0 ; i < out.GetNsmps() ; i++, sampleIndex[channel]+=playbackRate)
        {
            if(!loopEnded)
            {
                out[i] = channelSamples[int(sampleIndex[channel])];
                
                if((int)sampleIndex[channel]>=audioBuffer.getNumSamples())
                {
                    sampleIndex[channel] = 0;
                    if(loopMode == 0)
                        loopEnded = true;
                }
            }
        }
    }
    
    if(audioBuffer.getNumChannels() == 1)
    {
        csnd::AudioSig outL(this, outargs(0));
        csnd::AudioSig outR(this, outargs(1));
        std::copy(outL.begin(), outL.end(),
        outR.begin());
    }

    
    
    
    return OK;
}
    
