
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; Micropolyphony.csd
; Written by Iain McCurdy, 2016

; The main intention behind this instrument was to explore the techniques of tone clusters and micropolyphony, 
;  - as used by composers such as György Ligeti and Krzysztof Penderecki.

; Note that on account of integer rounding, wgbow may not always sound 'in tune' across the length of the keyboard. 
;  Other synth engines are offered which won't present this problem 

; CONTROLS
; --------

; [MONO/POLY]
; Leg.Time    -    time it will take to gliss 1 semitone
; Smear        -    when using multiple voices, the glissandos will move in and out of sync if this control is above zero
;                this will also affect realtime changed made to 'Spread'

; [MICROPOLYPHONY]
; N.Voices  -    number of voices
; Spread    -    interval between adjacent voices
;                What pitches are created depends upon the value of spread, the number of voices and whether an odd or even number of voices has been chosen.
;                If an odd number of voices is selected, then a central static pitch which ignores the 'Spread' value will be created.
;                If the number of voices is even then this central static pitch will not be created
;                For example, if 'N.Voices'=5 and 'Spread'=1 then semitone transpositions for the 5 voices will be: 0, +1, -1, +2 and -2
;                 if if 'N.Voices'=2 and 'Spread'=1 then the transpositions will be +1 and -1
; Scale        -    scale expressed by the voices.
;                when 'Chromatic' is selected spacing is defined purely by the spread control
;                other scales can be expressed but in order to hear them as described, spread should be set to '1'
; Synth        -    choice from four different synthesis engines:
;                 Bowed string, sine tone, bandpass filtered sine tone, gbuzz 'brass' sound.

; [KEYBOARD]
; Interval    -    number of semitones interval between each key
; Shift        -    transpose shift of all notes in semitones

; [AMPLITUDE ENVELOPE] this envelope is applied to the opcode amplitude input, it is not a simple audio gain envelope 
;                            and instead affects tone in addition to other characteristics.
; Attack    -    attack time
; Release    -    release time after the note has been released
; Amount    -    sustain level of the envelope. 
;                 Gain compensations are made according to the setting of this control so that it doesn't really control amplitude level, 
;                 instead it controls tone quality.

; [FILTER] a lowpass filter. 
;            Cutoff is expressed using an arbitrary value, as the actual cutoff can be either a static value -
;            - or a key-following value (or a mixture of both approaches)
; Cutoff    -    cutoff setting
; Follow    -    amount of key following/tracking to use. 0 = none/i.e. static value 1=completely track key played

; [STEREO]
; Mode        -    choose from one of three modes for the placement of each voice:
;                'Alternate' - voices are alternately sent to the left or right channels (Voice 1 central, Voice 2 right, Voice 3 left etc...)
;                'Rnd.Fixed' - voices are randomly located in the stereo field. During a note, pan locations remain fixed. New locations are generated for each new note.
;                'Rnd.Moving' - voices moving between left and right in random fashion (using an rspline)
; Width        -    stereo width

; [BOW POSITION] Bow position is expressed as a ratio along the length of the string, 
;                  0.5 will represent halfway along the string's length (flautando)
;                 and close to 1 or close to zero will represent playing close to the bridge or nut (i.e. sul pointicello). 
;                 Zero or 1 will result in no sound.
;                  Bow position can be randomly modulated between two values.
; Min        -    minimum value in the random modulation
; Max        -    maximum value in the random modulation
; Movement    -    rate/frequency of the random modulation

; [BOW PRESSURE]    Bow pressure will also affect note attack and whether or not it sounds at all
;                     If bow pressure is low, higher pitches may not sound
;                     Low bow pressures will tend to extend the attack of the note.
;                     Very high bow pressures will also prevent high notes from sounding.
;                     Bow pressure can be randomly modulated between two values.
; Min        -    minimum value in the random modulation
; Max        -    maximum value in the random modulation
; Movement    -    rate/frequency of the random modulation

; [VIBRATO] both a tonal and a pitch vibrato.
; Depth        -    depth of vibrato
; Rate        -    rate of the vibrato. This is randomly modulated subtly so that when multiple voices are used, 
;                 the vibrato modulations do no remain fixed in sync.
; Shape        -    shape of the vibrato function: 'sine' or 'random'

; [REVERB] screverb is used 

; [PRESETS] some 'factory' presets to demonstrate some of the sounds possible

; Level        -    output level

<Cabbage>
#define COLOUR  colour(172,127,100)
#define COLOUR2 colour(60,40,40)
#define TRACKER trackerColour("yellow")

form caption("Micropolyphony") size(1110,335), pluginId("MPSy"), colour("black")

;MONO/POLY
image   bounds(  5,  6,210,115), colour(0,0,0,0), shape("rounded"), outlineColour("white"), outlineThickness(2), plant("polyphony") {
label   bounds(  0,  5,210, 15), text("MONO/POLY"),  fontColour(white)
button  bounds( 10, 50, 60, 25), text("poly","mono"), channel("monopoly"), value(1), fontColour:0(200,200,50), fontColour:1(200,200,50)
rslider bounds( 70, 20, 80, 80), text("Leg.Time"),    channel("LegTim"), range(0.001,20, 0.8, 0.5, 0.001), valueTextBox(1), textBox(1), $TRACKER, fontColour("white"), textColour("white")
rslider bounds(130, 20, 80, 80), text("Smear"),    channel("PortSmear"), range(0,     8, 3,   0.5, 0.001), valueTextBox(1), textBox(1), $TRACKER, fontColour("white"), textColour("white")
}       

