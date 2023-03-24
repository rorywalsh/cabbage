
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; CrossedDelays.csd
; Written by Iain McCurdy, 2016

; This effect implements a dual delay effect in which the feedbacks between the two channels are crossed over.
; This becomes particularly useful when we add in separate ring modulators or frequency shifters on each channel.

; It is intended that the delays will be used in a coherent rhythmic function therefore delay times are set by means of a tempo control 
;  (either by the Cabbage widget or from a host) and subsequent subdivisions for each channel.
; An assumption is made that the base time signature 4/4, therefore a tempo of 60 bpm with divisions of 1/4 will result in delays of 1 second

; Feedback can be greater than a ratio of 1 without overloading occuring, this is on account of the gain reducing effect of ring modulating.
; Nonetheless a BDJ clip is activated if feedback is raised above 1 to protect against samples out of range is feedback is pushed even further.
; It should be born in mind that feeding back ring modulators can produce aliasing artefacts - in this context they are regarded as a characteristic of the effect.

; --LEFT_INPUT---+---DELAY_1----MOD_1---+---OUT_1--->
;                 \                    /
;                  \ /----[F.BACK]----/
;                   X
;                  / \----[F.BACK]----\
;                 /                    \
; --RIGHT_INPUT--+---DELAY_2----MOD_2---+---OUT_2--->

; Controls
; --------
;  [DELAYS]
; Clock Source    -    tempo source can be from the built-in 'Tempo' control or from the host's tempo, if used as a VST plugin
; Tempo            -    Cabbage defined tempo used in defining actual delay times (in conjunction with subdivision values)
; Mix            -    dry/wet mix for the effect
; Feedback        -    delay feedback. Caution: this can be set above 100%, feedback will be attenuated by ring modulation (not frequency shifting),
;                      but if feedback is greater than '1', soft clipping will be applied to protect against samples out of range
; Subdiv.1/Subdiv.2    -    metrical subdivisionsof the two delay times

;  [MODULATION]
; Type            -    choose between ring modulation and frequency shifting
; Freq.1/Freq.2    -    the frequencies of the ring modulators or frequency shifters

;  [OUTPUT]
; Pan1/Pan2     -    pan locations of the outputs of the two delays (OUT_1 and OUT_2 in the diagram above)
; Level            -    output level (dry and wet signals)

<Cabbage>
form caption("Crossed Delays") size(815, 130), pluginId("CrDl"), colour("Black")


image   bounds(  5,  5,370,120), outlineThickness(1), outlineColour("white"), colour("black"), plant("Delays_Plant")
{
label    bounds(  0,  4,370, 12), text("D E L A Y S")
label    bounds(  5, 20, 80, 12), text("Clock Source")
combobox bounds(  5, 35, 80, 20), channel("ClockSource"), text("Cabbage","Host"), value(1)
rslider  bounds( 75, 20, 90, 90), text("Tempo"), channel("Tempo"), valueTextBox(1), textBox(1), range(20,500,60,1,0.1)
rslider  bounds(140, 20, 90, 90), text("Mix"), channel("Mix"), valueTextBox(1), textBox(1), range(0,1,0.5,1,0.01)
rslider  bounds(205, 20, 90, 90), text("Feedback"), channel("Feedback"), valueTextBox(1), textBox(1), range(0,3,0.5,1,0.01)
label    bounds(285, 20, 70, 13), text("Subdiv.1")
combobox bounds(285, 33, 70, 20), channel("Div1"), text("1/1","1/2","1/3","1/4","1/6","1/8","1/12","1/16","1/24","1/32","1/48","1/64","1/96","1/128"), value(4)
label    bounds(285, 60, 70, 13), text("Subdiv.2")
combobox bounds(285, 73, 70, 20), channel("Div2"), text("1/1","1/2","1/3","1/4","1/6","1/8","1/12","1/16","1/24","1/32","1/48","1/64","1/96","1/128"), value(3)
}

image   bounds(380,  5,225,120), outlineThickness(1), outlineColour("white"), colour("black"), plant("RM_Plant")
{
label    bounds(  0,  4,225, 12), text("M O D U L A T I O N")
label    bounds(  5, 55, 80, 12), text("Type")
combobox bounds(  5, 70, 80, 20), channel("ModType"), text("Ring Mod.","Freq.Shift"), value(2)
checkbox bounds(  5, 30,120, 12), text("On/Off"), channel("ModOnOff"), value(1)
rslider  bounds( 75, 20, 90, 90), text("Freq.1"), channel("Freq1"), valueTextBox(1), textBox(1), range(-6000,6000,200,1,1)
rslider  bounds(140, 20, 90, 90), text("Freq.2"), channel("Freq2"), valueTextBox(1), textBox(1), range(-6000,6000,333,1,1)
}

