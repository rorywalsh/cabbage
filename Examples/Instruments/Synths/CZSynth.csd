
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; CZSynth.csd
; Iain McCurdy, 2015

; CZSynth employs phase distortion synthesis, a technique featured in Casio's CZ range of synthesisers from the 1980s

; This synth can create strident lead sounds and if higher partials are employed with high levels of phase distortion aliasing can occur.
; This is left in as a chacteristic of the instrument but if desired, raising the sample rate can reduce or prevent this aliasing 

; The basic waveform (before phase distortion) can be designed in one of three ways:
; Additive    -    is constructed from 16 harmonic partials selectable using the checkboxes.
; Buzz        -    is constructed from a stack of harmonically related cosines using GEN11 and in a manner similar to the buzz and gbuzz opcodes.
; Noise        -    is a table of triangle noise derived samples.

; PHASE DISTORTION
; Distort    -    manual offset for the amount of phase distortion
; Env.        -    amount of influence of the envelope upon distortion amount
; Att.        -    attack time for the phase distortion amount envelope
; Dec.        -    decay time for the phase distortion amount envelope
; Retrig.(checkbox)    -    if ticked, in mono mode the envelope will be retriggered each time a new note is played
; Vel.        -    amount of influence of key velocity upon phase distortion amount (affects both manual and envelope)
; LFO SHAPE (COMBOBOX)    - LFO shape can be either triangle or random (random splines). Random splines LFO can be useful in adding a natural fluctuation to the sound.
; LFO        -    amount of LFO influence upon phase distortion amount
; Rate        -    rate of the LFO
; Note that the manual control 'Distort', the envelope and the LFO are added together, therefore the influence of the envelope (or the LFO) may not be heard if 'Distort' is at its maximum setting  
; Kybd.Scal.    -    amount of keyboard scaling of phase distortion amount. 
;             Increasing this will result in attenuation of phase distortion of higher notes.
;             This can be used to reduce of prevent aliasing in higher notes. 

; FILTER (a filter built using the clfilt opcode - lowpass Cheyshev I)
; On/Off    -    turns the filter on and off
; Cutoff    -    cutoff frequency manual control expressed as a ratio of the frequency of the note played
; Poles        -    number of poles employed by the filter
; Ripple    -    amount of ripple at the cutoff point
; Env.        -    amount of influence of the envelope upon filter cutoff
; Att.        -    attack time for the envelope
; Dec.        -    decay time for the envelope
; Retrig.(checkbox)    -    if ticked, in mono mode the envelope will be retriggered each time a new note is played
; Vel.        -    amount of influence of key velocity upon filter cutoff (affects both manual and envelope)
; LFO SHAPE (COMBOBOX)    - LFO shape can be either triangle or random (random splines). Random splines LFO can be useful in adding a natural fluctuation to the sound. 
; LFO        -    amount of LFO influence upon filter cutoff
; Rate        -    rate of the LFO
; Note that 'Cutoff', the envelope and the LFO are simply added together, but that their combined output is internally limited to prevent the filter from 'blowing up'

; OSCILLATOR 2    -    besides transposition the second oscillator is identical to the main oscillator
; On/Off    -    turns the second oscillator on or off
; Semitone    -    transposition of the second oscillator in semitones
; Cents        -    transposition of the second oscillator in cents

; POLYPHONY
; Mono/Poly (button) - select mode
; Port.Time    -    portamento time in mono (legato) mode

; AMPLITUDE
; Att.        -    attack time for the amplitude envelope
; Dec.        -    decay time for the amplitude envelope
; Sus.        -    sustain level for the amplitude envelope
; Rel.        -    release time for the amplitude envelope
; Vel.        -    amount of influence of key velocity upon amplitude
; Clip        -    amount of clipping of the sound. This can be used to apply further waveshaping.
; Level        -    output level

<Cabbage>

#define SLIDER_STYLE trackerColour(150,150,255)
#define CHECKBOX_STYLE colour(255,255,100)

