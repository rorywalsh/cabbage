<Cabbage>
form caption("Spectroscope") size(790, 460), guiMode("queue"), pluginId("def1"), colour(70,50,60)
; bevel
image         bounds(  5,  5,690,330), colour(0,0,0,0), outlineThickness(10), outlineColour("Silver"), corners(20)
; grid
gentable      bounds( 10, 10,680,320), tableNumber(99),  tableGridColour("white")
signaldisplay bounds( 10, 10,680,320), colour("LightBlue"), alpha(0.85), displayType("spectroscope"), backgroundColour("Black"), zoom(-1), signalVariable("aSig", "a2"), channel("display")
rslider       bounds(705, 10, 70, 90), colour("white"), channel("Gain"), range(1,100,10,0.5), valueTextBox(1), text("Disp.Gain"), fontColour("White"), textColour("White")
label         bounds(700,106, 80, 12), text("Window Size"), fontColour("White")
combobox      bounds(700,120, 80, 20), text("16","32","64","128","256","512","1024","2048","4096","8192"), value(7), channel("WSize")
rslider       bounds(705,140, 70, 90), colour("white"), channel("Max"), range(2,32,2), valueTextBox(1), text("Max."), fontColour("White"), textColour("White")
rslider       bounds(705,230, 70, 90), colour("white"), channel("Min"), range(0,0.5), valueTextBox(1), text("Min."), fontColour("White"), textColour("White")

image   bounds(  5,335,780,120), colour(0,0,0,0), outlineThickness(2) {
label   bounds(  0,  4,780, 14), text("S O U N D   S O U R C E   P A R A M E T E R S  (gbuzz)"), fontColour("White")
combobox bounds( 5,  5,  80,  20), text("Oscillator","Live"), channel("Source"), value(1)
rslider bounds(175, 25, 80, 80), channel("OscFreq"), range(20,8000,440,0.5,1), text("Osc.Freq."), valueTextBox(1), fontColour("White"), textColour("White")
rslider bounds(260, 25, 80, 80), channel("OscAmp"), range(0,1,0.1,0.5), text("Osc.Amp."), valueTextBox(1), fontColour("White"), textColour("White")
rslider bounds(345, 25, 80, 80), channel("LHarm"), range(1,80,1,1,1), text("Lowest Harm"), valueTextBox(1), fontColour("White"), textColour("White")
rslider bounds(430, 25, 80, 80), channel("HHarm"), range(1,500,500,1,1), text("Highest Harm"), valueTextBox(1), fontColour("White"), textColour("White")
rslider bounds(515, 25, 80, 80), channel("Power"), range(0,1,0.8), text("Power"), valueTextBox(1), fontColour("White"), textColour("White")

}
</Cabbage>

<CsoundSynthesizer>
<CsOptions>
-n -d -+rtmidi=NULL -M0 --midi-key-cps=4 --midi-velocity-amp=5 --displays
</CsOptions>
<CsInstruments>
; Initialize the global variables. 
ksmps = 32
nchnls = 2
0dbfs = 1

gi1 ftgen 99,0,1024,10,0 ; table of zeroes

;instrument will be triggered by keyboard widget
instr 1

; SOUND SOURCE
kOscFreq cabbageGetValue "OscFreq"
kOscAmp  cabbageGetValue "OscAmp"
kLHarm   cabbageGetValue "LHarm"
kHHarm   cabbageGetValue "HHarm"
kPower   cabbageGetValue "Power"
kSource  cabbageGetValue "Source"
iCos  ftgen  3,0,131072,11,1
aOsc  gbuzz  kOscAmp, kOscFreq, kHHarm, kLHarm, kPower, 3

if kSource==1 then
 gaSig = aOsc
else
 aL,aR ins
 gaSig = aL+aR
endif
       outs gaSig, gaSig
endin

instr 2
aSig  =  gaSig

; SPECTROGRAM DISPLAY
kGain   cabbageGetValue "Gain"

kWSize cabbageGetValue "WSize"
kWSize init            7
kWSize =               2 ^ (kWSize + 3)
kWSize init            1024

kMax   cabbageGetValue "Max"
kMax   init            2

kMin   cabbageGetValue "Min"

if changed:k(kWSize,kMax,kMin)==1 then
 reinit UPDATE
endif
UPDATE:
iPeriod   =  0.001 ;i(kPeriod)
iWSize    =  i(kWSize)
iWType    =  0 ; (0 = rectangular)
iDBout    =  0 ; (0 = magnitude, 1 = decibels)
iWaitFlag =  0 ; (0 = no wait)
iMin      =  iWSize * i(kMin)
iMax      =  iWSize/i(kMax)
aSig      *= kGain                  ; aSig can't be scaled in the the 'display' line
          dispfft aSig, iPeriod, iWSize, iWType, iDBout, iWaitFlag, iMin, iMax

endin

</CsInstruments>
<CsScore>
;causes Csound to run for about 7000 years...
i 1 0 3600
i 2 0 3600
</CsScore>
</CsoundSynthesizer>
