
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; Dust.csd
; Written by Iain McCurdy, 2013.

; A simple encapsulation of the 'dust' and 'dust2' opcodes.
; Added features are stereo panning (spread) of the dust, a random tonal variation (lowpass filter with jumping cutoff frequency) and constant low and highpass filters.

<Cabbage>
form               size(410,190), caption("Dust"), pluginId("dust"), guiRefresh(8)
image    bounds(  0,  0,410,190), colour(155, 50, 50), shape("sharp"), outlineColour("white"), outlineThickness(2) 
checkbox bounds( 10, 20, 60, 15), text("On/Off"), channel("onoff"), value(0), fontColour:0("white"), fontColour:1("white")
combobox bounds( 10, 40, 60, 18), text("dust","dust2"), channel("opcode"), value(2)
rslider  bounds( 70, 20, 60, 60), text("Amplitude"), channel("amp"),     range(0, 100.00, 0.5, 0.5, 0.001),   textColour("white"), colour(105, 20, 20), outlineColour(155,100,100), trackerColour(220,160,160)
rslider  bounds(125, 20, 60, 60), text("Freq."),     channel("freq"),    range(0.1, 20000, 500, 0.5, 0.01), textColour("white"), colour(105, 20, 20), outlineColour(155,100,100), trackerColour(220,160,160)
rslider  bounds(180, 20, 60, 60), text("Spread"),    channel("spread"),  range(0, 1.00, 1),                 textColour("white"), colour(105, 20, 20), outlineColour(155,100,100), trackerColour(220,160,160)
rslider  bounds(235, 20, 60, 60), text("Tone Var."), channel("ToneVar"), range(0, 1.00, 0),                 textColour("white"), colour(105, 20, 20), outlineColour(155,100,100), trackerColour(220,160,160)
rslider  bounds(290, 20, 60, 60), text("Lowpass"),   channel("LPF"),     range(20,20000,20000,0.5),         textColour("white"), colour(105, 20, 20), outlineColour(155,100,100), trackerColour(220,160,160)
rslider  bounds(345, 20, 60, 60), text("Highpass"),  channel("HPF"),     range(20,20000,20,0.5),            textColour("white"), colour(105, 20, 20), outlineColour(155,100,100), trackerColour(220,160,160)
gentable bounds( 55, 90,350, 95), tableNumber(11), identChannel("table1"), ampRange(-1,1,-1), fill(1), outlineThickness(0), tableColour("yellow"), zoom(-1), tableGridColour(0,0,0,0)
image    bounds( 55,137,350,  1), colour("yellow"), alpha(0.3)
rslider  bounds(  5, 90, 45, 45), text("Scan"), channel("Scan"), range(0.25, 32, 16, 0.5, 0.01),                 textColour("white"), colour(105, 20, 20), outlineColour(155,100,100), trackerColour(220,160,160)
rslider  bounds(  5,135, 45, 45), text("Refresh"), channel("Refresh"), range(1, 64, 16),                 textColour("white"), colour(105, 20, 20), outlineColour(155,100,100), trackerColour(220,160,160)
</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-dm0 -n -+rtmidi=NULL -M0
</CsOptions>

<CsInstruments>

; sr set by host
ksmps    = 64
nchnls   = 2
0dbfs    = 1    ;MAXIMUM AMPLITUDE
massign  0, 0

i_ ftgen 11,0,1024,10,1
iCnt = 0
while iCnt<ftlen(11) do
tablew 0,iCnt,11
iCnt += 1
od
instr    1
konoff      chnget   "onoff"        ;read in on/off switch widget value
if konoff==0 goto SKIP        ;if on/off switch is off jump to 'SKIP' label
kamp        chnget   "amp"
kopcode      chnget  "opcode"
kfreq       chnget   "freq"
kspread     chnget   "spread"
if kopcode==2 then
asig        dust2    kamp, kfreq    ;GENERATE 'dust2' IMPULSES
else
asig        dust     kamp, kfreq    ;GENERATE 'dust' IMPULSES
endif

; tone variation
kToneVar    chnget   "ToneVar"
if(kToneVar>0) then
  kcfoct    random   14-(kToneVar*10),14
 asig       tonex    asig,cpsoct(kcfoct),1
endif

kpan        random   0.5-(kspread*0.5), 0.5+(kspread*0.5)
asigL,asigR pan2     asig,kpan

kporttime   linseg   0,0.001,0.05

; Lowpass Filter
kLPF        chnget   "LPF"
if kLPF<20000 then
 kLPF       portk    kLPF,kporttime
 asigL      clfilt   asigL,kLPF,0,2
 asigR      clfilt   asigR,kLPF,0,2
endif

; Highpass Filter
kHPF        chnget   "HPF"
if kHPF>20 then
 kHPF       limit    kHPF,20,kLPF
 kHPF       portk    kHPF,kporttime
 asigL      clfilt   asigL,kHPF,1,2
 asigR      clfilt   asigR,kHPF,1,2
endif

            outs     asigL,asigR    ; SEND AUDIO SIGNAL TO OUTPUT
aPtr    phasor   chnget:k("Scan")
tablew  asigL, aPtr, 11, 1
if metro:k(chnget:k("Refresh"))==1 then
chnset "tableNumber(11)", "table1"
endif

SKIP:                             ; A label. Skip to here is on/off switch is off 
endin

</CsInstruments>

<CsScore>
i 1 0 [60*60*24*7]
</CsScore>

</CsoundSynthesizer>