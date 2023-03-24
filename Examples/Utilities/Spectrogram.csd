<Cabbage>
form caption("Spectrogram") size(690,510), guiMode("queue"), pluginId("SpTr"), colour("DarkSlateGrey")

; SPECTROGRAM SETTINGS
signaldisplay bounds( 10, 10,480,360), colour("LightBlue"), displayType("spectrogram"), backgroundColour(100,100,100), zoom(-1), signalVariable("aSig", "a2"), channel("display")
rslider       bounds(500, 20, 70, 90), colour("white"), channel("Gain"), range(1,100,50,0.5), valueTextBox(1), text("Display Gain"), fontColour("White"), textColour("White")
;rslider       bounds(570, 20, 70, 90), channel("period"), text("Period"), range(0.01, 0.5, 0.01,1,0.01), valueTextBox(1), fontColour("White"), textColour("White")
label         bounds(510,126, 80, 12), text("Window Size"), fontColour("White")
combobox      bounds(510,140, 80, 20), text("16","32","64","128","256","512","1024","2048","4096","8192"), value(7), channel("WSize")
rslider       bounds(500,170, 70, 90), colour("white"), channel("Max"), range(2,32,4,1,1), valueTextBox(1), text("Max."), fontColour("White"), textColour("White")
label         bounds(565,210, 90, 15), fontColour("white"), text("= W.Size/Max")
rslider       bounds(500,270, 70, 90), colour("white"), channel("Min"), range(0,0.5), valueTextBox(1), text("Min."), fontColour("White"), textColour("White")
label         bounds(565,310,100, 15), fontColour("white"), text("= Nyquist*Min")

; SOUND SOURCE PARAMETERS
image bounds( 10,380,480,120), colour(0,0,0,0), outlineThickness(2) {
label   bounds( 40,  4,450, 14), text("S O U N D   S O U R C E   P A R A M E T E R S  (gbuzz)"), fontColour("White")
combobox bounds( 5,  5,  80,  20), text("Oscillator","Live"), channel("Source"), value(1)
rslider bounds(100, 25, 90, 80), channel("OscFreq"), range(20,8000,440,0.5,1), text("Osc.Freq."), valueTextBox(1), fontColour("White"), textColour("White")
rslider bounds(185, 25, 90, 80), channel("OscAmp"), range(0,1,0.1,0.5), text("Osc.Amp."), valueTextBox(1), fontColour("White"), textColour("White")
rslider bounds(270, 25, 90, 80), channel("LHarm"), range(1,80,1,1,1), text("Lowest Harm"), valueTextBox(1), fontColour("White"), textColour("White")
rslider bounds(355, 25, 90, 80), channel("Power"), range(0,1,0.7), text("Power"), valueTextBox(1), fontColour("White"), textColour("White")

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


;instrument will be triggered by keyboard widget
instr 1
;kPeriod cabbageGetValue "Period"
kPeriod init   0.1
kGain   cabbageGetValue "Gain"

kOscFreq cabbageGetValue "OscFreq"
kOscAmp  cabbageGetValue "OscAmp"
kLHarm   cabbageGetValue "LHarm"
kPower    cabbageGetValue "Power"

kSource             cabbageGetValue "Source"

iCos  ftgen  3,0,131072,11,1
aOsc  gbuzz  kOscAmp, kOscFreq, int(((sr*0.5)/kOscFreq)), kLHarm, kPower, 3

if kSource==1 then
 aSig = aOsc
else
 aL,aR ins
 aSig = aL+aR
endif


outs aSig, aSig

kWSize cabbageGetValue "WSize"
kWSize init            7
kWSize =               2 ^ (kWSize + 3)
kWSize init            1024

kMax   cabbageGetValue "Max"
kMax   init            2

kMin   cabbageGetValue "Min"

if changed:k(kPeriod,kWSize,kMax,kMin)==1 then
 reinit UPDATE
endif
UPDATE:
iWSize    =  i(kWSize)
iWType    =  0 ; (0 = rectangular)
iDBout    =  0 ; (0 = magnitude, 1 = decibels)
iWaitFlag =  0 ; (0 = no wait)
iMin      =  iWSize * i(kMin)
iMax      =  iWSize/i(kMax)
aSig      *= kGain                  ; aSig can't be scaled in the the 'display' line
          dispfft aSig, i(kPeriod), iWSize, iWType, iDBout, iWaitFlag, iMin, iMax
rireturn
endin

</CsInstruments>
<CsScore>
;causes Csound to run for about 7000 years...
i 1 0 3600
</CsScore>
</CsoundSynthesizer>
