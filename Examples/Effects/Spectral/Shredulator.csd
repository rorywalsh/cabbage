
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; Shredulator.csd
; Written by Iain McCurdy, 2016

; This effect implements an FFT delay, the delay time of which can be shifted using a random sample and gold function.
; In sync with this random function, amplitude can also be randomised ('Granulate' control), as can transposition.

; CONTROLS
; --------

; FFT Size        -  size of FFT window: smaller sizes provides better time resolution but possible distortion of frequency components
; Max Delay       -  Maximum delay time as defined by the circular (pvs) buffer. This is i-rate so altering it will cause discontinuities in the realtime audio stream.
;                    Note that maximum delay time can modulated at k-rate using the 'Depth' control
; Rate            -  Rate at which new random delay times are generated. This also controls the rate at which new random amplitude ('Granulation') 
;                    and random transposition ('Randomise') values are generated.
;                    This control can also be thought of as 'grain size'.
; Depth           -  Amplitude of the random delay time generator. This, along with 'Max.Delay', controls the maximum random delay time possible.
; Granulation     -  Amount of random amplitude variation
; Feedback        -  Ratio of output (pvs) signal that is fed back into the input

; Semitones       -  Number of semitones transposition
; Cents           -  Number of cents transposition
; Pre/Post        -  If 'Pre' is selected, the signal before transposition is sent to the output (transposition are only heard via the feedback signal), 
;                    if 'Post' is selected the, the transposed signal is sent directly to the output
; Randomise       -  If raised above zero, rather than transposition being a fixed value as defined by 'Semitones' and 'Cents' 
;                    it will be a bipolar random value in the range +/- the offset defined by 'Semitones' and 'Cents'.


<Cabbage>
form  size(460,395), caption("_"),colour(225,230,255), pluginId("Shrd"), guiMode("queue")
image   bounds(  0,-10,460, 45), colour(0,0,0,0), outlineThickness(0), plant("title")
{
label   bounds(  0,  1,460, 51), text("SHREDULATOR"), fontColour(155,155,155)
image   bounds( 55, 21,325,  5),   colour(225,230,255), shape("sharp"), rotate(0.1,162,2)
image   bounds( 90, 21,200,  7),   colour(225,230,255), shape("sharp"), rotate(-0.18,100,3)

label   bounds(  1,  3,459, 50), text("SHREDULATOR"), fontColour(  5,  5,  5)
image   bounds( 45, 21, 80,  2),   colour(225,230,255), shape("sharp"), rotate(-0.8,40,1)
image   bounds(110, 21, 55,  2),   colour(225,230,255), shape("sharp"), rotate( 0.8,22,1)
image   bounds( 85, 34,100,  3),   colour(225,230,255), shape("sharp"), rotate(-0.4,22,1)
image   bounds(160, 34,180,  3),   colour(225,230,255), shape("sharp"), rotate(-0.2,45,2)
image   bounds(215, 17, 70,  4),   colour(225,230,255), shape("sharp"), rotate( 0.5,22,2)
image   bounds(250, 21,150,  4),   colour(225,230,255), shape("sharp"), rotate( 0.2,60,2)
image   bounds(271, 31, 53,  2),   colour(225,230,255), shape("sharp"), rotate(-0.1,22,1)
image   bounds(340, 21, 60,  2),   colour(225,230,255), shape("sharp"), rotate( 0.7,22,1)
}

label bounds(358, 39,97,10), fontColour("black"), text("Iain McCurdy |2016|") align("right")