form caption("CZ Synthesiser") size(1075,365), pluginId("RMSy"), colour(40,40,40)

; PHASE DISTORTION
image      bounds(  5,  5,500,130), colour(0,0,0,0), plant("ShapeAmount"), outlineColour("white"), outlineThickness(1), shape("sharp") {
label      bounds(  5,  2,500, 12), text(". PHASE DISTORTION ."), fontColour(255,255,100)
rslider    bounds(  0, 40, 80, 80), valueTextBox(1), textBox(1), text("Distort"), channel("ShapeAmount"),  range(-1,  1, 0), $SLIDER_STYLE
checkbox   bounds( 80, 25, 80, 10), text("Retrig."), channel("SARetrig"), value(1), identChannel("SARetrigID"), visible(0), $CHECKBOX_STYLE
rslider    bounds( 60, 40, 80, 80), valueTextBox(1), textBox(1), text("Env."), channel("SAEnv"),  range(0,  1, 1), $SLIDER_STYLE
rslider    bounds(120, 40, 80, 80), valueTextBox(1), textBox(1), text("Att."), channel("SAAtt"),  range(0, 16, 0,0.5), $SLIDER_STYLE
rslider    bounds(180, 40, 80, 80), valueTextBox(1), textBox(1), text("Dec."), channel("SADec"),  range(0, 16, 2,0.5), $SLIDER_STYLE
rslider    bounds(240, 40, 80, 80), valueTextBox(1), textBox(1), text("Vel."), channel("SAVel"),  range(0,  1, 1), $SLIDER_STYLE
combobox   bounds(325, 20, 85, 20), channel("SALFOShape"), value(2), text("Tri.","Random")
rslider    bounds(300, 40, 80, 80), valueTextBox(1), textBox(1), text("LFO"),  channel("SALFO"),  range(0,  1, 0.09), $SLIDER_STYLE
rslider    bounds(360, 40, 80, 80), valueTextBox(1), textBox(1), text("Rate"), channel("SARate"), range(0, 14, 1), $SLIDER_STYLE
rslider    bounds(420, 40, 80, 80), valueTextBox(1), textBox(1), text("Kybd.Scal."), channel("KybdScal"), range(0, 1, 0), $SLIDER_STYLE
}

; FILTER
image      bounds(510,  5,560,130), plant("Filter"), colour(0,0,0,0), outlineColour("white"), outlineThickness(1), shape("sharp") {
label      bounds(  5,  2,550, 12), text(". FILTER ."), fontColour(255,255,100)
checkbox   bounds( 15, 25,100, 10), text("On/Off"), channel("FilterOnOff"), $CHECKBOX_STYLE
rslider    bounds(  0, 40, 80, 80), valueTextBox(1), textBox(1), text("Cutoff"), channel("Cutoff"),  range(1,100,10,1,0.1), identChannel("ID1"), visible(0), $SLIDER_STYLE
rslider    bounds( 60, 40, 80, 80), valueTextBox(1), textBox(1), text("Poles"), channel("Poles"),  range(2,  50, 8,1,2), identChannel("ID2"), visible(0), $SLIDER_STYLE
rslider    bounds(120, 40, 80, 80), valueTextBox(1), textBox(1), text("Ripple"), channel("Ripple"),  range(0.1, 50, 20, 0.5, 0.01), identChannel("ID3"), visible(0), $SLIDER_STYLE
checkbox   bounds(180, 25, 80, 10), text("Retrig."), textBox(1), channel("FRetrig"), value(1), identChannel("FRetrigID"), visible(0), visible(0), $CHECKBOX_STYLE
rslider    bounds(180, 40, 80, 80), valueTextBox(1), textBox(1), text("Env."), channel("FEnv"),  range(0, 50, 20,1,0.01), identChannel("ID4"), visible(0), $SLIDER_STYLE
rslider    bounds(240, 40, 80, 80), valueTextBox(1), textBox(1), text("Att."), channel("FAtt"),  range(0, 16, 0.1,0.5), identChannel("ID5"), visible(0), $SLIDER_STYLE
rslider    bounds(300, 40, 80, 80), valueTextBox(1), textBox(1), text("Dec."), channel("FDec"),  range(0, 16, 1,0.5), identChannel("ID6"), visible(0), $SLIDER_STYLE
rslider    bounds(360, 40, 80, 80), valueTextBox(1), textBox(1), text("Vel."), channel("FVel"),  range(0,  1, 1), identChannel("ID7"), visible(0), $SLIDER_STYLE
combobox   bounds(445, 20, 85, 20), channel("FLFOShape"), value(1), text("Tri.","Random"), identChannel("ID10"), visible(0)
rslider    bounds(420, 40, 80, 80), valueTextBox(1), textBox(1), text("LFO"),  channel("FLFO"),  range(0, 50, 0), identChannel("ID8"), visible(0), $SLIDER_STYLE
rslider    bounds(480, 40, 80, 80), valueTextBox(1), textBox(1), text("Rate"), channel("FRate"), range(0, 14, 1), identChannel("ID9"), visible(0), $SLIDER_STYLE
}

