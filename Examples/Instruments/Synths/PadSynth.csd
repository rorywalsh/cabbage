
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; PadSynth.csd
; Written by Iain McCurdy, 2015

; 'padsynth' is actually a GEN routine that generates a loopable function table exhibiting a dense texture that, 
; when replayed using an oscillator, resembles what is referred to as a 'pad'.
; Changing parameters that influence the padsynth GEN routine ('Table', 'Base Freq', 'Bandwidth', 'Part.Scal.', 
; 'Harm.Str.', 'Table Size' and the 'User Table' sliders) will force the table to be rebuilt, and result in 
; interruptions in the output. In performance it will be better to think of 'padsynth' as creating a fixed complex 
; spectrum which is then processed using techniques of subtractive synthesis.

; Two tables are actually created, one with a reduced number of partials (5 partials). As higher notes are played
; the synthesizer increasing uses this second table in order to avoid the aliasing issues that would occur with using 
; only the first table. 

; Table --    Function table type to be used by padsynth. If 'User Table' is selected the partials are 
;             set manually using the mini vertical sliders. Sliders can also be set by 'drawing' within 
;             the table area using click and drag.
;             A variety of additional 'preset' tables are also offered. If any of these are chosen the 
;             partial sliders and 'Base Freq.' are not available. 'Base Freq.' will be set automatically 
;             to the value corrsponding to the analysed pitch that produced the partial data. Note that 
;             these are single tables that are scaled scaled up or down the entire keyboard from their 
;             point of unison therefore they will sound less like their sources, the further away from 
;             their points of unison they are played.      
; 
; 
; Bandwidth -- effectively controls modulation of the partial frequencies 
;              (amplitude and phase modulation)
;              Increasing this increases the amount a noise component that will be preset in all partials.       
; Base Freq. -- The fundemental of the tone. Too high a value here will result in quantisation artefacts.            
; Part.Scal. -- scales the modulations from partial to partial, raising this beyond 1 will result in increasing     
;               excursive modulations in higher partials - this effect can be likened to adding 'air' into 
;               the pad texture whilst retaining clarity in the lower partials. Experimentation with this 
;               parameter should help in gaining an understanding of its effect.                                     
; Harm.Str.  -- Scales all partial frequencies.
; Table Size -- Size of the table created by padsynth. (Actual size will be 2 ^ 'Table Size'.)
;               Reducing the table size will reduce the load time and the time it takes to recalculate the 
;               table whenever a change is made to 'padsynth's parameters, but if the table becomes too small, 
;               looping will become obvious.                          
; 
; When 'User Table' is selected the amplitudes of the harmonic partials are set using the mini vertical slider bars.


; Amplitude Envelope - ADSR control of amplitude

; Filter Bandwidth - an envelope that controls the bandwidth of a bandpass filter using an envelope - Level 1, Attack, Sustain, Rel.Time, Rel.Time, Rel.Level
;  Position - the centre frequency of the bandpass filter as a partial number, related to fundamental frequency of the note played.
; The cutoff frequency is defined using the 'Pos' slider and defines a ratio above the base frequency. Therefore a value 
; of '3' here will emphasise the 3rd partial of any note when bandwidth is narrowed.

; Reverb - Sean Costello reverb (screverb)
; Send    - wet level
; Size    - room size
; Damping - low pass filter within the reverb

; Highpass/Lowpass - series highpass-lowpass filter. Manually adjustable as well as from the keyboard's modulation wheel

; Legato (portamento)
; Activates monophonic mode with controllable glide time from note to note.
; Shape of the legato glissando can be a straight line or a curve, slowing as it reaches the destination note.
; Bend range for the keyboard's pitch bend wheel is set here.

; Pitch Stack - single notes played on the MIDI keyboard can trigger multiple pitches, equidistantly offset by a user-specified interval. 
; An additional stack separated by intervals can also be accessed.
; Interval    - the interval (in semitones) that separates each note in the stack. Can be postive (a rising stack) or negative (a descending stack).
; Nr. Layers  - number of pitches in the stack. A value of '1' essentially means this feature is disabled.
; Gliss Time  - damping that is applied to changes made to the interval setting.
; Nr. Octaves - number of layers in the additional octave stack. This is a 'parent' stack meaning that the 'child' stack discussed previously will be repeated at octave intervals.

; Layers - controls a number of additional dissonant layers added for each note played. This is a feature that is still under development

; Detune - two padsynth oscillators are present and they can be detuned with respect to one another and their seperation into the left and right channels can also be controller.

; Meter - master gain control and stereo meters  
                                                                                                                                                                                 
<Cabbage>
form caption("Pad Synth"), size(1120, 475), pluginId("PdSy"), colour(0,0,0)

#define PlantBoxAppearance # shape("sharp"), colour( 20, 20, 40), outlineColour("grey"), outlineThickness(2) #

image    bounds(  5,  5,470,130), plant("Main"), $PlantBoxAppearance {
label    bounds(  5,  4,490, 15), text("Main")
label    bounds(  5, 28,100, 13), text("Table")
combobox bounds(  5, 42,100, 22), channel("Table"), text("User 30","D.Bass","Clarinet","Bass Clarinet","CB.Clarinet","Oboe","Bassoon","C.Bassoon","Bass Ahh"), value(1)

label    bounds(  5, 68,100, 13), text("Tuning")
combobox bounds(  5, 82,100, 22), channel("Tuning"), text("Equal","Just","Pythagorean","Quarter Tones"), value(1)

rslider  bounds(100, 25, 90, 90), channel("Base"), text("Base Freq."), range(0,127,60,1,1), valueTextBox(1), textBox(1), popupText("Base frequency of the created waveform"), identChannel("BaseID")
rslider  bounds(170, 25, 90, 90), channel("BW"),   text("Bandwidth"),  range(1,999,6,0.25,0.01), valueTextBox(1), textBox(1), popupText("Bandwidth of Modulation")
rslider  bounds(240, 25, 90, 90), channel("PartScal"), text("Part.Scal."),  range(1,30,1.6,0.5,0.01), valueTextBox(1), textBox(1), popupText("Partial Scaling")
rslider  bounds(310, 25, 90, 90), channel("HarmStr"), text("Harm.Str."),  range(0.1,8,1,0.5,0.01), valueTextBox(1), textBox(1), popupText("Harmonic Stretch")
rslider  bounds(380, 25, 90, 90), channel("TabSize"), text("Table Size"),  range(1,24,18,1,1), valueTextBox(1), textBox(1), popupText("Larger values slow down table rebuilds!")
}

