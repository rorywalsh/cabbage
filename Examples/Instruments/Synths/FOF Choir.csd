
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; FOF Choir.csd

; Note velocity is interpretted as attack time (along with a slight interpretation as amplitude)
; If N.Voices (number of voices) is set to '1' chorussing effect is bypassed, instead a fundamental modulation mechanism is enabled
; Vibrato/tremolo depth also controllable using midi controller 1 (mod. wheel), midi channel 1
; Vowel is controllable using midi controller 2, midi channel 1
; N.Voices value is not strictly speaking accurate:     1 = 1 voice
;                            2 = 2 voices
;                            3 = 4 voices
;                            4 = 6 voices
;                            5 = 8 voices, this is on account of how the mechanism implements a stereo effect

; The frequencies, bandwidths and decibel levels for the 5 formants are shown in a grid of number boxes

; Frequency Scale - scales the frequencies of all formants
; Bandwidth Scale - scales the bandwidth of all formants

; the five vertical sliders scale the amplitudes of all formants

<Cabbage>
form caption("FOF Choir") size(770, 465), pluginId("choi"), guiRefresh(16)

; main container
image bounds(  0,  0,770, 465), colour(15,20,20), shape("rounded"), outlineColour("white"), outlineThickness(3)

xypad    bounds( 10, 10,250,250), channel("vowel", "oct"), text("X:Vowel Y:Oct.Div."), rangeX(0, 1, 0), rangeY(0, 4, 0)

combobox bounds(265, 30,110, 25), channel("voice"), value(5), text("Bass", "Tenor", "Countertenor", "Alto", "Soprano")
button   bounds(265, 60,110, 25), text("polyphonic","monophonic"), channel("monopoly"), value(1)

rslider  bounds(385, 20, 60, 60), text("Leg.Time"),     channel("LegTim"),  range(0.005, 0.3, 0.025, 0.5, 0.005),colour("green"), trackerColour("white")
rslider  bounds(445, 20, 60, 60), text("Vowel"),        channel("vowel"),   range(0, 1.00, 0),       colour("green"),    trackerColour("white"),midCtrl(1, 2)
rslider  bounds(505, 20, 60, 60), text("Level"),        channel("lev"),     range(0, 5.00, 0.6),     colour("green"),    trackerColour("white")

rslider  bounds(265,100, 60, 60), text("Vib.Dep."),     channel("vibdep"),  range(0, 2.00, 0.35),    colour("tomato"),   trackerColour("white"),midCtrl(1, 1)
rslider  bounds(325,100, 60, 60), text("Trem.Dep."),    channel("trmdep"),  range(0, 1.00, 0.2),     colour("tomato"),   trackerColour("white"),midCtrl(1, 1)
rslider  bounds(385,100, 60, 60), text("Mod.Rate"),     channel("modrte"),  range(0.10,20, 5, 0.5),  colour("tomato"),   trackerColour("white")
rslider  bounds(445,100, 60, 60), text("Mod.Delay"),    channel("moddel"),  range(0, 2.00, 0.3, 0.5),colour("tomato"),   trackerColour("white")
rslider  bounds(505,100, 60, 60), text("Mod.Rise"),     channel("modris"),  range(0, 4.00, 2, 0.5),  colour("tomato"),   trackerColour("white")

rslider  bounds(265,180, 60, 60), text("N.Voices"),     channel("nvoices"), range(1,  50, 6, 1, 1),  colour("yellow"),   trackerColour("white")
rslider  bounds(325,180, 60, 60), text("Dtn.Dep."),     channel("DtnDep"),  range(0,    4.00, 1,0.5),colour("yellow"),   trackerColour("white")
rslider  bounds(385,180, 60, 60), text("Dtn.Rate"),     channel("DtnRte"),  range(0.01,40, 0.2,0.25),colour("yellow"),   trackerColour("white")
rslider  bounds(445,180, 60, 60), text("Rvb.Mix"),      channel("RvbMix"),  range(0, 1.00, 0.15),    colour("steelblue"),trackerColour("white")
rslider  bounds(505,180, 60, 60), text("Rvb.Size"),     channel("RvbSize"), range(0.5, 1.00, 0.82,2),colour("steelblue"),trackerColour("white")

