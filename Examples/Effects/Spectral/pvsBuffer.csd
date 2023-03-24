
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; pvsBuffer.csd
; Writes audio into a circular FFT buffer.
; Read speed can be modified as can the frequencies.
; Take Care! Feedback values above 1 are intended to be used only when transposition is not unison. 

<Cabbage>
form caption("pvsBuffer") size(580,135), pluginId("buff"), guiMode("queue")
image             bounds(0, 0, 580, 135), colour(100, 80, 80,125), shape("rounded"), outlineColour("white"), outlineThickness(4) 

label             bounds(10, 97, 48,10), text("RECORD"), align(right), fontColour(white)
label             bounds(10,112, 48,10), text("PLAY"),   align(right), fontColour(white)
image             bounds( 70,101, 490, 3), colour(255, 50, 50), shape("sharp"), channel("RecIndic")
image             bounds( 70,116, 490, 3), colour( 55,250, 50), shape("sharp"), channel("PlayIndic")

rslider  bounds(10, 11, 70, 70),  text("Speed"),     channel("speed"),     range(0, 4, 1, 0.5, 0.0001), textColour("white"),    colour(100, 80, 80,  5) trackerColour(silver)
rslider  bounds(80, 11, 70, 70),  text("Buf. Size"), channel("buflen"),    range(0.10,20.00, 5, 0.5),   textColour("white"),    colour(100, 80, 80,  5) trackerColour(silver)
rslider  bounds(150, 11, 70, 70), text("Semitones"), channel("semis"),     range(-24, 24, 0, 1, 1),     textColour("white"),    colour(100, 80, 80,  5) trackerColour(silver)
rslider  bounds(220, 11, 70, 70), text("Cents"),     channel("cents"),     range(-100, 100, 0, 1, 1),   textColour("white"),    colour(100, 80, 80,  5) trackerColour(silver)
rslider  bounds(290, 11, 70, 70), text("Feedback"),  channel("FB"),        range(0, 1.50, 0),           textColour("white"),    colour(100, 80, 80,  5) trackerColour(silver)
label    bounds(360, 10, 60,13), text("FFT Size"), fontColour("white")
combobox bounds(360, 25, 60,20), text("128","256","512","1024","2048","4096","8192"), channel("FFT"), value(4), fontColour(255,255,190)
button   bounds(360, 50, 60,20), text("RECORD","RECORD"), colour:0(50,0,0), colour:1(250,100,100), value(1), channel("Record")
rslider bounds(430, 11, 70, 70), text("Mix"),       channel("mix"),       range(0, 1.00, 1),           textColour("white"),    colour(100, 80, 80,  5) trackerColour(silver)
rslider bounds(500, 11, 70, 70), text("Level"),     channel("lev"),       range(0, 1.00, 0.5),         textColour("white"),    colour(100, 80, 80,  5) trackerColour(silver)
</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-dm0 -n
</CsOptions>
<CsInstruments>

; sr is set by host
ksmps       =     32
nchnls      =     2
0dbfs       =     1    ; MAXIMUM AMPLITUDE

;Author: Iain McCurdy (2012)
;http://iainmccurdy.org/csound.html

opcode    pvsbuffer_module,akk,akkkkkkik
    ain,kspeed,kbuflen,kscale,kfeedback,kmix,klev,iFFT,kRecord   xin
    kPhOffset      =         0
    kspeed         init      1
    kscale         init      1
    if changed:k(kbuflen)==1 then
     reinit    UPDATE
    endif
    UPDATE:
    ibuflen        =         i(kbuflen)
    iphasor        ftgen     0, 0, 65536, 7, 0, 65536, 1            ; WAVE SHAPE FOR A MOVING PHASE POINTER
    aread          osciliktp kspeed/ibuflen, iphasor, kPhOffset     ; CREATE MOVING POINTER TO READ FROM BUFFER
    kread          downsamp  aread
    kread          =         kread * ibuflen
    aFB            init      0
    f_anal         pvsanal   ain+aFB, iFFT, iFFT/4, iFFT, 1         ; ANALYSE THE AUDIO SIGNAL THAT WAS CREATED IN INSTRUMENT 1. OUTPUT AN F-SIGNAL.
    if kRecord==1 then
     ibuffer,ktime pvsbuffer f_anal, ibuflen                        ; BUFFER FSIG    
    endif
    rireturn
    khandle        init       ibuffer                               ; INITIALISE HANDLE TO BUFFER
    f_buf          pvsbufread kread , khandle                       ; READ BUFFER
    f_scale        pvscale    f_buf, kscale                         ; RESCALE FREQUENCIES
    aresyn         pvsynth    f_scale                               ; RESYNTHESIZE THE f-SIGNAL AS AN AUDIO SIGNAL    
    aFB            dcblock2   aresyn * kfeedback                    ; CREATE FEEDBACK SIGNAL FOR NEXT PASS AND BLOCK DC OFFSET ACCUMULATION
    amix           ntrpol     ain, aresyn, kmix                     ; CREATE DRY/WET MIX
                   xout       amix*klev,ktime/ibuflen,kread/ibuflen
endop

instr    1
    ainL,ainR    ins
    ;ainL,ainR     diskin    "SynthPad.wav",1,0,1    ;USE FOR TESTING
    kspeed         cabbageGetValue    "speed"
    kbuflen        cabbageGetValue    "buflen"
    ksemis         cabbageGetValue    "semis"
    kcents         cabbageGetValue    "cents"
    kRecord        cabbageGetValue    "Record"
    ksemis         init      0
    kcents         init      0
    kscale         =         semitone(ksemis)*cent(kcents)
    kscale         init      1
    kbuflen        init      5
    kspeed         init      1
    kmix           cabbageGetValue    "mix"
    kfeedback      cabbageGetValue    "FB"
    klev           cabbageGetValue    "lev"
    kmix           init      1
    kfeedback      init      0
    klev           init      0.5

    /* SET FFT ATTRIBUTES */
    kFFT           cabbageGetValue    "FFT"
    kFFT           init      4
    ktrig          changed   kFFT
    if ktrig==1 then
     reinit update
    endif
    update:
    
    aoutL,kwrite,kread     pvsbuffer_module    ainL,kspeed,kbuflen,kscale,kfeedback,kmix,klev,2^(i(kFFT)+6),kRecord
    aoutR,k1,k1            pvsbuffer_module    ainR,kspeed,kbuflen,kscale,kfeedback,kmix,klev,2^(i(kFFT)+6),kRecord
    rireturn
                   outs    aoutL,aoutR

                  cabbageSet  changed:k(kwrite),"RecIndic","bounds",70,97,kwrite*490,3
                  cabbageSet  changed:k(kread),"PlayIndic","bounds",70,112,kread*490,3
endin

</CsInstruments>
<CsScore>
i 1 0 z
</CsScore>
</CsoundSynthesizer>