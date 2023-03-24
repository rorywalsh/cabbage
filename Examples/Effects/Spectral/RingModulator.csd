
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; RingModulator.csd
; Written by Iain McCurdy, 2012.

; Ring Modulator
; --------------
; Modulating waveform is a harmonic waveform based on a sawtooth waveform.
;  The number of harmonics included is defined using 'Harms'.
;  An offset of harmonics to omit is defined using 'Offset'.
;  Therefore Harms=1 and Offset=0 will produce a sine wave.

; Input        -    input mode 

; Freq.        -    frequency of the ring modulator (if input mode is 'Knob')
;             If input mode is 'Kybd.' then base frequency is derived from the note played and the 'Freq.' knob is disregarded.

; 14 sliders - a list of adjustable slider controls the strength of harmonics in the modulating waveform

; Env.        -    amount of amplitude following used to influence the modulator frequency
; Att.        -    attack time used in amplitude following
; Dec.        -    decay time used in amplitude following

; Thresh    -    threshold which, if the rms (amplitude measurement) crosses through, will trigger the generation of a new random modulator frequency.
;             If thresh is zero, the random triggering of new modulating frequencies is bypassed
; Min.        -    minimum of the range within which a new modulating frequency will be generated (oct format)
; Max.        -    maximum of the range within which a new modulating frequency will be generated (oct format)

; Width        -    create stereo width by offsetting the phase of the left channel modulator with respect to that of the right
; Mix        -    dry/wet mix
; Level        -    output level

<Cabbage>
form caption("Ring Modulator") size(825,170), pluginId("rmod"), guiMode("queue")
image               pos(0, 0), size(825, 90), colour("black"), shape("rounded"), outlineColour("lime"), outlineThickness(5) 
label    bounds( 10, 20, 65, 13), text("Input"), fontColour(white)
combobox bounds( 10, 34, 65, 18), text("Knob","Keybd."), channel("input"), textColour("white"), value(1)
rslider bounds( 75, 12, 70, 70), text("Freq."),    channel("freq"),  range(0, 15000, 800, 0.25),   colour("yellow"),    trackerColour(white), textColour(white)

vslider bounds(145, 12,  6, 70), text("1"),     channel("1"),   range(0, 1, 1, 0.5),   colour("yellow"),    trackerColour(white), textColour(white)
vslider bounds(155, 12,  6, 70), text("2"),     channel("2"),   range(0, 1, 0, 0.5),   colour("yellow"),    trackerColour(white), textColour(white)
vslider bounds(165, 12,  6, 70), text("3"),     channel("3"),   range(0, 1, 0, 0.5),   colour("yellow"),    trackerColour(white), textColour(white)
vslider bounds(175, 12,  6, 70), text("4"),     channel("4"),   range(0, 1, 0, 0.5),   colour("yellow"),    trackerColour(white), textColour(white)
vslider bounds(185, 12,  6, 70), text("5"),     channel("5"),   range(0, 1, 0, 0.5),   colour("yellow"),    trackerColour(white), textColour(white)
vslider bounds(195, 12,  6, 70), text("6"),     channel("6"),   range(0, 1, 0, 0.5),   colour("yellow"),    trackerColour(white), textColour(white)
vslider bounds(205, 12,  6, 70), text("7"),     channel("7"),   range(0, 1, 0, 0.5),   colour("yellow"),    trackerColour(white), textColour(white)
vslider bounds(215, 12,  6, 70), text("8"),     channel("8"),   range(0, 1, 0, 0.5),   colour("yellow"),    trackerColour(white), textColour(white)
vslider bounds(225, 12,  6, 70), text("9"),     channel("9"),   range(0, 1, 0, 0.5),   colour("yellow"),    trackerColour(white), textColour(white)
vslider bounds(235, 12,  6, 70), text("10"),    channel("10"),  range(0, 1, 0, 0.5),   colour("yellow"),    trackerColour(white), textColour(white)
vslider bounds(245, 12,  6, 70), text("11"),    channel("11"),  range(0, 1, 0, 0.5),   colour("yellow"),    trackerColour(white), textColour(white)
vslider bounds(255, 12,  6, 70), text("12"),    channel("12"),  range(0, 1, 0, 0.5),   colour("yellow"),    trackerColour(white), textColour(white)
vslider bounds(265, 12,  6, 70), text("13"),    channel("13"),  range(0, 1, 0, 0.5),   colour("yellow"),    trackerColour(white), textColour(white)
vslider bounds(275, 12,  6, 70), text("14"),    channel("14"),  range(0, 1, 0, 0.5),   colour("yellow"),    trackerColour(white), textColour(white)


