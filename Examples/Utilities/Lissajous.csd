
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; Lissajous
; Iain McCurdy 2023

; Uses signaldisplay widget to display Lissajous patterns

; Lissagous patterns are created by sending different waveforms to the X and Y inputs of an oscilloscope. 
; In a normal oscilloscope the X input is simply a phase pointer.

; In this example, the audio output is the multiplication (ring modulation) of the two input audio signals.

; DISPLAY CONTROLS
; Period   - time period over which the curve is drawn
; X Scale  - display scaling of X input, audio level unaffected
; Y Scale  - display scaling of Y input, audio level unaffected

; SOUND SOURCE PARAMETERS
; A1 - A8  - amplitude of the first 8 harmonics of a waveform
; P1 - P8  - phase offsets of the first 8 harmonics of a waveform
; Osc. X Freq.    - frequency of the X input waveform
; Osc. X Amp.     - amplitude of the X input waveform, negative values invert the polarity.
; Osc. X Waveform - waveform of the X input oscillator - sine or the 8-harmonic waveform defined above

; Osc. Y Freq.    - frequency of the Y input waveform
; Spin            - a small value that scales Y oscillator frequency in cents, 
;    the upshot is that if the basic frequencies of the X and Y oscillators are the same of in integer ratio,
;    then non-zero values of 'Spin' will cause the lissajous pattern to visibly rotate.
; Osc. Y Amp.     - amplitude of the Y input waveform, negative values invert the polarity.
; RESYNC          - causes the two oscillators to restart form their initial phase locations
; Osc. Y Waveform - waveform of the Y input oscillator - sine or the 8-harmonic waveform defined above
; Osc. Y Phase    - initial phase of the Y input oscillator with respect to the X input oscillastor

<Cabbage>
form caption("Lissajous") size(595, 850), guiMode("queue"), pluginId("def1"), colour(40,40,60)

; LISSAJOUS WIDGETS
; bevel
image         bounds(  5,  5, 510, 510), colour(0,0,0,0), outlineThickness(10), outlineColour("Silver"), corners(20)
; grid
gentable      bounds( 10, 10, 500, 500), tableNumber(99),  tableGridColour("lime")
signaldisplay bounds( 10, 10, 500, 500), backgroundColour(255,255,255), colour(0,0,0), alpha(0.85), displayType("lissajous"), zoom(-1), signalVariable("aSigX", "aSigY"), channel("display")
rslider       bounds(510, 20,  90,  80), colour("white"), channel("Period"), range(0.0001,1,0.025,0.5,0.00001), valueTextBox(1), text("Period"), fontColour("White"), textColour("White")
rslider       bounds(510,115,  90,  80), colour("white"), channel("XScale"), range(0.001,10,9.5,0.5), valueTextBox(1), text("X Scale"), fontColour("White"), textColour("White")
rslider       bounds(510,215,  90,  80), colour("white"), channel("YScale"), range(0.001,10,0.95,0.5), valueTextBox(1), text("Y Scale"), fontColour("White"), textColour("White")
button        bounds(525,320,  60,  20), text("FREEZE"), colour:0(0,0,0), colour:1(150,150,255), channel("Freeze")
checkbox      bounds(527,350,  58,  18), text("GRID"), colour:0(0,0,0), colour:1(255,255,155), channel("Grid"), value(1), fontColour:0("White"), fontColour:1("White")

; changing display colours doesn't seem to work yet
;checkbox      bounds(527,375,  58,  18), text("INV."), colour:0(0,0,0), colour:1(255,255,155), channel("Inv"), value(0), fontColour:0("White"), fontColour:1("White")

rslider       bounds(510,400,  90,  80), colour("white"), channel("Noise"), range(0,1,0,0.5), valueTextBox(1), text("Noise"), fontColour("White"), textColour("White")