checkbox bounds(570, 10,190, 20), text("Filter On/Off") channel("FiltOnOff"), colour("lime")  value(0)
xypad    bounds(570, 35,190,250), channel("cf", "bw"), text("x:c.off/y:b.width"), rangeX(5, 13, 8), rangeY(0.1, 5, 0.3)

image bounds(0,265,770,200), colour(0,0,0,0)
{
nslider  bounds( 10, 10, 60, 25), text("Freq.1"), channel("f1"), range(0, 20000, 0, 1,1), active(0)
nslider  bounds( 10, 40, 60, 25), text("dB.1"), channel("dB1"), range(-120, 0, 0, 1,1), active(0)
nslider  bounds( 10, 70, 60, 25), text("BW.1"), channel("BW1"), range(0, 1000, 0, 1,1), active(0)

nslider  bounds( 80, 10, 60, 25), text("Freq.2"), channel("f2"), range(0, 20000, 0, 1,1), active(0)
nslider  bounds( 80, 40, 60, 25), text("dB.2"), channel("dB2"), range(-120, 0, 0, 1,1), active(0)
nslider  bounds( 80, 70, 60, 25), text("BW.2"), channel("BW2"), range(0, 1000, 0, 1,1), active(0)

nslider  bounds(150, 10, 60, 25), text("Freq.3"), channel("f3"), range(0, 20000, 0, 1,1), active(0)
nslider  bounds(150, 40, 60, 25), text("dB.3"), channel("dB3"), range(-120, 0, 0, 1,1), active(0)
nslider  bounds(150, 70, 60, 25), text("BW.3"), channel("BW3"), range(0, 1000, 0, 1,1), active(0)

nslider  bounds(220, 10, 60, 25), text("Freq.4"), channel("f4"), range(0, 20000, 0, 1,1), active(0)
nslider  bounds(220, 40, 60, 25), text("dB.4"), channel("dB4"), range(-120, 0, 0, 1,1), active(0)
nslider  bounds(220, 70, 60, 25), text("BW.4"), channel("BW4"), range(0, 1000, 0, 1,1), active(0)

nslider  bounds(290, 10, 60, 25), text("Freq.5"), channel("f5"), range(0, 20000, 0, 1,1), active(0)
nslider  bounds(290, 40, 60, 25), text("dB.5"), channel("dB5"), range(-120, 0, 0, 1,1), active(0)
nslider  bounds(290, 70, 60, 25), text("BW.5"), channel("BW5"), range(0, 1000, 0, 1,1), active(0)

rslider  bounds(370, 10, 80, 80), text("Frequency Scale"),     channel("FormScale"), range(0.25, 4.00, 1, 0.5, 0.001), colour(200,200,150), trackerColour("white")
rslider  bounds(470, 10, 80, 80), text("Bandwidth Scale"),   channel("BWScale"), range(0.5, 8.00, 1, 0.5, 0.001), colour(200,200,150), trackerColour("white")

vslider  bounds(600, 25, 20, 70), channel("FAmp1"), range(0, 1.00, 1, 0.5, 0.001), colour(200,200,150), trackerColour("white")
vslider  bounds(630, 25, 20, 70), channel("FAmp2"), range(0, 1.00, 1, 0.5, 0.001), colour(200,200,150), trackerColour("white")
vslider  bounds(660, 25, 20, 70), channel("FAmp3"), range(0, 1.00, 1, 0.5, 0.001), colour(200,200,150), trackerColour("white")
vslider  bounds(690, 25, 20, 70), channel("FAmp4"), range(0, 1.00, 1, 0.5, 0.001), colour(200,200,150), trackerColour("white")
vslider  bounds(720, 25, 20, 70), channel("FAmp5"), range(0, 1.00, 1, 0.5, 0.001), colour(200,200,150), trackerColour("white")

label    bounds(600, 90, 20, 12), text("1"), align("centre")
label    bounds(630, 90, 20, 12), text("2"), align("centre")
label    bounds(660, 90, 20, 12), text("3"), align("centre")
label    bounds(690, 90, 20, 12), text("4"), align("centre")
label    bounds(720, 90, 20, 12), text("5"), align("centre")
}