image    bounds(5,50,450,110), colour(225,230,255), outlineColour("Black"), outlineThickness(5), plant("Delay")
{  
label    bounds(  0,  3,440, 13), text("D    E    L    A    Y"), fontColour("Black")
label    bounds( 10, 20, 70, 14), text("FFT Size"), fontColour("Black")
combobox bounds( 10, 35, 70, 20), text("128","256","512","1024","2048","4096"), channel("FFTindex"), value(4)
rslider  bounds( 75, 15, 90,90), text("Max.Delay"), textBox(1), valueTextBox(1), channel("MaxDelay"), range(0.1, 8, 3.7,0.5), fontColour("Black"), textColour("Black"), colour(20,20,155), trackerColour(150,150,225)
rslider  bounds(145, 15, 90,90), text("Rate"), textBox(1), valueTextBox(1), channel("Rate"), range(0.1, 500, 10,0.5,0.1), fontColour("Black"), textColour("Black"), colour(20,20,155), trackerColour(150,150,225)
rslider  bounds(215, 15, 90,90), text("Depth"), textBox(1), valueTextBox(1), channel("Depth"), range(0, 1, 0.2), fontColour("Black"), textColour("Black"), colour(20,20,155), trackerColour(150,150,225)
rslider  bounds(285, 15, 90,90), text("Granulation"), textBox(1), valueTextBox(1), channel("Granulation"), range(0, 1, 0.2), fontColour("Black"), textColour("Black"), colour(20,20,155), trackerColour(150,150,225)
rslider  bounds(355, 15, 90,90), text("Feedback"), textBox(1), valueTextBox(1), channel("Feedback"), range(0, 1, 0.5), fontColour("Black"), textColour("Black"), colour(20,20,155), trackerColour(150,150,225)
}

image    bounds(5,165,450,110), colour(225,230,255), outlineColour("Black"), outlineThickness(5), plant("Transpose")
{  
label    bounds(  0,  3,450, 13), text("T    R    A    N    S    P    O    S    E"), fontColour("Black")
rslider  bounds( 60, 15, 90,90), text("Semitones"), textBox(1), valueTextBox(1), channel("Semitones"), range(-12, 12, 5,1,1), fontColour("Black"), textColour("Black"), colour(20,20,155), trackerColour(150,150,225)
rslider  bounds(130, 15, 90,90), text("Cents"), textBox(1), valueTextBox(1), channel("Cents"), range(-100, 100, 0,1,1), fontColour("Black"), textColour("Black"), colour(20,20,155), trackerColour(150,150,225)
label    bounds(220, 20, 70, 14), text("Pre/Post"), fontColour("Black")
combobox bounds(220, 35, 70, 20), text("Pre","Post"), channel("PrePost"), value(2)
rslider  bounds(290, 15, 90,90), text("Randomise"), textBox(1), valueTextBox(1), channel("TransRand"), range(0, 1, 0.35), fontColour("Black"), textColour("Black"), colour(20,20,155), trackerColour(150,150,225)
}

image    bounds(5,280,450,110), colour(225,230,255), outlineColour("Black"), outlineThickness(5), plant("Output")
{  
label    bounds(  0,  3,450, 13), text("O    U    T    P    U    T"), fontColour("Black")
rslider  bounds(110, 15, 90,90), text("Width"), textBox(1), valueTextBox(1), channel("Width"), range(0, 1, 1), fontColour("Black"), textColour("Black"), colour(20,20,155), trackerColour(150,150,225)
rslider  bounds(180, 15, 90,90), text("Dry/Wet Mix"), textBox(1), valueTextBox(1), channel("DryWetMix"), range(0, 1, 1), fontColour("Black"), textColour("Black"), colour(20,20,155), trackerColour(150,150,225)
rslider  bounds(250, 15, 90,90), text("Level"), textBox(1), valueTextBox(1), channel("Level"), range(0, 1, 1), fontColour("Black"), textColour("Black"), colour(20,20,155), trackerColour(150,150,225)
}

</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-n -d -+rtmidi=NULL -M0 -m0d 
</CsOptions>
                                           
<CsInstruments>

; sr is set by host
ksmps  = 16
nchnls = 2
0dbfs  = 1

