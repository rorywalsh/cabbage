//
//  CabbageMIDIOpcode.h
//  Cabbage
//
//  Created by Rory on 09/02/2023.
//

#ifndef CabbageMIDIOpcode_h
#define CabbageMIDIOpcode_h


#include <plugin.h>
#include <string>
 // #include <iomanip>
#include <fstream>
// #include <iostream>
#include "json.hpp"
// #include <algorithm>
#include "../CabbageCommonHeaders.h"
using json = nlohmann::json;

#include "../Widgets/CabbageWidgetData.h"
#include "JuceHeader.h"

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
    void stopAllNotes();
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
#endif /* CabbageMIDIOpcode_h */