;VOICES
image   bounds(220,  6,305,115), colour(0,0,0,0), shape("rounded"), outlineColour("white"), outlineThickness(2), plant("Voices") {
label   bounds(  0,  5,305, 15), text("VOICES"),  fontColour(white)
rslider bounds(  5, 20, 80, 80), text("N.Voices"),    channel("voices"), range(1,51,18,1,1), valueTextBox(1), textBox(1), $TRACKER, fontColour("white"), textColour("white")
rslider bounds( 65, 20, 80, 80), text("Spread"),    channel("spread"), range(0, 24, 0.5, 1, 0.01), valueTextBox(1), textBox(1), $TRACKER, fontColour("white"), textColour("white")
label    bounds(135, 35, 80, 13), text("Scale"), fontColour("white")                     
combobox bounds(135, 50, 80, 20), text("Chromatic","Major","Minor","Pentatonic"), channel("VScale"), fontColour(200,200,50)
label    bounds(220, 35, 80, 13), text("Synth"), fontColour("white") 
combobox bounds(220, 50, 80, 20), text("Bow","Pure Tone","Noisy Tone","Brass"), channel("Synth"), fontColour(200,200,50)

}

;KEYBOARD
image   bounds(530,  6,145,115), colour(0,0,0,0), shape("rounded"), outlineColour("white"), outlineThickness(2), plant("Keyboard") {
label   bounds(  0,  5,145, 15), text("KEYBOARD"),  fontColour(white)
rslider bounds(  5, 20, 80, 80), text("Interval"), channel("KIntvl"), range(0, 1,1,1,0.01), valueTextBox(1), textBox(1), $TRACKER, fontColour("white"), textColour("white") 
rslider bounds( 65, 20, 80, 80), text("Shift"),    channel("KShift"), range(-127,127, 0, 1, 1), valueTextBox(1), textBox(1), $TRACKER, fontColour("white"), textColour("white")
}

;AMP ENVELOPE
image   bounds(680,  6,210,115), colour(0,0,0,0), shape("rounded"), outlineColour("white"), outlineThickness(2), plant("AmpEnv") {
label   bounds(  0,  5,210, 15), text("AMPLITUDE ENVELOPE"),  fontColour(white)
rslider bounds(  5, 20, 80, 80), text("Attack"),    channel("AAtt"), range(0,20,1,0.5,0.01), valueTextBox(1), textBox(1), $TRACKER, fontColour("white"), textColour("white")
rslider bounds( 65, 20, 80, 80), text("Release"),    channel("ARel"), range(0,20,10,0.5,0.01), valueTextBox(1), textBox(1), $TRACKER, fontColour("white"), textColour("white")
rslider bounds(125, 20, 80, 80), text("Amount"),    channel("AAmt"), range(0,1,0.4,1,0.001), valueTextBox(1), textBox(1), $TRACKER, fontColour("white"), textColour("white")
}

;FILTER
image   bounds(895,  6,210,115), colour(0,0,0,0), shape("rounded"), outlineColour("white"), outlineThickness(2), plant("Filter") {
label   bounds(  0,  5,210, 15), text("FILTER"),  fontColour(white), identChannel("FilterID0")
checkbox bounds( 10, 50, 65, 15), text("On/Off"), channel("FOnOff"), value(1), colour("yellow"), fontColour:0("white"), fontColour:1("white")
rslider bounds( 65, 20, 80, 80), text("Cutoff"),    channel("FCutoff"), range(0,1,0.66,1,0.001), valueTextBox(1), textBox(1), identChannel("FilterID1"), $TRACKER, fontColour("white"), textColour("white")
rslider bounds(125, 20, 80, 80), text("Follow"),     channel("FFollow"), range(0,1,0,1,0.001), valueTextBox(1), textBox(1), identChannel("FilterID2"), $TRACKER, fontColour("white"), textColour("white")
}


;NOISY TONE PARAMETERS
image   bounds(  5,126,210,115), colour(0,0,0,0), shape("rounded"), outlineColour("white"), outlineThickness(2), plant("Noise"), identChannel("NoisyToneID"), visible(0) {
label   bounds(  0,  5,210, 15), text("NOISE FILTER"),  fontColour(white)
rslider bounds(  5, 20, 80, 80), text("Min"),    channel("NFMin"), range(0.01,5,0.1,0.5,0.01), valueTextBox(1), textBox(1), $TRACKER, fontColour("white"), textColour("white")
rslider bounds( 65, 20, 80, 80), text("Max"),    channel("NFMax"), range(0.01,5,0.1,0.5,0.01), valueTextBox(1), textBox(1), $TRACKER, fontColour("white"), textColour("white")
rslider bounds(125, 20, 80, 80), text("Movement"),    channel("NFMvt"), range(0.01,20,0.1,0.5,0.01), valueTextBox(1), textBox(1), $TRACKER, fontColour("white"), textColour("white")
}                                                     


;BOW POSITION
image   bounds(  5,126,210,115), colour(0,0,0,0), shape("rounded"), outlineColour("white"), outlineThickness(2), plant("BowPos"), identChannel("Bow1") {
label   bounds(  0,  5,210, 15), text("BOW POSITION"),  fontColour(white)
rslider bounds(  5, 20, 80, 80), text("Min"),    channel("RMin"), range(0,1,0.23,1,0.001), valueTextBox(1), textBox(1), $TRACKER, fontColour("white"), textColour("white")
rslider bounds( 65, 20, 80, 80), text("Max"),    channel("RMax"), range(0,1,0.35,1,0.001), valueTextBox(1), textBox(1), $TRACKER, fontColour("white"), textColour("white")
rslider bounds(125, 20, 80, 80), text("Movement"),    channel("RMvt"), range(0.01,20,0.1,0.5,0.001), valueTextBox(1), textBox(1), $TRACKER, fontColour("white"), textColour("white")
}