; SOUND SOURCE WIDGETS
image    bounds(  5,520,570,320), colour(0,0,0,0), outlineThickness(2) {
label    bounds(  0,  4,570, 14), text("S O U N D   S O U R C E   P A R A M E T E R S"), fontColour("White")

vslider bounds(  5, 25, 30, 85), channel("A1"), range(0,1,1),  text("A1"), valueTextBox(0), fontColour("White"), textColour("White")
vslider bounds( 25, 25, 30, 85), channel("P1"), range(0,360,0), text("P1"), valueTextBox(0), fontColour("White"), textColour("White")
vslider bounds( 45, 25, 30, 85), channel("N1"), range(1,128,1,1,1), text("N1"), valueTextBox(0), fontColour("White"), textColour("White")

vslider bounds( 75, 25, 30, 85), channel("A2"), range(0,1,0),  text("A2"), valueTextBox(0), fontColour("White"), textColour("White")
vslider bounds( 95, 25, 30, 85), channel("P2"), range(0,360,0), text("P2"), valueTextBox(0), fontColour("White"), textColour("White")
vslider bounds(115, 25, 30, 85), channel("N2"), range(1,128,2,1,1), text("N2"), valueTextBox(0), fontColour("White"), textColour("White")

vslider bounds(145, 25, 30, 85), channel("A3"), range(0,1,0),  text("A3"), valueTextBox(0), fontColour("White"), textColour("White")
vslider bounds(165, 25, 30, 85), channel("P3"), range(0,360,0), text("P3"), valueTextBox(0), fontColour("White"), textColour("White")
vslider bounds(185, 25, 30, 85), channel("N3"), range(1,128,3,1,1), text("N3"), valueTextBox(0), fontColour("White"), textColour("White")

vslider bounds(215, 25, 30, 85), channel("A4"), range(0,1,0),  text("A4"), valueTextBox(0), fontColour("White"), textColour("White")
vslider bounds(235, 25, 30, 85), channel("P4"), range(0,360,0), text("P4"), valueTextBox(0), fontColour("White"), textColour("White")
vslider bounds(255, 25, 30, 85), channel("N4"), range(1,128,4,1,1), text("N4"), valueTextBox(0), fontColour("White"), textColour("White")

vslider bounds(285, 25, 30, 85), channel("A5"), range(0,1,0),text("A5"), valueTextBox(0), fontColour("White"), textColour("White")
vslider bounds(305, 25, 30, 85), channel("P5"), range(0,360,0), text("P5"), valueTextBox(0), fontColour("White"), textColour("White")
vslider bounds(325, 25, 30, 85), channel("N5"), range(1,128,5,1,1), text("N5"), valueTextBox(0), fontColour("White"), textColour("White")

vslider bounds(355, 25, 30, 85), channel("A6"), range(0,1,0),  text("A6"), valueTextBox(0), fontColour("White"), textColour("White")
vslider bounds(375, 25, 30, 85), channel("P6"), range(0,360,0), text("P6"), valueTextBox(0), fontColour("White"), textColour("White")
vslider bounds(395, 25, 30, 85), channel("N6"), range(1,128,6,1,1), text("N6"), valueTextBox(0), fontColour("White"), textColour("White")

vslider bounds(425, 25, 30, 85), channel("A7"), range(0,1,0),text("A7"), valueTextBox(0), fontColour("White"), textColour("White")
vslider bounds(445, 25, 30, 85), channel("P7"), range(0,360,0), text("P7"), valueTextBox(0), fontColour("White"), textColour("White")
vslider bounds(465, 25, 30, 85), channel("N7"), range(1,128,7,1,1), text("N7"), valueTextBox(0), fontColour("White"), textColour("White")

vslider bounds(495, 25, 30, 85), channel("A8"), range(0,1,0.2),  text("A8"), valueTextBox(0), fontColour("White"), textColour("White")
vslider bounds(515, 25, 30, 85), channel("P8"), range(0,360,0), text("P8"), valueTextBox(0), fontColour("White"), textColour("White")
vslider bounds(535, 25, 30, 85), channel("N8"), range(1,128,8,1,1), text("N8"), valueTextBox(0), fontColour("White"), textColour("White")

rslider  bounds(  0,125, 80, 80), channel("OscXFreq"), range(20,8000,100,0.5, 0.01), text("Osc. X Freq."), valueTextBox(1), fontColour("White"), textColour("White")
rslider  bounds(150,125, 80, 80), channel("OscXAmp"), range(-1,1,0.1), text("Osc. X Amp."), valueTextBox(1), fontColour("White"), textColour("White")
label    bounds(235,125, 90, 12), text("Osc. X Waveform"), fontColour("White")
listbox  bounds(235,140, 90, 40), text("SINE","8-Harm"), value(2), channel("OscXTab")

button   bounds( 15,210, 55,  13), text("LINK"), colour:0(0,0,0), colour:1(250,150,255), channel("Link")

rslider  bounds(  0,225, 80, 80), channel("OscYFreq"), range(20,8000,100,0.5,0.01), text("Osc. Y Freq."), valueTextBox(1), fontColour("White"), textColour("White")
rslider  bounds( 75,225, 80, 80), channel("Spin"),  range(-100,100,1,1,0.01), text("Spin"), valueTextBox(1), fontColour("White"), textColour("White")
rslider  bounds(150,225, 80, 80), channel("OscYAmp"), range(-1,1,1), text("Osc. Y Amp."), valueTextBox(1), fontColour("White"), textColour("White")

label    bounds(235,225, 90, 12), text("Osc. Y Waveform"), fontColour("White")
listbox  bounds(235,240, 90, 40), text("SINE","8-Harm"), value(2), channel("OscYTab")

label    bounds(335,225, 90, 12), text("Osc. Y Phase"), fontColour("White")
combobox bounds(335,240, 80, 20), text("0","45","90","135","180","225","270","315","360"), value(3), channel("OscYPhs")
button   bounds(335,270,  60, 20),text("RESYNC"), latched(0), colour:0(0,0,0), colour:1(150,150,255), channel("Resync")
}
</Cabbage>

