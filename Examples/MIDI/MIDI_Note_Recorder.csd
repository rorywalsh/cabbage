
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; MIDI Note Recorder

; This midi instrument is intended to be used as a plugin.
; It should be patched in between something that produces MIDI notes (i.e. a keyboard) -
; - and something that plays notes.

; Notes are recorded after 'Record' is pressed. Recording only actually begins once the first note is played.
; Stop record by pressing the 'Stop' button or the 'Play' button.

; Up to 8 buffers can be recorded to or played from.

; The playback speed can be changed using the Speed Button.
; Speed changes are only implemented before playback begins.

; Notes can also be transposed in semitone steps.
; Velocities can be scaled to alter the dynamics of a phrase.

; The row of notes can be transformed using retrograde and shuffling (the rhythm remains intact)
; The original row can be restored using 'Restore'.

; Buffers are stored only while the plugin is active.

; Only records and play on MIDI channel 1

; Does not record controllers.

<Cabbage>
form caption("MIDI Note Recorder") size(445,110), pluginId("MRec")
image                    bounds(  0,  0,445,110), colour(0,0,0)
button   bounds( 10, 10, 60, 20), fontColour:0(50,50,50), fontColour:1(255,205,205), colour:0(10,0,0), colour:1(150,0,0), text("RECORD","RECORD"), channel("Record"), latched(1), radioGroup(1)
button   bounds( 10, 40, 60, 20), fontColour:0(50,50,50), fontColour:1(205,255,205), colour:0(0,10,0), colour:1(0,150,0), text("PLAY","PLAY"), channel("Play"), latched(1), radioGroup(1)
button   bounds( 10, 70, 60, 20), fontColour:0(50,50,50), fontColour:1(205,205,255), colour:0(0,0,10), colour:1(0,0,150), text("STOP","STOP"), channel("Stop"), latched(1), radioGroup(1), value(1)

label    bounds( 80, 10, 50, 15), text("Buffer");, fontColour("white")
combobox bounds( 80, 28, 50, 20), text("1","2","3","6","5","6","7","8"), value(1), channel("Buffer")

rslider  bounds(140,  5, 60, 90), channel("Speed"), text("Speed"), range(0.125,8,1,0.5,0.001), textBox(1)
rslider  bounds(210,  5, 60, 90), channel("Transpose"), text("Transpose"), range(-24,24,0,1,1), textBox(1)
rslider  bounds(280,  5, 60, 90), channel("Velocity"), text("Velocity"), range(0.5,2,1,0.5,0.01), textBox(1)

button   bounds(350, 10, 90, 20), fontColour:0(50,50,50), fontColour:1(255,205,255), colour:0(10,0,10), colour:1(150,0,150), text("RETROGRADE","RETROGRADE"), channel("Retrograde"), latched(0)
button   bounds(350, 40, 90, 20), fontColour:0(50,50,50), fontColour:1(205,255,255), colour:0(0,10,10), colour:1(0,150,150), text("SHUFFLE","SHUFFLE"), channel("Shuffle"), latched(0)
button   bounds(350, 70, 90, 20), fontColour:0(50,50,50), fontColour:1(255,255,205), colour:0(10,10,0), colour:1(150,150,0), text("RESTORE","RESTORE"), channel("Restore"), latched(0)

</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-dm0 -n -+rtmidi=NULL -Q0 -M0 --midi-key-cps=4 --midi-velocity-amp=5
</CsOptions>

<CsInstruments>

; sr set by host
ksmps  = 8
nchnls = 1
0dbfs  = 1

massign    0,10

giNBuffers    =    8            ; number of buffers for note storage
giNEvents    =    10000        ; number of event storage locations in each buffer. Divide by 2 for the number of notes that can be stored.

giMIDIChan    =    1            ; midi channel

; stores note data (5000 notes)
gkData[][][] init giNBuffers,giNEvents, 4            ; array for storage of notes while recording
gkSavedData[][][] init giNBuffers,giNEvents, 4        ; array for storage of original note if a speed change has been made
gkSeqLen[]    init    giNBuffers                        ; length of each buffer after recording (in events)


; UDO that sends note offs for all possible notes (useful to prevent hanging/stuck notes)
opcode    AllNotesOff,0,i
 iMIDIChan    xin                    ; read in input arguments
 kNote    =    0                    ; set for lowest possible note
 while kNote<=127 do            ; while kNote is still less than maximum possible MIDI note...
  midiout 128,iMIDIChan,kNote,0    ; send a note off for this note
  kNote    +=    1                    ; increment kNote read to turn of the note a semitone higher
 od
endop

instr   1
 ; read in  widgets
 gkRecord    chnget    "Record"
 gkPlay        chnget    "Play"
 gkSpeed    chnget    "Speed"
 gkBuffer    chnget    "Buffer"
 gkTranspose    chnget    "Transpose"
 gkVelocity    chnget    "Velocity"
 gkRetrograde    chnget    "Retrograde"
 gkShuffle    chnget    "Shuffle"
 gkRestore    chnget    "Restore"
 
; trigger when button value goes from off to on
 kOnTrig trigger gkRecord,0.5,0
; start instrument with a held note when trigger received
 schedkwhen      kOnTrig,0,0,2,0,-1
; trigger when button value goes from off to on
 kOnTrig trigger gkPlay,0.5,0
