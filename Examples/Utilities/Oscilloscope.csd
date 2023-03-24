
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; Oscilloscope
; Iain McCurdy, 2023

; Demonstration of the 'oscilloscope' mode in the signaldisplay widget with some suggestions for its usage

; Sound source is a 8-harmonic additive synthesis tone.
; The strengths of the 8 harmonics are adjusted using the 8 sliders and the results can be seen in the oscilloscope

; Update of the display - frequency with which the display is refilled - is controlled using 'X Scale'
;  This control has the consequence of scaling the displayed waveform on the X axis.
; There are two modes by which this is defined: 'X Free' and 'X Sync'
;  X Free - the time value for X Scale is defined in seconds.
;  X Sync - the refresh time is synced to the period of the oscillator waveform so a steady waveform can be displayed.
;           in this mode the dial controls the number of repetitions of the waveform shown
; Y Scale - scale the display in the Y axis. Audio output is unaffected.
; FREEZE  - freezes the display

<Cabbage>
form caption("Oscilloscope") size(470, 365), guiMode("queue"), pluginId("def1"), colour("DarkSlateGrey")

; OSCILLOSCOPE PARAMETERS
; bevel
image         bounds(  5,  5, 390, 230), colour(0,0,0,0), outlineThickness(10), outlineColour("Silver"), corners(20)
; grid
gentable      bounds( 10, 10, 380, 220), tableNumber(1),  tableGridColour("white")
; oscilloscope
signaldisplay bounds( 10, 10, 380, 220), colour("LightBlue"), alpha(0.85), displayType("waveform"), backgroundColour("Black"), zoom(-1), signalVariable("aSig", "a2"), channel("display")
image         bounds( 10,120, 380,   1), colour(100,100,100) ; x-axis indicator

combobox      bounds(400, 10,  60,  20), text("X Free","X Sync"), channel("PrdMode"), value(2)
rslider       bounds(400, 35,  60,  80), colour("white"), channel("Period"),     range(0.001,0.1,0.025,1,0.001), valueTextBox(1), text("X Scale"), fontColour("White"), textColour("White")
rslider       bounds(400, 35,  60,  80), colour("white"), channel("PeriodFrac"), range(1,32,5,1,1), valueTextBox(1), text("X Scale"), fontColour("White"), textColour("White"), visible(0)
rslider       bounds(400,123,  60,  80), colour("white"), channel("YScale"), range(0.001,10,2,0.5), valueTextBox(1), text("Y Scale"), fontColour("White"), textColour("White")
button        bounds(400,210,  60,  20), text("FREEZE"), colour:0(0,0,0), colour:1(150,150,255), channel("Freeze")

; SOUND SOURCE PARAMETERS
image bounds(5,240,460,120), colour(0,0,0,0), outlineThickness(2) {
label   bounds(  0,  4,490, 14), text("S O U N D   S O U R C E   P A R A M E T E R S"), fontColour("White")
combobox bounds( 5,  5,  80,  20), text("Oscillator","Live"), channel("Source"), value(1)
rslider bounds(  0, 25, 80, 80), channel("OscFreq"), range(20,8000,80,0.5,1), text("Osc.Freq."), valueTextBox(1), fontColour("White"), textColour("White")
rslider bounds( 75, 25, 80, 80), channel("OscAmp"), range(0,1,0.4,0.5), text("Osc.Amp."), valueTextBox(1), fontColour("White"), textColour("White")

label   bounds(148, 99, 75, 12), text("HARMONICS:"), fontColour("White")

vslider bounds(220, 25, 20, 85), channel("A1"), range(0,1,1,0.5), text("1"), valueTextBox(0), fontColour("White"), textColour("White")
vslider bounds(250, 25, 20, 85), channel("A2"), range(0,1,0,0.5), text("2"), valueTextBox(0), fontColour("White"), textColour("White")
vslider bounds(280, 25, 20, 85), channel("A3"), range(0,1,0,0.5), text("3"), valueTextBox(0), fontColour("White"), textColour("White")
vslider bounds(310, 25, 20, 85), channel("A4"), range(0,1,0,0.5), text("4"), valueTextBox(0), fontColour("White"), textColour("White")
vslider bounds(340, 25, 20, 85), channel("A5"), range(0,1,0,0.5), text("5"), valueTextBox(0), fontColour("White"), textColour("White")
vslider bounds(370, 25, 20, 85), channel("A6"), range(0,1,0,0.5), text("6"), valueTextBox(0), fontColour("White"), textColour("White")
vslider bounds(400, 25, 20, 85), channel("A7"), range(0,1,0,0.5), text("7"), valueTextBox(0), fontColour("White"), textColour("White")
vslider bounds(430, 25, 20, 85), channel("A8"), range(0,1,0,0.5), text("8"), valueTextBox(0), fontColour("White"), textColour("White")
}
</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-n -dm0 -+rtmidi=NULL --displays
</CsOptions>

