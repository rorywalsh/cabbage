
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; MouseChord.csd
; Written by Iain McCurdy, 2013

; Mouse Chord
; -----------
; This csd was originally written for use by someone who only had the use of a head-mouse for music performance.
; If the mouse enters one of the coloured boxes the chord indicated by the Roman numeral is played. 
; Continuous dynamic control is possible by moving the mouse within the box: the dynamic is lowest at the edges of each box and highest at the centre of each box.
; Key and tonality (major/minor) can also be selected from within the GUI.
; Professional head mouse or iris tracking software can be very expensive and requires practice. 
; Camera Mouse is a good free option for Windows users. It can be found at www.cameramouse.org.
; iNavigate is a free option for Mac OS 10.5 if the user wants to experiment with this approach.
; 
; You can change the size of the panel in the Cabbage 'form' setup. 
; If you do this, change the values for 'giPanelWidth' and 'giPanelHeight' just below the orchestra header and all elements
; will be stretch, compressed and moved appropriately.

<Cabbage>
form caption("Mouse Chord") size(700, 500), pluginId("MChd"), guiRefresh(64), colour(40,40,40)
image bounds(0,0,0,0),     colour(255,100,0),   shape("rounded"), outlineColour("white"), line(1), identChannel("1")         ; I
image bounds(0,0,0,0),     colour("Blue"),      shape("rounded"), outlineColour("white"), line(1), identChannel("2")         ; II
image bounds(0,0,0,0),     colour("Yellow"),    shape("rounded"), outlineColour("white"), line(1), identChannel("3")         ; III
image bounds(0,0,0,0),     colour("Green"),     shape("rounded"), outlineColour("white"), line(1), identChannel("4")         ; IV
image bounds(0,0,0,0),     colour(255,0,0),     shape("rounded"), outlineColour("white"), line(1), identChannel("5")         ; V
image bounds(0,0,0,0),     colour("Purple"),    shape("rounded"), outlineColour("white"), line(1), identChannel("6")         ; VI
image bounds(0,0,0,0),     colour(150, 40, 40), shape("rounded"), outlineColour("white"), line(1), identChannel("7")     ; VII
label bounds(132,270, 40, 50), text("I"),   fontColour("white"), align("centre"), identChannel("11")
label bounds(380,270, 45, 50), text("II"),  fontColour("white"), align("centre"), identChannel("22")
label bounds(615,270, 70, 50), text("III"), fontColour("white"), align("centre"), identChannel("33")
label bounds(875,270, 60, 50), text("IV"),  fontColour("white"), align("centre"), identChannel("44")
label bounds(220,620, 60, 50), text("V"),   fontColour("white"), align("centre"), identChannel("55")
label bounds(470,620, 60, 50), text("VI"),  fontColour("white"), align("centre"), identChannel("66")
label bounds(708,620, 80, 50), text("VII"), fontColour("white"), align("centre"), identChannel("77")

combobox bounds(  5, 20, 70, 20), channel("key"), value(12), text("B", "A#", "A", "G#", "G", "F#", "F", "E", "D#", "D", "C#", "C")
combobox bounds( 80, 20, 70, 20), channel("oct"), value(3), text("2", "1", "0", "-1", "-2")
combobox bounds(155, 20, 70, 20), channel("ton"), value(2), text("minor", "major")
combobox bounds(230, 20, 70, 20), channel("ins"), value(1), text("strings", "brass")
label    bounds( 20,  5, 30, 11), text("Key"), fontColour("white")
label    bounds( 85,  5, 50, 11), text("Octave"), fontColour("white")
label    bounds(158,  5, 55, 11), text("Tonality"), fontColour("white")
label    bounds(230,  5, 70, 11), text("Instrument"), fontColour("white"), align("centre")

</Cabbage>


<CsoundSynthesizer>

<CsOptions>
-n -dm0
</CsOptions>

<CsInstruments>

; sr set by host
ksmps    =     32
nchnls    =     2
0dbfs    =    1    ;MAXIMUM AMPLITUDE REGARDLESS OF BIT DEPTH
    seed    0
giPanelWidth    =    700
giPanelHeight    =    500
giBoxWidth    =    giPanelWidth / 5.5
giBoxHeight    =    giPanelHeight / 4

giBox1X        =    giPanelWidth / (1100/75)
giBox1Y        =    giPanelHeight / (800/130)
giBox2X        =    giPanelWidth / (1100/325)
giBox2Y        =    giPanelHeight / (800/130)
giBox3X        =    giPanelWidth / (1100/575)
giBox3Y        =    giPanelHeight / (800/130)
giBox4X        =    giPanelWidth / (1100/825)
giBox4Y        =    giPanelHeight / (800/130)
giBox5X        =    giPanelWidth / (1100/175)
giBox5Y        =    giPanelHeight / (800/480)
giBox6X        =    giPanelWidth / (1100/425)
giBox6Y        =    giPanelHeight / (800/480)
giBox7X        =    giPanelWidth / (1100/675)
giBox7Y        =    giPanelHeight / (800/480)