rslider bounds(285, 12, 70, 70), text("Shape"),   channel("shape"),range(0.125, 64, 1,0.5),          colour("yellow"),    trackerColour(white), textColour(white)

line    bounds(360,  2, 3, 86), colour("lime")
rslider bounds(370, 12, 70, 70), text("Env."),     channel("env"),   range(0, 1.00, 0.0, 0.5,0.001), colour("orange"),    trackerColour(white), textColour(white)
rslider bounds(440,  6, 40, 40), text("Att."),     channel("att"),   range(0.001, 2, 0.01, 0.5),   colour("orange"),    trackerColour(white), textColour(white)
rslider bounds(440, 46, 40, 40), text("Dec."),     channel("dec"),   range(0.001, 2, 0.1,  0.5),   colour("orange"),    trackerColour(white), textColour(white)
line    bounds(485,  2, 3, 86), colour("lime")

rslider bounds(495, 12, 70, 70), text("Thresh"),   channel("thresh"),   range(0, 1.00, 0, 0.5,0), colour("orange"),    trackerColour(white), textColour(white)
rslider bounds(560,  6, 40, 40), text("Min."),     channel("min"),   range(0, 13, 5),   colour("orange"),    trackerColour(white), textColour(white)
rslider bounds(560, 46, 40, 40), text("Max."),     channel("max"),   range(0, 13, 8),   colour("orange"),    trackerColour(white), textColour(white)
line    bounds(605,  2, 3, 86), colour("lime")

rslider bounds(610, 12, 70, 70), text("Width"),    channel("width"), range(0, 1.0, 0.0,1,0.001),   colour("chocolate"), trackerColour(white), textColour(white)
rslider bounds(680, 12, 70, 70), text("Mix"),      channel("mix"),   range(0, 1.00, 1),            colour("chocolate"), trackerColour(white), textColour(white)
rslider bounds(750, 12, 70, 70), text("Level"),    channel("level"), range(0, 1.00, 1),            colour("chocolate"), trackerColour(white), textColour(white)

keyboard bounds(0,92,825,78)
</Cabbage>
<CsoundSynthesizer>
<CsOptions>
;-d -n
-dm0 -n -+rtmidi=NULL -M0
</CsOptions>
<CsInstruments>

; sr is set by host
ksmps   = 32
nchnls  = 2
0dbfs   = 1
massign 0,2

;Author: Iain McCurdy (2012)
;http://iainmccurdy.org/csound.html

gisaw    ftgen    0,0,4096,7,1,4096,-1    ; source modulating waveform (unbandlimited sawtooth waveform) before harmonic filtering
giAmps   ftgen  900, 0, 12, -2, 1

opcode    RingModulator,a,akkkkkkii
    ain,kmix,kfreq,kshape,kenv,katt,kdec,ifn,iphs    xin ; READ IN INPUT ARGUMENTS
    iWet     ftgentmp   0,0,1024,-7,0,512,1,512,1        ; RESCALING FUNCTION FOR WET LEVEL CONTROL
    iDry     ftgentmp   0,0,1024,-7,1,512,1,512,0        ; RESCALING FUNCTION FOR DRY LEVEL CONTROL
    kWet     table      kmix, iWet, 1                    ; RESCALE WET LEVEL CONTROL ACCORDING TO FUNCTION TABLE iWet
    kDry     table      kmix, iDry, 1                    ; RESCALE DRY LEVEL CONTROL ACCORDING TO FUNCTION TABLE iDry
    aFollow  follow2    ain, katt, kdec                  ; AMPLITUDE FOLLOWING kModFrq + (cpsoct(kFollow*kenv*30))         ;CREATE A LEFT CHANNEL MODULATING FREQUENCY BASE ON THE STATIC VALUE CREATED BY kfreq AND THE AMOUNT OF DYNAMIC ENVELOPE FOLLOWING GOVERNED BY kenv
    kFollow  downsamp   aFollow
    kFollow  logcurve   kFollow/0dbfs,2
    kfreq    =          kfreq + (kFollow*kenv*8000)      ; CREATE A LEFT CHANNEL MODULATING FREQUENCY BASE ON THE STATIC VALUE CREATED BY kfreq AND THE AMOUNT OF DYNAMIC ENVELOPE FOLLOWING GOVERNED BY kenv
    aMod     poscil     1, kfreq, ifn, iphs              ; CREATE RING MODULATING SIGNAL
    aMod     powershape aMod, kshape
    
    aout     sum        ain*kDry, ain*aMod*kWet          ; MIX DRY AND WET SIGNALS
             xout       aout                             ; SEND AUDIO BACK TO CALLER INSTRUMENT
