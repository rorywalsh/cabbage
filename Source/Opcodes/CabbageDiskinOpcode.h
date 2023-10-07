#pragma once


#include "../CabbageCommonHeaders.h"
#include "../Widgets/CabbageWidgetData.h"
#undef _CR
#include <plugin.h>

#include "JuceHeader.h"


struct CabbageDiskin : csnd::Plugin<2, 4>
{
    AudioBuffer<float> audioBuffer;
    File audioFile = {};
    int init();
    int aperf();
    float playbackRate = 1;
    int loopMode = 0;
    int skipTime = 0;
    float sampleIndex[16] = {0};
    bool loopEnded = false;
    std::unique_ptr<LagrangeInterpolator> resample;

    void resampleBuffer(double ratio, AudioBuffer<float>& buffer, int numChannels);
};





