
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; TempoDelay.csd
; Written by Iain McCurdy, 2012.

; Units for the delay are assumed to be demi-semiquavers.
; Knob for Rhy.Mult. will be replaced with a combobox once comboboxes work in plugins within hosts.
; Width control only applicable when ping-pong delay selected.
; If 'external' is selected as clock source tempo is taken from the host's BPM. 

<Cabbage>
form caption("Tempo Delay") size(565, 90), pluginId("TDel")
image pos(0, 0), size(565, 90), colour("LightBlue"), shape("rounded"), outlineColour("white"), outlineThickness(4) 
rslider bounds(10, 11, 70, 70), text("Tempo"),         textColour("black"),         channel("tempo"),     range(40, 500, 90, 1, 1),   colour(100,100,255),trackerColour(100,100,150)
rslider bounds(75, 11, 70, 70), text("Rhy.Mult."),    textColour("black"),         channel("RhyMlt"),     range(1, 16, 4, 1, 1),      colour(100,100,255),trackerColour(100,100,150)
rslider bounds(140, 11, 70, 70), text("Damping"),     textColour("black"),         channel("damp"),     range(20,20000, 20000,0.5), colour(100,100,255),trackerColour(100,100,150)
rslider bounds(205, 11, 70, 70), text("Feedback"),     textColour("black"),         channel("fback"),     range(0, 1.00, 0.8),        colour(100,100,255),trackerColour(100,100,150)
rslider bounds(270, 11, 70, 70), text("Width"),    textColour("black"),             channel("width"),     range(0,  1.00, 1),         colour(100,100,255),trackerColour(100,100,150)
label  bounds(342,  10, 75, 12), text("Clock Source"), fontColour("black")
button bounds(340,  22, 80, 20), text("Internal","External"), channel("ClockSource"), value(0), fontColour:0("yellow"), fontColour:1("yellow")
label  bounds(345,  45, 70, 12), text("Delay Type"), fontColour("black")
button bounds(340,  57, 80, 20), text("Simple","Ping-pong"), channel("DelType"), value(1), fontColour:0("yellow"), fontColour:1("yellow")
rslider bounds(420, 11, 70, 70), text("Mix"),         textColour("black"),         channel("mix"),     range(0, 1.00, 0.5), colour(100,100,255),trackerColour(100,100,150)
rslider bounds(485, 11, 70, 70), text("Level"),        textColour("black"),         channel("level"),     range(0, 1.00, 1),   colour(100,100,255),trackerColour(100,100,150)
}
</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-d -n
</CsOptions>

<CsInstruments>

; sr set by host
ksmps         =     32    ;NUMBER OF AUDIO SAMPLES IN EACH CONTROL CYCLE
nchnls         =     2    ;NUMBER OF CHANNELS (2=STEREO)
0dbfs        =    1

;Author: Iain McCurdy (2012)

instr    1
    kfback        chnget    "fback"                ;read in widgets
    kdamp        chnget    "damp"                ;
    kmix        chnget    "mix"                ;
    klevel        chnget    "level"                ;
    kbpm        chnget    "HOST_BPM"            ;
    kRhyMlt        chnget    "RhyMlt"            ;
    kClockSource    chnget    "ClockSource"            ;
    kDelType    chnget    "DelType"            ;
    kwidth        chnget    "width"                ;
    if kClockSource==0 then                ;if internal clock source has been chosen...
     ktempo    chnget    "tempo"                ;tempo taken from GUI knob control
    else
     ktempo    chnget    "bpm"                ;tempo taken from host BPM
     ktempo    limit    ktempo,40,500            ;limit range of possible tempo values. i.e. a tempo of zero would result in a delay time of infinity.
    endif

    ktime    divz    (60*kRhyMlt),(ktempo*8),0.1        ;derive delay time. 8 in the denominator indicates that kRhyMult will be in demisemiquaver divisions
    atime    interp    ktime                ;interpolate k-rate delay time to create an a-rate version which will give smoother results when tempo is modulated
    
    ainL,ainR    ins                ;read stereo inputs
    
    if kDelType==0 then                ;if 'simple' delay type is chosen...
     abuf    delayr    5
     atapL    deltap3    atime
     atapL    tone    atapL,kdamp
        delayw    ainL+(atapL*kfback)

     abuf    delayr    5
     atapR    deltap3    atime
     atapR    tone    atapR,kdamp
        delayw    ainR+(atapR*kfback)    
    else                        ;otherwise 'ping-pong' delay type must have been chosen
     ;offset delay (no feedback)
     abuf    delayr    5
     afirst    deltap3    atime
     afirst    tone    afirst,kdamp
        delayw    ainL

     ;left channel delay (note that 'atime' is doubled) 
     abuf    delayr    10            ;
     atapL    deltap3    atime*2
     atapL    tone    atapL,kdamp
        delayw    afirst+(atapL*kfback)

     ;right channel delay (note that 'atime' is doubled) 
     abuf    delayr    10
     atapR    deltap3    atime*2
     atapR    tone    atapR,kdamp;        delayw    ainR+(atapR*kfback)
        delayw    ainR+(atapR*kfback)
    
     ;create width control. note that if width is zero the result is the same as 'simple' mode
     atapL    =    afirst+atapL+(atapR*(1-kwidth))
     atapR    =    atapR+(atapL*(1-kwidth))

    endif
    
    amixL        ntrpol        ainL, atapL, kmix    ;CREATE A DRY/WET MIX BETWEEN THE DRY AND THE EFFECT SIGNAL
    amixR        ntrpol        ainR, atapR, kmix    ;CREATE A DRY/WET MIX BETWEEN THE DRY AND THE EFFECT SIGNAL
            outs        amixL * klevel, amixR * klevel
endin
        
</CsInstruments>

<CsScore>
i 1 0 [3600*24*7]
</CsScore>

</CsoundSynthesizer>
