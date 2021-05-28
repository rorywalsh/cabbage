
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
form caption("pvsBuffer") size(580,135), pluginId("buff")
image             bounds(0, 0, 580, 135), colour(100, 80, 80,125), shape("rounded"), outlineColour("white"), outlineThickness(4) 

label             bounds(10, 97, 48,10), text("RECORD"), align(right), fontColour(white)
label             bounds(10,112, 48,10), text("PLAY"),   align(right), fontColour(white)
image             bounds( 70,101, 490, 3), colour(255, 50, 50), shape("sharp"), identChannel("RecIndic")
image             bounds( 70,116, 490, 3), colour( 55,250, 50), shape("sharp"), identChannel("PlayIndic")

rslider bounds(10, 11, 70, 70),  text("Speed"),     channel("speed"),     range(0, 4, 1, 0.5, 0.0001), textColour("white"),    colour(100, 80, 80,  5) trackerColour(silver)
rslider bounds(80, 11, 70, 70),  text("Buf. Size"), channel("buflen"),    range(0.10,20.00, 5, 0.5),   textColour("white"),    colour(100, 80, 80,  5) trackerColour(silver)
rslider bounds(150, 11, 70, 70), text("Semitones"), channel("semis"),     range(-24, 24, 0, 1, 1),     textColour("white"),    colour(100, 80, 80,  5) trackerColour(silver)
rslider bounds(220, 11, 70, 70), text("Cents"),     channel("cents"),     range(-100, 100, 0, 1, 1),   textColour("white"),    colour(100, 80, 80,  5) trackerColour(silver)
rslider bounds(290, 11, 70, 70), text("Feedback"),  channel("FB"),        range(0, 1.50, 0),           textColour("white"),    colour(100, 80, 80,  5) trackerColour(silver)
label    bounds(360,20, 60,13), text("FFT Size"), fontColour("white")
combobox bounds(360,35, 60,20), text("64","128","256","512","1024","2048","4096","8192"), channel("att_table"), value(5), fontColour(255,255,190)

rslider bounds(430, 11, 70, 70), text("Mix"),       channel("mix"),       range(0, 1.00, 1),           textColour("white"),    colour(100, 80, 80,  5) trackerColour(silver)
rslider bounds(500, 11, 70, 70), text("Level"),     channel("lev"),       range(0, 1.00, 0.5),         textColour("white"),    colour(100, 80, 80,  5) trackerColour(silver)
</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-d -n
</CsOptions>
<CsInstruments>

; sr is set by host
ksmps         =     32
nchnls         =     2
0dbfs        =    1    ;MAXIMUM AMPLITUDE

;Author: Iain McCurdy (2012)
;http://iainmccurdy.org/csound.html

/* FFT attribute tables */
giFFTattributes1    ftgen    0, 0, 4, -2,   64,  32,   64, 1
giFFTattributes2    ftgen    0, 0, 4, -2,  128,  64,  128, 1
giFFTattributes3    ftgen    0, 0, 4, -2,  256, 128,  256, 1
giFFTattributes4    ftgen    0, 0, 4, -2,  512, 128,  512, 1
giFFTattributes5    ftgen    0, 0, 4, -2, 1024, 256, 1024, 1
giFFTattributes6    ftgen    0, 0, 4, -2, 2048, 512, 2048, 1
giFFTattributes7    ftgen    0, 0, 4, -2, 4096,1024, 4096, 1
giFFTattributes8    ftgen    0, 0, 4, -2, 8192,2048, 8192, 1

opcode    pvsbuffer_module,akk,akkkkkkiiii
    ain,kspeed,kbuflen,kscale,kfeedback,kmix,klev,iFFTsize,ioverlap,iwinsize,iwintype    xin
    kPhOffset    =    0
    ktrig        changed        kbuflen
    ibuflen    init    1
    kspeed    init    1
    kscale    init    1
    if ktrig==1 then
     reinit    UPDATE
    endif
    UPDATE:
    ibuflen        =    i(kbuflen)
    iphasor        ftgen        0, 0, 65536, 7, 0, 65536, 1            ;WAVE SHAPE FOR A MOVING PHASE POINTER
    aread         osciliktp     kspeed/ibuflen, iphasor, kPhOffset        ;CREATE MOVING POINTER TO READ FROM BUFFER
    kread        downsamp    aread
    kread        =        kread * ibuflen
    aFB        init        0
    f_anal         pvsanal        ain+aFB, iFFTsize, ioverlap, iwinsize, iwintype    ;ANALYSE THE AUDIO SIGNAL THAT WAS CREATED IN INSTRUMENT 1. OUTPUT AN F-SIGNAL.
    ibuffer,ktime      pvsbuffer       f_anal, ibuflen                    ;BUFFER FSIG    
    rireturn
    khandle        init         ibuffer                        ;INITIALISE HANDLE TO BUFFER
    f_buf          pvsbufread      kread , khandle            ;READ BUFFER
    f_scale        pvscale     f_buf, kscale                    ;RESCALE FREQUENCIES
    aresyn         pvsynth      f_scale                                   ;RESYNTHESIZE THE f-SIGNAL AS AN AUDIO SIGNAL    
    aFB        dcblock2    aresyn * kfeedback                ;CREATE FEEDBACK SIGNAL FOR NEXT PASS AND BLOCK DC OFFSET ACCUMULATION
    amix        ntrpol        ain, aresyn, kmix                ;CREATE DRY/WET MIX
            xout        amix*klev,ktime/ibuflen,kread/ibuflen
endop

instr    1
    ainL,ainR    ins
    ;ainL,ainR    diskin    "SynthPad.wav",1,0,1    ;USE FOR TESTING
    kspeed        chnget    "speed"
    kbuflen        chnget    "buflen"
    ksemis        chnget    "semis"
    kcents        chnget    "cents"
    ksemis        init    0
    kcents        init    0
    kscale        =    semitone(ksemis)*cent(kcents)
    kscale        init    1
    kbuflen        init    1
    kspeed        init    1
    kmix        chnget    "mix"
    kfeedback    chnget    "FB"
    klev        chnget    "lev"
    kmix        init    1
    kfeedback    init    0
    klev        init    0.5

    /* SET FFT ATTRIBUTES */
    katt_table    chnget    "att_table"    ; FFT atribute table
    katt_table    init    5
    ktrig        changed    katt_table
    if ktrig==1 then
     reinit update
    endif
    update:
    iFFTsize    table    0, giFFTattributes1 + i(katt_table) - 1
    ioverlap    table    1, giFFTattributes1 + i(katt_table) - 1
    iwinsize    table    2, giFFTattributes1 + i(katt_table) - 1
    iwintype    table    3, giFFTattributes1 + i(katt_table) - 1
    /*-------------------*/
    
    aoutL,kwrite,kread        pvsbuffer_module    ainL,kspeed,kbuflen,kscale,kfeedback,kmix,klev,iFFTsize,ioverlap,iwinsize,iwintype
    aoutR,k1,k1            pvsbuffer_module    ainR,kspeed,kbuflen,kscale,kfeedback,kmix,klev,iFFTsize,ioverlap,iwinsize,iwintype
            outs    aoutL,aoutR

    ktrig    metro    kr/(ksmps*2)
    if ktrig==1 then
     Smsg    sprintfk    "size(%d,3)",kwrite*490
     chnset    Smsg,"RecIndic" 
     Smsg    sprintfk    "size(%d,3)",kread*490
     chnset    Smsg,"PlayIndic" 
    endif
endin

</CsInstruments>
<CsScore>
i 1 0.1 [60*60*24*7]
</CsScore>
</CsoundSynthesizer>