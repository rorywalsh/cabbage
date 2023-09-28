//
//  CabbageMIDIOpcode.cpp
//  Cabbage
//
//  Created by Rory on 09/02/2023.
//

#include "CabbageMidiOpcodes.h"


int CabbageMidiReader::init()
{
    currentTrack = 0;
    sampleIndex = 0;
    nextStartTime = -1.0;
    startTime = 0;
    skipTime = 0;
    
    if (in_count() < 4)
    {
        csound->init_error("Not enough input arguments\n");
        return NOTOK;
    }
    else if (in_count() == 7)
    {
        skipTime = inargs[6];
    }
            
    
    currentTrack = static_cast<int>(inargs[1]);


    if(!File::getCurrentWorkingDirectory().getChildFile(inargs.str_data(0).data).existsAsFile()){
        csound->init_error("Could not open MIDI file. Please make sure you provide a full path\n");
        return NOTOK;
    }
    
    juce::FileInputStream midiStream(File::getCurrentWorkingDirectory().getChildFile(inargs.str_data(0).data));
    midiFile.readFrom(midiStream, true);
    midiFile.convertTimestampTicksToSeconds();
    lastTimeStamp = midiFile.getLastTimestamp();
    
    csnd::Vector<MYFLT>& statusOut = outargs.myfltvec_data(0);
    csnd::Vector<MYFLT>& channelOut = outargs.myfltvec_data(1);
    csnd::Vector<MYFLT>& noteNumberOut = outargs.myfltvec_data(2);
    csnd::Vector<MYFLT>& velocityOut = outargs.myfltvec_data(3);
    statusOut.init(csound, 1024);
    channelOut.init(csound, 1024);
    noteNumberOut.init(csound, 1024);
    velocityOut.init(csound, 1024);
    
    return OK;
}



int CabbageMidiReader::kperf()
{
    
    if(in_count()<4)
    {
        csound->init_error("Not enough input arguments\n");
        return NOTOK;
    }

    if(currentTrack>midiFile.getNumTracks()-1)
    {
        csound->perf_error("Your track index is greater than the number of MIDI tracks in the curent MIDI file.\n", this);
        return NOTOK;
    }
   
    if(inargs[5] == 1)
        sampleIndex = 0;
    
    bool isPlaying = static_cast<bool>(inargs[2]);
    shouldLoop = inargs[3];
    double playBackSpeed = inargs[4];
    int numEvents = 0;
    
    csnd::Vector<MYFLT>& statusOut = outargs.myfltvec_data(0);
    csnd::Vector<MYFLT>& channelOut = outargs.myfltvec_data(1);
    csnd::Vector<MYFLT>& noteNumberOut = outargs.myfltvec_data(2);
    csnd::Vector<MYFLT>& velocityOut = outargs.myfltvec_data(3);

    outargs[5] = 0;
    int noteOnEvents = 0;
    
    if (isPlaying)
    {
        hasStopped = false;
        const juce::MidiMessageSequence *theSequence = midiFile.getTrack(currentTrack);
        
        if(theSequence == nullptr)
        {
            csound->init_error("There was a problem reading events from track..\n");
            return NOTOK;
        }


        noteOnEvents = 0;

        startTime = (sampleIndex)/sr() + skipTime;
        if (startTime > lastTimeStamp * playBackSpeed && shouldLoop)
        {
            sampleIndex = 0;
            startTime = 0 + skipTime;
        }
        else
            sampleIndex+=ksmps();
        
        double endTime = startTime + (ksmps() / sr());

        if (theSequence->getNumEvents() > 0)
        {
            for (auto p = 0; p < theSequence->getNumEvents(); p++)
            {
                MidiMessageSequence::MidiEventHolder* event = theSequence->getEventPointer(p);
                
                if (event->message.getTimeStamp() * playBackSpeed >= startTime &&
                    event->message.getTimeStamp() * playBackSpeed < endTime)
                {
                    status[numEvents] = getStatusType(event->message);
                    channel[numEvents] = event->message.getChannel();
                    noteNumber[numEvents] = event->message.getNoteNumber();
                    velocity[numEvents] = event->message.getVelocity();
                    numEvents++;
                    outargs[5] = 1;
                    
                }
            }
        }
    }
    else
    {
        //if user can stopped reading, destroy all notes
        if(hasStopped==false)
        {
            for(int e = 0 ; e < 1024 ; e++)
            {
                for(int i = 0 ; i < 128 ; i++)
                {
                    for( int c = 0 ; c < 16 ; c++)
                    {
                        status[e] = (128);
                        channel[e] = c;
                        noteNumber[e] = i;
                        velocity[e] = 0;
                    }
                }
            }
            hasStopped = true;
            outargs[5] = 1;
            numEvents = 128;
            sampleIndex = 0;
            startTime = 0;
        }
    }
    
    //update output arrays with current events
    for( size_t i = 0 ; i < numEvents ; i++)
    {
        statusOut[i] = status[i];
        channelOut[i] = channel[i];
        noteNumberOut[i] = noteNumber[i];
        velocityOut[i] = velocity[i];
    }

    outargs[4] = numEvents;


    
    return OK;
}
    

