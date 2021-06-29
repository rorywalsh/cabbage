
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; TR-808.csd
; Written by Iain McCurdy, 2012
; 
; Saving patterns saves tables as text file to the pwd. Each save to a new pattern number creates a new text file on disk for that pattern.

; Choosing 'Host Control' allows the VST plugin host to decide 'Tempo' and 'Run/Stop' status

<Cabbage>
form caption("TR-808") size(800, 400), colour("SlateGrey"), pluginId("T808"), guiRefresh(32)

label         bounds(  5,385,120, 12), text("Iain McCurdy |2012|"), fontColour("black")

groupbox bounds( 0,  0,  50, 225), text("B.Drum"),fontColour("black"), colour(192,192,192), plant(1) {
checkbox bounds(10, 24,   5,   5), colour("yellow"), channel("Act1"),  value(0), shape("ellipse"), active(0)
rslider  bounds( 5, 25,  45,  45), text("Level"), textColour("black"), colour(80,100,150), channel("level1"), range(0, 2, 1,1,0.01)
rslider  bounds( 5, 75,  45,  45), text("Tune"),  textColour("black"), colour(80,100,150), channel("tune1"),  range(-2, 2, 0,1,0.01)
rslider  bounds( 5,125,  45,  45), text("Decay"), textColour("black"), colour(80,100,150), channel("dur1"),   range(0.25, 2, 1, 0.8)
rslider  bounds( 5,175,  45,  45), text("Pan"),   textColour("black"), colour(80,100,150), channel("pan1"),   range(0, 1, 0.5,1,0.01)
}
groupbox bounds(50,  0,  50, 225), text("Snare"), fontColour("black"),colour(202,202,202), plant(2) {
checkbox bounds(10, 24,   5,   5), colour("yellow"), channel("Act2"),  value(0), shape("ellipse"), active(0)
rslider  bounds( 5, 25,  45,  45), text("Level"), textColour("black"), colour(80,100,150), channel("level2"), range(0, 2, 1,1,0.01)
rslider  bounds( 5, 75,  45,  45), text("Tune"),  textColour("black"), colour(80,100,150), channel("tune2"),  range(-2, 2, 0,1,0.01)
rslider  bounds( 5,125,  45,  45), text("Decay"), textColour("black"), colour(80,100,150), channel("dur2"),   range(0.25, 2, 1, 0.8)
rslider  bounds( 5,175,  45,  45), text("Pan"),   textColour("black"), colour(80,100,150), channel("pan2"),   range(0, 1, 0.5,1,0.01)
}
groupbox bounds(100,  0,  50, 225), text("O. HH"), fontColour("black"), colour(212,212,212), plant(3) {
checkbox bounds(10, 24,   5,   5), colour("yellow"), channel("Act3"),  value(0), shape("ellipse"), active(0)
rslider  bounds( 5, 25,  45,  45), text("Level"),  textColour("black"), colour(80,100,150), channel("level3"), range(0, 2, 1,1,0.01)
rslider  bounds( 5, 75,  45,  45), text("Tune"),   textColour("black"), colour(80,100,150), channel("tune3"),  range(-2, 2, 0,1,0.01)
rslider  bounds( 5,125,  45,  45), text("Decay"),  textColour("black"), colour(80,100,150), channel("dur3"),   range(0.25, 2, 1, 0.8)
rslider  bounds( 5,175,  45,  45), text("Pan"),    textColour("black"), colour(80,100,150), channel("pan3"),  range(0, 1, 0.5,1,0.01)
}                                
groupbox bounds(150,  0,  50, 225), text("Cl. HH"),fontColour("black"),colour(222,222,222), plant(4) {
checkbox bounds(10, 24,   5,   5), colour("yellow"), channel("Act4"),  value(0), shape("ellipse"), active(0)
rslider  bounds( 5, 25,  45,  45), text("Level"),  textColour("black"), colour(80,100,150), channel("level4"), range(0, 2, 1,1,0.01)
rslider  bounds( 5, 75,  45,  45), text("Tune"),   textColour("black"), colour(80,100,150), channel("tune4"),  range(-2, 2, 0,1,0.01)
rslider  bounds( 5,125,  45,  45), text("Decay"),  textColour("black"), colour(80,100,150), channel("dur4"),   range(0.25, 2, 1, 0.8)
rslider  bounds( 5,175,  45,  45), text("Pan"),    textColour("black"), colour(80,100,150), channel("pan4"),  range(0, 1, 0.5,1,0.01)
}
groupbox bounds(200,  0,  50, 225), text("H. Tom"), fontColour("black"),colour(232,232,232), plant(5) {
checkbox bounds(10, 24,   5,   5), colour("yellow"), channel("Act5"),  value(0), shape("ellipse"), active(0)
rslider  bounds( 5, 25,  45,  45), text("Level"),  textColour("black"), colour(80,100,150), channel("level5"), range(0, 2, 1,1,0.01)
rslider  bounds( 5, 75,  45,  45), text("Tune"),   textColour("black"), colour(80,100,150), channel("tune5"), range(-2, 2, 0,1,0.01)
rslider  bounds( 5,125,  45,  45), text("Decay"),  textColour("black"), colour(80,100,150), channel("dur5"),   range(0.25, 2, 1, 0.8)
rslider  bounds( 5,175,  45,  45), text("Pan"),    textColour("black"), colour(80,100,150), channel("pan5"),  range(0, 1, 0.5,1,0.01)
}
groupbox bounds(250,  0,  50, 225), text("M. Tom"),fontColour("black"), colour(242,242,242), plant(6) {
checkbox bounds(10, 24,   5,   5), colour("yellow"), channel("Act6"),  value(0), shape("ellipse"), active(0)
rslider  bounds( 5, 25,  45,  45), text("Level"),  textColour("black"), colour(80,100,150), channel("level6"), range(0, 2, 1,1,0.01)
rslider  bounds( 5, 75,  45,  45), text("Tune"),   textColour("black"), colour(80,100,150), channel("tune6"), range(-2, 2, 0,1,0.01)
rslider  bounds( 5,125,  45,  45), text("Decay"),  textColour("black"), colour(80,100,150), channel("dur6"), range(0.25, 2, 1, 0.8)
rslider  bounds( 5,175,  45,  45), text("Pan"),    textColour("black"), colour(80,100,150), channel("pan6"),  range(0, 1, 0.5,1,0.01)
}
groupbox bounds(300,  0,  50, 225), text("L. Tom"),fontColour("black"), colour(232,232,232), plant(7) {
checkbox bounds(10, 24,   5,   5), colour("yellow"), channel("Act7"),  value(0), shape("ellipse"), active(0)
rslider  bounds( 5, 25,  45,  45), text("Level"),  textColour("black"), colour(80,100,150), channel("level7"), range(0, 2, 1,1,0.01)
rslider  bounds( 5, 75,  45,  45), text("Tune"),   textColour("black"), colour(80,100,150), channel("tune7"), range(-2, 2, 0,1,0.01)
rslider  bounds( 5,125,  45,  45), text("Decay"),  textColour("black"), colour(80,100,150), channel("dur7"), range(0.25, 2, 1, 0.8)
rslider  bounds( 5,175,  45,  45), text("Pan"),    textColour("black"), colour(80,100,150), channel("pan7"),  range(0, 1, 0.5,1,0.01)
}
groupbox bounds(350,  0,  50, 225), text("Cymbal"), fontColour("black"),colour(222,222,222), plant(8) {
checkbox bounds(10, 24,   5,   5), colour("yellow"), channel("Act8"),  value(0), shape("ellipse"), active(0)
rslider  bounds( 5, 25,  45,  45), text("Level"),  textColour("black"), colour(80,100,150), channel("level8"), range(0, 2, 1,1,0.01)
rslider  bounds( 5, 75,  45,  45), text("Tune"),   textColour("black"), colour(80,100,150), channel("tune8"),  range(-2, 2, 0,1,0.01)
rslider  bounds( 5,125,  45,  45), text("Decay"),  textColour("black"), colour(80,100,150), channel("dur8"),  range(0.25, 2, 1, 0.8)
rslider  bounds( 5,175,  45,  45), text("Pan"),    textColour("black"), colour(80,100,150), channel("pan8"),  range(0, 1, 0.5,1,0.01)
}
groupbox bounds(400,  0,  50, 225), text("Rim Shot"),fontColour("black"), colour(212,212,212), plant(9) {
checkbox bounds(10, 24,   5,   5), colour("yellow"), channel("Act9"),  value(0), shape("ellipse"), active(0)
rslider  bounds( 5, 25,  45,  45), text("Level"),   textColour("black"), colour(80,100,150), channel("level9"), range(0, 2, 1,1,0.01)
rslider  bounds( 5, 75,  45,  45), text("Tune"),    textColour("black"), colour(80,100,150), channel("tune9"),  range(-2, 2, 0,1,0.01)
rslider  bounds( 5,125,  45,  45), text("Decay"),   textColour("black"), colour(80,100,150), channel("dur9"),  range(0.25, 2, 1, 0.8)
rslider  bounds( 5,175,  45,  45), text("Pan"),     textColour("black"), colour(80,100,150), channel("pan9"),  range(0, 1, 0.5,1,0.01)
}
groupbox bounds(450,  0,  50, 225), text("Claves"), fontColour("black"),colour(202,202,202), plant(10) {
checkbox bounds(10, 24,   5,   5), colour("yellow"), channel("Act10"),  value(0), shape("ellipse"), active(0)
rslider  bounds( 5, 25,  45,  45), text("Level"),  textColour("black"), colour(80,100,150), channel("level10"), range(0, 2, 1,1,0.01)
rslider  bounds( 5, 75,  45,  45), text("Tune"),   textColour("black"), colour(80,100,150), channel("tune10"),  range(-2, 2, 0,1,0.01)
rslider  bounds( 5,125,  45,  45), text("Decay"),  textColour("black"), colour(80,100,150), channel("dur10"),  range(0.25, 2, 1, 0.8)
rslider  bounds( 5,175,  45,  45), text("Pan"),    textColour("black"), colour(80,100,150), channel("pan10"), range(0, 1, 0.5,1,0.01)
}
groupbox bounds(500,  0,  50, 225), text("Cowbell"),fontColour("black"), colour(192,192,192), plant(11) {
checkbox bounds(10, 24,   5,   5), colour("yellow"), channel("Act11"),  value(0), shape("ellipse"), active(0)
rslider  bounds( 5, 25,  45,  45), text("Level"),  textColour("black"), colour(80,100,150), channel("level11"), range(0, 2, 1,1,0.01)
rslider  bounds( 5, 75,  45,  45), text("Tune"),   textColour("black"), colour(80,100,150), channel("tune11"),  range(-2, 2, 0,1,0.01)
rslider  bounds( 5,125,  45,  45), text("Decay"),  textColour("black"), colour(80,100,150), channel("dur11"),  range(0.25, 2, 1, 0.8)
rslider  bounds( 5,175,  45,  45), text("Pan"),    textColour("black"), colour(80,100,150), channel("pan11"), range(0, 1, 0.5,1,0.01)
}
groupbox bounds(550,  0,  50, 225), text("Clap"), fontColour("black"),colour(182,182,182), plant(12) {
checkbox bounds(10, 24,   5,   5), colour("yellow"), channel("Act12"),  value(0), shape("ellipse"), active(0)
rslider  bounds( 5, 25,  45,  45), text("Level"), textColour("black"), colour(80,100,150), channel("level12"), range(0, 2, 1,1,0.01)
rslider  bounds( 5, 75,  45,  45), text("Tune"),  textColour("black"), colour(80,100,150), channel("tune12"),  range(-2, 2, 0,1,0.01)
rslider  bounds( 5,125,  45,  45), text("Decay"), textColour("black"), colour(80,100,150), channel("dur12"),  range(0.25, 2, 1, 0.8)
rslider  bounds( 5,175,  45,  45), text("Pan"),   textColour("black"), colour(80,100,150), channel("pan12"), range(0, 1, 0.5,1,0.01)
}
groupbox bounds(600,  0,  50, 225), text("Maraca"),fontColour("black"), colour(172,172,172), plant(13) {
checkbox bounds(10, 24,   5,   5), colour("yellow"), channel("Act13"),  value(0), shape("ellipse"), active(0)
rslider  bounds( 5, 25,  45,  45), text("Level"), textColour("black"), colour(80,100,150), channel("level13"), range(0, 2, 1,1,0.01)
rslider  bounds( 5, 75,  45,  45), text("Tune"),  textColour("black"), colour(80,100,150), channel("tune13"),  range(-2, 2, 0,1,0.01)
rslider  bounds( 5,125,  45,  45), text("Decay"), textColour("black"), colour(80,100,150), channel("dur13"),  range(0.25, 2, 1, 0.8)
rslider  bounds( 5,175,  45,  45), text("Pan"),   textColour("black"), colour(80,100,150), channel("pan13"), range(0, 1, 0.5,1,0.01)
}
groupbox bounds(650,  0,  50, 225), text("Hi Conga"),fontColour("black"), colour(162,162,162), plant(14) {
checkbox bounds(10, 24,   5,   5), colour("yellow"), channel("Act14"),  value(0), shape("ellipse"), active(0)
rslider  bounds( 5, 25,  45,  45), text("Level"),   textColour("black"), colour(80,100,150), channel("level14"), range(0, 2, 1,1,0.01)
rslider  bounds( 5, 75,  45,  45), text("Tune"),    textColour("black"), colour(80,100,150), channel("tune14"),  range(-2, 2, 0,1,0.01)
rslider  bounds( 5,125,  45,  45), text("Decay"),   textColour("black"), colour(80,100,150), channel("dur14"),   range(0.25, 2, 1, 0.8)
rslider  bounds( 5,175,  45,  45), text("Pan"),     textColour("black"), colour(80,100,150), channel("pan14"), range(0, 1, 0.5,0.01)
}
groupbox bounds(700,  0,  50, 225), text("Md Cnga"),fontColour("black"), colour(152,152,152), plant(15) {
checkbox bounds(10, 24,   5,   5), colour("yellow"), channel("Act15"),  value(0), shape("ellipse"), active(0)
rslider  bounds( 5, 25,  45,  45), text("Level"),  textColour("black"), colour(80,100,150), channel("level15"), range(0, 2, 1,1,0.01)
rslider  bounds( 5, 75,  45,  45), text("Tune"),   textColour("black"), colour(80,100,150), channel("tune15"),  range(-2, 2, 0,1,0.01)
rslider  bounds( 5,125,  45,  45), text("Decay"),  textColour("black"), colour(80,100,150), channel("dur15"),   range(0.25, 2, 1, 0.8)
rslider  bounds( 5,175,  45,  45), text("Pan"),    textColour("black"), colour(80,100,150), channel("pan15"), range(0, 1, 0.5,1,0.01)
}
groupbox bounds(750,  0,  50, 225), text("Lo Cnga"),fontColour("black"), colour(142,142,142), plant(16) {
checkbox bounds(10, 24,   5,   5), colour("yellow"), channel("Act16"),  value(0), shape("ellipse"), active(0)
rslider  bounds( 5, 25,  45,  45), text("Level"),  textColour("black"), colour(80,100,150), channel("level16"), range(0, 2, 1,1,0.01)
rslider  bounds( 5, 75,  45,  45), text("Tune"),   textColour("black"), colour(80,100,150), channel("tune16"),  range(-2, 2, 0,1,0.01)
rslider  bounds( 5,125,  45,  45), text("Decay"),  textColour("black"), colour(80,100,150), channel("dur16"),   range(0.25, 2, 1, 0.8)
rslider  bounds( 5,175,  45,  45), text("Pan"),    textColour("black"), colour(80,100,150), channel("pan16"), range(0, 1, 0.5,1,0.01)
}

