
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; FFTSpectrum.csd
; written by Iain McCurdy 2014, updated 2016
; FFT spectrum display

; Choose between live input and a range of synthesised inputs.

; Choose from a range of filters.
;  Lowpass and highpass are 12dB/oct butterworth filters.
;  Resonant lowpass is the moogladder filter.

; 'FFT gain' applies amplitude gain before FFT analysis, therefore resulting in y-axis zoom.
; 'Shift' shifts all frequencies equally up the x-axis (using pvshift)
; 'Zoom' zooms in on the x axis
; 'Smooth' applies smoothing to changes of amplitude which can be helpful in stabilising flickering spectrum representations.

<Cabbage>
form caption("FFT Spectrum"), size(610,680), colour( 50, 50, 50), pluginId("spec"), guiRefresh(16)
gentable outlineThickness(1), bounds( 0,  0, 610,300), tableNumber(1), tablebackgroundColour("white"), tableGridColour(100,100,100,50), tableColour(0,0,200,200), identChannel("ampFFT"), ampRange(0,1,-1), outlineThickness(0), sampleRange(0, 512) 

image bounds(0,305,600,185), plant("source"), colour(0,0,0,0)
{
checkbox bounds(  5,  0,100,13), channel("Live"), text("Live"), radioGroup(3), value(1)
checkbox bounds(  5, 18,100,13), channel("White"), text("White Noise"), radioGroup(3)
checkbox bounds(  5, 36,100,13), channel("Pink"), text("Pink Noise"), radioGroup(3)
checkbox bounds(  5, 54,100,13), channel("Sawtooth"), text("Sawtooth"), radioGroup(3)
checkbox bounds(  5, 72,100,13), channel("Square"), text("Square"), radioGroup(3)
checkbox bounds(  5, 90,100,13), channel("Pulse"), text("Pulse"), radioGroup(3)
checkbox bounds(  5,108,100,13), channel("Triangle"), text("Triangle"), radioGroup(3)
checkbox bounds(  5,126,100,13), channel("Buzz"), text("GBuzz"), radioGroup(3)
checkbox bounds(  5,144,100,13), channel("FM"), text("FM"), radioGroup(3)
checkbox bounds(  5,164,100,13), channel("hsboscil"), text("hsboscil"), radioGroup(3)

rslider bounds(105, 50,90, 90), channel("OscillatorFreq"), text("Osc. Freq."), textBox(1), valueTextBox(1), range(0,11025,300,0.5,1), visible(0), identChannel("OscFreqID")
rslider bounds(195, 50,90, 90), channel("PW"), text("Pulse Width"), textBox(1), valueTextBox(1), range(0.005,0.995,0.005,1,0.001), visible(0), identChannel("PWID")
rslider bounds(195, 50,90, 90), channel("NH"), text("Num. Harms."), textBox(1), valueTextBox(1), range(1,160,40,1,1), visible(0), identChannel("Buzz1ID")
rslider bounds(285, 50,90, 90), channel("LH"), text("Lowest Harm."), textBox(1), valueTextBox(1), range(1,20,1,1,1), visible(0), identChannel("Buzz2ID")
rslider bounds(375, 50,90, 90), channel("Mul"), text("Power Mult."), textBox(1), valueTextBox(1), range(0,2,1,1,0.001), visible(0), identChannel("Buzz3ID")
rslider bounds(465, 50,90, 90), channel("AudioGain"), text("Audio Gain"), textBox(1), valueTextBox(1), range(0,5.00,0.0,0.5)
rslider bounds(195, 50,90, 90), channel("ModFreq"), text("Mod. Freq."), textBox(1), valueTextBox(1), range(0,3000,300,0.4,0.1), visible(0), identChannel("FM1ID")
rslider bounds(285, 50,90, 90), channel("Index"), text("Index"), textBox(1), valueTextBox(1), range(0,20,0), visible(0), identChannel("FM2ID")
combobox bounds(375, 60, 80, 20), channel("FM_PM"), text("Freq. Mod.","Phase Mod."), value(1), identChannel("FM3ID"), visible(0)
rslider bounds(185, 50,90, 90), channel("HsbBrite"), text("Brite"), textBox(1), valueTextBox(1), range(-16,16,0), visible(0), identChannel("hsboscil1ID")
rslider bounds(265, 50,90, 90), channel("HsbTone"), text("Tone"), textBox(1), valueTextBox(1), range(-1,1,0), visible(0), identChannel("hsboscil2ID")
rslider bounds(345, 50,90, 90), channel("HsbRange"), text("Octave Count"), textBox(1), valueTextBox(1), range(2,12,2,1,1), visible(0), identChannel("hsboscil3ID")
}

