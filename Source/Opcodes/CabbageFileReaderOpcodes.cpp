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

    
    int num_channels = vorbisInfo->channels; // Stereo audio
    long pcm_size = 1024; // Number of samples to read at a time
    float** pcm;
    
    pcm = (float**)malloc(sizeof(float*) * num_channels);
    for (int i = 0; i < num_channels; i++) {
        pcm[i] = (float*)malloc(sizeof(float) * pcm_size);
    }

    
    int bitstream;
    long samples;
    //int sampleIndex = 0;
    
    auto numSamples = ov_pcm_total(&file, -1);
    audioBuffer.setSize(num_channels, numSamples);
    auto buf = audioBuffer.getArrayOfWritePointers();
    // Read audio data as float samples

    while ((samples = ov_read_float(&file, &pcm, pcm_size, &bitstream)) > 0) {
        // 'pcm' now contains 'samples' of audio data for each channel
        // You can process or save the audio samples here
        for (int i = 0; i < num_channels; i++) {
//            buf[i] = pcm[i];
            for (long j = 0; j < samples; j++) {
                auto s = (int)sampleIndex[i];
                if((int)sampleIndex[i]<numSamples-1)
                    buf[i][(int)sampleIndex[i]++] = pcm[i][j];
            }
        }
        
    }

    for( int i = 0 ; i < 16 ; i++)
        sampleIndex[i] = 0;
    return OK;
}
//
//size_t CabbageFileReader::read(void* ptr, size_t size, size_t nmemb, void* datasource) {
//    FILE* file = (FILE*)datasource;
//    //return fread(ptr, size, nmemb, file);
//}

//size_t CabbageFileReader::read(void* dst, size_t size1, size_t size2, void* fh){
//
////size_t CabbageFileReader::read(void* buffer, size_t elementSize, size_t elementCount, void* dataSource) {
//    //    assert(elementSize == 1);
//    //
//    //    std::ifstream& stream = *static_cast<std::ifstream*>(dataSource);
//    //    stream.read(static_cast<char*>(buffer), elementCount);
//    //    const std::streamsize bytesRead = stream.gcount();
//    //    stream.clear(); // In case we read past EOF
//    //    return static_cast<size_t>(bytesRead);
//    //}
//    OggVorbis_File* of = reinterpret_cast<OggVorbis_File*>(fh);
//    size_t len = size1 * size2;
//    if ( of->curPtr + len > of->filePtr + of->fileSize )
//    {
//        len = of->filePtr + of->fileSize - of->curPtr;
//    }
//    memcpy( dst, of->curPtr, len );
//    of->curPtr += len;
//    return len;
//}

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
    
    // Allocate memory for 'pcm' based on the number of channels
    // Replace 'num_channels' with the actual number of channels in your audio file

    
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

    
    
    
    return OK;
}
    