keyboard bounds(0, 225, 800, 80)
button   bounds( 10,315, 40, 25), fontColour:0(50,50,50), fontColour:1(205,255,205), colour:0(0,10,0), colour:1(0,150,0), text("Play","Play"), channel("Play"), latched(1), radioGroup(1), identChannel("PlayButton")
button   bounds( 50,315, 40, 25), fontColour:0(50,50,50), fontColour:1(255,205,205), colour:0(10,0,0), colour:1(150,0,0), text("Stop","Stop"), channel("Stop"), latched(1), radioGroup(1), value(1), identChannel("StopButton")


combobox bounds( 10,345, 80, 15), channel("sound"), value(1), text("Bass Drum","Snare","Open HH","Cl. HH","Hi Tom","Mid Tom","Lo Tom","Cymbal","Rimshot","Claves","Cowbell","Clap","Maraca","Hi Conga","Mid Conga","Lo Conga")
button   bounds(462,315, 80, 20), text("Clear","Clear"), channel("clear"), value(0)
button   bounds(462,338, 38, 20), text("Save","Save"), channel("save"), value(0)
button   bounds(502,338, 38, 20), text("Load","Load"), channel("load"), value(0)
label    bounds(542,328, 48, 10), text("PATTERN"), fontColour("black")
combobox bounds(544,338, 45, 20), channel("pattern"), value(1), text("1","2","3","4","5","6","7","8","9","10","11","12","13","14","15","16")
rslider  bounds(593,310, 60, 60), text("Accent"),  textColour("black"), colour(80,100,150), channel("AccLev"), range(0, 1.00, 0.4)
rslider  bounds(643,310, 60, 60), text("Tempo"),   textColour("black"), colour(80,100,150), channel("tempo"), range(10, 500, 70, 1, 1)
rslider  bounds(693,310, 60, 60), text("Swing"),   textColour("black"), colour(80,100,150), channel("swing"), range(0, 0.50, 0)
rslider  bounds(743,310, 60, 60), text("Level"),   textColour("black"), colour(80,100,150), channel("level"), range(0, 2.00, 1)

label    bounds(143,312,  6, 10), text("1"), fontColour("black")
label    bounds(163,312,  6, 10), text("2"), fontColour("black")
label    bounds(183,312,  6, 10), text("3"), fontColour("black")
label    bounds(203,312,  6, 10), text("4"), fontColour("black")
label    bounds(223,312,  6, 10), text("5"), fontColour("black")
label    bounds(243,312,  6, 10), text("6"), fontColour("black")
label    bounds(263,312,  6, 10), text("7"), fontColour("black")
label    bounds(283,312,  6, 10), text("8"), fontColour("black")
label    bounds(303,312,  6, 10), text("9"), fontColour("black")
label    bounds(320,312, 12, 10), text("10"), fontColour("black")
label    bounds(340,312, 12, 10), text("11"), fontColour("black")
label    bounds(360,312, 12, 10), text("12"), fontColour("black")
label    bounds(380,312, 12, 10), text("13"), fontColour("black")
label    bounds(400,312, 12, 10), text("14"), fontColour("black")
label    bounds(420,312, 12, 10), text("15"), fontColour("black")
label    bounds(440,312, 12, 10), text("16"), fontColour("black")