; WAVEFORM
image      bounds(  5,140,320,130), plant("Waveform"), colour(0,0,0,0), outlineColour("white"), outlineThickness(1), shape("sharp") {
label      bounds(  5,  2,320, 12), text(". WAVEFORM ."), fontColour(255,255,100)
combobox   bounds(  5, 20, 85, 20), channel("WaveformMode"), value(1), text("Additive","Buzz","Noise")
label      bounds(145, 23, 85, 13), text("Octave Shift:")
combobox   bounds(230, 20, 85, 20), channel("OctShift"), value(1), text("0","-1","-2","-3","-4","-5","-6")
}

image      bounds(  5,190,320,130), plant("WaveformAdd"), colour(0,0,0,0), shape("sharp"), identChannel("WaveformAddID") {
image      bounds(  8,  3,304, 19), colour("silver") shape("sharp")
checkbox   bounds( 10,  5, 15, 15), channel("F0") ,  colour("lime"), value(1)  ;, shape("ellipse")
checkbox   bounds( 25,  5, 15, 15), channel("F1") ,  colour("lime")            ;, shape("ellipse")
checkbox   bounds( 40,  5, 15, 15), channel("F2") ,  colour("lime")            ;, shape("ellipse")
checkbox   bounds( 55,  5, 15, 15), channel("F3") ,  colour("lime")            ;, shape("ellipse")
checkbox   bounds( 70,  5, 15, 15), channel("F4") ,  colour("lime")            ;, shape("ellipse")
checkbox   bounds( 85,  5, 15, 15), channel("F5") ,  colour("lime")            ;, shape("ellipse")
checkbox   bounds(100,  5, 15, 15), channel("F6") ,  colour("lime")            ;, shape("ellipse")
checkbox   bounds(115,  5, 15, 15), channel("F7") ,  colour("lime"), value(1)  ;, shape("ellipse")
checkbox   bounds(130,  5, 15, 15), channel("F8") ,  colour("lime")            ;, shape("ellipse")
checkbox   bounds(145,  5, 15, 15), channel("F9") ,  colour("lime")            ;, shape("ellipse")
checkbox   bounds(160,  5, 15, 15), channel("F10"),  colour("lime")            ;, shape("ellipse")
checkbox   bounds(175,  5, 15, 15), channel("F11"),  colour("lime"), value(1)  ;, shape("ellipse")
checkbox   bounds(190,  5, 15, 15), channel("F12"),  colour("lime")            ;, shape("ellipse")
checkbox   bounds(205,  5, 15, 15), channel("F13"),  colour("lime")            ;, shape("ellipse")
checkbox   bounds(220,  5, 15, 15), channel("F14"),  colour("lime")            ;, shape("ellipse")
checkbox   bounds(235,  5, 15, 15), channel("F15"),  colour("lime")            ;, shape("ellipse")
checkbox   bounds(250,  5, 15, 15), channel("F16"),  colour("lime")            ;, shape("ellipse")
checkbox   bounds(265,  5, 15, 15), channel("F17"),  colour("lime")            ;, shape("ellipse")
checkbox   bounds(280,  5, 15, 15), channel("F18"),  colour("lime")            ;, shape("ellipse")
checkbox   bounds(295,  5, 15, 15), channel("F19"),  colour("lime")            ;, shape("ellipse")

label      bounds( 10, 24, 15, 9), text("1")
label      bounds( 25, 24, 15, 9), text("2")
label      bounds( 40, 24, 15, 9), text("3")
label      bounds( 55, 24, 15, 9), text("4")
label      bounds( 70, 24, 15, 9), text("5")
label      bounds( 85, 24, 15, 9), text("6")
label      bounds(100, 24, 15, 9), text("7")
label      bounds(115, 24, 15, 9), text("8")
label      bounds(130, 24, 15, 9), text("9")
label      bounds(145, 24, 15, 9), text("10")
label      bounds(160, 24, 15, 9), text("11")
label      bounds(175, 24, 15, 9), text("12")
label      bounds(190, 24, 15, 9), text("13")
label      bounds(205, 24, 15, 9), text("14")
label      bounds(220, 24, 15, 9), text("15")
label      bounds(235, 24, 15, 9), text("16")
label      bounds(250, 24, 15, 9), text("17")
label      bounds(265, 24, 15, 9), text("18")
label      bounds(280, 24, 15, 9), text("19")
label      bounds(295, 24, 15, 9), text("20")
}