int CabbageMidiReader::getStatusType(juce::MidiMessage mess)
{
    if(mess.isNoteOn())
        return 144;
    if(mess.isNoteOff())
        return 128;
    if(mess.isAftertouch())
        return 160;
    if(mess.isController())
        return 176;
    if(mess.isProgramChange())
        return 208;
    if(mess.isPitchWheel())
        return 244;

    return 0;
}

int CabbageMidiFileInfo::init()
{
    juce::MidiFile midiFile;
    
    if(!File::getCurrentWorkingDirectory().getChildFile(args.str_data(0).data).existsAsFile()){
        csound->init_error("Could not open MIDI file. Please make sure you provide a full path\n");
        return NOTOK;
    }

    juce::FileInputStream midiStream(File::getCurrentWorkingDirectory().getChildFile(args.str_data(0).data));
    int* type = 0;
    midiFile.readFrom(midiStream, true, type);
    midiFile.convertTimestampTicksToSeconds();
    csound->message("\n***** Midi file information *****");
    csound->message("Nidi file name:\""+std::string(args.str_data(0).data)+"\"");

    if(type)
        csound->message("Midi file type:"+std::to_string(*type));
    else
        csound->message("Could not determine the type of midi file...");
    csound->message("Number of tracks:"+std::to_string(midiFile.getNumTracks()));
    csound->message("Last time stamp in seconds:"+std::to_string(midiFile.getLastTimestamp()));
    MidiMessageSequence tempos;
    midiFile.findAllTempoEvents(tempos);
    csound->message("The following tempo changes were detected:");
    for (auto p = 0; p < tempos.getNumEvents(); p++)
    {
        csound->message("\tSecondsPerQuarterNote:"+std::to_string(tempos.getEventPointer(p)->message.getTempoSecondsPerQuarterNote())+
                        " at " + std::to_string(tempos.getEventPointer(p)->message.getTimeStamp()) + " seconds");
    }

    MidiMessageSequence timeSigs;
    midiFile.findAllTimeSigEvents(timeSigs);
    csound->message("The following time signature changes were detected:");
    int num, den;
    for (auto p = 0; p < timeSigs.getNumEvents(); p++)
    {
        timeSigs.getEventPointer(p)->message.getTimeSignatureInfo(num, den);
        csound->message("\t"+std::to_string(num)+"/"+std::to_string(den)+
                        " at " + std::to_string(tempos.getEventPointer(p)->message.getTimeStamp()) + " seconds");
    }

    return OK;

}
       


int CabbageMidiListener::init()
{

    csnd::Vector<MYFLT>& midiNotes = outargs.myfltvec_data(0);
    csnd::Vector<MYFLT>& velocities = outargs.myfltvec_data(1);
    csnd::Vector<MYFLT>& channels = outargs.myfltvec_data(2);
    midiNotes.init(csound, 128);
    velocities.init(csound, 128);
    channels.init(csound, 128);

    notes = (MidiNotes**)csound->query_global_variable("cabbageMidiNotes");
    MidiNotes* noteData;

    if (notes != nullptr)
    {
        noteData = *notes;
    }
    else
    {
        csound->create_global_variable("cabbageMidiNotes", sizeof(MidiNotes*));
        notes = (MidiNotes**)csound->query_global_variable("cabbageMidiNotes");
        *notes = new MidiNotes();
        noteData = *notes;
        noteData->notes.resize(128);
    }

    noteData->count = 0;

    return OK;
}

