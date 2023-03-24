
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; BouncingObject.csd
; Written by Iain McCurdy, 2014

; MIDI note translates to overall transposition of the gesture
; MIDI velocity translates to a scaling of the gesture duration (lower velocities translate to shorter gesture durations) 
; Using an input other than 'sample' will probably demand giving 'Element Amplitude Envelope' a percussive-type envelope.

; Time Gap - trajectory of gap between subsequent sample iterations across the gesture (bounces)

; Transpose - trajectory of transposition of sample iterations across the gesture (in semitones)

; Filter - trajectory of filter cutoff frequencies across the gesture (in octave pitch format)

; Gesture Amplitude Envelope - amplitude envelope applied across the entire gesture

; Element Amplitude Envelope - amplitude envelope applied to each sample iteration (bounce)

<Cabbage>
form caption("Bouncing Object"), size(390, 635), pluginId("bnOb")

#define RSliderStyle trackerColour(255,255,255), colour(100,200,140), outlineColour(40,40,40), textColour(white)

image      bounds(  5,  5,380, 70), colour( 50,100, 70), outlineColour("white"), line(2), shape("sharp"),plant("main"){
filebutton bounds(  5,  8, 80, 25), text("Open File","Open File"), fontColour("white") channel("filename"), shape("ellipse")
button     bounds(  5, 37, 80, 25), channel("GestureTrigger"), text("Trigger","Trigger"), value(0), latched(0)
rslider    bounds( 90,  5, 60, 60), channel("dur"), text("Duration"), range(0.1, 20.00,  4,0.25), $RSliderStyle
label      bounds(150, 12, 65, 12), text("Input:"),  fontColour("white")
combobox   bounds(150, 25, 65, 20), channel("input"), value(2), text("Sample", "Sine", "Saw", "Square", "Tri", "Noise")

label    bounds(220, 12, 90, 12), text("Filter Type:"),  fontColour("white")
combobox bounds(220, 25, 90, 20), channel("FiltType"), value(1), text("Lowpass", "Bandpass", "Reson", "Highpass", "Moogladder")
rslider  bounds(310,  5, 60, 60), channel("bandwidth"), text("Bandwidth"), range(0.05, 2, 0.1, 0.25), $RSliderStyle
}

image   bounds(  5, 80,380, 90), colour( 50,100, 70), outlineColour("white"), line(2), shape("sharp"),plant("time"){
label   bounds(  0,  3,170, 12), text("Time Gap"),  fontColour("white")
rslider bounds(  0, 25, 60, 60), channel("TimeS"), text("Start"), range(0.01, 1.00,  0.5), $RSliderStyle
rslider bounds( 55, 25, 60, 60), channel("TimeE"), text("End"),   range(0.01, 1.00,  0.05), $RSliderStyle
rslider bounds(110, 25, 60, 60), channel("TimeC"), text("Curve"), range(  -40, 40.00,-0.4), $RSliderStyle
gentable   bounds(175,  5,200, 80), identChannel("table1"), tableNumbers(1), tableColour("lime"), ampRange(0.01,1,1), zoom(-1), fill(0)
}

image   bounds(  5,175,380, 90), colour( 50,100, 70), outlineColour("white"), line(2), shape("sharp"),plant("Transpose"){
label   bounds(  0,  3,170, 12), text("Transpose"),  fontColour("white")
rslider bounds(  0, 25, 60, 60), channel("TransS"), text("Start"), range(-48, 48,  0), $RSliderStyle
rslider bounds( 55, 25, 60, 60), channel("TransE"), text("End"),   range(-48, 48,  0), $RSliderStyle
rslider bounds(110, 25, 60, 60), channel("TransC"), text("Curve"), range(  -40, 40.00, 0), $RSliderStyle
gentable   bounds(175,  5,200, 80), identChannel("table2"), tableNumbers(2), tableColour("yellow"), ampRange(-48,48,2), zoom(-1), fill(0)
}

