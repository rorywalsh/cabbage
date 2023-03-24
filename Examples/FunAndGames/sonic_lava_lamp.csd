/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; SonicLavaLamp.csd
; Written by Iain McCurdy, 2012.

<Cabbage>
form caption("Sonic Lava Lamp") size(510, 540), pluginId("sonl"), colour(50,50,50), guiMode("queue")
xypad bounds(  5,  5, 250, 250), channel("LFOspeed", "LFO_OS"),   rangeX(0, 1.00, 0.01), rangeY(5, 11, 8),     text("Speed/OS"), ballColour("White"), fontColour("White"), textColour("White")
xypad bounds(255,  5, 250, 250), channel("LFOshape", "LFOdepth"), rangeX(0, 1, 0.7),     rangeY(0.1, 4, 2),    text("Shape/Depth"), ballColour("Orange"), fontColour("Orange"), textColour("Orange")
xypad bounds(  5,260,250,250),   channel("pan", "amp"),           rangeX(0, 1, 0.5),     rangeY(0, 1, 0.8),    text("pan/amp"), ballColour("Pink"), fontColour("Pink"), textColour("Pink")
xypad bounds(255,260,250,250),   channel("DelTim", "DelSnd"),     rangeX(0, 1.00, 0.5),  rangeY(0, 1.00, 0.5), text("Delay Time/Send"), ballColour("Green"), fontColour("Green"), textColour("Green")
infobutton bounds(5,515, 100, 20), text("Help"), file("SonicLavaLampHelp.html")                     
</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-dm0 -n -+rtmidi=NULL -M0
</CsOptions>

<CsInstruments>

; sr set by host
ksmps    =     32
nchnls   =     2
0dbfs    =     1

;Author: Iain McCurdy (2012)

gisine       ftgen    0, 0, 4096, 10, 1
giSawUp      ftgen    0, 0, 4096, -7, -1, 4096, 1
giTri        ftgen    0, 0, 4096, -7,  -1, 2048, 1, 2048, -1
giSawDn      ftgen    0, 0, 4096, -7, 1, 4096, -1
giSq         ftgen    0, 0, 4096, -7, 1, 2048, 1, 0, -1, 2048, -1
giMorfTab    ftgen    0, 0, 4096, -7, 1, 2048, 1, 0, -1, 2048, -1
giTabs4Morf  ftgen    0, 0, 8, -2, giSq, giSawUp, giTri, giSawDn, giSq, giSq, giSq, giSq
giAmpCurve   ftgen    0, 0, 16, -16,   1, 6, 0, 1,   5, -4, 0.2,   5, 0, 0.2
giEnv        ftgen    0, 0, 4096, 20,3,1    ;9, 0.5, 1, 0

opcode    PingPongDelay, aa,aakk
    ainL,ainR,ktime,kfeedback  xin
    atime        interp        ktime                              ; INTERPOLATED A-RATE VERSION OF DELAY TIME
    aL_OS        vdelay        ainL,(atime*1000)/2,(10*1000)/2    ; DELAYED OFFSET OF LEFT CHANNEL (FIRST 'PING')
    ;LEFT CHANNEL
    abuf         delayr        10                                 ; ESTABLISH DELAY BUFFER
    aDelL        deltapi       atime                              ; TAP BUFFER
                 delayw        aL_OS+(aDelL*kfeedback)            ; WRITE INPUT AUDIO INTO BUFFER
    ;RIGHT CHANNEL
    abuf         delayr        10                                 ; ESTABLISH DELAY BUFFER
    aDelR        deltapi       atime                              ; TAP BUFFER
                 delayw        ainR+(aDelR*kfeedback)             ; WRITE INPUT AUDIO INTO BUFFER
                 xout          aDelL+aL_OS,aDelR
endop

instr    1
    kLFOspeed    cabbageGetValue        "LFOspeed"
    kLFO_OS      cabbageGetValue        "LFO_OS"
    kLFOshape    cabbageGetValue        "LFOshape"
    kLFOdepth    cabbageGetValue        "LFOdepth"
    kamp         cabbageGetValue        "amp"
    kpan         cabbageGetValue        "pan"
    kDelTim      cabbageGetValue        "DelTim"
    kDelSnd      cabbageGetValue        "DelSnd"

    kLFOspeed    expcurve      kLFOspeed, 6
    kLFOspeed    scale         kLFOspeed, 40, 0.01
    kDelTim      expcurve      kDelTim,6
    kDelTim      scale         kDelTim, 0.5, 0.01

    kporttime    linseg        0, 0.01, 0.03
    kDelSnd      portk         kDelSnd, kporttime * 3
    kLFOdepth    portk         kLFOdepth, kporttime
    kLFO_OS      portk         kLFO_OS, kporttime
    kLFOshape    portk         kLFOshape, kporttime
    kamp         portk         kamp, kporttime
    kamp         expcurve      kamp, 2
    kpan         portk         kpan, kporttime
                 ftmorf        kLFOshape * 5, giTabs4Morf, giMorfTab
    aoctLFO      oscili        kLFOdepth, kLFOspeed, giMorfTab
    aoct         =             aoctLFO + kLFO_OS
    koct         downsamp      aoct
    kAmpCurve    tablei        koct, giAmpCurve
    kcps         =             cpsoct(koct)
    aamp         interp        kAmpCurve / 3
    asig         oscili        aamp, tone:a(a(kcps),50), gisine    ;AUDIO OSCILLATOR
    aenv         oscili        1,kLFOspeed,giEnv
    kEnvXFade    jspline       1, 0.2, 0.6
    kEnvXFade    limit         kEnvXFade, 0,1
    asig         ntrpol        asig * aenv, asig, kEnvXFade

    
    aL,aR        pan2          asig, kpan
    aRvbL,aRvbR  reverbsc      aL, aR, 0.8, 4000
    aL           sum           (aL * kamp), (aRvbL * 0.1)
    aR           sum           (aR * kamp), (aRvbR * 0.1)
    kDelTim      portk         kDelTim, kporttime * 10
    aDlyL,aDlyR  PingPongDelay aL * kDelSnd, aR * kDelSnd, 2 * kDelTim, 0.6
    aL           sum           aL, (aDlyL * 0.5)
    aR           sum           aR, (aDlyR * 0.5)

                 outs          aL, aR
endin

</CsInstruments>

<CsScore>
i 1 0.2 [3600*24*7]
</CsScore>

</CsoundSynthesizer>