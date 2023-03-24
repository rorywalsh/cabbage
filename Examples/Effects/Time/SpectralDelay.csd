
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; SpectralDelay.csd
; Written by Iain McCurdy, 2016

; This effect implements a spectral delay wherein banded analysed data - amplitudes and frequencies - can be delayed on a band by band basis 
;  and independently for each data type.

; Delay times are typically controlled using function tables as individual controls for each delay time would be impractical.
; In this example the user can choosed from 9 preset table types for delaying amplitude and frequency data. 
; In the future I will add further tables and a user-drawable table option.

; Max.Delay       -    Maximum delay time (before scaling by the table). This is an i-rate control.
; FFT Size        -    FFT size affects the quality of the effect. Small FFT sizes will result in distortions of frequency data.
; Mono/Stereo     -    sets whether each channel uses a unique table or the same table for freq and amp delay times
;                       this will have effect if 'random' tables are chosen or if 'Smear' is greater than zero
; Smear           -    delay time random shuffling on the buffer (this is not spectral)  
; Feedback        -    feedback around the spectral delay is performed while the signal is stoll in the format of an f-signal
; Dry/Wet         -    dry/wet control of the effect
; Level           -    output level

<Cabbage>
form caption("")     size(620,345), colour("black"), pluginId("SpDl")
image     pos(0, 0), size(620,345), colour("black"), shape("rounded"), outlineColour(125,130,155), outlineThickness(5) 
;line      bounds(15, 53, 610,  1), colour(150,150,150)

label   bounds( 45,  1,530, 58), text("SPECTRAL DELAY"), fontColour( 70, 50, 50)
label   bounds( 45,  4,530, 52), text("SPECTRAL DELAY"), fontColour( 75, 95, 75)
label   bounds( 45,  7,530, 46), text("SPECTRAL DELAY"), fontColour(150,150,200)
label   bounds( 45, 10,530, 40), text("SPECTRAL DELAY"), fontColour(220,220,220)

rslider bounds( 20, 60, 90,120), text("Max.Delay"), textBox(1), valueTextBox(1), channel("MaxDelay"), range(0.01, 8, 1, 0.5), colour(105,70,70), trackerColour(205,170,170)

label    bounds(120, 65, 80, 14), text("FFT Size")
combobox bounds(120, 80, 80, 20), text("64","128","256","512","1024","2048"), channel("FFTindex"), value(3)

label    bounds(120,115, 80, 14), text("Mono/Stereo")
combobox bounds(120,130, 80, 20), text("Mono","Stereo"), channel("MonoStereo"), value(1)

rslider  bounds(210, 60, 90,120), text("Feedback"), textBox(1), valueTextBox(1), channel("Feedback"), range(0, 1, 0.85), colour(105,70,70), trackerColour(205,170,170)
rslider  bounds(310, 60, 90,120), text("Smear"), textBox(1), valueTextBox(1), channel("Smear"), range(0, 1, 0, 0.5), colour(105,70,70), trackerColour(205,170,170)
rslider  bounds(410, 60, 90,120), text("Dry/Wet Mix"), textBox(1), valueTextBox(1), channel("DryWetMix"), range(0, 1, 1), colour(105,70,70), trackerColour(205,170,170)
rslider  bounds(510, 60, 90,120), text("Level"), textBox(1), valueTextBox(1), channel("Level"), range(0, 1, 0.5), colour(105,70,70), trackerColour(205,170,170)

label    bounds( 20,193, 80, 14), text("Amp.Table")
combobox bounds( 20,208, 80, 20), text("Hi to Lo","Lo to Hi","Random","Peak 1","Peak 2","Peak 3","Comb 1","Comb 2","Spring","Flat"), channel("AmpTable"), value(3)
gentable bounds( 20,230,280, 90), identChannel("AmpTableID"), tableNumber(101), ampRange(0,1,101), tableColour("DarkBlue"), zoom(-1), tablebackgroundColour(200,200,200), tableGridColour(100,100,100)
label    bounds( 20,322,280, 14), text("Amplitudes Table")