;BOW PRESSURE     
image   bounds(220,126,210,115), colour(0,0,0,0), shape("rounded"), outlineColour("white"), outlineThickness(2), plant("Bow Pressure"), identChannel("Bow2") {
label   bounds(  0,  5,210, 15), text("BOW PRESSURE"),  fontColour(white)
rslider bounds(  5, 20, 80, 80), text("Min"),    channel("PMin"), range(0.01,40,1,0.5,0.01), valueTextBox(1), textBox(1), $TRACKER, fontColour("white"), textColour("white")
rslider bounds( 65, 20, 80, 80), text("Max"),    channel("PMax"), range(0.01,40,3,0.5,0.01), valueTextBox(1), textBox(1), $TRACKER, fontColour("white"), textColour("white")
rslider bounds(125, 20, 80, 80), text("Movement"),    channel("PMvt"), range(0.01,20,0.23,0.5,0.001), valueTextBox(1), textBox(1), $TRACKER, fontColour("white"), textColour("white")
}

;VIBRATO                                      
image   bounds(435,126,230,115), colour(0,0,0,0), shape("rounded"), outlineColour("white"), outlineThickness(2), plant("Vibrato"), identChannel("Bow3") {
label   bounds(  0,  5,230, 15), text("VIBRATO"),  fontColour(white), identChannel("VibratoID0"), fontColour(white)
rslider bounds(  5, 20, 80, 80), text("Depth"),    channel("VDep"), range(0,2,0.001,0.75,0.001), valueTextBox(1), textBox(1), $TRACKER, fontColour("white"), textColour("white")
rslider bounds( 65, 20, 80, 80), text("Rate"),     channel("VRate"), range(0.01,999,0.5,0.25,0.01), valueTextBox(1), textBox(1), identChannel("VibratoID1"), $TRACKER, fontColour("white"), textColour("white")
label    bounds(140, 35, 75, 13), text("Shape"), identChannel("VibratoID2"), fontColour(white)
combobox bounds(140, 50, 75, 20), text("Sine","Random","Triangle","Square","Squarish","Random 2"), channel("VShape"), value(2), identChannel("VibratoID3"), fontColour(200,200,50)
}

;REVERB
image   bounds(670,126,210,115), colour(0,0,0,0), shape("rounded"), outlineColour("white"), outlineThickness(2), plant("Reverb") {
label   bounds(  0,  5,210, 15), text("REVERB"),  fontColour(white), identChannel("ReverbID1"), fontColour(white)
rslider bounds(  5, 20, 80, 80), text("Mix"),     channel("RvbMix"), range(0,1,0.4,1,0.01), valueTextBox(1), textBox(1), $TRACKER, fontColour("white"), textColour("white")
rslider bounds( 65, 20, 80, 80), text("Size"),    channel("RvbSiz"), range(0.3,0.99,0.75,1,0.01), valueTextBox(1), textBox(1), identChannel("ReverbID2"), $TRACKER, fontColour("white"), textColour("white")
rslider bounds(125, 20, 80, 80), text("Damping"), channel("RvbDmp"), range(200,20000,8000,0.5,1), valueTextBox(1), textBox(1), identChannel("ReverbID3"), $TRACKER, fontColour("white"), textColour("white")
}                                  

;PRESETS
image   bounds(885,126, 65,115), colour(0,0,0,0), shape("rounded"), outlineColour("white"), outlineThickness(2), plant("Presets") {
label   bounds(  0,  5, 65, 15), text("PRESETS"),  fontColour(white)
combobox bounds( 10, 50, 40, 20), text("1","2","3","4","5","6","7","8","9","10","11","12","13"), channel("preset"), value(1), fontColour(200,200,50)
}

;STEREO
image    bounds(955,126,150,115), colour(0,0,0,0), shape("rounded"), outlineColour("white"), outlineThickness(2), plant("Stereo") {
label    bounds(  0,  5,150, 15), text("STEREO"), fontColour(white)
label    bounds(  5, 25, 85, 13), text("Mode"), fontColour(white)
combobox bounds(  5, 40, 85, 20), text("Alternate","Rnd.Fixed","Rnd.Moving"), channel("StMode"), value(1), fontColour(200,200,50)
label    bounds(  5, 66, 85, 13), text("Speed"), identChannel("StSpeedID0"), visible(0), fontColour(white)
hslider  bounds(  5, 83, 85, 12), channel("StSpeed"), identChannel("StSpeedID1"), range(0.01,10,0.1,0.5,0.01), $TRACKER, visible(0)
rslider  bounds( 80, 20, 80, 80), text("Width"),     channel("StWidth"), range(0,1,0.8,1,0.01), valueTextBox(1), textBox(1), $TRACKER, fontColour("white"), textColour("white")
}

keyboard pos(5, 250), size(1035, 80)
label   bounds(1060,315, 30, 12), text("Level"), fontColour("white")
vslider bounds(1065,250, 20, 70),  channel("OutLevel"), range(0,10,2,1,0.01), $TRACKER

</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-dm0 -n -+rtmidi=NULL -M0
</CsOptions>

<CsInstruments>

; sr set by host
ksmps         =     32
nchnls         =     2
0dbfs        =    1    ;MAXIMUM AMPLITUDE

seed    0                ; random number generators seeded from system clock
massign    0,2                ; midi note triggering on all channels directed to instrument 2
gkactive    init    0    ; Will contain number of active instances of instr 3 when legato mode is chosen. NB. notes in release stage will not be regarded as active. 

