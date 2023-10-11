//
//  CabbageMIDIOpcode.cpp
//  Cabbage
//
//  Created by Rory on 09/02/2023.
//

#include "CabbageFileReaderOpcodes.h"


int CabbageFileReader::init()
{
    if(!File::getCurrentWorkingDirectory().getChildFile(inargs.str_data(0).data).existsAsFile()){
        csound->init_error("Could not open audio file. Please make sure you provide a full path\n");
        return NOTOK;
    }

    csound->plugin_deinit(this);
    resample = std::make_unique<LagrangeInterpolator>();
    loopMode = static_cast<int>(inargs[3]);
    skipTime = static_cast<int>(inargs[2]);
    loopEnded = false;
    
    std::ifstream stream;
    stream.open(inargs.str_data(0).data, std::ios::binary);
        
    const ov_callbacks callbacks{read, seek, nullptr, tell};
    int result = ov_open_callbacks(&stream, &file, nullptr, 0, callbacks);
    if (result < 0) {
        csound->init_error("Could not open ogg file. Please make sure you provide a full path\n");
        return 0;
    }

    // Read file info
    vorbis_info* vorbisInfo = ov_info(&file, -1);
//    std::cout << "File info: " << vorbisInfo->rate << "Hz, "
//        << vorbisInfo->channels << " channels" << std::endl;

    
    numChannels = vorbisInfo->channels; // Stereo audio
    long pcm_size = 1024; // Number of samples to read at a time
    float** pcm;
    
    pcm = (float**)malloc(sizeof(float*) * numChannels);
    for (int i = 0; i < numChannels; i++) {
        pcm[i] = (float*)malloc(sizeof(float) * pcm_size);
    }

    
    int bitstream;
    long samples;

    
    numSamples = ov_pcm_total(&file, -1);
    audioBuffer.clear();
    audioBuffer.setSize(numChannels, numSamples);
    ov_time_seek_lap(&file, 0);
    auto buf = audioBuffer.getArrayOfWritePointers();

    for( int i = 0 ; i < 16 ; i++)
        sampleIndex[i] = 0;
    
    while ((samples = ov_read_float(&file, &pcm, pcm_size, &bitstream)) > 0)
    {
        for (int channel = 0; channel < numChannels; channel++) {

            for (long j = 0; j < samples; j++) {
                if((int)sampleIndex[channel]<numSamples)
                    buf[channel][(int)sampleIndex[channel]++] = pcm[channel][j];
            }
        }
    }
    
    for( int i = 0 ; i < 16 ; i++)
        sampleIndex[i] = 0;
    return OK;
}

int CabbageFileReader::deinit(){
    
    ov_clear (&file);
    audioBuffer.clear();
    
    return OK;
}

void CabbageFileReader::resampleBuffer(double ratio, AudioBuffer<float>& buffer, int numChannels)
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

int CabbageFileReader::aperf()
{
    playbackRate = inargs[1];
    for (int channel = 0; channel < numChannels; ++channel)
    {
        auto channelSamples = audioBuffer.getArrayOfWritePointers();
        // auto* channelData = buffer.getWritePointer (channel);
        csnd::AudioSig out(this, outargs(channel));
        for( int i = 0 ; i < out.GetNsmps() ; i++, sampleIndex[channel]+=playbackRate)
        {
            if(!loopEnded)
            {
                auto s = int(sampleIndex[channel]);
                out[i] = channelSamples[channel][int(sampleIndex[channel])];
                if((int)sampleIndex[channel]>=numSamples)
                {
                    sampleIndex[channel] = 0;
                    if(loopMode == 0)
                        loopEnded = true;
                }
            }
            else{
                out[i] = 0;
            }
        }
    }

    
    
    
    return OK;
}
    
