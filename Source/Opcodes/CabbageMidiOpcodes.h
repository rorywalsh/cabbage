#pragma once


#include "../CabbageCommonHeaders.h"
#include "../Widgets/CabbageWidgetData.h"
#undef _CR
#include <plugin.h>

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

    bool hasStopped = false;
    int status[1024];
    int channel[1024];
    int noteNumber[1024];
    int velocity[1024];
    int skipTime = 0;
};

struct CabbageMidiFileInfo : csnd::InPlug<1>
{
    int init();
};


class MidiNotes
{
public:
    struct NoteInfo {
        int note;
        int channel;
        int vel;
    };

    NoteInfo notes[128];
    int count = 0;
};

struct CabbageMidiSender : csnd::Plugin<0, 0>
{
    int init();
    int deinit();

    MidiNotes** notes = nullptr;
};

struct CabbageMidiListener : csnd::Plugin<4, 1>
{
    int init();
    int kperf();
    int getMidiInfo();


    MidiNotes** notes = nullptr;
};