giVibFn1    ftgen    0,0,1024,10,1                        ; gisine
giVibFn2     ftgen     0,0,1024,21,6,1                     ; random (gaussian)
giVibFn3    ftgen    0,0,1024,7,0,256,1,512,-1,256,0        ; triangle
giVibFn4    ftgen    0,0,1024,7,1,512,1,0,-1,512,-1        ; square
giVibFn5    ftgen    0,0,1024,10,1                        ; squarish
giVibFn6    ftgen    0,0,1024,41,    (rnd(1)*2)-1,rnd(50), \
                                    (rnd(1)*2)-1,rnd(50), \
                                    (rnd(1)*2)-1,rnd(50), \
                                    (rnd(1)*2)-1,rnd(50), \
                                    (rnd(1)*2)-1,rnd(50), \
                                    (rnd(1)*2)-1,rnd(50), \
                                    (rnd(1)*2)-1,rnd(50), \
                                    (rnd(1)*2)-1,rnd(50), \
                                    (rnd(1)*2)-1,rnd(50), \
                                    (rnd(1)*2)-1,rnd(50), \
                                    (rnd(1)*2)-1,rnd(50), \
                                    (rnd(1)*2)-1,rnd(50), \
                                    (rnd(1)*2)-1,rnd(50), \
                                    (rnd(1)*2)-1,rnd(50), \
                                    (rnd(1)*2)-1,rnd(50), \
                                    (rnd(1)*2)-1,rnd(50), \
                                    (rnd(1)*2)-1,rnd(50), \
                                    (rnd(1)*2)-1,rnd(50), \
                                    (rnd(1)*2)-1,rnd(50), \
                                    (rnd(1)*2)-1,rnd(50), \
                                    (rnd(1)*2)-1,rnd(50), \
                                    (rnd(1)*2)-1,rnd(50), \
                                    (rnd(1)*2)-1,rnd(50), \
                                    (rnd(1)*2)-1,rnd(50), \
                                    (rnd(1)*2)-1,rnd(50), \
                                    (rnd(1)*2)-1,rnd(50), \
                                    (rnd(1)*2)-1,rnd(50), \
                                    (rnd(1)*2)-1,rnd(50), \
                                    (rnd(1)*2)-1,rnd(50), \
                                    (rnd(1)*2)-1,rnd(50), \
                                    (rnd(1)*2)-1,rnd(50), \
                                    (rnd(1)*2)-1,rnd(50), \
                                    (rnd(1)*2)-1,rnd(50), \
                                    (rnd(1)*2)-1,rnd(50), \
                                    (rnd(1)*2)-1,rnd(50), \
                                    (rnd(1)*2)-1,rnd(50), \
                                    (rnd(1)*2)-1,rnd(50), \
                                    (rnd(1)*2)-1,rnd(50), \
                                    (rnd(1)*2)-1,rnd(50), \
                                    (rnd(1)*2)-1,rnd(50), \
                                    (rnd(1)*2)-1,rnd(50), \
                                    (rnd(1)*2)-1,rnd(50), \
                                    (rnd(1)*2)-1,rnd(50), \
                                    (rnd(1)*2)-1,rnd(50), \
                                    (rnd(1)*2)-1,rnd(50), \
                                    (rnd(1)*2)-1,rnd(50), \
                                    (rnd(1)*2)-1,rnd(50), \
                                    (rnd(1)*2)-1,rnd(50), \
                                    (rnd(1)*2)-1,rnd(50), \
                                    (rnd(1)*2)-1,rnd(50), \
                                    (rnd(1)*2)-1,rnd(50), \
                                    (rnd(1)*2)-1,rnd(50)
                                                       
icnt        init    0
loop:
ival        table    icnt,giVibFn5
            tableiw    (abs(ival)^0.125) * (ival<0?-1:1),icnt,giVibFn5
            loop_lt    icnt,1,ftlen(giVibFn5),loop

gitun1        ftgen    0,0,-7,-2,    0,2,4,5,7,9,11    ; major scale
gitun2        ftgen    0,0,-7,-2,    0,2,3,5,7,8,11    ; minor scale
gitun3        ftgen    0,0,-5,-2,    0,3,5,7,10        ; pentatonic

gicos        ftgen    0,0,131072,11,1

opcode    Randomh2,k,kkkk
 kMin,kMax,kRateMin,kRateMax    xin
 ktrig    init    1
 kRate    trandom    ktrig,kRateMin,kRateMax
 kVal    randomh    kMin,kMax,kRate
 ktrig    changed    kVal
         xout    kVal
endop

opcode    SsplinePort,k,KkkO                                        ; DEFINE OPCODE
    knum,kporttime,kcurve,iopt    xin                                ; READ IN INPUT ARGUMENTS
    kout    init    i(knum)                                        ; INITIALISE TO OUTPUT VALUE (PORTAMENTO APPLIED VALUE)
    ktrig    changed    knum                                        ; ...GENERATE A TRIGGER IS A NEW NOTE NUMBER IS GENERATED (FROM INSTR. 1)
     if ktrig=1 then                                            ; IF A NEW (LEGATO) NOTE HAS BEEN PRESSED 
      reinit    S_CURVE                                            ; BEGIN A REINITIALISATION PASS FROM LABEL
     endif                                                        ; END OF CONDITIONAL BRANCH
     S_CURVE:                                                    ; A LABEL. REINITIALISATION BEGINS FROM HERE.
    if iopt!=0 then                                                ; IF ABSOLUTE/PROPORTIONAL SWITCH IS ON... (I.E. PROPORTIONAL)
     idiff    =    1+abs(i(knum)-i(kout))                            ; ABSOLUTE DIFFERENCE BETWEEN OLD NOTE AND NEW NOTE IN STEPS (+ 1)
     kporttime    =    kporttime*idiff                                ; SCALE PORTAMENTO TIME ACCORDING TO THE NOTE GAP
    endif                                                        ; END OF CONDITIONAL BRANCH
    imid    =    i(kout)+((i(knum)-i(kout))/2)                    ; SPLINE MID POINT VALUE
    isspline    ftgentmp    0,0,4096,-16,i(kout),4096*0.5,i(kcurve),imid,(4096/2)-1,-i(kcurve),i(knum)    ;GENERATE 'S' SPLINE
    kspd    =    i(kporttime)/kporttime                            ; POINTER SPEED AS A RATIO (WITH REFERENCE TO THE ORIGINAL DURATION)
    kptr    init    0                                            ; POINTER INITIAL VALUE    
    kout    tablei    kptr,isspline                                ; READ VALUE FROM TABLE
    kptr    limit    kptr+((ftlen(isspline)/(i(kporttime)*kr))*kspd), 0, ftlen(isspline)-1                ; INCREMENT THE POINTER BY THE REQUIRED NUMBER OF TABLE POINTS IN ONE CONTROL CYCLE AND LIMIT IT BETWEEN FIRST AND LAST TABLE POINT - FINAL VALUE WILL BE HELD IF POINTER ATTEMPTS TO EXCEED TABLE DURATION
    rireturn                                                    ; RETURN FROM REINITIALISATION PASS
             xout    kout                                                                                ; SEND PORTAMENTOED VALUES BACK TO CALLER INSTRUMENT
