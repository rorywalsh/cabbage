
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; MultiModeDelay.csd
; Written by Iain McCurdy, 2012.

<Cabbage>
form caption("Multi-mode Delay") size(435, 90), pluginId("MMDl")
image                    bounds(0, 0,435, 90), colour("Maroon"), shape("rounded"), outlineColour("white"), outlineThickness(4)
label    bounds( 10, 22, 80, 12), text("Type"), fontColour("white")
combobox bounds( 10, 35, 80, 20),  text("Ping Pong","Stereo"), channel("type"), value(1)
rslider  bounds( 95, 11, 70, 70),  text("Time"),     channel("time"),     range(0.001, 10, 0.4, 0.5), colour( 85,  0,  0) trackerColour("silver"), textColour("silver")
rslider  bounds(160, 11, 70, 70),  text("Cutoff"),   channel("cutoff"),   range(20,20000,20000,0.5),  colour( 85,  0,  0) trackerColour("silver"), textColour("silver")
rslider  bounds(225, 11, 70, 70), text("Feedback"), channel("feedback"), range(0, 1.00, 0.5),        colour( 85,  0,  0) trackerColour("silver"), textColour("silver")
rslider  bounds(290, 11, 70, 70), text("Mix"),      channel("mix"),      range(0, 1.00, 0.5),        colour( 85,  0,  0) trackerColour("silver"), textColour("silver")
rslider  bounds(355, 11, 70, 70), text("Level"),    channel("level"),    range(0, 1.00, 0.7),        colour( 85,  0,  0) trackerColour("silver"), textColour("silver")

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
    ktime        chnget    "time"                    ;READ WIDGETS...
    kcutoff        chnget    "cutoff"                ;
    kfeedback    chnget    "feedback"                ;
    kmix        chnget    "mix"                    ;
    klevel        chnget    "level"                    ;
    asigL, asigR    ins
    kporttime    linseg    0,0.01,0.03                ;CREATE A VARIABLE THAT WILL BE USED FOR PORTAMENTO TIME
    ktime        portk    ktime,kporttime                ;PORTAMENTO SMOOTHING OF DELAT TIME
    atime        interp    ktime                    ;INTERPOLATED A-RATE VERSION OF DELAY TIME
    ktype        chnget    "type"
    if ktype==1 then
     aL_OS        vdelay    asigL,(atime*1000)/2,(10*1000)/2    ;DELAYED OFFSET OF LEFT CHANNEL (FIRST 'PING')
     ;LEFT CHANNEL
     abuf        delayr    10                    ;ESTABLISH DELAY BUFFER
     aDelL        deltapi    atime                    ;TAP BUFFER
     aDelL        tone    aDelL,kcutoff                ;LOWPASS FILTER DELAY TAP 
             delayw    aL_OS+(aDelL*kfeedback)            ;WRITE INPUT AUDIO INTO BUFFER
     ;RIGHT CHANNEL
     abuf        delayr    10                    ;ESTABLISH DELAY BUFFER
     aDelR        deltapi    atime                    ;TAP BUFFER
     aDelR        tone    aDelR,kcutoff                ;LOWPASS FILTER DELAY TAP
             delayw    asigR+(aDelR*kfeedback)            ;WRITE INPUT AUDIO INTO BUFFER
     amixL        ntrpol    asigL,aDelL+aL_OS,kmix            ;MIX DRY AND WET SIGNALS (LEFT CHANNEL)
     amixR        ntrpol    asigR,aDelR,kmix            ;MIX DRY AND WET SIGNALS (RIGHT CHANNEL)
    elseif ktype==2 then
     ;LEFT CHANNEL
     abuf        delayr    10                    ;ESTABLISH DELAY BUFFER
     aDelL        deltapi    atime                    ;TAP BUFFER
     aDelL        tone    aDelL,kcutoff                ;LOWPASS FILTER DELAY TAP 
             delayw    asigL+(aDelL*kfeedback)            ;WRITE INPUT AUDIO INTO BUFFER
     ;RIGHT CHANNEL
     abuf        delayr    10                    ;ESTABLISH DELAY BUFFER
     aDelR        deltapi    atime                    ;TAP BUFFER
     aDelR        tone    aDelR,kcutoff                ;LOWPASS FILTER DELAY TAP
             delayw    asigR+(aDelR*kfeedback)            ;WRITE INPUT AUDIO INTO BUFFER
     amixL        ntrpol    asigL,aDelL,kmix            ;MIX DRY AND WET SIGNALS (LEFT CHANNEL)
     amixR        ntrpol    asigR,aDelR,kmix            ;MIX DRY AND WET SIGNALS (RIGHT CHANNEL)         
    endif
            outs    amixL*klevel, amixR*klevel        ;PING PONG DELAY OUTPUTS ARE SENT OUT
endin
        
</CsInstruments>

<CsScore>
i 1 0 [3600*24*7]
</CsScore>

</CsoundSynthesizer>