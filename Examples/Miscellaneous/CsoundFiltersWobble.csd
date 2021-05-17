<Cabbage>
form caption("Csound Filters") size(400, 220), pluginId("CsFW")
keyboard bounds(12, 84, 381, 95), value(20)
combobox bounds(12, 8, 381, 30), align("centre"), channel("filerTypeCombo"), items("MOOG_LADDER", "MOOG_VCF   ", "LPF18      ", "BQREZ      ", "CLFILT     ", "BUTTERLP   ", "LOWRES     ", "REZZY      ", "SVFILTER   ", "VLOWRES    ", "STATEVAR   ", "MVCLPF1    ", "MVCLPF2    ", "MVCLPF3    ")
hslider bounds(12, 40, 361, 39), channel("coeffSlider"), range(0.01, 2, .5), text("LFO Rate")
</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-n -d -+rtmidi=NULL -M0 --midi-key-cps=4
</CsOptions>
<CsInstruments>
;sr is set by the host
ksmps = 32
nchnls = 2
0dbfs = 1

gaOut init 0
giSpb   init 0.45


; Filter types

#define MOOG_LADDER #1#
#define MOOG_VCF    #2# 
#define LPF18       #3#
#define BQREZ       #4#
#define CLFILT      #5#
#define BUTTERLP    #6#
#define LOWRES      #7#
#define REZZY       #8#
#define SVFILTER    #9# 
#define VLOWRES     #10#
#define STATEVAR    #11#
#define MVCLPF1     #12#
#define MVCLPF2     #13#
#define MVCLPF3     #14#




opcode MultiFilter, a, akki
ain, kcfq, kres, iType xin

kType init iType
if kType == $MOOG_LADDER then
    aout    moogladder ain, kcfq, kres
elseif kType == $MOOG_VCF then
    aout    moogvcf ain, kcfq, kres    
elseif kType == $LPF18 then
    aout    lpf18 ain, kcfq, kres, 0.5
elseif kType == $BQREZ then
    aout    bqrez ain, kcfq, 99 * kres + 1
elseif kType == $CLFILT then
    aout    clfilt ain, kcfq, 0, 2
elseif kType == $BUTTERLP then
    aout    butterlp ain, kcfq
elseif kType == $LOWRES then
    aout    lowres ain, kcfq, kres
elseif kType == $REZZY then
   aout     rezzy ain, kcfq, kres
elseif kType == $SVFILTER then
  aout, ahigh, aband  svfilter ain, kcfq, (499 / 10) * kres + 1 ; reScales to make it musical
elseif kType == $VLOWRES then
    aout    vlowres ain, kcfq, kres, 2, 0
elseif kType == $STATEVAR then
    ahp, aout, abp, abr     statevar ain, kcfq, kres
elseif kType == $MVCLPF1 then
    aout mvclpf1 ain, kcfq, kres
elseif kType == $MVCLPF2 then
    aout mvclpf2 ain, kcfq, kres
elseif kType == $MVCLPF3 then
    aout mvclpf3 ain, kcfq, kres
else
    aout = 0
endif
    xout aout
endop

#define MOOG_LADDER #1#
#define MOOG_VCF    #2# 
#define LPF18       #3#
#define BQREZ       #4#
#define CLFILT      #5#
#define BUTTERLP    #6#
#define LOWRES      #7#
#define REZZY       #8#
#define SVFILTER    #9# 
#define VLOWRES     #10#
#define STATEVAR    #11#
#define MVCLPF1     #12#
#define MVCLPF2     #13#
#define MVCLPF3     #14#


opcode Wave, a, k
kcps    xin

asqr    vco2 1, kcps * 0.495, 10      ; square
asaw    vco2 1, kcps * 1.005, 0       ; wave
        xout    0.5 * (asqr + asaw)
endop


opcode Filter, a, aiii
ain, iFilterType, iCoeff, iCps  xin

iDivision = 1 / (iCoeff * giSpb)
kLfo    loopseg iDivision, 0, 0, 0, 0.5, 1, 0.5, 0, 0
iBase   = iCps
iMod    = iBase * 9

kcfq    = iBase + iMod * kLfo
kres    init 0.6

aout    MultiFilter ain,   kcfq, kres, iFilterType
aout    balance aout, ain

        xout aout
endop

opcode Reverb, aa, aaii
adryL, adryR, ifeedback, imix xin
awetL, awetR reverbsc adryL, adryR, ifeedback, 10000

aoutL  = (1 - imix) * adryL  + imix * awetL
aoutR  = (1 - imix) * adryR  + imix * awetR

       xout aoutL, aoutR
endop

;bass instrument, gets triggered each time a note is pressed
instr 1
    iCoeff      chnget "coeffSlider"
    iCps        = p4    
    iFilterType chnget "filerTypeCombo"
    
    aWave   Wave iCps
    aOut    Filter aWave, iFilterType, iCoeff, iCps       

	aEnv madsr 0.01, 0, 1, 0.01

    gaOut   = (gaOut + aOut)*aEnv
    
endin


instr Main
iVolume = 0.5
iReverbFeedback = 0.3
iMixLevel       = p4

aoutL, aoutR Reverb gaOut, gaOut, iReverbFeedback, iMixLevel
outs (iVolume * aoutL), (iVolume * aoutR)

gaOut = 0
endin

</CsInstruments>

<CsScore>
f0 z
i"Main" 0 36000 0.35
; the fourth parameter is a reverb mix level
;i "PlayAll" 0 0 0.35

; uncomment to save output to wav files
;i "DumpAll" 0 0 0.35
</CsScore>

</CsoundSynthesizer>