image    bounds(480,  5,635,130), plant("UserControlSliders"), identChannel("UserControlSlidersID"), $PlantBoxAppearance {
label    bounds(  4,  5,635, 15), text("User Table")
vslider  bounds( 25, 25, 10, 85), channel("1"),  range(0,1,0     ,0.5), visible(0)
vslider  bounds( 50, 25, 10, 85), channel("2"),  range(0,1,0.1     ,0.5), visible(0)
vslider  bounds( 75, 25, 10, 85), channel("3"),  range(0,1,0.2     ,0.5), visible(0)
vslider  bounds(100, 25, 10, 85), channel("4"),  range(0,1,0.1     ,0.5), visible(0)
vslider  bounds(125, 25, 10, 85), channel("5"),  range(0,1,0    ,0.5), visible(0)
vslider  bounds(150, 25, 10, 85), channel("6"),  range(0,1,0    ,0.5), visible(0)
vslider  bounds(175, 25, 10, 85), channel("7"),  range(0,1,0    ,0.5), visible(0)
vslider  bounds(200, 25, 10, 85), channel("8"),  range(0,1,0    ,0.5), visible(0)
vslider  bounds(225, 25, 10, 85), channel("9"),  range(0,1,0    ,0.5), visible(0)
vslider  bounds(250, 25, 10, 85), channel("10"), range(0,1,0    ,0.5), visible(0)
vslider  bounds(275, 25, 10, 85), channel("11"), range(0,1,0.1     ,0.5), visible(0)
vslider  bounds(300, 25, 10, 85), channel("12"), range(0,1,0.2     ,0.5), visible(0)
vslider  bounds(325, 25, 10, 85), channel("13"), range(0,1,0.4     ,0.5), visible(0)
vslider  bounds(350, 25, 10, 85), channel("14"), range(0,1,0.5     ,0.5), visible(0)
vslider  bounds(375, 25, 10, 85), channel("15"), range(0,1,0.7     ,0.5), visible(0)
vslider  bounds(400, 25, 10, 85), channel("16"), range(0,1,0.9     ,0.5), visible(0)
vslider  bounds(425, 25, 10, 85), channel("17"), range(0,1,0.5     ,0.5), visible(0)
vslider  bounds(450, 25, 10, 85), channel("18"), range(0,1,0.2     ,0.5), visible(0)
vslider  bounds(475, 25, 10, 85), channel("19"), range(0,1,0.001,0.5), visible(0)
vslider  bounds(500, 25, 10, 85), channel("20"), range(0,1,0    ,0.5), visible(0)
vslider  bounds(525, 25, 10, 85), channel("21"), range(0,1,0    ,0.5), visible(0)
vslider  bounds(550, 25, 10, 85), channel("22"), range(0,1,0    ,0.5), visible(0)
vslider  bounds(575, 25, 10, 85), channel("23"), range(0,1,0    ,0.5), visible(0)
vslider  bounds(600, 25, 10, 85), channel("24"), range(0,1,0    ,0.5), visible(0)

image  bounds( 20, 25, 20, 1), colour("silver"), identChannel("Slider1")  , outlineColour( 20, 20, 40), outlineThickness(1)
image  bounds( 40, 25, 20, 0), colour("silver"), identChannel("Slider2")  , outlineColour( 20, 20, 40), outlineThickness(1)
image  bounds( 60, 25, 20, 0), colour("silver"), identChannel("Slider3")  , outlineColour( 20, 20, 40), outlineThickness(1)
image  bounds( 80, 25, 20, 0), colour("silver"), identChannel("Slider4")  , outlineColour( 20, 20, 40), outlineThickness(1)
image  bounds(100 ,25, 20, 0), colour("silver"), identChannel("Slider5")  , outlineColour( 20, 20, 40), outlineThickness(1)
image  bounds(120, 25, 20, 0), colour("silver"), identChannel("Slider6")  , outlineColour( 20, 20, 40), outlineThickness(1)
image  bounds(140, 25, 20, 0), colour("silver"), identChannel("Slider7")  , outlineColour( 20, 20, 40), outlineThickness(1)
image  bounds(160, 25, 20, 0), colour("silver"), identChannel("Slider8")  , outlineColour( 20, 20, 40), outlineThickness(1)
image  bounds(180, 25, 20, 0), colour("silver"), identChannel("Slider9")  , outlineColour( 20, 20, 40), outlineThickness(1)
image  bounds(200, 25, 20, 0), colour("silver"), identChannel("Slider10") , outlineColour( 20, 20, 40), outlineThickness(1)
image  bounds(220, 25, 20, 0), colour("silver"), identChannel("Slider11") , outlineColour( 20, 20, 40), outlineThickness(1)
image  bounds(240, 25, 20, 0), colour("silver"), identChannel("Slider12") , outlineColour( 20, 20, 40), outlineThickness(1)
image  bounds(260, 25, 20, 0), colour("silver"), identChannel("Slider13") , outlineColour( 20, 20, 40), outlineThickness(1)
image  bounds(280, 25, 20, 0), colour("silver"), identChannel("Slider14") , outlineColour( 20, 20, 40), outlineThickness(1)
image  bounds(300, 25, 20, 0), colour("silver"), identChannel("Slider15") , outlineColour( 20, 20, 40), outlineThickness(1)
image  bounds(320, 25, 20, 0), colour("silver"), identChannel("Slider16") , outlineColour( 20, 20, 40), outlineThickness(1)
image  bounds(340, 25, 20, 0), colour("silver"), identChannel("Slider17") , outlineColour( 20, 20, 40), outlineThickness(1)
image  bounds(360, 25, 20, 0), colour("silver"), identChannel("Slider18") , outlineColour( 20, 20, 40), outlineThickness(1)
image  bounds(380, 25, 20, 0), colour("silver"), identChannel("Slider19") , outlineColour( 20, 20, 40), outlineThickness(1)
image  bounds(400, 25, 20, 0), colour("silver"), identChannel("Slider20") , outlineColour( 20, 20, 40), outlineThickness(1)
image  bounds(420, 25, 20, 0), colour("silver"), identChannel("Slider21") , outlineColour( 20, 20, 40), outlineThickness(1)
image  bounds(440, 25, 20, 0), colour("silver"), identChannel("Slider22") , outlineColour( 20, 20, 40), outlineThickness(1)
image  bounds(460, 25, 20, 0), colour("silver"), identChannel("Slider23") , outlineColour( 20, 20, 40), outlineThickness(1)
image  bounds(480, 25, 20, 0), colour("silver"), identChannel("Slider24") , outlineColour( 20, 20, 40), outlineThickness(1)
image  bounds(500, 25, 20, 0), colour("silver"), identChannel("Slider25") , outlineColour( 20, 20, 40), outlineThickness(1)
image  bounds(520, 25, 20, 0), colour("silver"), identChannel("Slider26") , outlineColour( 20, 20, 40), outlineThickness(1)
image  bounds(540, 25, 20, 0), colour("silver"), identChannel("Slider27") , outlineColour( 20, 20, 40), outlineThickness(1)
image  bounds(560, 25, 20, 0), colour("silver"), identChannel("Slider28") , outlineColour( 20, 20, 40), outlineThickness(1)
image  bounds(580, 25, 20, 0), colour("silver"), identChannel("Slider29") , outlineColour( 20, 20, 40), outlineThickness(1)
image  bounds(600, 25, 20, 0), colour("silver"), identChannel("Slider30") , outlineColour( 20, 20, 40), outlineThickness(1)

label  bounds( 20,106, 20,11), text("1")
label  bounds( 40,106, 20,11), text("2")
label  bounds( 60,106, 20,11), text("3")
label  bounds( 80,106, 20,11), text("4")
label  bounds(100,106, 20,11), text("5")
label  bounds(120,106, 20,11), text("6")
label  bounds(140,106, 20,11), text("7")
label  bounds(160,106, 20,11), text("8")
label  bounds(180,106, 20,11), text("9")
label  bounds(200,106, 20,11), text("10")
label  bounds(220,106, 20,11), text("11")
label  bounds(240,106, 20,11), text("12")
label  bounds(260,106, 20,11), text("13")
label  bounds(280,106, 20,11), text("14")
label  bounds(300,106, 20,11), text("15")
label  bounds(320,106, 20,11), text("16")
label  bounds(340,106, 20,11), text("17")
label  bounds(360,106, 20,11), text("18")
label  bounds(380,106, 20,11), text("19")
label  bounds(400,106, 20,11), text("20")
label  bounds(420,106, 20,11), text("21")
label  bounds(440,106, 20,11), text("22")
label  bounds(460,106, 20,11), text("23")
label  bounds(480,106, 20,11), text("24")
label  bounds(500,106, 20,11), text("25")
label  bounds(520,106, 20,11), text("26")
label  bounds(540,106, 20,11), text("27")
label  bounds(560,106, 20,11), text("28")
label  bounds(580,106, 20,11), text("29")
label  bounds(600,106, 20,11), text("30")
}               

