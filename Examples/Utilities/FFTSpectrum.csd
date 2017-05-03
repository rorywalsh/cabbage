; Spectrum.csd
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
form caption("FFT Spectrum"), size(610,620), colour( 50, 50, 50), pluginID("spec"), guirefresh(16)
gentable bounds(0, 0, 610, 300) tablenumber(1) tablebackgroundcolour(255, 255, 255, 255) tablegridcolour(245, 245, 245, 255) tablecolour("black"), identchannel("ampFFT") amprange(0, 1, -1, 0.0100) outlinethickness(0) samplerange(0, 512) tablecolour:0(0,0,0, 255)

hslider bounds(105,300,250, 30), channel("gain"), text("Y Zoom"), textBox(1), range(0,99.00,6,0.5,0.01)
hslider bounds(105,330,250, 30), channel("smooth"), text("Smooth"), textBox(1), range(0,5,0.5)

label	bounds(0,0,60,13), text(""), align("left"), identchannel("MOUSEFreqID"), visible(1), fontcolour("black"), colour(255,255,255,200)

label    bounds(370,310,80,14), text("FFT Size")  
combobox bounds(370,325,80,20), channel("FFTSize"), text("512","1024","2048","4096","8192"), value(2)

checkbox bounds(  5,305,100,15), channel("Bypass"), text("Bypass"), radiogroup(1), value(1)
checkbox bounds(  5,325,100,15), channel("LPF"), text("Lowpass"), radiogroup(1)
checkbox bounds(  5,345,100,15), channel("HPF"), text("Highpass"), radiogroup(1)
checkbox bounds(  5,365,100,15), channel("BPF"), text("Bandpass"), radiogroup(1)
checkbox bounds(  5,385,100,15), channel("BRF"), text("Bandreject"), radiogroup(1)
checkbox bounds(  5,405,100,15), channel("LPFRes"), text("Lowpass Res."), radiogroup(1)
checkbox bounds(  5,425,100,15), channel("Phaser"), text("Phaser"), radiogroup(1)
checkbox bounds(  5,445,100,15), channel("Comb"), text("Comb"), radiogroup(1)

rslider bounds(105,360, 90, 90), channel("cf"), text("Freq."), textBox(1), range(1,20000,1000,0.5,1), identchannel("CFID"), visible(0)
rslider bounds(185,360, 90, 90), channel("res"), text("Res./F.back"), textBox(1), range(-0.99,0.99,0.5,1,0.01), identchannel("ResID"), visible(0)
rslider bounds(265,360, 90, 90), channel("ord"), text("Ordinates"), textBox(1), range(1,80,8,1,1), identchannel("Phs1ID"), visible(0)
rslider bounds(345,360, 90, 90), channel("Q"), text("Q"), textBox(1), range(0.1,10,1), identchannel("Phs2ID"), visible(0)
rslider bounds(425,360, 90, 90), channel("sep"), text("Separation"), textBox(1), range(-3,3,1), identchannel("Phs3ID"), visible(0)


checkbox bounds(515,415,100,15), channel("Mode1"), text("Mode 1"), radiogroup(2), value(1), identchannel("Phs4ID"), visible(0)
checkbox bounds(515,435,100,15), channel("Mode2"), text("Mode 2"), radiogroup(2), identchannel("Phs5ID"), visible(0)
                                 
line bounds(5,470,600,1), colour("silver")

rslider bounds(105,480,90, 90), channel("OscillatorFreq"), text("Osc. Freq."), textBox(1), range(1,11025,300,0.5,1), visible(0), identchannel("OscFreqID")
rslider bounds(195,480,90, 90), channel("PW"), text("Pulse Width"), textBox(1), range(0.005,0.995,0.005,1,0.001), visible(0), identchannel("PWID")
rslider bounds(195,480,90, 90), channel("NH"), text("Num. Harms."), textBox(1), range(1,80,40,1,1), visible(0), identchannel("Buzz1ID")
rslider bounds(285,480,90, 90), channel("LH"), text("Lowest Harm."), textBox(1), range(1,20,1,1,1), visible(0), identchannel("Buzz2ID")
rslider bounds(375,480,90, 90), channel("Mul"), text("Power Mult."), textBox(1), range(0,2,1,1,0.001), visible(0), identchannel("Buzz3ID")
rslider bounds(465,480,90, 90), channel("AudioGain"), text("Audio Gain"), textBox(1), range(0,5.00,0.1,0.5)