endop


opcode    IntervalRescale,k,ki
 kstep,ifn    xin
 ilen    =    ftlen(ifn)
 kdir    =        kstep<0?-1:1        
 kval    tablei    wrap:k(kstep,-0.5,ilen-0.5),ifn,0,0,1
 koct    =        int(kstep / ftlen(ifn)) 
 if kdir=-1 then
  kval    wrap    kval,0.5,12
  kshft    =        (12-kval)*kdir + (koct*12)
 else
  kshft    =        kval + koct*12
 endif
          xout    kshft
endop


opcode    StringVoice,aa,kkkkkkkkkkkkkkkikkiiip
 knum,kspread,kPortSmear,kAEnv,kRMin,kRMax,kRMvt,kPMin,kPMax,kPMvt,kNFMin,kNFMax,kNFMvt,kVDep,kVRate,iStMode,kStSpeed,kSynth,iVShape,iVScale,ilayers,icnt        xin
 krndcent    rspline        -10,10,0.05,0.2                            ; random pitch wobble for this layer in cents. This feature can be defeated by reducing vibrato depth to zero.

; krndcent    Randomh2    -100,100,10,15                            ; sample and hold

 if frac(ilayers*0.5)==0 then                                    ; if an even number of layers, increment the counter for this layer by 1
  icntL    =    icnt+1                                                ; 'icntL' = 'local counter'
 else
  icntL    =    icnt
 endif
 idir    =        (((icntL-1) % 2) * 2) - 1                        ; direction of pitch divergence, alternately +1 or -1, layer by layer
 kshft        =    ( (int((icntL*0.5)+1)-1) * kspread * idir )        ; pitch shift for this voice in semitones
 if iVScale>0 then
  kshft        IntervalRescale    kshft,iVScale
 endif
 knumL        vdel_k    knum+kshft,randomi:k(0,kPortSmear,0.3,1),8    ; note number for this layer. It will be randomly delayed to implement the 'smearing' effect. Make sure that the maximum delay time specified here corresponds to the maximum value of the accompanying GUI control

 kfreq        =        cpsmidinn(knumL)*cent(krndcent*ceil(kVDep))    ; derive frequency for wgbow opcode
 aL,aR        init    0
 if kfreq>sr/3 goto SKIP
 kpres        rspline        kPMin,kPMax,kPMvt,kPMvt*2                ; bow pressure
 krat        rspline        kRMin,kRMax,kRMvt,kRMvt*2                ; bow position (ratio)
 kVRate        rspline        kVRate*0.75,kVRate*1.25,0.2,0.5            ; vibrato rate (some randomness is incorporated to prevent sync. between layers)
 kvibamp    rspline        0,0.5,0.1,0.3                            ; vibrato amplitude (some randomness is incorporated to prevent parallel movement)
 iminfreq    =        20
 if kSynth==1 then
  asig        wgbow    kAEnv * ilayers/(ilayers^1.5), kfreq, kpres, krat, kVRate, kVDep, iVShape, iminfreq
;  aNse        pinkish    1
;  asig        streson    aNse, kfreq, 0.99
;  asig        streson    asig, kfreq, 0.99
;  asig        dcblock2    asig
  asig        *=        kAEnv * ilayers/(ilayers^1.5)
 elseif kSynth==2 then
  asig        poscil    kAEnv * ilayers/(ilayers^1.5) *0.3, kfreq
 elseif kSynth==3 then
  asig        reson    pinkish:a(ilayers/(ilayers^1.5) *0.5), kfreq, kfreq*randomi:k(kNFMin,kNFMax,kNFMvt)*0.01, 2
  asig        *=        kAEnv
 else
  asig        gbuzz    kAEnv * ilayers/(ilayers^1.5) *0.4, kfreq, 30,1,rspline(0.1,0.9,0.1,0.2),gicos
 endif 
 
 if icnt==1&&frac(ilayers*0.5)!=0 then                            ; if first voice and odd number of voices
  aL    =    asig                                                ; locate the first voice (unaffected by 'Spread') in the centre
  aR    =    asig
 else
  if iStMode==1 then
   ipan        init    wrap:i(icntL,0,2)
   aL,aR        pan2    asig*6,ipan
  elseif iStMode==2 then
   ipan        random    0,1
   aL,aR        pan2    asig*6,ipan
  else
   kpan        rspline    0.1,0.9,kStSpeed,kStSpeed*2
   aL,aR        pan2    asig*6,kpan
  endif
 endif
 SKIP:
 aMixL        =    0                                                ; clear accumulating 'mix' audio variables
 aMixR        =    0
 if icnt<ilayers then
  aMixL,aMixR    StringVoice    knum,kspread,kPortSmear,kAEnv,kRMin,kRMax,kRMvt,kPMin,kPMax,kPMvt,kNFMin,kNFMax,kNFMvt,kVDep,kVRate,iStMode,kStSpeed,kSynth,iVShape,iVScale,ilayers,icnt+1
 endif
             xout    aL + aMixL, aR + aMixR                        ; mix this layers will all subsequent layers
endop