keyboard pos(10, 375), size(750, 80)
</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-dm0 -n -+rtmidi=NULL -M0
</CsOptions>

<CsInstruments>

; sr set by host
ksmps     =     64
nchnls     =     2
0dbfs    =    1
massign    0,2
seed    0

;Author: Iain McCurdy (2012)

gisine        ftgen    0, 0, 4096, 10, 1                ;SINE WAVE
giexp        ftgen    0, 0, 1024, 19, 0.5, 0.5, 270, 0.5        ;EXPONENTIAL CURVE USED TO DEFINE THE ENVELOPE SHAPE OF FOF PULSES
gasendL,gasendR    init    0

;FUNCTION TABLES STORING DATA FOR VARIOUS VOICE FORMANTS
;THE FIRST VALUE OF EACH TABLE DEFINES THE NUMBER OF DATA ELEMENTS IN THE TABLE
;THIS IS NEEDED BECAUSE TABLES SIZES MUST BE POWERS OF 2 TO FACILITATE INTERPOLATED TABLE READING (tablei) 
;BASS
giBF1        ftgen    0,  0, 8, -2,    4, 600,        400,    250,    350    ;FREQ
giBF2        ftgen    0,  0, 8, -2,    4, 1040,    1620,    1750,    600    ;FREQ
giBF3        ftgen    0,  0, 8, -2,    4, 2250,    2400,    2600,    2400    ;FREQ
giBF4        ftgen    0,  0, 8, -2,    4, 2450,    2800,    3050,    2675    ;FREQ
giBF5        ftgen    0,  0, 8, -2,    4, 2750,    3100,    3340,    2950    ;FREQ
                
giBDb1        ftgen    0, 0, 8, -2,    4, 0,    0,    0,    0    ;dB
giBDb2        ftgen    0, 0, 8, -2,    4, -7,    -12,    -30,    -20    ;dB
giBDb3        ftgen    0, 0, 8, -2,    4, -9,    -9,    -16,    -32    ;dB
giBDb4        ftgen    0, 0, 8, -2,    4, -9,    -12,    -22,    -28    ;dB
giBDb5        ftgen    0, 0, 8, -2,    4, -20,    -18,    -28,    -36    ;dB
                
giBBW1        ftgen    0, 0, 8, -2,    4, 60,    40,    60,    40    ;BAND WIDTH
giBBW2        ftgen    0, 0, 8, -2,    4, 70,    80,    90,    80    ;BAND WIDTH
giBBW3        ftgen    0, 0, 8, -2,    4, 110,    100,    100,    100    ;BAND WIDTH
giBBW4        ftgen    0, 0, 8, -2,    4, 120,    120,    120,    120    ;BAND WIDTH
giBBW5        ftgen    0, 0, 8, -2,    4, 130,    120,    120,    120    ;BAND WIDTH
;TENOR      
giTF1        ftgen    0, 0, 8, -2,    5, 650,     400,    290,    400,    350    ;FREQ
giTF2        ftgen    0, 0, 8, -2,    5, 1080,     1700,   1870,    800,    600    ;FREQ
giTF3        ftgen    0, 0, 8, -2,    5, 2650,    2600,   2800,    2600,    2700    ;FREQ
giTF4        ftgen    0, 0, 8, -2,    5, 2900,    3200,   3250,    2800,    2900    ;FREQ
giTF5        ftgen    0, 0, 8, -2,    5, 3250,    3580,   3540,    3000,    3300    ;FREQ
                
giTDb1        ftgen    0, 0, 8, -2,    5, 0,    0,    0,    0,    0    ;dB
giTDb2        ftgen    0, 0, 8, -2,    5, -6,    -14,    -15,    -10,    -20    ;dB
giTDb3        ftgen    0, 0, 8, -2,    5, -7,    -12,    -18,    -12,    -17    ;dB
giTDb4        ftgen    0, 0, 8, -2,    5, -8,    -14,    -20,    -12,    -14    ;dB
giTDb5        ftgen    0, 0, 8, -2,    5, -22,    -20,    -30,    -26,    -26    ;dB
                
