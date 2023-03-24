/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; CombFilters.csd
; Written by Iain McCurdy 2023

; A set of six parallel comb filters

<Cabbage>
form caption("Comb Filters") size(1200, 340), guiMode("queue"), pluginId("CbFi"), colour(50,50,50)
image bounds(0,0,1200, 60), colour(0,0,0,0), outlineThickness(2)
{
button  bounds( 20, 10, 70, 17), text("LIVE","LIVE"), channel("LiveInput"), latched(1), colour:1(70,70,30), colour:1(200,200,100), radioGroup(1), value(1)
button  bounds( 20, 30, 70, 17), text("TEST","TEST"), channel("TestInput"), latched(1), colour:1(70,70,30), colour:1(200,200,100), radioGroup(1), value(0)
hslider bounds(110, 20,280, 20), text("In Gain"), range(0,1,0), textColour("white"), channel("InGain"), valueTextBox(1)
button  bounds(420, 15, 70, 30), text("LP On","LP On"), colour:0(80,80,0), colour:1(200,200,0), channel("InLPOn"), latched(1)
hslider bounds(500, 20,280, 20), text("In LP"),   range(20,20000,20000,0.5), textColour("white"), channel("InLP"), valueTextBox(1)
button  bounds(820, 15, 70, 30), text("HP On","HP On"), colour:0(80,80,0), colour:1(200,200,0), channel("InHPOn"), latched(1)
hslider bounds(900, 20,280, 20), text("In HP"),   range(20,20000,20,0.5), textColour("white"), channel("InHP"), valueTextBox(1)
}

