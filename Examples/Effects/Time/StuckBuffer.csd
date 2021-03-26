; StuckBuffer.csd
; Written by Iain McCurdy 2014

; Imitates the sound of a audio stuck buffer.

; CONTROLS
; Thresh.	-	probability of there being a stuck buffer
; Freq.Min	-	Rate of probability test (minimum)
; Freq.Max	-	Rate of probability test (maximum)
;			Threshold setting will also affect the frequency with which new buffer sizes are generated. i.e. a threshold setting of '1' will mean that a stuck buffer will be held indefinitely.
; Hold (button)	-	Hold the current buffer indefinitely
; Size.Min.	-	Random Buffer Size (minimum)
; Size.Max.	-	Random Buffer Size (maximum) - (these values raised to the power of 2 give the actual buffer size in samples)
; Intergerise (button)	- round generated buffer size values to the nearest integer. (Fractional offset of Size.Min is retained)
;			This option also integerises transposition values (see below) 
; Trans.Min.	-	Random Transpose (minimum) in octaves 
; Trans.Max.	-	Random Transpose (maximum) in octaves. Transposition is achieved by playing a stuck buffer faster or slower
; Quality (combobox)	-	interpolation used in playing back the buffer: none/linear/cubic. 
;			This will affect the quality of playback when transposed. Lower quality may be desired for 'Lo Fi' effects.
; Width		-	width of random panning of stuck buffers

<Cabbage>
#define RSliderStyle # colour(150,150,150), trackerColour(white), outlineColour(85,85,85)#

form caption("Stuck Buffer") size(600,110), colour(50,50,50), pluginId("StBu") style("legacy")

image pos(0, 0), size(600, 110), colour(50,50,50), shape("sharp"), outlineColour("grey"), outlineThickness(1)

line     bounds( 10, 10,160, 2), colour("grey")
label    bounds( 50,  5, 80, 12), text("PROBABILITY"), colour(50,50,50)
rslider  bounds(  0, 20, 60, 60), text("Thresh."),   channel("thresh"),  range(0, 1.00, 0.4), $RSliderStyle
checkbox bounds( 10, 85,100, 12), text("HOLD"), colour("lime"), channel("hold"),  value(0)
rslider  bounds( 60, 20, 60, 60), text("Freq.Min."), channel("FreqMin"), range(0.1,100.00, 2, 0.5), $RSliderStyle
rslider  bounds(120, 20, 60, 60), text("Freq.Max."), channel("FreqMax"), range(0.1,100.00, 8, 0.5), $RSliderStyle

button   bounds( 95, 84, 50, 16), text("STUCK","STUCK"), active(0), channel("STUCK"), fontColour:0(40,40,40), fontColour:1(255,200,200), colour:1(255,80,80)

line     bounds(190, 10,100,  2), colour("grey")
label    bounds(203	,  5, 75, 12), text("BUFFER SIZE"), colour(50,50,50)
rslider  bounds(180, 20, 60, 60), text("Size.Min."), channel("SizeMin"), range(2.00, 16, 10), $RSliderStyle
rslider  bounds(240, 20, 60, 60), text("Size.Max."), channel("SizeMax"), range(2.00, 16, 14), $RSliderStyle

line     bounds(310, 10,100,  2), colour("grey")
label    bounds(325,  5, 70, 12), text("TRANSPOSE"), colour(50,50,50)
rslider  bounds(300, 20, 60, 60), text("Trans.Min."), channel("TransMin"), range(-7.00, 7, 0), $RSliderStyle
rslider  bounds(360, 20, 60, 60), text("Trans.Max."), channel("TransMax"), range(-7.00, 7, 0), $RSliderStyle
combobox bounds(315, 82, 90, 18	), channel("interp"), value(2), text("No Interp.", "Linear", "Cubic")

line     bounds(430, 10,160,  2), colour("grey")
label    bounds(484,  5, 50, 12), text("OUTPUT"), colour(50,50,50)
rslider  bounds(420,20, 60, 60), text("Width"), channel("PanWidth"), range(0, 1.00, 0.5), $RSliderStyle
rslider  bounds(480,20, 60, 60), text("Mix"), channel("mix"), range(0, 1.00, 0.85), $RSliderStyle
rslider  bounds(540,20, 60, 60), text("Level"), channel("level"), range(0, 1.00, 1), $RSliderStyle

checkbox bounds(190, 85,100, 12), text("INTEGERISE"), colour("lime"), channel("integer"),  value(0)

</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-n -+rtmidi=NULL -M0
;-odac -dm0
</CsOptions>

<CsInstruments>