giTBW1        ftgen    0, 0, 8, -2,    5, 80,    70,    40,    40,    40    ;BAND WIDTH
giTBW2        ftgen    0, 0, 8, -2,    5, 90,    80,    90,    80,    60    ;BAND WIDTH
giTBW3        ftgen    0, 0, 8, -2,    5, 120,    100,    100,    100,    100    ;BAND WIDTH
giTBW4        ftgen    0, 0, 8, -2,    5, 130,    120,    120,    120,    120    ;BAND WIDTH                                         
giTBW5        ftgen    0, 0, 8, -2,    5, 140,    120,    120,    120,    120    ;BAND WIDTH
;COUNTER TENOR
giCTF1        ftgen    0, 0, 8, -2,    5, 660,    440,    270,    430,    370        ;FREQ
giCTF2        ftgen    0, 0, 8, -2,    5, 1120,    1800,    1850,    820,    630    ;FREQ
giCTF3        ftgen    0, 0, 8, -2,    5, 2750,    2700,    2900,    2700,    2750    ;FREQ
giCTF4        ftgen    0, 0, 8, -2,    5, 3000,    3000,    3350,    3000,    3000    ;FREQ
giCTF5        ftgen    0, 0, 8, -2,    5, 3350,    3300,    3590,    3300,    3400    ;FREQ
                
giTBDb1        ftgen    0, 0, 8, -2,    5, 0,    0,    0,    0,    0    ;dB
giTBDb2        ftgen    0, 0, 8, -2,    5, -6,    -14,    -24,    -10,    -20    ;dB
giTBDb3        ftgen    0, 0, 8, -2,    5, -23,    -18,    -24,    -26,    -23    ;dB
giTBDb4        ftgen    0, 0, 8, -2,    5, -24,    -20,    -36,    -22,    -30    ;dB
giTBDb5        ftgen    0, 0, 8, -2,    5, -38,    -20,    -36,    -34,    -30    ;dB
                
giTBW1        ftgen    0, 0, 8, -2,    5, 80,    70,    40,    40,    40    ;BAND WIDTH
giTBW2        ftgen    0, 0, 8, -2,    5, 90,    80,    90,    80,    60    ;BAND WIDTH
giTBW3        ftgen    0, 0, 8, -2,    5, 120,    100,    100,    100,    100    ;BAND WIDTH
giTBW4        ftgen    0, 0, 8, -2,    5, 130,    120,    120,    120,    120    ;BAND WIDTH
giTBW5        ftgen    0, 0, 8, -2,    5, 140,    120,    120,    120,    120    ;BAND WIDTH
;ALTO       
giAF1        ftgen    0, 0, 8, -2,    5, 800,    400,    350,    450,    325        ;FREQ
giAF2        ftgen    0, 0, 8, -2,    5, 1150,    1600,    1700,    800,    700    ;FREQ
giAF3        ftgen    0, 0, 8, -2,    5, 2800,    2700,    2700,    2830,    2530    ;FREQ
giAF4        ftgen    0, 0, 8, -2,    5, 3500,    3300,    3700,    3500,    2500    ;FREQ
giAF5        ftgen    0, 0, 8, -2,    5, 4950,    4950,    4950,    4950,    4950    ;FREQ
                
giADb1        ftgen    0, 0, 8, -2,    5, 0,    0,    0,    0,    0    ;dB
giADb2        ftgen    0, 0, 8, -2,    5, -4,    -24,    -20,    -9,    -12    ;dB
giADb3        ftgen    0, 0, 8, -2,    5, -20,    -30,    -30,    -16,    -30    ;dB
giADb4        ftgen    0, 0, 8, -2,    5, -36,    -35,    -36,    -28,    -40    ;dB
giADb5        ftgen    0, 0, 8, -2,    5, -60,    -60,    -60,    -55,    -64    ;dB
                