label    bounds(120, 325, 17, 12), text("On"), fontColour("black")
checkbox bounds(140, 325, 12, 12), channel("On1"),  value(1)
checkbox bounds(160, 325, 12, 12), channel("On2"),  value(0)
checkbox bounds(180, 325, 12, 12), channel("On3"),  value(0)
checkbox bounds(200, 325, 12, 12), channel("On4"),  value(1)
checkbox bounds(220, 325, 12, 12), channel("On5"),  value(1)
checkbox bounds(240, 325, 12, 12), channel("On6"),  value(0)
checkbox bounds(260, 325, 12, 12), channel("On7"),  value(0)
checkbox bounds(280, 325, 12, 12), channel("On8"),  value(0)
checkbox bounds(300, 325, 12, 12), channel("On9"),  value(1)
checkbox bounds(320, 325, 12, 12), channel("On10"), value(0)
checkbox bounds(340, 325, 12, 12), channel("On11"), value(0)
checkbox bounds(360, 325, 12, 12), channel("On12"), value(1)
checkbox bounds(380, 325, 12, 12), channel("On13"), value(0)
checkbox bounds(400, 325, 12, 12), channel("On14"), value(1)
checkbox bounds(420, 325, 12, 12), channel("On15"), value(0)
checkbox bounds(440, 325, 12, 12), channel("On16"), value(0)

label    bounds( 98, 345, 38, 12), text("Accent"), fontColour("black")
checkbox bounds(140, 345, 12, 12), channel("Acc1"),  value(1), colour("yellow")
checkbox bounds(160, 345, 12, 12), channel("Acc2"),  value(0), colour("yellow")
checkbox bounds(180, 345, 12, 12), channel("Acc3"),  value(0), colour("yellow")
checkbox bounds(200, 345, 12, 12), channel("Acc4"),  value(0), colour("yellow")
checkbox bounds(220, 345, 12, 12), channel("Acc5"),  value(0), colour("yellow")
checkbox bounds(240, 345, 12, 12), channel("Acc6"),  value(0), colour("yellow")
checkbox bounds(260, 345, 12, 12), channel("Acc7"),  value(0), colour("yellow")
checkbox bounds(280, 345, 12, 12), channel("Acc8"),  value(0), colour("yellow")
checkbox bounds(300, 345, 12, 12), channel("Acc9"),  value(0), colour("yellow")
checkbox bounds(320, 345, 12, 12), channel("Acc10"), value(0), colour("yellow")
checkbox bounds(340, 345, 12, 12), channel("Acc11"), value(0), colour("yellow")
checkbox bounds(360, 345, 12, 12), channel("Acc12"), value(0), colour("yellow")
checkbox bounds(380, 345, 12, 12), channel("Acc13"), value(0), colour("yellow")
checkbox bounds(400, 345, 12, 12), channel("Acc14"), value(0), colour("yellow")
checkbox bounds(420, 345, 12, 12), channel("Acc15"), value(0), colour("yellow")
checkbox bounds(440, 345, 12, 12), channel("Acc16"), value(0), colour("yellow")

button   bounds(458,362, 90, 18), text("Local Control","Host Control"), channel("control"), value(0)

</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-n -+rtmidi=NULL -M0 -dm0
</CsOptions>

<CsInstruments>

; sr set by host
ksmps         =     32    ;NUMBER OF AUDIO SAMPLES IN EACH CONTROL CYCLE
nchnls         =     2    ;NUMBER OF CHANNELS (2=STEREO)
0dbfs        =    1
massign    0,2

;Author: Iain McCurdy (2012)

gisine        ftgen    0,0,1024,10,1        ;A SINE WAVE
gicos        ftgen    0,0,65536,9,1,1,90    ;A COSINE WAVE
gkoff    init    0
;                1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15,16
giOn1    ftgen    0,0,16,2,    1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0    ;BD
giOn2    ftgen    0,0,16,2,    0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0    ;SN
giOn3    ftgen    0,0,16,2,    0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0  ;HHO
giOn4    ftgen    0,0,16,2,    1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1  ;HHCl
giOn5    ftgen    0,0,16,2,    0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0  ;Hi Tom
giOn6    ftgen    0,0,16,2,    0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0  ;Mid Tom
giOn7    ftgen    0,0,16,2,    0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0  ;Lo Tom
giOn8    ftgen    0,0,16,2,    0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0  ;Cym
giOn9    ftgen    0,0,16,2,    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0  ;Rimshot
giOn10    ftgen    0,0,16,2,    1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0  ;Claves
giOn11    ftgen    0,0,16,2,    1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1  ;Cowbell
giOn12    ftgen    0,0,16,2,    0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0  ;Clap
giOn13    ftgen    0,0,16,2,    1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0  ;Maraca
giOn14    ftgen    0,0,16,2,    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0  ;Hi Conga
giOn15    ftgen    0,0,16,2,    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0  ;Mid Conga
giOn16    ftgen    0,0,16,2,    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1  ;Lo Conga
giAcc1    ftgen    0,0,16,2,    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0    ;BD
giAcc2    ftgen    0,0,16,2,    0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0    ;SN
giAcc3    ftgen    0,0,16,2,    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0  ;HHO
giAcc4    ftgen    0,0,16,2,    1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0  ;HHCl
giAcc5    ftgen    0,0,16,2,    0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0  ;Hi Tom
giAcc6    ftgen    0,0,16,2,    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0  ;Mid Tom
giAcc7    ftgen    0,0,16,2,    0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0  ;Lo Tom
giAcc8    ftgen    0,0,16,2,    0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0  ;Cym
giAcc9    ftgen    0,0,16,2,    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0  ;Rimshot
giAcc10    ftgen    0,0,16,2,    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0  ;Claves
giAcc11    ftgen    0,0,16,2,    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0  ;Cowbell
giAcc12    ftgen    0,0,16,2,    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0  ;Clap
giAcc13    ftgen    0,0,16,2,    1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0  ;Maraca
giAcc14    ftgen    0,0,16,2,    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0  ;Hi Conga
giAcc15    ftgen    0,0,16,2,    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0  ;Mid Conga
giAcc16    ftgen    0,0,16,2,    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0  ;Lo Conga
giblank    ftgen    0,0,16,2,    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0  ;blank

