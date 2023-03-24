
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; LiveSndwarp.csd
; Iain McCurdy (2012)
; 
; Description and Instructions
; ----------------------------
; 
; This instrument implements live granulation of an audio input stream using the sndwarp opcode.                         
; 
; Live audio from the first input channel (left input if stereo) is written into a function table from which sndwarp reads 
; audio. (If 'stereo in/out' is chosen from 'In/Out Mode' then audio from the second/right channel is written into a second 
; table.) The key is that manual pointer read mode is used with sndwarp (as opposed to time-stretch mode) and that the read 
; pointer follows on behind the pointer which is used to write audio into the function table(s). Some care is needed to ensure 
; that the read pointer does not 'overtake' the write pointer (which would result in dicontinuities in the audio it reads). 
; This could be possible if pitch transposition upwards of grains is used as the grain pointer is then moving faster than the 
; write pointer. This example prevents this from happening internally so the user does not need to worry. The user can also 
; define a random offset for the grain read pointer using the 'Grain Delay' settings. Delay times are randomly chosen 
; according to a 'betarand' distribution the 'beta' of which the user can set: if distribution shape is 1 the distribution is 
; uniform, if it is 2 the distribution is linear and beyond 2 it is increasingly exponential.
; 
; Note that 'Size' (grain size) and 'Size Rnd.' (random grain size) are i-rate variables so that changing them requires 
; reinitialisation in the orchestra. For this reason discontinuity in the audio output can be heard when they are modified.
; Grain Size and Size Random (bandwith) are in sample frames. Divide by sample rate to derive a value in seconds.
; 
; Pitch transposition can be set using either the 'Pitch' knob (ratio multiplier) or 'Semis' (transposition in semitones).
; Changes made to 'Semis' will be reflected in the setting of the 'Pitch' knob, but not vice versa.
; Pitch can also be controlled through MIDI input (in which case 'Pitch' and 'Semis' will be ignored). Using MIDI will 
; polyphony will be possible. If you intend to use MIDI to start and stop sndwarp instances, turn 'On/Off [MIDI]' off.
; You can also adjust the MIDI note at which unison (no transposition) will occur using the 'Uni.Note' knob.
; 
; Sound output from sndwarp can be fed back into the input to be mixed with the live audio in. The amount of feedback can be 
; controlled using the 'Feedback' slider. Using high levels of feedback can result in overloading but this will also be 
; dependent upon other factors such as random delay time, grain size (window size), density and transposition so user caution 
; is advised. If the 'clip' button is activated the feedback signal will be clipped at the clip level set (a ratio of then 
; maximum amplitude) providing at least some control over a runaway feedback loop. Note that 'Clip Lev.' defines the amplitude 
; at which clipping begins, therefore lower settings will result in the signal being clipped sooner. The feedback signal can 
; also be filtered by a lowpass filter.
; 
; If 'balance' is activated the output of sndwarp is dynamically balanced with the input signal. This can be useful for 
; compensating for increases in amplitude caused when 'Number of Overlaps' (grain density) is increased. There are 3 
; mono/stereo modes: 'mono in - mono out (x2)', 'mono in - stereo out' (stereo effect is created using sndwarp's built-in 
; window/grain size randomisation, and 'stereo in - stereo out' mode.
; 
; The buffer size used in the example is just under 23 seconds long (function table size 1048576 at sr=44100). This could be 
; enlarged if required but bear in mind that sndwarp needs a power of two table size.        
; 
; Activating 'Freeze' will pause writing of live audio to the function table and allow the user to manually navigate through 
; the buffered audio. The feedback loop will also be deactivated when 'freeze' is active.

<Cabbage>
form caption("Live Sndwarp") size(530, 495), pluginId("lwrp"), colour("Black")

groupbox bounds(  0,  0, 300,100), text("Master"), colour(30, 30, 40), fontColour(255,125,125), plant("Master")
{
checkbox bounds( 10, 28, 110, 20), colour("yellow"), channel("OnOff"),  value(1), text("On/Off [MIDI]"), trackerColour("red")
label    bounds( 22, 53,  80, 13), text("In/Out Mode")
combobox bounds( 10, 69, 100, 20), channel("monostereo"), value(2), text("mono","stereo out","stereo in/out")
rslider  bounds(115, 30,  60, 60), text("In Gain"),  channel("InGain"), range(0, 2.00, 1, 0.5), colour( 67, 99, 99), trackerColour(207,239,239)
rslider  bounds(175, 30,  60, 60), text("Out Gain"), channel("amp"),    range(0, 2.00, 1, 0.5), colour( 67, 99, 99), trackerColour(207,239,239)
rslider  bounds(235, 30,  60, 60), text("Mix"),      channel("mix"),    range(0, 1.00, 1),      colour( 67, 99, 99), trackerColour(207,239,239)
}

groupbox bounds(300,  0, 230,100), text("Feedback - CAUTION!"), colour(25, 25, 35), fontColour(250,120,120), plant("Feedback")
{
rslider  bounds(  5, 30,  60, 60), text("Amount"), channel("feedback"), range(0, 1.00, 0), colour( 67, 99, 99), trackerColour(207,239,239)
checkbox bounds( 65, 35,  70, 20), colour("yellow"), channel("clip"),  value(1), text("Clip"), trackerColour("red")
checkbox bounds( 65, 65,  70, 20), colour("yellow"), channel("LPF_On"),  value(0), text("LPF"), trackerColour("red")
rslider  bounds(110, 30,  60, 60), text("Clip Lev."), channel("ClipLev"), range(0.01, 1, 0.5, 0.5), colour( 67, 99, 99), trackerColour(207,239,239)
rslider  bounds(165, 30,  60, 60), text("LPF"), channel("Cutoff"), range(20, 20000,  4000, 0.5), colour( 67, 99, 99), trackerColour(207,239,239)
}

groupbox bounds(  0,100, 530,100), text("Grains"), colour(35, 35, 45), fontColour(255,130,130), plant("Grains")
{
rslider  bounds(  5, 30,  60, 60), text("Size"), channel("wsize"), range(1, 88200, 7000, 0.25, 1), colour( 67, 99, 99), trackerColour(207,239,239)
rslider  bounds( 65, 30,  60, 60), text("Size Rnd."), channel("rnd"), range(0, 30000, 1000, 0.375, 1), colour( 67, 99, 99), trackerColour(207,239,239)
rslider  bounds(125, 30,  60, 60), text("Pitch"), channel("pch"), range(0.01, 8, 1, 0.5), colour( 67, 99, 99), trackerColour(207,239,239)
rslider  bounds(185, 30,  60, 60), text("Semis"), channel("semis"), range(-48, 48, 0,1,1), colour( 67, 99, 99), trackerColour(207,239,239)
rslider  bounds(245, 30,  60, 60), text("Density"), channel("olaps"), range(1, 100, 10, 1, 1), colour( 67, 99, 99), trackerColour(207,239,239)
label    bounds(305, 25, 100, 13), text("Grain Envelope")
combobox bounds(305, 41, 100, 20), channel("wfn"), value(1), text("Half Sine","Perc. 1","Perc. 2","Gate","Rev. Perc. 1 ","Rev. Perc. 2")
checkbox bounds(315, 70, 100, 20), colour("yellow"), channel("balance"),  value(0), text("Balance")
rslider  bounds(405, 30,  60, 60), text("Delay"), channel("dly"), range(0, 5, 0.01, 0.5), colour( 67, 99, 99), trackerColour(207,239,239)
rslider  bounds(465, 30,  60, 60), text("Distr."), channel("beta"), range(1, 16.0, 1, 0.5), colour( 67, 99, 99), trackerColour(207,239,239)
}

groupbox bounds(  0,200,530,165), text("Freeze"), colour(45, 45, 55), fontColour(255,100,100), plant("Freeze")
{
button   bounds( 10, 25, 80, 20), fontColour:0(50,50,100), fontColour:1(205,205,255), colour:0(0,0,10), colour:1(100,100,150), channel("freeze"), text("FREEZE","FREEZE"),  value(0)
hslider  bounds( 90, 29,430, 10), text("Port.Time"), channel("ManPtrPort"), range(0, 1.00, 0.5), colour( 67, 99, 99), trackerColour(207,239,239)
gentable bounds( 10, 50,510, 80), tableNumber(1), tableColour("lime"), ampRange(-1,1,1), identChannel(table), zoom(-1), fill(0)
image    bounds(518, 50,  2, 80), colour(200,200,200,100), identChannel("wiper")
hslider  bounds(  5,132,522, 11), channel("ManPtr"), range(-1.00, 0, 0, 1, 0.001), colour( 67, 99, 99), trackerColour(207,239,239)
label    bounds(220,145,100, 13), text("Manual Pointer")
}

groupbox bounds(  0,365,530,100), text("MIDI"), colour(30, 30, 40), fontColour(255,100,100), plant("MIDI")
{
rslider  bounds( 10, 35, 60, 60), text("Uni.Note"), channel("UniNote"), range(0, 127, 72,1,1), colour( 67, 99, 99), trackerColour(207,239,239)
keyboard bounds( 80,  0,450,100)
}
image bounds( 5, 470, 215, 20), colour(75, 85, 90, 100), plant("credit"), outlineThickness(0){
label bounds( 3,   3, 210, 14), text("Author: Iain McCurdy |2012|"), fontColour("white")
}
</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-dm0 -n -+rtmidi=NULL -M0
</CsOptions>

<CsInstruments>

; sr is set by host
ksmps         =     32    ;NUMBER OF AUDIO SAMPLES IN EACH CONTROL CYCLE
nchnls         =     2    ;NUMBER OF CHANNELS (2=STEREO)
0dbfs        =    1
massign    0,2

giview        ftgen    1, 0, 512, 2, 0        ; display buffer

gibuffer    ftgen    0, 0, 1048576, 2, 0    ; Buffer table. Roughly 23 seconds duration.
gibufferR    ftgen    0, 0, 1048576, 2, 0    ; right channel

;GRAIN ENVELOPE WINDOW FUNCTION TABLES:
giwfn1    ftgen    0,  0, 131072,  9,   .5, 1,     0                             ; HALF SINE
giwfn2    ftgen    0,  0, 131072,  7,    0, 3072,  1, 128000,     0            ; PERCUSSIVE - STRAIGHT SEGMENTS
giwfn3    ftgen    0,  0, 131072,  5, .001, 3072,  1, 128000, 0.001            ; PERCUSSIVE - EXPONENTIAL SEGMENTS
giwfn4    ftgen    0,  0, 131072,  7,    0, 1536,  1, 128000,     1, 1536, 0    ; GATE - WITH ANTI-CLICK RAMP UP AND RAMP DOWN SEGMENTS
giwfn5    ftgen    0,  0, 131072,  7,    0, 128000,1, 3072,       0            ; REVERSE PERCUSSIVE - STRAIGHT SEGMENTS
giwfn6    ftgen    0,  0, 131072,  5, .001, 128000,1, 3072,   0.001            ; REVERSE PERCUSSIVE - EXPONENTIAL SEGMENTS

instr    1
    gkOnOff        chnget    "OnOff"
    ktrigger    trigger    gkOnOff,0.5,0
    schedkwhen    ktrigger,0,0,2,0,-1

    ginsamp        =    ftlen(gibuffer)-1            ;index of the final sample in the function table

    gkamp        chnget    "amp"
    gkInGain    chnget    "InGain"
    gkmix        chnget    "mix"
    gkbalance    chnget    "balance"
    gkmonostereo    chnget    "monostereo"
    gkfback        chnget    "feedback"
    gkclip      chnget    "clip"
    gkClipLev      chnget    "ClipLev"
    gkCutoff    chnget    "Cutoff"
    gkpch        chnget    "pch"
    gkLPF_On      chnget    "LPF_On"
    gkwsize        chnget    "wsize"
    gkwsize        =    int(gkwsize)    ; workaround
    gkrnd        chnget    "rnd"
    gkornd        =    int(gkrnd)    ; workaround
    gkolap        chnget    "olaps"
    gkolap        =    int(gkolap)    ; workaround
    gkwfn        chnget    "wfn"
    gkdly        chnget    "dly"
    gkbeta        chnget    "beta"
    gkfreeze    chnget    "freeze"
    gkManPtrPort    chnget    "ManPtrPort"
    gkManPtr    chnget    "ManPtr"
    gkUniNote    chnget    "UniNote"

    ktrigger    trigger    gkfreeze,0.5,1                ; if 'freeze' switch is turned off...
    if ktrigger==1 then                        
     chnset    1-ktrigger,"ManPtr"                    ; reset Manual Pointer slider to its default (maximum) position
    endif

    ain    inch    1                        ; read audio input from the left input channel
    ain    =    ain*gkInGain                    ; scale input signal according to 'In Gain' control position
        outch    1,ain*(1-gkmix)                    ; send some dry signal to output according to dry/wet 'Mix' control position
    gaFBackSig,gaFBackSigR    init    0                ; audio feedback signal (initialised for first performance iteration)
    
    /*DC offset filter feedback signal*/
    aFBackSig    dcblock2    gaFBackSig            ; filter dc offset from left channel feedback signal
    if gkmonostereo==3 then                        ; if 'stereo in' mode is active...
     aFBackSigR    dcblock2    gaFBackSigR            ; filter dc offset from right channel feedback signal
    endif
    
    /*lowpass filter feedback signal*/
    if gkLPF_On==1 then                        ; if lowpass filter button is on...
     aFBackSig tone    aFBackSig,gkCutoff                ; ...filter left feedback channel
     if gkmonostereo==3 then                    ; if 'stereo in' mode is active...
      aFBackSigR tone    aFBackSigR,gkCutoff            ; lowpass filter the right channel
     endif
    endif

    /*clip feedback signal*/
    if gkclip==1 then                        ; if clip switch is on...
     ktrig    changed    gkClipLev                    ; if clip level control is adjusted generate a trigger impulse (momentary '1')
     if ktrig==1 then                        ; if a trigger impulse has been received...
      reinit    UPDATE_CLIP_L                    ; reinitialise clip opcode (clip level is i-rate only)
     endif
     UPDATE_CLIP_L:
     aFBackSig    clip    aFBackSig, 0, 0dbfs*i(gkClipLev)    ; clip left feedback signal at maximum amplitude using bram de jong method
     if gkmonostereo==3 then                    ; and if stereo in/out mode is also chosen
      aFBackSigR    clip    aFBackSigR, 0, 0dbfs*i(gkClipLev)    ; clip right channel feedback signal
      rireturn
     endif
    endif
    gaphsW    phasor    (sr*(1-gkfreeze))/ginsamp            ; pointer 0 - 1    ;create a moving phase value that will be used to point to locations in a function table where input audio signal will be written

    /*write audio from left input to function table*/
    if gkfreeze==0 then
        tablew    ain+aFBackSig,gaphsW,gibuffer,1            ; write input audio to table
        tablew    k(ain)*5,1-k(gaphsW),giview,1            ; write a value to the view table (k-rate is sufficient). Boost its amplitude before writing.
     if metro(32)==1 then                                ; peg rate of table updates
             chnset    "tableNumber(1)", "table"            ; update table display    
     endif

    endif
    
    /*if stereo in - stereo out mode*/
    if gkmonostereo==3 then                        ; if stereo in/out mode has been chosen...
     aR    inch    2                        ; read right channel audio input
     aR    =    aR*gkInGain                    ; reScale its amplitude with 'Input Gain' slider
     if gkfreeze==0 then
        tablew    aR+aFBackSigR,gaphsW,gibufferR,1        ; write right channel audio input audio to table
     endif
        outch    2,aR*(1-gkmix)                    ; if 'stereo in' mode is selected, send some right channel dry signal to output according to dry/wet 'Mix' control position
    else
        outch    2,ain*(1-gkmix)                    ; otherwise not 'stereo in' mode so just send some left channel dry signal to output
    endif
        clear    gaFBackSig,gaFBackSigR                ; clear feedback signals
        
    if changed(gkManPtr)==1 then
     Smsg    sprintfk    "pos(%d,250)",10+((gkManPtr+1)*508)
         chnset        Smsg,"wiper"
    endif
endin

instr    2
    iMIDIActiveValue    =    1                            ; IF MIDI ACTIVATED
    iMIDIflag        =    0                            ; IF FLTK ACTIVATED
    mididefault    iMIDIActiveValue, iMIDIflag                        ; IF NOTE IS MIDI ACTIVATED REPLACE iMIDIflag WITH iMIDIActiveValue 
    kMIDIflag    init    iMIDIflag
    if gkOnOff==0&&iMIDIflag==0 then
     turnoff
    endif

    if iMIDIflag==1 then
     icps    cpsmidi
     kpch    =    icps/cpsmidinn(gkUniNote)
    else
     kpch        =    gkpch
    endif    

    kporttime    linseg    0,0.001,0.03                            ; portamento time. Rises quickly from zero to a held value.
    kpch        portk    kpch,kporttime                            ; Apply portamento smoothing to changes made to the pitch multiplier
    apch    interp    kpch                                    ; interpolate pitch multiplier variable to create an a-rate version. This will produce higher quality results when pitch is modulated.
    
    kManPtr    portk    gkManPtr,kporttime*10*gkfreeze*gkManPtrPort

    kmetro    metro    5                                    ; peg rate of reinits
    if kmetro==1 then
     ktrig    changed    gkwsize,gkrnd,gkolap,gkwfn                        ; if any of the list of input args. change, generate a trigger impulse (momentary '1'). The input args are all i-rate in sndwarp so reinitialisation will be required for their changes to register.
    endif
    if ktrig==1 then                                    ; if a trigger has been generated... 
     reinit    UPDATE_SNDWARP                                    ; ... begin a reinitialisation pass from the given label
    endif
    UPDATE_SNDWARP:                                        ; a label. Reinitialisation begins from here.
    imode    =    1                                    ; sndwarp mode. (1=pointer mode, timestretch mode not so useful in a live audio in application)
    ibeg    =    0                                    ; point in the function table from which to begin reading audio (0=beginning)
    iwsize    =    i(gkwsize)                                ; window (grain) size in samples
    irnd    =    i(gkrnd)                                ; window (grain) size randomisation bandwidth in samples
    iolap    =    i(gkolap)                                ; number of grain overlaps
    kRndDly    betarand    gkdly,1,gkbeta                            ; random grain delay time
    if gkmonostereo!=0 then
     kRndDlyR    betarand    gkdly,1,gkbeta                        ; random grain delay time
    endif
    
    iMaxDur    =    (iwsize+irnd)/sr                            ; maximum grain duration in seconds
    kTransComp    limit    iMaxDur*(kpch-1),0,ginsamp/sr
    kdelay    =    (kTransComp+kRndDly) / (ginsamp/sr)                    ; delay time required when reading grains from the function table
    if gkmonostereo!=0 then
     kdelayR    =    (kTransComp+kRndDlyR) / (ginsamp/sr)                ; delay time required when reading grains from the function table
    endif
    if gkfreeze==1 then
     kdelay        =    kdelay + (sr/(ginsamp)*1.75*iwsize/sr)                ; if freeze mode is active regress the read pointer a small amount
     if gkmonostereo!=0 then
      kdelayR    =    kdelayR + (sr/(ginsamp)*1.75*iwsize/sr)                ; if freeze mode is active regress the read pointer a small amount
     endif
    endif
    aphsR    wrap    (gaphsW-kdelay+kManPtr)*(ginsamp/sr),0,(ginsamp-iwsize-irnd)/sr        ; location from which to read grain. This is always directly related to the poistion of the write pointer.
    aphsR_R    wrap    (gaphsW-kdelayR+kManPtr)*(ginsamp/sr),0,(ginsamp-iwsize-irnd)/sr    ; location from which to read grain. This is always directly related to the poistion of the write pointer.
    iwfn    =    giwfn1+i(gkwfn)-1                            ; Grain amplitude windowing shape
    
    /*sndwarp*/
    asig,ac    sndwarp 1, aphsR, apch, gibuffer, ibeg, iwsize, irnd, iolap, iwfn, imode
    if gkbalance==1 then                                    ; if 'balance switch is on...
     asig    balance    asig,ac                                    ; ... amplitude balance the signal
    endif
    if gkmonostereo==1 then                                    ; if 'mono' mode seleced...
     gaFBackSig    =    gaFBackSig+(asig*gkfback)                    ; create feedback signal for next iteration. (This will be written into the function table along with the live audio in.)
     aR    =    asig
    elseif gkmonostereo==2 then                                ; or if 'stereo out' mode
     aR,acR    sndwarp 1, aphsR_R, apch, gibuffer, ibeg, iwsize, irnd, iolap, iwfn, imode
     if gkbalance==1 then
      aR    balance    aR,acR
     endif
     gaFBackSig    =    gaFBackSig+((asig+aR)*gkfback)                    ; create feedback signal, a mixture of the left and right sndwarp output channels
    else                                            ; otherwise 'stereo in/out' mode
     aR,acR    sndwarp 1, aphsR_R, apch, gibufferR, ibeg, iwsize, irnd, iolap, iwfn, imode
     if gkbalance==1 then
      aR    balance    aR,acR
     endif
     gaFBackSig    =    gaFBackSig+(asig*gkfback)                    ; left channel feedback signal
     gaFBackSig    =    gaFBackSig+(aR*gkfback)                        ; right channel feedback signal
    endif    
    rireturn                                        ; return from reinitialisation
    aAntiClick    linsegr    0,0.03,1,0.03,0
        outs    asig*gkamp*gkmix*aAntiClick, aR*gkamp*gkmix*aAntiClick            ; send audio to outputs
endin

instr    UpdateWidgets
    ksemis    chnget    "semis"                                    ; read in 'semis' widget
    ktrig1    changed    ksemis                                    ; if 'semis' knob is moved...
    if ktrig1==1 then            
     chnset    semitone(ksemis), "pch"                                ; update 'Pitch' knob with the value of semis (converted to a ratio)
    endif
endin

</CsInstruments>

<CsScore>
i 1 0 [3600*24*7]
;i 2 0 [3600*24*7]
i "UpdateWidgets" 0 [3600*24*7]
</CsScore>


</CsoundSynthesizer>



