giABW1        ftgen    0, 0, 8, -2,    5, 50,    60,    50,    70,    50    ;BAND WIDTH
giABW2        ftgen    0, 0, 8, -2,    5, 60,    80,    100,    80,    60    ;BAND WIDTH
giABW3        ftgen    0, 0, 8, -2,    5, 170,    120,    120,    100,    170    ;BAND WIDTH
giABW4        ftgen    0, 0, 8, -2,    5, 180,    150,    150,    130,    180    ;BAND WIDTH
giABW5        ftgen    0, 0, 8, -2,    5, 200,    200,    200,    135,    200    ;BAND WIDTH
;SOPRANO
giSF1        ftgen    0, 0, 8, -2,    5, 800,    350,    270,    450,    325        ;FREQ
giSF2        ftgen    0, 0, 8, -2,    5, 1150,    2000,    2140,    800,    700    ;FREQ
giSF3        ftgen    0, 0, 8, -2,    5, 2900,    2800,    2950,    2830,    2700    ;FREQ
giSF4        ftgen    0, 0, 8, -2,    5, 3900,    3600,    3900,    3800,    3800    ;FREQ
giSF5        ftgen    0, 0, 8, -2,    5, 4950,    4950,    4950,    4950,    4950    ;FREQ
                
giSDb1        ftgen    0, 0, 8, -2,    5, 0,    0,    0,    0,    0    ;dB
giSDb2        ftgen    0, 0, 8, -2,    5, -6,    -20,    -12,    -11,    -16    ;dB
giSDb3        ftgen    0, 0, 8, -2,    5, -32,    -15,    -26,    -22,    -35    ;dB
giSDb4        ftgen    0, 0, 8, -2,    5, -20,    -40,    -26,    -22,    -40    ;dB
giSDb5        ftgen    0, 0, 8, -2,    5, -50,    -56,    -44,    -50,    -60    ;dB
                
giSBW1        ftgen    0, 0, 8, -2,    5, 80,    60,    60,    70,    50    ;BAND WIDTH
giSBW2        ftgen    0, 0, 8, -2,    5, 90,    90,    90,    80,    60    ;BAND WIDTH
giSBW3        ftgen    0, 0, 8, -2,    5, 120,    100,    100,    100,    170    ;BAND WIDTH
giSBW4        ftgen    0, 0, 8, -2,    5, 130,    150,    120,    130,    180    ;BAND WIDTH
giSBW5        ftgen    0, 0, 8, -2,    5, 140,    200,    120,    135,    200    ;BAND WIDTH

gkactive    init    0    ; Will contain number of active instances of instr 3 when legato mode is chosen. NB. notes in release stage will not be regarded as active. 

opcode         fofx5, a, kkkkkkkkkki
    kfund,kvowel,koct,kFormScale,kBWScale,kFAmp1,kFAmp2,kFAmp3,kFAmp4,kFAmp5,ivoice    xin
        
    ivoice        limit        ivoice,0,4                    ;protect against out of range values for ivoice
    ;create a macro for each formant to reduce code repetition
#define    FORMANT(N)
    #
    invals    table    0, giBF1+(ivoice*15)+$N-1                    ;number of data elements in each table
    invals        =        invals-1                                ;
    k$N.form     tablei        1+(kvowel*invals), giBF1+(ivoice*15)+$N-1    ;read formant frequency from table
    k$N.form     *=        kFormScale
    k$N.db         tablei        1+(kvowel*invals), giBDb1+(ivoice*15)+$N-1    ;read decibel value from table
    k$N.amp        =        ampdb(k$N.db)                    ;convert to an amplitude value                                                
    k$N.band     tablei        1+(kvowel*invals), giBBW1+(ivoice*15)+$N-1    ;read bandwidth from table
    k$N.band     *=           kBWScale
    if changed:k(k$N.form,k$N.db,k$N.band)==1 && active:i(p1)==1 then
     Schan      sprintfk "f%d",k($N)
     chnset k$N.form, Schan
     Schan      sprintfk "dB%d",k($N)
     chnset k$N.db, Schan
     Schan      sprintfk "BW%d",k($N)
     chnset k$N.band, Schan
    endif
    ;kRandForm$N    randomi    -0.025,0.025,8,1    
    ;k$N.form    limit    k$N.form*octave(kRandForm$N),0,1000
    #
    ;EXECUTE MACRO MULTIPLE TIMES
    $FORMANT(1)                                                                                      
    $FORMANT(2)                                                                                      
    $FORMANT(3)                                                                                        
    $FORMANT(4)
    $FORMANT(5)
    ;======================================================================================================================================================================
    iris        =        0.003    ;grain pulse rise time
    idur        =        0.02    ;grain pulse duration
    idec        =        0.007    ;grain pulse decay
    iolaps        =        14850    ;maximum number of overlaps (overestimate)
    ifna        =        gisine    ;function table for audio contained within fof grains
    ifnb        =        giexp    ;function table that defines the attack and decay shapes of each fof grain
    itotdur        =        3600    ;total maximum duration of a note (overestimate)
    ;FOF===================================================================================================================================================================
    iRandRange    =        .1