;sr is set by the host
ksmps = 32
nchnls = 2
0dbfs=1

gibufferL	ftgen	0,0,131072,2,0	; buffer tables
gibufferR	ftgen	0,0,131072,2,0


instr	1

 ;aInL,aInR	diskin	"ClassicalGuitar.wav",1,0,1	; for testing
 ;aL	diskin	"MFEM.wav",1,0,1	; for testing
 ;aR	=	aL
 aInL,aInR	ins

 kporttime	linseg	0,0.01,0.05	; portamento time
 kthresh	chnget	"thresh"
 khold		chnget	"hold"
 kthresh	+=	khold		; add hold value to threshold value
 kFreqMin	chnget	"FreqMin"
 kFreqMax	chnget	"FreqMax"
 kSizeMin	chnget	"SizeMin"
 kSizeMax	chnget	"SizeMax"
 kinteger	chnget	"integer"
 kTransMin	chnget	"TransMin"
 kTransMax	chnget	"TransMax"
 kinterp	chnget	"interp"
 kPanWidth	chnget	"PanWidth"
 kmix		chnget	"mix"
 klevel		chnget	"level"

 kwet		limit	kmix*2,0,1
 kdry		limit	2-(kmix*2),0,1
 
 kSizeMin	portk	kSizeMin,kporttime		; smooth changes to widgets
 kSizeMax	portk   kSizeMax,kporttime	
 kTransMin	portk	kTransMin,kporttime		
 kTransMax	portk   kTransMax,kporttime	

 krate		randomh kFreqMin,kFreqMax,1	; rate of generation of random values
 kTestVal	randomh	0,1,krate		; generate a random value for testing
 
 ktrig	trigger	kTestVal,kthresh,1		; if we are exiting a stuck buffer event, generate a trigger (i.e. new buffer sizes are only generated when a stuck buffer becomes unstuck)
 ksize	trandom	ktrig,kSizeMin,kSizeMax		; generate a new buffer size when a stuck buffer unsticks

 kpan	trandom	ktrig,0.5-(kPanWidth*0.5),0.5+(kPanWidth*0.5)	; generate a random pan value when triggered
 ktrans	trandom	ktrig,kTransMin,kTransMax			; generate a random transposition value when triggered

 if kinteger==1 then				; if 'INTEGERISE' is on...
  kfrac	=	frac(kSizeMin)			; remember fractional value of Size.Min. 
  ksize	=	int(ksize)+kfrac
  ktrans	=	int(ktrans)
 endif

 ksize	limit	ksize,kSizeMin,kSizeMax		; limit buffer size value
 if kinterp==1 then
  ksize	=	int(2 ^ ksize)			; create power of two value (integer)
 else
  ksize	=	2 ^ ksize			; create power of two value
 endif
 
 ktrans	limit	ktrans,kTransMin,kTransMax	; limit transposition
 
 ; write to buffer
 
 if kTestVal>=kthresh then			; normal
  kPhsFrq	=	sr/ksize
 else
  kPhsFrq	=	(sr*octave(ktrans))/ksize
 endif 
 
 aphsr	phasor	kPhsFrq			; create a moving phasor
 aphsr	=	aphsr * ksize		; reScale scope of phasor according to buffer size
 
 if kTestVal>=kthresh then		; normal
  	tablew	aInL,aphsr,gibufferL	; write to buffer
  	tablew	aInR,aphsr,gibufferR
 else					; stuck buffer
  if kinterp==1 then			; choose interpolation method:
   aL	table	aphsr,gibufferL		; none
   aR	table	aphsr,gibufferR
  elseif kinterp==2 then		; linear
   aL	tablei	aphsr,gibufferL
   aR	tablei	aphsr,gibufferR
  else					; cubic
   aL	table3	aphsr,gibufferL
   aR	table3	aphsr,gibufferR
  endif
  aL	=	aL * kpan * kwet	; pan audio
  aR	=	aR * (1-kpan) * kwet
 endif

  aL	sum	aL*kwet, aInL*kdry
  aR	sum	aR*kwet, aInR*kdry
 
 	outs	aL*klevel, aR*klevel
  aL	=	0
  aR	=	0

  ; toggle GUI "STUCK" indicator
  if trigger:k(kTestVal-khold,kthresh,0)==1 then
   chnset	k(0),"STUCK"
  elseif  trigger:k(kTestVal+khold,kthresh,1)==1 then
   chnset	k(1),"STUCK"
  endif
endin

</CsInstruments>  

<CsScore>
i 1 0 3600
e
</CsScore>

</CsoundSynthesizer>