image    bounds(100,300,445, 60), plant("display"), colour(0,0,0,0)
{
hslider  bounds(  0,  0,350, 30), channel("gain"), text("Y Zoom"), textBox(1), valueTextBox(1), range(0,99.00,6,0.5,0.01)
hslider  bounds(  0, 30,350, 30), channel("smooth"), text("Smooth"), textBox(1), valueTextBox(1), range(0,5,0.5)
label    bounds(365, 10, 80, 14), text("FFT Size")  
combobox bounds(365, 25, 80, 20), channel("FFTSize"), text("512","1024","2048","4096","8192"), value(2)
}

image    bounds(  5,490,600,185), plant("filter"), colour(0,0,0,0)
{
line     bounds(5,  0,600,1), colour("silver")

checkbox bounds(  0, 10,100,13), channel("Bypass"), text("Bypass"), radioGroup(1), value(1)
checkbox bounds(  0, 30,100,13), channel("LPF"), text("Lowpass"), radioGroup(1)
checkbox bounds(  0, 50,100,13), channel("HPF"), text("Highpass"), radioGroup(1)
checkbox bounds(  0, 70,100,13), channel("BPF"), text("Bandpass"), radioGroup(1)
checkbox bounds(  0, 90,100,13), channel("BRF"), text("Bandreject"), radioGroup(1)
checkbox bounds(  0,110,100,13), channel("LPFRes"), text("Lowpass Res."), radioGroup(1)
checkbox bounds(  0,130,100,13), channel("Phaser"), text("Phaser"), radioGroup(1)
checkbox bounds(  0,150,100,13), channel("Comb"), text("Comb"), radioGroup(1)
checkbox bounds(  0,170,100,13), channel("Resony"), text("Resony"), radioGroup(1)

rslider  bounds(100, 65, 90, 90), channel("cf"), text("Freq."), textBox(1), valueTextBox(1), range(1,20000,1000,0.5,1), identChannel("CFID"), visible(0)
rslider  bounds(180, 65, 90, 90), channel("res"), text("Res./F.back"), textBox(1), valueTextBox(1), range(-0.99,0.99,0.5,1,0.01), identChannel("ResID"), visible(0)
rslider  bounds(260, 65, 90, 90), channel("ord"), text("Ordinates"), textBox(1), valueTextBox(1), range(1,80,8,1,1), identChannel("Phs1ID"), visible(0)
rslider  bounds(340, 65, 90, 90), channel("Q"), text("Q"), textBox(1), valueTextBox(1), range(0.1,10,1), identChannel("Phs2ID"), visible(0)
rslider  bounds(420, 65, 90, 90), channel("sep"), text("Separation"), textBox(1), valueTextBox(1), range(-3,3,1), identChannel("Phs3ID"), visible(0)

; combobox bounds(180,100, 80,20), channel("LPFType"), items("tone, butlp"), value(1), identChannel("LPFTypeID"), visible(1)

checkbox bounds(510,120,100,15), channel("Mode1"), text("Mode 1"), radioGroup(2), value(1), identChannel("Phs4ID"), visible(0)
checkbox bounds(510,140,100,15), channel("Mode2"), text("Mode 2"), radioGroup(2), identChannel("Phs5ID"), visible(0)

rslider  bounds(260, 65, 90, 90), channel("ResonySep"), text("Sep."), textBox(1), valueTextBox(1), range(0.1,12,5), identChannel("Resony1ID"), visible(0)
rslider  bounds(340, 65, 90, 90), channel("ResonyNum"), text("Num."), textBox(1), valueTextBox(1), range(1,40,8,1,1), identChannel("Resony2ID"), visible(0)
label    bounds(425, 65,90,11), text("Separation Mode"), identChannel("Resony3ID"), visible(0)
combobox bounds(425, 80,90,20), channel("ResonyMode"), text("Octaves","Harmonics"), value(1), identChannel("Resony4ID"), visible(0)
}