image      bounds(  5,190,320,130), plant("WaveformBuzz"), colour(0,0,0,0), shape("sharp"), identChannel("WaveformBuzzID"), visible(0) {
rslider    bounds( 25,  0, 70, 70), text("Harms."), channel("Harms"),  range(1,  80, 5,1,1), $SLIDER_STYLE
rslider    bounds( 95,  0, 70, 70), text("Lowest"), channel("Lowest"),  range(1,  80, 7,1,1), $SLIDER_STYLE
rslider    bounds(165,  0, 70, 70), text("Power"), channel("Power"),  range(0,  2, 0.9), $SLIDER_STYLE
}

image      bounds(  5,190,320,130), plant("WaveformNoise"), colour(0,0,0,0), shape("sharp"), identChannel("WaveformNoiseID"), visible(0) {
rslider    bounds( 95,  0, 70, 70), text("Size"), channel("NoiseSize"),  range(2, 12, 7,1,1), $SLIDER_STYLE
}

; OSCILLATOR 2
image      bounds(330,140,140,130), plant("Oscillator2"), colour(0,0,0,0), outlineColour("white"), outlineThickness(1), shape("sharp") {
label      bounds(  5,  2,130, 12), text(". OSCILLATOR 2 ."), fontColour(255,255,100)
checkbox   bounds( 15, 25,100, 10), text("On/Off"), channel("Osc2OnOff"), value(0), $CHECKBOX_STYLE
rslider    bounds(  0, 40, 80, 80), valueTextBox(1), textBox(1), text("Semitone"), channel("Semitones"),  range(-24, 24, -12,1,1), $SLIDER_STYLE
rslider    bounds( 60, 40, 80, 80), valueTextBox(1), textBox(1), text("Cents"), channel("Cents"),  range(-100, 100, 0,1,1), $SLIDER_STYLE
}

; POLYPHONY
image      bounds(475,140,150,130), plant("Polyphony"), colour(0,0,0,0), outlineColour("white"), outlineThickness(1), shape("sharp") {
label      bounds(  5,  2,140, 12), text(". POLYPHONY ."), fontColour(255,255,100)
button     bounds( 15, 40, 50, 20), text("Mono","Poly"), channel("MonoPoly"), value(1)
rslider    bounds( 60, 40, 80, 80), valueTextBox(1), textBox(1), text("Port.Time"), channel("PortTime"),  range(0, 1, 0.05), identChannel("PortTimeID"), visible(0), $SLIDER_STYLE
}