image    bounds(  5,140,260,120), plant("AmplitudeEnvelope"), $PlantBoxAppearance {
label    bounds(  0,  4,260, 15), text("Amplitude Envelope")
rslider  bounds(  0, 30, 80, 80), channel("AAtt"), text("Attack"), range(0.001,5,0.5,0.5,0.001), valueTextBox(1), textBox(1)   
rslider  bounds( 60, 30, 80, 80), channel("ADec"), text("Decay"),  range(0.001,5,0.01,0.5,0.001), valueTextBox(1), textBox(1)
rslider  bounds(120, 30, 80, 80), channel("ASus"), text("Sustain"),range(0,    1,1,0.5,0.001), valueTextBox(1), textBox(1)
rslider  bounds(180, 30, 80, 80), channel("ARel"), text("Release"),range(0.001,5,0.5,0.5,0.001), valueTextBox(1), textBox(1)
}                               

image    bounds(270,140,380,120), plant("Filter"), $PlantBoxAppearance 
{
label    bounds(  0,  4,380, 15), text("Filter Bandwidth")
checkbox bounds( 10, 10, 80, 12), channel("FOnOff"), text("On/Off"), value(1)
rslider  bounds(  0, 30, 80, 80), channel("FL1"), text("Level 1"), range(0.001,9.999,9.999,0.5,0.001), valueTextBox(1), textBox(1)
rslider  bounds( 60, 30, 80, 80), channel("FT1"), text("Attack"), range(0.001,8,3,0.5), valueTextBox(1), textBox(1)
rslider  bounds(120, 30, 80, 80), channel("FSus"), text("Sustain"), range(0.001,9.999,1.5,0.5,0.01), valueTextBox(1), textBox(1)
rslider  bounds(180, 30, 80, 80), channel("FRelTim"), text("Rel.Time"), range(0.001,8,0.25,0.5), valueTextBox(1), textBox(1)
rslider  bounds(240, 30, 80, 80), channel("FRelLev"), text("Rel.Level"), range(0.001,9.999,0.1,0.5,0.01), valueTextBox(1), textBox(1)
rslider  bounds(300, 30, 80, 80), channel("FPos"), text("Position"), range(1,24,3,1,1), valueTextBox(1), textBox(1)
}

image    bounds(655,140,200,120), plant("Reverb"), $PlantBoxAppearance {
label    bounds(  0,  4,200, 15), text("Reverb")
rslider  bounds(  0, 30, 80, 80), channel("RSend"), text("Send"), range(0,1,0.5), valueTextBox(1), textBox(1)
rslider  bounds( 60, 30, 80, 80), channel("RSize"), text("Size"),  range(0,0.99,0.85), valueTextBox(1), textBox(1)
rslider  bounds(120, 30, 80, 80), channel("R__CF"), text("Damping"),range(20,20000,8000,0.5,1), valueTextBox(1), textBox(1)
}

image    bounds(860,140,255,120), plant("Highpass/Lowpass"), $PlantBoxAppearance {
label    bounds(  0,  4,255, 15), text("Highpass/Lowpass")
checkbox bounds( 10, 40, 80, 12), channel("HPFLPFOnOff"), text("On/Off"), value(1)
checkbox bounds( 10, 60, 80, 12), channel("ModWhl"), text("Mod.Wheel"), value(0)
rslider  bounds( 90, 30, 80, 80), channel("HPFFreq"), text("HPF Freq"),  range(4,14, 4), valueTextBox(1), textBox(1)
rslider  bounds(165, 30, 80, 80), channel("LPFFreq"), text("LPF Freq"),  range(4,14,14), valueTextBox(1), textBox(1)
}

image    bounds(  5,265,230,120), plant("Legato"), $PlantBoxAppearance {
label    bounds(  0,  4,230, 15), text("Legato")
checkbox bounds( 10, 10, 60, 12), channel("LegatoOnOff"), text("On/Off"), value(0)
combobox bounds( 10, 50, 60, 22), channel("LegType"), text("Line","Curve"), value(1)
rslider  bounds( 70, 30, 80, 80), channel("LegTime"), text("Legato Time"),  range(0,3,0.2,0.5), valueTextBox(1), textBox(1), popupText("Partial Scaling")
rslider  bounds(140, 30, 80, 80), channel("PBRange"), text("Bend Range"),  range(0,48,2,1,1), valueTextBox(1), textBox(1), popupText("Partial Scaling")
}