zakinit    4,10
gkRvbSze    init    0.85
gkRvbSnd    init    0.2



gisaw        ftgen    0,0,4096,11,80,1,0.9

/* generate bandlimited sawtooth waves */
i0    =  0
loop1:
imaxh    =  sr / (2 * 440.0 * exp (log(2.0) * (i0 - 69) / 12))
i_    ftgen i0 + 10, 0, 4096, -30, gisaw, 1, imaxh                    ;use gen 30
i0    =  i0 + 1
    if (i0 < 127.5) igoto loop1





;giahh55     ftgen    0,0,4096,10,0.667225,0.194524,0.098683,0.096875,0.021209,0.006311,0.002978,0.001013,0.001249,0.001446,0.002393,0.004826,0.018741,0.012161,0.010480,0.005261,0.004569,0.001376,0.001132,0.003605,0.001846,0.002757,0.005346,0.004712,0.004806,0.002357,0.001109,0.001302,0.001860,0.001054,0.001120,0.001642,0.002240,0.004382,0.005473,0.003755,0.002444,0.002088,0.001822,0.000946,0.000790,0.001222,0.001653,0.001374,0.001401,0.002118,0.002061,0.001470,0.001198,0.001635,0.002387,0.002248,0.001327,0.000951,0.000884,0.000844,0.000805,0.000667,0.000669,0.000701,0.000591,0.000445,0.000367,0.000314,0.000272,0.000225,0.000179,0.000152,0.000164,0.000169,0.000151,0.000144,0.000137,0.000121,0.000115,0.000123,0.000125,0.000116,0.000102,0.000095,0.000102,0.000120,0.000135,0.000132,0.000117,0.000100,0.000084,0.000074,0.000072,0.000075,0.000077,0.000079,0.000083,0.000083,0.000081,0.000082,0.000084,0.000081,0.000074,0.000063
giooh         ftgen     0,0,4096,10, 0.110941,0.606890,0.046168,0.040510,0.031310,0.007323,0.003115,0.002749,0.002420,0.002253,0.002853,0.002337,0.002672,0.002061,0.002202,0.002410,0.002615,0.005079,0.003192,0.002699,0.003708,0.003501,0.002264,0.001713,0.001713

gicos        ftgen    0,0,131072,11,1        ;COSINE WAVE (USED BY THE LFOS)
gieqffn        ftgen    0,0,4097,7,-1,4096,1
gieqlfn        ftgen    0,0,4097,7,-1,4096,1
gieqqfn        ftgen    0,0,4097,7,-1,4096,1

;DEFINE CHORDS I-VII IN PCH FORMAT FOR MAJOR KEY
gichord1    ftgen    0,0,4,-2,8.00,8.04,8.07,9.00
gichord2    ftgen    0,0,4,-2,8.02,8.05,8.09,9.02
gichord3    ftgen    0,0,4,-2,8.04,8.07,8.11,9.04
gichord4    ftgen    0,0,4,-2,8.00,8.05,8.09,9.00
gichord5    ftgen    0,0,4,-2,7.11,8.02,8.07,8.11
gichord6    ftgen    0,0,4,-2,8.00,8.04,8.09,9.00
gichord7    ftgen    0,0,4,-2,8.02,8.05,8.11,9.02

;DEFINE CHORDS I-VII IN PCH FORMAT FOR MINOR KEY
gichord1m    ftgen    0,0,4,-2,8.00,8.03,8.07,9.00
gichord2m    ftgen    0,0,4,-2,8.02,8.05,8.08,9.02
gichord3m    ftgen    0,0,4,-2,8.03,8.07,8.11,9.03
gichord4m    ftgen    0,0,4,-2,8.05,8.08,8.00,9.05
gichord5m    ftgen    0,0,4,-2,8.07,8.11,8.02,9.07
gichord6m    ftgen    0,0,4,-2,8.08,8.00,8.03,9.08
gichord7m    ftgen    0,0,4,-2,8.02,8.05,8.11,9.02    
    
instr    1

 gkMOUSE_X    chnget    "MOUSE_X"
 gkMOUSE_Y    chnget    "MOUSE_Y"
 gkton        chnget    "ton"
 gkins        chnget    "ins"
 gkkey        chnget    "key"
 gkkey        =    11 - (gkkey - 1)
 gkoct        chnget    "oct"
 gkoct        =    3-gkoct