checkbox bounds(  5,480,100,15), channel("Live"), text("Live"), radiogroup(3), value(1)
checkbox bounds(  5,500,100,15), channel("White"), text("White Noise"), radiogroup(3)
checkbox bounds(  5,520,100,15), channel("Pink"), text("Pink Noise"), radiogroup(3)
checkbox bounds(  5,540,100,15), channel("Sawtooth"), text("Sawtooth"), radiogroup(3)
checkbox bounds(  5,560,100,15), channel("Square"), text("Square"), radiogroup(3)
checkbox bounds(  5,580,100,15), channel("Pulse"), text("Pulse"), radiogroup(3)
checkbox bounds(  5,600,100,15), channel("Buzz"), text("Buzz"), radiogroup(3)
</Cabbage>                                                   

<CsoundSynthesizer>                                                                                                 

<CsOptions>                                                     
-dm0 -n
</CsOptions>
                                  
<CsInstruments>

sr 		= 	44100
ksmps 	= 	32
nchnls 	= 	2
0dbfs	=	1

gicos			ftgen		0,0,131072,11,1
giFFTSizes[]	fillarray	512,1024,2048,4096,8192

instr	1
 kFFTIndex	chnget	"FFTSize"
 kFFTSize	=		giFFTSizes[kFFTIndex-1]
 kFFTSize	init	1024
 kporttime	linseg	0,0.01,0.05
 kcf	chnget	"cf"
 kcf	portk	kcf,kporttime
 kres	chnget	"res"
 kord	chnget	"ord"
 kQ		chnget	"Q"
 ksep	chnget	"sep"
 kMode1	chnget	"Mode1"
 kMode2	chnget	"Mode2"
 kMode	=		kMode1 + (kMode2*2)
 kMode	init	1
 kBypass	chnget	"Bypass"
 kLPF	chnget	"LPF"
 kHPF	chnget	"HPF"
 kBPF	chnget	"BPF"
 kBRF	chnget	"BRF"
 kLPFRes	chnget	"LPFRes"
 kPhaser	chnget	"Phaser"
 kComb	chnget	"Comb"
 kLive	chnget	"Live"
 kWhite	chnget	"White"
 kPink	chnget	"Pink"
 kSawtooth	chnget	"Sawtooth"
 kSquare	chnget	"Square"
 kPulse	chnget	"Pulse"
 kBuzz	chnget	"Buzz"
 kOscillatorFreq	chnget	"OscillatorFreq"
 kNH	chnget	"NH"
 kLH	chnget	"LH"
 kMul	chnget	"Mul"
 kPW	chnget	"PW"
 kOscillatorFreq	portk	kOscillatorFreq, kporttime
 
 kSmooth	chnget	"smooth"
 kAudioGain	chnget	"AudioGain"
 kAudioGain	portk	kAudioGain,kporttime
 
 if kLive==1 then
  aSig	inch	1
 elseif kWhite==1 then
  aSig	noise	1,0
 elseif kPink==1 then
  aSig	pinkish	1
 elseif kSawtooth==1 then
  aSig	vco2	0.4,kOscillatorFreq
 elseif kSquare==1 then
  aSig	vco2	0.4,kOscillatorFreq,2,0.5
 elseif kPulse==1 then
  aSig	vco2	3,kOscillatorFreq,2,kPW
 elseif kBuzz==1 then
  aSig	gbuzz	3,kOscillatorFreq,kNH,kLH,kMul,gicos
 endif
 
 ; Hide osc. freq control of not an oscillator
 if changed(kLive,kWhite,kPink)==1 then
  if kLive+kWhite+kPink == 0 then
   chnset	"visible(1)","OscFreqID"
  else
   chnset	"visible(0)","OscFreqID"  
  endif
 endif

 ; Hide buzz controls if not buzz
 if changed(kBuzz)==1 then
  if kBuzz == 1 then
   chnset	"visible(1)","Buzz1ID"
   chnset	"visible(1)","Buzz2ID"
   chnset	"visible(1)","Buzz3ID"
  else
   chnset	"visible(0)","Buzz1ID"
   chnset	"visible(0)","Buzz2ID"
   chnset	"visible(0)","Buzz3ID"
  endif
 endif

 ; Hide buzz controls if not buzz
 if changed(kPulse)==1 then
  if kPulse == 1 then
   chnset	"visible(1)","PWID"
  else
   chnset	"visible(0)","PWID"
  endif
 endif

 acf	interp	kcf
 if changed(kLPF,kHPF,kBPF,kBRF,kLPFRes,kPhaser,kComb)==1 then
  reinit REINIT_FILTER
 endif
 REINIT_FILTER:
 if i(kLPF)==1 then
  aSig	butlp	aSig,acf
  aSig	butlp	aSig,acf
  aSig	butlp	aSig,acf
  aSig	butlp	aSig,acf
 elseif i(kHPF)==1 then
  aSig	buthp	aSig,acf
  aSig	buthp	aSig,acf
  aSig	buthp	aSig,acf
  aSig	buthp	aSig,acf
 elseif i(kBPF)==1 then
  aSig	butbp	aSig,acf,kcf*abs(1-kres)
 elseif i(kBRF)==1 then
  aSig	butbr	aSig,acf,kcf*abs(1-kres)
 elseif i(kLPFRes)==1 then 
   aSig	moogladder	aSig,kcf,abs(kres)
 elseif i(kPhaser)==1 then 
  if changed(kord,kMode)==1 then 
   reinit UPDATE
  endif
  UPDATE:
   aSig	phaser2	aSig, kcf, kQ, kord, kMode, ksep, kres
  rireturn
 elseif i(kComb)==1 then 
  aSig	streson	aSig,kcf,kres