image   bounds(  5,270,380, 90), colour( 50,100, 70), outlineColour("white"), line(2), shape("sharp"),plant("Filter"){
label   bounds(  0,  3,170, 12), text("Filter"),  fontColour("white")
rslider bounds(  0, 25, 60, 60), channel("FilterS"), text("Start"), range(4, 14.00,  14), $RSliderStyle
rslider bounds( 55, 25, 60, 60), channel("FilterE"), text("End"),   range(4, 14.00,  5), $RSliderStyle
rslider bounds(110, 25, 60, 60), channel("FilterC"), text("Curve"), range(-40, 40.00,-2), $RSliderStyle
gentable   bounds(175,  5,200, 80), identChannel("table3"), tableNumbers(3), tableColour("LightBlue"), ampRange(4,14,3), zoom(-1), fill(0)
}

image   bounds(  5,365,380, 90), colour( 50,100, 70), outlineColour("white"), line(2), shape("sharp"),plant("Amp"){
label   bounds(  0,  3,170, 12), text("Gesture Amplitude Envelope"),  fontColour("white")
rslider bounds(  0, 25, 60, 60), channel("AmpS"), text("Start"), range(0, 1,  1), $RSliderStyle
rslider bounds( 55, 25, 60, 60), channel("AmpE"), text("End"),   range(0, 1,0.5), $RSliderStyle
rslider bounds(110, 25, 60, 60), channel("AmpC"), text("Curve"), range(-40, 40.00, 0), $RSliderStyle
gentable   bounds(175,  5,200, 80), identChannel("table4"), tableNumbers(4), tableColour("Pink"), ampRange(0,1,4), zoom(-1), fill(0)
}

image   bounds(  5,460,380, 90), colour( 50,100, 70), outlineColour("white"), line(2), shape("sharp"),plant("ElementAmp"){
label   bounds(  0,  3,170, 12), text("Element Amplitude Envelope"),  fontColour("white")
rslider bounds(  0, 25, 60, 60), channel("ElementAmpS"), text("Start"), range(0, 1,  1), $RSliderStyle
rslider bounds( 55, 25, 60, 60), channel("ElementAmpE"), text("End"),   range(0, 1,  1), $RSliderStyle
rslider bounds(110, 25, 60, 60), channel("ElementAmpC"), text("Curve"), range(-40, 40.00, 0), $RSliderStyle
gentable   bounds(175,  5,200, 80), identChannel("table5"), tableNumbers(5), tableColour("Orange"), ampRange(0,1,5), zoom(-1), fill(0)
}

keyboard bounds(  0,555,390, 80)

</Cabbage>
                    
<CsoundSynthesizer>

<CsOptions>   
-dm0 -n -+rtmidi=NULL -M0
</CsOptions>

<CsInstruments>

; sr set by host
ksmps         =     32    ;NUMBER OF AUDIO SAMPLES IN EACH CONTROL CYCLE
nchnls         =     2    ;NUMBER OF CHANNELS (1=MONO)
0dbfs        =    1    ;MAXIMUM AMPLITUDE

massign    0,2

giTabSize    =    4096

gitimes        ftgen    1,0,giTabSize,-16, 1,giTabSize,0,0
gitrans        ftgen    2,0,giTabSize,-16, 0,giTabSize,0,0
gifilter    ftgen    3,0,giTabSize,-16,14,giTabSize,0,4
giamp        ftgen    4,0,giTabSize,-16, 1,giTabSize,0,0.5
giElementAmp    ftgen    5,0,giTabSize,-16, 1,giTabSize,0,0.5

gisine        ftgen    6,0,4096,10,1

gichans        init    0
giReady        init    0
gSfilepath    init    ""