#define CHORD_BOX(N)
 #
 if (gkMOUSE_X > giBox$N.X)  && (gkMOUSE_X < (giBox$N.X+giBoxWidth))  && (gkMOUSE_Y > giBox$N.Y)  && (gkMOUSE_Y < (giBox$N.Y+giBoxHeight)) then
  kinside$N    =    1
 else
  kinside$N    =    0
 endif
 ktrig$N    changed    kinside$N    ;OUTPUT A MOMENTARY '1' VIA ktrig$N IF kinside$N CHANGES, I.E. IF THE MOUSE ENTERS OR LEAVES THIS CHORD BOX
 if    ktrig$N=1    then    ;IF MOUSE HAS ENTERED OR LEFT THIS CHORD BOX (I.E. IF ktrig$N has taken a value of 1)...
     if    kinside$N=1    then    ;IF MOUSE IS IN THE BOX...
         event    "i", 2, 0, 3600, $N    ;PLAY A LONG NOTE INSTRUMENT $N
     elseif    kinside$N=0    then    ;IF MOUSE HAS JUST LEFT THE BOX...
         turnoff2    2, 0, 1    ;TURNOFF INSTRMENT $N ALLOWING RELEASE STAGES TO BE COMPLETED
     endif                ;END OF CONDITIONAL BRANCHING
 endif
 if kinside$N = 1 then
  kx$N    mirror    (gkMOUSE_X - giBox$N.X) / giBoxWidth, 0, 0.5 
  ky$N    mirror    (gkMOUSE_Y - giBox$N.Y) / giBoxHeight, 0, 0.5 
  k$N    =    kx$N + ky$N
  zkw    k$N, $N-1    ;WRITE LOCATION VALUE TO ZAK VARIABLE
 endif
 #

$CHORD_BOX(1)
$CHORD_BOX(2)
$CHORD_BOX(3)
$CHORD_BOX(4)
$CHORD_BOX(5)
$CHORD_BOX(6)
$CHORD_BOX(7)

endin

instr    2
    ipch2    table    1, gichord1 + p4 - 1    ;PITCH OF FIRST NOTE OF CHORD
    ipch3    table    2, gichord1 + p4 - 1    ;PITCH OF SECOND NOTE OF CHORD
    ipch4    table    3, gichord1 + p4 - 1    ;PITCH OF THIRD NOTE OF CHORD
    aenv    expsegr    0.001,0.2,1,1,0.001    ;AMPLITUDE ENVELOPE
    
    ;DEFINE MACRO OF CODE TO PRODUCE EACH NOTE OF THE CHORD
