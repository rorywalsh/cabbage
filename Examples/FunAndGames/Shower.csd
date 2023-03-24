
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; Shower
; Iain McCurdy, 2021

; Physical model of a shower including a squeaking tap

; TAP      - adjusts the intensity of the stream of water (density of drips)
; Duration - duration of the individual drips
; Pitch    - even though the pitches of the drips are slightly random, this controls the average pitch
; Spread   - random range of the pitches of drips

<Cabbage>
#define RSliderStyle  trackerInsideRadius(.9), trackerOutsideRadius(1), markerColour(100,100,255), markerThickness(2), trackerColour(130,130,255), trackerThickness(4), colour(60,65,80), fontColour(200,200,200), textColour(200,200,200), outlineColour(50,50,50), valuetextBox(1)
#define RSliderStyle2  trackerInsideRadius(.9), trackerOutsideRadius(1), markerColour(100,100,255), markerThickness(2), trackerColour(130,130,255), trackerThickness(4), colour(60,65,80), fontColour(200,200,200), textColour(200,200,200), outlineColour(50,50,50), valuetextBox(0)

form caption("Shower") size(510, 240), pluginId("Shwr") colour(20,25,30), guiRefresh(50)

; Drips
image      bounds( -10, -10,5,5), colour(205,205,255), widgetArray("Drip", 100), shape("ellipse")

rslider    bounds( 10 , 50, 120, 120), text("TAP"),  channel("DripsDens"),  range(0,600,3,0.5,0.1), popupText(0),  $RSliderStyle2

rslider    bounds(285, 80, 70, 70), text("Duration"),  channel("DripsDur"),  range(0.2,2,0.5,0.667,0.001),            $RSliderStyle
rslider    bounds(350, 80, 70, 70), text("Pitch"),  channel("DripsPitch"),  range(1000,8000,2000,0.5,1),            $RSliderStyle
rslider    bounds(415, 80, 70, 70), text("Spread"),  channel("DripsDiverg"),  range(0, 8, 1, 0.5, 0.01),            $RSliderStyle


</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-n -d -+rtmidi=NULL -M0 -m0d
</CsOptions>

<CsInstruments>

ksmps = 32
nchnls = 2
0dbfs = 1


giSfn    ftgen   0, 0, 262144, 10, 1               ; source waveform for the fof2 burbles (a sine wave)
giAttDec ftgen   0, 0, 262144, 16, 0, 262144, 4, 1   ; envelope shape for fof2 grains

instr  1

; READ IN WIDGETS
kDrips       chnget "Drips"
gkDripsDens  chnget "DripsDens"
kDripsDur    chnget "DripsDur"
kDripsDur    *=     0.01
kDripsDur    =      kDripsDur * 2 ^ (exprand:k(0.3))
kDripsPitch  chnget "DripsPitch"
kDripsDiverg chnget "DripsDiverg"


gkDripNum   init  1
kTrig dust 1, limit:k(gkDripsDens,0,50)
schedkwhen kTrig, 0,0, 2, 0, 1, gkDripNum

; DRIPS
#define DRIPS(PAN)
#
kFund   =        kDripsPitch       ; minimum burble pitch
kDiverg bexprnd    kDripsDiverg                ; divergence in octaves
kFund   *=       2 ^ kDiverg 


kdur    linrand  kDripsDur         ; random duration range of burbles
kdur    +=       kDripsDur*0.5      ; minimum burble duration

kris    =        kdur * 0.2
kdec    =        kdur * 0.8

kDens   randomi  gkDripsDens*0.5, gkDripsDens*2, gkDripsDens, 1 ; randomly wandering burble density

kAmp    linrand  6.4      ; random amplitude range

kGliss exprand  0.4 * 0.2       ; burble glissando
kGliss  +=       8 * 0.2      ; minimum burble glissando

kband   =    10 ;limit    1000*kDripsDur, 2, 20
;ares   fof2     xamp,                                        xfund, xform, koct, kband, kris, kdur,  kdec, iolaps, ifna,  ifnb,     itotdur, kphs, kgliss [, iskip]

