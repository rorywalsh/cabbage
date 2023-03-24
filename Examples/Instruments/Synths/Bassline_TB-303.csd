
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; Bassline_TB-303.csd
; Written by Iain McCurdy, 2012, updated 2015

; An emulation of a Roland Bassline TB-303

; Hint: For instant gratification just click the 'Random' button.

; Note (16 mini rotary controls)    - set the note number of each step of the sequence. Notes can also be set by typing directly over the numbers beneath the knobs.
; On/Off         -    On/Off status of each step of the sequence
; Hold           -    If a hold button is active the filter envelope will not be retriggered for this step
; Cutoff         -    Manual control of filter cutoff frequency (moogladder opcode)
;            N.B. the filter follows the note being played so cutoff values set here are ratios of the current note
; Env.Mod.       -    Amount of envelope modification of the filter cutoff frequency. 
;            Note that this control can be set to negative values resulting in an inversion of the filter envelope.
; Follow         -    Amount by which filter cutoff follows the note being played. I think the authentic TB303 method is not to use any key following.
; Res.           -    Resonance of the filter
; Decay          -    Decay time of the filter envelope
; Dist.          -    Amount of waveshaping distortion (clip opcode)
; Reset          -    Cancel all sequence button settings
; Random         -    Set 'Note', 'On', 'Hold' and 'Accent' sequence control to random values
; -semi/+semi    -    Increment all 'Note' controls up or down 1 semitone step 
; -octave/+octave-    Increment all 'Note' controls up or down 1 octave step
; Gliss          -    glissando time from note to note (where two notes are tied with an active 'Hold' button)
; -- A bandpass filter is used to EQ the overall sound and to fatten the bass response of the synth --
; BPF.CF.        -    Cutoff frequency of a bandpass filter (resonr). This is a fixed frequency filter, i.e. doesn't follow the note being played 
; BPF.BW.        -    Bandwidth of the bandpass filter
; BPF.Bal.       -    Balancing method of the bandpass filter (resonr).     0=none 
;                                        1=peak_response_of_1 
;                                        2=RMS_of_1
; BPF.Mix        -    Mix between un-bandpass filtered sound and bandpass filtered sound.
; Attack         -    Attack time for the main lowpass filter (moogladder) envelope
; Phr.Len.       -    Phrase length in steps
; Tempo          -    Tempo in beats per minute (this control is hidden if 'Clock: External' is selected
; Volume         -    Volume control
; Filter         -    (drop down menu) choose between a moogladder filter and an LPF18 filter. moogladder sounds fatter but LPF18 sounds better when used with distortion
; Waveform       -    (drop down menu) basic waveform type. (Sawtooth, Square, Triangle, Pulse Width Modulation (PWM) or Noise)
;                     If PWM is selected a control for pulse width is revealed
; Sub.           -    If active a sub-oscillator (1 octave below the main oscillator) is added
; Clock:---      -    Clock source internal (Tempo knob) or external (VST host)
<Cabbage>
#define RSliderStyle trackerColour(255,255,255), colour(  0, 55,100), textColour(0,0,0), outlineColour(50,50,50), valueTextBox(1), textBox(1), fontColour("black")

form caption("Bassline TB-303") size(615,335), pluginId("T303"), guiRefresh(64)
image                bounds(  0,  0,615,335), colour("grey"), shape("sharp"), outlineColour("white"), line(4)
checkbox bounds( 65,  8,  5,  5), channel("Act1"),  value(0), colour("yellow"), shape("ellipse"), active(0)
checkbox bounds(100,  8,  5,  5), channel("Act2"),  value(0), colour("yellow"), shape("ellipse"), active(0)
checkbox bounds(135,  8,  5,  5), channel("Act3"),  value(0), colour("yellow"), shape("ellipse"), active(0)
checkbox bounds(170,  8,  5,  5), channel("Act4"),  value(0), colour("yellow"), shape("ellipse"), active(0)
checkbox bounds(205,  8,  5,  5), channel("Act5"),  value(0), colour("yellow"), shape("ellipse"), active(0)
checkbox bounds(240,  8,  5,  5), channel("Act6"),  value(0), colour("yellow"), shape("ellipse"), active(0)
checkbox bounds(275,  8,  5,  5), channel("Act7"),  value(0), colour("yellow"), shape("ellipse"), active(0)
checkbox bounds(310,  8,  5,  5), channel("Act8"),  value(0), colour("yellow"), shape("ellipse"), active(0)
checkbox bounds(345,  8,  5,  5), channel("Act9"),  value(0), colour("yellow"), shape("ellipse"), active(0)
checkbox bounds(380,  8,  5,  5), channel("Act10"), value(0), colour("yellow"), shape("ellipse"), active(0)
checkbox bounds(415,  8,  5,  5), channel("Act11"), value(0), colour("yellow"), shape("ellipse"), active(0)
checkbox bounds(450,  8,  5,  5), channel("Act12"), value(0), colour("yellow"), shape("ellipse"), active(0)
checkbox bounds(485,  8,  5,  5), channel("Act13"), value(0), colour("yellow"), shape("ellipse"), active(0)
checkbox bounds(520,  8,  5,  5), channel("Act14"), value(0), colour("yellow"), shape("ellipse"), active(0)
checkbox bounds(555,  8,  5,  5), channel("Act15"), value(0), colour("yellow"), shape("ellipse"), active(0)
checkbox bounds(590,  8,  5,  5), channel("Act16"), value(0), colour("yellow"), shape("ellipse"), active(0)

label    bounds( 10, 43, 40, 12), text("Note"), fontColour("black")
rslider  bounds( 45, 15, 45, 65), text("1"),  channel("Note1"),  range(12, 96, 60, 1, 1), valueTextBox(1), textBox(1), fontColour("black"), $RSliderStyle
rslider  bounds( 80, 15, 45, 65), text("2"),  channel("Note2"),  range(12, 96, 60, 1, 1), valueTextBox(1), textBox(1), fontColour("black"), $RSliderStyle
rslider  bounds(115, 15, 45, 65), text("3"),  channel("Note3"),  range(12, 96, 60, 1, 1), valueTextBox(1), textBox(1), fontColour("black"), $RSliderStyle
rslider  bounds(150, 15, 45, 65), text("4"),  channel("Note4"),  range(12, 96, 60, 1, 1), valueTextBox(1), textBox(1), fontColour("black"), $RSliderStyle
rslider  bounds(185, 15, 45, 65), text("5"),  channel("Note5"),  range(12, 96, 60, 1, 1), valueTextBox(1), textBox(1), fontColour("black"), $RSliderStyle
rslider  bounds(220, 15, 45, 65), text("6"),  channel("Note6"),  range(12, 96, 60, 1, 1), valueTextBox(1), textBox(1), fontColour("black"), $RSliderStyle
rslider  bounds(255, 15, 45, 65), text("7"),  channel("Note7"),  range(12, 96, 60, 1, 1), valueTextBox(1), textBox(1), fontColour("black"), $RSliderStyle
rslider  bounds(290, 15, 45, 65), text("8"),  channel("Note8"),  range(12, 96, 60, 1, 1), valueTextBox(1), textBox(1), fontColour("black"), $RSliderStyle
rslider  bounds(325, 15, 45, 65), text("9"),  channel("Note9"),  range(12, 96, 60, 1, 1), valueTextBox(1), textBox(1), fontColour("black"), $RSliderStyle
rslider  bounds(360, 15, 45, 65), text("10"), channel("Note10"), range(12, 96, 60, 1, 1), valueTextBox(1), textBox(1), fontColour("black"), $RSliderStyle
rslider  bounds(395, 15, 45, 65), text("11"), channel("Note11"), range(12, 96, 60, 1, 1), valueTextBox(1), textBox(1), fontColour("black"), $RSliderStyle
rslider  bounds(430, 15, 45, 65), text("12"), channel("Note12"), range(12, 96, 60, 1, 1), valueTextBox(1), textBox(1), fontColour("black"), $RSliderStyle
rslider  bounds(465, 15, 45, 65), text("13"), channel("Note13"), range(12, 96, 60, 1, 1), valueTextBox(1), textBox(1), fontColour("black"), $RSliderStyle
rslider  bounds(500, 15, 45, 65), text("14"), channel("Note14"), range(12, 96, 60, 1, 1), valueTextBox(1), textBox(1), fontColour("black"), $RSliderStyle
rslider  bounds(535, 15, 45, 65), text("15"), channel("Note15"), range(12, 96, 60, 1, 1), valueTextBox(1), textBox(1), fontColour("black"), $RSliderStyle
rslider  bounds(570, 15, 45, 65), text("16"), channel("Note16"), range(12, 96, 60, 1, 1), valueTextBox(1), textBox(1), fontColour("black"), $RSliderStyle

label    bounds( 10, 82, 40, 12), text("On/Off"), fontColour("black"), FontColour("black")
checkbox bounds( 61, 82, 12, 12), channel("On1"),  value(0)
checkbox bounds( 96, 82, 12, 12), channel("On2"),  value(0)
checkbox bounds(131, 82, 12, 12), channel("On3"),  value(0)
checkbox bounds(166, 82, 12, 12), channel("On4"),  value(0)
checkbox bounds(201, 82, 12, 12), channel("On5"),  value(0)
checkbox bounds(236, 82, 12, 12), channel("On6"),  value(0)
checkbox bounds(271, 82, 12, 12), channel("On7"),  value(0)
checkbox bounds(306, 82, 12, 12), channel("On8"),  value(0)
checkbox bounds(341, 82, 12, 12), channel("On9"),  value(0)
checkbox bounds(376, 82, 12, 12), channel("On10"), value(0)
checkbox bounds(411, 82, 12, 12), channel("On11"), value(0)
checkbox bounds(446, 82, 12, 12), channel("On12"), value(0)
checkbox bounds(481, 82, 12, 12), channel("On13"), value(0)
checkbox bounds(516, 82, 12, 12), channel("On14"), value(0)
checkbox bounds(551, 82, 12, 12), channel("On15"), value(0)
checkbox bounds(586, 82, 12, 12), channel("On16"), value(0)

label    bounds( 11,100, 40, 12), text("Hold"), fontColour("black"), FontColour("black")
checkbox bounds( 61,100, 12, 12), channel("Hold1"),  value(0)
checkbox bounds( 96,100, 12, 12), channel("Hold2"),  value(0)
checkbox bounds(131,100, 12, 12), channel("Hold3"),  value(0)
checkbox bounds(166,100, 12, 12), channel("Hold4"),  value(0)
checkbox bounds(201,100, 12, 12), channel("Hold5"),  value(0)
checkbox bounds(236,100, 12, 12), channel("Hold6"),  value(0)
checkbox bounds(271,100, 12, 12), channel("Hold7"),  value(0)
checkbox bounds(306,100, 12, 12), channel("Hold8"),  value(0)
checkbox bounds(341,100, 12, 12), channel("Hold9"),  value(0)
checkbox bounds(376,100, 12, 12), channel("Hold10"), value(0)
checkbox bounds(411,100, 12, 12), channel("Hold11"), value(0)
checkbox bounds(446,100, 12, 12), channel("Hold12"), value(0)
checkbox bounds(481,100, 12, 12), channel("Hold13"), value(0)
checkbox bounds(516,100, 12, 12), channel("Hold14"), value(0)
checkbox bounds(551,100, 12, 12), channel("Hold15"), value(0)
checkbox bounds(586,100, 12, 12), channel("Hold16"), value(0)

label    bounds( 11,118, 40, 12), text("Accent"), fontColour("black"), FontColour("black")
checkbox bounds( 61,118, 12, 12), channel("Acc1"),  value(0)
checkbox bounds( 96,118, 12, 12), channel("Acc2"),  value(0)
checkbox bounds(131,118, 12, 12), channel("Acc3"),  value(0)
checkbox bounds(166,118, 12, 12), channel("Acc4"),  value(0)
checkbox bounds(201,118, 12, 12), channel("Acc5"),  value(0)
checkbox bounds(236,118, 12, 12), channel("Acc6"),  value(0)
checkbox bounds(271,118, 12, 12), channel("Acc7"),  value(0)
checkbox bounds(306,118, 12, 12), channel("Acc8"),  value(0)
checkbox bounds(341,118, 12, 12), channel("Acc9"),  value(0)
checkbox bounds(376,118, 12, 12), channel("Acc10"), value(0)
checkbox bounds(411,118, 12, 12), channel("Acc11"), value(0)
checkbox bounds(446,118, 12, 12), channel("Acc12"), value(0)
checkbox bounds(481,118, 12, 12), channel("Acc13"), value(0)
checkbox bounds(516,118, 12, 12), channel("Acc14"), value(0)
checkbox bounds(551,118, 12, 12), channel("Acc15"), value(0)
checkbox bounds(586,118, 12, 12), channel("Acc16"), value(0)

button   bounds( 10,150, 70, 25), fontColour:0(50,50,50), fontColour:1(205,255,205), colour:0(0,10,0), colour:1(0,150,0), text("Play","Play"), channel("Play"), latched(1), radioGroup(1), value(1), identChannel("PlayButton")
button   bounds( 10,180, 70, 25), fontColour:0(50,50,50), fontColour:1(255,205,205), colour:0(10,0,0), colour:1(150,0,0), text("Stop","Stop"), channel("Stop"), latched(1), radioGroup(1), identChannel("StopButton")

rslider  bounds( 80,140, 60, 80), text("Cutoff"),  channel("CfBase"),  range(0,1,0.3),              $RSliderStyle
rslider  bounds(135,140, 60, 80), text("Env.Mod"), channel("CfEnv"),   range(-1,1.00, 0.38),        $RSliderStyle
rslider  bounds(190,140, 60, 80), text("Follow"),  channel("CfFol"),   range(0,1.00, 0),            $RSliderStyle
rslider  bounds(245,140, 60, 80), text("Res."),    channel("Res"),     range(0,0.98,0.45),          $RSliderStyle
rslider  bounds(300,140, 60, 80), text("Attack"),  channel("Attack"),  range(0,1,0.01,0.5),         $RSliderStyle
rslider  bounds(355,140, 60, 80), text("Decay"),   channel("Decay"),   range(0.01,5,0.5,0.5,0.001), $RSliderStyle
rslider  bounds(410,140, 60, 80), text("Accent"),  channel("Accent"),  range(0, 1.00, .5),          $RSliderStyle

image                bounds(140,222,215,81), colour(155,155,155), shape("sharp")

rslider  bounds( 80,220, 60, 80), text("Gliss"),     channel("NoteGliss"),     range(0, 0.03, 0.01,0.75,0.001),        $RSliderStyle
rslider  bounds(135,220, 60, 80), text("BPF.CF."),   channel("BPF_StaticCF"),  range(50,3000,100,0.5,1),        $RSliderStyle
rslider  bounds(190,220, 60, 80), text("BPF.BW."),   channel("BPF_StaticBW"),  range(0.1,99,1,0.5,0.01),        $RSliderStyle
rslider  bounds(245,220, 60, 80), text("BPF.Bal."),  channel("BPF_StaticBal"), range(0,2,1,1,1),        $RSliderStyle
rslider  bounds(300,220, 60, 80), text("BPF.Mix"),   channel("BPF_StaticMix"), range(0,1,0.3),        $RSliderStyle
rslider  bounds(355,220, 60, 80), text("Dist."),     channel("Dist"), range(0,1.00, 0,1,0.001), textColour("black"), $RSliderStyle
rslider  bounds(410,220, 60, 80), text("Dist.LPF."), channel("Dist_LPF"),     range(50,20000,4000,0.5,1), identChannel("Dist_LPF_ID"),       $RSliderStyle

rslider  bounds(470,220, 70, 90), text("Tempo"),   channel("Tempo"),   range(1, 500, 110, 1, 1), identChannel("TempoID"), $RSliderStyle
rslider  bounds(535,220, 70, 90), text("Vol."),    channel("Vol"),     range(0, 5.00, 1),        $RSliderStyle

label      bounds(480,146, 90, 12), text("Phrase Length:"), fontColour("black")                                                                       
nslider  bounds(570,141, 30, 24), channel("PhrLen"),       range(1,16,16,1,1)
button     bounds(480,165, 60, 15), text("Reset"),   channel("Reset"),  value(0), latched(0)
button     bounds(540,165, 60, 15), text("Random"),  channel("Rnd"),    value(0), latched(0)
button     bounds(480,185, 60, 15), text("-semi"),   channel("SemiDn"), value(0), latched(0)
button     bounds(540,185, 60, 15), text("+semi"),   channel("SemiUp"), value(0), latched(0)
button     bounds(480,205, 60, 15), text("-oct"),    channel("OctDn"),  value(0), latched(0)
button     bounds(540,205, 60, 15), text("+oct"),    channel("OctUp"),  value(0), latched(0)

label    bounds( 10,207, 70, 12), text("Filter"), fontColour("black")                                                                       
combobox bounds( 10,220, 70, 20), channel("FiltType"), value(1), text("303","LPF18"), fontColour("white")
label    bounds( 10,247, 70, 12), text("Waveform"), fontColour("black")                                                                       
combobox bounds( 10,260, 70, 20), channel("Waveform"), value(1), text("Saw","Square","Triangle","PWM","Noise"), fontColour("white")
hslider  bounds( 10,308,270, 25), text("P.W."),    channel("pw"),      range(0.01,0.99, 0.25), identChannel("pwID"), visible(0), trackerColour(255,255,255), colour(  0, 55,100), textColour(0,0,0), outlineColour(50,50,50)
checkbox bounds(280,313, 45, 13), text("Sub."), channel("sub"), value(0), fontColour:0("black"), fontColour:1("black")
button   bounds(330,310,100, 20), text("Clock: Internal","Clock: External"), channel("ClockSource"), value(0)                                                   
label    bounds(435,321,165, 12), text("Author: Iain McCurdy |2012|"), fontColour("black")
                                                                       
</Cabbage>                                                             
                                                                       
<CsoundSynthesizer>                                                    
                                                                       
<CsOptions>                                                            
-dm0 -n
</CsOptions>                                                           
                                                                       
<CsInstruments>                                                        
                                                                       
; sr set by host
ksmps         =     32    ;NUMBER OF AUDIO SAMPLES IN EACH CONTROL CYCLE
nchnls         =     2    ;NUMBER OF CHANNELS (2=STEREO)         
0dbfs        =    1                                              
        seed    0                                              

giAmpScl    ftgen    0,0,128,-16,1,127,-2,0.2    ; NOTE TO AMPLITUDE MAPPING. USED TO ATTENUATE AMPLITUDE OF HIGHER NOTES.

;Author: Iain McCurdy (2012)                                           
                                                                       
opcode    resonsr,a,akki
    asig,kcf,kbw,iscal    xin
        setksmps    1
    acf    interp    kcf
    kcf    downsamp    acf
    abw    interp    kbw
    kbw    downsamp    abw
    asig      reson   asig, kcf, kbw,iscal
        xout    asig
endop

instr    1    ;BASSLINE INSTRUMENT
    kporttime    linseg    0,0.001,0.01    ;PORTAMENTO TIME RAMPS UP QUICKLY FROM ZERO TO A HELD VALUE
    ;READ IN WIDGETS...
    ;gkOnOff        chnget    "OnOff"
    gkPlay        chnget    "Play"
    gkStop        chnget    "Stop"
    if trigger(gkPlay,0.5,0)==1 then
     kOnOff    =    1
    elseif trigger(gkStop,0.5,0)==1 then
     kOnOff    =    0
    endif
    gkOnOff    init    0

    gkNote1        chnget    "Note1"
    gkNote2        chnget    "Note2"
    gkNote3        chnget    "Note3"
    gkNote4        chnget    "Note4"
    gkNote5        chnget    "Note5"
    gkNote6        chnget    "Note6"
    gkNote7        chnget    "Note7"
    gkNote8        chnget    "Note8"
    gkNote9        chnget    "Note9"
    gkNote10    chnget    "Note10"
    gkNote11    chnget    "Note11"
    gkNote12    chnget    "Note12"
    gkNote13    chnget    "Note13"
    gkNote14    chnget    "Note14"
    gkNote15    chnget    "Note15"
    gkNote16    chnget    "Note16"
    gkOn1        chnget    "On1"
    gkOn2        chnget    "On2"
    gkOn3        chnget    "On3"
    gkOn4        chnget    "On4"
    gkOn5        chnget    "On5"
    gkOn6        chnget    "On6"
    gkOn7        chnget    "On7"
    gkOn8        chnget    "On8"
    gkOn9        chnget    "On9"
    gkOn10        chnget    "On10"
    gkOn11        chnget    "On11"
    gkOn12        chnget    "On12"
    gkOn13        chnget    "On13"
    gkOn14        chnget    "On14"
    gkOn15        chnget    "On15"
    gkOn16        chnget    "On16"
    gkHold1        chnget    "Hold1"
    gkHold2        chnget    "Hold2"
    gkHold3        chnget    "Hold3"
    gkHold4        chnget    "Hold4"
    gkHold5        chnget    "Hold5"
    gkHold6        chnget    "Hold6"
    gkHold7        chnget    "Hold7"
    gkHold8        chnget    "Hold8"
    gkHold9        chnget    "Hold9"
    gkHold10    chnget    "Hold10"
    gkHold11    chnget    "Hold11"
    gkHold12    chnget    "Hold12"
    gkHold13    chnget    "Hold13"
    gkHold14    chnget    "Hold14"
    gkHold15    chnget    "Hold15"
    gkHold16    chnget    "Hold16"
    gkAcc1    chnget    "Acc1"
    gkAcc2    chnget    "Acc2"
    gkAcc3    chnget    "Acc3"
    gkAcc4    chnget    "Acc4"
    gkAcc5    chnget    "Acc5"
    gkAcc6    chnget    "Acc6"
    gkAcc7    chnget    "Acc7"
    gkAcc8    chnget    "Acc8"
    gkAcc9    chnget    "Acc9"
    gkAcc10    chnget    "Acc10"
    gkAcc11    chnget    "Acc11"
    gkAcc12    chnget    "Acc12"
    gkAcc13    chnget    "Acc13"
    gkAcc14    chnget    "Acc14"
    gkAcc15    chnget    "Acc15"
    gkAcc16    chnget    "Acc16"
    gkVol    chnget    "Vol"
    gkCfBase    chnget    "CfBase"
    gkCfBase    portk    gkCfBase,kporttime*5
    gkCfEnv    chnget    "CfEnv"
    gkCfEnv    portk    gkCfEnv,kporttime*5
    gkCfFol    chnget    "CfFol"
    gkFiltType    chnget    "FiltType"
    gkRes    chnget    "Res"
    gkDist    chnget    "Dist"
    gkDecay    chnget    "Decay"
    gkAccent    chnget    "Accent"
    gkNoteGliss    chnget    "NoteGliss"
    gkDist_LPF    chnget    "Dist_LPF"    
    gkBPF_StaticCF    chnget    "BPF_StaticCF"    
    gkBPF_StaticBW    chnget    "BPF_StaticBW"    
    gkBPF_StaticBal    chnget    "BPF_StaticBal"    
    gkBPF_StaticMix    chnget    "BPF_StaticMix"    
    gkAttack    chnget    "Attack"    
    gkPhrLen    chnget    "PhrLen"    
    gkWaveform    chnget    "Waveform"
    kpw    chnget    "pw"
    ksub    chnget    "sub"
    gkReset    chnget    "Reset"
    gkRnd    chnget    "Rnd"
    kSemiDn    chnget    "SemiDn"
    kSemiUp    chnget    "SemiUp"
    kOctDn    chnget    "OctDn"
    kOctUp    chnget    "OctUp"
    kClockSource    chnget    "ClockSource"

    /* SHOW/HIDE DISTORTION FILTER CONTROL */
    if trigger(gkDist,0.0005,2)==1 then
     kvis    =    gkDist>0?1:0
     chnset    sprintfk("visible(%d)",kvis),"Dist_LPF_ID"
    endif

    /* SHOW/HIDE PULSE WIDTH CONTROL AS REQUIRED */
    if changed(gkWaveform)==1 then
     kvis    =    gkWaveform==4?1:0
     chnset    sprintfk("visible(%d)",kvis), "pwID"
    endif
    
    /* SET TEMPO AND PLAY/STOP ACCORDING TO CLOCK SOURCE */
    if kClockSource==0 then
     gkTempo    chnget    "Tempo"
     gkOnOff    =    kOnOff
    else
     gkTempo    chnget    "HOST_BPM"
     gkTempo    limit    gkTempo,1,2000
     gkOnOff    chnget    "IS_PLAYING"
    endif
    
    /* SHOW/HIDE TEMPO CONTROL */
    if changed(kClockSource)==1||timeinstk()==1 then
     chnset    sprintfk("visible(%d)",1-kClockSource),"TempoID"
    endif
    
    kStop    trigger    gkOnOff,0.5,1    ; IS LOOP PLAYING TURNED OFF...
    if kStop==1 then
     chnset k(0),"Act1"
     chnset k(0),"Act2"
     chnset k(0),"Act3"
     chnset k(0),"Act4"
     chnset k(0),"Act5"
     chnset k(0),"Act6"
     chnset k(0),"Act7"
     chnset k(0),"Act8"
     chnset k(0),"Act9"
     chnset k(0),"Act10"
     chnset k(0),"Act11"
     chnset k(0),"Act12"
     chnset k(0),"Act13"
     chnset k(0),"Act14"
     chnset k(0),"Act15"
     chnset k(0),"Act16"
     reinit RESET_LOOP
    endif
    RESET_LOOP:

    if gkOnOff==0 kgoto SKIP
    kPhFreq   =            gkTempo/240    ;FREQUENCY WITH WHICH TO REPEAT THE ENTIRE PHRASE
    kBtFreq   =            gkTempo/15    ;FREQUENCY OF EACH 1/16TH NOTE
    kNewNote    metro    kBtFreq        ;GENERATES A TRIGGER FOR EACH BEAT
    
    ; FORCED RETRIGGERING OF SEQUENCE
    kStep    init    1            ; INITIAL STEP VALUE
    if kNewNote==1 then            ; IF A NEW NOTE TRIGGER HAS BEEN GENERATED... 
     kStep    +=    1            ; INCREMENT STEP VALUE
    endif
    
    if kStep==(gkPhrLen+1) then        ; IF END OF SEQUENCE (+1) HAS BEEN REACHED...
     kReTrigPhr    =    1        ; ...GENERATE A 'RESTART SEQUENCE' TRIGGER
     kStep        =    1        ; AND RESET STEP VALUE TO THE BEGINNING OF THE SEQUENCE
    else
     kReTrigPhr    =    0        ; ...OTHERWISE NO TRIGGER
    endif     
    
    scoreline "i 100 0 0", kReTrigPhr    ; CALL INSTRUMENT TO CLEAR ALL INDICATOR WHENEVER A SEQUENCE IS RETRIGGERED (PREVENTS STUCK INDICATORS)
    
    ;ENVELOPES WITH HELD SEGMENTS
    ;                      freq     trig      ph  NOTE:1 (dur) 2            3            4            5            6            7            8            9            10            11            12            13            14            15            16             DUMMY
    kNoteLoop lpshold      kPhFreq, kReTrigPhr,        0,  gkNote1, 1,  gkNote2, 1,  gkNote3, 1,  gkNote4, 1,  gkNote5, 1,  gkNote6, 1,  gkNote7, 1,  gkNote8, 1,  gkNote9, 1,  gkNote10, 1,  gkNote11, 1,  gkNote12, 1,  gkNote13, 1,  gkNote4,  1,  gkNote15, 1,  gkNote16,   1  ;,gkNote1    ;NEED AN EXTRA 'DUMMY' VALUE
    kOn       lpshold      kPhFreq, kReTrigPhr,        0,  gkOn1,   1,  gkOn2,   1,  gkOn3,   1,  gkOn4,   1,  gkOn5,   1,  gkOn6,   1,  gkOn7,   1,  gkOn8,   1,  gkOn9,   1,  gkOn10,   1,  gkOn11,   1,  gkOn12,   1,  gkOn13,   1,  gkOn14,   1,  gkOn15,   1,  gkOn16,     1  ;,1        ;NEED AN EXTRA 'DUMMY' VALUE
    kAcc      lpshold      kPhFreq, kReTrigPhr,        0,  gkAcc1,  1,  gkAcc2,  1,  gkAcc3,  1,  gkAcc4,  1,  gkAcc5,  1,  gkAcc6,  1,  gkAcc7,  1,  gkAcc8,  1,  gkAcc9,  1,  gkAcc10,  1,  gkAcc11,  1,  gkAcc12,  1,  gkAcc13,  1,  gkAcc14,  1,  gkAcc15,  1,  gkAcc16,    1  ;,1        ;NEED AN EXTRA 'DUMMY' VALUE
    kHoldLoop lpshold      kPhFreq, kReTrigPhr,        0,  gkHold1, 1,  gkHold2, 1,  gkHold3, 1,  gkHold4, 1,  gkHold5, 1,  gkHold6, 1,  gkHold7, 1,  gkHold8, 1,  gkHold9, 1,  gkHold10, 1,  gkHold11, 1,  gkHold12, 1,  gkHold13, 1,  gkHold14, 1,  gkHold15, 1,  gkHold16,   1  ;,0        ;NEED AN EXTRA 'DUMMY' VALUE
    
    kNewOn    trigger    kOn,0.5,0            ; SENSE A NOTE ON FOLLOWING A NOTE OFF
    kAcc    portk    kAcc*gkAccent,kporttime        ; SCALE ACCENT USING ON-SCREEN KNOB AND SMOOTH CHANGES IN ACCENT VALUE TO PREVENT CLICKS
    
    ;IGNORE PITCH VALUES AND HOLD VALUES FOR 'OFF' NOTES
    kNote    init    i(gkNote1)
    if kOn==1 then            ;IF THIS IS AN 'ON' NOTE...
     kNote = kNoteLoop        ;USE NOTE LOOP VALUE FOR PCH
     kHold = kHoldLoop        ;USE HOLD LOOP VALUE FOR HOLD
    endif    
    
    kHoldDel  vdel_k       kHold, 1/(kBtFreq*2), 1    ;OFFSET HOLD SO THAT ITS VALUE WILL OVERLAP THE NEXT NOTE
    kNote      portk        kNote, gkNoteGliss*kHoldDel*kOn    ;APPLY PORTAMENTO TO PITCH CHANGES - IF NOTE IS NOT HELD, NO PORTAMENTO WILL BE APPLIED
        
    /* FILTER ENVELOPE */
    kFiltRetrig    =    (1-kHoldDel) * kNewNote * kOn ; + kOnTrig
    kFundOct    =    octmidinn(kNote)    ; FUNDEMENTAL NOTE IN OCT FORMAT
        
    /* FILTER CUTOFF ENVELOPE */
    /* KEY FOLLOWING */
    kCfBase1    scale    gkCfBase,8,-2
    kCfEnv1        scale    (gkCfEnv*0.5)+0.5,8,-8
    ;                       freq    trig                ph value              time      value                               t        v                  t                      v                  t
    kCfOct1      loopseg    1/3600, kFiltRetrig+kNewOn, 0, kFundOct+kCfBase1, gkAttack, kFundOct+kCfBase1+kCfEnv1+(kAcc*2), gkDecay, kFundOct+kCfBase1, 3600-gkDecay-gkAttack, kFundOct+kCfBase1, 0
    /* NO KEY FOLLOW */
    ;                       freq    trig                ph v                          t        v         t             v         t(?)
    kCfBase2    scale    gkCfBase,14,4
    kCfEnv2        scale    (gkCfEnv*0.5)+0.5,8,-8
    kCfOct2      loopseg    1/3600, kFiltRetrig+kNewOn, 0, kCfBase2, gkAttack, kCfBase2+kCfEnv2+(kAcc*2), gkDecay, kCfBase2, 3600-gkAttack-gkDecay, kCfBase2, 0
    /* MIX KEY FOLLOWING AND NO KEY FOLLOWING */
    kCfOct    ntrpol    kCfOct2,kCfOct1,gkCfFol
    kCfOct    limit        kCfOct, 4, 14    ;LIMIT THE CUTOFF FREQUENCY TO BE WITHIN SENSIBLE LIMITS


    /* AMPLITUDE ENVELOPE - SEPARATES NOTES THAT ARE NOT HELD */
    katt    =    0.02 * (60/gkTempo)
    kdec    =    0.02 * (60/gkTempo)
    ;                      freq     trig  ph   v   tim   v   t                      v   t    v  t(?)
    kAmpEnv   loopseg      kBtFreq, 0,    0,   0,  katt, 1,  (1/kBtFreq)-katt-kdec, 1,  kdec,0, 0    ;SUSTAIN SEGMENT DURATION (AND THEREFORE ATTACK AND DECAY SEGMENT DURATIONS) ARE DEPENDENT UPON TEMPO
    kAmp    =    (kHoldDel==1?1:kAmpEnv)
    
    /* AUDIO OSCILLATOR */
    kWavTrig  changed      gkWaveform        ;GENERATE A 'BANG' IF WAVEFORM SELECTOR CHANGES
    if kWavTrig=1 then                ;IF A 'BANG' HAS BEEN GENERATED...
     reinit REINIT_VCO                ;BEGIN A REINITIALISATION PASS FROM THE LABEL 'REINIT_VCO'
    endif
    REINIT_VCO:                            ;A LABEL
    if     i(gkWaveform)==1 then    ;SAWTOOTH
     imode    =    0
    elseif i(gkWaveform)==2 then    ;SQUARE
     imode    =    10
    elseif i(gkWaveform)==3 then    ;TRIANGLE
     imode    =    12
    elseif i(gkWaveform)==4 then    ;PWM
     imode    =    2
    elseif i(gkWaveform)==5 then    ;NOISE
     anoise    pinkish    1
     aSig    resonsr    anoise,cpsmidinn(kNote),cpsmidinn(kNote)*0.005,1
     if ksub==1 then                            ;CONDITIONALLY ADD IN SUB-OCTAVE OSCILLATOR
      aSub      resonsr    anoise,cpsmidinn(kNote-12),cpsmidinn(kNote-12)*0.005,1
      aSig    +=        aSub                    ;MIX IN SUB-OSCILLATOR    
     endif
     aSig    =    aSig*20
     goto SKIP_VCO2
    endif
    kpw    portk    kpw,kporttime
    aSig      vco2         0.2, cpsmidinn(kNote), imode, kpw    ;GENERATE AUDIO USING VCO OSCILLATOR
    if ksub==1 then                            ;CONDITIONALLY ADD IN SUB-OCTAVE OSCILLATOR
     aSub      vco2         0.2, cpsmidinn(kNote-12), imode, kpw    ;GENERATE AUDIO USING VCO OSCILLATOR (1 OCTAVE LOWER)
     aSig    +=        aSub                    ;MIX IN SUB-OSCILLATOR    
    endif
    SKIP_VCO2:
    rireturn                            ;RETURN FROM INITIALIZATION PASS TO PERFORMANCE PASSES

    kres    limit    gkRes+(kAcc*0.4),0,0.98                ;PREVENT EXCESSIVE RESONANCE THAT COULD RESULT FROM THE COMBINATION OF RESONANCE SETTING AND ACCENTING
        
    /* FILTER */
    if gkFiltType==1 then                        ;MOOGLADDER
     ;aSig    moogladder    aSig,cpsoct(kCfOct),kres
     aSig   diode_ladder aSig, cpsoct(kCfOct), kres*17;, 1, 100
    else                                ;LPF18
      aSig    lpf18    aSig,a(cpsoct(kCfOct)),kres, (gkDist^2)*20
     iSclGain2  ftgentmp     0, 0, 1024, -16, 1, 1024,   -8, 0.1    ;RESCALING CURVE FOR GAIN COMPENSATION
     if gkDist>0 then
      kGain     table        gkDist, iSclGain2, 1            ;READ GAIN VALUE FROM RESCALING CURVE
      aSig    *=    kGain
      aSig    butlp    aSig, gkDist_LPF                ;TONE WHEN DISTORTION IS ACTIVE
     endif
    endif
    
    /* STATIC BANDPASS FILTER (USED TO FATTEN THE OVERALL SOUND) */
    if changed(gkBPF_StaticBal)==1 then
     reinit UPDATE_RESONZ
    endif
    UPDATE_RESONZ:
    af1    resonz    aSig,gkBPF_StaticCF,gkBPF_StaticCF*gkBPF_StaticBW,i(gkBPF_StaticBal)
    rireturn
    aSig    ntrpol    aSig,af1*5,gkBPF_StaticMix  

    /* DISTORTION */
    if gkFiltType==1 then    ; (moogladder)
     if gkDist==0 kgoto SKIP_DISTORTION 
     iSclLimit ftgentmp     0, 0, 1024, -16, 1, 1024,  -8, 0.01    ;RESCALING CURVE FOR CLIP 'LIMIT' PARAMETER
     iSclGain  ftgentmp     0, 0, 1024, -16, 1, 1024,   2, 10    ;RESCALING CURVE FOR GAIN COMPENSATION
     kLimit    table        gkDist, iSclLimit, 1            ;READ LIMIT VALUE FROM RESCALING CURVE
     kGain     table        gkDist, iSclGain, 1            ;READ GAIN VALUE FROM RESCALING CURVE
     kTrigDist changed      kLimit                    ;IF LIMIT VALUE CHANGES GENERATE A 'BANG'
     if kTrigDist=1 then                        ;IF A 'BANG' HAS BEEN GENERATED...
         reinit REINIT_CLIP                    ;BEGIN A REINITIALIZATION PASS FROM LABEL 'REINIT_CLIP'
     endif
     REINIT_CLIP:
     aSig      clip         aSig*(1+gkDist), 0, i(kLimit)            ;CLIP DISTORT AUDIO SIGNAL

     rireturn                            ;
     aSig      =            aSig * kGain                ;COMPENSATE FOR GAIN LOSS FROM 'CLIP' PROCESSING
     aSig    butlp    aSig, gkDist_LPF                ;TONE WHEN DISTORTION IS ACTIVE
     SKIP_DISTORTION:
    endif
        
    kOn    port    kOn, 0.006                    ;SMOOTH CHANGES IN ON OFF SWITCHING
    kAmpScl    tablei    kNote,giAmpScl                    ;NOTE TO AMPLITUDE MAPPING. USED TO ATTENUATE AMPLITUDE OF HIGHER NOTES.
    aAmp    interp    ((kAmp*kOn)*((kAcc*0.7)+1)*gkVol*kAmpScl)    ;COMBINE ALL FACTORS THAT CONTRIBUTE TO AMPLITUDE AND INTERPOLATE AND CREATE AN A-RATE VERSION OF THIS RESULT (TO PREVENT QUANTISATION NOISE) 
    aSig    =    aSig * aAmp * 3                    ;SCALE AUDIO USING AMPLITUDE CONTROL FUNCTION
        outs    aSig, aSig                         ;AUDIO SENT TO OUTPUT, APPLY AMP. ENVELOPE, VOLUME CONTROL AND NOTE ON/OFF STATUS
    SKIP:
    
    /* RESET SEQUENCE TO BLANK */
    ktrig    trigger    gkReset,0.5,0
    if ktrig==1 then
     chnset    k(60),"Note1"
     chnset    k(60),"Note2"
     chnset    k(60),"Note3"
     chnset    k(60),"Note4"
     chnset    k(60),"Note5"
     chnset    k(60),"Note6"
     chnset    k(60),"Note7"
     chnset    k(60),"Note8"
     chnset    k(60),"Note9"
     chnset    k(60),"Note10"
     chnset    k(60),"Note11"
     chnset    k(60),"Note12"
     chnset    k(60),"Note13"
     chnset    k(60),"Note14"
     chnset    k(60),"Note15"
     chnset    k(60),"Note16"
     chnset    k(0),"On1"
     chnset    k(0),"On2"
     chnset    k(0),"On3"
     chnset    k(0),"On4"
     chnset    k(0),"On5"
     chnset    k(0),"On6"
     chnset    k(0),"On7"
     chnset    k(0),"On8"
     chnset    k(0),"On9"
     chnset    k(0),"On10"
     chnset    k(0),"On11"
     chnset    k(0),"On12"
     chnset    k(0),"On13"
     chnset    k(0),"On14"
     chnset    k(0),"On15"
     chnset    k(0),"On16"
     chnset    k(0),"Hold1"
     chnset    k(0),"Hold2"
     chnset    k(0),"Hold3"
     chnset    k(0),"Hold4"
     chnset    k(0),"Hold5"
     chnset    k(0),"Hold6"
     chnset    k(0),"Hold7"
     chnset    k(0),"Hold8"
     chnset    k(0),"Hold9"
     chnset    k(0),"Hold10"
     chnset    k(0),"Hold11"
     chnset    k(0),"Hold12"
     chnset    k(0),"Hold13"
     chnset    k(0),"Hold14"
     chnset    k(0),"Hold15"
     chnset    k(0),"Hold16"
     chnset    k(0),"Acc1"
     chnset    k(0),"Acc2"
     chnset    k(0),"Acc3"
     chnset    k(0),"Acc4"
     chnset    k(0),"Acc5"
     chnset    k(0),"Acc6"
     chnset    k(0),"Acc7"
     chnset    k(0),"Acc8"
     chnset    k(0),"Acc9"
     chnset    k(0),"Acc10"
     chnset    k(0),"Acc11"
     chnset    k(0),"Acc12"
     chnset    k(0),"Acc13"
     chnset    k(0),"Acc14"
     chnset    k(0),"Acc15"
     chnset    k(0),"Acc16"
    endif

    /* RANDOMISE SEQUENCE */
    ktrig    trigger    gkRnd,0.5,0    
    if ktrig==1 then
#define    RANDOM_STEP(N)
     #
     kval    random    24,72.999
     chnset    int(kval),"Note$N"
     kval    random    0,1.999
     chnset    int(kval),"On$N"
     kval    random    0,1.999
     chnset    int(kval),"Hold$N"
     kval    random    0,1.999
     chnset    int(kval),"Acc$N"
     #
     $RANDOM_STEP(1)
     $RANDOM_STEP(2)
     $RANDOM_STEP(3)
     $RANDOM_STEP(4)
     $RANDOM_STEP(5)
     $RANDOM_STEP(6)
     $RANDOM_STEP(7)
     $RANDOM_STEP(8)
     $RANDOM_STEP(9)
     $RANDOM_STEP(10)
     $RANDOM_STEP(11)
     $RANDOM_STEP(12)
     $RANDOM_STEP(13)
     $RANDOM_STEP(14)
     $RANDOM_STEP(15)
     $RANDOM_STEP(16)
     
    endif
    
    /* PROGRESS INDICATORS */
    kStepChange    changed    kStep
    if kStepChange==1 then
     if kStep==1 then
      chnset k(1),"Act1"
      chnset k(0),"Act16"
     elseif kStep==2 then
      chnset k(1),"Act2"
      chnset k(0),"Act1"
     elseif kStep==3 then
      chnset k(1),"Act3"
      chnset k(0),"Act2"
     elseif kStep==4 then
      chnset k(1),"Act4"
      chnset k(0),"Act3"
     elseif kStep==5 then
      chnset k(1),"Act5"
      chnset k(0),"Act4"
     elseif kStep==6 then
      chnset k(1),"Act6"
      chnset k(0),"Act5"
     elseif kStep==7 then
      chnset k(1),"Act7"
      chnset k(0),"Act6"
     elseif kStep==8 then
      chnset k(1),"Act8"
      chnset k(0),"Act7"
     elseif kStep==9 then
      chnset k(1),"Act9"
      chnset k(0),"Act8"
     elseif kStep==10 then
      chnset k(1),"Act10"
      chnset k(0),"Act9"
     elseif kStep==11 then
      chnset k(1),"Act11"
      chnset k(0),"Act10"
     elseif kStep==12 then
      chnset k(1),"Act12"
      chnset k(0),"Act11"
     elseif kStep==13 then
      chnset k(1),"Act13"
      chnset k(0),"Act12"
     elseif kStep==14 then
      chnset k(1),"Act14"
      chnset k(0),"Act13"
     elseif kStep==15 then
      chnset k(1),"Act15"
      chnset k(0),"Act14"
     elseif kStep==16 then
      chnset k(1),"Act16"
      chnset k(0),"Act15"
     endif
    endif    

    /* TRANSPOSE */
    ktrig1    trigger    kSemiDn,0.5,0
    ktrig2    trigger    kSemiUp,0.5,0
    ktrig3    trigger    kOctDn,0.5,0
    ktrig4    trigger    kOctUp,0.5,0
    if ktrig1+ktrig2+ktrig3+ktrig4==1 then
#define    TRANSPOSE(N)
     #
     kval    limit    gkNote$N+kInt,12,96
     chnset    kval,"Note$N"
     #
     if ktrig1==1 then
      kInt    =    -1
     elseif ktrig2==1 then
      kInt    =    1
     elseif ktrig3==1 then
      kInt    =    -12
     elseif ktrig4==1 then
      kInt    =    12
     endif      
     $TRANSPOSE(1)
     $TRANSPOSE(2)
     $TRANSPOSE(3)
     $TRANSPOSE(4)
     $TRANSPOSE(5)
     $TRANSPOSE(6)
     $TRANSPOSE(7)
     $TRANSPOSE(8)
     $TRANSPOSE(9)
     $TRANSPOSE(10)
     $TRANSPOSE(11)
     $TRANSPOSE(12)
     $TRANSPOSE(13)
     $TRANSPOSE(14)
     $TRANSPOSE(15)
     $TRANSPOSE(16)
    endif
endin

instr    99    ; trigger a random sequence at start up
    /* RANDOMISE SEQUENCE */
#define    RANDOM_STEP(N)
     #
     kval    random    24,72.999
     chnset    int(kval),"Note$N"
     kval    random    0,1.999
     chnset    int(kval),"On$N"
     kval    random    0,1.999
     chnset    int(kval),"Hold$N"
     kval    random    0,1.999
     chnset    int(kval),"Acc$N"
     #
     $RANDOM_STEP(1)
     $RANDOM_STEP(2)
     $RANDOM_STEP(3)
     $RANDOM_STEP(4)
     $RANDOM_STEP(5)
     $RANDOM_STEP(6)
     $RANDOM_STEP(7)
     $RANDOM_STEP(8)
     $RANDOM_STEP(9)
     $RANDOM_STEP(10)
     $RANDOM_STEP(11)
     $RANDOM_STEP(12)
     $RANDOM_STEP(13)
     $RANDOM_STEP(14)
     $RANDOM_STEP(15)
     $RANDOM_STEP(16)
 ;turnoff
endin

instr    100    ;CLEAR ALL INDICATORS EXCEPT THE FIRST
    chnset    0,"Act2"
    chnset    0,"Act3"
    chnset    0,"Act4"
    chnset    0,"Act5"
    chnset    0,"Act6"
    chnset    0,"Act7"
    chnset    0,"Act8"
    chnset    0,"Act9"
    chnset    0,"Act10"
    chnset    0,"Act11"
    chnset    0,"Act12"
    chnset    0,"Act13"
    chnset    0,"Act14"
    chnset    0,"Act15"
    chnset    0,"Act16"
endin

</CsInstruments>

<CsScore>
i 1 0 [3600*24*7]
i 99 0 0.1        ; trigger a random sequence
</CsScore>

</CsoundSynthesizer>