image    bounds(240,265,290,120), plant("Layers"), $PlantBoxAppearance 
{
label    bounds(  0,  4,290, 15), text("Pitch Stack")
rslider  bounds(  0, 30, 80, 80), channel("Interval"), range(-24,24,5,1,0.1), valueTextBox(1), textBox(1), text("Interval")
rslider  bounds( 70, 30, 80, 80), channel("Layers"),   range(1,20,1,1,1), valueTextBox(1), textBox(1), text("Nr. Layers")
rslider  bounds(140, 30, 80, 80), channel("GTime"),     range(0, 5,0.1), valueTextBox(1), textBox(1), text("Gliss Time")
rslider  bounds(210, 30, 80, 80), channel("Octaves"),  range(1, 8,1,1,1), valueTextBox(1), textBox(1), text("Nr. Octaves")
}

image    bounds(535,265,150,120), plant("Detune"), $PlantBoxAppearance 
{
label    bounds(  0,  4,150, 15), text("Detune")
rslider  bounds(  0, 30, 80, 80), channel("DetuneInterval"), text("Interval"), range(-100,100,10,1,1), valueTextBox(1), textBox(1)
rslider  bounds( 70, 30, 80, 80), channel("DetuneWidth"), text("Width"),   range(0,1,1), valueTextBox(1), textBox(1)
}

image    bounds(690,265,150,120), plant("Exciter"), identChannel("ExciterID") alpha(0.5) $PlantBoxAppearance 
{
label    bounds(  0,  4,150, 15), text("Exciter")
rslider  bounds(  0, 30, 80, 80), channel("ExciterAmount"), text("Amount"),  range(0,100,0,1,0.1), valueTextBox(1), textBox(1)
rslider  bounds( 70, 30, 80, 80), channel("ExciterFreq"), text("Freq."),  range(1000,10000,3000,1,1), valueTextBox(1), textBox(1)
}

image    bounds(845,265,270,120), plant("OutputMeter"), $PlantBoxAppearance 
{
label    bounds(  0,  4,270, 15), text("Meter")
rslider  bounds( 60, 30, 80, 80), channel("AGain"),text("Gain"),   range(0,  5,0.3,0.5,0.001), valueTextBox(1), textBox(1)
vmeter   bounds(160, 30, 15, 65) channel("vMeter1") value(0) overlayColour(0, 0, 0, 255) meterColour:0(255, 255, 0) meterColour:1(255, 103, 171) meterColour:2(250,250, 0) outlineThickness(1)
vmeter   bounds(180, 30, 15, 65) channel("vMeter2") value(0) overlayColour(0, 0, 0, 255) meterColour:0(255, 255, 0) meterColour:1(255, 103, 171) meterColour:2(250,250, 0) outlineThickness(1)
label    bounds(160, 96, 15, 15), text("L")
label    bounds(180, 96, 15, 15), text("R")
}

keyboard bounds(5,390,1110, 80)
</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-dm0 -n -+rtmidi=NULL -M0
</CsOptions>

<CsInstruments>

; sr set by host
ksmps    =    32
nchnls   =    2
0dbfs    =    1

massign    0,3                        ; DIRECT ALL MIDI EVENTS TO INSTRUMENT 3
giFade    ftgen    0,0,128,-7,0,60,0,36,1,128-60-36,1    ; KEY-FOLLOWING MAP DEFINING FADES BETWEEN TWO TABLES USED TO PREVENT ALIASING

giequal   ftgen  201,           0,        64,        -2,          12,         2,     cpsmidinn(60),        60,                       1, 1.059463,1.1224619,1.1892069,1.2599207,1.33483924,1.414213,1.4983063,1.5874001,1.6817917,1.7817962, 1.8877471,     2    ;STANDARD
gijust    ftgen  202,           0,        64,        -2,          12,         2,     cpsmidinn(60),        60,                       1,   16/15,    9/8,     6/5,      5/4,       4/3,     45/32,     3/2,     8/5,      5/3,       9/5,     15/8,     2        ;RATIOS FOR JUST INTONATION
gipyth    ftgen  203,           0,        64,        -2,          12,         2,     cpsmidinn(60),        60,                       1,  256/243,   9/8,    32/27,    81/64,      4/3,    729/512,    3/2,    128/81,   27/16,     16/9,     243/128,  2        ;RATIOS FOR PYTHAGOREAN TUNING
giquat    ftgen  204,           0,        64,        -2,          24,         2,     cpsmidinn(60),        60,                       1, 1.0293022,1.059463,1.0905076,1.1224619,1.1553525,1.1892069,1.2240532,1.2599207,1.2968391,1.33483924,1.3739531,1.414213,1.4556525,1.4983063, 1.54221, 1.5874001, 1.6339145,1.6817917,1.73107,  1.7817962,1.8340067,1.8877471,1.9430623,    2    ;QUARTER TONES

gaRvbL,gaRvbR    init    0                ; GLOBAL AUDIO VARIABLE USED TO MIX AND SEND SIGNAL TO THE REVERB INSTRUMENT

; INITIALISE TABLE
gk1,gk2,gk3,gk4,gk5,gk6,gk7,gk8,gk9,gk10,gk11,gk12,gk13,gk14,gk15,gk16,gk17,gk18,gk19,gk20,gk21,gk22,gk23,gk24,gk25,gk26,gk27,gk28,gk29,gk30    init    0
gkBW         init    6
gkPartScal   init    1.6
gkTabSize    init    18
gkBase       init    cpsmidinn(60)
gkHarmStr    init    1
gkTable      init    1
giTable      ftgen 1, 0, 2^i(gkTabSize), "padsynth", i(gkBase), i(gkBW), i(gkPartScal), i(gkHarmStr), 1, 1, 0.00001+(i(gk1)*0.99999),i(gk2),i(gk3),i(gk4),i(gk5),i(gk6),i(gk7),i(gk8),i(gk9),i(gk10),i(gk11),i(gk12),i(gk13),i(gk14),i(gk15),i(gk16),i(gk17),i(gk18),i(gk19),i(gk20),i(gk21),i(gk22),i(gk23),i(gk24),i(gk25),i(gk26),i(gk27),i(gk28),i(gk29),i(gk30)
giTable2     ftgen 2, 0, 2^i(gkTabSize), "padsynth", i(gkBase), i(gkBW), i(gkPartScal), i(gkHarmStr), 1, 1, 0.00001+(i(gk1)*0.99999),i(gk2),i(gk3),i(gk4),i(gk5)