instr    1    ;READ IN WIDGETS, SCAN MIDI NOTES PLAYED AND RELAY TO DRUM SOUNDS 
    gkPlay        chnget    "Play"
    gkStop        chnget    "Stop"
    if trigger(gkPlay,0.5,0)==1 then
     gkOnOff    =    1
    elseif trigger(gkStop,0.5,0)==1 then
     gkOnOff    =    0
    endif

    ;READ IN WIDGETS
    gklevel1    chnget    "level1"
    gklevel2    chnget    "level2"
    gklevel3    chnget    "level3"
    gklevel4    chnget    "level4"
    gklevel5    chnget    "level5"
    gklevel6    chnget    "level6"
    gklevel7    chnget    "level7"
    gklevel8    chnget    "level8"
    gklevel9    chnget    "level9"
    gklevel10    chnget    "level10"    
    gklevel11    chnget    "level11"    
    gklevel12    chnget    "level12"    
    gklevel13    chnget    "level13"    
    gklevel14    chnget    "level14"    
    gklevel15    chnget    "level15"    
    gklevel16    chnget    "level16"    
    gktune1        chnget    "tune1"    
    gktune2        chnget    "tune2"    
    gktune3        chnget    "tune3"    
    gktune4        chnget    "tune4"    
    gktune5        chnget    "tune5"    
    gktune6        chnget    "tune6"    
    gktune7        chnget    "tune7"    
    gktune8        chnget    "tune8"    
    gktune9        chnget    "tune9"    
    gktune10    chnget    "tune10"    
    gktune11    chnget    "tune11"    
    gktune12    chnget    "tune12"    
    gktune13    chnget    "tune13"    
    gktune14    chnget    "tune14"    
    gktune15    chnget    "tune15"    
    gktune16    chnget    "tune16"
    gkdur1    chnget    "dur1"
    gkdur2    chnget    "dur2"
    gkdur3    chnget    "dur3"
    gkdur4    chnget    "dur4"
    gkdur5    chnget    "dur5"
    gkdur6    chnget    "dur6"
    gkdur7    chnget    "dur7"
    gkdur8    chnget    "dur8"
    gkdur9    chnget    "dur9"
    gkdur10    chnget    "dur10"    
    gkdur11    chnget    "dur11"    
    gkdur12    chnget    "dur12"    
    gkdur13    chnget    "dur13"    
    gkdur14    chnget    "dur14"    
    gkdur15    chnget    "dur15"    
    gkdur16    chnget    "dur16"    
    gkpan1        chnget    "pan1"    
    gkpan2        chnget    "pan2"    
    gkpan3        chnget    "pan3"    
    gkpan4        chnget    "pan4"    
    gkpan5        chnget    "pan5"    
    gkpan6        chnget    "pan6"    
    gkpan7        chnget    "pan7"    
    gkpan8        chnget    "pan8"    
    gkpan9        chnget    "pan9"    
    gkpan10        chnget    "pan10"    
    gkpan11        chnget    "pan11"    
    gkpan12        chnget    "pan12"    
    gkpan13        chnget    "pan13"    
    gkpan14        chnget    "pan14"    
    gkpan15        chnget    "pan15"    
    gkpan16        chnget    "pan16"
    gksound        chnget    "sound"
    gkOn1        chnget    "On1"
    gkOn2        chnget    "On2"
    gkOn3        chnget    "On3"
    gkOn4        chnget    "On4"
    gkOn5        chnget    "On5"
    gkOn6        chnget    "On6"
    gkOn7        chnget    "On7"
    gkOn8        chnget    "On8"
    gkOn9        chnget    "On9"
    gkOn10        chnget    "On10"
    gkOn11        chnget    "On11"
    gkOn12        chnget    "On12"
    gkOn13        chnget    "On13"
    gkOn14        chnget    "On14"
    gkOn15        chnget    "On15"
    gkOn16        chnget    "On16"
    gkAcc1        chnget    "Acc1"
    gkAcc2        chnget    "Acc2"
    gkAcc3        chnget    "Acc3"
    gkAcc4        chnget    "Acc4"
    gkAcc5        chnget    "Acc5"
    gkAcc6        chnget    "Acc6"
    gkAcc7        chnget    "Acc7"
    gkAcc8        chnget    "Acc8"
    gkAcc9        chnget    "Acc9"
    gkAcc10        chnget    "Acc10"
    gkAcc11        chnget    "Acc11"
    gkAcc12        chnget    "Acc12"
    gkAcc13        chnget    "Acc13"
    gkAcc14        chnget    "Acc14"
    gkAcc15        chnget    "Acc15"
    gkAcc16        chnget    "Acc16"
    gkclear        chnget    "clear"
    gksave        chnget    "save"
    gkload        chnget    "load"
    gkpattern    chnget    "pattern"
    gkAccLev    chnget    "AccLev"
    gktempo        chnget    "tempo"
    gkswing        chnget    "swing"
    gklevel        chnget    "level"

    kcontrol    chnget    "control"
    if kcontrol==1 then            ; host control over start stop and tempo
     gktempo    chnget    "HOST_BPM"
     gkOnOff    chnget    "IS_PLAYING"
     if changed(gktempo)==1 then
      chnset    gktempo, "tempo"
     endif
     if changed(gkOnOff)==1 then
      chnset    gkOnOff, "OnOff"
     endif
    endif
    
    ;START/STOP SEQUENCER
    ktrig    changed    gkOnOff        ;if Run/Stop button is changed...
    if ktrig==1 then        ;
     if gkOnOff==1 then        ;...if Run/Stop has been changed to 'Run'...
      event    "i",3,0,-1        ;...start instr 3 playing a held note
     else                ;otherwise...
      event    "i",-3,0,0        ;...stop instr 3 playing
     endif
    endif
    
    ;CLEAR SEQUENCES FOR ALL DRUM SOUNDS
    ktrig changed    gkclear        
    if ktrig==1 then
     tablecopy    giOn1,    giblank     ;copy blank table over existing sequence for voice 1
     tablecopy    giOn2,    giblank     ;repeat for every other voice
     tablecopy    giOn3,    giblank     
     tablecopy    giOn4,    giblank     
     tablecopy    giOn5,    giblank     
     tablecopy    giOn6,    giblank     
     tablecopy    giOn7,    giblank     
     tablecopy    giOn8,    giblank     
     tablecopy    giOn9,    giblank     
     tablecopy    giOn10,    giblank     
     tablecopy    giOn11,    giblank     
     tablecopy    giOn12,    giblank     
     tablecopy    giOn13,    giblank     
     tablecopy    giOn14,    giblank     
     tablecopy    giOn15,    giblank     
     tablecopy    giOn16,    giblank     
     tablecopy    giAcc1,        giblank     
     tablecopy    giAcc2,        giblank     
     tablecopy    giAcc3,        giblank     
     tablecopy    giAcc4,        giblank     
     tablecopy    giAcc5,        giblank     
     tablecopy    giAcc6,        giblank     
     tablecopy    giAcc7,        giblank     
     tablecopy    giAcc8,        giblank     
     tablecopy    giAcc9,        giblank     
     tablecopy    giAcc10,    giblank     
     tablecopy    giAcc11,    giblank     
     tablecopy    giAcc12,    giblank     
     tablecopy    giAcc13,    giblank     
     tablecopy    giAcc14,    giblank     
     tablecopy    giAcc15,    giblank     
     tablecopy    giAcc16,    giblank     
    endif
    
    ;STORE SEQUENCE TABLES
    ktrig changed    gksave    ;if save button is toggled...
    igoto    SKIP
    if ktrig==1 then
     reinit    SAVE_TABLES    ;begin a reinitialisation pass from label (ftsave operates only at i-time)
    endif
    SAVE_TABLES:
    SFileName sprintf "TR-808_pattern.%d.txt", i(gkpattern)    ;create the file name (string variable) that will be used to name the text file that will contain the pattern data
    ftsave SFileName, 1, giOn1, giOn2, giOn3, giOn4, giOn5, giOn6, giOn7, giOn8, giOn9, giOn10, giOn11, giOn12, giOn13, giOn14, giOn15, giOn16, giAcc1, giAcc2, giAcc3, giAcc4, giAcc5, giAcc6, giAcc7, giAcc8, giAcc9, giAcc10, giAcc11, giAcc12, giAcc13, giAcc14, giAcc15, giAcc16    ;save all tables (on information and accents) to a text file in the pwd
    rireturn    
    SKIP:
    
    ;LOAD SEQUENCE TABLES
    ktrig changed    gkload    ;if load button is toggled...
    igoto    SKIP2
    if ktrig==1 then
     reinit    LOAD_TABLES    ;begin a reinitialisation pass from label (ftload operates only at i-time)
    endif
    LOAD_TABLES:
    SFileName sprintf "TR-808_pattern.%d.txt", i(gkpattern)    ;create the file name (string variable) that will be the name of the file from which data will be retrieved
    ftload SFileName, 1, giOn1, giOn2, giOn3, giOn4, giOn5, giOn6, giOn7, giOn8, giOn9, giOn10, giOn11, giOn12, giOn13, giOn14, giOn15, giOn16, giAcc1, giAcc2, giAcc3, giAcc4, giAcc5, giAcc6, giAcc7, giAcc8, giAcc9, giAcc10, giAcc11, giAcc12, giAcc13, giAcc14, giAcc15, giAcc16    ;load all tables (on information and accents) to a text file in the pwd
    rireturn    
    SKIP2:
    
    ;UPDATE SEQUENCE DISPLAY
    ktrig    changed    gksound,gkclear,gkload
    if ktrig==1 then
#define    UPDATE_SEQUENCE(N)
     #
     kval    tablekt    $N-1,giOn1+gksound-1
     chnset    kval,"On$N"
     kval    tablekt    $N-1,giAcc1+gksound-1
     chnset    kval,"Acc$N"
     #
     $UPDATE_SEQUENCE(1)
     $UPDATE_SEQUENCE(2)
     $UPDATE_SEQUENCE(3)
     $UPDATE_SEQUENCE(4)
     $UPDATE_SEQUENCE(5)
     $UPDATE_SEQUENCE(6)
     $UPDATE_SEQUENCE(7)
     $UPDATE_SEQUENCE(8)
     $UPDATE_SEQUENCE(9)
     $UPDATE_SEQUENCE(10)
     $UPDATE_SEQUENCE(11)
     $UPDATE_SEQUENCE(12)
     $UPDATE_SEQUENCE(13)
     $UPDATE_SEQUENCE(14)
     $UPDATE_SEQUENCE(15)
     $UPDATE_SEQUENCE(16)
    endif
    
    ;UPDATE TABLE IF SEQUENCE BUTTON CHANGED
#define    UPDATE_TABLE(N)
    #
    ktrig    changed    gkOn$N
    if ktrig==1 then
     tablewkt    gkOn$N,$N-1,giOn1+gksound-1
    endif
    ktrig    changed    gkAcc$N
    if ktrig==1 then
     tablewkt    gkAcc$N,$N-1,giAcc1+gksound-1
    endif
    #
    $UPDATE_TABLE(1)
    $UPDATE_TABLE(2)
    $UPDATE_TABLE(3)
    $UPDATE_TABLE(4)
    $UPDATE_TABLE(5)
    $UPDATE_TABLE(6)
    $UPDATE_TABLE(7)
    $UPDATE_TABLE(8)
    $UPDATE_TABLE(9)
    $UPDATE_TABLE(10)
    $UPDATE_TABLE(11)
    $UPDATE_TABLE(12)
    $UPDATE_TABLE(13)
    $UPDATE_TABLE(14)
    $UPDATE_TABLE(15)
    $UPDATE_TABLE(16)
endin

instr    2    ;READ MIDI AND TRIGGER NOTES
    inum    notnum                    ;READ IN MIDI NOTE NUMBER
    idb    veloc    -30,0                ;READ IN MIDI NOTE VELOCITY (WILL BE USED AS A DECIBEL VALUE)
    iinstr    wrap    inum,0,16            ;MAP MIDI NOTE NUMBERS (RANGE: 0 - 127) TO DRUM SOUND NUMBER (0 - 15)
    event_i    "i",101+iinstr,0,0.001,ampdbfs(idb)    ;CALL THE RELEVANT INSTRUMENT AND SEND AMPLITUDE VALUE AS p4
endin

instr    3    ;PLAY SEQUENCE    
    gindx    init    0
    ktick    metro        (gktempo*4)/60
    
    ;SWING
    kSwingBeat init    0
    kSwingTime    =    (kSwingBeat==1?(60*gkswing)/(gktempo*4):0)
    if ktick==1 then
     kSwingBeat    =    abs(kSwingBeat-1)    ;FLIP BETWEEN ZERO AND 1
    endif
    
        schedkwhen    ktick, 0, 0, 4, kSwingTime, 0.001
endin

instr    4    ;TEST FOR NOTE ON SEQUENCE STEP
#define    VOICE(N)    
    #
    iOnOff$N    table        gindx, giOn$N
    if iOnOff$N==1 then
     iAcc    table    gindx,giAcc$N
     event_i    "i", 100+$N, 0, 0.001, 0.4 + (iAcc*i(gkAccLev))
    endif
    #
$VOICE(1)
$VOICE(2)
$VOICE(3)
$VOICE(4)
$VOICE(5)
$VOICE(6)
$VOICE(7)
$VOICE(8)
$VOICE(9)
$VOICE(10)
$VOICE(11)
$VOICE(12)
$VOICE(13)
$VOICE(14)
$VOICE(15)
$VOICE(16)
    gindx        wrap    gindx+1, 0, 16
            turnoff
