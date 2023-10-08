#pragma once#include "../CabbageCommonHeaders.h"
#include "../Widgets/CabbageWidgetData.h"
#undef _CR
#include <plugin.h>
#include "JuceHeader.h"

struct FileBuffers
{
    struct FileBuffer {
        String name;
        float sampleIndex = 0;
        AudioBuffer<float> buffer;
        FileBuffer(String n){
            name = n;
        }
    };
    std::vector<FileBuffer> buffers;
};

struct CabbageFileLoader : csnd::InPlug<1>
{
    int init();
    std::unique_ptr<LagrangeInterpolator> resample;
    void resampleBuffer(double ratio, AudioBuffer<float>& buffer, int numChannels);
    FileBuffers** fileBuffers = nullptr;
};

struct CabbageFileReader : csnd::Plugin<2, 4>
{
    int init();
    int aperf();
    float playbackRate = 1;
    int loopMode = 0;
    int skipTime = 0;
    bool loopEnded = false;
    FileBuffers** fileBuffers = nullptr;
    AudioBuffer<float> sampleBuffer;
    float sampleIndex[16] = {0};
};