instr    1    ; always on. Read in widgets and send presets.
 gkmonopoly    chnget    "monopoly"        ; read in widgets
 gkLegTim    chnget    "LegTim"
 gkspread    chnget    "spread"
 gkvoices    chnget    "voices"
 gkVScale    chnget    "VScale"
 gkSynth    chnget    "Synth"
 gkPortSmear    chnget    "PortSmear"
 gkAAtt        chnget    "AAtt"
 gkARel        chnget    "ARel"
 gkAAmt        chnget    "AAmt"
 gkRMin        chnget    "RMin"
 gkRMax        chnget    "RMax"
 gkRMvt        chnget    "RMvt"
 gkPMin        chnget    "PMin"
 gkPMax        chnget    "PMax"
 gkPMvt        chnget    "PMvt"
 gkNFMin    chnget    "NFMin"
 gkNFMax    chnget    "NFMax"
 gkNFMvt    chnget    "NFMvt"
 gkRvbMix    chnget    "RvbMix"
 gkRvbSiz    chnget    "RvbSiz"
 gkRvbDmp    chnget    "RvbDmp"
 gkVDep        chnget    "VDep"
 gkVRate        chnget    "VRate"
 gkVShape    chnget    "VShape"
 gkStWidth    chnget    "StWidth"
 gkStSpeed    chnget    "StSpeed"
 gkStMode    chnget    "StMode"
 gkOutLevel    port    chnget:k("OutLevel"),0.05
 gkFOnOff    chnget    "FOnOff"
 gkFCutoff    chnget    "FCutoff"
 gkFFollow    chnget    "FFollow"
 gkKIntvl    chnget    "KIntvl"
 gkKShift    chnget    "KShift"
 kpreset    chnget    "preset"
 ktrig        changed    kpreset
 if ktrig==1 then
  reinit    PRESET
  PRESET:
  
  #define    SEND_SETTINGS(N'monopoly'LegTim'PortSmear'voices'spread'VScale'KIntvl'KShift'AAtt'ARel'AAmt'FOnOff'FCutoff'FFollow'StMode'StWidth'StSpeed'OutLevel'RMin'RMax'RMvt'PMin'PMax'PMvt'VDep'VRate'VShape'RvbMix'RvbSiz'RvbDmp)#
  if i(kpreset)==$N then
   chnset    $monopoly    ,"monopoly"
   chnset    $LegTim       ,"LegTim"
   chnset    $PortSmear    ,"PortSmear"
   chnset    $voices       ,"voices"
   chnset    $spread       ,"spread"
   chnset    $VScale       ,"VScale"
   chnset    $KIntvl        ,"KIntvl"
   chnset    $KShift        ,"KShift"
   chnset    $AAtt         ,"AAtt"
   chnset    $ARel         ,"ARel"
   chnset    $AAmt         ,"AAmt"
   chnset    $FOnOff       ,"FOnOff"
   chnset    $FCutoff      ,"FCutoff"
   chnset    $FFollow      ,"FFollow"
   chnset    $StMode      ,"StMode"
   chnset    $StWidth      ,"StWidth"
   chnset    $StSpeed      ,"StSpeed"
   chnset    $OutLevel     ,"OutLevel"
   chnset    $RMin         ,"RMin"
   chnset    $RMax         ,"RMax"
   chnset    $RMvt         ,"RMvt"
   chnset    $PMin         ,"PMin"
   chnset    $PMax         ,"PMax"
   chnset    $PMvt         ,"PMvt"
   chnset    $VDep         ,"VDep"
   chnset    $VRate        ,"VRate"
   chnset    $VShape       ,"VShape"
   chnset    $RvbMix       ,"RvbMix"
   chnset    $RvbSiz       ,"RvbSiz"
   chnset    $RvbDmp       ,"RvbDmp"
  endif#
  ;                 monopoly'LegTim'PortSmear'voices'spread'VScale'KIntvl'KShift'AAtt'ARel'AAmt 'FOnOff'FCutoff'FFollow'StMode'StWidth'StSpeed'OutLevel'RMin 'RMax 'RMvt'PMin'PMax'PMvt'VDep 'VRate'VShape'RvbMix'RvbSiz'RvbDmp)#
  $SEND_SETTINGS(1  '1      '0.8   '3        '18    '0.5   '1     '1     '0     '1   '10  '0.4  '1     '0.66   '0      '1     '0.7    '0.1    '4       '0.23 '0.35 '0.1 '1.0 '3   '0.23'0.001'0.5  '2     '0.4   '0.75  '8000  )            
  $SEND_SETTINGS(2  '1      '0.5   '0.3      '11    '7     '1     '1     '0     '2   '10  '0.4  '1     '0.45   '0.01   '1     '0.7    '0.1    '3       '0.1  '0.2  '0.1 '0.7 '2   '0.1 '0.001'0.5  '2     '0.4   '0.75  '8000  )
  $SEND_SETTINGS(3  '1      '0.5   '2        '7     '0.02  '1     '1     '0     '0.12'0.9 '0.3  '1     '0.5    '1      '1     '0.8    '0.1    '2       '0.008'0.7  '0.1 '1.35'3.3 '0.23'0.001'0.01 '2     '0.4   '0.75  '8000  )
  $SEND_SETTINGS(4  '1      '0.5   '3        '51    '1.15  '1     '1     '0     '6   '10  '0.7  '1     '0.4    '0      '1     '0.9    '0.1    '2       '0.03 '0.07 '0.1 '1   '3   '0.23'0    '0.01 '2     '0.4   '0.75  '8000  )
  $SEND_SETTINGS(5  '0      '0.5   '3        '2     '3     '1     '1     '0     '2   '5   '0.1  '1     '0.002  '1      '1     '0.8    '0.1    '2       '0.7  '0.8  '0.1 '7   '8   '0.23'0.1  '0.01 '2     '0.4   '0.75  '8000  )
  $SEND_SETTINGS(6  '1      '0.4   '3        '9     '2     '1     '1     '0     '1.5 '2.5 '0.3  '1     '0      '1      '1     '0.7    '0.1    '10      '0.05 '0.07 '0.1 '7   '8   '0.23'0.01 '2.2  '1     '0.3   '0.9   '15000 )
  $SEND_SETTINGS(7  '1      '0.8   '2        '5     '1     '1     '1     '0     '1   '0.5 '0.2  '1     '0.055  '1      '1     '0.8    '0.1    '4       '0.04 '0.37 '0.1 '7   '8   '0.23'0.32 '0.13 '1     '0.28  '0.89  '12000 )
  $SEND_SETTINGS(8  '1      '0.003 '0        '4     '12    '1     '1     '0     '0   '0   '1    '0     '0      '0      '1     '0.3    '0.1    '2       '0.04 '0.38 '0.01'3.29'5.3 '0.01'0    '0.01 '2     '0.5   '0.47  '15000 )
  $SEND_SETTINGS(9  '0      '1     '1        '9     '0.1   '1     '1     '0     '0.5 '6   '1    '1     '0.07   '1      '1     '0.9    '0.1    '3       '0.002'0.025'0.9 '3.2 '5.3 '0.01'0.066'0.01 '2     '0.5   '0.67  '15000 )
  $SEND_SETTINGS(10 '1      '1     '1        '51    '0.5   '1     '1     '0     '2.5 '20  '0.6  '1     '0.04   '1      '1     '0.7    '0.1    '10      '0.35 '0.37 '1   '0.01'0.4 '0.01'0.01 '0.1  '1     '0.4   '0.75  '15000 )
  $SEND_SETTINGS(11 '1      '0.8   '3        '9     '1     '2     '1     '0     '1   '10  '0.5  '1     '0.66   '0      '1     '0.7    '0.1    '2       '0.23 '0.35 '0.1 '1.0 '3   '0.23'0.001'0.5  '2     '0.4   '0.75  '8000  )            
  $SEND_SETTINGS(12 '1      '0.8   '3        '9     '1     '3     '1     '0     '1   '10  '0.2  '1     '0.66   '0      '1     '0.7    '0.1    '2       '0.23 '0.35 '0.1 '1.0 '3   '0.23'0.001'0.5  '2     '0.4   '0.75  '8000  )            
  $SEND_SETTINGS(13 '1      '0.8   '3        '7     '1     '4     '1     '0     '1   '10  '0.3  '1     '0.66   '0      '1     '0.7    '0.1    '2       '0.23 '0.35 '0.1 '1.0 '3   '0.23'0.001'0.5  '2     '0.4   '0.75  '8000  )            
  rireturn
 endif