#define    RandFact
    #
    kRndFact    rspline        -iRandRange,iRandRange,1,10
    kRndFact    =        semitone(kRndFact)
    #
    $RandFact
    a1         fof         k1amp, kfund*kRndFact, k1form, koct, k1band, iris, idur, idec, iolaps, ifna, ifnb, itotdur
    a2         fof         k2amp, kfund*kRndFact, k2form, koct, k2band, iris, idur, idec, iolaps, ifna, ifnb, itotdur
    a3         fof         k3amp, kfund*kRndFact, k3form, koct, k3band, iris, idur, idec, iolaps, ifna, ifnb, itotdur
    a4         fof         k4amp, kfund*kRndFact, k4form, koct, k4band, iris, idur, idec, iolaps, ifna, ifnb, itotdur
    a5         fof         k5amp, kfund*kRndFact, k5form, koct, k5band, iris, idur, idec, iolaps, ifna, ifnb, itotdur
    ;======================================================================================================================================================================

    ;OUT===================================================================================================================================================================
    asig        =        (a1*kFAmp1 + a2*kFAmp2 + a3*kFAmp3 + a4*kFAmp4 + a5*kFAmp5)/5    ;mix the five fof streams and reduce amplitude five-fold

            xout        asig            ;send audio back to caller instrument
endop

opcode    ChoVoice,a,kkiii
    kDtnDep,kDtnRte,ifn,icount,invoices    xin            ;read in input args.
    ktime    randomi    0.01,0.1*kDtnDep,kDtnRte,1            ;create delay time value (linearly interpolating random function will implement pitch/time modulations)
    kptime    linseg    0,0.001,1                    ;portamento time (ramps up quickly from zero to a held value)
    ktime    portk    ktime,kptime                    ;apply portamento smoothing to delay time changes (prevents angular pitch changes)
    atime    interp    ktime                        ;create an interpolated a-rate version of delay time function (this will prevent qualtisation artifacts)
    atap    deltapi    atime+0.0015                    ;tap the delay buffer (nb. buffer opened and closed in caller instrument, UDO exists within the buffer)
    iDel    random    ksmps/sr,0.2                    ;random fixed delay time. By also apply a fixed delay time we prevent excessive amplitude at ote onsets when many chorus voices (N.Voices) are used
    atap    delay    atap,iDel                    ;apply fixed delay
    amix    init    0                        ;initialise amix variable (needed incase N.Voices is 1 in which case recirsion would not be used) 
    if icount<invoices then                        ;if stack of chorus voices is not yet completed...
     amix    ChoVoice    kDtnDep,kDtnRte,ifn,icount+1,invoices    ;.. call the UDO again. Increment count number.
    endif
        xout    atap+amix                    ;send chorus voice created in this interation (and all subsequent voices) back to caller instrument
endop

