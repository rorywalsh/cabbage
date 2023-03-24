
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

;  MiniPops.csd
;  Written by Iain McCurdy, 2014.
;
;  Emulation of the Korg/Univibe Mini Pops 7

; The Mini Pops 7 was an early non-programmable drum machine from the Japanese company Korg. It is perhaps most through 
; its use by Jean Michel Jarre in his early work. I have tried to retain as close a tie as possible with the original 
; design whilst making some additions and modifications for convenience.
; Preset rhythms are selected using the large black buttons. The lower row of rhythms (in red) are selected by first 
; setting the 'Lower' checkbox to 'on'.
; Additional effects are 'O.D.' - clipping distortion, 'Dec' - sample rate decimation and 'Mash' - break-beat style cut
; ups.
; The 'Rhythm' selector buttons were radio buttons (depressing one would release any others) but a popular trick was to 
; force more than one to stay depressed and to create a combination of two or more rhythms. The rhythms would combine in
; a rather enigmatic way. Unfortunately I quite figured out how this worked yet so in my implementation they simply mix.



<Cabbage>
form size(840,250), caption("Mini Pops"), pluginId("MPop"), colour(255,255,205), guiRefresh(64)

checkbox bounds( 13,  6,   45,   44), colour("tomato"), channel("Act1"),  value(0), shape("ellipse")
checkbox bounds( 68,  6,   45,   44), colour("tomato"), channel("Act2"),  value(0), shape("ellipse")
checkbox bounds(123,  6,   45,   44), colour("tomato"), channel("Act3"),  value(0), shape("ellipse")
checkbox bounds(178,  6,   45,   44), colour("tomato"), channel("Act4"),  value(0), shape("ellipse")
checkbox bounds(233,  6,   45,   44), colour("tomato"), channel("Act5"),  value(0), shape("ellipse")
checkbox bounds(288,  6,   45,   44), colour("tomato"), channel("Act6"),  value(0), shape("ellipse")
checkbox bounds(343,  6,   45,   44), colour("tomato"), channel("Act7"),  value(0), shape("ellipse")
checkbox bounds(398,  6,   45,   44), colour("tomato"), channel("Act8"),  value(0), shape("ellipse")
checkbox bounds(453,  6,   45,   44), colour("tomato"), channel("Act9"),  value(0), shape("ellipse")
checkbox bounds(508,  6,   45,   44), colour("tomato"), channel("Act10"),  value(0), shape("ellipse")
checkbox bounds(563,  6,   45,   44), colour("tomato"), channel("Act11"),  value(0), shape("ellipse")
checkbox bounds(618,  6,   45,   44), colour("tomato"), channel("Act12"),  value(0), shape("ellipse")
checkbox bounds(673,  6,   45,   44), colour("tomato"), channel("Act13"),  value(0), shape("ellipse")
checkbox bounds(728,  6,   45,   44), colour("tomato"), channel("Act14"),  value(0), shape("ellipse")
checkbox bounds(783,  6,   45,   44), colour("tomato"), channel("Act15"),  value(0), shape("ellipse")

rslider  bounds(  5,  5, 60, 60), text("Bass Drum"),  textColour("black"), colour(255,220,155), alpha(0.75), channel("bassdrum"),    range(0, 1, 0.6), trackerColour(white)
rslider  bounds( 60,  5, 60, 60), text("Snare 1"),    textColour("black"), colour(255,220,155), alpha(0.75), channel("snare1"),      range(0, 1, 0.6), trackerColour(255,255,205)
rslider  bounds(115,  5, 60, 60), text("Snare 2"),    textColour("black"), colour(255,220,155), alpha(0.75), channel("snare2"),      range(0, 1, 0.6), trackerColour(255,255,205)
rslider  bounds(170,  5, 60, 60), text("Rimshot"),    textColour("black"), colour(255,220,155), alpha(0.75), channel("rimshot"),     range(0, 1, 0.6), trackerColour(255,255,205)
rslider  bounds(225,  5, 60, 60), text("Cymbal 1"),   textColour("black"), colour(255,220,155), alpha(0.75), channel("cymbal1"),     range(0, 1, 0.6), trackerColour(255,255,205)
rslider  bounds(280,  5, 60, 60), text("Cymbal 2"),   textColour("black"), colour(255,220,155), alpha(0.75), channel("cymbal2"),     range(0, 1, 0.6), trackerColour(255,255,205)
rslider  bounds(335,  5, 60, 60), text("Bongo 1"),    textColour("black"), colour(255,220,155), alpha(0.75), channel("bongo1"),      range(0, 1, 0.6), trackerColour(255,255,205)
rslider  bounds(390,  5, 60, 60), text("Bongo 2"),    textColour("black"), colour(255,220,155), alpha(0.75), channel("bongo2"),      range(0, 1, 0.6), trackerColour(255,255,205)
rslider  bounds(445,  5, 60, 60), text("Bongo 3"),    textColour("black"), colour(255,220,155), alpha(0.75), channel("bongo3"),      range(0, 1, 0.6), trackerColour(255,255,205)
rslider  bounds(500,  5, 60, 60), text("Claves"),     textColour("black"), colour(255,220,155), alpha(0.75), channel("claves"),      range(0, 1, 0.6), trackerColour(255,255,205)
rslider  bounds(555,  5, 60, 60), text("Cowbell"),    textColour("black"), colour(255,220,155), alpha(0.75), channel("cowbell"),     range(0, 1, 0.6), trackerColour(255,255,205)
rslider  bounds(610,  5, 60, 60), text("Guiro"),      textColour("black"), colour(255,220,155), alpha(0.75), channel("guiro"),       range(0, 1, 0.6), trackerColour(255,255,205)
rslider  bounds(665,  5, 60, 60), text("Maracas"),    textColour("black"), colour(255,220,155), alpha(0.75), channel("maracas"),     range(0, 1, 0.6), trackerColour(255,255,205)
rslider  bounds(720,  5, 60, 60), text("Quijada"),    textColour("black"), colour(255,220,155), alpha(0.75), channel("quijada"),     range(0, 1, 0.6), trackerColour(255,255,205)
rslider  bounds(775,  5, 60, 60), text("Tamb."),      textColour("black"), colour(255,220,155), alpha(0.75), channel("tambourine"),  range(0, 1, 0.6), trackerColour(255,255,205)

checkbox bounds( 13, 66, 60, 10), text("mute"),  channel("mute1"), fontColour:0("black"), fontColour:1("black"), colour(red),value(0)
checkbox bounds( 13, 78, 60, 10), text("solo"),  channel("solo1"), fontColour:0("black"), fontColour:1("black"), colour(lime),value(0)
checkbox bounds( 68, 66, 60, 10), text("mute"),  channel("mute2"), fontColour:0("black"), fontColour:1("black"), colour(red),value(0)
checkbox bounds( 68, 78, 60, 10), text("solo"),  channel("solo2"), fontColour:0("black"), fontColour:1("black"), colour(lime),value(0)
checkbox bounds(123, 66, 60, 10), text("mute"),  channel("mute3"), fontColour:0("black"), fontColour:1("black"), colour(red),value(0)
checkbox bounds(123, 78, 60, 10), text("solo"),  channel("solo3"), fontColour:0("black"), fontColour:1("black"), colour(lime),value(0)
checkbox bounds(178, 66, 60, 10), text("mute"),  channel("mute4"), fontColour:0("black"), fontColour:1("black"), colour(red),value(0)
checkbox bounds(178, 78, 60, 10), text("solo"),  channel("solo4"), fontColour:0("black"), fontColour:1("black"), colour(lime),value(0)
checkbox bounds(233, 66, 60, 10), text("mute"),  channel("mute5"), fontColour:0("black"), fontColour:1("black"), colour(red),value(0)
checkbox bounds(233, 78, 60, 10), text("solo"),  channel("solo5"), fontColour:0("black"), fontColour:1("black"), colour(lime),value(0)
checkbox bounds(288, 66, 60, 10), text("mute"),  channel("mute6"), fontColour:0("black"), fontColour:1("black"), colour(red),value(0)
checkbox bounds(288, 78, 60, 10), text("solo"),  channel("solo6"), fontColour:0("black"), fontColour:1("black"), colour(lime),value(0)
checkbox bounds(343, 66, 60, 10), text("mute"),  channel("mute7"), fontColour:0("black"), fontColour:1("black"), colour(red),value(0)
checkbox bounds(343, 78, 60, 10), text("solo"),  channel("solo7"), fontColour:0("black"), fontColour:1("black"), colour(lime),value(0)
checkbox bounds(398, 66, 60, 10), text("mute"),  channel("mute8"), fontColour:0("black"), fontColour:1("black"), colour(red),value(0)
checkbox bounds(398, 78, 60, 10), text("solo"),  channel("solo8"), fontColour:0("black"), fontColour:1("black"), colour(lime),value(0)
checkbox bounds(453, 66, 60, 10), text("mute"),  channel("mute9"), fontColour:0("black"), fontColour:1("black"), colour(red),value(0)
checkbox bounds(453, 78, 60, 10), text("solo"),  channel("solo9"), fontColour:0("black"), fontColour:1("black"), colour(lime),value(0)
checkbox bounds(508, 66, 60, 10), text("mute"), channel("mute10"), fontColour:0("black"), fontColour:1("black"), colour(red),value(0)
checkbox bounds(508, 78, 60, 10), text("solo"), channel("solo10"), fontColour:0("black"), fontColour:1("black"), colour(lime),value(0)
checkbox bounds(563, 66, 60, 10), text("mute"), channel("mute11"), fontColour:0("black"), fontColour:1("black"), colour(red),value(0)
checkbox bounds(563, 78, 60, 10), text("solo"), channel("solo11"), fontColour:0("black"), fontColour:1("black"), colour(lime),value(0)
checkbox bounds(618, 66, 60, 10), text("mute"), channel("mute12"), fontColour:0("black"), fontColour:1("black"), colour(red),value(0)
checkbox bounds(618, 78, 60, 10), text("solo"), channel("solo12"), fontColour:0("black"), fontColour:1("black"), colour(lime),value(0)
checkbox bounds(673, 66, 60, 10), text("mute"), channel("mute13"), fontColour:0("black"), fontColour:1("black"), colour(red),value(0)
checkbox bounds(673, 78, 60, 10), text("solo"), channel("solo13"), fontColour:0("black"), fontColour:1("black"), colour(lime),value(0)
checkbox bounds(728, 66, 60, 10), text("mute"), channel("mute14"), fontColour:0("black"), fontColour:1("black"), colour(red),value(0)
checkbox bounds(728, 78, 60, 10), text("solo"), channel("solo14"), fontColour:0("black"), fontColour:1("black"), colour(lime),value(0)
checkbox bounds(783, 66, 60, 10), text("mute"), channel("mute15"), fontColour:0("black"), fontColour:1("black"), colour(red),value(0)
checkbox bounds(783, 78, 60, 10), text("solo"), channel("solo15"), fontColour:0("black"), fontColour:1("black"), colour(lime),value(0)

;control
image    bounds(620,100, 220,150), colour(0,0,0,0), line(0), plant("control"){
;combobox bounds( 15, 20, 60, 18), channel("UpperLower"), value(1), text("Upper", "Lower")

checkbox bounds( 15, 16, 12, 13), channel("r1") fontColour("white") colour(yellow) value(1)
checkbox bounds( 15, 28, 12, 13), channel("r2") fontColour("white") colour(yellow) 
label    bounds( 28, 17, 60, 12), text("Upper"), fontColour("black"), align("left")
label    bounds( 28, 29, 60, 12), text("Lower"), fontColour(205,0,0), align("left")


rslider  bounds( 85,  5, 60, 60), text("O.D."), textColour("black"), colour(200,160, 80), trackerColour(120,100, 52), outlineColour(60,50,26), channel("dist"),      range(0, 1, 0)
rslider  bounds(145,  5, 60, 60), text("Dec."), textColour("black"), colour(200,160, 80), trackerColour(120,100, 52), outlineColour(60,50,26), channel("fold"),      range(0, 1, 0, 0.5, 0.0001)

checkbox bounds(130,  2,  7,  7), channel("DistOn"), value(0), colour(red), shape("ellipse")
checkbox bounds(190,  2,  7,  7), channel("FoldOn"), value(0), colour(red), shape("ellipse")

checkbox bounds( 15, 45, 70, 12), channel("cutup"), text("Mash"), fontColour:0("black"), fontColour:1("black"), colour("yellow"),value(0)

rslider  bounds(  0, 70, 70, 70), text("Volume"),     textColour("black"), colour(200,160, 80), trackerColour(120,100, 52), outlineColour(60,50,26), channel("volume"),      range(0, 1.00, .4)
rslider  bounds( 70, 70, 70, 70), text("Tempo"),      textColour("black"), colour(200,160, 80), trackerColour(120,100, 52), outlineColour(60,50,26), channel("tempo"),       range(40, 480, 115, 1, 1)
checkbox bounds(153, 77, 45, 45), channel("OnOff"), fontColour("black"), colour("yellow"),value(1)
label    bounds(148,127, 60, 11), text("Start/Stop"),     fontColour("black"), align("left")
}