<CsoundSynthesizer>
<CsOptions>
-n -+rtmidi=NULL -dm0 --displays
</CsOptions>
<CsInstruments>
; Initialize the global variables. 
ksmps = 32
nchnls = 2
0dbfs = 1

gi1 ftgen 99,0,1024,10,0 ; table of zeroes

;instrument will be triggered by keyboard widget
instr 1
kPeriod cabbageGetValue "Period"
kPeriod init            0.05
kXScale cabbageGetValue "XScale"
kYScale cabbageGetValue "YScale"
kFreeze cabbageGetValue "Freeze"
kGrid,kT cabbageGetValue "Grid"
cabbageSet kT,"display","alpha", 1-kGrid*0.15

; changing table colours doesn't seem to work yet
;kInv,kT2 cabbageGetValue "Inv"
;cabbageSet kT2,"display","backgroundColour", 255*kInv, 255*kInv, 255*kInv
;cabbageSet kT2,"display","colour", 255-(255*kInv), 255-(255*kInv), 255-(255*kInv)

kA1,kT1 cabbageGetValue "A1"
kA2,kT2 cabbageGetValue "A2"
kA3,kT3 cabbageGetValue "A3"
kA4,kT4 cabbageGetValue "A4"
kA5,kT5 cabbageGetValue "A5"
kA6,kT6 cabbageGetValue "A6"
kA7,kT7 cabbageGetValue "A7"
kA8,kT8 cabbageGetValue "A8"

kP1,kPT1 cabbageGetValue "P1"
kP2,kPT2 cabbageGetValue "P2"
kP3,kPT3 cabbageGetValue "P3"
kP4,kPT4 cabbageGetValue "P4"
kP5,kPT5 cabbageGetValue "P5"
kP6,kPT6 cabbageGetValue "P6"
kP7,kPT7 cabbageGetValue "P7"
kP8,kPT8 cabbageGetValue "P8"

kN1,kNT1 cabbageGetValue "N1"
kN2,kNT2 cabbageGetValue "N2"
kN3,kNT3 cabbageGetValue "N3"
kN4,kNT4 cabbageGetValue "N4"
kN5,kNT5 cabbageGetValue "N5"
kN6,kNT6 cabbageGetValue "N6"
kN7,kNT7 cabbageGetValue "N7"
kN8,kNT8 cabbageGetValue "N8"