; AMPLITUDE
image      bounds(630,140,440,130), plant("Amplitude"), colour(0,0,0,0), outlineColour("white"), outlineThickness(1), shape("sharp") {
label      bounds(  5,  2,430, 12), text(". AMPLITUDE ."), fontColour(255,255,100)
rslider    bounds(  0, 40, 80, 80), valueTextBox(1), textBox(1), text("Att."), channel("AAtt"),  range(0,  5, 0.05), $SLIDER_STYLE
rslider    bounds( 60, 40, 80, 80), valueTextBox(1), textBox(1), text("Dec."), channel("ADec"),  range(0,  5, 0.5), $SLIDER_STYLE
rslider    bounds(120, 40, 80, 80), valueTextBox(1), textBox(1), text("Sus."), channel("ASus"),  range(0,  1, 1), $SLIDER_STYLE
rslider    bounds(180, 40, 80, 80), valueTextBox(1), textBox(1), text("Rel."), channel("ARel"),  range(0,  5, 0.2), $SLIDER_STYLE
rslider    bounds(240, 40, 80, 80), valueTextBox(1), textBox(1), text("Vel."), channel("AVel"),  range(0,  1, 1), $SLIDER_STYLE
rslider    bounds(300, 40, 80, 80), valueTextBox(1), textBox(1), text("Clip"), channel("Clip"),  range(0.1,  10, 1,0.5), $SLIDER_STYLE
rslider    bounds(360, 40, 80, 80), valueTextBox(1), textBox(1), text("Level"), channel("Level"),  range(0,  1, 0.15), $SLIDER_STYLE
}

keyboard   bounds(  5,275,1065, 85)

</Cabbage>


<CsoundSynthesizer>


<CsOptions>
-dm0 -n -+rtmidi=NULL -M0
</CsOptions>

<CsInstruments>

; sr set by host
ksmps = 16
nchnls = 2
0dbfs = 1

massign    0,2
maxalloc 3,1

giUniSine    ftgen    0, 0, 4097, 19, 1, 0.5, 0, 0.5
giUniTri     ftgen    0, 0, 4097, 7, 0, 2048, 1, 2048, 0
giSine       ftgen    0, 0, 4096, 10, 1

giKybdScal   ftgen    0, 0, 128, 7, 1, 127, 0
gaL,gaR      init     0

opcode    lineto2,k,kk
 kinput,ktime    xin
 ktrig    changed    kinput,ktime    ; reset trigger
 if ktrig==1 then                    ; if new note has been received or if portamento time has been changed...
  reinit RESTART
 endif
 RESTART:                            ; restart 'linseg' envelope
 if i(ktime)==0 then                 ; 'linseg' fails if duration is zero...
  koutput    =    i(kinput)          ; ...in which case output simply equals input
 else
  koutput    linseg    i(koutput),i(ktime),i(kinput)    ; linseg envelope from old value to new value
 endif
 rireturn
         xout    koutput
endop