label    bounds(320,193, 80, 14), text("Freq.Table")
combobox bounds(320,208, 80, 20), text("Hi to Lo","Lo to Hi","Random","Peak 1","Peak 2","Peak 3","Comb 1","Comb 2","Spring","Flat"), channel("FrqTable"), value(7)
gentable bounds(320,230,280, 90), identChannel("FrqTableID"), tableNumber(102), ampRange(0,1,102), tableColour("DarkGreen"), zoom(-1), tablebackgroundColour(200,200,200), tableGridColour(100,100,100)
label    bounds(320,322,280, 14), text("Frequencies Table")

</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-n -d -+rtmidi=NULL -M0 -m0d 
</CsOptions>

<CsInstruments>

; sr set by host
ksmps = 16
nchnls = 2
0dbfs = 1

giViewTabSize    =    -235*2
giAmpTableView    ftgen    101,0,giViewTabSize,10,1
giFrqTableView    ftgen    102,0,giViewTabSize,10,1

; Tables used purely for viewing
giV1                ftgen            201,0,giViewTabSize,-7,1,abs(giViewTabSize),0                                          ; Hi to Lo
giV2                ftgen            202,0,giViewTabSize,-7,0,abs(giViewTabSize),1                                          ; Lo to Hi
giV3                ftgen            203,0,giViewTabSize,-21,1,1                                                            ; Random
giV4                ftgen            204,0,giViewTabSize,-16,0,abs(giViewTabSize)*0.25,-4,1,abs(giViewTabSize)*0.75,4,0     ; Peak 1
giV5                ftgen            205,0,giViewTabSize,-16,0,abs(giViewTabSize)*0.125,-4,1,abs(giViewTabSize)*0.75,4,0    ; Peak 2
giV6                ftgen            206,0,giViewTabSize,-16,0,abs(giViewTabSize)*0.0125,-4,1,abs(giViewTabSize)*0.075,4,0  ; Peak 3
giV7                ftgen            207,0,giViewTabSize,-19,8,1*0.5,0,1*0.5                                                ; Comb 1
giV8                ftgen            208,0,giViewTabSize,-19,16,1*0.5,0,1*0.5                                               ; Comb 2
giV9                ftgen            209,0,giViewTabSize,-7,0.9,abs(giViewTabSize),1                                        ; Spring
giV9                ftgen            210,0,giViewTabSize,-7,1,abs(giViewTabSize),1                                          ; Flat