image bounds(0,100,1200,240), colour(0,0,0,0), outlineThickness(2)
{
hslider bounds(10, 10,280,20), text("FREQ"),   range(0.2,2,1,0.5), textColour("white"), channel("FREQ"), valueTextBox(1)
hslider bounds(10, 50,280,20), text("Freq.1"), range(50,4000,300,0.5), textColour("white"), channel("Freq1"), valueTextBox(1)
hslider bounds(10, 80,280,20), text("Freq.2"), range(50,4000,410,0.5), textColour("white"), channel("Freq2"), valueTextBox(1)
hslider bounds(10,110,280,20), text("Freq.3"), range(50,4000,133,0.5), textColour("white"), channel("Freq3"), valueTextBox(1)
hslider bounds(10,140,280,20), text("Freq.4"), range(50,4000,101,0.5), textColour("white"), channel("Freq4"), valueTextBox(1)
hslider bounds(10,170,280,20), text("Freq.5"), range(50,4000,237,0.5), textColour("white"), channel("Freq5"), valueTextBox(1)

hslider bounds(310, 10,280,20), text("RES"),   range(0,1,1,8), textColour("white"), channel("RES"), valueTextBox(1), trackerColour(100,100,200)
hslider bounds(310, 50,280,20), text("Res.1"), range(0,1,0.98,8), textColour("white"), channel("Res1"), valueTextBox(1), trackerColour(100,100,200)
hslider bounds(310, 80,280,20), text("Res.2"), range(0,1,0.98,8), textColour("white"), channel("Res2"), valueTextBox(1), trackerColour(100,100,200)
hslider bounds(310,110,280,20), text("Res.3"), range(0,1,0.98,8), textColour("white"), channel("Res3"), valueTextBox(1), trackerColour(100,100,200)
hslider bounds(310,140,280,20), text("Res.4"), range(0,1,0.98,8), textColour("white"), channel("Res4"), valueTextBox(1), trackerColour(100,100,200)
hslider bounds(310,170,280,20), text("Res.5"), range(0,1,0.98,8), textColour("white"), channel("Res5"), valueTextBox(1), trackerColour(100,100,200)

hslider bounds(610, 10,280,20), text("LP"),   range(0,1,1), textColour("white"), channel("LP"), valueTextBox(1), trackerColour(255,255,100)
hslider bounds(610, 50,280,20), text("LP 1"), range(0,18000,12000), textColour("white"), channel("LP1"), valueTextBox(1), trackerColour(255,255,100)
hslider bounds(610, 80,280,20), text("LP 2"), range(0,18000,12000), textColour("white"), channel("LP2"), valueTextBox(1), trackerColour(255,255,100)
hslider bounds(610,110,280,20), text("LP 3"), range(0,18000,12000), textColour("white"), channel("LP3"), valueTextBox(1), trackerColour(255,255,100)
hslider bounds(610,140,280,20), text("LP 4"), range(0,18000,12000), textColour("white"), channel("LP4"), valueTextBox(1), trackerColour(255,255,100)
hslider bounds(610,170,280,20), text("LP 5"), range(0,18000,12000), textColour("white"), channel("LP5"), valueTextBox(1), trackerColour(255,255,100)

hslider bounds(910, 10,280,20), text("GAIN"),   range(0,1,0.5), textColour("white"), channel("GAIN"), valueTextBox(1), trackerColour(0,255,255)
hslider bounds(910, 50,280,20), text("Gain 1"), range(0,1,1), textColour("white"), channel("Gain1"), valueTextBox(1), trackerColour(0,255,255)
hslider bounds(910, 80,280,20), text("Gain 2"), range(0,1,1), textColour("white"), channel("Gain2"), valueTextBox(1), trackerColour(0,255,255)
hslider bounds(910,110,280,20), text("Gain 3"), range(0,1,1), textColour("white"), channel("Gain3"), valueTextBox(1), trackerColour(0,255,255)
hslider bounds(910,140,280,20), text("Gain 4"), range(0,1,1), textColour("white"), channel("Gain4"), valueTextBox(1), trackerColour(0,255,255)
hslider bounds(910,170,280,20), text("Gain 5"), range(0,1,1), textColour("white"), channel("Gain5"), valueTextBox(1), trackerColour(0,255,255)

button bounds( 10,200,70,30),  text("Quantise","Quantise"), colour:0(80,80,0), colour:1(200,200,0), channel("Quantise"), l;atched(1)
hslider bounds(110,205,480,20), text("MIX"), range(0,1,1), textColour("white"), channel("MIX"), valueTextBox(1), trackerColour(200,100,100)
}
</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-n -d -+rtmidi=NULL -M0 --midi-key-cps=4 --midi-velocity-amp=5
</CsOptions>
<CsInstruments>
; Initialize the global variables. 
ksmps = 32
nchnls = 2
0dbfs = 1

instr 1
kFREQ     cabbageGetValue "FREQ"
kFreq1    cabbageGetValue "Freq1"
kFreq2    cabbageGetValue "Freq2"
kFreq3    cabbageGetValue "Freq3"
kFreq4    cabbageGetValue "Freq4"
kFreq5    cabbageGetValue "Freq5"

kRES      cabbageGetValue "RES"
kRes1     cabbageGetValue "Res1"
kRes2     cabbageGetValue "Res2"
kRes3     cabbageGetValue "Res3"
kRes4     cabbageGetValue "Res4"
kRes5     cabbageGetValue "Res5"

kLP       cabbageGetValue "LP"
kLP1      cabbageGetValue "LP1"
kLP2      cabbageGetValue "LP2"
kLP3      cabbageGetValue "LP3"
kLP4      cabbageGetValue "LP4"
kLP5      cabbageGetValue "LP5"

kGAIN       cabbageGetValue "GAIN"
kGain1      cabbageGetValue "Gain1"
kGain2      cabbageGetValue "Gain2"
kGain3      cabbageGetValue "Gain3"
kGain4      cabbageGetValue "Gain4"
kGain5      cabbageGetValue "Gain5"

kMIX       cabbageGetValue "MIX"

kInGain  cabbageGetValue  "InGain"
kInLPOn  cabbageGetValue  "InLPOn"
kInLP    cabbageGetValue  "InLP"
kInHPOn  cabbageGetValue  "InHPOn"
kInHP    cabbageGetValue  "InHP"