kOscXFreq cabbageGetValue "OscXFreq"
kOscXAmp  cabbageGetValue "OscXAmp"
kOscXTab  cabbageGetValue "OscXTab"
kOscXTab  init  2

kOscYFreq cabbageGetValue "OscYFreq"
kLink     cabbageGetValue "Link"
          cabbageSetValue "OscYFreq", kOscXFreq, trigger:k(kLink,0.5,0)
          cabbageSetValue "OscYFreq", kOscXFreq, changed:k(kOscXFreq)*kLink
          cabbageSetValue "OscXFreq", kOscYFreq, changed:k(kOscYFreq)*kLink

kOscYAmp  cabbageGetValue "OscYAmp"
kSpin     cabbageGetValue "Spin"
kResync   cabbageGetValue "Resync"
kOscYTab  cabbageGetValue "OscYTab"
kOscYTab  init  1
kOscYPhs  cabbageGetValue "OscYPhs"
kOscYPhs  init  3
kOscYPhs  =     (kOscYPhs-1)/8

iSine     ftgen  1,0,4096,10,1

if (kT1+kT2+kT3+kT4+kT5+kT6+kT7+kT8+kPT1+kPT2+kPT3+kPT4+kPT5+kPT6+kPT7+kPT8+kN1+kN2+kN3+kN4+kN5+kN6+kN7+kN8)>0 then
 reinit NewTable
endif
NewTable:
iTab ftgen 2, 0, 4096, 19, i(kN1), i(kA1), i(kP1), 0,  i(kN2), i(kA2), i(kP2), 0, i(kN3), i(kA3), i(kP3), 0, i(kN4), i(kA4), i(kP4), 0, i(kN5), i(kA5), i(kP5), 0, i(kN6), i(kA6), i(kP6), 0, i(kN7), i(kA7), i(kP7), 0, i(kN8), i(kA8), i(kP8), 0
rireturn

kNoise cabbageGetValue "Noise"

if trigger:k(kResync,0.5,1)==1 || changed:k(kOscXTab,kOscYTab,kOscYPhs)==1 then
 reinit RESYNC
endif
RESYNC:
;aSigX  poscil  kOscXAmp, kOscXFreq+(gauss:a(kNoise*kOscXFreq*100)), i(kOscXTab)
;aSigY  poscil  kOscYAmp, kOscYFreq+(gauss:a(kNoise*kOscXFreq*100))*cent(kSpin), i(kOscYTab), i(kOscYPhs)
aSigX  poscil  kOscXAmp, kOscXFreq, i(kOscXTab)
aSigY  poscil  kOscYAmp, kOscYFreq*cent(kSpin), i(kOscYTab), i(kOscYPhs)
rireturn

aSigXA  poscil  kOscXAmp, kOscXFreq+gauss:a(kNoise*kOscXFreq^2), i(kOscXTab)
aSigYA  poscil  kOscYAmp, kOscYFreq+gauss:a(kNoise*kOscXFreq^2)*cent(kSpin), i(kOscYTab), i(kOscYPhs)

outs aSigXA*aSigY, aSigXA*aSigY

if changed:k(kPeriod)==1 then
 reinit UPDATE
endif
UPDATE:
iPeriod   =  i(kPeriod)
iWaitFlag =  0
aSigX     *= kXScale                  ; aSig can't be scaled in the the 'display' line
aSigY *= kYScale

kNoise cabbageGetValue "Noise"
aSigX += gauss:a(kNoise)
aSigY += gauss:a(kNoise)



if kFreeze==0 then
 display	aSigX, iPeriod, iWaitFlag
endif
display	aSigY, iPeriod, 1
rireturn

endin

</CsInstruments>
<CsScore>
;causes Csound to run for about 7000 years...
i 1 0 3600
</CsScore>
</CsoundSynthesizer>