instr    1
 ; read in widgets
 kFFTindex        chnget              "FFTindex"
 kFFTindex        init                1
 kAmpTable        chnget              "AmpTable"
 kAmpTable        init                1
 kFrqTable        chnget              "FrqTable"
 kFrqTable        init                1
 kMaxDelay        chnget              "MaxDelay"
 kDryWetMix       chnget              "DryWetMix"
 kFeedback        chnget              "Feedback"
 kLevel           chnget              "Level"
 kMonoStereo      chnget              "MonoStereo"
 kMonoStereo      init                1
 kSmear           chnget              "Smear"
 
 iFFTsizes[]      fillarray           64,128,256,512,1024,2048    ; array of FFT size values

 aL,aR            ins
 
 ; if changed reinitialise mechanism
 if changed(kFFTindex,kAmpTable,kFrqTable,kMaxDelay,kMonoStereo)==1 then    ; if any of the variables in the brackets change...
  reinit    RESTART                                                    ; begin a reinitialisation pass from the label
 endif
 RESTART:                                                            ; a label
 
 iFFT                =                iFFTsizes[i(kFFTindex)-1]        ; retrieve FFT size value from array
 iMaxDelay           limit            i(kMaxDelay), iFFT/sr,8            ; max.delay time must be i-rate

 iftamps1            ftgen            1,0,iFFT,-7,iMaxDelay,iFFT,0                            ; Hi to Lo
 iftamps2            ftgen            2,0,iFFT,-7,0,iFFT,iMaxDelay                            ; Lo to Hi
 iftamps3            ftgen            3,0,iFFT,-21,1,iMaxDelay                                ; Random
 iftamps4            ftgen            4,0,iFFT,-16,0,iFFT*0.25,-4,iMaxDelay,iFFT*0.75,4,0     ; Peak 1
 iftamps5            ftgen            5,0,iFFT,-16,0,iFFT*0.125,-4,iMaxDelay,iFFT*0.75,4,0    ; Peak 2
 iftamps6            ftgen            6,0,iFFT,-16,0,iFFT*0.0125,-4,iMaxDelay,iFFT*0.075,4,0  ; Peak 3
 iftamps7            ftgen            7,0,iFFT,-19,8,iMaxDelay*0.5,0,iMaxDelay*0.5            ; Comb 1
 iftamps8            ftgen            8,0,iFFT,-19,16,iMaxDelay*0.5,0,iMaxDelay*0.5           ; Comb 2
 iftamps9            ftgen            9,0,iFFT,-7,iMaxDelay-(iMaxDelay*0.1),iFFT,iMaxDelay    ; Spring
 iftamps10           ftgen            10,0,iFFT,-7,iFFT,iFFT,iFFT                             ; Flat
 
 ; right channel
 iftamps1R           ftgen            21,0,iFFT,-7,iMaxDelay,iFFT,0                            ; Hi to Lo
 iftamps2R           ftgen            22,0,iFFT,-7,0,iFFT,iMaxDelay                            ; Lo to Hi
 iftamps3R           ftgen            23,0,iFFT,-21,1,iMaxDelay                                ; Random
 iftamps4R           ftgen            24,0,iFFT,-16,0,iFFT*0.25,-4,iMaxDelay,iFFT*0.75,4,0     ; Peak 1
 iftamps5R           ftgen            25,0,iFFT,-16,0,iFFT*0.125,-4,iMaxDelay,iFFT*0.75,4,0    ; Peak 2
 iftamps6R           ftgen            26,0,iFFT,-16,0,iFFT*0.0125,-4,iMaxDelay,iFFT*0.075,4,0  ; Peak 3
 iftamps7R           ftgen            27,0,iFFT,-19,8,iMaxDelay*0.5,0,iMaxDelay*0.5            ; Comb 1
 iftamps8R           ftgen            28,0,iFFT,-19,16,iMaxDelay*0.5,0,iMaxDelay*0.5           ; Comb 2
 iftamps9R           ftgen            29,0,iFFT,-7,iMaxDelay-(iMaxDelay*0.1),iFFT,iMaxDelay    ; Spring
 iftamps10R          ftgen            30,0,iFFT,-7,iFFT,iFFT,iFFT                              ; Flat
 
 iftfrqs1            ftgen            51,0,iFFT,-7,iMaxDelay,iFFT,0                           ; Hi to Lo        
 iftfrqs2            ftgen            52,0,iFFT,-7,0,iFFT,iMaxDelay                           ; Lo to hi
 iftfrqs3            ftgen            53,0,iFFT,-21,1,iMaxDelay                               ; Random
 iftfrqs4            ftgen            54,0,iFFT,-16,0,iFFT*0.25,-4,iMaxDelay,iFFT*0.75,4,0    ; Peak 1
 iftfrqs5            ftgen            55,0,iFFT,-16,0,iFFT*0.125,-4,iMaxDelay,iFFT*0.75,4,0   ; Peak 2
 iftfrqs6            ftgen            56,0,iFFT,-16,0,iFFT*0.0125,-4,iMaxDelay,iFFT*0.075,4,0 ; Peak 3
 iftfrqs7            ftgen            57,0,iFFT,-19,8,iMaxDelay*0.5,0,iMaxDelay*0.5           ; Comb 1
 iftfrqs8            ftgen            58,0,iFFT,-19,16,iMaxDelay*0.5,0,iMaxDelay*0.5          ; Comb 2
 iftfrqs9            ftgen            59,0,iFFT,-7,iMaxDelay-(iMaxDelay*0.1),iFFT,iMaxDelay   ; Spring
 iftfrqs10           ftgen            60,0,iFFT,-7,iFFT,iFFT,iFFT                             ; Flat

 iftfrqs1R           ftgen            71,0,iFFT,-7,iMaxDelay,iFFT,0                           ; Hi to Lo        
 iftfrqs2R           ftgen            72,0,iFFT,-7,0,iFFT,iMaxDelay                           ; Lo to hi
 iftfrqs3R           ftgen            73,0,iFFT,-21,1,iMaxDelay                               ; Random
 iftfrqs4R           ftgen            74,0,iFFT,-16,0,iFFT*0.25,-4,iMaxDelay,iFFT*0.75,4,0    ; Peak 1
 iftfrqs5R           ftgen            75,0,iFFT,-16,0,iFFT*0.125,-4,iMaxDelay,iFFT*0.75,4,0   ; Peak 2
 iftfrqs6R           ftgen            76,0,iFFT,-16,0,iFFT*0.0125,-4,iMaxDelay,iFFT*0.075,4,0 ; Peak 3
 iftfrqs7R           ftgen            77,0,iFFT,-19,8,iMaxDelay*0.5,0,iMaxDelay*0.5           ; Comb 1
 iftfrqs8R           ftgen            78,0,iFFT,-19,16,iMaxDelay*0.5,0,iMaxDelay*0.5          ; Comb 2
 iftfrqs9R           ftgen            79,0,iFFT,-7,iMaxDelay-(iMaxDelay*0.1),iFFT,iMaxDelay   ; Spring
 iftfrqs10R          ftgen            80,0,iFFT,-7,iFFT,iFFT,iFFT                             ; Flat

                     tablecopy        101,200+i(kAmpTable)
                     tablecopy        102,200+i(kFrqTable)
                     chnset            "tableNumber(101)","AmpTableID"
                     chnset            "tableNumber(102)","FrqTableID"

 kSmear1             exprand          kSmear*kMaxDelay*0.5
 kSmear2             exprand          kSmear*kMaxDelay*0.5
            

 fsig_outL           pvsinit          iFFT, iFFT/4, iFFT, 1
 fsig_inL            pvsanal          aL, iFFT, iFFT/4, iFFT, 1                           ; analyse signal
 fsig_FBL            pvsgain          fsig_outL, kFeedback
 fsig_mixL           pvsmix           fsig_inL, fsig_FBL
 ihandleL, ktimeL    pvsbuffer        fsig_mixL, iMaxDelay                                ; write into PV buffer
 fsig_outL           pvsbufread2      ktimeL-kSmear1, ihandleL, i(kAmpTable), 50+i(kFrqTable)     ; read from buffer (with delays)
 awetL               pvsynth          fsig_outL                                           ; resynthesise
 amixL               ntrpol           aL, awetL, kDryWetMix                               ; dry/wet mix

 fsig_outR           pvsinit          iFFT, iFFT/4, iFFT, 1
 fsig_inR            pvsanal          aL, iFFT, iFFT/4,iFFT, 1                            ; analyse signal
 fsig_FBR            pvsgain          fsig_outR, kFeedback
 fsig_mixR           pvsmix           fsig_inR, fsig_FBR
 ihandleR, ktimeR    pvsbuffer        fsig_mixR, iMaxDelay                                ; write into PV buffer
 iAmpTab             =                i(kMonoStereo)==1?i(kAmpTable):i(kAmpTable)+20      ; mono/stereo switch
 iFrqTab             =                i(kMonoStereo)==1?i(kFrqTable):i(kFrqTable)+20 
 kSmearR             =                i(kMonoStereo)==1?kSmear1:kSmear2
 fsig_outR           pvsbufread2      ktimeR-kSmearR, ihandleR, iAmpTab, 50+iFrqTab               ; read from buffer (with delays)
 awetR               pvsynth          fsig_outR                                           ; resynthesise
 amixR               ntrpol           aR, awetR, kDryWetMix                               ; dry/wet mix
 
                     outs             amixL * kLevel, amixR * kLevel
endin

</CsInstruments>

<CsScore>
i 1 0 -1
f 0 3700
</CsScore>

</CsoundSynthesizer>