instr    1    ; Always on. Reads widgets.
 kPortTime     linseg  0,0.01,0.1
 gkLegatoOnOff chnget  "LegatoOnOff"
 gkLegType     chnget  "LegType"
 gkLegTime     chnget  "LegTime"
 gkPBRange  chnget  "PBRange"

 gkHPFLPFOnOff chnget  "HPFLPFOnOff"
 gkHPFFreq     chnget  "HPFFreq"
 gkLPFFreq     chnget  "LPFFreq"
 kModWhl       chnget  "ModWhl"
 if kModWhl==1 then
  iHPF  ftgen  0,0,128,-7,4,32,4,96,14
  iLPF  ftgen  0,0,128,-7,4,96,14,32,14
  k1    ctrl7  1,1,0,1
        chnset table(k1,iHPF,1), "HPFFreq"
        chnset table(k1,iLPF,1), "LPFFreq"
 endif
 gkHPFFreq  portk   cpsoct(gkHPFFreq), kPortTime
 gkLPFFreq  portk   cpsoct(gkLPFFreq), kPortTime
 gkDetuneInterval  chnget  "DetuneInterval"
 gkDetuneWidth  chnget  "DetuneWidth" 

 gkExciterAmount chnget "ExciterAmount"    
 if trigger:k(gkExciterAmount,0.1,2)==1 then
  reinit UPDATE_EXCITER
 endif 
 UPDATE_EXCITER:
 if i(gkExciterAmount)>0 then
  chnset    "alpha(1)", "ExciterID"
 else
  chnset    "alpha(.5)", "ExciterID"
 endif
 gkExciterFreq  chnget  "ExciterFreq"
 rireturn
  
 kMOUSE_X   chnget  "MOUSE_X"                ; READ IN MOUSE X POSITION ON A RESERVED CABBAGE CHANNEL
 kMOUSE_Y   chnget  "MOUSE_Y"                ; READ IN MOUSE Y POSITION ON A RESERVED CABBAGE CHANNEL
 kMOUSE_DOWN_LEFT    chnget    "MOUSE_DOWN_LEFT"        ; LEFT CLICK STATUS (ONLY ALLOWING A REINIT WHEN THE MOUSE BUTTON IS RELEASED SMOOTHS INTERACTION)
 if kMOUSE_DOWN_LEFT==1 then                                ; IF LEFT CLICK BUTTON IS HELD...
 
 #define    SLIDER(X1'WIDTH'COUNT)
 #
  if kMOUSE_X>$X1 && kMOUSE_X<$X1+$WIDTH && kMOUSE_Y>10 && kMOUSE_Y<120 then    ; IF MOUSE LIES WITHIN BOUNDARYS OF SLIDER (IMAGE WIDGET)...
   kLen    limit    105 - kMOUSE_Y,0,80                            ; LENGTH OF SLIDER IN PIXELS. 
               ; N.B. MOUSE CAN STILL STRAY BEYOND LIMITS OF THE WIDGET SLIGHTLY WITH THE VALUE OUTPUT WILL STILL BE LIMITED TO BE HELD WITHIN RANGE
   if changed(kLen)==1 then                                                     ; IF MOUSE HAS MOVED WITHIN WIDGET AREA
    Sstr    sprintfk    "bounds(%d,%d,%d,%d)",$X1-480,25+(80-kLen),$WIDTH,kLen  ; NEW STRING FOR SLIDER POSITION
            chnset        Sstr,"Slider$COUNT"                                   ; SEND 'BOUNDS' MESSAGE TO WIDGET
    k$COUNT    =    (kLen/80)^1.5                                               ; CREATE THE ACTUAL VALUE OF THE SLIDER (RANGE 0 - 1)
   endif
  endif
#
 $SLIDER(500'20'1)
 $SLIDER(520'20'2)
 $SLIDER(540'20'3)
 $SLIDER(560'20'4)
 $SLIDER(580'20'5)
 $SLIDER(600'20'6)
 $SLIDER(620'20'7)
 $SLIDER(640'20'8)
 $SLIDER(660'20'9)
 $SLIDER(680'20'10)
 $SLIDER(700'20'11)
 $SLIDER(720'20'12)
 $SLIDER(740'20'13)
 $SLIDER(760'20'14)
 $SLIDER(780'20'15)
 $SLIDER(800'20'16)
 $SLIDER(820'20'17)
 $SLIDER(840'20'18)
 $SLIDER(860'20'19)
 $SLIDER(880'20'20)
 $SLIDER(900'20'21)
 $SLIDER(920'20'22)
 $SLIDER(940'20'23)
 $SLIDER(960'20'24)
 $SLIDER(980'20'25)
 $SLIDER(1000'20'26)
 $SLIDER(1020'20'27)
 $SLIDER(1040'20'28)
 $SLIDER(1060'20'29)
 $SLIDER(1080'20'30)

endif
 gkTable     chnget    "Table"
 gkTuning    chnget    "Tuning"
 if timeinstk()==1 then
  kMOUSE_DOWN_LEFT    =    1
 endif
 if trigger(kMOUSE_DOWN_LEFT,0.5,1)==1 then
  gkBase      chnget    "Base"
  gkBW        chnget    "BW"
  gkPartScal  chnget    "PartScal"
  gkHarmStr   chnget    "HarmStr"
  gkTabSize   chnget    "TabSize"
  
  gk1         =    k1    
  gk2         =    k2    
  gk3         =    k3
  gk4         =    k4
  gk5         =    k5
  gk6         =    k6
  gk7         =    k7
  gk8         =    k8
  gk9         =    k9
  gk10        =    k10
  gk11        =    k11
  gk12        =    k12
  gk13        =    k13
  gk14        =    k14
  gk15        =    k15
  gk16        =    k16
  gk17        =    k17
  gk18        =    k18
  gk19        =    k19
  gk20        =    k20
  gk21        =    k21
  gk22        =    k22
  gk23        =    k23
  gk24        =    k24
  gk25        =    k25
  gk26        =    k26
  gk27        =    k27
  gk28        =    k28
  gk29        =    k29
  gk30        =    k30
 endif

 gkAAtt         chnget    "AAtt"
 gkADec         chnget    "ADec"    
 gkASus         chnget    "ASus"    
 gkARel         chnget    "ARel"    
 gkAGain        chnget    "AGain"

 gkFOnOff       chnget    "FOnOff"
 gkFL1          chnget    "FL1"
 gkFT1          chnget    "FT1"
 gkFSus         chnget    "FSus"
 gkFRelTim      chnget    "FRelTim"
 gkFRelLev      chnget    "FRelLev"
 gkFPos         chnget    "FPos"

 gkRSend        chnget    "RSend"
 gkRSize        chnget    "RSize"
 gkR__CF        chnget    "R__CF"

 if changed(gkTable)==1 then
  kVisible    =    gkTable==1?1:0
   Smsg        sprintfk    "visible(%d)", kVisible
      chnset    Smsg, "BaseID"
      chnset    Smsg, "UserControlSlidersID"
 endif
