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
    
    if(in_count()<4)
    {
        csound->init_error("Not enough input arguments\n");
        return NOTOK;
    }
    else if(in_count() == 5)
        skipTime = inargs[4];
    
    currentTrack = static_cast<int>(inargs[1]);


    if(!File::getCurrentWorkingDirectory().getChildFile(inargs.str_data(0).data).existsAsFile()){
        csound->init_error("Could not open MIDI file. Please make sure you provide a full path\n");
        return NOTOK;
    }
    
    juce::FileInputStream midiStream(File::getCurrentWorkingDirectory().getChildFile(inargs.str_data(0).data));
    midiFile.readFrom(midiStream, true);
    midiFile.convertTimestampTicksToSeconds();
    
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

    
    
    bool isPlaying = static_cast<bool>(inargs[2]);
    double playBackSpeed = inargs[3];
    int numEvents = 0;
    
    csnd::Vector<MYFLT>& statusOut = outargs.myfltvec_data(0);
    csnd::Vector<MYFLT>& channelOut = outargs.myfltvec_data(1);
    csnd::Vector<MYFLT>& noteNumberOut = outargs.myfltvec_data(2);
    csnd::Vector<MYFLT>& velocityOut = outargs.myfltvec_data(3);

    outargs[5] = 0;
    
    if (isPlaying)
    {
        hasStopped = false;
        const juce::MidiMessageSequence *theSequence = midiFile.getTrack(currentTrack);
        
        if(theSequence == nullptr)
        {
            csound->init_error("There was a problem reading events\n");
            return NOTOK;
        }

        //run through all midi event in sequence, and add
        //those that are set to play within this k-cycle to
        //event vectors
        for( int i = 0 ; i < ksmps() ; i++)
        {
            startTime = (sampleIndex++)/sr() + skipTime;
            double endTime = startTime + (ksmps() / sr());

            for (auto p = 0; p < theSequence->getNumEvents(); p++)
            {
                MidiMessageSequence::MidiEventHolder *event = theSequence->getEventPointer(p);

                if (event->message.getTimeStamp()*playBackSpeed >= startTime &&
                    event->message.getTimeStamp()*playBackSpeed < endTime)
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
            for(int i = 0 ; i < 128 ; i++)
            {
                for( int c = 0 ; c < 16 ; c++)
                {
                    status[i] = (128);
                    channel[i] = c;
                    noteNumber[i] = i;
                    velocity[i] = 0;
                }
            }
            hasStopped = true;
            outargs[5] = 1;
            numEvents = 128;
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

int CabbageMidiInfo::init()
{
    juce::MidiFile midiFile;
    
    if(!File::getCurrentWorkingDirectory().getChildFile(args.str_data(0).data).existsAsFile()){
        csound->init_error("Could not open MIDI file. Please make sure you provide a full path\n");
        return NOTOK;
    }

    juce::FileInputStream midiStream(File::getCurrentWorkingDirectory().getChildFile(args.str_data(0).data));
    int* type;
    midiFile.readFrom(midiStream, true, type);
    midiFile.convertTimestampTicksToSeconds();
    csound->message("\n***** Midi file information *****");
    csound->message("Nidi file name:\""+std::string(args.str_data(0).data)+"\"");
    csound->message("Midi file type:"+std::to_string(*type));
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
       
       
 
    
