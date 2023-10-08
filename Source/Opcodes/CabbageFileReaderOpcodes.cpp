//
//  CabbageMIDIOpcode.cpp
//  Cabbage
//
//  Created by Rory on 09/02/2023.
//

#include "CabbageFileReaderOpcodes.h"


int CabbageFileLoader::init()
{
    
    if(!File::getCurrentWorkingDirectory().getChildFile(args.str_data(0).data).existsAsFile()){
        csound->init_error("Could not open audio file. Please make sure you provide a full path\n");
        return NOTOK;
    }
    
    fileBuffers = (FileBuffers**)csound->query_global_variable("cabbageFileBuffers");
    FileBuffers* fileBuffer;

    if (fileBuffers != nullptr)
    {
        fileBuffer = *fileBuffers;
    }
    else
    {
        csound->create_global_variable("cabbageFileBuffers", sizeof(FileBuffers*));
        fileBuffers = (FileBuffers**)csound->query_global_variable("cabbageFileBuffers");
        *fileBuffers = new FileBuffers();
        fileBuffer = *fileBuffers;
    }
    


    resample = std::make_unique<LagrangeInterpolator>();
  
    for(auto& f : fileBuffer->buffers)
    {
        if(f.name == String(args.str_data(0).data))
            return OK;
    }
    
    fileBuffer->buffers.push_back( { String(args.str_data(0).data) });
    int currentIndex = fileBuffer->buffers.size()-1;
    juce::AudioFormatManager formatManager;
    formatManager.registerBasicFormats();
    auto* reader = formatManager.createReaderFor(File::getCurrentWorkingDirectory().getChildFile(args.str_data(0).data));
    
    if (reader != nullptr)//don't bother trying to read file unless it's valid...
    {
        fileBuffer->buffers[currentIndex].buffer.setSize((int) reader->numChannels, (int) reader->lengthInSamples);
        reader->read(&fileBuffer->buffers[currentIndex].buffer, 0, (int) reader->lengthInSamples, 0, true, true);
        resampleBuffer((double)reader->sampleRate/(double)csound->sr(), fileBuffer->buffers[currentIndex].buffer, fileBuffer->buffers[currentIndex].buffer.getNumChannels());
        delete reader;
    }


    return OK;
}


void CabbageFileLoader::resampleBuffer(double ratio, AudioBuffer<float>& buffer, int numChannels)
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

int CabbageFileReader::init()
{
    
    if(in_count()!=4)
        csound->init_error("Not enough input arguments\n");
    
    fileBuffers = (FileBuffers**)csound->query_global_variable("cabbageFileBuffers");
    FileBuffers* fileBuffer;

    if (fileBuffers != nullptr)
    {
        fileBuffer = *fileBuffers;
    }
    else
    {
        csound->create_global_variable("cabbageFileBuffers", sizeof(FileBuffers*));
        fileBuffers = (FileBuffers**)csound->query_global_variable("cabbageFileBuffers");
        *fileBuffers = new FileBuffers();
        fileBuffer = *fileBuffers;
    }
    
    for(int i = 0 ; i < 16 ; i++)
            sampleIndex[i] = skipTime*csound->sr();
    
    for(auto & f : fileBuffer->buffers)
    {
        if(f.name == inargs.str_data(0).data)
        {
            sampleBuffer = f.buffer;
            return OK;
        }
    }
    
    csound->init_error("Could not find file, make sure you loaded it with cabbageFileLoader..\n");
    return NOTOK;
    
}

int CabbageFileReader::aperf()
{
    
    loopMode = static_cast<int>(inargs[3]);
    skipTime = static_cast<int>(inargs[2]);
    
    playbackRate = inargs[1];

    for (int channel = 0; channel < sampleBuffer.getNumChannels(); ++channel)
    {
        auto* channelSamples = sampleBuffer.getReadPointer (channel);
        // auto* channelData = buffer.getWritePointer (channel);
        csnd::AudioSig out(this, outargs(channel));
        for( int i = 0 ; i < out.GetNsmps() ; i++, sampleIndex[channel]+=playbackRate)
        {
            if(!loopEnded)
            {
                out[i] = channelSamples[int(sampleIndex[channel])];
                
                if((int)sampleIndex[channel]>=sampleBuffer.getNumSamples())
                {
                    sampleIndex[channel] = 0;
                    if(loopMode == 0)
                        loopEnded = true;
                }
            }
            else
                out[i] = 0;
        }
        
        if(sampleBuffer.getNumChannels() == 1)
        {
            csnd::AudioSig outR(this, outargs(1));
            std::copy(out.begin(), out.end(), outR.begin());
        }
    }
    
    
    
    return OK;
}
    