instr    1    ;instrument that continuously scans widgets
    gkmonopoly    chnget    "monopoly"        ;read widgets...
    gkDtnDep    chnget    "DtnDep"
    gkDtnRte    chnget    "DtnRte"    
    gkvibdep    chnget    "vibdep"                
    gkmodrte    chnget    "modrte"            
    gktrmdep    chnget    "trmdep"            
    gklevel        chnget    "lev"                
    gkvowel        chnget    "vowel"                
    gkoct        chnget    "oct"
    gkLegTim    chnget    "LegTim"
    gkRvbMix    chnget    "RvbMix"
    gkRvbSize    chnget    "RvbSize"
    kporttime    linseg    0,0.001,0.1        ;portamento time (ramps up quickly from zero to a held value)
    gkvowel        portk    gkvowel,kporttime    ;apply portamento smoothing
    gkoct        portk    gkoct,kporttime
    gkFiltOnOff    chnget    "FiltOnOff"
    gkcf    chnget    "cf"
    gkbw    chnget    "bw"
    gkcf        portk    cpsoct(gkcf),kporttime        ;apply portamento smoothing
    gkbw        portk    gkbw*gkcf,kporttime        ;apply portamento smoothing
    gkFormScale chnget  "FormScale"
    gkBWScale   chnget  "BWScale"
    gkFAmp1     chnget   "FAmp1"
    gkFAmp2     chnget   "FAmp2"
    gkFAmp3     chnget   "FAmp3"
    gkFAmp4     chnget   "FAmp4"
    gkFAmp5     chnget   "FAmp5"
endin

instr    2    ;triggered via MIDI
    gkNoteTrig    init    1    ;at the beginning of a new note set note trigger flag to '1'
    icps        cpsmidi        ;read in midi note pitch in cycles per second
    givel        veloc    0,1    ;read in midi note velocity
    gkcps    =    icps        ;update a global krate variable for note pitch

    if i(gkmonopoly)==0 then        ;if we are *not* in legato mode...
     inum    notnum                        ; read midi note number (0 - 127)
         event_i    "i",p1+1+(inum*0.001),0,-1,icps    ; call soud producing instr
     krel    release                        ; release flag (1 when note is released, 0 otherwise)
     if krel==1 then                    ; when note is released...
      turnoff2    p1+1+(inum*0.001),4,1            ; turn off the called instrument
     endif                            ; end of conditional
    else                ;otherwise... (i.e. legato mode)
     iactive    =    i(gkactive)            ;number of active notes of instr 3 (note in release are disregarded)
     if iactive==0 then                    ;...if no notes are active
      event_i    "i",p1+1,0,-1                ;...start a new held note
     endif
    endif
endin