endin




instr    2    ; triggered via MIDI
    gkNoteTrig    init    1                                ; at the beginning of a new note set note trigger flag to '1'

    inum        notnum                                    ; read in midi note number
    inum        =        (inum*i(gkKIntvl))+i(gkKShift)
            
    gknum    =    inum                                    ; update a global krate variable for note pitch
    ivel    veloc    0,1
    
    if i(gkmonopoly)==0 then                            ; if we are *not* in legato mode...
     inum    notnum                                        ; read midi note number (0 - 127)
         event_i    "i",p1+1+(inum*0.001),0,-1,inum,ivel    ; call sound producing instr
     krel    release                                        ; release flag (1 when note is released, 0 otherwise)
     if krel==1 then                                    ; when note is released...
      turnoff2    p1+1+(inum*0.001),4,1                    ; turn off the called instrument
     endif                                                ; end of conditional
    else                                                ; otherwise... (i.e. legato mode)
     iactive    =    i(gkactive)                            ; number of active notes of instr 3 (note in release are disregarded)
     if iactive==0 then                                    ; ...if no notes are active
      event_i    "i",p1+1,0,-1,0,ivel                    ; ...start a new held note
     endif
    endif
endin

instr    3
    kPortRamp    linseg    0,0.001,1                        ; portamento time function rises quickly from zero to a held value
    kporttime    =    (kPortRamp*gkLegTim)                ; scale portamento time function with value from GUI knob widget
    if i(gkmonopoly)==1 then                            ; if we are in legato mode...
     krel    release                                        ; sense when  note has been released
     gkactive    =    1-krel                                ; if note is in release, gkactive=0, otherwise =1
     if krel==0 then
       knum    SsplinePort    gknum,kporttime,1,1                ; GLISSANDO TIME PROPORTIONAL TO NOTE GAP (OPTION SET TO '1'), THEREFORE PORTAMENTO TIME DEPENDENT UPON NOTE GAP. LARGER INTERVALS WILL RESULT IN PROPORTIONALLY LONGER PORTAMENTO TIMES.
     endif
     kactive    active    p1-1                            ; ...check number of active midi notes (previous instrument)
     if kactive==0 then                                    ; if no midi notes are active...
      turnoff                                            ; ... turn this instrument off
     endif
    else                                                ; otherwise... (polyphonic / non-legato mode)
     knum    =    p4                                         ; pitch equal to the original note pitch
    endif

 ; amplitude envelope
 ;kAEnv    linsegr    0,0.005+i(gkAAtt),1,0.01+i(gkARel),0
 ;kAEnv    expcurve    kAEnv,12
 kAEnv    linsegr    0,0.005+i(gkAAtt),1,0.01+i(gkARel),0
 aFiltMlt linsegr    0,0.005+i(gkAAtt),1,0.01+i(gkARel),0
 
 ; smooth changes in spread
 kspread    portk    gkspread, kporttime*gkLegTim
 
 
 if changed(gkvoices,gkVShape,gkVScale)==1 then                            ; changing number of voices requires a restart
  reinit RESTART
 endif
 RESTART:
 ; setup intervals scaling table
 iVScale    =    i(gkVScale)>1?gitun1+i(gkVScale)-2:0
 ; vibrato setup values
 iVShape    =    i(gkVShape)                                ; vibrato shape. 1=sine 2=random
 kVRate        =    iVShape==2?gkVRate/100:gkVRate            ; if random shape, reduce rate
 aL,aR        StringVoice    knum,kspread,gkPortSmear,kAEnv*gkAAmt*p5,gkRMin,gkRMax,gkRMvt,gkPMin*p5,gkPMax*p5,gkPMvt,gkNFMin,gkNFMax,gkNFMvt,gkVDep,kVRate,i(gkStMode),gkStSpeed,gkSynth,giVibFn1+iVShape-1,iVScale,i(gkvoices)
 rireturn                                                           
 
 ; scale according to opcode amplitude level to balance (we need to protect against a divide by zero), gkOutLevel offers independent gain control
 aL        *=        divz(1,gkAAmt,1) * gkOutLevel
 aR        *=        divz(1,gkAAmt,1) * gkOutLevel
                                       
 ; lowpass filter
 if gkFOnOff==1 then                                                                ; only do the filter code if filter on/off button is on
  kCFstat    =        cpsoct(scale:k(gkFCutoff,14,6))                                    ; static cutoff value
  kCFfol    =        cpsmidinn(knum) * scale:k(gkFCutoff,128,0.5)                    ; key-following cutoff value
  kCF        portk    limit:k(ntrpol(kCFstat,kCFfol,gkFFollow),20,sr/2),kPortRamp*0.1    ; interpolate between the two according to user's setting of gkFFollow
  
  aFiltL    butlp    aL,kCF*aFiltMlt                                                            ; lowpass filter the sound
  aFiltR    butlp    aR,kCF*aFiltMlt
  
  aL        balance    aFiltL,aL
  aR        balance    aFiltR,aR
  
 endif
             
 ; stereo width control
 aL        ntrpol    (aL+aR)*0.5, aL, gkStWidth                                            ; interpolate between hard stereo and mixed to mono
 aR        ntrpol    (aL+aR)*0.5, aR, gkStWidth
 
         outs    aL*(1-gkRvbMix),aR*(1-gkRvbMix)                                        ; dry signal to output
         chnmix    aL*gkRvbMix,"sendL"                                                    ; send some signal to reverb
         chnmix    aR*gkRvbMix,"sendR"
         
