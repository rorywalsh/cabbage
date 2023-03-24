
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; No-Warp Delay
; Iain McCurdy, 2021

; This is a dual/stereo delay effect using streaming phase vocoding so that changes in the delay tome will not cause glitches or pitch warping effects.
; The number of echoes is controlled by defination of the RT60 value so that rate of decay is independent of delay time 
;    (unlike when using a feedback ratio control)

; Maximum delay time is 5 seconds

; In Gain         - gain control on the input signal
; Time L / Time R - control the delay times on the left and right channels independently
; Link            - links the Time L / Time R controls
; Time Scale      - scales the delay time on both channels
; Decay Time      - time for delays to decay to -60dB (RT60 time)
; Pan L / Pan R   - panning location for two delays
; FFT Size        - FFT window size used in the streaming spectral analysis. 
;                   Smaller values will give better transient resolution, larger values better frequency resolution
; Dry/Wet         - Mix between the original input signal and the delayed signal

<Cabbage>
form caption("No-Warp Delay") size(665,125), pluginId("NWDl"), colour(50,50,50)
;label    bounds( 25, 24,120,  1), colour("silver")
rslider  bounds( 10, 25, 80, 90),  text("In Gain"),    channel("InGain"), range(0, 1, 1, 0.5), textColour("white"), valueTextBox(1)   colour(100, 80, 80,  5) trackerColour(silver)
checkbox bounds( 65,  5, 70, 15),  text("LINK"),    channel("Link"), textColour("white"), fontColour:0("white"), fontColour:1("white")
rslider  bounds( 80, 25, 80, 90),  text("Time L"),    channel("DlyTimL"), range(0.01, 5, 0.2, 0.5, 0.0001), textColour("white"), valueTextBox(1)   colour(100, 80, 80,  5) trackerColour(silver)
rslider  bounds(150, 25, 80, 90),  text("Time R"),    channel("DlyTimR"), range(0.01, 5, 0.2, 0.5, 0.0001), textColour("white"), valueTextBox(1),    colour(100, 80, 80,  5) trackerColour(silver)
rslider  bounds(220, 25, 80, 90),  text("Time Scale"),    channel("TimeScale"), range(0.01, 1, 1, 0.5), textColour("white"), valueTextBox(1),    colour(100, 80, 80,  5) trackerColour(silver)
rslider  bounds(290, 25, 80, 90),  text("Decay Time"), channel("DecayTime"),   range(0.001, 60, 10, 0.5, 0.0001), textColour("white"), valueTextBox(1),    colour(100, 80, 80,  5) trackerColour(silver)
rslider  bounds(360, 25, 80, 90),  text("Pan L"),      channel("PanL"), range(0, 1, 0), textColour("white"), valueTextBox(1),    colour(100, 80, 80,  5) trackerColour(silver)
rslider  bounds(430, 25, 80, 90),  text("Pan R"),      channel("PanR"), range(0, 1, 1), textColour("white"), valueTextBox(1),    colour(100, 80, 80,  5) trackerColour(silver)
label    bounds(505, 20, 80, 13), text("FFT Size") fontColour("white")
combobox bounds(505, 35, 80, 25), text("FFTsize"),      channel("FFTsize"), items("512","1024","2048","4096"), value(2)
rslider  bounds(580, 25, 80, 90),  text("Dry/Wet"),      channel("DryWet"), range(0, 1, 0.5), textColour("white"), valueTextBox(1),    colour(100, 80, 80,  5) trackerColour(silver)
</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-d -n
</CsOptions>

<CsInstruments>

nchnls = 2
0dbfs  = 1

; UDO for the delay, this saves having to repeat a lot of code for both channels