endin

instr    2    ; Always on. Updates PadSynth table when relevant widgets are changed.
 ktrig    changed    gkBase,gkBW,gkPartScal,gkHarmStr,gkTabSize,gkTable,gk1,gk2,gk3,gk4,gk5,gk6,gk7,gk8,gk9,gk10,gk11,gk12,gk13,gk14,gk15,gk16,gk17,gk18,gk19,gk20,gk21,gk22,gk23,gk24,gk25,gk26,gk27,gk28,gk29,gk30 
 if ktrig==1 then
  reinit    UpdateTable
 endif
 UpdateTable:
 giBW  = i(gkBW)
 giPartScal    =    i(gkPartScal)
 giTabLen    =    2^i(gkTabSize)    ; IF THIS IS TOO SMALL, LOOPING OF THE SPECTRUM WILL BECOME AUDIBLE
 ;                                                                                        FIRST TWO VALUES FOR hampl NEED TO BE '1'.
 if i(gkTable)==1 then
  giBase        =    i(gkBase)
  giTable      ftgen 1, 0, giTabLen, "padsynth", giBase, giBW, giPartScal, i(gkHarmStr), 1, 1, 0.00001+(i(gk1)*0.99999),i(gk2),i(gk3),i(gk4),i(gk5),i(gk6),i(gk7),i(gk8),i(gk9),i(gk10),i(gk11),i(gk12),i(gk13),i(gk14),i(gk15),i(gk16),i(gk17),i(gk18),i(gk19),i(gk20),i(gk21),i(gk22),i(gk23),i(gk24),i(gk25),i(gk26),i(gk27),i(gk28),i(gk29),i(gk30)
  giTable2     ftgen 2, 0, giTabLen, "padsynth", giBase, giBW, giPartScal, i(gkHarmStr), 1, 1, 0.00001+(i(gk1)*0.99999),i(gk2),i(gk3),i(gk4),i(gk5)
 elseif i(gkTable)==2 then    ; double bass
  giBase        =    cpsmidinn(24)
  giTable      ftgen 1, 0, giTabLen, "padsynth", giBase, giBW, giPartScal, i(gkHarmStr), 1, 1, 0.050982,0.269147,1.000005,0.658602,0.409921,0.234611,0.371689,0.081631,0.193677,0.176088,0.325643,0.073972,0.139508,0.089290,0.133734,0.084015,0.019990,0.027201,0.017902,0.041521,0.007125,0.015291,0.015586,0.060973,0.014650,0.002686,0.018208,0.022200,0.005778,0.016678,0.011366,0.005039,0.010690,0.004182,0.004080,0.022196,0.001317,0.015622,0.006383,0.048913,0.043438,0.028458,0.019492,0.004084,0.022969,0.008893,0.009430,0.005096,0.011939,0.003356,0.002472,0.010081,0.003020,0.007064,0.020432,0.002298,0.004490,0.004640,0.005406,0.000578,0.002732,0.004642,0.003090,0.002380,0.003776,0.003052,0.002165,0.001143,0.004033,0.002833,0.000987,0.002324,0.003665,0.000774,0.003714,0.003576,0.003869,0.000651,0.001704,0.000521,0.002186,0.000385,0.001712,0.000832,0.000895,0.001616,0.000491,0.000556,0.000794,0.000267,0.000365,0.000915,0.000410,0.000555,0.000234,0.000876,0.001052,0.001316,0.000456
  giTable2     ftgen 2, 0, giTabLen, "padsynth", giBase, giBW, giPartScal, i(gkHarmStr), 1, 1, 0.050982,0.269147,1.000005,0.658602,0.409921
 elseif i(gkTable)==3 then    ; clarinet
  giBase        =    cpsmidinn(50)
  giTable      ftgen 1, 0, giTabLen, "padsynth", giBase, giBW, giPartScal, i(gkHarmStr), 1, 1, 0.273597,0.027434,0.737705,0.049480,0.448437,0.272536,0.131175,0.129945,0.283082,0.130714,0.026719,0.037582,0.018953,0.010729,0.067580,0.024573,0.016586,0.049988,0.033294,0.017090,0.008591,0.021128,0.007237,0.016060,0.016060
  giTable2     ftgen 2, 0, giTabLen, "padsynth", giBase, giBW, giPartScal, i(gkHarmStr), 1, 1, 0.273597,0.027434,0.737705,0.049480,0.448437
 elseif i(gkTable)==4 then    ; bass clarinet
  giBase        =    cpsmidinn(35)
  giTable      ftgen 1, 0, giTabLen, "padsynth", giBase, giBW, giPartScal, i(gkHarmStr), 1, 1, 0.384232,0.038085,0.578537,0.029558,0.143002,0.119033,0.397678,0.113380,0.577246,0.158831,0.577514,0.094994,0.440674,0.109136,0.500666,0.132354,0.360370,0.104810,0.231403,0.089565,0.207353,0.099773,0.209066,0.123801,0.158769,0.079383,0.036078,0.019583,0.010310,0.017060,0.029465,0.045821,0.031622,0.038326,0.052222,0.058647,0.083956,0.079748,0.081955,0.097274,0.069934,0.075100,0.049259,0.058121,0.068078,0.065276,0.070165,0.065898,0.072432,0.055423,0.052283,0.036547,0.034082,0.035287,0.044801,0.053917,0.050263,0.036979,0.034264,0.035892,0.035011,0.037199,0.041542,0.043201,0.039923,0.035164,0.035828,0.036193,0.037155,0.035493,0.034546,0.035091,0.029891,0.027394,0.026174,0.023757,0.021365,0.019468,0.016295,0.015301,0.015263,0.014310,0.013239,0.011972,0.011445,0.011727,0.012391,0.013892,0.015395,0.015147,0.015137,0.014816,0.013898,0.012682,0.011462,0.009883,0.008579,0.007797,0.007749
  giTable2     ftgen 2, 0, giTabLen, "padsynth", giBase, giBW, giPartScal, i(gkHarmStr), 1, 1, 0.384232,0.038085,0.578537,0.029558,0.143002
 elseif i(gkTable)==5 then    ; contrabass clarinet
  giBase        =    cpsmidinn(26)
  giTable      ftgen 1, 0, giTabLen, "padsynth", giBase, giBW, giPartScal, i(gkHarmStr), 1, 1, 0.100160,0.005519,0.631940,0.013046,0.749042,0.308868,0.475605,0.152792,0.074315,0.238927,0.152260,0.251283,0.030787,0.052465,0.032473,0.121972,0.064172,0.090564,0.043994,0.091868,0.039563,0.058622,0.024531,0.023127,0.026665,0.067522,0.081377,0.057914,0.066176,0.036134,0.026135,0.021056,0.038011,0.036534,0.058393,0.040915,0.050051,0.038446,0.034166,0.021341,0.014481,0.015708,0.025527,0.026622,0.033577,0.027355,0.034434,0.022920,0.016354,0.010905,0.011160,0.015075,0.019871,0.017505,0.013189,0.011442,0.008511,0.007974,0.006368,0.005988,0.005976,0.005922,0.006590,0.008199,0.006566,0.005254,0.004955,0.005576,0.005463,0.005101,0.003955,0.003622,0.004027,0.003772,0.003504,0.002848,0.002183,0.002075,0.002143,0.002014,0.001907,0.001850,0.001736,0.001543,0.001318,0.001180,0.001107,0.001066,0.001169,0.001372,0.001533,0.001667,0.001738,0.001655,0.001604,0.001603,0.001571,0.001575,0.001638,0.001696
  giTable2     ftgen 2, 0, giTabLen, "padsynth", giBase, giBW, giPartScal, i(gkHarmStr), 1, 1, 0.100160,0.005519,0.631940,0.013046,0.749042
 elseif i(gkTable)==6 then    ; oboe
  giBase        =    cpsmidinn(59)
  giTable      ftgen 1, 0, giTabLen, "padsynth", giBase, giBW, giPartScal, i(gkHarmStr), 1, 1, 0.239013,0.078927,0.241030,0.206347,0.832266,0.054540,0.013821,0.007450,0.022905,0.021737,0.018123,0.013105,0.002361,0.001433,0.003509,0.002589,0.001326,0.000743,0.000990,0.000868,0.000863,0.000994,0.000406,0.000288,0.000288
  giTable2     ftgen 2, 0, giTabLen, "padsynth", giBase, giBW, giPartScal, i(gkHarmStr), 1, 1, 0.239013,0.078927,0.241030,0.206347,0.832266
 elseif i(gkTable)==7 then    ; bassoon
  giBase        =    cpsmidinn(34)
  giTable      ftgen 1, 0, giTabLen, "padsynth", giBase, giBW, giPartScal, i(gkHarmStr), 1, 1, 0.213868,0.268493,0.244166,0.230801,0.105833,0.308087,0.556920,0.478956,0.523357,0.900665,0.213470,0.229596,0.031221,0.040117,0.067113,0.060158,0.030778,0.061028,0.143814,0.063668,0.126426,0.055009,0.049138,0.085854,0.113027,0.111464,0.088765,0.037342,0.050990,0.035579,0.040460,0.032219,0.027305,0.034141,0.019655,0.009315,0.008270,0.006748,0.006472,0.007088,0.008133,0.007046,0.007850,0.005791,0.006273,0.006847,0.007249,0.009398,0.010309,0.010418,0.010247,0.010333,0.009562,0.008180,0.009576,0.009469,0.008529,0.008844,0.008053,0.007565,0.008026,0.007284,0.007299,0.007423,0.008280,0.008374,0.008239,0.008512,0.009431,0.010246,0.010350,0.009381,0.008652,0.008150,0.007888,0.007951,0.008166,0.008210,0.007921,0.007548,0.007147,0.006991,0.006978,0.006527,0.005617,0.004781,0.004549,0.004707,0.004803,0.004640,0.004303,0.003866,0.003524,0.003348,0.003108,0.002766,0.002439,0.002278,0.002406,0.002733
  giTable2     ftgen 2, 0, giTabLen, "padsynth", giBase, giBW, giPartScal, i(gkHarmStr), 1, 1, 0.213868,0.268493,0.244166,0.230801,0.105833
 elseif i(gkTable)==8 then    ; contrabassoon
  giBase        =    cpsmidinn(38)
  giTable      ftgen 1, 0, giTabLen, "padsynth", giBase, giBW, giPartScal, i(gkHarmStr), 1, 1, 0.240531,0.304670,0.289169,0.727482,0.578083,0.169149,0.057305,0.193624,0.167977,0.206006,0.182632,0.057408,0.103574,0.044785,0.084239,0.068303,0.030771,0.133375,0.096231,0.037016,0.046566,0.020770,0.020264,0.015211,0.029647,0.018603,0.027940,0.062861,0.033828,0.015594,0.016305,0.025478,0.034356,0.038642,0.028451,0.026747,0.014914,0.016727,0.015524,0.013236,0.014264,0.013651,0.014838,0.016490,0.021208,0.017660,0.013009,0.014867,0.013013,0.010426,0.009144,0.009462,0.009367,0.009624,0.008079,0.007399,0.009012,0.009384,0.008661,0.009051,0.009394,0.010578,0.011610,0.012184,0.010442,0.009130,0.008795,0.008468,0.010039,0.011205,0.011438,0.011489,0.010526,0.008902,0.007391,0.006198,0.005970,0.005264,0.004331,0.003874,0.003514,0.003418,0.003518,0.003409,0.003150,0.003023,0.003047,0.003199,0.003624,0.003999,0.003839,0.003629,0.003712,0.003872,0.003974,0.003836,0.003596,0.003353,0.003177,0.003070
  giTable2     ftgen 2, 0, giTabLen, "padsynth", giBase, giBW, giPartScal, i(gkHarmStr), 1, 1, 0.240531,0.304670,0.289169,0.727482,0.578083
 elseif i(gkTable)==9 then    ; contrabassoon
  giBase        =    cpsmidinn(48)
  giTable      ftgen 1, 0, giTabLen, "padsynth", giBase, giBW, giPartScal, i(gkHarmStr), 1, 1, 0.001482,0.007584,0.024916,0.084669,0.120825,0.037691,0.032860,0.097651,0.021318,0.007128,0.001455,0.000373,0.000311,0.001646,0.006016,0.016943,0.049158,0.018410,0.045129,0.018996,0.013181,0.005616,0.002740,0.001081,0.000371,0.000296,0.000174,0.000109
  giTable2     ftgen 2, 0, giTabLen, "padsynth", giBase, giBW, giPartScal, i(gkHarmStr), 1, 1, 0.001482,0.007584,0.024916,0.084669,0.120825
 endif
 prints    "Ready!"