;rhythms
image bounds(  0,  100, 640, 65), colour(0,0,0,0), line(0), plant("rhythms"){
checkbox bounds( 10,  0, 60, 20), channel("but1"), colour(250,250,105), value(0)
checkbox bounds( 70,  0, 60, 20), channel("but2"), colour(250,250,105), value(0)
checkbox bounds(130,  0, 60, 20), channel("but3"), colour(250,250,105), value(0), text("")
checkbox bounds(190,  0, 60, 20), channel("but4"), colour(250,250,105), value(0), text("")
checkbox bounds(250,  0, 60, 20), channel("but5"), colour(250,250,105), value(0), text("")
checkbox bounds(310,  0, 60, 20), channel("but6"), colour(250,250,105), value(0), text("")
checkbox bounds(370,  0, 60, 20), channel("but7"), colour(250,250,105), value(0), text("")
checkbox bounds(430,  0, 60, 20), channel("but8"), colour(250,250,105), value(0), text("")
checkbox bounds(490,  0, 60, 20), channel("but9"), colour(250,250,105), value(0), text("")
checkbox bounds(550,  0, 60, 20), channel("but10"), colour(250,250,105), value(0), text("")

label    bounds( 10, 20, 60, 10), text("WALTZ"),     fontColour("black"), align("centre")
label    bounds( 70, 20, 60, 10), text("SAMBA"),     fontColour("black"), align("centre")
label    bounds(130, 20, 60, 10), text("MAMBO"),     fontColour("black"), align("centre")
label    bounds(190, 20, 60, 10), text("RUMBA"),     fontColour("black"), align("centre")
label    bounds(250, 20, 60, 10), text("TANGO"),     fontColour("black"), align("centre")
label    bounds(310, 20, 60, 10), text("BOSSANOVA"), fontColour("black"), align("centre")
label    bounds(370, 20, 60, 10), text("ROCK 1"),    fontColour("black"), align("centre")
label    bounds(430, 20, 60, 10), text("ROCK 2"),    fontColour("black"), align("centre")
label    bounds(490, 20, 60, 10), text("MARCH 2/4"), fontColour("black"), align("centre")
label    bounds(550, 20, 60, 10), text("SWING"),     fontColour("black"), align("centre")

label    bounds( 10, 30, 60, 10), text("JAZZ WALTZ"),  fontColour(205,  0,  0), align("centre")
label    bounds( 70, 30, 60, 10), text("MERINGUE"),    fontColour(205,  0,  0), align("centre")
label    bounds(130, 30, 60, 10), text("CHA CHA"),     fontColour(205,  0,  0), align("centre")
label    bounds(190, 30, 60, 10), text("BEGUINE"),     fontColour(205,  0,  0), align("centre")
label    bounds(250, 30, 60, 10), text("HABANERA"),    fontColour(205,  0,  0), align("centre")
label    bounds(310, 30, 60, 10), text("SLOW ROCK"),   fontColour(205,  0,  0), align("centre")
label    bounds(370, 30, 60, 10), text("ROCK 3"),      fontColour(205,  0,  0), align("centre")
label    bounds(430, 30, 60, 10), text("ROCK 4"),      fontColour(205,  0,  0), align("centre")
label    bounds(490, 30, 60, 10), text("MARCH 6/8"),   fontColour(205,  0,  0), align("centre")
label    bounds(550, 30, 65, 10), text("LATIN SWING"), fontColour(205,  0,  0), align("centre")

checkbox bounds(618, 19, 10, 10), channel("Indic1"), value(0), colour( 70,255, 70), shape("ellipse"), active(0)
checkbox bounds(618, 30, 10, 10), channel("Indic2"), value(0), colour(255, 30, 30), shape("ellipse"), active(0)
}

keyboard bounds( 10,155, 600, 80)

label   bounds(695,239, 145, 10), text("Author: Iain McCurdy |2014|"), fontColour("DarkSlateGrey"), align("left")
</Cabbage>
              
<CsoundSynthesizer>
<CsOptions>
-n -dm0 -+rtmidi=NULL -M0
</CsOptions>
<CsInstruments>

; sr set by host
ksmps = 64
nchnls = 2
0dbfs=1

massign    0,2    ;MIDI notes assigned to instr 2

;UDOs---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

opcode    CabbageRadio2,k,SS            ; change opcode name and number is string variable inputs
S1,S2    xin                    ; add string inputs for the required number of inputs 
kon        =    1
koff        =    0
ksum        =    0
ktrigsum    =    0
#define READ_CHANGES(NAME)    #
k$NAME    chnget    $NAME
ksum    =    ksum + k$NAME            ; sum of values
kon$NAME    trigger    k$NAME,0.5,0        ; trigger that a checkbox has been turned on
ktrigsum    =    ktrigsum + kon$NAME#    ; sum of all on triggers

#define WRITE_CHANGES(NAME'COUNT)    #
if kon$NAME!=1 then
 chnset    koff,$NAME
else
 kval    =    $COUNT
endif#

#define PREVENT_ALL_OFF(NAME)    #
koff$NAME    trigger    k$NAME,0.5,1        ; trigger that a checkbox has been turned on
if koff$NAME==1&&ksum==0 then
 chnset    kon,$NAME
endif#

$READ_CHANGES(S1) 
$READ_CHANGES(S2)                ; add macro expansions for the required number of radio buttons