endin

instr    101    ;BASS DRUM
    xtratim    0.1
    krelease    release        ;SENSE RELEASE OF THIS NOTE ('1' WHEN RELEASED, OTHERWISE ZERO)
    chnset    1-krelease,"Act1"    ;TURN ON ACTIVE LIGHT WHEN NOTE STARTS, TURN IT OFF WHEN NOTE ENDS

    p3    =    2*i(gkdur1)                            ;NOTE DURATION. SCALED USING GUI 'Decay' KNOB

    ;SUSTAIN AND BODY OF THE SOUND
    kmul    transeg    0.2,p3*0.5,-15,0.01, p3*0.5,0,0                    ;PARTIAL STRENGTHS MULTIPLIER USED BY GBUZZ. DECAYS FROM A SOUND WITH OVERTONES TO A SINE TONE.
    kbend    transeg    0.5,1.2,-4, 0,1,0,0                        ;SLIGHT PITCH BEND AT THE START OF THE NOTE 
    asig    gbuzz    0.5,50*octave(gktune1)*semitone(kbend),20,1,kmul,gicos        ;GBUZZ TONE
    aenv    transeg    1,p3-0.004,-6,0                            ;AMPLITUDE ENVELOPE FOR SUSTAIN OF THE SOUND
    aatt    linseg    0,0.004,1                            ;SOFT ATTACK
    asig    =    asig*aenv*aatt

    ;HARD, SHORT ATTACK OF THE SOUND
    aenv    linseg    1,0.07,0                            ;AMPLITUDE ENVELOPE (FAST DECAY)                        
    acps    expsega    400,0.07,0.001,1,0.001                        ;FREQUENCY OF THE ATTACK SOUND. QUICKLY GLISSES FROM 400 Hz TO SUB-AUDIO
    aimp    oscili    aenv,acps*octave(gktune1*0.25),gisine                ;CREATE ATTACK SOUND
    
    amix    =    ((asig*0.5)+(aimp*0.35))*gklevel1*p4*gklevel            ;MIX SUSTAIN AND ATTACK SOUND ELEMENTS AND SCALE USING GUI 'Level' KNOB
    
    aL,aR    pan2    amix,gkpan1                            ;PAN THE MONOPHONIC SOUND
        outs    aL,aR                                ;SEND AUDIO TO OUTPUTS
endin

instr    102    ;SNARE DRUM
    xtratim    0.1
    krelease    release        ;SENSE RELEASE OF THIS NOTE ('1' WHEN RELEASED, OTHERWISE ZERO)   
    chnset    1-krelease,"Act2"       ;TURN ON ACTIVE LIGHT WHEN NOTE STARTS, TURN IT OFF WHEN NOTE ENDS
    
    ;SOUND CONSISTS OF TWO SINE TONES, AN OCTAVE APART AND A NOISE SIGNAL
    ifrq      =    342        ;FREQUENCY OF THE TONES
    iNseDur    =    0.3 * i(gkdur2)    ;DURATION OF THE NOISE COMPONENT
    iPchDur    =    0.1 * i(gkdur2)    ;DURATION OF THE SINE TONES COMPONENT
    p3    =    iNseDur     ;p3 DURATION TAKEN FROM NOISE COMPONENT DURATION (ALWATS THE LONGEST COMPONENT)
    
    ;SINE TONES COMPONENT
    aenv1    expseg    1,iPchDur,0.0001,p3-iPchDur,0.0001        ;AMPLITUDE ENVELOPE
    apitch1    oscili    1,ifrq*octave(gktune2),gisine            ;SINE TONE 1
    apitch2    oscili    0.25,ifrq*0.5*octave(gktune2),gisine        ;SINE TONE 2 (AN OCTAVE LOWER)
    apitch    =    (apitch1+apitch2)*0.75                ;MIX THE TWO SINE TONES

    ;NOISE COMPONENT
    aenv2    expon    1,p3,0.0005                    ;AMPLITUDE ENVELOPE
    anoise    noise    0.75,0                        ;CREATE SOME NOISE
    anoise    butbp    anoise,10000*octave(gktune2),10000        ;BANDPASS FILTER THE NOISE SIGNAL
    anoise    buthp    anoise,1000                    ;HIGHPASS FILTER THE NOISE SIGNAL
    kcf    expseg    5000,0.1,3000,p3-0.2,3000            ;CUTOFF FREQUENCY FOR A LOWPASS FILTER
    anoise    butlp    anoise,kcf                    ;LOWPASS FILTER THE NOISE SIGNAL
    amix    =    ((apitch*aenv1)+(anoise*aenv2))*gklevel2*p4*gklevel    ;MIX AUDIO SIGNALS AND SCALE ACCORDING TO GUI 'Level' CONTROL
    aL,aR    pan2    amix,i(gkpan2)                    ;PAN THE MONOPHONIC AUDIO SIGNAL
        outs    aL,aR                        ;SEND AUDIO TO OUTPUTS
endin

instr    103    ;OPEN HIGH HAT
    xtratim    0.1
    kFrq1    =    296*octave(gktune3)     ;FREQUENCIES OF THE 6 OSCILLATORS
    kFrq2    =    285*octave(gktune3)     
    kFrq3    =    365*octave(gktune3)     
    kFrq4    =    348*octave(gktune3)     
    kFrq5    =    420*octave(gktune3)     
    kFrq6    =    835*octave(gktune3)     
    p3    =    0.5*i(gkdur3)        ;DURATION OF THE NOTE
    
    ;SOUND CONSISTS OF 6 PULSE OSCILLATORS MIXED WITH A NOISE COMPONENT
    ;PITCHED ELEMENT
    aenv    linseg    1,p3-0.05,0.1,0.05,0        ;AMPLITUDE ENVELOPE FOR THE PULSE OSCILLATORS
    ipw    =    0.25                ;PULSE WIDTH
    a1    vco2    0.5,kFrq1,2,ipw            ;PULSE OSCILLATORS...
    a2    vco2    0.5,kFrq2,2,ipw
    a3    vco2    0.5,kFrq3,2,ipw
    a4    vco2    0.5,kFrq4,2,ipw
    a5    vco2    0.5,kFrq5,2,ipw
    a6    vco2    0.5,kFrq6,2,ipw
    amix    sum    a1,a2,a3,a4,a5,a6        ;MIX THE PULSE OSCILLATORS
    amix    reson    amix,5000*octave(gktune3),5000,1    ;BANDPASS FILTER THE MIXTURE
    amix    buthp    amix,5000            ;HIGHPASS FILTER THE SOUND...
    amix    buthp    amix,5000            ;...AND AGAIN
    amix    =    amix*aenv            ;APPLY THE AMPLITUDE ENVELOPE
    
    ;NOISE ELEMENT
    anoise    noise    0.8,0                ;GENERATE SOME WHITE NOISE
    aenv    linseg    1,p3-0.05,0.1,0.05,0        ;CREATE AN AMPLITUDE ENVELOPE
    kcf    expseg    20000,0.7,9000,p3-0.1,9000    ;CREATE A CUTOFF FREQ. ENVELOPE
    anoise    butlp    anoise,kcf            ;LOWPASS FILTER THE NOISE SIGNAL
    anoise    buthp    anoise,8000            ;HIGHPASS FILTER THE NOISE SIGNAL
    anoise    =    anoise*aenv            ;APPLY THE AMPLITUDE ENVELOPE
    
    ;MIX PULSE OSCILLATOR AND NOISE COMPONENTS
    amix    =    (amix+anoise)*gklevel3*p4*0.55*gklevel
    aL,aR    pan2    amix,gkpan3            ;PAN MONOPHONIC SIGNAL
        outs    aL,aR                ;SEND TO OUTPUTS
    ;kactive    active    p1+1                ;CHECK NUMBER OF ACTIVE INSTANCES OF CLOSED HIGH HAT INSTRUMENT
    ;if kactive>0 then            ;IF HIGH-HAT CLOSED IS ACTIVE...
    ; turnoff                ;TURN OFF THIS INSTRUMENT
    ;endif
    krelease    release            ;SENSE RELEASE OF THIS NOTE ('1' WHEN RELEASED, OTHERWISE ZERO)   
    chnset    1-krelease,"Act3"           ;TURN ON ACTIVE LIGHT WHEN NOTE STARTS, TURN IT OFF WHEN NOTE ENDS
endin