; start instrument with a held note when trigger received
 schedkwhen      kOnTrig,0,0,3,0,-1
 
 ; Retrograde
 if trigger:k(gkRetrograde,0.5,0)==1 then
  gkSavedData[][][]    =    gkData        ; backup recording
  kNdx        =    0
  kSeqLen    =    gkSeqLen[gkBuffer-1]            ; length of sequence
  while kNdx<kSeqLen do
   kNote        =    gkSavedData[gkBuffer-1][kSeqLen - 1 - kNdx][2]
   kVel        =    gkSavedData[gkBuffer-1][kSeqLen - 1 - kNdx][3]
   gkData[gkBuffer-1][kNdx][2]    =    kNote
   gkData[gkBuffer-1][kNdx][3]    =    kVel
   kNdx    +=    1
  od
 endif

 ; Shuffle
 if trigger:k(gkShuffle,0.5,0)==1 then
  gkData[][][]    =    gkSavedData        ; backup recording
  kNdx        =    0
  kSeqLen    =    gkSeqLen[gkBuffer-1]            ; length of sequence
  while kNdx<kSeqLen do
   kNote1    =    gkData[gkBuffer-1][kNdx][2]
   kVel1        =    gkData[gkBuffer-1][kNdx][3]
   kRndNdx    random    0,kSeqLen-0.001
   kNote2    =    gkData[gkBuffer-1][kRndNdx][2]
   kVel2        =    gkData[gkBuffer-1][kRndNdx][3]
   gkData[gkBuffer-1][kNdx][2]        =    kNote2
   gkData[gkBuffer-1][kNdx][3]        =    kVel2
   gkData[gkBuffer-1][kRndNdx][2]    =    kNote1
   gkData[gkBuffer-1][kRndNdx][3]    =    kVel1
   kNdx     +=    1
  od
 endif

 ; Restore
 if trigger:k(gkRestore,0.5,0)==1 then
  gkData[][][]    =    gkSavedData        ; backup recording 
 endif

 ; MIDI Thru
 gkStatus,gkChan,gkData1,gkData2    midiin
                                     midiout gkStatus,gkChan,gkData1,gkData2


endin

instr   2       ; Record gesture

 gkNdx    init    0                 ; event index
 gkTime    init    0                ; timer (counts k-cycles). Timer only starts once the first note has been played.
 kTrig    init    0                ; trig which indicates when the first note has been played. 0 before first note has been played, 1 as soon as the first note has been played
 
 if gkStatus!=0 && kTrig==0 then        ; sense first note
  kTrig    =    1                            ; flip trigger to 1
 endif
  
 if kTrig==1 then        ; if the first note has been played
  gkTime    +=    1            ; increment the k-rate counter
 endif
   
 if gkRecord==0 then                ; If record button is deactivated...
  gkSeqLen[gkBuffer-1]    =    gkNdx    ; recording is completed, write the recording length (in events) into the array that stores recording lengths
  gkSavedData[][][]    =    gkData        ; backup recording
  turnoff                           ; ...turn this instrument off.
 endif
 
endin

instr   3       ; Playback recorded gesture
 kNdx        init    0                         ; event index
 kSeqLen    =        gkSeqLen[gkBuffer-1]    ; length of the sequence. Used to sense when to stop.

 kTimer        init    0

 kNextEventTime    =        gkData[gkBuffer-1][kNdx][0]

 if trigger:k(kTimer,kNextEventTime,0)==1 then
  kStart    =    gkData[gkBuffer-1][kNdx][0]
  kDur        =    gkData[gkBuffer-1][kNdx][1]
  kNote        =    gkData[gkBuffer-1][kNdx][2]
  kVel        =    gkData[gkBuffer-1][kNdx][3]
  event    "i",6,0,kDur/gkSpeed, limit(kNote+gkTranspose,0,127), limit(kVel*gkVelocity,0,127)
  kNdx    +=    1
 endif

 kEndTime    =        gkData[gkBuffer-1][kSeqLen-1][0] + gkData[gkBuffer-1][kSeqLen-1][1]
 if kTimer>=kEndTime then
  chnset    k(0),"Play"
  chnset    k(1),"Stop"  
 endif

 kTimer        +=    ksmps/sr * gkSpeed

 if gkPlay==0 then                     ; if play button is deactivated...
  AllNotesOff    giMIDIChan            ; call UDO that sends note offs for every possible note
   turnoff                          ; ...turn this instrument off.
 endif
 
endin

instr   4       ; save array (not used yet)

endin


instr   5       ; load array (not used yet)

endin

instr        6    ; Output instrument

 noteondur giMIDIChan, p4, p5, p3

 aSig        poscil    p5/127,cpsmidinn(p4)
 aEnv        linsegr    0,0.001,1,0.1,0
         out        aSig/10 * aEnv
 
endin

instr    10
 iNdx    =    i(gkNdx)
 gkNdx    init    iNdx+1
 iNote    notnum
 iVel    veloc    0,127
 iStart    = i(gkTime)/kr
 kDur    timeinsts
 
 if release:k()==1 then
  gkData[gkBuffer-1][iNdx][0] = iStart
  gkData[gkBuffer-1][iNdx][1] = kDur
  gkData[gkBuffer-1][iNdx][2] = iNote
  gkData[gkBuffer-1][iNdx][3] = iVel
 endif
 
 
 if release:k()==1 then
  event "i", 11, 0, 0, iStart, kDur, iNote, iVel
 endif

 ; Audition note when not in a plugin
 aSig    poscil    iVel/127*0.1,cpsmidinn(iNote)
         outs    aSig, aSig
 
endin

instr 11
 fprints "scor.orc", "i %2.0f\t%15.6f\t%15.6f\t%d\t%d\n", p1, p4, p5, p6, p7
endin

</CsInstruments>

<CsScore>
i 1 0 3600
</CsScore>

</CsoundSynthesizer>