endop

instr    1
 kporttime  linseg    0,0.001,0.1
 gkfreq     cabbageGetValue    "freq"
 gkfreq     portk     gkfreq,kporttime
 gkmix      cabbageGetValue    "mix"
 gkwidth    cabbageGetValue    "width"
 gkenv      cabbageGetValue    "env"
 gkatt      cabbageGetValue    "att"
 gkdec      cabbageGetValue    "dec"
 gkthresh   cabbageGetValue    "thresh"
 gkmin      cabbageGetValue    "min"
 gkmax      cabbageGetValue    "max"
 gklevel    cabbageGetValue    "level"
 gkshape    cabbageGetValue    "shape"

 gk1        cabbageGetValue    "1"
 gk2        cabbageGetValue    "2"
 gk3        cabbageGetValue    "3"
 gk4        cabbageGetValue    "4"
 gk5        cabbageGetValue    "5"
 gk6        cabbageGetValue    "6"
 gk7        cabbageGetValue    "7"
 gk8        cabbageGetValue    "8"
 gk9        cabbageGetValue    "9"
 gk10       cabbageGetValue    "10"
 gk11       cabbageGetValue    "11"
 gk12       cabbageGetValue    "12"
 gk13       cabbageGetValue    "13"
 gk14       cabbageGetValue    "14"
 

 ga1,ga2    ins
 ;ga1,ga2    diskin2    "808loop.wav",1,0,1

 gkinput    cabbageGetValue    "input"
 gkinput    init      1
 if changed(gkinput)==1&&gkinput==1 then
            event     "i",2,0,-1
 endif
endin

instr    2
 /* MIDI AND GUI INTEROPERABILITY */
 iMIDIflag  =            0              ; IF MIDI ACTIVATED = 1, NON-MIDI = 0
            mididefault  1, iMIDIflag   ; IF NOTE IS MIDI ACTIVATED REPLACE iMIDIflag WITH '1'
 
 if iMIDIflag==1 then                   ; IF THIS IS A MIDI ACTIVATED NOTE...
  icps      cpsmidi                     ; READ MIDI PITCH VALUES - THIS VALUE CAN BE MAPPED TO GRAIN DENSITY AND/OR PITCH DEPENDING ON THE SETTING OF THE MIDI MAPPING SWITCHES
  kfreq     =            icps
 else
  kfreq     =            gkfreq
 endif                                  ; END OF THIS CONDITIONAL BRANCH

 if gkinput!=1&&iMIDIflag!=1 then
            turnoff
 endif
 
 if gkthresh>0 then
  a1,a2     init         0
  krms      rms          a1+a2
  kRTrig    init         1
  kfreq     trandom      kRTrig,gkmin,gkmax
  kfreq     =            cpsoct(kfreq)
  if changed(kfreq)==1 then
            cabbageSetValue       "freq",kfreq
  endif
  kRTrig    trigger    krms,gkthresh,0
 endif
 
 if metro(8)==1 then 
  if changed(gk1, gk2, gk3, gk4, gk5, gk6, gk7, gk8, gk9, gk10, gk11, gk12, gk13, gk14, gkwidth)==1 then
   reinit UPDATE
  endif
  UPDATE:
 endif
 ifn        ftgen            1, 0, 4096, 10, i(gk1), i(gk2), i(gk3), i(gk4), i(gk5), i(gk6), i(gk7), i(gk8), i(gk9), i(gk10), i(gk11), i(gk12), i(gk13), i(gk14)
 a1         RingModulator    ga1,gkmix,kfreq,gkshape,gkenv,gkatt,gkdec,ifn,i(gkwidth)
 a2         RingModulator    ga2,gkmix,kfreq,gkshape,gkenv,gkatt,gkdec,ifn,0
 rireturn
 aEnv       linsegr          0,0.01,1,0.01,0
 a1         =                a1 * gklevel * aEnv
 a2         =                a2 * gklevel * aEnv
            outs             a1,a2
endin

</CsInstruments>

<CsScore>
i 1 0 z
</CsScore>

</CsoundSynthesizer>