if ktrigsum>0 then
 $WRITE_CHANGES(S1'1)
 $WRITE_CHANGES(S2'2)                ; add macro expansions for the required number of radio buttons
endif

$PREVENT_ALL_OFF(S1)
$PREVENT_ALL_OFF(S2)                ; add macro expansions for the required number of radio buttons

kval    =    (ksum=0?0:kval)
    xout    kval
endop
;-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------




gisine    ftgen    0,0,131072,10,1

gasend    init    0            ; global audio send variable

giOn    =    1            ; ivals for on and off values
giOff    =    0


;PRESET_RHYTHMS-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

;1: WALTZ (in 3)
;                instr    strt    dur    amp
giBD01    ftgen    0,0,-12,-2,    101,    1,    1,    1,\
                101,    4,    1,    1,\
                -1,    4,    -1,    -1
                
giSn201    ftgen    0,0,-16,-2,    0,    1,    0,    1,\
                103,    2,    1,    1,\
                103,    3,    1,    1,\
                -1,    4,    -1,    -1
                
giCy201    ftgen    0,0,-12,-2,    106,    1,    1,    1,\
                106,    4,    1,    1,\
                -1,    4,    -1,    -1
                
giGu01    ftgen    0,0,-36,-2,    112,    1,    1,    0.7,\
                112,    2,    1,    0.7,\
                112,    3,    0.125,    1,\
                112,    3.5,    0.125,    1,\
                112,    4,    1,    0.7,\
                -1,    4,    -1,    -1
                
giQu01    ftgen    0,0,-12,-2,    0,    1,    0,    1,\
                114,    3,    1,    1,\
                -1,    4,    -1,    -1                                                     

giTa01    ftgen    0,0,-40,-2,    115,    1,    0.5,    1,\
                115,    1.5,    0.5,    1,\
                115,    2,    0.5,    1,\
                115,    2.5,    0.5,    1,\
                115,    3,    0.5,    1,\
                115,    3.5,    0.5,    1,\
                115,    4,    0.5,    1,\
                115,    4.5,    0.5,    1,\
                115,    4,    0.5,    1,\
                -1,    4,    -1,    -1

;2: SAMBA (4/4)
;                instr    strt    dur    amp
giBD02    ftgen    0,0,-16,-2,    101,    1,    1,    1,\
                101,    3,    1,    1,\
                101,    5,    1,    1,\
                -1,    5,    -1,    -1
                
giBo302    ftgen    0,0,-12,-2,    0,    1,    0,    1,\
                109,    4,    1,    1,\
                -1,    5,    -1,    -1         

giCo02    ftgen    0,0,-20,-2,    111,    1,    .1,    1,\
                111,    2.5,    .1,    1,\
                111,    4,    .1,    1,\
                111,    5,    .1,    0,\
                -1,    5,    -1,    -1

                
giGu02    ftgen    0,0,-32,-2,    112,    1,    1,    0.7,\
                112,    2,    0.125,    1,\
                112,    2.5,    0.125,    1,\
                112,    3,    1,    0.7,\
                112,    4,    0.125,    1,\
                112,    4.5,    0.125,    1,\
                112,    5,    1,    0.7,\
                -1,    5,    -1,    -1

giQu02    ftgen    0,0,-12,-2,    0,    1,    0,    1,\
                114,    4,    1,    1,\
                -1,    5,    -1,    -1         
                
giTa02    ftgen    0,0,40,-2,    115,    1,    0.5,    1,\
                115,    1.5,    0.5,    1,\
                115,    2,    0.5,    1,\
                115,    2.5,    0.5,    1,\
                115,    3,    0.5,    1,\
                115,    3.5,    0.5,    1,\
                115,    4,    0.5,    1,\
                115,    4.5,    0.5,    1,\
                115,    5,    0.5,    1,\
                -1,    5,    -1,    -1


;3: MAMBO (4/4)
;                instr    strt    dur    amp
giBD03    ftgen    0,0,-24,-2,    101,    1,    1,    1,\
                101,    2.5,    0.5,    1,\
                101,    3,    1,    1,\
                101,    4,    1,    1,\
                101,    5,    1,    1,\
                -1,    5,    -1,    -1
                
giBo303    ftgen    0,0,16,-2,    0,    1,    0,    1,\
                109,    4,    0.5,    1,\
                109,    4.5,    0.5,    1,\
                -1,    5,    -1,    -1         

giCo03    ftgen    0,0,-24,-2,    111,    1,    .1,    1,\
                111,    2,    .1,    1,\
                111,    3,    .1,    1,\
                111,    4,    .1,    1,\
                111,    5,    .1,    1,\
                -1,    5,    -1,    -1

                
giGu03    ftgen    0,0,-32,-2,    112,    1,    1,    0.7,\
                112,    2,    0.125,    1,\
                112,    2.5,    0.125,    1,\
                112,    3,    1,    0.7,\
                112,    4,    0.125,    1,\
                112,    4.5,    0.125,    1,\
                112,    5,    1,    0.7,\
                -1,    5,    -1,    -1

giQu03    ftgen    0,0,12,-2,    0,    1,    0,    1,\
                114,    4,    1,    1,\
                -1,    5,    -1,    -1         
                
giTa03    ftgen    0,0,40,-2,    115,    1,    0.5,    1,\
                115,    1.5,    0.5,    1,\
                115,    2,    0.5,    1,\
                115,    2.5,    0.5,    1,\
                115,    3,    0.5,    1,\
                115,    3.5,    0.5,    1,\
                115,    4,    0.5,    1,\
                115,    4.5,    0.5,    1,\
                115,    5,    0.5,    1,\
                -1,    5,    -1,    -1


;4: RUMBA (4/4)
;                instr    strt    dur    amp
giBD04    ftgen    0,0,-20,-2,    101,    1,    1,    1,\
                101,    3,    1,    1,\
                101,    4,    1,    1,\
                101,    5,    1,    1,\
                -1,    5,    -1,    -1
                
giCy104    ftgen    0,0,-28,-2,    105,    1,    0.5,    1,\
                105,    1.75,    0.5,    1,\
                105,    2,    0.5,    1,\
                105,    3,    0.5,    1,\
                105,    4,    0.5,    1,\
                105,    5,    0.5,    1,\
                -1,    5,    -1,    -1

giBo104    ftgen    0,0,-12,-2,    0,    1,    0,    1,\
                107,    2,    0.5,    1,\
                -1,    5,    -1,    -1         

giBo204    ftgen    0,0,-20,-2,    108,    1,    0,    1,\
                108,    2.5,    0.5,    1,\
                108,    3,    0.5,    1,\
                108,    5,    0.5,    1,\
                -1,    5,    -1,    -1         


giBo304    ftgen    0,0,-16,-2,    0,    1,    0,    1,\
                109,    4,    0.5,    1,\
                109,    4.5,    0.5,    1,\
                -1,    5,    -1,    -1         

giCl04    ftgen    0,0,-16,-2,    110,    1,    .1,    1,\
                110,    3,    .1,    1,\
                110,    5,    .1,    1,\
                -1,    5,    -1,    -1
                
giGu04    ftgen    0,0,-32,-2,    112,    1,    1,    0.7,\
                112,    2,    0.125,    1,\
                112,    2.5,    0.125,    1,\
                112,    3,    1,    0.7,\
                112,    4,    0.125,    1,\
                112,    4.5,    0.125,    1,\
                112,    5,    1,    0.7,\
                -1,    5,    -1,    -1

giQu04    ftgen    0,0,-12,-2,    0,    1,    0,    1,\
                114,    4,    1,    1,\
                -1,    5,    -1,    -1         
                
giTa04    ftgen    0,0,-40,-2,    115,    1,    0.5,    1,\
                115,    1.5,    0.5,    1,\
                115,    2,    0.5,    1,\
                115,    2.5,    0.5,    1,\
                115,    3,    0.5,    1,\
                115,    3.5,    0.5,    1,\
                115,    4,    0.5,    1,\
                115,    4.5,    0.5,    1,\
                115,    5,    0.5,    1,\
                -1,    5,    -1,    -1


;5: TANGO (4/4)
;                instr    strt    dur    amp
giBD05    ftgen    0,0,-24,-2,    101,    1,    1,    1,\
                101,    2,    1,    1,\
                101,    3,    1,    1,\
                101,    4,    1,    1,\
                101,    5,    1,    1,\
                -1,    5,    -1,    -1

giCy205    ftgen    0,0,-12,-2,    0,    1,    1,    1,\
                106,    4.5,    1,    1,\
                -1,    5,    -1,    -1
                
                
giGu05    ftgen    0,0,-32,-2,    112,    1,    1,    0.7,\
                112,    2,    0.125,    1,\
                112,    2.5,    0.125,    1,\
                112,    3,    1,    0.7,\
                112,    4,    0.125,    1,\
                112,    4.5,    0.125,    1,\
                112,    5,    1,    0.7,\
                -1,    5,    -1,    -1

giQu05    ftgen    0,0,-12,-2,    0,    1,    0,    1,\
                114,    4,    1,    1,\
                -1,    5,    -1,    -1         
                
giTa05    ftgen    0,0,-40,-2,    115,    1,    0.5,    1,\
                115,    1.5,    0.5,    1,\
                115,    2,    0.5,    1,\
                115,    2.5,    0.5,    1,\
                115,    3,    0.5,    1,\
                115,    3.5,    0.5,    1,\
                115,    4,    0.5,    1,\
                115,    4.5,    0.5,    1,\
                115,    5,    0.5,    1,\
                -1,    5,    -1,    -1

;6: BOSSANOVA (4/4)
;                instr    strt    dur    amp
giBD06    ftgen    0,0,-24,-2,    101,    1,    1,    1,\
                101,    2.5,    1,    1,\
                101,    3,    1,    1,\
                101,    4.5,    1,    1,\
                101,    5,    1,    1,\
                -1,    5,    -1,    -1

giRS06    ftgen    0,0,-12,-2,    104,    1,    1,    1,\
                104,    5,    1,    1,\
                -1,    5,    -1,    -1
                
                
giGu06    ftgen    0,0,-32,-2,    112,    1,    1,    0.7,\
                112,    2,    0.125,    1,\
                112,    2.5,    0.125,    1,\
                112,    3,    1,    0.7,\
                112,    4,    0.125,    1,\
                112,    4.5,    0.125,    1,\
                112,    5,    1,    0.7,\
                -1,    5,    -1,    -1

giQu06    ftgen    0,0,-12,-2,    0,    1,    0,    1,\
                114,    4,    1,    1,\
                -1,    5,    -1,    -1         
                
giTa06    ftgen    0,0,-40,-2,    115,    1,    0.5,    1,\
                115,    1.5,    0.5,    1,\
                115,    2,    0.5,    1,\
                115,    2.5,    0.5,    1,\
                115,    3,    0.5,    1,\
                115,    3.5,    0.5,    1,\
                115,    4,    0.5,    1,\
                115,    4.5,    0.5,    1,\
                115,    5,    0.5,    1,\
                -1,    5,    -1,    -1

;7: ROCK 1 (4/4)
;                instr    strt    dur    amp
giBD07    ftgen    0,0,-24,-2,    101,    1,    1,    1,\
                101,    2.5,    1,    1,\
                101,    3,    1,    1,\
                101,    4.5,    1,    1,\
                101,    5,    1,    1,\
                -1,    5,    -1,    -1

giSn107    ftgen    0,0,-12,-2,    0,    1,    1,    1,\
                102,    2,    1,    1,\
                -1,    5,    -1,    -1

giSn207    ftgen    0,0,-12,-2,    0,    1,    1,    1,\
                103,    4,    1,    1,\
                -1,    5,    -1,    -1
                
                
giGu07    ftgen    0,0,-32,-2,    112,    1,    1,    0.7,\
                112,    2,    0.125,    1,\
                112,    2.5,    0.125,    1,\
                112,    3,    1,    0.7,\
                112,    4,    0.125,    1,\
                112,    4.5,    0.125,    1,\
                112,    5,    1,    0.7,\
                -1,    5,    -1,    -1

giMa07    ftgen    0,0,-40,-2,    113,    1,    0.5,    1,\
                113,    1.5,    0.5,    1,\
                113,    2,    0.5,    1,\
                113,    2.5,    0.5,    1,\
                113,    3,    0.5,    1,\
                113,    3.5,    0.5,    1,\
                113,    4,    0.5,    1,\
                113,    4.5,    0.5,    1,\
                113,    5,    0.5,    1,\
                -1,    5,    -1,    -1

giQu07    ftgen    0,0,-12,-2,    0,    1,    0,    1,\
                114,    4,    1,    1,\
                -1,    5,    -1,    -1         
                
giTa07    ftgen    0,0,-40,-2,    115,    1,    0.5,    1,\
                115,    1.5,    0.5,    1,\
                115,    2,    0.5,    1,\
                115,    2.5,    0.5,    1,\
                115,    3,    0.5,    1,\
                115,    3.5,    0.5,    1,\
                115,    4,    0.5,    1,\
                115,    4.5,    0.5,    1,\
                115,    5,    0.5,    1,\
                -1,    5,    -1,    -1

;8: ROCK 2 (4/4)
;                instr    strt    dur    amp
giBD08    ftgen    0,0,-24,-2,    101,    1,    1,    1,\
                101,    2.5,    1,    1,\
                101,    3,    1,    1,\
                101,    4.5,    1,    1,\
                101,    5,    1,    1,\
                -1,    5,    -1,    -1

giSn108    ftgen    0,0,-16,-2,    0,    1,    1,    1,\
                102,    2,    1,    1,\
                102,    4,    1,    1,\
                -1,    5,    -1,    -1

giSn208    ftgen    0,0,-16,-2,    0,    1,    1,    1,\
                103,    2.75,    1,    1,\
                103,    3.25,    1,    1,\
                -1,    5,    -1,    -1
                
                
giGu08    ftgen    0,0,-32,-2,    112,    1,    1,    0.7,\
                112,    2,    0.125,    1,\
                112,    2.5,    0.125,    1,\
                112,    3,    1,    0.7,\
                112,    4,    0.125,    1,\
                112,    4.5,    0.125,    1,\
                112,    5,    1,    0.7,\
                -1,    5,    -1,    -1

giMa08    ftgen    0,0,-40,-2,    113,    1,    0.5,    1,\
                113,    1.5,    0.5,    1,\
                113,    2,    0.5,    1,\
                113,    2.5,    0.5,    1,\
                113,    3,    0.5,    1,\
                113,    3.5,    0.5,    1,\
                113,    4,    0.5,    1,\
                113,    4.5,    0.5,    1,\
                113,    5,    0.5,    1,\
                -1,    5,    -1,    -1

giQu08    ftgen    0,0,-12,-2,    0,    1,    0,    1,\
                114,    4,    1,    1,\
                -1,    5,    -1,    -1         
                
giTa08    ftgen    0,0,-40,-2,    115,    1,    0.5,    1,\
                115,    1.5,    0.5,    1,\
                115,    2,    0.5,    1,\
                115,    2.5,    0.5,    1,\
                115,    3,    0.5,    1,\
                115,    3.5,    0.5,    1,\
                115,    4,    0.5,    1,\
                115,    4.5,    0.5,    1,\
                115,    5,    0.5,    1,\
                -1,    5,    -1,    -1

;9: MARCH 2/4 (12/8?!)
;                instr    strt    dur    amp
giBD09    ftgen    0,0,-16,-2,    101,    1,    1,    1,\
                101,    3,    1,    1,\
                101,    5,    1,    1,\
                -1,    5,    -1,    -1

giSn209    ftgen    0,0,-16,-2,    0,    1,    1,    1,\
                103,    2,    1,    1,\
                103,    4,    1,    1,\
                -1,    5,    -1,    -1

giMa09    ftgen    0,0,-40,-2,    115,    1,    0.5,    1,\
                115,    1.667,    0.5,    1,\
                115,    2,    0.5,    1,\
                115,    2.667,    0.5,    1,\
                115,    3,    0.5,    1,\
                115,    3.667,    0.5,    1,\
                115,    4,    0.5,    1,\
                115,    4.667,    0.5,    1,\
                115,    5,    0.5,    1,\
                -1,    5,    -1,    -1                
                
giGu09    ftgen    0,0,-32,-2,    112,    1,    1,    0.7,\
                112,    2,    0.125,    1,\
                112,    2.667,    0.125,    1,\
                112,    3,    1,    0.7,\
                112,    4,    0.125,    1,\
                112,    4.667,    0.125,    1,\
                112,    5,    1,    0.7,\
                -1,    5,    -1,    -1

giQu09    ftgen    0,0,-12,-2,    0,    1,    0,    1,\
                114,    4,    1,    1,\
                -1,    5,    -1,    -1         

;10: Swing (4/4)
;                instr    strt    dur    amp
giBD10    ftgen    0,0,-24,-2,    101,    1,    1,    1,\
                101,    2,    1,    1,\
                101,    3,    1,    1,\
                101,    4,    1,    1,\
                101,    5,    1,    1,\
                -1,    5,    -1,    -1

giCy110    ftgen    0,0,-32,-2,    105,    1,    0.5,    1,\
                105,    2,    0.5,    1,\
                105,    2.75,    0.5,    1,\
                105,    3,    0.5,    1,\
                105,    4,    0.5,    1,\
                105,    4.75,    0.5,    1,\
                105,    5,    0.5,    1,\
                -1,    5,    -1,    -1                
                
giGu10    ftgen    0,0,-32,-2,    112,    1,    1,    0.7,\
                112,    2,    0.125,    1,\
                112,    2.75,    0.125,    1,\
                112,    3,    1,    0.7,\
                112,    4,    0.125,    1,\
                112,    4.75,    0.125,    1,\
                112,    5,    1,    0.7,\
                -1,    5,    -1,    -1

giQu10    ftgen    0,0,-12,-2,    0,    1,    0,    1,\
                114,    4,    1,    1,\
                -1,    5,    -1,    -1         

;11: Jazz Waltz (3/4)
;                instr    strt    dur    amp
giBD11    ftgen    0,0,-12,-2,    101,    1,    1,    1,\
                101,    4,    1,    1,\
                -1,    4,    -1,    -1

giSn211    ftgen    0,0,-16,-2,    0,    1,    0.5,    1,\
                103,    1.5,    0.5,    1,\
                103,    2,    0.5,    1,\
                -1,    4,    -1,    -1                

giCy211    ftgen    0,0,-12,-2,    106,    1,    1,    1,\
                106,    4,    1,    1,\
                -1,    4,    -1,    -1                

                
giGu11    ftgen    0,0,-20,-2,    112,    1,    1,    0.7,\
                112,    2.5,    0.125,    1,\
                112,    3.5,    0.125,    1,\
                112,    4,    1,    0.7,\
                -1,    4,    -1,    -1

giQu11    ftgen    0,0,-12,-2,    0,    1,    0,    1,\
                114,    3,    1,    1,\
                -1,    4,    -1,    -1         

giTa11    ftgen    0,0,-32,-2,    115,    1,    0.5,    1,\
                115,    1.5,    0.5,    1,\
                115,    2,    0.5,    1,\
                115,    2.5,    0.5,    1,\
                115,    3,    0.5,    1,\
                115,    3.5,    0.5,    1,\
                115,    4,    0.5,    1,\
                -1,    4,    -1,    -1

;12: Meringue (4/4)
;                instr    strt    dur    amp
giBD12    ftgen    0,0,-16,-2,    101,    1,    1,    1,\
                101,    2.5,    1,    1,\
                101,    3,    1,    1,\
                -1,    5,    -1,    -1

giBo312    ftgen    0,0,-16,-2,    0,    1,    1,    1,\
                109,    4,    1,    1,\
                0,    5,    1,    1,\
                -1,    5,    -1,    -1                


giCo12    ftgen    0,0,-16,-2,    111,    1,    1,    1,\
                111,    2.5,    1,    1,\
                111,    3,    1,    1,\
                -1,    5,    -1,    -1                

                
giGu12    ftgen    0,0,-32,-2,    112,    1,    1,    0.7,\
                112,    2,    0.125,    1,\
                112,    2.5,    0.125,    1,\
                112,    3,    1,    0.7,\
                112,    4,    0.125,    1,\
                112,    4.5,    0.125,    1,\
                112,    5,    1,    0.7,\
                -1,    5,    -1,    -1

giQu12    ftgen    0,0,-12,-2,    0,    1,    0,    1,\
                114,    4,    1,    1,\
                -1,    5,    -1,    -1         

giTa12    ftgen    0,0,-40,-2,    115,    1,    0.5,    1,\
                115,    1.5,    0.5,    1,\
                115,    2,    0.5,    1,\
                115,    2.5,    0.5,    1,\
                115,    3,    0.5,    1,\
                115,    3.5,    0.5,    1,\
                115,    4,    0.5,    1,\
                115,    4.5,    0.5,    1,\
                115,    5,    0.5,    1,\
                -1,    5,    -1,    -1

;13: Cha Cha (4/4)
;                instr    strt    dur    amp
giBD13    ftgen    0,0,-20,-2,    101,    1,    1,    1,\
                101,    2.5,    1,    1,\
                101,    3,    1,    1,\
                101,    4,    1,    1,\
                -1,    5,    -1,    -1

giBo313    ftgen    0,0,-16,-2,    0,    1,    1,    1,\
                109,    4,    1,    1,\
                109,    4.5,    1,    1,\
                -1,    5,    -1,    -1                

giCo13    ftgen    0,0,-28,-2,    111,    1,    1,    1,\
                111,    2,    1,    1,\
                111,    3,    0.5,    1,\
                111,    3.5,    0.5,    1,\
                111,    4,    1,    1,\
                111,    5,    1,    1,\
                -1,    5,    -1,    -1                

giGu13    ftgen    0,0,-32,-2,    112,    1,    1,    0.7,\
                112,    2,    0.125,    1,\
                112,    2.5,    0.125,    1,\
                112,    3,    1,    0.7,\
                112,    4,    0.125,    1,\
                112,    4.5,    0.125,    1,\
                112,    5,    1,    0.7,\
                -1,    5,    -1,    -1

giQu13    ftgen    0,0,-12,-2,    0,    1,    0,    1,\
                114,    4,    1,    1,\
                -1,    5,    -1,    -1         

giTa13    ftgen    0,0,-40,-2,    115,    1,    0.5,    1,\
                115,    1.5,    0.5,    1,\
                115,    2,    0.5,    1,\
                115,    2.5,    0.5,    1,\
                115,    3,    0.5,    1,\
                115,    3.5,    0.5,    1,\
                115,    4,    0.5,    1,\
                115,    4.5,    0.5,    1,\
                115,    5,    0.5,    1,\
                -1,    5,    -1,    -1


;14: Beguine (4/4)
;                instr    strt    dur    amp
giBD14    ftgen    0,0,-20,-2,    101,    1,    1,    1,\
                101,    2.5,    1,    1,\
                101,    3,    1,    1,\
                101,    4,    1,    1,\
                -1,    5,    -1,    -1

giCy214    ftgen    0,0,-12,-2,    0,    1,    1,    1,\
                106,    1.5,    1,    1,\
                -1,    5,    -1,    -1

giBo114    ftgen    0,0,-24,-2,    0,    1,    1,    1,\
                107,    1.5,    1,    1,\
                107,    2.5,    1,    1,\
                107,    3.5,    1,    1,\
                107,    4.5,    1,    1,\
                -1,    5,    -1,    -1                

giBo214    ftgen    0,0,-20,-2,    108,    1,    1,    1,\
                108,    3,    1,    1,\
                108,    4,    1,    1,\
                108,    5,    1,    1,\
                -1,    5,    -1,    -1                

giBo314    ftgen    0,0,-16,-2,    0,    1,    1,    1,\
                109,    4,    0.5,    1,\
                109,    4.5,    0.5,    1,\
                -1,    5,    -1,    -1                

giGu14    ftgen    0,0,-32,-2,    112,    1,    1,    0.7,\
                112,    2,    0.125,    1,\
                112,    2.5,    0.125,    1,\
                112,    3,    1,    0.7,\
                112,    4,    0.125,    1,\
                112,    4.5,    0.125,    1,\
                112,    5,    1,    0.7,\
                -1,    5,    -1,    -1

giQu14    ftgen    0,0,-12,-2,    0,    1,    0,    1,\
                114,    4,    1,    1,\
                -1,    5,    -1,    -1         

giTa14    ftgen    0,0,-40,-2,    115,    1,    0.5,    1,\
                115,    1.5,    0.5,    1,\
                115,    2,    0.5,    1,\
                115,    2.5,    0.5,    1,\
                115,    3,    0.5,    1,\
                115,    3.5,    0.5,    1,\
                115,    4,    0.5,    1,\
                115,    4.5,    0.5,    1,\
                115,    5,    0.5,    1,\
                -1,    5,    -1,    -1

;15: Habanera (4/4)
;                instr    strt    dur    amp
giBD15    ftgen    0,0,-20,-2,    101,    1,    1,    1,\
                101,    2.5,    1,    1,\
                101,    3,    1,    1,\
                101,    4,    1,    1,\
                -1,    5,    -1,    -1

giCy215    ftgen    0,0,-12,-2,    106,    1,    1,    1,\
                106,    5,    1,    1,\
                -1,    5,    -1,    -1
                
giGu15    ftgen    0,0,-32,-2,    112,    1,    1,    0.7,\
                112,    2,    0.125,    1,\
                112,    2.5,    0.125,    1,\
                112,    3,    1,    0.7,\
                112,    4,    0.125,    1,\
                112,    4.5,    0.125,    1,\
                112,    5,    1,    0.7,\
                -1,    5,    -1,    -1

giQu15    ftgen    0,0,-12,-2,    0,    1,    0,    1,\
                114,    4,    1,    1,\
                -1,    5,    -1,    -1         

giTa15    ftgen    0,0,-40,-2,    115,    1,    0.5,    1,\
                115,    1.5,    0.5,    1,\
                115,    2,    0.5,    1,\
                115,    2.5,    0.5,    1,\
                115,    3,    0.5,    1,\
                115,    3.5,    0.5,    1,\
                115,    4,    0.5,    1,\
                115,    4.5,    0.5,    1,\
                115,    5,    0.5,    1,\
                -1,    5,    -1,    -1

;16: Slow Rock (4/4)
;                instr    strt    dur    amp
giBD16    ftgen    0,0,-24,-2,    101,    1,    1,    1,\
                101,    2.667,    0.333,    1,\
                101,    3,    1,    1,\
                101,    4.667,    0.333,    1,\
                101,    5,    1,    1,\
                -1,    5,    -1,    -1

giRS16    ftgen    0,0,-16,-2,    0,    1,    1,    1,\
                104,    2,    1,    1,\
                104,    4,    1,    1,\
                -1,    5,    -1,    -1


giCy216    ftgen    0,0,-12,-2,    106,    1,    1,    1,\
                106,    5,    1,    1,\
                -1,    5,    -1,    -1
                
giGu16    ftgen    0,0,-32,-2,    112,    1,    1,    0.7,\
                112,    2,    0.125,    1,\
                112,    2.5,    0.125,    1,\
                112,    3,    1,    0.7,\
                112,    4,    0.125,    1,\
                112,    4.5,    0.125,    1,\
                112,    5,    1,    0.7,\
                -1,    5,    -1,    -1

giQu16    ftgen    0,0,-12,-2,    0,    1,    0,    1,\
                114,    4,    1,    1,\
                -1,    5,    -1,    -1         

giTa16    ftgen    0,0,-(14*4),-2,    115,    1,    0.3,    1,\
                115,    1.333,    0.3,    1,\
                115,    1.667,    0.3,    1,\
                115,    2,    0.3,    1,\
                115,    2.333,    0.3,    1,\
                115,    2.667,    0.3,    1,\
                115,    3,    0.3,    1,\
                115,    3.333,    0.3,    1,\
                115,    3.667,    0.3,    1,\
                115,    4,    0.3,    1,\
                115,    4.333,    0.3,    1,\
                115,    4.667,    0.3,    1,\
                115,    5,    0.3,    1,\
                -1,    5,    -1,    -1

;17: Rock 3 (4/4)
;                instr    strt    dur    amp
giBD17    ftgen    0,0,-24,-2,    101,    1,    1,    1,\
                101,    2.5,    0.5,    1,\
                101,    3,    1,    1,\
                101,    4.5,    0.5,    1,\
                101,    5,    1,    1,\
                -1,    5,    -1,    -1

giSn117    ftgen    0,0,-16,-2,    0,    1,    1,    1,\
                102,    2,    1,    1,\
                102,    3.5,    1,    1,\
                -1,    5,    -1,    -1
                
giGu17    ftgen    0,0,-32,-2,    112,    1,    1,    0.7,\
                112,    2,    0.125,    1,\
                112,    2.5,    0.125,    1,\
                112,    3,    1,    0.7,\
                112,    4,    0.125,    1,\
                112,    4.5,    0.125,    1,\
                112,    5,    1,    0.7,\
                -1,    5,    -1,    -1

giQu17    ftgen    0,0,-12,-2,    0,    1,    0,    1,\
                114,    4,    1,    1,\
                -1,    5,    -1,    -1         

giTa17    ftgen    0,0,-40,-2,    115,    1,    0.5,    1,\
                115,    1.5,    0.5,    1,\
                115,    2,    0.5,    1,\
                115,    2.5,    0.5,    1,\
                115,    3,    0.5,    1,\
                115,    3.5,    0.5,    1,\
                115,    4,    0.5,    1,\
                115,    4.5,    0.5,    1,\
                115,    5,    0.5,    1,\
                -1,    5,    -1,    -1

;18: Rock 4 (4/4)
;                instr    strt    dur    amp
giBD18    ftgen    0,0,-24,-2,    101,    1,    1,    1,\
                101,    3,    1,    1,\
                101,    4,    0.5,    1,\
                101,    4.5,    1,    1,\
                101,    5,    1,    1,\
                -1,    5,    -1,    -1

giSn118    ftgen    0,0,-16,-2,    0,    1,    1,    1,\
                102,    2,    1,    1,\
                102,    3.5,    1,    1,\
                -1,    5,    -1,    -1

giSn218    ftgen    0,0,-12,-2,    0,    1,    1,    1,\
                103,    2.75,    1,    1,\
                -1,    5,    -1,    -1
                
giGu18    ftgen    0,0,-32,-2,    112,    1,    1,    0.7,\
                112,    2,    0.125,    1,\
                112,    2.5,    0.125,    1,\
                112,    3,    1,    0.7,\
                112,    4,    0.125,    1,\
                112,    4.5,    0.125,    1,\
                112,    5,    1,    0.7,\
                -1,    5,    -1,    -1

giQu18    ftgen    0,0,-12,-2,    0,    1,    0,    1,\
                114,    4,    1,    1,\
                -1,    5,    -1,    -1         

giTa18    ftgen    0,0,-40,-2,    115,    1,    0.5,    1,\
                115,    1.5,    0.5,    1,\
                115,    2,    0.5,    1,\
                115,    2.5,    0.5,    1,\
                115,    3,    0.5,    1,\
                115,    3.5,    0.5,    1,\
                115,    4,    0.5,    1,\
                115,    4.5,    0.5,    1,\
                115,    5,    0.5,    1,\
                -1,    5,    -1,    -1


;19: March 6/8 (4/4)
;                instr    strt    dur    amp
giBD19    ftgen    0,0,-24,-2,    101,    1,    1,    1,\
                101,    2,    1,    1,\
                101,    3,    1,    1,\
                101,    4,    1,    1,\
                101,    5,    1,    1,\
                -1,    5,    -1,    -1

giCy119    ftgen    0,0,-40,-2,    105,    1,    0.3,    1,\
                105,    1.667,    0.3,    1,\
                105,    2,    0.3,    1,\
                105,    2.667,    0.3,    1,\
                105,    3,    0.3,    1,\
                105,    3.667,    0.3,    1,\
                105,    4,    0.3,    1,\
                105,    4.667,    0.3,    1,\
                105,    5,    0.3,    1,\
                -1,    5,    -1,    -1

giGu19    ftgen    0,0,-32,-2,    112,    1,    1,    0.7,\
                112,    2,    0.125,    1,\
                112,    2.667,    0.125,    1,\
                112,    3,    1,    0.7,\
                112,    4,    0.125,    1,\
                112,    4.667,    0.125,    1,\
                112,    5,    1,    0.7,\
                -1,    5,    -1,    -1

giQu19    ftgen    0,0,-12,-2,    0,    1,    0,    1,\
                114,    4,    1,    1,\
                -1,    5,    -1,    -1         


;20: Latin Swing (4/4)
;                instr    strt    dur    amp
giBD20    ftgen    0,0,-20,-2,    101,    1,    1,    1,\
                101,    3,    1,    1,\
                101,    4,    1,    1,\
                101,    5,    1,    1,\
                -1,    5,    -1,    -1

giCy120    ftgen    0,0,-32,-2,    105,    1,    0.3,    1,\
                105,    2,    0.3,    1,\
                105,    2.75,    0.3,    1,\
                105,    3,    0.3,    1,\
                105,    4,    0.3,    1,\
                105,    4.75,    0.3,    1,\
                105,    5,    0.3,    1,\
                -1,    5,    -1,    -1

giBo120    ftgen    0,0,-12,-2,    0,    1,    0.3,    1,\
                107,    2,    0.3,    1,\
                -1,    5,    -1,    -1

giBo320    ftgen    0,0,-16,-2,    0,    1,    0.5,    1,\
                109,    4,    0.5,    1,\
                109,    4.75,    0.25,    1,\
                -1,    5,    -1,    -1

                
giGu20    ftgen    0,0,-32,-2,    112,    1,    1,    0.7,\
                112,    2,    0.125,    1,\
                112,    2.5,    0.125,    1,\
                112,    3,    1,    0.7,\
                112,    4,    0.125,    1,\
                112,    4.5,    0.125,    1,\
                112,    5,    1,    0.7,\
                -1,    5,    -1,    -1

giQu20    ftgen    0,0,-12,-2,    0,    1,    0,    1,\
                114,    4,    1,    1,\
                -1,    5,    -1,    -1         

giTa20    ftgen    0,0,-40,-2,    115,    1,    0.5,    1,\
                115,    1.5,    0.5,    1,\
                115,    2,    0.5,    1,\
                115,    2.5,    0.5,    1,\
                115,    3,    0.5,    1,\
                115,    3.5,    0.5,    1,\
                115,    4,    0.5,    1,\
                115,    4.5,    0.5,    1,\
                115,    5,    0.5,    1,\
                -1,    5,    -1,    -1

;-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

instr    1    ; read widgets
    gktempo        chnget    "tempo"

    ; volume controls
    gkbassdrum    chnget    "bassdrum"
    gksnare1    chnget    "snare1"    
    gksnare2    chnget    "snare2"    
    gkrimshot    chnget    "rimshot"   
    gkcymbal1    chnget    "cymbal1"   
    gkcymbal2    chnget    "cymbal2"   
    gkbongo1    chnget    "bongo1"    
    gkbongo2    chnget    "bongo2"    
    gkbongo3    chnget    "bongo3"    
    gkclaves    chnget    "claves"    
    gkcowbell    chnget    "cowbell"   
    gkguiro        chnget    "guiro"     
    gkmaracas    chnget    "maracas"   
    gkquijada    chnget    "quijada"   
    gktambourine    chnget    "tambourine"

    ; mute switches
    gkmute1        chnget    "mute1"
    gkmute2        chnget    "mute2"
    gkmute3        chnget    "mute3"
    gkmute4        chnget    "mute4"
    gkmute5        chnget    "mute5"
    gkmute6        chnget    "mute6"
    gkmute7        chnget    "mute7"
    gkmute8        chnget    "mute8"
    gkmute9        chnget    "mute9"
    gkmute10    chnget    "mute10"
    gkmute11    chnget    "mute11"
    gkmute12    chnget    "mute12"
    gkmute13    chnget    "mute13"
    gkmute14    chnget    "mute14"
    gkmute15    chnget    "mute15"

    ; solo switches
    gksolo1        chnget    "solo1"
    gksolo2        chnget    "solo2"
    gksolo3        chnget    "solo3"
    gksolo4        chnget    "solo4"
    gksolo5        chnget    "solo5"
    gksolo6        chnget    "solo6"
    gksolo7        chnget    "solo7"
    gksolo8        chnget    "solo8"    
    gksolo9        chnget    "solo9"    
    gksolo10    chnget    "solo10"    
    gksolo11    chnget    "solo11"    
    gksolo12    chnget    "solo12"    
    gksolo13    chnget    "solo13"    
    gksolo14    chnget    "solo14"    
    gksolo15    chnget    "solo15"    

    ; add up all solo switch values (if greater than '1' then a solo button must be active)
    kSoloSum    =    gksolo1+gksolo2+gksolo3+gksolo4+gksolo5+gksolo6+gksolo7+gksolo8+gksolo9+gksolo10+gksolo11+gksolo12+gksolo13+gksolo14+gksolo15

    ; macro in which the play/don't play status for each instrument is defined
#define    SOLO_MUTE_STATUS(N)    
    #
    if gksolo$N==1 then            ; if solo button is on...
     gkstatus$N    =    1        ; ... definitely play this instrument
    elseif kSoloSum>0 then            ; otherwise if any one or more of the other solo buttons is active
     gkstatus$N    =    0        ; mute this instrument
    else                    ; otherwise
     gkstatus$N    =    (1-gkmute$N)    ; status defined by the inverse of the mute button value
    endif
    #
    ; expand macro for each instrument
    $SOLO_MUTE_STATUS(1)
    $SOLO_MUTE_STATUS(2)
    $SOLO_MUTE_STATUS(3)
    $SOLO_MUTE_STATUS(4)
    $SOLO_MUTE_STATUS(5)
    $SOLO_MUTE_STATUS(6)
    $SOLO_MUTE_STATUS(7)
    $SOLO_MUTE_STATUS(8)
    $SOLO_MUTE_STATUS(9)
    $SOLO_MUTE_STATUS(10)
    $SOLO_MUTE_STATUS(11)
    $SOLO_MUTE_STATUS(12)
    $SOLO_MUTE_STATUS(13)
    $SOLO_MUTE_STATUS(14)
    $SOLO_MUTE_STATUS(15)
    
    ; rhythm buttons
    gkbut1        chnget    "but1"
    gkbut2        chnget    "but2"
    gkbut3        chnget    "but3"
    gkbut4        chnget    "but4"
    gkbut5        chnget    "but5"
    gkbut6        chnget    "but6"
    gkbut7        chnget    "but7"
    gkbut8        chnget    "but8"
    gkbut9        chnget    "but9"
    gkbut10        chnget    "but10"

    ; derive a value for upper/lower row by using the two switches (r1 and r2) as radio buttons
    gkUpperLower    CabbageRadio2    "r1","r2"    ; call UDO

    ; miscellaneos controls    
    gkvolume    chnget    "volume"
    gkdist        chnget    "dist"
    gkcomp        chnget    "comp"
    gkfold        chnget    "fold"
    gkOnOff        chnget    "OnOff"  
    gkcutup        chnget    "cutup"

    ktrig1    trigger    gkOnOff,0.5,0        ; if on/off switch is turned 'on' generate a '1'
    ktrig2    trigger    gkcutup,0.5,0        ; if 'Mash' button is turned 'on' generate a '1'

    schedkwhen    ktrig1+ktrig2,0,0,  3,0,-1    ; start rhythm generater instrument (with a held note) whenever either the on/off or the 'Mash' button are turned on.

    schedkwhen    ktrig2,0,0,199,0,-1        ; start bbcut instrument if Mash button is turned on.
endin


instr    2    ;READ MIDI AND TRIGGER NOTES
    inum    notnum                        ;READ IN MIDI NOTE NUMBER
    idb    veloc    -30,0                    ;READ IN MIDI NOTE VELOCITY (WILL BE USED AS A DECIBEL VALUE)
    iinstr    wrap    inum,0,15                ;MAP MIDI NOTE NUMBERS (RANGE: 0 - 127) TO DRUM SOUND NUMBER (0 - 14)
    if iinstr==11 then                    ;if this is instr 11 (guiro). i.e. sustains and responds to midi note release...
     event_i    "i",101+iinstr,0,3600,ampdbfs(idb)    ;CALL guiro INSTRUMENT AND SEND AMPLITUDE VALUE AS p4
     krel    release                        ;sense not release
     if krel==1 then                    ;if this note is released...
      turnoff2    101+iinstr,0,1                ;...turn off the subinstrumet
     endif
    else                            ;otherwise (any instrument besides the guiro) i.e. no sustain and no midi release
     event_i    "i",101+iinstr,0,0.956,ampdbfs(idb)    ;CALL THE RELEVANT INSTRUMENT AND SEND AMPLITUDE VALUE AS p4
    endif
endin

instr    3    ; rhythm sequencer instrument
    if gkOnOff==0 then    ; if on/off switch is off...
     turnoff        ; turnoff this instrument
    endif
    
    if i(gkcutup)==1 then                ;start bbcut instr when sequencer is started (if Mash button is on)
     event_i    "i",199,0,-1
    endif
    
    kBPS    =    gktempo/60            ;DERIVE BEATS-PER-SECOND 

    if gkcutup==1 then                ;while bbcut is active...
     ktrig    changed    gktempo                ;sense for changes in the tempo button
     if ktrig==1 then                ;if the tempo has changed...
      reinit RESTART_PHASORS            ;restart the rhythm sequencer phasors
      event    "i",199,0,-1                ;restart bbcut instrument
     endif
     RESTART_PHASORS:
    endif
    kphase3    phasor  kBPS/3            ;3/4 phasor (range 0 - 1)
    kphase3    =    kphase3 * 3            ;reScale to the range 0 - 3
    kphase4    phasor  kBPS/4            ;4/4 phasor (range 0 - 1)
    kphase4    =    kphase4 * 4            ;reScale tot he range 0 - 4
    rireturn
    
    kphase    =    kphase4                ;default phase mode (4/4)
    
    ;lower buttons always take precedence so start with higher button in 'if' interrogations
    if     gkbut10==1 then
     kphase    =    kphase4
    endif
    if     gkbut9==1 then
     kphase    =    kphase4
    endif
    if     gkbut8==1 then
     kphase    =    kphase4
    endif
    if     gkbut7==1 then
     kphase    =    kphase4
    endif
    if     gkbut6==1 then
     kphase    =    kphase4
    endif
    if     gkbut5==1 then
     kphase    =    kphase4
    endif
    if     gkbut4==1 then
     kphase    =    kphase4
    endif
    if     gkbut3==1 then
     kphase    =    kphase4
    endif
    if     gkbut2==1 then
     kphase    =    kphase4
    endif
    if     gkbut1==1 then
     kphase    =    kphase3
    endif
    
    ;initialise the p fields (timedseqs p-field outputs are also input variables)
    kpBD_1        init    0
    kpBD_2        init    0
    kpBD_3        init    0
    kpBD_4        init    0

    kpSn1_1        init    0
    kpSn1_2        init    0
    kpSn1_3        init    0
    kpSn1_4        init    0

    kpSn2_1        init    0
    kpSn2_2        init    0
    kpSn2_3        init    0
    kpSn2_4        init    0

    kpRS_1        init    0
    kpRS_2        init    0
    kpRS_3        init    0
    kpRS_4        init    0

    kpCy1_1        init    0
    kpCy1_2        init    0
    kpCy1_3        init    0
    kpCy1_4        init    0
    
    kpCy2_1        init    0
    kpCy2_2        init    0
    kpCy2_3        init    0
    kpCy2_4        init    0
    
    kpBo1_1        init    0
    kpBo1_2        init    0
    kpBo1_3        init    0
    kpBo1_4        init    0
    
    kpBo2_1        init    0
    kpBo2_2        init    0
    kpBo2_3        init    0
    kpBo2_4        init    0
    
    kpBo3_1        init    0
    kpBo3_2        init    0
    kpBo3_3        init    0
    kpBo3_4        init    0
    
    kpCl_1        init    0
    kpCl_2        init    0
    kpCl_3        init    0
    kpCl_4        init    0
    
    kpCo_1        init    0
    kpCo_2        init    0
    kpCo_3        init    0
    kpCo_4        init    0
    
    kpGu_1        init    0
    kpGu_2        init    0
    kpGu_3        init    0
    kpGu_4        init    0
    
    kpMa_1        init    0
    kpMa_2        init    0
    kpMa_3        init    0
    kpMa_4        init    0
    
    kpQu_1        init    0
    kpQu_2        init    0
    kpQu_3        init    0
    kpQu_4        init    0
    
    kpTa_1        init    0
    kpTa_2        init    0
    kpTa_3        init    0
    kpTa_4        init    0


    ;initialise note triggers
    ktriggerBD    =    0
    ktriggerSn1    =    0
    ktriggerSn2    =    0
    ktriggerRS    =    0
    ktriggerCy1    =    0
    ktriggerCy2    =    0
    ktriggerBo1    =    0
    ktriggerBo2    =    0
    ktriggerBo3    =    0
    ktriggerCo    =    0
    ktriggerCl    =    0
    ktriggerMa    =    0
    ktriggerGu    =    0
    ktriggerQu    =    0
    ktriggerTa    =    0
    
    kmintim    init    0.01    ;minimum allowed time between note triggering (to prevent erroneous double triggerings)
    
    ; macro within which an instrument is scanned
#define    SCAN_INSTRUMENT(NAME'NUM)
    #
    ktrigger    timedseq    kphase+0.99, gi$NAME.$NUM, kp$NAME._1, kp$NAME._2,kp$NAME._3,kp$NAME._4    ; read table using phasor
    ktrigger$NAME    =    ktrigger$NAME + ktrigger                            ; add trigger for this instrument to the accumulator for all instruments 
    #
    
    ; read instrument rhythm tables
    if gkUpperLower==1 then            ; upper rhythm selected

     if gkbut1==1 then    ;WALTZ
      $SCAN_INSTRUMENT(BD'01)
      $SCAN_INSTRUMENT(Sn2'01)
      $SCAN_INSTRUMENT(Cy2'01)
      $SCAN_INSTRUMENT(Gu'01)
      $SCAN_INSTRUMENT(Qu'01)
      $SCAN_INSTRUMENT(Ta'01)
     endif  


     if gkbut2==1 then    ;SAMBA
      $SCAN_INSTRUMENT(BD'02)
      $SCAN_INSTRUMENT(Bo3'02)
      $SCAN_INSTRUMENT(Co'02)
      $SCAN_INSTRUMENT(Qu'02)
      $SCAN_INSTRUMENT(Gu'02)
      $SCAN_INSTRUMENT(Ta'02)
     endif

     if gkbut3==1 then    ;MAMBO
      $SCAN_INSTRUMENT(BD'03)
      $SCAN_INSTRUMENT(Bo3'03)
      $SCAN_INSTRUMENT(Co'03)
      $SCAN_INSTRUMENT(Qu'03)
      $SCAN_INSTRUMENT(Gu'03)
      $SCAN_INSTRUMENT(Ta'03)
     endif

     if gkbut4==1 then    ;RUMBA
      $SCAN_INSTRUMENT(BD'04)
      $SCAN_INSTRUMENT(Cy1'04)
      $SCAN_INSTRUMENT(Bo1'04)
      $SCAN_INSTRUMENT(Bo2'04)
      $SCAN_INSTRUMENT(Bo3'04)
      $SCAN_INSTRUMENT(Cl'04)
      $SCAN_INSTRUMENT(Qu'04)
      $SCAN_INSTRUMENT(Gu'04)
      $SCAN_INSTRUMENT(Ta'04)
     endif

     if gkbut5==1 then    ;TANGO
      $SCAN_INSTRUMENT(BD'05)
      $SCAN_INSTRUMENT(Cy2'05)
      $SCAN_INSTRUMENT(Qu'04)
      $SCAN_INSTRUMENT(Gu'04)
      $SCAN_INSTRUMENT(Ta'05)
     endif

      if gkbut6==1 then    ;BOSSANOVA
      $SCAN_INSTRUMENT(BD'06)
      $SCAN_INSTRUMENT(RS'06)
      $SCAN_INSTRUMENT(Qu'06)
      $SCAN_INSTRUMENT(Gu'06)
      $SCAN_INSTRUMENT(Ta'06)
     endif

      if gkbut7==1 then    ;ROCK 1
      $SCAN_INSTRUMENT(BD'07)
      $SCAN_INSTRUMENT(Sn1'07)
      $SCAN_INSTRUMENT(Sn2'07)
      $SCAN_INSTRUMENT(Ma'08)
      $SCAN_INSTRUMENT(Qu'07)
      $SCAN_INSTRUMENT(Gu'07)
      $SCAN_INSTRUMENT(Ta'07)
     endif

      if gkbut8==1 then    ;ROCK 2
      $SCAN_INSTRUMENT(BD'08)
      $SCAN_INSTRUMENT(Sn1'08)
      $SCAN_INSTRUMENT(Sn2'08)
      $SCAN_INSTRUMENT(Ma'08)
      $SCAN_INSTRUMENT(Qu'08)
      $SCAN_INSTRUMENT(Gu'08)
      $SCAN_INSTRUMENT(Ta'08)
     endif

      if gkbut9==1 then    ;MARCH 2/4
      $SCAN_INSTRUMENT(BD'09)
      $SCAN_INSTRUMENT(Sn2'09)
      $SCAN_INSTRUMENT(Ma'09)
      $SCAN_INSTRUMENT(Qu'09)
      $SCAN_INSTRUMENT(Gu'09)
     endif

      if gkbut10==1 then    ;SWING
      $SCAN_INSTRUMENT(BD'10)
      $SCAN_INSTRUMENT(Cy1'10)
      $SCAN_INSTRUMENT(Qu'10)
      $SCAN_INSTRUMENT(Gu'10)
     endif

     ; START OF BAR GREEN LED INDICATOR    
       ktrigger    trigger    kphase,0.01,0        ; indicator trigger
     schedkwhen    ktrigger, 0, 0, 150, 0, 0    ; call indicator 'on' instrument (which then calls indicator off instrument)

    else                        ; lower rhythm selected

     if gkbut1==1 then    ;JAZZ WALTZ
      $SCAN_INSTRUMENT(BD'11)
      $SCAN_INSTRUMENT(Sn2'11)
      $SCAN_INSTRUMENT(Cy2'11)
      $SCAN_INSTRUMENT(Gu'11)
      $SCAN_INSTRUMENT(Qu'11)
      $SCAN_INSTRUMENT(Ta'11)
     endif  

     if gkbut2==1 then    ;MERINGUE
      $SCAN_INSTRUMENT(BD'12)
      $SCAN_INSTRUMENT(Bo3'12)
      $SCAN_INSTRUMENT(Co'12)
      $SCAN_INSTRUMENT(Gu'12)
      $SCAN_INSTRUMENT(Qu'12)
      $SCAN_INSTRUMENT(Ta'12)
     endif  

     if gkbut3==1 then    ;CHA CHA
      $SCAN_INSTRUMENT(BD'13)
      $SCAN_INSTRUMENT(Bo3'13)
      $SCAN_INSTRUMENT(Co'13)
      $SCAN_INSTRUMENT(Gu'13)
      $SCAN_INSTRUMENT(Qu'13)
      $SCAN_INSTRUMENT(Ta'13)
     endif  

     if gkbut4==1 then    ;BEGUINE
      $SCAN_INSTRUMENT(BD'14)
      $SCAN_INSTRUMENT(Cy2'14)
      $SCAN_INSTRUMENT(Bo1'14)
      $SCAN_INSTRUMENT(Bo2'14)
      $SCAN_INSTRUMENT(Bo3'14)
      $SCAN_INSTRUMENT(Gu'14)
      $SCAN_INSTRUMENT(Qu'14)
      $SCAN_INSTRUMENT(Ta'14)
     endif  

     if gkbut5==1 then    ;HABANERA
      $SCAN_INSTRUMENT(BD'15)
      $SCAN_INSTRUMENT(Cy2'15)
      $SCAN_INSTRUMENT(Gu'15)
      $SCAN_INSTRUMENT(Qu'15)
      $SCAN_INSTRUMENT(Ta'15)
     endif  

     if gkbut6==1 then    ;SLOW ROCK
      $SCAN_INSTRUMENT(BD'16)
      $SCAN_INSTRUMENT(RS'16)
      $SCAN_INSTRUMENT(Gu'16)
      $SCAN_INSTRUMENT(Qu'16)
      $SCAN_INSTRUMENT(Ta'16)
     endif  

     if gkbut7==1 then    ;ROCK 3
      $SCAN_INSTRUMENT(BD'17)
      $SCAN_INSTRUMENT(Sn1'17)
      $SCAN_INSTRUMENT(Gu'17)
      $SCAN_INSTRUMENT(Qu'17)
      $SCAN_INSTRUMENT(Ta'17)
     endif  

     if gkbut8==1 then    ;ROCK 4
      $SCAN_INSTRUMENT(BD'18)
      $SCAN_INSTRUMENT(Sn1'18)
      $SCAN_INSTRUMENT(Sn2'18)
      $SCAN_INSTRUMENT(Gu'18)
      $SCAN_INSTRUMENT(Qu'18)
      $SCAN_INSTRUMENT(Ta'18)
     endif  

     if gkbut9==1 then    ;MARCH 6/8
      $SCAN_INSTRUMENT(BD'19)
      $SCAN_INSTRUMENT(Cy1'19)
      $SCAN_INSTRUMENT(Gu'19)
      $SCAN_INSTRUMENT(Qu'19)
     endif  

     if gkbut10==1 then    ;LATIN SWING
      $SCAN_INSTRUMENT(BD'20)
      $SCAN_INSTRUMENT(Cy1'20)
      $SCAN_INSTRUMENT(Bo1'20)
      $SCAN_INSTRUMENT(Bo3'20)
      $SCAN_INSTRUMENT(Gu'20)
      $SCAN_INSTRUMENT(Qu'20)
      $SCAN_INSTRUMENT(Ta'20)
     endif  


     ; START OF BAR RED LED INDICATOR    
       ktrigger    trigger    kphase,0.01,0        ; indicator trigger
     schedkwhen    ktrigger, 0, 0, 160, 0, 0    ; call indicator 'on' instrument (which then calls indicator off instrument)

    endif

    ; play instruments according to triggers and p-fields read from the tables
    schedkwhen    ktriggerBD*kpBD_1*gkstatus1, kmintim, 0, kpBD_1, 0, 0.001,kpBD_4
    schedkwhen    ktriggerSn1*kpSn1_1*gkstatus2, kmintim, 0, kpSn1_1, 0, 0.001,kpSn1_4
    schedkwhen    ktriggerSn2*kpSn2_1*gkstatus3, kmintim, 0, kpSn2_1, 0, 0.001,kpSn2_4
    schedkwhen    ktriggerRS*kpRS_1*gkstatus4, kmintim, 0, kpRS_1, 0, 0.001,kpRS_4
    schedkwhen    ktriggerCy1*kpCy1_1*gkstatus5, kmintim, 0, kpCy1_1, 0, 0.001,kpCy1_4
    schedkwhen    ktriggerCy2*kpCy2_1*gkstatus6, kmintim, 0, kpCy2_1, 0, 0.001,kpCy2_4
    schedkwhen    ktriggerBo1*kpBo1_1*gkstatus7, kmintim, 0, kpBo1_1, 0, 0.001,kpBo1_4
    schedkwhen    ktriggerBo2*kpBo2_1*gkstatus8, kmintim, 0, kpBo2_1, 0, 0.001,kpBo2_4
    schedkwhen    ktriggerBo3*kpBo3_1*gkstatus9, kmintim, 0, kpBo3_1, 0, 0.001,kpBo3_4
    schedkwhen    ktriggerCo*kpCo_1*gkstatus10, kmintim, 0, kpCo_1, 0, 0.001,kpCo_4
    schedkwhen    ktriggerCl*kpCl_1*gkstatus11, kmintim, 0, kpCl_1, 0, 0.001,kpCl_4
    kpGu_3        limit        kpGu_3*(60/gktempo),0.04,300                ;duration of guiro read from table and is also dependent upon tempo
    schedkwhen    ktriggerGu*kpGu_1*gkstatus12, kmintim, 0, kpGu_1, 0, kpGu_3,kpGu_4
    schedkwhen    ktriggerMa*kpMa_1*gkstatus13, kmintim, 0, kpMa_1, 0, 0.001,kpMa_4
    schedkwhen    ktriggerQu*kpQu_1*gkstatus14, kmintim, 0, kpQu_1, 0, 0.001,kpQu_4
    schedkwhen    ktriggerTa*kpTa_1*gkstatus15, kmintim, 0, kpTa_1, 0, 0.001,kpTa_4
endin

giFlashTime    =    0.1

instr    101    ; Korg Minipops 7: Bass Drum
;    chnset    giOn,"Act1"                    ; turn instrument indicator on
;    scoreline_i    {{ i 201 0.1 0 "Act1"}}            ; call instrument to turn off indicator after a short delay
    xtratim    giFlashTime
    krelease    release                ;SENSE RELEASE OF THIS NOTE ('1' WHEN RELEASED, OTHERWISE ZERO)   
    chnset    1-krelease,"Act1"                   ;TURN ON ACTIVE LIGHT WHEN NOTE STARTS, TURN IT OFF WHEN NOTE ENDS
    p3    =    0.437
    kactive active    p1                    ; turnoff older active notes
    ;if kactive>1 then
    ; turnoff
    ;endif
    ifrq    =    64                    ; fundemantal pitch
    asig    mpulse    1,0                    ; generate a click impulse
    asig    reson    asig,ifrq,ifrq*0.001,2            ; reson filter it
    asig    tone    asig,500                ; lowpass filter it
    aenv    transeg    1,p3,-14,0                ; create an amplitude envelope
    asig    =    asig * aenv * 225 * p4 * i(gkbassdrum)    ; scale signal
    gasend    =    gasend + asig                ; add to accumulating global audio send variable
endin

instr    102    ; Korg Minipops 7: Snare 1
;    chnset    giOn,"Act2"                    ; turn instrument indicator on
;    scoreline_i    {{ i 201 0.1 0 "Act2"}}             ; call instrument to turn off indicator after a short delay
    xtratim    giFlashTime
    krelease    release                ;SENSE RELEASE OF THIS NOTE ('1' WHEN RELEASED, OTHERWISE ZERO)   
    chnset    1-krelease,"Act2"                   ;TURN ON ACTIVE LIGHT WHEN NOTE STARTS, TURN IT OFF WHEN NOTE ENDS
    kactive active    p1                    ; turnoff older overlapping notes
    ;if kactive>1 then
    ; turnoff
    ;endif
    p3    =    0.387
    
    ; body of the sound (duration governed by p3)
    anoise    pinkish    1                    ; generate some pink noise
    asig    reson    anoise,6250,9000,1            ; reson filter it
    aenv    transeg    1,p3,-5,0                ; create an amplitude envelope
    asig    =    asig * aenv                
    asig    buthp    asig,3000
    
    ; 'crack' at the beginning of the sound (duration governed by idur)
    idur    =    0.006
    acps    transeg    800,idur,-4,60                ; pitch envelope
    astrike    poscil    1,acps,gisine                ; sine oscillator 'crack'
    aenv2    transeg    1,idur,-2,0                ; amplitude envelope
    astrike    =    astrike * aenv2                ; apply amplitude envelope
    asig    =    ( ( astrike * 0.7 ) + (asig * 2 ) ) * p4 * i(gksnare1)    ; balance the two sound components and scale the sound mixture
    gasend    =    gasend + asig                ; add to accumulating global audio send variable
endin

instr    103    ; Korg Minipops 7: Snare 2
;    chnset    giOn,"Act3"                ; turn instrument indicator on                             
;    scoreline_i    {{ i 201 0.1 0 "Act3"}}         ; call instrument to turn off indicator after a short delay
    xtratim    giFlashTime
    krelease    release                ;SENSE RELEASE OF THIS NOTE ('1' WHEN RELEASED, OTHERWISE ZERO)   
    chnset    1-krelease,"Act3"                   ;TURN ON ACTIVE LIGHT WHEN NOTE STARTS, TURN IT OFF WHEN NOTE ENDS
    kactive active    p1
    ;if kactive>1 then
    ; turnoff
    ;endif
    p3    =    0.4
    
    anoise    pinkish    1
    ;asig    reson    anoise,5200,5000,1
    asig    butbp    anoise,5000,8000
    aenv    transeg    1,p3,-8,0
    asig    =    asig * aenv*p4
    asig    buthp    asig,3000
    
    idur    =    0.005
    acps    transeg    800,idur,-4,200
    astrike    poscil    1,acps,gisine
    aenv2    transeg    1,idur,-2,0             
    astrike    =    astrike * aenv2
    asig    =    ((astrike*0.5) + (asig*2.3)) * p4 * i(gksnare2)
    gasend    =    gasend + asig
endin

instr    104    ;  Korg Minipops 7: Rimshot
;    chnset    giOn,"Act4"                ; turn instrument indicator on                             
;    scoreline_i    {{ i 201 0.1 0 "Act4"}}         ; call instrument to turn off indicator after a short delay
    xtratim    giFlashTime
    krelease    release                ;SENSE RELEASE OF THIS NOTE ('1' WHEN RELEASED, OTHERWISE ZERO)   
    chnset    1-krelease,"Act4"                   ;TURN ON ACTIVE LIGHT WHEN NOTE STARTS, TURN IT OFF WHEN NOTE ENDS
    kactive active    p1
    ;if kactive>1 then
    ; turnoff
    ;endif
    p3    =    0.005
    icps    =    1700
    aenv    expon    1,p3,0.0001
    iphs    =    0.2
    asig    oscili    aenv,icps,gisine,iphs
    asig2    reson    asig,icps,1500,2
    asig    buthp    ( (asig2 * 0.4 ) + asig ) * 0.3 * p4 * i(gkrimshot), 500
    gasend    =    gasend + asig
endin

instr    105    ; Korg Minipops 7: Cymbal 1
;    chnset    giOn,"Act5"                ; turn instrument indicator on                             
;    scoreline_i    {{ i 201 0.1 0 "Act5"}}         ; call instrument to turn off indicator after a short delay
    xtratim    giFlashTime
    krelease    release                ;SENSE RELEASE OF THIS NOTE ('1' WHEN RELEASED, OTHERWISE ZERO)   
    chnset    1-krelease,"Act5"                   ;TURN ON ACTIVE LIGHT WHEN NOTE STARTS, TURN IT OFF WHEN NOTE ENDS
    kactive active    p1
    ;if kactive>1 then
    ; turnoff
    ;endif
    p3    =    0.304
    aenv    transeg    1,p3,-2,0
    anoise    dust2    1, 30000            ;CREATE NOISE SIGNAL    
    icf    =    6000
    asig1    reson    anoise*aenv,icf,icf*0.7,1
    asig1    butlp    asig1,14000
    asig2    buthp    (asig1+(anoise*0.001))*aenv,6000
    asig    =    asig2 * aenv * 0.5 * p4 * i(gkcymbal1)
    gasend    =    gasend + asig
endin

instr    106    ; Korg Minipops 7: Cymbal 2
;    chnset    giOn,"Act6"                ; turn instrument indicator on                             
;    scoreline_i    {{ i 201 0.1 0 "Act6"}}         ; call instrument to turn off indicator after a short delay
    xtratim    giFlashTime
    krelease    release                ;SENSE RELEASE OF THIS NOTE ('1' WHEN RELEASED, OTHERWISE ZERO)   
    chnset    1-krelease,"Act6"                   ;TURN ON ACTIVE LIGHT WHEN NOTE STARTS, TURN IT OFF WHEN NOTE ENDS
    kactive active    p1
    ;if kactive>1 then
    ; turnoff
    ;endif
    p3    =    1.404

    aenv    transeg    1,p3,-2,0
    anoise    dust2    1, 10000
    icf    =    5000
    asig    reson    anoise*aenv,icf,icf*0.9,1
    asig    tone    asig,12000
    asig    buthp    asig*aenv,6000
    asig    =    asig*aenv

    idur    =    0.004
    acps    transeg    1000,idur,-4,400
    astrike    poscil    1,acps,gisine
    aenv2    transeg    1,idur,-2,0
    astrike    =    astrike * aenv2

    asig    =    ( (astrike * 0.2 ) + (asig * 1.5 ) ) * p4 * i(gkcymbal2)
    gasend    =    gasend + asig
endin

instr    107    ; Minipops: Bongo 1
;    chnset    giOn,"Act7"                ; turn instrument indicator on                             
;    scoreline_i    {{ i 201 0.1 0 "Act7"}}         ; call instrument to turn off indicator after a short delay
    xtratim    giFlashTime
    krelease    release                ;SENSE RELEASE OF THIS NOTE ('1' WHEN RELEASED, OTHERWISE ZERO)   
    chnset    1-krelease,"Act7"                   ;TURN ON ACTIVE LIGHT WHEN NOTE STARTS, TURN IT OFF WHEN NOTE ENDS
    kactive active    p1
    ;if kactive>1 then
    ; turnoff
    ;endif
    p3    =    0.2
    aenv    transeg    1,p3,13,0
    asig    mpulse    1,0
    icps    =    630;588
    asig    reson    asig,icps,icps*0.03,2
    asig    buthp    asig,300
    asig    butlp    asig,8000    
    asig    =    asig * 4 * aenv * p4 * i(gkbongo1)
    gasend    =    gasend + asig
endin

instr    108    ; Korg Minipops 7: Bongo 2
;    chnset    giOn,"Act8"                ; turn instrument indicator on                             
;    scoreline_i    {{ i 201 0.1 0 "Act8"}}         ; call instrument to turn off indicator after a short delay
    xtratim    giFlashTime
    krelease    release                ;SENSE RELEASE OF THIS NOTE ('1' WHEN RELEASED, OTHERWISE ZERO)   
    chnset    1-krelease,"Act8"                   ;TURN ON ACTIVE LIGHT WHEN NOTE STARTS, TURN IT OFF WHEN NOTE ENDS
    kactive active    p1
    ;if kactive>1 then
    ; turnoff
    ;endif
    p3    =    0.3
    kcps    expon    400,p3,390    ; bongo 2 freq. and that of claves (body) should correspond
    aenv    transeg    1,p3-0.005,0,0.1,0.005,0, 0
    asig    mpulse    1,0
    asig    reson    asig,kcps,kcps*0.03,2
    asig    tone    asig,5000
    asig    buthp    asig,100
    asig    =    asig * 4 * aenv * p4 * i(gkbongo2)
    gasend    =    gasend + asig
endin

instr    109    ; Korg Minipops 7: Bongo 3
;    chnset    giOn,"Act9"                ; turn instrument indicator on                             
;    scoreline_i    {{ i 201 0.1 0 "Act9"}}         ; call instrument to turn off indicator after a short delay
    xtratim    giFlashTime
    krelease    release                ;SENSE RELEASE OF THIS NOTE ('1' WHEN RELEASED, OTHERWISE ZERO)   
    chnset    1-krelease,"Act9"                   ;TURN ON ACTIVE LIGHT WHEN NOTE STARTS, TURN IT OFF WHEN NOTE ENDS
    kactive active    p1
    ;if kactive>1 then
    ; turnoff
    ;endif
    p3    =    1.229
    icps    =    194;187
    aenv    transeg    0, 0.001, -2, 1, p3-0.001, -2, 0
    asig    mpulse    1,0
    kbw    linseg    0.05,0.01,0.008
    asig    reson    asig,icps,icps*kbw,2
    asig    butlp    asig,11000
    asig    =    asig * 5 * aenv * p4 * i(gkbongo3)
    gasend    =    gasend + asig
endin

instr    110    ; Korg Minipops 7: Claves
;    chnset    giOn,"Act10"                ; turn instrument indicator on                             
;    scoreline_i    {{ i 201 0.1 0 "Act10"}}        ; call instrument to turn off indicator after a short delay
    xtratim    giFlashTime
    krelease    release                ;SENSE RELEASE OF THIS NOTE ('1' WHEN RELEASED, OTHERWISE ZERO)   
    chnset    1-krelease,"Act10"                   ;TURN ON ACTIVE LIGHT WHEN NOTE STARTS, TURN IT OFF WHEN NOTE ENDS
    kactive active    p1
    ;if kactive>1 then
    ; turnoff
    ;endif
    p3    =    0.186
    aenv    line    1,p3,0
    asig    mpulse    1,0
    icps1    =    400;410    ; body -         ; bongo 2 freq. and that of claves (body) should correspond
    asig1    reson    asig,icps1,icps1*0.025,2
    icps2    =    2180    ; strike
    asig2    reson    asig,icps2,icps2*0.03,2
    asig    =    (asig1 + (asig2 * 1.3) ) * 3.2 * aenv * p4 * i(gkclaves)
    gasend    =    gasend + asig
endin

instr    111    ; Korg Minipops 7: Cowbell
;    chnset    giOn,"Act11"                ; turn instrument indicator on                             
;    scoreline_i    {{ i 201 0.1 0 "Act11"}}        ; call instrument to turn off indicator after a short delay
    xtratim    giFlashTime
    krelease    release                ;SENSE RELEASE OF THIS NOTE ('1' WHEN RELEASED, OTHERWISE ZERO)   
    chnset    1-krelease,"Act11"                   ;TURN ON ACTIVE LIGHT WHEN NOTE STARTS, TURN IT OFF WHEN NOTE ENDS
    kactive active    p1
    ;if kactive>1 then
    ; turnoff
    ;endif
    p3    =    0.3
    aenv    line    1,p3,0
    astrike    mpulse    1,0
    icps1    =    850
    asig1    reson    astrike,icps1,icps1*0.007,2    ;low (body)
    icps2    =    4707
    asig2    reson    astrike,icps2,icps2*0.03,2    ;high (strike)
    amix    =    asig1 + (asig2 * 0.8)
    amix    buthp    amix, 100
    asig    =    amix * aenv * p4 * i(gkcowbell) * 3
    gasend    =    gasend + asig
endin

instr    112    ; Korg Minipops 7: Guiro
;    chnset    giOn,"Act12"                ; turn instrument indicator on                             
;    scoreline_i    {{ i 201 0.1 0 "Act12"}}        ; call instrument to turn off indicator after a short delay
    xtratim    giFlashTime
    krelease    release                ;SENSE RELEASE OF THIS NOTE ('1' WHEN RELEASED, OTHERWISE ZERO)   
    chnset    1-krelease,"Act12"                   ;TURN ON ACTIVE LIGHT WHEN NOTE STARTS, TURN IT OFF WHEN NOTE ENDS
    kactive active    p1
    ;if kactive>1 then        
    ; turnoff
    ;endif
    idur    =    (p3=3600?0.256:p3*0.9)    
    kcps    transeg    66,idur,2,77
    asig    vco2    0.1,kcps,6;,0.0005
    icf    =    2000
    asig    reson    asig,4300,3000,1
    asig    buthp    asig,1000
    aenv    linseg    0,0.001,1,p3-0.111,0.6,0.1,1,0.01,0
    asig    =    asig * 0.012 * aenv * i(gkguiro) * p4
    gasend    =    gasend + asig
endin

instr    113    ; Korg Minipops 7: Maracas
;    chnset    giOn,"Act13"                ; turn instrument indicator on                             
;    scoreline_i    {{ i 201 0.1 0 "Act13"}}        ; call instrument to turn off indicator after a short delay
    xtratim    giFlashTime
    krelease    release                ;SENSE RELEASE OF THIS NOTE ('1' WHEN RELEASED, OTHERWISE ZERO)   
    chnset    1-krelease,"Act13"                   ;TURN ON ACTIVE LIGHT WHEN NOTE STARTS, TURN IT OFF WHEN NOTE ENDS
    kactive active    p1
    ;if kactive>1 then
    ; turnoff
    ;endif
    p3    =    0.05
    asig    noise    1,0.04
    icf    =    5000
    asig    reson    asig,9000,4000,2
    asig    buthp    asig,2000
    aenv    transeg    1,p3,-4,0
    asig    =    asig * aenv * 0.35 * p4 * i(gkmaracas)
    gasend    =    gasend + asig
endin

instr    114    ; Korg Minipops 7: Quijada
;    chnset    giOn,"Act14"                ; turn instrument indicator on                             
;    scoreline_i    {{ i 201 0.1 0 "Act14"}}        ; call instrument to turn off indicator after a short delay
    xtratim    giFlashTime
    krelease    release                ;SENSE RELEASE OF THIS NOTE ('1' WHEN RELEASED, OTHERWISE ZERO)   
    chnset    1-krelease,"Act14"                   ;TURN ON ACTIVE LIGHT WHEN NOTE STARTS, TURN IT OFF WHEN NOTE ENDS
    kactive active    p1
    ;if kactive>1 then
    ; turnoff
    ;endif
    p3    =    0.817

    idur1    =    0.817
    apulse1    mpulse    1,1/22.7272
    kenv1    transeg    0.8,0.05,1, 1,idur1-0.05,-6,0
    apulse1    reson    apulse1,2727,400,1

    idur2    =    0.32
    apulse2    mpulse    1,1/13.1579
    kenv2    transeg    0.8,0.05,1, 1,idur2-0.05,-6,0
    apulse2    reson    apulse2,2727,400,1
    
    amix    =    ((apulse1 * kenv1 * 4) + (apulse2 * kenv2 * 4)) * i(gkquijada) * p4
    asig    buthp    amix, 550
    gasend    =    gasend + asig
endin

instr    115    ; Korg Minipops 7: Tambourine
;    chnset    giOn,"Act15"                ; turn instrument indicator on                             
;    scoreline_i    {{ i 201 0.1 0 "Act15"}}        ; call instrument to turn off indicator after a short delay
    xtratim    giFlashTime
    krelease    release                ;SENSE RELEASE OF THIS NOTE ('1' WHEN RELEASED, OTHERWISE ZERO)   
    chnset    1-krelease,"Act15"                   ;TURN ON ACTIVE LIGHT WHEN NOTE STARTS, TURN IT OFF WHEN NOTE ENDS
    kactive active    p1
    ;if kactive>1 then
    ; turnoff
    ;endif
    p3    =    0.271
    aenv    transeg    1,p3,-8,0
    ;aenv    linseg    1, 0.02, 1, 0.01, 0.4, p3-0.02-0.01, 0 
    anoise    noise    1,0
    asig    reson    anoise*aenv,4600, 100,2
    asig    buthp    asig+(anoise*0.1*aenv),7000
    asig    reson    asig,9000,3000,1
    asig    =    asig * aenv * 1.5 * i(gktambourine) * p4
    gasend    =    gasend + asig
endin




instr    150    ; green indicator on
    chnset    giOn,"Indic1"
    event_i    "i",p1+1,0.1,0
endin

instr    151    ; green indicator off
    chnset    giOff,"Indic1"
endin

instr    160    ; green indicator on
    chnset    giOn,"Indic2"
    event_i    "i",p1+1,0.1,0
endin

instr    161    ; green indicator off
    chnset    giOff,"Indic2"
endin

instr    199
    if gkcutup==0 then    ; if 'mash' button is off...
     turnoff        ; ...turn this instrument off
    endif
    
    ; input parameters for bbcut 
    ;; << 4/4 >>
    isubdiv        =    8
    ibarlen        =    2     
    iphrase        =    8     
    irepeats       =    2     
    istutspd       =    4     
    istutchnc      =    1     

    ;; << 12/8 >>
    ;isubdiv        =    12
    ;ibarlen        =    4     
    ;iphrase        =    4     
    ;irepeats       =    3     
    ;istutspd       =    3     
    ;istutchnc      =    1     

     asig    bbcutm    gasend,   i(gktempo)/60, isubdiv,  ibarlen,  iphrase, irepeats, istutspd, istutchnc
    gasend    =    gasend + asig

    if gkOnOff==0 then    ; if main on/off button is turned off...
     turnoff        ; ... turn this instrument off too
    endif
endin

instr    200    ; output
    kon    =    1
    koff    =    0

    kporttime    linseg    0,0.001,0.05

    gasend    =    gasend * 3

     ktrig    changed    gkdist
     if ktrig==1 then
      reinit RESTART_DIST
     endif
     RESTART_DIST:
     if i(gkdist)>0 then
      chnset    giOn,"DistOn"
     else
      chnset    giOff,"DistOn"
     endif
     if gkdist>0 then
      gasend    clip        gasend,0,1-(i(gkdist)*0.99)
      rireturn
      gasend    dam         gasend, 0.25,1,   1.35,   0.01, 0.01    ; compress the signal (whenever distortion is on) 
     endif
     ;                              thr  rat1 rat2 tim1  tim2


    ; foldover effect (downsampling)
    if gkfold>0 then
     chnset    kon,"FoldOn"
    else
     chnset    koff,"FoldOn"
    endif
    gkfold    portk    gkfold,kporttime
    if gkfold>0 then
     gasend    fold        gasend,1+(gkfold*300)    
    endif
    ktrig    changed    gkfold

    gasend    tone        gasend,10000    ; cut high frequencies
    gasend    atone        gasend,100    ; cut low frequencies
    
    gasend    =    gasend * gkvolume

        outs    gasend,gasend
        clear    gasend
endin


instr    201
    Sname    =    p4
    chnset    giOff,Sname
endin

</CsInstruments>  
<CsScore>
i 1 0 [3600*24*7]    ; read widgets
i 200 0 [3600*24*7]    ; output
</CsScore>
</CsoundSynthesizer>