instr    104    ;CLOSED HIGH HAT
    xtratim    0.1
    krelease    release            ;SENSE RELEASE OF THIS NOTE ('1' WHEN RELEASED, OTHERWISE ZERO)   
    chnset    1-krelease,"Act4"           ;TURN ON ACTIVE LIGHT WHEN NOTE STARTS, TURN IT OFF WHEN NOTE ENDS
    kFrq1    =    296*octave(gktune4)     ;FREQUENCIES OF THE 6 OSCILLATORS
    kFrq2    =    285*octave(gktune4)     
    kFrq3    =    365*octave(gktune4)     
    kFrq4    =    348*octave(gktune4)     
    kFrq5    =    420*octave(gktune4)     
    kFrq6    =    835*octave(gktune4)     
    idur    =    0.088*i(gkdur4)        ;DURATION OF THE NOTE
    p3    limit    idur,0.1,10        ;LIMIT THE MINIMUM DURATION OF THE NOTE (VERY SHORT NOTES CAN RESULT IN THE INDICATOR LIGHT ON-OFF NOTE BEING TO0 SHORT)

    iactive    active    p1-1            ;SENSE ACTIVITY OF PREVIOUS INSTRUMENT (OPEN HIGH HAT) 
    if iactive>0 then            ;IF 'OPEN HIGH HAT' IS ACTIVE...
     turnoff2    p1-1,0,0        ;TURN IT OFF (CLOSED HIGH HAT TAKES PRESIDENCE)
     chnset    gkoff,"Act3"           ;TURN OFF ACTIVE LIGHT FOR OPEN HIGH HAT
    endif

    ;PITCHED ELEMENT
    aenv    expsega    1,idur,0.001,1,0.001        ;AMPLITUDE ENVELOPE FOR THE PULSE OSCILLATORS
    ipw    =    0.25                ;PULSE WIDTH
    a1    vco2    0.5,kFrq1,2,ipw            ;PULSE OSCILLATORS...            
    a2    vco2    0.5,kFrq2,2,ipw
    a3    vco2    0.5,kFrq3,2,ipw
    a4    vco2    0.5,kFrq4,2,ipw
    a5    vco2    0.5,kFrq5,2,ipw
    a6    vco2    0.5,kFrq6,2,ipw
    amix    sum    a1,a2,a3,a4,a5,a6        ;MIX THE PULSE OSCILLATORS
    amix    reson    amix,5000*octave(gktune4),5000,1    ;BANDPASS FILTER THE MIXTURE
    amix    buthp    amix,5000            ;HIGHPASS FILTER THE SOUND...
    amix    buthp    amix,5000            ;...AND AGAIN
    amix    =    amix*aenv            ;APPLY THE AMPLITUDE ENVELOPE
    
    ;NOISE ELEMENT
    anoise    noise    0.8,0                ;GENERATE SOME WHITE NOISE
    aenv    expsega    1,idur,0.001,1,0.001        ;CREATE AN AMPLITUDE ENVELOPE
    kcf    expseg    20000,0.7,9000,idur-0.1,9000    ;CREATE A CUTOFF FREQ. ENVELOPE
    anoise    butlp    anoise,kcf            ;LOWPASS FILTER THE NOISE SIGNAL
    anoise    buthp    anoise,8000            ;HIGHPASS FILTER THE NOISE SIGNAL
    anoise    =    anoise*aenv            ;APPLY THE AMPLITUDE ENVELOPE
    
    ;MIX PULSE OSCILLATOR AND NOISE COMPONENTS
    amix    =    (amix+anoise)*gklevel4*p4*0.55*gklevel
    aL,aR    pan2    amix,gkpan4            ;PAN MONOPHONIC SIGNAL
        outs    aL,aR                ;SEND TO OUTPUTS
endin

instr    105    ;HIGH TOM
    xtratim    0.1
    krelease    release                    ;SENSE RELEASE OF THIS NOTE ('1' WHEN RELEASED, OTHERWISE ZERO)   
    chnset    1-krelease,"Act5"                   ;TURN ON ACTIVE LIGHT WHEN NOTE STARTS, TURN IT OFF WHEN NOTE ENDS
    ifrq         =    200 * octave(i(gktune5))    ;FREQUENCY
    p3          =    0.5 * i(gkdur5)            ;DURATION OF THIS NOTE

    ;SINE TONE SIGNAL
    aAmpEnv    transeg    1,p3,-10,0.001                ;AMPLITUDE ENVELOPE FOR SINE TONE SIGNAL
    afmod    expsega    5,0.125/ifrq,1,1,1            ;FREQUENCY MODULATION ENVELOPE. GIVES THE TONE MORE OF AN ATTACK.
    asig    oscili    -aAmpEnv*0.6,ifrq*afmod,gisine        ;SINE TONE SIGNAL

    ;NOISE SIGNAL
    aEnvNse    transeg    1,p3,-6,0.001                ;AMPLITUDE ENVELOPE FOR NOISE SIGNAL
    anoise    dust2    0.4, 8000                ;GENERATE NOISE SIGNAL
    anoise    reson    anoise,400*octave(i(gktune5)),800,1    ;BANDPASS FILTER THE NOISE SIGNAL
    anoise    buthp    anoise,100*octave(i(gktune5))        ;HIGHPASS FILTER THE NOSIE SIGNAL
    anoise    butlp    anoise,1000*octave(i(gktune5))        ;LOWPASS FILTER THE NOISE SIGNAL
    anoise    =    anoise * aEnvNse            ;SCALE NOISE SIGNAL WITH AMPLITUDE ENVELOPE
    
    ;MIX THE TWO SOUND COMPONENTS
    amix    =    (asig + anoise)*gklevel5*p4*gklevel
    aL,aR    pan2    amix,gkpan5                ;PAN MONOPHONIC SIGNAL
        outs    aL,aR                    ;SEND AUDIO TO OUTPUTS
endin

instr    106    ;MID TOM
    xtratim    0.1
    krelease    release                    ;SENSE RELEASE OF THIS NOTE ('1' WHEN RELEASED, OTHERWISE ZERO)   
    chnset    1-krelease,"Act6"                       ;TURN ON ACTIVE LIGHT WHEN NOTE STARTS, TURN IT OFF WHEN NOTE ENDS
    ifrq         =    133*octave(i(gktune6))         ;FREQUENCY
    p3          =    0.6 * i(gkdur6)            ;DURATION OF THIS NOTE

    ;SINE TONE SIGNAL
    aAmpEnv    transeg    1,p3,-10,0.001                ;AMPLITUDE ENVELOPE FOR SINE TONE SIGNAL
    afmod    expsega    5,0.125/ifrq,1,1,1            ;FREQUENCY MODULATION ENVELOPE. GIVES THE TONE MORE OF AN ATTACK.
    asig    oscili    -aAmpEnv*0.6,ifrq*afmod,gisine        ;SINE TONE SIGNAL

    ;NOISE SIGNAL
    aEnvNse    transeg    1,p3,-6,0.001                ;AMPLITUDE ENVELOPE FOR NOISE SIGNAL
    anoise    dust2    0.4, 8000                ;GENERATE NOISE SIGNAL
    anoise    reson    anoise, 400*octave(i(gktune6)),800,1    ;BANDPASS FILTER THE NOISE SIGNAL
    anoise    buthp    anoise,100*octave(i(gktune6))        ;HIGHPASS FILTER THE NOSIE SIGNAL
    anoise    butlp    anoise,600*octave(i(gktune6))        ;LOWPASS FILTER THE NOISE SIGNAL
    anoise    =    anoise * aEnvNse            ;SCALE NOISE SIGNAL WITH AMPLITUDE ENVELOPE
    
    ;MIX THE TWO SOUND COMPONENTS
    amix    =    (asig + anoise)*gklevel6*p4*gklevel
    aL,aR    pan2    amix,i(gkpan6)                ;PAN MONOPHONIC SIGNAL
        outs    aL,aR                    ;SEND AUDIO TO OUTPUTS
endin

instr    107    ;LOW TOM
    xtratim    0.1
    krelease    release                ;SENSE RELEASE OF THIS NOTE ('1' WHEN RELEASED, OTHERWISE ZERO)   
    chnset    1-krelease,"Act7"                   ;TURN ON ACTIVE LIGHT WHEN NOTE STARTS, TURN IT OFF WHEN NOTE ENDS
    ifrq         =    90 * octave(i(gktune7))    ;FREQUENCY
    p3          =    0.7*i(gkdur7)         ;DURATION OF THIS NOTE

    ;SINE TONE SIGNAL
    aAmpEnv    transeg    1,p3,-10,0.001                ;AMPLITUDE ENVELOPE FOR SINE TONE SIGNAL
    afmod    expsega    5,0.125/ifrq,1,1,1            ;FREQUENCY MODULATION ENVELOPE. GIVES THE TONE MORE OF AN ATTACK.
    asig    oscili    -aAmpEnv*0.6,ifrq*afmod,gisine        ;SINE TONE SIGNAL

    ;NOISE SIGNAL
    aEnvNse    transeg    1,p3,-6,0.001                ;AMPLITUDE ENVELOPE FOR NOISE SIGNAL
    anoise    dust2    0.4, 8000                ;GENERATE NOISE SIGNAL
    anoise    reson    anoise,40*octave(gktune7),800,1        ;BANDPASS FILTER THE NOISE SIGNAL
    anoise    buthp    anoise,100*octave(i(gktune7))        ;HIGHPASS FILTER THE NOSIE SIGNAL
    anoise    butlp    anoise,600*octave(i(gktune7))        ;LOWPASS FILTER THE NOISE SIGNAL
    anoise    =    anoise * aEnvNse            ;SCALE NOISE SIGNAL WITH AMPLITUDE ENVELOPE
    
    ;MIX THE TWO SOUND COMPONENTS
    amix    =    (asig + anoise)*gklevel7*p4*gklevel
    aL,aR    pan2    amix,i(gkpan7)                ;PAN MONOPHONIC SIGNAL
        outs    aL,aR                    ;SEND AUDIO TO OUTPUTS
endin