instr    1
 gkMonoPoly    chnget    "MonoPoly"
 gkFilterOnOff    chnget    "FilterOnOff"

 if changed(gkMonoPoly)==1 then
  if gkMonoPoly==0 then
      chnset    "visible(1)", "SARetrigID"
   if gkFilterOnOff==1 then
      chnset    "visible(1)", "FRetrigID"
   endif
      chnset    "visible(1)", "PortTimeID"
  elseif gkMonoPoly==1 then
      chnset    "visible(0)", "SARetrigID"
      chnset    "visible(0)", "FRetrigID"
      chnset    "visible(0)", "PortTimeID"
  endif

 endif
 
 kWaveformMode    chnget    "WaveformMode"
 if changed(kWaveformMode)==1 then
  if kWaveformMode==1 then
      chnset    "visible(1)", "WaveformAddID"
      chnset    "visible(0)", "WaveformBuzzID"
      chnset    "visible(0)", "WaveformNoiseID"
  elseif kWaveformMode==2 then
      chnset    "visible(0)", "WaveformAddID"
      chnset    "visible(1)", "WaveformBuzzID"
      chnset    "visible(0)", "WaveformNoiseID"
  else
      chnset    "visible(0)", "WaveformAddID"
      chnset    "visible(0)", "WaveformBuzzID"
      chnset    "visible(1)", "WaveformNoiseID"
  endif
 endif
 kHarms         chnget    "Harms"
 kLowest        chnget    "Lowest"
 kPower         chnget    "Power"
 kHarms         init      5
 kLowest        init      7
 kPower         init      0.9
 kNoiseSize     chnget    "NoiseSize"
 kNoiseSize     init      3
 
 kShapeAmount   chnget    "ShapeAmount"
 gkSARetrig     chnget    "SARetrig"
 gkSAEnv        chnget    "SAEnv"
 gkSAAtt        chnget    "SAAtt"
 gkSADec        chnget    "SADec"
 gkSAVel        chnget    "SAVel"
 gkSALFOShape   chnget    "SALFOShape"
 gkSALFO        chnget    "SALFO"
 gkSARate       chnget    "SARate"
 gkKybdScal     chnget    "KybdScal"
 
 kClip          chnget    "Clip"
 
 if changed(gkFilterOnOff)==1 then
  if gkFilterOnOff==1 then
      if gkMonoPoly==0 then
       chnset    "visible(1)", "FRetrigID"
      endif
      chnset    "visible(1)", "ID1"
      chnset    "visible(1)", "ID2"
      chnset    "visible(1)", "ID3"
      chnset    "visible(1)", "ID4"
      chnset    "visible(1)", "ID5"
      chnset    "visible(1)", "ID6"
      chnset    "visible(1)", "ID7"
      chnset    "visible(1)", "ID8"
      chnset    "visible(1)", "ID9"
      chnset    "visible(1)", "ID10"
  elseif gkFilterOnOff==0 then
      chnset    "visible(0)", "FRetrigID"
      chnset    "visible(0)", "ID1"
      chnset    "visible(0)", "ID2"
      chnset    "visible(0)", "ID3"
      chnset    "visible(0)", "ID4"
      chnset    "visible(0)", "ID5"
      chnset    "visible(0)", "ID6"
      chnset    "visible(0)", "ID7"
      chnset    "visible(0)", "ID8"
      chnset    "visible(0)", "ID9"
      chnset    "visible(0)", "ID10"
  endif
 endif

 kCutoff    chnget    "Cutoff"
 gkPoles    chnget    "Poles"
 gkRipple    chnget    "Ripple"
 gkFRetrig    chnget    "FRetrig"
 gkFEnv        chnget    "FEnv"
 gkFAtt        chnget    "FAtt"
 gkFDec        chnget    "FDec"
 gkFVel        chnget    "FVel"
 gkFLFOShape    chnget    "FLFOShape"
 gkFLFO        chnget    "FLFO"
 gkFRate    chnget    "FRate"

 kPortTime    linseg    0,0.001,0.1
 gkShapeAmount    portk    kShapeAmount,kPortTime
 gkClip        portk    kClip,kPortTime
 gkCutoff    portk    kCutoff,kPortTime    

 gkOctShift    chnget    "OctShift"
 gkOctShift    =    -(gkOctShift-1)

 gkOsc2OnOff    chnget    "Osc2OnOff"
 kSemitones    chnget    "Semitones"
 kCents        chnget    "Cents"
 gkTransRto    =    semitone(kSemitones)*cent(kCents)
 gkPortTime    chnget    "PortTime"
 kF0        chnget    "F0"        
 kF1        chnget    "F1"        
 kF2        chnget    "F2"
 kF3        chnget    "F3"
 kF4        chnget    "F4"
 kF5        chnget    "F5"
 kF6        chnget    "F6"
 kF7        chnget    "F7"
 kF8        chnget    "F8"
 kF9        chnget    "F9"
 kF10        chnget    "F10"
 kF11        chnget    "F11"
 kF12        chnget    "F12"
 kF13        chnget    "F13"
 kF14        chnget    "F14"
 kF15        chnget    "F15"
 kF16        chnget    "F16"
 kF17        chnget    "F17"
 kF18        chnget    "F18"
 kF19        chnget    "F19"
 cngoto changed(kF0,kF1,kF2,kF3,kF4,kF5,kF6,kF7,kF8,kF9,kF10,kF11,kF12,kF13,kF14,kF15,kF16,kF17,kF18,kF19,kWaveformMode,kHarms,kLowest,kPower,kNoiseSize)==1, CREATE_TABLE
 reinit CREATE_TABLE
 CREATE_TABLE:
 if i(kWaveformMode)==1 then
  gisource    ftgen    1,0,131072,9,   1, i(kF0),   90,   \
                 2, i(kF1),   90,   \
                 3, i(kF2),   90,   \
                 4, i(kF3),   90,   \
                 5, i(kF4),   90,   \
                 6, i(kF5),   90,   \
                 7, i(kF6),   90,   \
                 8, i(kF7),   90,   \
                 9, i(kF8),   90,   \
                 10,i(kF9),   90,   \
                 11,i(kF10),  90,   \
                 12,i(kF11),  90,   \
                 13,i(kF12),  90,   \
                 14,i(kF13),  90,   \
                 15,i(kF14),  90,   \
                 16,i(kF15),  90,   \
                 17,i(kF16),  90,   \
                 18,i(kF17),  90,   \
                 19,i(kF18),  90,   \
                 20,i(kF19),  90
 elseif i(kWaveformMode)==2 then
  gisource    ftgen    1,0,131072,11,   i(kHarms), i(kLowest), i(kPower)
 else
  gisource    ftgen    1,0,2^i(kNoiseSize),21,3,1
 endif
 
 gkAAtt        chnget        "AAtt"
 gkADec        chnget        "ADec"
 gkASus        chnget        "ASus"
 gkARel        chnget        "ARel"
 gkAVel        chnget        "AVel"
 gkLevel       chnget        "Level"