kQuantise     cabbageGetValue "Quantise"
if kQuantise==1 then
 kFreq1    = cpsmidinn(round(ftom(kFreq1)))
 kFreq2    = cpsmidinn(round(ftom(kFreq2)))
 kFreq3    = cpsmidinn(round(ftom(kFreq3)))
 kFreq4    = cpsmidinn(round(ftom(kFreq4)))
 kFreq5    = cpsmidinn(round(ftom(kFreq5)))
endif

kPortTime linseg 0,0.01,0.05

kFREQ     portk          kFREQ, kPortTime
kFreq1    portk          kFreq1, kPortTime
kFreq2    portk          kFreq2, kPortTime
kFreq3    portk          kFreq3, kPortTime
kFreq4    portk          kFreq4, kPortTime
kFreq5    portk          kFreq5, kPortTime

kLP     portk          kLP, kPortTime
kLP1    portk          kLP1, kPortTime
kLP2    portk          kLP2, kPortTime
kLP3    portk          kLP3, kPortTime
kLP4    portk          kLP4, kPortTime
kLP5    portk          kLP5, kPortTime

kGAIN     portk          kGAIN, kPortTime
kGain1    portk          kGain1, kPortTime
kGain2    portk          kGain2, kPortTime
kGain3    portk          kGain3, kPortTime
kGain4    portk          kGain4, kPortTime
kGain5    portk          kGain5, kPortTime

kMIX     portk          kMIX, kPortTime

kInGain   portk          kInGain, kPortTime
kInLP     portk          kInLP, kPortTime
kInHP     portk          kInHP, kPortTime

kLiveInput cabbageGetValue "LiveInput"
kTestInput cabbageGetValue "TestInput"
if kLiveInput==1 then
 aL,aR  ins
else
 aL  noise  0.1,0
 aR  noise  0.1,0
endif

aL  *=  kInGain
aR  *=  kInGain

if kInLPOn==1 then
 aL  butlp  aL, kInLP
 aR  butlp  aR, kInLP
endif

if kInHPOn==1 then
 aL  buthp  aL, kInHP
 aR  buthp  aR, kInHP
endif

a1L wguide1  aL, a(kFreq1*kFREQ), kLP1*kLP, kRes1*kRES
a2L wguide1  aL, a(kFreq2*kFREQ), kLP2*kLP, kRes2*kRES
a3L wguide1  aL, a(kFreq3*kFREQ), kLP3*kLP, kRes3*kRES
a4L wguide1  aL, a(kFreq4*kFREQ), kLP4*kLP, kRes4*kRES
a5L wguide1  aL, a(kFreq5*kFREQ), kLP5*kLP, kRes5*kRES

a1R wguide1  aR, a(kFreq1*kFREQ), kLP1*kLP, kRes1*kRES
a2R wguide1  aR, a(kFreq2*kFREQ), kLP2*kLP, kRes2*kRES
a3R wguide1  aR, a(kFreq3*kFREQ), kLP3*kLP, kRes3*kRES
a4R wguide1  aR, a(kFreq4*kFREQ), kLP4*kLP, kRes4*kRES
a5R wguide1  aR, a(kFreq5*kFREQ), kLP5*kLP, kRes5*kRES


aMixL   sum  a1L*a(kGain1), a2L*a(kGain2), a3L*a(kGain3), a4L*a(kGain4), a5L*a(kGain5) 
aMixR   sum  a1R*a(kGain1), a2R*a(kGain2), a3R*a(kGain3), a4R*a(kGain4), a5R*a(kGain5)

aMixL   ntrpol  aL,aMixL,kMIX
aMixR   ntrpol  aR,aMixR,kMIX

        outs     aMixL*a(kGAIN),aMixR*a(kGAIN)
endin

</CsInstruments>
<CsScore>
i 1 0 z
</CsScore>
</CsoundSynthesizer>