;  a_	delayr	1
;  aSig2	deltap3	a(1/kcf)
;  		delayw	aSig+aSig2*kres
;  aSig	+=		aSig2
 endif

 if changed(kFFTSize)==1 then
  reinit	RESTART_FFT
 endif
 RESTART_FFT:
 iTabSize	=	i(kFFTSize)
 print iTabSize
 
 iampFFT		ftgen	1,0,iTabSize,2,0
  
 /* FFT ATTRIBUTES */
 ifftsize	=	iTabSize*2
 ioverlap	=	ifftsize/4
 iwinsize	=	ifftsize
 iwintype	=	1

 kgain	chnget	"gain"
 
 fSig	pvsanal	aSig*kgain*2, ifftsize, ioverlap, iwinsize, iwintype
 fSig2	pvsblur fSig,kSmooth,5

 if	metro(16)==1 then
  kflag	pvsftw fSig2, iampFFT
 endif
 if kflag==1 then
 	 chnset	"tablenumber(1)", "ampFFT"
 endif
 
 aAudioGain	interp	kAudioGain
 		outs	aSig*aAudioGain, aSig*aAudioGain
 		
 if changed(kPhaser)==1 then
  if kPhaser==1 then  
   chnset	"visible(1)","Phs1ID"
   chnset	"visible(1)","Phs2ID"
   chnset	"visible(1)","Phs3ID"
   chnset	"visible(1)","Phs4ID"
   chnset	"visible(1)","Phs5ID"
  else
   chnset	"visible(0)","Phs1ID"
   chnset	"visible(0)","Phs2ID"
   chnset	"visible(0)","Phs3ID"
   chnset	"visible(0)","Phs4ID"
   chnset	"visible(0)","Phs5ID"  
  endif
 endif
 
 if changed(kLPF,kHPF,kBypass)==1 then
  if kLPF==1||kHPF==1||kBypass==1 then  
   chnset	"visible(0)","ResID"
  else
   chnset	"visible(1)","ResID"
  endif
 endif

 if changed(kBypass)==1 then
  if kBypass==1 then  
   chnset	"visible(0)","CFID"
  else
   chnset	"visible(1)","CFID"
  endif
 endif

 kMOUSE_X	chnget	"MOUSE_X"
 kMOUSE_Y	chnget	"MOUSE_Y"
 if changed(kMOUSE_X,kMOUSE_Y)==1 then
  if kMOUSE_X>1 && kMOUSE_X<609 && kMOUSE_Y>1 && kMOUSE_Y<299 then
   kMOUSEFreq	=	(kMOUSE_X/610) * (11025 * (1024/kFFTSize))
   Spos	sprintfk	"pos(%d,%d), text(%d Hz), visible(1)",limit:k(kMOUSE_X,0,553),kMOUSE_Y-10,kMOUSEFreq
  		chnset		Spos,"MOUSEFreqID"
  else
   		chnset		"visible(0)","MOUSEFreqID"
  endif
 endif
  
endin                                                                                                                     

</CsInstruments>
                              
<CsScore>
i 1 0 [3600*24*7]
e
</CsScore>                            

</CsoundSynthesizer>