endin


opcode PoscilLayer,a,kiikkkp
 kCPS,iTable,iPhase,kInterval,kGTime,iLayers,iCount    xin
 kPortTime   linseg   0, 0.001, 1
 kIntervalL  portk    kInterval, kPortTime * rnd(i(kGTime))
 kAmp        limit    (iLayers+1) - iCount, 0, 1
 aSig        poscil   kAmp,kCPS,iTable,iPhase
 aMix        =        0
 if iCount<iLayers then
  aMix    PoscilLayer    kCPS*semitone(kIntervalL*iCount),iTable,iPhase,kInterval,kGTime,iLayers,iCount+1
 endif
             xout        aMix+aSig
endop

opcode PoscilOctave,a,kiikkiip
 kCPS,iTable,iPhase,kInterval,kGTime,iLayers,iOctaves,iCount    xin
 aSig   PoscilLayer   kCPS*octave(iCount-1), iTable, 0, kInterval, kGTime, iLayers
 aMix   =             0
 if iCount<iOctaves then
  aMix   PoscilOctave   kCPS, iTable, 0, kInterval, kGTime, iLayers, iOctaves, iCount+1
 endif
 xout aSig + aMix
endop


instr 3 ; triggered by MIDI
 icps cpsmidi
 icps   cpstmid  i(gkTuning) + 200

 gkcps = icps
 iAmp ampmidi 1
 if i(gkLegatoOnOff)==0 then ;polyphonic
   iNum    notnum
   event_i "i",4 + iNum*0.001,0,3600*24*365,icps,iAmp
   if release:k()==1 then
    turnoff2 4 + iNum*0.001,4,1
   endif
 elseif i(gkLegatoOnOff)==1 && active:i(4,0,1)==0 then
  event_i "i", 4, 0, -1, icps, iAmp
 endif