opcode	pvsdelay,a,akkiiiii
	ain,kDlyTim,kFBamt,iMaxDelay,iFFTsize,ioverlap,iwinsize,iwintype	xin
	f_FB		pvsinit 	iFFTsize,ioverlap,iwinsize,iwintype, 0		; INITIALISE FEEDBACK FSIG
	f_Dry  		pvsanal		ain, iFFTsize, ioverlap, iwinsize, iwintype	; ANALYSE THE AUDIO SIGNAL THAT WAS CREATED IN INSTRUMENT 1. OUTPUT AN F-SIGNAL.
	f_Mix		pvsmix		f_Dry, f_FB					                ; MIX AUDIO INPUT WITH FEEDBACK SIGNAL	
	ibuffer,ktime  	pvsbuffer	f_Mix, iMaxDelay+1/ksmps				; BUFFER FSIG
	khandle		init 		ibuffer						                ; INITIALISE HANDLE TO BUFFER
	kread		=		    ktime-kDlyTim					            ; READ POINTER LAGS BEHIND WRITE POINTER BY gkdlt SECONDS
	f_Delay  	pvsbufread  kread , khandle					            ; READ BUFFER
	f_FB		pvsgain 	f_Delay, kFBamt					            ; CREATE FEEDBACK F-SIGNAL FOR NEXT PASS
	aresyn 		pvsynth  	f_Delay						                ; RESYNTHESIZE THE f-SIGNAL AS AN AUDIO SIGNAL	
			    xout		aresyn						                ; SEND AUDIO BACK TO CALLER INSTRUMENT
endop

instr   1

aInL,aInR      ins
kInGain    chnget "InGain"
kPortTime linseg 0, 0.001, 0.1
kInGain portk kInGain, kPortTime

aInL *= kInGain
aInR *= kInGain

kDlyTimL chnget "DlyTimL"
kDlyTimR chnget "DlyTimR"
kLink    chnget "Link"

; code for linking delay time controls
if kLink==1 then
 if changed:k(kDlyTimL)==1 then
  chnset kDlyTimL,"DlyTimR"
 elseif changed:k(kDlyTimR)==1 then
  chnset kDlyTimR,"DlyTimL"
 endif
endif

kTimeScale chnget "TimeScale"

kDecayTime   chnget "DecayTime"

; calculate feedback ratio for chosen RT60 value (left channel)
kNumL      =         kDecayTime / (kDlyTimL*kTimeScale)
kStepL     =         -60 / kNumL 
kFeedbackL =         ampdbfs(kStepL)

; calculate feedback ratio for chosen RT60 value (right channel)
kNumR      =         kDecayTime / (kDlyTimR*kTimeScale)
kStepR     =         -60 / kNumR
kFeedbackR =         ampdbfs(kStepR)

kPanL chnget "PanL"
kPanR chnget "PanR"

iMaxDelay=      5

kFFTsize chnget "FFTsize"
kFFTsize init   2

if changed:k(kFFTsize)==1 then
 reinit RESTART
endif
RESTART:

iFFTsize = 2 ^ (i(kFFTsize) + 8)

ioverlap =      iFFTsize/8
iwinsize =      iFFTsize
iwintype =      0

; call UDO
aOutL    pvsdelay aInL,kDlyTimL*kTimeScale,kFeedbackL,iMaxDelay,iFFTsize,ioverlap,iwinsize,iwintype
aOutR    pvsdelay aInR,kDlyTimR*kTimeScale,kFeedbackR,iMaxDelay,iFFTsize,ioverlap,iwinsize,iwintype

rireturn

; dry/wet control
kDryWet  chnget "DryWet"
kDry     limit  (2 - kDryWet*2), 0, 1
kWet     limit  (kDryWet * 2), 0, 1

; send audio mix to outputs
         outs   (aInL*kDry) + ((aOutL*(1-kPanL) + aOutR*(1-kPanR))*kWet)  , (aInR*kDry) + ((aOutL*kPanL + aOutR*kPanR)*kWet)
                 
endin

</CsInstruments>

<CsScore>
i 1 0 [3600*24]
</CsScore>

</CsoundSynthesizer>