endin

instr    99    ; reverb
 if gkRvbMix>0 then                                                                    ; only do reverb code if mix is above zero
  aInL    chnget    "sendL"                                                                ; read in send channels
  aInR    chnget    "sendR"
         chnclear    "sendL"                                                            ; clear send channels
         chnclear    "sendR"
  aL,aR    reverbsc    aInL,aInR,gkRvbSiz,gkRvbDmp
         outs        aL,aR
 endif
 
 ; activate reverb controls on or off as 'Mix' switches between zero and non-zero
 if trigger(gkRvbMix,0.001,2)==1 then
  if gkRvbMix>0 then
   chnset    "alpha(1)","ReverbID1"
   chnset    "alpha(1), active(1)","ReverbID2"
   chnset    "alpha(1), active(1)","ReverbID3"
  else
   chnset    "alpha(0.2)","ReverbID1"
   chnset    "alpha(0.2), active(0)","ReverbID2"
   chnset    "alpha(0.2), active(0)","ReverbID3"
  endif
 endif
 
 ; activate filter controls on or off as 'Filter On/Off' switches on or off
 if trigger(gkFOnOff,0.5,0)==1 then
   chnset    "alpha(1)","FilterID0"
   chnset    "alpha(1), active(1)","FilterID1"
   chnset    "alpha(1), active(1)","FilterID2"
  elseif trigger(gkFOnOff,0.5,1)==1 then
   chnset    "alpha(0.2)","FilterID0"
   chnset    "alpha(0.2), active(0)","FilterID1"
   chnset    "alpha(0.2), active(0)","FilterID2"
  endif

 ; activate filter controls on or off as 'Depth' switches between zero and non-zero
 if trigger(gkVDep,0.001,2)==1 then
  if gkVDep>0 then
   chnset    "alpha(1)","VibratoID0"
   chnset    "alpha(1)","VibratoID2"
   chnset    "alpha(1), active(1)","VibratoID1"
   chnset    "alpha(1), active(1)","VibratoID3"
  else
   chnset    "alpha(0.2)","VibratoID0"
   chnset    "alpha(0.2)","VibratoID2"
   chnset    "alpha(0.2), active(0)","VibratoID1"
   chnset    "alpha(0.2), active(0)","VibratoID3"
  endif
 endif

 ; Stereo Speed control on/off
 if changed(gkStMode)==1 then
  if gkStMode==3 then
   chnset    "visible(1)","StSpeedID0"
   chnset    "visible(1)","StSpeedID1"
  else
   chnset    "visible(0)","StSpeedID0"
   chnset    "visible(0)","StSpeedID1"
  endif
 endif

 ; Stereo Speed control on/off
 if changed(gkSynth)==1 then
  if gkSynth==1 then
   chnset    "visible(1)","Bow1"
   chnset    "visible(1)","Bow2"
   chnset    "visible(1)","Bow3"
   chnset    "visible(0)","NoisyToneID"
  elseif gkSynth==2 then
   chnset    "visible(0)","Bow1"
   chnset    "visible(0)","Bow2"
   chnset    "visible(0)","Bow3"
   chnset    "visible(0)","NoisyToneID"
  elseif gkSynth==3 then
   chnset    "visible(0)","Bow1"
   chnset    "visible(0)","Bow2"
   chnset    "visible(0)","Bow3"
   chnset    "visible(1)","NoisyToneID"
  elseif gkSynth==4 then
   chnset    "visible(0)","Bow1"
   chnset    "visible(0)","Bow2"
   chnset    "visible(0)","Bow3"
   chnset    "visible(0)","NoisyToneID"
  endif
 endif

endin

</CsInstruments>

<CsScore>
i 1  0 z    ; read in widgets / send presets
i 99 0 z     ; reverb / 'turn on and off' bits of the GUI
</CsScore>

</CsoundSynthesizer>