instr    3
    ivoice        chnget    "voice"            ;read widgets...
    imoddel        chnget    "moddel"        ;
    imodris        chnget    "modris"        ;
    invoices    chnget    "nvoices"        ;
    
    kporttime    linseg    0,0.001,1        ;portamento time function rises quickly from zero to a held value
    kporttime    =    kporttime*gkLegTim    ;scale portamento time function with value from GUI knob widget
    
    if i(gkmonopoly)==1 then            ;if we are in legato mode...
     krel    release                    ;sense when  note has been released
     gkactive    =    1-krel            ;if note is in release, gkactive=0, otherwise =1
     kcps    portk    gkcps,kporttime            ;apply portamento smooth to changes in note pitch (this will only have an effect in 'legato' mode)
     kactive    active    p1-1            ;...check number of active midi notes (previous instrument)
     if kactive==0 then                ;if no midi notes are active...
      turnoff                    ;... turn this instrument off
     endif
    else                        ;otherwise... (polyphonic / non-legato mode)
     kcps    =    p4                 ;pitch equal to the original note pitch
    endif

    if gkNoteTrig==1&&gkmonopoly==1 then        ;if a new note is beginning and if we are in monophonic mode...
     reinit    RESTART_ENVELOPE            ;reinitialise the modulations build up
    endif
    RESTART_ENVELOPE:
    ;VIBRATO (PITCH MODULATION)
    kmodenv    linseg    0,0.001+imoddel,0,0.001+imodris,1        ;modulation depth envelope - modulation can be delayed by the first envelope segement and the rise time is defined by the duration of the second segment
    kDepVar    randomi    0.5,1,4                        ;random variance of the depth of modulation
    kmodenv    portk    kmodenv*kDepVar,kporttime            ;smooth changes in modulation envelope to prevent discontinuities whnever the envelope is restarted
    rireturn
    
    kRteVar    randi    0.1,4                        ;random variation of the rate of modulation
        
    kvib    lfo    gkvibdep*kmodenv,gkmodrte*octave(kRteVar),0    ;vibrato function
    
    ;TREMOLO (AMPLITUDE MODULATION)
    ktrem        lfo        kmodenv*(gktrmdep/2),gkmodrte*octave(kRteVar),0    ;TREMOLO LFO FUNCTION
    ktrem        =        (ktrem+0.5) + (gktrmdep * 0.5)    ;OFFSET AND RESCALE TREMOLO FUNCTION ACCORDING TO TREMOLO DEPTH WIDGET SETTING 
    
    iRelTim    =    0.05
    kCpsAtt    expsegr    0.6,rnd(0.004)+0.001,1,iRelTim,1-rnd(0.05)    ;a little jump in pitch at the beginning of a note will give the note a realistic attack sound. This will be most apparent when note velocity is high. And a little gliss at the end of notes.
    
    kcpsRnd    gaussi    1,0.01,10                    ;create a function that will be used to apply naturalistic pitch instability
    kcps    =    kcps*(1+kcpsRnd)                ;apply pitch instability
    asig    fofx5    kcps*semitone(kvib)*kCpsAtt, gkvowel, gkoct, gkFormScale, gkBWScale, gkFAmp1, gkFAmp2, gkFAmp3, gkFAmp4, gkFAmp5, ivoice-1    ;CALL fofx5 UDO
    if gkFiltOnOff==1 then
     asig        reson        asig,gkcf,gkbw,1            ;parametric EQ
    endif
    aatt    linseg    0,(0.3*(1-givel)*(invoices^0.8))+0.01,1            ;AMPLITUDE ENVELOPE - ATTACK TIME IS INFLUENCED BY KEY VELOCITY
    asig    =    asig*aatt*ktrem*(0.3+givel*0.7)*gklevel            ;APPLY AMPLITUDE CONTROLS: ENVELOPE, TREMOLO, KEY VELOCITY AND LEVEL

    /*CHORUS*/    
    if invoices>1 then
     abuf    delayr    2                        ;--left channel--
     amixL    ChoVoice    gkDtnDep,gkDtnRte,gisine,1,invoices    ;call UDO
        delayw    asig

     abuf    delayr    2                        ;--right channel--
     amixR    ChoVoice    gkDtnDep,gkDtnRte,gisine,1,invoices    ;call UDO
        delayw    asig

     asigL    =    amixL/(invoices^0.5)                ;scale mix of chorus voices according to the number of voices...
     asigR    =    amixR/(invoices^0.5)                ;...and the right channel
    else                                ;otherwise... (N.Voices = 1)
     asigL    =    asig                        ;send mono signal to both channels
     asigR    =    asig     
    endif
    arel    linsegr    1,iRelTim,0                    ;release envelope
    asigL    =    asigL*arel                    ;apply release envelope
    asigR    =    asigR*arel

    kwet    limit    2*gkRvbMix,0,1                    ;wet (reverb) level control (reaches maximum a knob halfway point and hold that value for the remainder of its travel)
    gasendL    =    gasendL+asigL*kwet                ;send some audio to the reverb instrument
    gasendR    =    gasendR+asigR*kwet
    kdry    limit    2*(1-gkRvbMix),0,1                ;dry level (stays at maximum for the first half of the knob's travel then ramps down to zero for the remainder of its travel)
        outs    asigL*kdry,asigR*kdry                ;SEND AUDIO TO OUTPUT
    gkNoteTrig    =    0                    ;reset new-note trigger (in case it was '1')
endin

instr    Effects
    if gkRvbMix>0 then
     aL,aR    reverbsc    gasendL,gasendR,gkRvbSize,12000        ;create stereo reverb signal
        outs        aL,aR                    ;send reverb signal to speakers
        clear        gasendL,gasendR                ;clear reverb send variables
    endif
endin

instr 999
a1,a2 monitor
      fout    "fofvoice.wav",4,a1,a2
endin

</CsInstruments>

<CsScore>
f 0 [3600*24*7]
i 1 0 [3600*24*7]        ;read widgets
i "Effects" 0 [3600*24*7]     ;reverb
;i 999 0 3600
</CsScore>

</CsoundSynthesizer>