instr    1
 gkGestureTrigger    chnget    "GestureTrigger"
 gkdur        chnget    "dur"
 gkinput    chnget    "input"
 gkFiltType    chnget    "FiltType"
 gkFiltType    init    1
 gkbandwidth    chnget    "bandwidth"

 gkTimeS    chnget    "TimeS"
 gkTimeC    chnget    "TimeC"
 gkTimeE    chnget    "TimeE"

 gkTransS    chnget    "TransS"
 gkTransC    chnget    "TransC"
 gkTransE    chnget    "TransE"
 
 gkFilterS    chnget    "FilterS"
 gkFilterC    chnget    "FilterC"
 gkFilterE    chnget    "FilterE"

 gkAmpS    chnget    "AmpS"
 gkAmpC    chnget    "AmpC"
 gkAmpE    chnget    "AmpE"

 gkElementAmpS    chnget    "ElementAmpS"
 gkElementAmpC    chnget    "ElementAmpC"
 gkElementAmpE    chnget    "ElementAmpE"

 gSfilepath    chnget    "filename"
 kNewFileTrg    changed    gSfilepath        ; if a new file is loaded generate a trigger
 if kNewFileTrg==1 then                ; if a new file has been loaded...
  event    "i",99,0,0                ; call instrument to update sample storage function table 
 endif  
 
 ktrig    changed    gkTimeS,gkTimeE,gkTimeC
 if ktrig==1 then
  reinit    UPDATE
  UPDATE:
  gitimes    ftgen    1,0,giTabSize,-16,i(gkTimeS),giTabSize,i(gkTimeC),i(gkTimeE)
  rireturn
         chnset    "tableNumber(1)", "table1"
 endif

 ktrig    changed    gkTransS,gkTransE,gkTransC
 if ktrig==1 then
  reinit    UPDATE2
  UPDATE2:
  gitrans    ftgen    2,0,giTabSize,-16,i(gkTransS),giTabSize,i(gkTransC),i(gkTransE)
  rireturn
         chnset    "tableNumber(2)", "table2"
 endif

 ktrig    changed    gkFilterS,gkFilterE,gkFilterC
 if ktrig==1 then
  reinit    UPDATE3
  UPDATE3:
  gifilter    ftgen    3,0,giTabSize,-16,i(gkFilterS),giTabSize,i(gkFilterC),i(gkFilterE)
  rireturn
         chnset    "tableNumber(3)", "table3"
 endif

 ktrig    changed    gkAmpS,gkAmpE,gkAmpC
 if ktrig==1 then
  reinit    UPDATE4
  UPDATE4:
  giamp        ftgen    4,0,giTabSize,-16,i(gkAmpS),giTabSize,i(gkAmpC),i(gkAmpE)
  rireturn
         chnset    "tableNumber(4)", "table4"
 endif

 ktrig    changed    gkElementAmpS,gkElementAmpE,gkElementAmpC
 if ktrig==1 then
  reinit    UPDATE5
  UPDATE5:
  giElementAmp    ftgen    5,0,giTabSize,-16,i(gkElementAmpS),4096,i(gkElementAmpC),i(gkElementAmpE)
  rireturn
         chnset    "tableNumber(5)", "table5"
 endif

 if trigger:k(gkGestureTrigger,0.6,0)==1 then
  event    "i",3,0,gkdur,60
 endif
endin

instr    2
 inum        notnum
 ivel        veloc    0,1
 event_i    "i",3,0,i(gkdur)*ivel,inum
endin

instr    3    ; TRIGGERS GESTURE
 iInSkip    =    (i(gkdur)-p3)/i(gkdur)
 iOffset    =    p4-60
 itrans        tablei    0,gitrans,1
 ifilter    tablei    0,gifilter,1
 iamp        tablei    0,giamp,1
 itime        tablei    1,gitimes
 kphase        line        0,p3,1
 ktime        table        kphase+iInSkip,gitimes,1
 ktrans        table        kphase+iInSkip,gitrans,1
 kfilter    table        kphase,gifilter,1
 kamp        table        kphase+iInSkip,giamp,1

 ktrig        metro        1/ktime
     schedkwhen    ktrig, 0, 0, 4, 0, 0.01, ktrans+iOffset, kfilter, kamp
 
endin