#define    NOTE(N)
    #
    
    if    i(gkton)=2    then                        ; IF TONALITY IS 'MAJOR'...
        ipch$N    table    $N-1, gichord1 + p4 - 1                ; DEFINE PITCH (PCH FORMAT) FOR THIS NOTE (MAJOR)
    else                                    ; OTHERWISE TONALITY MUST BE MINOR...
        ipch$N    table    $N-1, gichord1m + p4 - 1            ; DEFINE PITCH (PCH FORMAT) FOR THIS NOTE (MINOR)
    endif                                    ; END OF CONDITIONAL BRANCHING
    kcps$N        =    cpspch(ipch$N + (i(gkkey)*0.01) + i(gkoct))    ; DERIVE A CPS VALUE

    kamp    zkr    p4 - 1        ;READ AMPLITUDE FROM ZAK VARIABLE
    kamp    port    kamp, 0.05    ;APPLY PORTAMENTO (TO PREVENT QUANTISATION / ZIPPER NOISE)
    aamp    interp    kamp        ;INTERPOLATE TO CREATE AN AUDIO RATE VERSION OF THIS VARIABLE

    if i(gkins)==1 then
     kfnum    =  (10 + 69 + 0.5 + 12 * log(kcps$N / 440.0) / log(2.0))                ;table number
      ;TWO ITERATIONS OF oscbnk ARE CREATED, ONE FOR EACH OF THE STEREO AUDIO CHANNELS. THE OUTPUTS WILL BE DIFFERENT AS THE RANDOM NUMBER GENERATORS WILL BE SEEDED BY THE SYSTEM CLOCK
      ;OUTPUT    OPCODE  CPS    | AMD  |    FMD       | PMD | OVERLAPS   | SEED  | L1MINF  | L1MAXF  | L2MINF  | L2MAXF  | LFOMODE | EQMINF  | EQMAXF | EQMINL | EQMAXL | EQMINQ | EQMAXQ  | EQMODE | KFN  | L1FN | L2FN | EQFFN  | EQLF   |  EQQFN |  TABL  | OUTFN
      aL$N    oscbnk    kcps$N,   0,   0.005*kcps$N,    0,     10,        rnd(1),   0,         1,       0,        0,       238,      0,       8000,      1,       1,       1,       1,       -1, kfnum,  gicos, gicos, gieqffn, gieqlfn, gieqqfn
      aR$N    oscbnk    kcps$N,   0,   0.005*kcps$N,    0,     10,        rnd(1),   0,        -1,       0,        0,       238,      0,       8000,      1,       1,       1,       1,       -1, kfnum,  gicos, gicos, gieqffn, gieqlfn, gieqqfn

    else    
     kmul    scale    kamp,0.9,0.1
     kdtn    rspline    -0.1,0.1,0.05,0.4
     aL$N.1    gbuzz    kamp,kcps$N*semitone(kdtn),200,1,kmul,gicos
     kdtn    rspline    -0.1,0.1,0.05,0.4
     aR$N.1    gbuzz    kamp,kcps$N*semitone(kdtn),200,1,kmul,gicos
     kdtn    rspline    -0.1,0.1,0.05,0.4
     aL$N.2    gbuzz    kamp,kcps$N*semitone(kdtn),200,1,kmul,gicos
     kdtn    rspline    -0.1,0.1,0.05,0.4
     aR$N.2    gbuzz    kamp,kcps$N*semitone(kdtn),200,1,kmul,gicos
     aL$N    sum    aL$N.1,aL$N.2    
     aR$N    sum    aR$N.1,aR$N.2
    endif
    #
    ;EXECUTE MACRO FOR EACH NOTE
    $NOTE(1)
    $NOTE(2)
    $NOTE(3)
    $NOTE(4)
    
    ;SUM (MIX) THE FOUR NOTES
    aL    sum    aL1, aL2, aL3, aL4
    aR    sum    aR1, aR2, aR3, aR4
    
    kcfoct    =    (8*kamp)+6    ;DEFINE A FILTER CUTOFF FREQUENCY WHICH IS RELATED TO DISTANCE FROM THE CENTRE OF THE BOX
    
    aL    tonex    aL, cpsoct(kcfoct),2    ;APPLY LOW PASS FILTERING (TONE CONTROL)
    aR    tonex    aR, cpsoct(kcfoct),2    ;APPLY LOW PASS FILTERING (TONE CONTROL)
    
    aL    =    aL * aenv * aamp * 0.03    ;SCALE AUDIO SIGNAL WITH AMPLITIUDE ENVELOPE
    aR    =    aR * aenv * aamp * 0.03
    outs    aL, aR
        zawm    aL * gkRvbSnd, 0                      ;SEND SOME OF THE AUDIO TO THE REVERB VIA ZAK PATCHING (LEFT CHANNEL) 
        zawm    aR * gkRvbSnd, 1                      ;SEND SOME OF THE AUDIO TO THE REVERB VIA ZAK PATCHING (RIGHT CHANNEL)


endin

instr    1000    ;REVERB
    ainL    zar    0    ;READ IN AUDIO FROM ZAK CHANNELS
    ainR    zar    1    ;READ IN AUDIO FROM ZAK CHANNELS
    denorm        ainL, ainR        ;...DENORMALIZE BOTH CHANNELS OF AUDIO SIGNAL
        arvbL, arvbR     reverbsc     ainL, ainR, gkRvbSze, 10000    ;USE reverbsc OPCODE
    outs    arvbL, arvbR                    ;SEND AUDIO TO OUTPUTS
    zacl    0,3                        ;CLEAR ZAK AUDIO CHANNELS
endin

instr    1001    ; SETUP INITIAL LOCATIONS AND SIZES OF BOXES AND LABELS
 #define SETUP_BOX(N)
 #
 Smsg    sprintf    "bounds(%d,%d,%d,%d)",giBox$N.X,giBox$N.Y,giBoxWidth,giBoxHeight
      chnset    Smsg, "$N"
 Smsg    sprintf    "bounds(%d,%d,%d,%d)",giBox$N.X,giBox$N.Y+giBoxHeight,giBoxWidth,giBoxHeight/3
      chnset    Smsg, "$N$N"
 #
 $SETUP_BOX(1)
 $SETUP_BOX(2)
 $SETUP_BOX(3)
 $SETUP_BOX(4)
 $SETUP_BOX(5)
 $SETUP_BOX(6)
 $SETUP_BOX(7)
endin

</CsInstruments>

<CsScore>
i 1001 0 0    ; SETUP INITIAL LOCATIONS AND SIZES OF BOXES AND LABELS
i 1 0 3600    ; INSTRUMENT 1 PLAYS A NOTE FOR 1 HOUR (AND KEEPS REAL-TIME PERFORMANCE GOING)
i 1000 0 3600    ; reverb
</CsScore>

</CsoundSynthesizer>