instr    108    ;CYMBAL
    xtratim    0.1
    krelease    release                ;SENSE RELEASE OF THIS NOTE ('1' WHEN RELEASED, OTHERWISE ZERO)   
    chnset    1-krelease,"Act8"                       ;TURN ON ACTIVE LIGHT WHEN NOTE STARTS, TURN IT OFF WHEN NOTE ENDS
    kFrq1    =    296*octave(gktune8)     ;FREQUENCIES OF THE 6 OSCILLATORS
    kFrq2    =    285*octave(gktune8)
    kFrq3    =    365*octave(gktune8)
    kFrq4    =    348*octave(gktune8)     
    kFrq5    =    420*octave(gktune8)
    kFrq6    =    835*octave(gktune8)
    p3    =    2*i(gkdur8)        ;DURATION OF THE NOTE

    ;SOUND CONSISTS OF 6 PULSE OSCILLATORS MIXED WITH A NOISE COMPONENT
    ;PITCHED ELEMENT
    aenv    expon    1,p3,0.0001        ;AMPLITUDE ENVELOPE FOR THE PULSE OSCILLATORS 
    ipw    =    0.25            ;PULSE WIDTH      
    a1    vco2    0.5,kFrq1,2,ipw        ;PULSE OSCILLATORS...  
    a2    vco2    0.5,kFrq2,2,ipw
    a3    vco2    0.5,kFrq3,2,ipw
    a4    vco2    0.5,kFrq4,2,ipw
    a5    vco2    0.5,kFrq5,2,ipw                                                                   
    a6    vco2    0.5,kFrq6,2,ipw
    amix    sum    a1,a2,a3,a4,a5,a6        ;MIX THE PULSE OSCILLATORS
    amix    reson    amix,5000 *octave(gktune8),5000,1    ;BANDPASS FILTER THE MIXTURE
    amix    buthp    amix,10000            ;HIGHPASS FILTER THE SOUND
    amix    butlp    amix,12000            ;LOWPASS FILTER THE SOUND...
    amix    butlp    amix,12000            ;AND AGAIN...
    amix    =    amix*aenv            ;APPLY THE AMPLITUDE ENVELOPE
    
    ;NOISE ELEMENT
    anoise    noise    0.8,0                ;GENERATE SOME WHITE NOISE
    aenv    expsega    1,0.3,0.07,p3-0.1,0.00001    ;CREATE AN AMPLITUDE ENVELOPE
    kcf    expseg    14000,0.7,7000,p3-0.1,5000    ;CREATE A CUTOFF FREQ. ENVELOPE
    anoise    butlp    anoise,kcf            ;LOWPASS FILTER THE NOISE SIGNAL
    anoise    buthp    anoise,8000            ;HIGHPASS FILTER THE NOISE SIGNAL
    anoise    =    anoise*aenv            ;APPLY THE AMPLITUDE ENVELOPE            

    ;MIX PULSE OSCILLATOR AND NOISE COMPONENTS
    amix    =    (amix+anoise)*gklevel8*p4*0.85*gklevel
    aL,aR    pan2    amix,i(gkpan8)            ;PAN MONOPHONIC SIGNAL
        outs    aL,aR                ;SEND TO OUTPUTS
endin

instr    109    ;RIM SHOT
    xtratim    0.1
    iTR808RimShot    ftgenonce    0,0,1024,10,    0.971,0.269,0.041,0.054,0.011,0.013,0.08,0.0065,0.005,0.004,0.003,0.003,0.002,0.002,0.002,0.002,0.002,0.001,0.001,0.001,0.001,0.001,0.002,0.001,0.001    ;WAVEFORM FOR TR808 RIMSHOT
    krelease    release                ;SENSE RELEASE OF THIS NOTE ('1' WHEN RELEASED, OTHERWISE ZERO)   
    chnset    1-krelease,"Act9"                       ;TURN ON ACTIVE LIGHT WHEN NOTE STARTS, TURN IT OFF WHEN NOTE ENDS
    idur     =    0.027*i(gkdur9)         ;NOTE DURATION
    p3    limit    idur,0.1,10            ;LIMIT THE MINIMUM DURATION OF THE NOTE (VERY SHORT NOTES CAN RESULT IN THE INDICATOR LIGHT ON-OFF NOTE BEING TO0 SHORT)

    ;RING
    aenv1    expsega    1,idur,0.001,1,0.001        ;AMPLITUDE ENVELOPE FOR SUSTAIN ELEMENT OF SOUND
    ifrq1    =    1700*octave(i(gktune9))        ;FREQUENCY OF SUSTAIN ELEMENT OF SOUND
    aring    oscili    1,ifrq1,iTR808RimShot,0        ;CREATE SUSTAIN ELEMENT OF SOUND    
    aring    butbp    aring,ifrq1,ifrq1*8    
    aring    =    aring*(aenv1-0.001)*0.5            ;APPLY AMPLITUDE ENVELOPE

    ;NOISE
    anoise    noise    1,0                    ;CREATE A NOISE SIGNAL
    aenv2    expsega    1, 0.002, 0.8, 0.005, 0.5, idur-0.002-0.005, 0.0001, 1, 0.0001    ;CREATE AMPLITUDE ENVELOPE
    anoise    buthp    anoise,800            ;HIGHPASS FILTER THE NOISE SOUND
    kcf    expseg    4000,p3,20                ;CUTOFF FREQUENCY FUNCTION FOR LOWPASS FILTER
    anoise    butlp    anoise,kcf            ;LOWPASS FILTER THE SOUND
    anoise    =    anoise*(aenv2-0.001)    ;APPLY ENVELOPE TO NOISE SIGNAL

    ;MIX
    amix    =    (aring+anoise)*gklevel9*p4*0.8*gklevel
    aL,aR    pan2    amix,gkpan9            ;PAN MONOPHONIC SIGNAL  
        outs    aL,aR                ;SEND TO OUTPUTS
endin

instr    110    ;CLAVES
    xtratim    0.1
    krelease    release                ;SENSE RELEASE OF THIS NOTE ('1' WHEN RELEASED, OTHERWISE ZERO)   
    chnset    1-krelease,"Act10"                      ;TURN ON ACTIVE LIGHT WHEN NOTE STARTS, TURN IT OFF WHEN NOTE ENDS
    ifrq    =    2500*octave(i(gktune10))    ;FREQUENCY OF OSCILLATOR
    idur    =    0.045     * i(gkdur10)        ;DURATION OF THE NOTE
    p3    limit    idur,0.1,10            ;LIMIT THE MINIMUM DURATION OF THE NOTE (VERY SHORT NOTES CAN RESULT IN THE INDICATOR LIGHT ON-OFF NOTE BEING TO0 SHORT)            
    aenv    expsega    1,idur,0.001,1,0.001        ;AMPLITUDE ENVELOPE
    afmod    expsega    3,0.00005,1,1,1            ;FREQUENCY MODULATION ENVELOPE. GIVES THE SOUND A LITTLE MORE ATTACK.
    asig    oscili    -(aenv-0.001),ifrq*afmod,gisine,0    ;AUDIO OSCILLATOR
    asig    =    asig*0.4*gklevel10*p4*gklevel        ;RESCALE AMPLITUDE
    aL,aR    pan2    asig,gkpan10            ;PAN MONOPHONIC AUDIO SIGNAL
        outs    aL,aR                ;SEND AUDIO TO OUTPUTS
endin

instr    111    ;COWBELL
    xtratim    0.1
    krelease    release                ;SENSE RELEASE OF THIS NOTE ('1' WHEN RELEASED, OTHERWISE ZERO)   
    chnset    1-krelease,"Act11"                  ;TURN ON ACTIVE LIGHT WHEN NOTE STARTS, TURN IT OFF WHEN NOTE ENDS
    ifrq1    =    562 * octave(i(gktune11))    ;FREQUENCIES OF THE TWO OSCILLATORS
    ifrq2    =    845 * octave(i(gktune11))    ;
    ipw      =    0.5                 ;PULSE WIDTH OF THE OSCILLATOR    
    idur     =    0.7                 ;NOTE DURATION
    ishp     =    -30     
    idur     =    0.7                 ;NOTE DURATION
    p3    =    idur*i(gkdur11)            ;LIMIT THE MINIMUM DURATION OF THE NOTE (VERY SHORT NOTES CAN RESULT IN THE INDICATOR LIGHT ON-OFF NOTE BEING TO0 SHORT)
    ishape    =    -30                ;SHAPE OF THE CURVES IN THE AMPLITUDE ENVELOPE
    kenv1    transeg    1,p3*0.3,ishape,0.2, p3*0.7,ishape,0.2    ;FIRST AMPLITUDE ENVELOPE - PRINCIPALLY THE ATTACK OF THE NOTE
    kenv2    expon    1,p3,0.0005                ;SECOND AMPLITUDE ENVELOPE - THE SUSTAIN PORTION OF THE NOTE
    kenv    =    kenv1*kenv2            ;COMBINE THE TWO ENVELOPES
    itype    =    2                ;WAVEFORM FOR VCO2 (2=PULSE)
    a1    vco2    0.65,ifrq1,itype,ipw        ;CREATE THE TWO OSCILLATORS
    a2    vco2    0.65,ifrq2,itype,ipw
    amix    =    a1+a2                ;MIX THE TWO OSCILLATORS 
    iLPF2    =    10000                ;LOWPASS FILTER RESTING FREQUENCY
    kcf    expseg    12000,0.07,iLPF2,1,iLPF2    ;LOWPASS FILTER CUTOFF FREQUENCY ENVELOPE
    alpf    butlp    amix,kcf            ;LOWPASS FILTER THE MIX OF THE TWO OSCILLATORS (CREATE A NEW SIGNAL)
    abpf    reson    amix, ifrq2, 25            ;BANDPASS FILTER THE MIX OF THE TWO OSCILLATORS (CREATE A NEW SIGNAL)
    amix    dcblock2    (abpf*0.06*kenv1)+(alpf*0.5)+(amix*0.9)    ;MIX ALL SIGNALS AND BLOCK DC OFFSET
    amix    buthp    amix,700            ;HIGHPASS FILTER THE MIX OF ALL SIGNALS
    amix    =    amix*0.07*kenv*p4*gklevel11*gklevel    ;RESCALE AMPLITUDE
    aL,aR    pan2    amix,i(gkpan11)            ;PAN THE MONOPHONIC AUDIO SIGNAL
        outs    aL,aR                ;SEND AUDIO TO OUTPUTS
endin