instr    4    ; INDIVIDUAL IMPULSE SOUNDS
 if giReady==1||i(gkinput)!=1 then                        ; i.e. if a file has been loaded
  if i(gkinput)==1 then
   ifn        =        101
   iNChans    =        ftchnls(ifn)
   imlt        =        semitone(p4)
   p3        =        ftlen(ifn)/(sr*imlt)
  else
   p3        =        0.1
  endif
  icf        =        cpsoct(p5)
  aline        line        0,p3,1
  aenv        tablei        aline,giElementAmp,1
  ilevel    =        p6
  ;iFiltType    =        i(gkFiltType)  
  if iNChans==1||i(gkinput)!=1 then
   if i(gkinput)==1 then
    ;asig        loscil3        ilevel,imlt,ifn,1   ; commented while loscil transposition is broken
    asig        poscil        ilevel,imlt/p3,ifn
   elseif i(gkinput)==2 then
    aenv2    linseg    0,0.005,1,p3-0.005,0
    asig    poscil    aenv2*ilevel*0.2,cpsmidinn(p4+60),gisine
   elseif i(gkinput)==3 then
    kenv2    linseg    0,0.005,1,p3-0.005,0
    asig    vco2    kenv2*ilevel*0.2,cpsmidinn(p4+60),0
   elseif i(gkinput)==4 then
    kenv2    linseg    0,0.005,1,p3-0.005,0
    asig    vco2    kenv2*ilevel*0.2,cpsmidinn(p4+60),2,0.5
   elseif i(gkinput)==5 then
    kenv2    linseg    0,0.005,1,p3-0.005,0
    asig    vco2    kenv2*ilevel*0.2,cpsmidinn(p4+60),4,0.5
   elseif i(gkinput)==6 then
    kenv    linseg    0,0.005,1,p3-0.005,0
    asig    pinkish    kenv2*ilevel
   endif
   ;asig        poscil3        ilevel,imlt,ifn
   if i(gkFiltType)==1 then
    asig        butlp        asig,icf
   elseif i(gkFiltType)==2 then
    asig        butbp        asig,icf,icf*i(gkbandwidth)
   elseif i(gkFiltType)==3 then
    asig        reson        asig,icf,icf*i(gkbandwidth),1
   elseif i(gkFiltType)==4 then
    asig        buthp        asig,icf
   elseif i(gkFiltType)==5 then
    asig        moogladder    asig,icf,i(gkbandwidth)*0.49
   endif
   asig        =        asig * aenv
           outs        asig,asig
  elseif iNChans==2 then
   aL,aR        loscil3        ilevel,imlt,ifn,1
   if i(gkFiltType)==1 then
    aL        butlp        aL,icf
    aR        butlp        aR,icf
   elseif i(gkFiltType)==2 then
    aL        butbp        aL,icf,icf*i(gkbandwidth)
    aR        butbp        aR,icf,icf*i(gkbandwidth)
   elseif i(gkFiltType)==3 then
    aL        reson        aL,icf,icf*i(gkbandwidth),1
    aR        reson        aR,icf,icf*i(gkbandwidth),1
   elseif i(gkFiltType)==4 then
    aL        buthp        aL,icf
    aR        buthp        aR,icf
   elseif i(gkFiltType)==5 then
    aL        moogladder    aL,icf,i(gkbandwidth)*0.49
    aR        moogladder    aR,icf,i(gkbandwidth)*0.49
   endif
   aL        =        aL * aenv
   aR        =        aR * aenv
           outs        aL,aR
  endif
 endif
endin


instr    99    ; load sound file
 gitable    ftgen    101,0,0,1,gSfilepath,0,0,0        ; load sound file into a GEN 01 function table 
 gichans    filenchnls    gSfilepath            ; derive the number of channels (mono=1,stereo=2) in the sound file
 giReady     =    1                    ; if no string has yet been loaded giReady will be zero
endin



</CsInstruments>

<CsScore>
i 1 0 3600
</CsScore>

</CsoundSynthesizer>