label bounds(506,319,60,13), text(""), align("left"), identChannel("MOUSEFreqID"), visible(1), fontColour("black"), colour(255,255,255,200)
</Cabbage>                                                   

<CsoundSynthesizer>                                                                                                 

<CsOptions>                                                     
-dm0 -n
</CsOptions>
                                  
<CsInstruments>

; sr set by host
ksmps  =  32
nchnls =  2
0dbfs  =  1

gicos           ftgen       0,0,131072,11,1
giFFTSizes[]    fillarray   512,1024,2048,4096,8192
gisine          ftgen       0,0,4096,10,1
giGauss         ftgen       0, 0, 1024, 20, 6, 1, 1.343
giHanning       ftgen       0, 0, 1024, 20, 2, 1

instr  1
 kFFTIndex        chnget  "FFTSize"
 kFFTSize         =       giFFTSizes[kFFTIndex-1]
 kFFTSize         init    1024
 kporttime        linseg  0,0.01,0.05
 kcf              chnget  "cf"
 kcf              portk   kcf,kporttime
 kres             chnget  "res"
 kord             chnget  "ord"
 kQ               chnget  "Q"
 ksep             chnget  "sep"
 ksep             portk   ksep, kporttime
 kMode1           chnget  "Mode1"
 kMode2           chnget  "Mode2"
 kMode            =       kMode1 + (kMode2*2)
 kMode            init    1
 kBypass          chnget  "Bypass"
 kLPF             chnget  "LPF"
 kHPF             chnget  "HPF"
 kBPF             chnget  "BPF"
 kBRF             chnget  "BRF"
 kLPFRes          chnget  "LPFRes"
 kPhaser          chnget  "Phaser"
 kComb            chnget  "Comb"
 kResony          chnget  "Resony"
 kLive            chnget  "Live"
 kWhite           chnget  "White"
 kPink            chnget  "Pink"
 kSawtooth        chnget  "Sawtooth"
 kSquare          chnget  "Square"
 kPulse           chnget  "Pulse"
 kTriangle        chnget  "Triangle"
 kBuzz            chnget  "Buzz"
 kFM              chnget  "FM"
 khsboscil        chnget  "hsboscil"
 kHsbBrite        chnget  "HsbBrite"
 kHsbTone         chnget  "HsbTone"
 kHsbRange        chnget  "HsbRange"
 kOscillatorFreq  chnget  "OscillatorFreq"
 kNH              chnget  "NH"
 kLH              chnget  "LH"
 kMul             chnget  "Mul"
 kMul             portk   kMul, kporttime
 kPW              chnget  "PW"
 kPW              portk   kPW, kporttime
 kSmooth          chnget  "smooth"
 kAudioGain       chnget  "AudioGain"
 kAudioGain       portk  kAudioGain,kporttime
 
 if kLive==1 then
  aSig  inch  1
 elseif kWhite==1 then
  aSig  noise  1,0
 elseif kPink==1 then
  aSig  pinkish  1
 elseif kSawtooth==1 then
  aSig  vco2  0.4,kOscillatorFreq
 elseif kSquare==1 then
  aSig  vco2  0.4,kOscillatorFreq,2,0.5
 elseif kPulse==1 then
  aSig  vco2  3,kOscillatorFreq,2,kPW
  aSig  dcblock aSig
 elseif kTriangle==1 then
  aSig  vco2  3,kOscillatorFreq,4,0.5
 elseif kBuzz==1 then
  aSig  gbuzz  3,kOscillatorFreq,kNH,kLH,kMul,gicos
 elseif kFM==1 then
  kModFreq chnget  "ModFreq"
  kIndex chnget  "Index"
  kFM_PM chnget  "FM_PM"
  if kFM_PM==1 then 
   ;aSig  foscili  1, kOscillatorFreq, 1, kModFreq/kOscillatorFreq, kIndex, gisine
   aMod  poscil  kOscillatorFreq * kIndex, kModFreq
   aSig  poscil  1, kOscillatorFreq + aMod
  else
   aMod  poscil  kIndex, kModFreq, gisine
   aCar  phasor  kOscillatorFreq + aMod * kModFreq
   aSig  tablei  aCar, gisine, 1, 0, 1
  endif
 elseif khsboscil==1 then
   ibasfreq = 100
   ioctcnt = 2
   iphs = 0
   kHsbBrite portk kHsbBrite, kporttime
   if changed:k(kOscillatorFreq,kHsbRange)==1 then
    reinit REINIT_hsboscil
   endif
   REINIT_hsboscil:
   kHsbBrite init i(kHsbBrite)
   aSig  hsboscil 1, kHsbTone, kHsbBrite, i(kOscillatorFreq), gisine, giHanning, i(kHsbRange), -1
   rireturn
 endif
  
 ; Hide osc. freq control if not an oscillator
 if changed(kLive,kWhite,kPink)==1 then
  if kLive+kWhite+kPink == 0 then
   chnset  "visible(1)","OscFreqID"
  else
   chnset  "visible(0)","OscFreqID"  
  endif
 endif

 ; Hide buzz controls if not buzz
 if changed(kBuzz)==1 then
  if kBuzz == 1 then
   chnset  "visible(1)","Buzz1ID"
   chnset  "visible(1)","Buzz2ID"
   chnset  "visible(1)","Buzz3ID"
  else
   chnset  "visible(0)","Buzz1ID"
   chnset  "visible(0)","Buzz2ID"
   chnset  "visible(0)","Buzz3ID"
  endif
 endif

 ; Hide PWM controls if not buzz
 if changed(kPulse)==1 then
  if kPulse == 1 then
   chnset  "visible(1)","PWID"
  else
   chnset  "visible(0)","PWID"
  endif
 endif

 ; Hide FM controls if not FM
 if changed(kFM)==1 then
  if kFM == 1 then
   chnset  "visible(1)","FM1ID"
   chnset  "visible(1)","FM2ID"
   chnset  "visible(1)","FM3ID"
  else
   chnset  "visible(0)","FM1ID"
   chnset  "visible(0)","FM2ID"
   chnset  "visible(0)","FM3ID"
  endif
 endif
 
 ; Hide hsboscil controls if not hsboscil
 if changed(khsboscil)==1 then
  if khsboscil == 1 then
   chnset  "visible(1)","hsboscil1ID"
   chnset  "visible(1)","hsboscil2ID"
   chnset  "visible(1)","hsboscil3ID"
  else
   chnset  "visible(0)","hsboscil1ID"
   chnset  "visible(0)","hsboscil2ID"
   chnset  "visible(0)","hsboscil3ID"
  endif
 endif
 
 
 acf  interp  kcf
 if changed(kLPF,kHPF,kBPF,kBRF,kLPFRes,kPhaser,kComb,kResony)==1 then
  reinit REINIT_FILTER
 endif
 REINIT_FILTER:
 if i(kLPF)==1 then
  aSig  butlp  aSig,acf
  aSig  butlp  aSig,acf
  aSig  butlp  aSig,acf
  aSig  butlp  aSig,acf
 elseif i(kHPF)==1 then
  aSig  buthp  aSig,acf
  aSig  buthp  aSig,acf
  aSig  buthp  aSig,acf
  aSig  buthp  aSig,acf
 elseif i(kBPF)==1 then
  aSig  butbp  aSig,acf,kcf*abs(1-kres)
 elseif i(kBRF)==1 then
  aSig  butbr  aSig,acf,kcf*abs(1-kres)
 elseif i(kLPFRes)==1 then 
   aSig  moogladder  aSig,kcf,abs(kres)
 elseif i(kPhaser)==1 then 
  if changed(kord,kMode)==1 then 
   reinit UPDATE
  endif
  UPDATE:
   aSig  phaser2  aSig, kcf, kQ, kord, kMode, ksep, kres
  rireturn
 elseif i(kComb)==1 then
  ;kcf =  kres < 0 ? kcf*2 : kcf 
  aSig  streson  aSig,kcf,kres
  a2  streson  aSig,kcf*2,-kres
  aSig  +=      a2
  ;aSig  dcblock    aSig
 elseif i(kResony)==1 then
  kResonySep  chnget    "ResonySep"
  iscl  =  2
  kbw  limit  kres*100,0.001,99
  kResonyNum    chnget  "ResonyNum"
  kResonyMode    chnget  "ResonyMode"
  kResonyMode    init  1
  if changed:k(kResonyNum,kResonyMode)==1 then
   reinit ReinitResony
  endif
  ReinitResony:
  aSig  resony  aSig/10,kcf,kbw,i(kResonyNum),(kResonySep/12)*kResonyNum,i(kResonyMode)-1,iscl
 endif
 rireturn
 
 if changed(kFFTSize)==1 then
  reinit  RESTART_FFT
 endif
 RESTART_FFT:
 iTabSize  =  i(kFFTSize)
 
 iampFFT    ftgen  1,0,iTabSize,2,0
  
 /* FFT ATTRIBUTES */
 ifftsize  =  iTabSize*2
 ioverlap  =  ifftsize/4
 iwinsize  =  ifftsize
 iwintype  =  1

 kgain  chnget  "gain"
 
 fSig  pvsanal  aSig*kgain*2, ifftsize, ioverlap, iwinsize, iwintype
 fSig2  pvsblur fSig,kSmooth,5

 if  metro(16)==1 then
  kflag  pvsftw fSig2, iampFFT
 endif
 if kflag==1 then
    chnset  "tableNumber(1)", "ampFFT"
 endif
  
 aAudioGain  interp  kAudioGain
     outs  aSig*aAudioGain, aSig*aAudioGain
     
 if changed(kResony)==1 then
  if kResony==1 then  
   chnset  "visible(1)","Resony1ID"
   chnset  "visible(1)","Resony2ID"
   chnset  "visible(1)","Resony3ID"
   chnset  "visible(1)","Resony4ID"
  else
   chnset  "visible(0)","Resony1ID"
   chnset  "visible(0)","Resony2ID"
   chnset  "visible(0)","Resony3ID"
   chnset  "visible(0)","Resony4ID"
  endif
 endif

  if changed(kPhaser)==1 then
  if kPhaser==1 then  
   chnset  "visible(1)","Phs1ID"
   chnset  "visible(1)","Phs2ID"
   chnset  "visible(1)","Phs3ID"
   chnset  "visible(1)","Phs4ID"
   chnset  "visible(1)","Phs5ID"
  else
   chnset  "visible(0)","Phs1ID"
   chnset  "visible(0)","Phs2ID"
   chnset  "visible(0)","Phs3ID"
   chnset  "visible(0)","Phs4ID"
   chnset  "visible(0)","Phs5ID"  
  endif
 endif
 
 if changed(kLPF,kHPF,kBypass)==1 then
  if kLPF==1||kHPF==1||kBypass==1 then  
   chnset  "visible(0)","ResID"
  else
   chnset  "visible(1)","ResID"
  endif
 endif

 if changed(kBypass)==1 then
  if kBypass==1 then  
   chnset  "visible(0)","CFID"
  else
   chnset  "visible(1)","CFID"
  endif
 endif

 ; Frequency pop-up
 kMOUSE_X  chnget  "MOUSE_X"
 kMOUSE_Y  chnget  "MOUSE_Y"
 if changed(kMOUSE_X,kMOUSE_Y)==1 then
  if kMOUSE_X>1 && kMOUSE_X<609 && kMOUSE_Y>1 && kMOUSE_Y<299 then
   kMOUSEFreq  =  (kMOUSE_X/610) * (11025 * (1024/kFFTSize))
   Spos  sprintfk  "pos(%d,%d), text(%d Hz), visible(1)",limit:k(kMOUSE_X,0,553),kMOUSE_Y-10,kMOUSEFreq
      chnset    Spos,"MOUSEFreqID"
  else
       chnset    "visible(0)","MOUSEFreqID"
  endif
 endif
  
endin                                                                                                                     

</CsInstruments>
                              
<CsScore>
i 1 0 [3600*24*7]
e
</CsScore>                            

</CsoundSynthesizer>