instr    112    ;CLAP
    ;;xtratim    0.1
    krelease    release                ;SENSE RELEASE OF THIS NOTE ('1' WHEN RELEASED, OTHERWISE ZERO)   
    chnset    1-krelease,"Act12"                  ;TURN ON ACTIVE LIGHT WHEN NOTE STARTS, TURN IT OFF WHEN NOTE ENDS
    iTimGap    =    0.01                ;GAP BETWEEN EVENTS DURING ATTACK PORTION OF CLAP
    idur1      =    0.02                ;DURING OF THE THREE INITIAL 'CLAPS'
    idur2      =    2*i(gkdur12)            ;DURATION OF THE FOURTH, MAIN, CLAP
    idens      =    8000                ;DENSITY OF THE NOISE SIGNAL USED TO FORM THE CLAPS
    iamp1      =    0.5                ;AMPLITUDE OF AUDIO BEFORE BANDPASS FILTER IN OUTPUT
    iamp2      =    1                ;AMPLITUDE OF AUDIO AFTER BANDPASS FILTER IN OUTPUT
    if frac(p1)==0 then                ;IF THIS IS THE INITIAL NOTE (p1 WILL BE AN INTEGER)
     ;            del.  dur  env.shape
     event_i    "i", p1+0.1, 0,          idur1, p4    ;CALL THIS INSTRUMENT 4 TIMES. ADD A FRACTION ONTO p1 TO BE ABLE TO DIFFERENTIATE THESE SUBSEQUENT NOTES
     event_i    "i", p1+0.1, iTimGap,    idur1, p4
     event_i    "i", p1+0.1, iTimGap*2,  idur1, p4
     event_i    "i", p1+0.1, iTimGap*3,  idur2, p4
    else
     kenv    transeg    1,p3,-25,0                ;AMPLITUDE ENVELOPE
     iamp    random    0.7,1                    ;SLIGHT RANDOMISATION OF AMPLITUDE    
     anoise    dust2    kenv*iamp, idens            ;CREATE NOISE SIGNAL
    
     iBPF       =    1100*octave(i(gktune12))    ;FREQUENCY OF THE BANDPASS FILTER
     ibw        =    2000*octave(i(gktune12))    ;BANDWIDTH OF THE BANDPASS FILTER
     iHPF       =    1000                ;FREQUENCY OF A HIGHPASS FILTER
     iLPF       =    1                ;SCALER FOR FREQUENCY OF A LOWPASS FILTER
     kcf    expseg    8000,0.07,1700,1,800,2,500,1,500    ;CREATE CUTOFF FREQUENCY ENVELOPE
     asig    butlp    anoise,kcf*iLPF                ;LOWPASS FILTER THE SOUND
     asig    buthp    asig,iHPF                ;HIGHPASS FILTER THE SOUND
     ares    reson    asig,iBPF,ibw,1                ;BANDPASS FILTER THE SOUND (CREATE A NEW SIGNAL)
     asig    dcblock2    (asig*iamp1)+(ares*iamp2)    ;MIX BANDPASS FILTERED AND NON-BANDPASS FILTERED SOUND ELEMENTS
     asig    =    asig*p4*i(gklevel12)*1.75*gklevel    ;SCALE AMPLITUDE
     aL,aR    pan2    asig,i(gkpan12)                ;PAN MONOPHONIC SIGNAL
        outs    aL,aR                    ;SEND AUDIO TO OUTPUTS
    endif
endin

instr    113    ;MARACA
    xtratim    0.1
    krelease    release                ;SENSE RELEASE OF THIS NOTE ('1' WHEN RELEASED, OTHERWISE ZERO)   
    chnset    1-krelease,"Act13"                      ;TURN ON ACTIVE LIGHT WHEN NOTE STARTS, TURN IT OFF WHEN NOTE ENDS
    idur    =    0.07*i(gkdur13)                ;DURATION 3
    p3    limit    idur,0.1,10                ;LIMIT THE MINIMUM DURATION OF THE NOTE (VERY SHORT NOTES CAN RESULT IN THE INDICATOR LIGHT ON-OFF NOTE BEING TO0 SHORT)
    iHPF     limit    6000*octave(i(gktune13)),20,sr/2    ;HIGHPASS FILTER FREQUENCY    
    iLPF     limit    12000*octave(i(gktune13)),20,sr/3    ;LOWPASS FILTER FREQUENCY. (LIMIT MAXIMUM TO PREVENT OUT OF RANGE VALUES)
    ;AMPLITUDE ENVELOPE
    iBP1     =    0.4                    ;BREAK-POINT 1
    iDur1    =    0.014*i(gkdur13)             ;DURATION 1
    iBP2     =    1                    ;BREAKPOINT 2
    iDur2    =    0.01 *i(gkdur13)            ;DURATION 2
    iBP3     =    0.05                     ;BREAKPOINT 3
    p3    limit    idur,0.1,10                ;LIMIT THE MINIMUM DURATION OF THE NOTE (VERY SHORT NOTES CAN RESULT IN THE INDICATOR LIGHT ON-OFF NOTE BEING TO0 SHORT)
    aenv    expsega    iBP1,iDur1,iBP2,iDur2,iBP3        ;CREATE AMPLITUDE ENVELOPE
    anoise    noise    0.75,0                    ;CREATE A NOISE SIGNAL
    anoise    buthp    anoise,iHPF                ;HIGHPASS FILTER THE SOUND
    anoise    butlp    anoise,iLPF                ;LOWPASS FILTER THE SOUND
    anoise    =    anoise*aenv*p4*gklevel13*gklevel    ;SCALE THE AMPLITUDE
    aL,aR    pan2    anoise,i(gkpan13)            ;PAN THE MONOPONIC SIGNAL
        outs    aL,aR                    ;SEND AUDIO TO OUTPUTS
endin

instr    114    ;HIGH CONGA
    xtratim    0.1
    krelease    release                ;SENSE RELEASE OF THIS NOTE ('1' WHEN RELEASED, OTHERWISE ZERO)   
    chnset    1-krelease,"Act14"                      ;TURN ON ACTIVE LIGHT WHEN NOTE STARTS, TURN IT OFF WHEN NOTE ENDS
    ifrq      =    420*octave(i(gktune14))        ;FREQUENCY OF NOTE
    p3      =    0.22*i(gkdur14)            ;DURATION OF NOTE
    aenv    transeg    0.7,1/ifrq,1,1,p3,-6,0.001    ;AMPLITUDE ENVELOPE
    afrq    expsega    ifrq*3,0.25/ifrq,ifrq,1,ifrq    ;FREQUENCY ENVELOPE (CREATE A SHARPER ATTACK)
    asig    oscili    -aenv*0.25,afrq,gisine        ;CREATE THE AUDIO OSCILLATOR
    asig    =    asig*p4*gklevel14*gklevel    ;SCALE THE AMPLITUDE
    aL,aR    pan2    asig,gkpan14            ;PAN THE MONOPHONIC AUDIO SIGNAL
        outs    aL,aR                ;SEND AUDIO TO THE OUTPUTS
endin

instr    115    ;MID CONGA
    xtratim    0.1
    krelease    release                ;SENSE RELEASE OF THIS NOTE ('1' WHEN RELEASED, OTHERWISE ZERO)   
    chnset    1-krelease,"Act15"                      ;TURN ON ACTIVE LIGHT WHEN NOTE STARTS, TURN IT OFF WHEN NOTE ENDS
    ifrq       =    310*octave(i(gktune15))        ;FREQUENCY OF NOTE
    p3       =    0.33*i(gkdur15)            ;DURATION OF NOTE
    aenv    transeg    0.7,1/ifrq,1,1,p3,-6,0.001    ;AMPLITUDE ENVELOPE    
    afrq    expsega    ifrq*3,0.25/ifrq,ifrq,1,ifrq    ;FREQUENCY ENVELOPE (CREATE A SHARPER ATTACK)
    asig    oscili    -aenv*0.25,afrq,gisine        ;CREATE THE AUDIO OSCILLATOR
    asig    =    asig*p4*gklevel15*gklevel        ;SCALE THE AMPLITUDE
    aL,aR    pan2    asig,gkpan15            ;PAN THE MONOPHONIC AUDIO SIGNAL
        outs    aL,aR                ;SEND AUDIO TO THE OUTPUTS
endin

instr    116    ;LOW CONGA
    xtratim    0.1
    krelease    release                ;SENSE RELEASE OF THIS NOTE ('1' WHEN RELEASED, OTHERWISE ZERO)   
    chnset    1-krelease,"Act16"                      ;TURN ON ACTIVE LIGHT WHEN NOTE STARTS, TURN IT OFF WHEN NOTE ENDS
    ifrq       =    227*octave(i(gktune16))        ;FREQUENCY OF NOTE
    p3       =    0.41*i(gkdur16)            ;DURATION OF NOTE     
    aenv    transeg    0.7,1/ifrq,1,1,p3,-6,0.001    ;AMPLITUDE ENVELOPE    
    afrq    expsega    ifrq*3,0.25/ifrq,ifrq,1,ifrq    ;FREQUENCY ENVELOPE (CREATE A SHARPER ATTACK)
    asig    oscili    -aenv*0.25,afrq,gisine        ;CREATE THE AUDIO OSCILLATOR
    asig    =    asig*p4*gklevel16*gklevel    ;SCALE THE AMPLITUDE
    aL,aR    pan2    asig,gkpan16            ;PAN THE MONOPHONIC AUDIO SIGNAL
        outs    aL,aR                ;SEND AUDIO TO THE OUTPUTS
endin

</CsInstruments>

<CsScore>
f 0 [3600*24*7]
i 1 0 [3600*24*7]
</CsScore>

</CsoundSynthesizer>