endin


instr    2                                ; RESPOND TO MIDI NOTES
 icps     cpsmidi
 inum     notnum
 givel    veloc    0,1
 gkVel    init     givel
 gkcps    =        icps
 gicps    init     icps
 if i(gkMonoPoly)==0 then                 ; IF MONO MODE SELECTED...
          event_i  "i",3,0,-1, icps, inum
 else                                     ; OTHERWISE POLYPHONIC MODE HAS BEEN SELECTED
  aL,aR   subinstr    3, icps, inum
          outs        aL,aR
 endif
 
 gkNewNote init    1
endin


instr    3
 if active:k(2)==0 then                   ; IF ALL INSTANCES OF INSTR 2 HAVE BEEN TURNED OFF...
  turnoff                                 ; TURN OFF THIS INSTRUMENT
 endif
 kPortTime linseg  0, 0.001, 1

 if i(gkMonoPoly)==0 then
  kcps     lineto2    gkcps, kPortTime * gkPortTime        
 else
  kcps     init       p4
 endif
 
 aosc      phasor     kcps * octave(gkOctShift)
 
 cngoto gkNewNote==1&&gkSARetrig==1&&i(gkMonoPoly)==0, SHAPE_ENV
 reinit SHAPE_ENV
 SHAPE_ENV:
 kShapeEnv transeg    0, i(gkSAAtt)+0.000001, 1, i(gkSAEnv),i(gkSADec)+0.000001, -2, 0     ; SHAPE AMOUNT ENVELOPE
 rireturn
 iSAVel    init    (i(gkVel)*i(gkSAVel)) + (1-i(gkSAVel))                    ; SHAPE AMOUNT VELOCITY
 if gkSALFOShape==1 then                                ; SHAPE AMOUNT LFO
  kSALFO   oscili    gkSALFO,gkSARate,giUniTri                    ; TRIANGLE LFO
 else
  kSALFO   rspline    0,gkSALFO,gkSARate,gkSARate*4                    ; RANDOM LFO
 endif
 
 iSAKybdScal    table   p5+((i(gkKybdScal)*256)-128), giKybdScal            ; SHAPE AMOUNT KEYBOARD SCALING (REDUCING SHAPE AMOUNT FOR HIGHER NOTE CAN REDUCE ALIASING
 kShapeAmount   =       (gkShapeAmount+kShapeEnv+kSALFO)*iSAVel*iSAKybdScal
 ibipolar       =       0                                                   ; UNIPOLAR/BIPOLAR SWITCH (0=UNIPOLAR 1=BIPOLAR)
 ifullscale     =       1                                                   ; FULLSCALE VALUE
 apd            pdhalf  aosc, kShapeAmount, ibipolar, ifullscale            ; PHASE DISTORT THE PHASOR (aosc) CREATED 4 LINES ABOVE
 asig           tablei  apd,gisource,1
 
 if gkOsc2OnOff==1 then
  aosc2        phasor   kcps * octave(gkOctShift) * gkTransRto
  apd2         pdhalf   aosc2, kShapeAmount, ibipolar, ifullscale           ; PHASE DISTORT THE PHASOR (aosc) CREATED 4 LINES ABOVE
  asig         +=       tablei:a(apd2,gisource,1)
 endif
 
 
 ; POWERSHAPE DISTORTION
 ifullscale    =           0dbfs                     ; DEFINE FULLSCALE AMPLITUDE VALUE
 asig          powershape  asig, gkClip, ifullscale  ; CREATE POWERSHAPED SIGNAL 
 
 ; FILTER
 if gkFilterOnOff==1 then

  if gkFLFOShape==1 then
   kFLFO       oscili    gkFLFO,gkFRate,giUniTri
  else
   kFLFO       rspline    0,gkFLFO,gkFRate,gkFRate*4
  endif
  cngoto gkNewNote==1&&gkFRetrig==1&&i(gkMonoPoly)==0, FILTER_ENV
  reinit FILTER_ENV
  FILTER_ENV:
  kFiltEnv     transeg    0,i(gkFAtt)+0.000001,2, i(gkFEnv),i(gkFDec)+0.000001,-2,0 
  rireturn
  iFVel        init     (i(gkVel)*i(gkFVel)) + (1-i(gkFVel))
  kCF          limit    kcps*((gkCutoff+kFiltEnv+kFLFO)*iFVel),1,sr/3
               cngoto   changed(gkPoles,gkRipple)==1, FILTER
  reinit FILTER
  FILTER:
  asig         clfilt   asig*8,kCF,0,i(gkPoles),1,i(gkRipple)
 endif
 
 ; AMPLITUDE ENVELOPE
 aEnv         expsegr  0.01,i(gkAAtt)+0.00001,1.01,i(gkADec)+0.00001,i(gkASus)+0.01,i(gkARel)+0.00001,0.01
 aEnv         -=       0.01
 iAVel        init     (i(gkVel)*i(gkAVel)) + (1-i(gkAVel))
 asig         *=       aEnv*iAVel
 
 ; STEREO RIGHT CHANNEL
 aDly        interp    0.5 / kcps                               ; RIGHT CHANNEL WILL BE DELAYED BY 1/2 PERIOD OF THE FUNDEMENTAL                    
 aR          vdelay    asig, aDly*1000, (0.5/cpsmidinn(0))*1000 ; VARIABLE DELAY (NEEDS TO BE VARIABLE FOR MONOPHONIC/LEGATO MODE)
 
 ; OUTPUT
;        outs    asig*gkLevel, aR*gkLevel
 gaL        +=         asig * gkLevel
 gaR        +=         aR * gkLevel
 
 gkNewNote  =          0                                        ; RESET NEW NOTE FLAG
endin

instr    4
 aL         resonr     gaL, 100, 10, 1
 aR         resonr     gaR, 100, 10, 1
            outs       aL + gaL / 4, aR + gaR / 4
            clear      gaL, gaR
endin

</CsInstruments>  

<CsScore>
i 1  0 [3600*24*7]
i 4  0 [3600*24*7]
</CsScore>

</CsoundSynthesizer>