endin


instr    4    ; SOUNDING INSTRUMENT
 kRelease  release
 if i(gkLegatoOnOff)==0 then ; poly mode
  kcps = p4
 else                     ; mono mode
  if kRelease==0 then
   if i(gkLegType)==1 then
    kRamp     linseg  0,0.001,1
    kcps      lineto gkcps, gkLegTime*kRamp
   else
    kcps      portk gkcps, gkLegTime, i(gkcps)   
   endif
  endif
  if active:k(3)==0 then
   turnoff
  endif
 endif

 ivel    =        p5
 
; aAmpEnv    cossegr        0, i(gkAAtt),1, i(gkADec), i(gkASus), 36000, i(gkASus),i(gkARel), 0    ; SUSTAIN SEGMENT (36000) NECESSARY DUE TO cosseg BUG
 aAmpEnv    linsegr        0, i(gkAAtt),1, i(gkADec), i(gkASus),                  i(gkARel), 0    
 
 kInterval   chnget    "Interval"
 iLayers     chnget    "Layers"
 gkGTime     chnget    "GTime"
 iOctaves    chnget    "Octaves"
 ifreq       =         p4

 kporttime linseg   0, 0.001,0.1
 kBend     pchbend  0, i(gkPBRange)
 kBend     portk    kBend, kporttime 

 a1        PoscilOctave        (kcps*semitone(kBend)*sr*cent(gkDetuneInterval))/(giTabLen*giBase), giTable, 0,   kInterval, gkGTime, iLayers, iOctaves
 a2        PoscilOctave        (kcps*semitone(kBend)*sr*cent(-gkDetuneInterval))     /(giTabLen*giBase), giTable, 0.5, kInterval, gkGTime, iLayers, iOctaves    ; OFFSET RIGHT CHANNEL PHASE FOR STEREO BREADTH 
 
 aL        ntrpol              a1, a2, (1-gkDetuneWidth)*0.5
 aR        ntrpol              a2, a1, (1-gkDetuneWidth)*0.5
 
 aL        *=                ivel*aAmpEnv*gkAGain
 aR        *=                ivel*aAmpEnv*gkAGain
 
 if i(gkFOnOff)==1 then                                                            ; IF FILTER SWITCH IS 'ON'
  kBW         expsegr   i(gkFL1), i(gkFT1), i(gkFSus), i(gkFRelTim), i(gkFRelLev)  ; BANDWIDTH ENVELOPE
  kPortTime   linseg    0,0.001,0.1                                                ; RAMPING UP FUNCTION
  kCF         portk     kcps * semitone(kBend) * gkFPos, kPortTime                                  ; SMOOTH CHANGES TO CUTOFF POSITION COUNTER
  aL          reson     aL, kCF, kCF*kBW, 2                                        ; BANDPASS FILTER
  aR          reson     aR, kCF, kCF*kBW, 2
 endif 
 
 if i(gkHPFLPFOnOff)==1 then                                                       ; IF HIGHPASS/LOWPASS SWITCH IS 'ON'
  aL        buthp     aL, gkHPFFreq
  aL        buthp     aL, gkHPFFreq
  aR        buthp     aR, gkHPFFreq
  aR        buthp     aR, gkHPFFreq
  aL        butlp     aL, gkLPFFreq
  aL        butlp     aL, gkLPFFreq
  aR        butlp     aR, gkLPFFreq
  aR        butlp     aR, gkLPFFreq
 endif 

 /* EXCITER */
 if gkExciterAmount>0 then
  aEL     exciter   aL, gkExciterFreq, 20000, 1, 10
  aER     exciter   aR, gkExciterFreq, 20000, 1, 10
  aL      +=        aEL*gkExciterAmount
  aR      +=        aER*gkExciterAmount
 endif
 
     outs    aL,aR

 gaRvbL    +=    aL*gkRSend                                ; MIX IN DRY SIGNAL TO SEND TO REVERB
 gaRvbR    +=    aR*gkRSend
 
endin


instr    99    ; Reverb. Always on.
 aL,aR    reverbsc    gaRvbL,gaRvbR,gkRSize,gkR__CF
          outs        aL,aR
          clear       gaRvbL,gaRvbR
endin



instr 2000 ; meter and spectrogram

a1,a2 monitor

; meter
if   metro:k(10)==1 then
reinit REFRESH_METER
endif
REFRESH_METER:
kres init    0
kres limit   kres-0.001,0,1 
kres peak    a1                            
rireturn
chnset kres, "vMeter1"              
if release:k()==1 then
 chnset k(0), "vMeter1"              
endif

kresR init    0
kresR limit   kres-0.001,0,1 
kresR peak    a2                            
rireturn
chnset kresR, "vMeter2"              
if release:k()==1 then
 chnset k(0), "vMeter2"              
endif

endin

</CsInstruments>

<CsScore>
i 1  0 [3600*24*7]    ; READ IN WIDGETS (ALWAYS ON)
i 2  0.01 [3600*24*7] ; UPDATE TABLES (ALWAYS ON)
i 99 0 [3600*24*7]    ; REVERB (ALWAYS ON)
i 2000 0 [3600*24*7]  ; METER
</CsScore>

</CsoundSynthesizer>