image   bounds(610,  5,200,120), outlineThickness(1), outlineColour("white"), colour("black"), plant("Output_Plant")
{
label    bounds(  0,  4,200, 12), text("O U T P U T")
rslider  bounds(-10, 20, 90, 90), text("Pan 1"), channel("Pan1"), valueTextBox(1), textBox(1), range(0,1,0)
rslider  bounds( 55, 20, 90, 90), text("Pan 2"), channel("Pan2"), valueTextBox(1), textBox(1), range(0,1,1)
rslider  bounds(120, 20, 90, 90), text("Level"), channel("Level"), valueTextBox(1), textBox(1), range(0,1,1)
}

</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-n -d -+rtmidi=NULL -M0 -m0d 
</CsOptions>

<CsInstruments>

; sr set by host
ksmps = 16
nchnls = 2
0dbfs = 1

gkDivs[]    fillarray    1,2,3,4,6,8,12,16,24,32,48,64,96,128

opcode    FreqShifterSimple,a,aki                    ; SIMPLE FREQUENCY SHIFTER
    asig,kfshift,ifn    xin                ; READ IN INPUT ARGUMENTS
    areal, aimag hilbert asig                ; HILBERT OPCODE OUTPUTS TWO PHASE SHIFTED SIGNALS, EACH 90 OUT OF PHASE WITH EACH OTHER
    asin     oscili       1,    kfshift,     ifn,          0
    acos     oscili       1,    kfshift,     ifn,          0.25    
    ;RING MODULATE EACH SIGNAL USING THE QUADRATURE OSCILLATORS AS MODULATORS
    amod1    =        areal * acos
    amod2    =        aimag * asin    
    ;UPSHIFTING OUTPUT
    ares    = (amod1 - amod2)                ; MIX OUTPUTS (INVERTED WITH RESPECT TO EACH OTHER)
        xout    ares                    ; SEND AUDIO BACK TO CALLER INSTRUMENT
endop

gisine    ftgen    0,0,131072,10,1

instr    1
; aSig        diskin2    "808loopMono.wav",1,0,1
; ilen        filelen    "808loopMono.wav"
; klen        init    ilen

 kbpm        chnget    "HOST_BPM"
 kClockSource    chnget    "ClockSource"
 kClockSource    init    1
 if kClockSource==1 then                ;if internal clock source has been chosen...
  ktempo    chnget    "Tempo"                ;tempo taken from GUI knob control
 else
  ktempo    chnget    "bpm"                ;tempo taken from host BPM
  ktempo    limit    ktempo,20,500        ;limit range of possible tempo values. i.e. a tempo of zero would result in a delay time of infinity.
 endif

 aL,aR        ins
 klen        =        60*2/ktempo

 kMix        chnget    "Mix" 
 kFeedback    chnget    "Feedback"
 kModOnOff    chnget    "ModOnOff"
 kFreq1        chnget    "Freq1"
 kFreq2        chnget    "Freq2" 
 kDiv1        chnget    "Div1"
 kDiv2        chnget    "Div2"
 kModType    chnget    "ModType"
 
 aTap1,aTap2    init    0
 
 aBuf        delayr    60*2/20
 aTap1        deltapi    klen/gkDivs[kDiv1]
 aMod1        poscil    1,kFreq1
 if kModOnOff==1 then
  if kModType==1 then
   aTap1        *=        aMod1
  else
   aTap1        FreqShifterSimple    aTap1,kFreq1,gisine
  endif
 endif
             delayw    aL + (aTap2 * kFeedback)
 
 aBuf        delayr    60*2/20
 aTap2        deltapi    klen/gkDivs[kDiv2]
 aMod2        poscil    1,kFreq2
 if kModOnOff==1 then
  if kModType==1 then
   aTap2        *=        aMod2
  else
   aTap2        FreqShifterSimple    aTap2,kFreq2,gisine
  endif                                               
 endif
             delayw    aR + (aTap1 * kFeedback)
 
 if kFeedback>1 then
  aTap1 clip     aTap1, 0, 0dbfs, 0dbfs
  aTap2 clip     aTap2, 0, 0dbfs, 0dbfs
 endif

 kPan1        chnget    "Pan1"
 kPan2        chnget    "Pan2"
 kLevel        chnget    "Level"
             
 aMixL        ntrpol    aL, aTap1*(1-kPan1) + aTap2*(1-kPan2), kMix
 aMixR        ntrpol    aR, aTap1*kPan1 + aTap2*kPan2, kMix
            outs    aMixL*kLevel, aMixR*kLevel
endin

</CsInstruments>

<CsScore>
i 1 0 [60*60*24*7] 
</CsScore>

</CsoundSynthesizer>