int CabbageMidiListener::kperf()
{
    return getMidiInfo();
}

bool compareByNote(const MidiNotes::NoteInfo& a, const MidiNotes::NoteInfo& b)
{
    if (a.note == 0) 
        return false;
    if (b.note == 0) 
        return true;

    return a.note < b.note;
}

int CabbageMidiListener::getMidiInfo()
{
    int shouldSort = 0;

    if (in_count() > 1)
    {
        csound->init_error("cabbageMidiInfo takes 1 parameter..\n");
        return NOTOK;
    }

    if (in_count() == 1)
    {
        shouldSort = inargs[0];
    }


    notes = (MidiNotes**)csound->query_global_variable("cabbageMidiNotes");
    MidiNotes* noteData;

    if (notes != nullptr)
    {
        noteData = *notes;

    }
    else
    {
        csound->create_global_variable("cabbageMidiNotes", sizeof(MidiNotes*));
        notes = (MidiNotes**)csound->query_global_variable("cabbageMidiNotes");
        *notes = new MidiNotes();
        noteData = *notes;
        noteData->notes.resize(128);
    }

    outargs[3] = noteData->count;

    csnd::Vector<MYFLT>& notesOut = outargs.myfltvec_data(0);
    csnd::Vector<MYFLT>& velocitiesOut = outargs.myfltvec_data(1);
    csnd::Vector<MYFLT>& channelsOut = outargs.myfltvec_data(2);

    if (shouldSort == 1)
    {
        std::sort(noteData->notes.begin(), noteData->notes.end(), compareByNote);
    }



    for (size_t i = 0; i < 128; i++)
    {
        notesOut[i] = noteData->notes[i].note;
        velocitiesOut[i] = noteData->notes[i].vel;
        channelsOut[i] = noteData->notes[i].channel;
    }

    
    
    return OK;
}

//=============================================================================================
int CabbageMidiSender::init()
{
    if (in_count() > 0)
    {
        csound->init_error("cabbageMidiSender takes no parameters..\n");
        return NOTOK;
    }

    csound->plugin_deinit(this);

    notes = (MidiNotes**)csound->query_global_variable("cabbageMidiNotes");
    MidiNotes* noteData;
    

    if (notes != nullptr)
    {
        noteData = *notes;        
    }
    else
    {
        csound->create_global_variable("cabbageMidiNotes", sizeof(MidiNotes*));
        notes = (MidiNotes**)csound->query_global_variable("cabbageMidiNotes");
        *notes = new MidiNotes();
        noteData = *notes;
        noteData->notes.resize(128);
    }

    const int noteNumber = midi_note_num();
    const int noteChannel = midi_channel();
    const int noteVelocity = midi_note_vel();

    bool noteAlreadyRegistered = false;
    int size = noteData->notes.size();
    for (const auto& n :noteData->notes)
    {
        if (noteNumber == n.note)
            noteAlreadyRegistered = true;
    }

    if (!noteAlreadyRegistered) {
        noteData->notes[noteData->count].note = noteNumber;
        noteData->notes[noteData->count].vel = noteVelocity;
        noteData->notes[noteData->count].channel = noteChannel;
        noteData->count++;
    }

    return OK;
}


int CabbageMidiSender::deinit()
{
    notes = (MidiNotes**)csound->query_global_variable("cabbageMidiNotes");
    MidiNotes* noteData;

    if (notes != nullptr)
    {
        noteData = *notes;
    }
    else
    {
        csound->perf_error("Error - global pointer is not valid", nullptr);
    }


    noteData->count = (noteData->count > 1 ? noteData->count - 1 : 0);

    int index = -1;



    for (int i = 0; i < 128; i++)
    {
        if (noteData->notes[i].note == midi_note_num())
        {
            index = i;
            noteData->notes[i].note = 0;
            noteData->notes[i].vel = 0;
            noteData->notes[i].channel = -1;
        }
    }

    //fill gap left in array
    for (int i = index; i < 127; i++)
        noteData->notes[i] = noteData->notes[i+1];

    return OK;
}
