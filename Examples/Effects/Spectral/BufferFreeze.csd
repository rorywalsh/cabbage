; BufferFreeze.csd
; Written by Iain McCurdy, 2014, updated 2016

<Cabbage>
form caption("Buffer Freeze"), size(610, 250), pluginID("BfFr"), colour(50,50,75)
gentable      bounds(  5,  5,600,120), tablenumber(1), tablecolour("LightBlue"), amprange(-1,1,1), identchannel(table), zoom(-1)

hslider    bounds(  0,120, 610, 25), channel("ptr"),    range(0, 1.00, 1), trackercolour("LightBlue")
label      bounds(205,143,200,  12), text("Pointer"), fontcolour("white")

checkbox bounds( 15,180, 80, 30), channel("freeze"), text("Freeze"), fontcolour("white"), colour("LightBlue")

rslider    bounds(100,170, 70, 70), channel("InGain"), text("Input Gain"), textcolour("white"), range(0, 8.00, 1,0.5), trackercolour("LightBlue")
rslider    bounds(170,170, 70, 70), channel("DryGain"), text("Dry Gain"), textcolour("white"), range(0, 8.00, 1,0.5), trackercolour("LightBlue")
rslider    bounds(240,170, 70, 70), channel("FreezeGain"), text("Freeze Gain"), textcolour("white"), range(0, 8.00, 1), trackercolour("LightBlue")

label      bounds(325,175, 60,13), text("FFT Size"), fontcolour("white")
combobox   bounds(325,190, 60,20), text("128","256","512","1024","2048","4096","8192"), channel("FFTSize"), value(4), fontcolour(255,255,255)

rslider    bounds(395,170, 70, 70), channel("PtrRand"), text("Ptr.Rand."), textcolour("white"), range(0, 1.00, 0), trackercolour("LightBlue")
rslider    bounds(465,170, 70, 70), channel("Transpose"), text("Transpose"), textcolour("white"), range(-24, 24.00, 0), trackercolour("LightBlue")
rslider    bounds(535,170, 70, 70), channel("TransRand"), text("Trans.Rand."), textcolour("white"), range(0, 24.00, 0), trackercolour("LightBlue")

</Cabbage>
                    
<CsoundSynthesizer>

<CsOptions>   
-dm0 -n
</CsOptions>

<CsInstruments>

sr 		= 	44100	; SAMPLE RATE
ksmps 		= 	16	; NUMBER OF AUDIO SAMPLES IN EACH CONTROL CYCLE
nchnls 		= 	2	; NUMBER OF CHANNELS
0dbfs		=	1	; MAXIMUM AMPLITUDE
			 
giDispBuffer	ftgen	1,0, -600,-2, 0	; define live audio buffer table for display. It makes sense if this corresponds to the display size in pixels.
giAudBuffer	ftgen	2,0, 2^18,10, 0	; define live audio buffer table

instr	1
	aL,aR	ins
	kfreeze	chnget	"freeze"

	koffset	init	0	
	kInGain	chnget	"InGain"
	aL	*=	kInGain
	aR	*=	kInGain
	kDryGain	chnget	"DryGain"
	kFreezeGain	chnget	"FreezeGain"
		
	if kfreeze==0 then							; if writing to table mode...

	 ; write audio to table
	 aptr	phasor	sr/ftlen(giAudBuffer)				
		tablew	aL,aptr,giAudBuffer,1

	koffset			downsamp	aptr				; amount of offset added to freeze read pointer (NB. audio buffer does not scroll)
	
	if metro(sr*ftlen(giDispBuffer)/ftlen(giAudBuffer))==1 then		; update according to size of display table and size of audio buffer	 
	 	tablew	k(aL),ftlen(giDispBuffer),giDispBuffer			; write current audio sample value (as a downsampled krate value) to table
	 	
	 ; SHUNT ENTIRE TABLE CONTENTS ONE STEP TO THE RIGHT
	 kcount	=	0
	 loop:
	 kval	table	kcount+1,giDispBuffer
	 	tablew	kval,kcount,giDispBuffer
	 loop_lt	kcount,1,ftlen(giDispBuffer),loop	

       	 chnset	"tablenumber(1)", "table"					; update table display	

	endif
	 		outs	aL*kDryGain,aR*kDryGain
	 
	else									; otherwise freeze/read mode
	
	 kptr	chnget	"ptr"
	 kporttime linseg 0,0.001,0.01
	 kptr portk kptr, kporttime
	 kPtrRand	gauss	chnget:k("PtrRand")
	 kptr	wrap	kptr+koffset+kPtrRand,0,1
	 kFFTSize	chnget	"FFTSize"
	 kFFTSize	init	4
	 kPitch	=	semitone( chnget:k("Transpose") + gauss:k(chnget:k("TransRand")) )
	 if changed(kFFTSize)==1 then
	  reinit UPDATE
	 endif
	 UPDATE:
	 iFFTSize	=	2^(i(kFFTSize)+6)
	 asig 	mincer 	a(kptr*(ftlen(giAudBuffer)/sr)), kFreezeGain, kPitch, giAudBuffer, 0, iFFTSize
	 rireturn 
		outs	asig,asig

	endif

endin

</CsInstruments>

<CsScore>
i 1 0 [3600*24*7]
</CsScore>

</CsoundSynthesizer>
