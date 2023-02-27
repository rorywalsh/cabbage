//
//  CabbageMIDIOpcode.h
//  Cabbage
//
//  Created by Rory on 09/02/2023.
//

#ifndef CabbageMIDIOpcode_h
#define CabbageMIDIOpcode_h

#ifndef WIN32

#include <plugin.h>
#include "../CabbageCommonHeaders.h"
#include "../Widgets/CabbageWidgetData.h"
#include "JuceHeader.h"
#include <string>
#include <fstream>
//====================================================================================================
// ReadStateData
//====================================================================================================

struct CabbageMidiReader : csnd::Plugin<6, 5>
{
    int init();
    int kperf();
    juce::MidiFile midiFile;
    int currentTrack = 1;
    double sampleIndex = 0;
    double nextStartTime = -1.0;
    double startTime;
    int getStatusType(juce::MidiMessage mess);

    bool hasStopped = false;
    int status[1024];
    int channel[1024];
    int noteNumber[1024];
    int velocity[1024];
    int skipTime = 0;
};

struct CabbageMidiInfo : csnd::InPlug<1>
{
    int init();
};
#endif

#endif /* CabbageMIDIOpcode_h */