<CsInstruments>

; Initialize the global variables. 
ksmps = 32
nchnls = 2
0dbfs = 1

gi1 ftgen 1,0,1024,10,0 ; table of zeroes


instr 1
kOscFreq,kTOscFreq cabbageGetValue "OscFreq"
kOscAmp            cabbageGetValue "OscAmp"

kPeriod,kTPeriod cabbageGetValue "Period"
kPeriod          init            0.05

kPeriodFrac,kTPeriodFrac cabbageGetValue "PeriodFrac"
kPeriodFrac              init            1

kPrdMode, kTPrdMode cabbageGetValue "PrdMode"

kSource             cabbageGetValue "Source"

if kTPrdMode==1 then
 if kPrdMode==1 then
  cabbageSet kTPrdMode,"Period", "visible", 1
  cabbageSet kTPrdMode,"PeriodFrac", "visible", 0
 else
  cabbageSet kTPrdMode,"Period", "visible", 0
  cabbageSet kTPrdMode,"PeriodFrac", "visible", 1
 endif
endif

kPeriod = kPrdMode == 1 ? kPeriod : 2 * 80/(kOscFreq*2^kPeriodFrac) ;* kOscFreq/(2^kPeriodFrac)

kYScale cabbageGetValue "YScale"
kFreeze cabbageGetValue "Freeze"

kA1,kT1 cabbageGetValue "A1"
kA2,kT2 cabbageGetValue "A2"
kA3,kT3 cabbageGetValue "A3"
kA4,kT4 cabbageGetValue "A4"
kA5,kT5 cabbageGetValue "A5"
kA6,kT6 cabbageGetValue "A6"
kA7,kT7 cabbageGetValue "A7"
kA8,kT8 cabbageGetValue "A8"

if (kT1+kT2+kT3+kT4+kT5+kT6+kT7+kT8+kTPeriodFrac+kTPeriod+kTOscFreq+kTPrdMode)>0 then
 reinit RebuildTable
endif
RebuildTable:
iTab ftgen 555, 0, 4096, 10, i(kA1), i(kA2), i(kA3), i(kA4), i(kA5), i(kA6), i(kA7), i(kA8)

aOsc  poscil  kOscAmp, kOscFreq, 555, -1
rireturn

if kSource==1 then
 aSig = aOsc
else
 aL,aR ins
 aSig = aL+aR
endif

outs aSig*0.1, aSig*0.1


if (kTPeriodFrac+kTPeriod+kTOscFreq+kTPrdMode)>0 then
 reinit RestartDisplay
endif
RestartDisplay:

iPeriod   =  i(kPeriod)
aSig      *= kYScale                  ; aSig can't be scaled in the the 'display' line
if kFreeze==0 then
 display	aSig, iPeriod
endif

endin

</CsInstruments>
<CsScore>
;causes Csound to run for about 7000 years...
i 1 0 3600
</CsScore>
</CsoundSynthesizer>