aDripsle fof2     (ampdbfs(1.6)-ampdbfs(kAmp))*0.2, kDens, kFund, 0,    kband, kris, kdur,  kdec, 5000,    giSfn, giAttDec, 3600,    0,    kGliss
aL,aR   pan2     aDripsle, $PAN
        outs     aL, aR
        chnmix   aL, "SendL"
        chnmix   aR, "SendR"   
#
iPan = 0.2
$DRIPS(0.5-iPan)
$DRIPS(0.5+iPan)


; ROAR
#define ROAR(CHAN)
#
aRoar   dust2    rspline(2,2.4,2,3), rspline:k(1500,2000,2,3)*(gkDripsDens/600)      ; create some noise
aRoar   butbp    aRoar, 500, 500*5  ; bandpass filter the noise
kCF     rspline  1200, 2000, 4, 1 ; create a moving cutoff frequency for a lowpass filter
aRoar   tone     aRoar, kCF       ; lowpass filter the noise
outch $CHAN, aRoar*(gkDripsDens/600)
#
$ROAR(1)
$ROAR(2)



endin


; Draw drips
instr 2
kHeight line     0, p3, 245
iRange  trirand  10 * limit:i(i(gkDripsDens)^0.25, 0, 50)
iX      =        200
kX      transeg  iX, p3, -4, iX + iRange

iSize  random 2, 6

Smsg sprintfk "bounds(%d,%d,%d,%d)", kX, kHeight,iSize,iSize
SID  sprintfk "Drip_ident%d", p4
chnset Smsg, SID

gkDripNum init (p4 + 1) % 100
endin





; squeaky tap
instr    10
    
    kDripsDensPrev init   0
    kChange        =      abs(gkDripsDens - kDripsDensPrev)
    kDripsDensPrev delayk gkDripsDens, 0.1
    kChange        port   kChange,0.01
    kChange        limit  kChange, 0, 1
    
    if timeinsts()<0.2 then
     kChange       =      0
    endif
    
    /* BOW POSITION */
    kpos        =        0.2
    gkPosModDep =        0.4
    gkPosModRte =        9
    kPosMod     rspline  -gkPosModDep, gkPosModDep, gkPosModRte*0.5, gkPosModRte
    kpos        limit    kpos + kPosMod, 0, 1
    kpos        *=       kChange

    /* BOW PRESSURE */    
    kbowpres      =        12
    
    kcps          rspline    300,350,2,4
    gkgain        =          0.8
    gkconst       =          0.999
    abowedbar     wgbowedbar k(0), kcps, kpos, kbowpres, gkgain, i(gkconst), 0, 0, 20
        
    /* OUTPUT LEVEL CONTROL */
    abowedbar     *=         1.2 * kChange
    abowedbar     *=         oscili:a(1,333)
    abowedbar     butbp      abowedbar, 2000, 2000
    kRndDelL        rspline        0, 0.01, 0.4, 0.8
    kRndDelR        rspline        0, 0.01, 0.4, 0.8
    kRndDelL        limit        kRndDelL, 0, 0.01
    kRndDelR        limit        kRndDelR, 0, 0.01
    aRndDelL        interp        kRndDelL
    aRndDelR        interp        kRndDelR
    aL            vdelay        abowedbar, 0.1+(aRndDelL*1000), 0.1 + 10
    aR            vdelay        abowedbar, 0.1+(aRndDelR*1000), 0.1 + 10

                  outs       aL, aR
                  chnmix     aL, "SendL"
                  chnmix     aR, "SendR"   
endin








instr 99
aInL  chnget   "SendL"
aInR  chnget   "SendR"
      chnclear "SendL"
      chnclear "SendR"
aL,aR  reverbsc aInL,aInR,0.45,1500
      outs      aL,aR
endin

</CsInstruments>

<CsScore>
i 1 0 3600 ; play a long note...
i 10 0 0.1    ; INITIALISES THE ALGORITHM (OTHERWISE THE FIRST NOTE PLAYED DOESN'T SOUND)
i 10 1 3600
i 99 0 3600 ; play a long note...
f 0 z
</CsScore>

</CsoundSynthesizer>