opcode SHREDULATOR_LAYER, a, kkkkkkkikkp
 kMaxDelay,kDepth,kRate,kGranulation,kTranspose,kTransRand,kTime,iHandle1,kFeedback,kPrePost,iCount xin
 kDly1                randomh        0,i(kMaxDelay)*kDepth, kRate, 1                ; delay time
 kAmp1                trandom        changed(kDly1),-kGranulation*60,0
 kTranspose1_2        trandom        changed(kDly1),kTranspose-(2*kTranspose*kTransRand),kTranspose
 fsigOut              pvsbufread     kTime-kDly1, iHandle1                      ; read from buffer
 fsigGran             pvsgain        fsigOut,ampdbfs(kAmp1)
 fScale               pvscale        fsigGran,semitone(kTranspose1_2)
 fsigFB               pvsgain        fScale, kFeedback                          ; create feedback signal for next pass
 if kPrePost==1 then
  aDly                pvsynth        fsigGran                                   ; resynthesise read buffer output
 else
  aDly                pvsynth        fScale                                     ; resynthesise read buffer output
 endif
 xout aDly
endop

instr    1
 kMaxDelay            cabbageGetValue         "MaxDelay"
 kMaxDelay            init           1
 kSemitones           cabbageGetValue         "Semitones"
 kCents               cabbageGetValue         "Cents"
 kTransRand           cabbageGetValue         "TransRand"
 kDepth               cabbageGetValue         "Depth"
 kRate                cabbageGetValue         "Rate"
 kTranspose           =              kSemitones + kCents*0.01
 kFeedback            cabbageGetValue         "Feedback"
 kWidth               cabbageGetValue         "Width"
 kDryWetMix           cabbageGetValue         "DryWetMix"
 kLevel               cabbageGetValue         "Level"
 kFFTindex            cabbageGetValue         "FFTindex"
 kFFTindex            init           4
 kGranulation         cabbageGetValue         "Granulation"
 kPrePost             cabbageGetValue         "PrePost"
 kPrePost             init           1
 iFFTsizes[]          fillarray      128,256,512,1024,2048,4096               ; array of FFT size values
 
 ;aL        diskin    "JAG.wav",1,0,1                                         ; read in sound file
 ;aL        poscil    0.4,300
 ;aR        =        aL
 aL,aR        ins
 
 if changed(kMaxDelay,kFFTindex)==1 then
  reinit RESTART
 endif
 RESTART:

 iFFTsize             =              iFFTsizes[i(kFFTindex)-1]                ; retrieve FFT size value from array

 fsigInL              pvsanal        aL, iFFTsize, iFFTsize/4, iFFTsize, 1    ; FFT analyse audio
 fsigInR              pvsanal        aR, iFFTsize, iFFTsize/4, iFFTsize, 1    ; FFT analyse audio
 fsigFB               pvsinit        iFFTsize                                 ; initialise feedback signal
 fsigMixL             pvsmix         fsigInL, fsigFB                          ; mix feedback with input
 fsigMixR             pvsmix         fsigInR, fsigFB                          ; mix feedback with input

 iHandle1, kTime      pvsbuffer      fsigMixL, i(kMaxDelay)                   ; create a circular fsig buffer
  
  kLayers  =   1
  
 aDly                 SHREDULATOR_LAYER kMaxDelay,kDepth,kRate,kGranulation,kTranspose,kTransRand,kTime,iHandle1,kFeedback,kPrePost,i(kLayers)
 aMix                 ntrpol         aL,aDly,kDryWetMix                       ; dry/wet audio mix
                      outs           aMix*kLevel,aMix*kLevel*(1-kWidth)

 iHandle2, kTime      pvsbuffer      fsigMixR, i(kMaxDelay)                   ; create a circular fsig buffer
 aDly                 SHREDULATOR_LAYER kMaxDelay,kDepth,kRate,kGranulation,kTranspose,kTransRand,kTime,iHandle2,kFeedback,kPrePost,i(kLayers)
 aMix                 ntrpol         aR,aDly,kDryWetMix                       ; dry/wet audio mix
                      outs           aMix*kLevel*(1-kWidth),aMix*kLevel

endin

</CsInstruments>

<CsScore>
i 1 0 [60*60*24*365]
</CsScore>

</CsoundSynthesizer>
