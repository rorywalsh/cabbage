
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; GEN09.csd
; Written by Iain McCurdy
; "P.N.1-8"     sets the partial number for each of the 8 partials. Note that "Base" and "Int" will also have an influence upon the partial number values.
; "Str.1-8"     sets the strengths for the 8 partials.
; "Mute 1-8"    allow the user to mute individual partials
; "Solo 1-8"    allow the user to solo individual partials (multiple solos can be employed)
; "Ph.1-8"      sets the initial phases (in degrees) for the 8 partials.
; "Base"        sets a base offset (integer) for all partial numbers. The oscillator frequency will also be divided by this value.
; "Int."        defines an additional cumulative interval (integer) between partials. E.g. if "Int." is '2', partials 1 will be unaffected, an additional interval of '2' will be added to partial 2, an additional interval of '4' will be added to partial 3 and so on. 
; Table Size    size of the table used by the synthesizer.
;               If table size is reduced quantisation artefacts will become more prevalent, particularly if partial numbers are high. This 'lo-fi' effect may be desirable. 
;               In actual fact, separate tables are used for each table size but the table display widget only ever displays the ninth table.

;               The frequency of the audio oscillator is always scaled down according to the lowest partial number defined. This is to ensure that an audible fundemental is always played, something that may not otherwise occur if all partial numbers are high.
;               The user can choose between three opcodes for synthesis: oscil, oscili or poscil. 
;               The interpolating opcodes oscili and poscil are less likely to produce quantisation artifacts when small table sizes are used.
;               The waveform can be played back using oscbnk (if 'voices' is greater than 1), in which case 'spread' and 'speed' can be used to modify the texture of the tone cluster.

<Cabbage>
form caption("GEN09"), size(420, 555), pluginId("gn09"), colour("Black")

gentable bounds( 10,  5, 400, 120), tableNumber(8), tableColour("LightBlue"), identChannel("table"), zoom(-1)

rslider bounds( 13,130, 43, 43), channel("pn1"), range(1, 200, 1,1,1), colour(230,230,230,200)
rslider bounds( 63,130, 43, 43), channel("pn2"), range(1, 200, 2,1,1), colour(230,230,230,200)
rslider bounds(113,130, 43, 43), channel("pn3"), range(1, 200, 3,1,1), colour(230,230,230,200)
rslider bounds(163,130, 43, 43), channel("pn4"), range(1, 200, 4,1,1), colour(230,230,230,200)
rslider bounds(213,130, 43, 43), channel("pn5"), range(1, 200, 5,1,1), colour(230,230,230,200)
rslider bounds(263,130, 43, 43), channel("pn6"), range(1, 200, 6,1,1), colour(230,230,230,200)
rslider bounds(313,130, 43, 43), channel("pn7"), range(1, 200, 7,1,1), colour(230,230,230,200)
rslider bounds(363,130, 43, 43), channel("pn8"), range(1, 200, 8,1,1), colour(230,230,230,200)

;rslider  bounds(  0,175, 70, 26), text("P.N.1"), textBox(1), valueTextBox(1), channel("pn1_out"), range(1, 10000, 1, 1, 1)
;rslider  bounds( 50,175, 70, 26), text("P.N.2"), textBox(1), valueTextBox(1), channel("pn2_out"), range(1, 10000, 1, 1, 1)
;rslider  bounds(100,175, 70, 26), text("P.N.3"), textBox(1), valueTextBox(1), channel("pn3_out"), range(1, 10000, 1, 1, 1)
;rslider  bounds(150,175, 70, 26), text("P.N.4"), textBox(1), valueTextBox(1), channel("pn4_out"), range(1, 10000, 1, 1, 1)
;rslider  bounds(200,175, 70, 26), text("P.N.5"), textBox(1), valueTextBox(1), channel("pn5_out"), range(1, 10000, 1, 1, 1)
;rslider  bounds(250,175, 70, 26), text("P.N.6"), textBox(1), valueTextBox(1), channel("pn6_out"), range(1, 10000, 1, 1, 1)
;rslider  bounds(300,175, 70, 26), text("P.N.7"), textBox(1), valueTextBox(1), channel("pn7_out"), range(1, 10000, 1, 1, 1)
;rslider  bounds(350,175, 70, 26), text("P.N.8"), textBox(1), valueTextBox(1), channel("pn8_out"), range(1, 10000, 1, 1, 1)

nslider  bounds(  0,175, 70, 26), text("P.N.1"), textBox(1), valueTextBox(1), channel("pn1_out"), range(1, 10000, 1, 1, 1)
nslider  bounds( 50,175, 70, 26), text("P.N.2"), textBox(1), valueTextBox(1), channel("pn2_out"), range(1, 10000, 1, 1, 1)
nslider  bounds(100,175, 70, 26), text("P.N.3"), textBox(1), valueTextBox(1), channel("pn3_out"), range(1, 10000, 1, 1, 1)
nslider  bounds(150,175, 70, 26), text("P.N.4"), textBox(1), valueTextBox(1), channel("pn4_out"), range(1, 10000, 1, 1, 1)
nslider  bounds(200,175, 70, 26), text("P.N.5"), textBox(1), valueTextBox(1), channel("pn5_out"), range(1, 10000, 1, 1, 1)
nslider  bounds(250,175, 70, 26), text("P.N.6"), textBox(1), valueTextBox(1), channel("pn6_out"), range(1, 10000, 1, 1, 1)
nslider  bounds(300,175, 70, 26), text("P.N.7"), textBox(1), valueTextBox(1), channel("pn7_out"), range(1, 10000, 1, 1, 1)
nslider  bounds(350,175, 70, 26), text("P.N.8"), textBox(1), valueTextBox(1), channel("pn8_out"), range(1, 10000, 1, 1, 1)

rslider bounds(  5,205, 50, 70), channel("str1"), text("Str.1"), textBox(1), valueTextBox(1), range(0, 1, 1, 0.5, 0.01), colour(200,200,200,200)
rslider bounds( 55,205, 50, 70), channel("str2"), text("Str.2"), textBox(1), valueTextBox(1), range(0, 1, 0.5, 0.5, 0.01), colour(200,200,200,200)
rslider bounds(105,205, 50, 70), channel("str3"), text("Str.3"), textBox(1), valueTextBox(1), range(0, 1, 0.3, 0.5, 0.01), colour(200,200,200,200)
rslider bounds(155,205, 50, 70), channel("str4"), text("Str.4"), textBox(1), valueTextBox(1), range(0, 1, 0.25, 0.5, 0.01), colour(200,200,200,200)
rslider bounds(205,205, 50, 70), channel("str5"), text("Str.5"), textBox(1), valueTextBox(1), range(0, 1, 0.2, 0.5, 0.01), colour(200,200,200,200)
rslider bounds(255,205, 50, 70), channel("str6"), text("Str.6"), textBox(1), valueTextBox(1), range(0, 1, 0.16, 0.5, 0.01), colour(200,200,200,200)
rslider bounds(305,205, 50, 70), channel("str7"), text("Str.7"), textBox(1), valueTextBox(1), range(0, 1, 0.14287, 0.5, 0.01), colour(200,200,200,200)
rslider bounds(355,205, 50, 70), channel("str8"), text("Str.8"), textBox(1), valueTextBox(1), range(0, 1, 0.125, 0.5, 0.01), colour(200,200,200,200)

checkbox bounds( 29,275, 12, 12), channel("mute1"),  value(0), colour("red"), shape("square")
checkbox bounds( 79,275, 12, 12), channel("mute2"),  value(0), colour("red"), shape("square")
checkbox bounds(129,275, 12, 12), channel("mute3"),  value(0), colour("red"), shape("square")
checkbox bounds(179,275, 12, 12), channel("mute4"),  value(0), colour("red"), shape("square")
checkbox bounds(229,275, 12, 12), channel("mute5"),  value(0), colour("red"), shape("square")
checkbox bounds(279,275, 12, 12), channel("mute6"),  value(0), colour("red"), shape("square")
checkbox bounds(329,275, 12, 12), channel("mute7"),  value(0), colour("red"), shape("square")
checkbox bounds(379,275, 12, 12), channel("mute8"),  value(0), colour("red"), shape("square")

checkbox bounds( 29,297, 12, 12), channel("solo1"),  value(0), colour("yellow"), shape("square")
checkbox bounds( 79,297, 12, 12), channel("solo2"),  value(0), colour("yellow"), shape("square")
checkbox bounds(129,297, 12, 12), channel("solo3"),  value(0), colour("yellow"), shape("square")
checkbox bounds(179,297, 12, 12), channel("solo4"),  value(0), colour("yellow"), shape("square")
checkbox bounds(229,297, 12, 12), channel("solo5"),  value(0), colour("yellow"), shape("square")
checkbox bounds(279,297, 12, 12), channel("solo6"),  value(0), colour("yellow"), shape("square")
checkbox bounds(329,297, 12, 12), channel("solo7"),  value(0), colour("yellow"), shape("square")
checkbox bounds(379,297, 12, 12), channel("solo8"),  value(0), colour("yellow"), shape("square")

label    bounds( 23,286, 24, 10), text("Mute"),  fontColour("white")
label    bounds( 73,286, 24, 10), text("Mute"),  fontColour("white")
label    bounds(123,286, 24, 10), text("Mute"),  fontColour("white")
label    bounds(173,286, 24, 10), text("Mute"),  fontColour("white")
label    bounds(223,286, 24, 10), text("Mute"),  fontColour("white")
label    bounds(273,286, 24, 10), text("Mute"),  fontColour("white")
label    bounds(323,286, 24, 10), text("Mute"),  fontColour("white")
label    bounds(373,286, 24, 10), text("Mute"),  fontColour("white")

label    bounds( 24,307, 21, 10), text("Solo"),  fontColour("white")
label    bounds( 74,307, 21, 10), text("Solo"),  fontColour("white")
label    bounds(124,307, 21, 10), text("Solo"),  fontColour("white")
label    bounds(174,307, 21, 10), text("Solo"),  fontColour("white")
label    bounds(224,307, 21, 10), text("Solo"),  fontColour("white")
label    bounds(274,307, 21, 10), text("Solo"),  fontColour("white")
label    bounds(324,307, 21, 10), text("Solo"),  fontColour("white")
label    bounds(374,307, 21, 10), text("Solo"),  fontColour("white")

rslider bounds(  5,320, 60, 70), channel("ph1"), text("Ph.1"), textBox(1), valueTextBox(1), range(0, 360, 0, 1, 1), colour(150,150,150,200)
rslider bounds( 55,320, 60, 70), channel("ph2"), text("Ph.2"), textBox(1), valueTextBox(1), range(0, 360, 0, 1, 1), colour(150,150,150,200)
rslider bounds(105,320, 60, 70), channel("ph3"), text("Ph.3"), textBox(1), valueTextBox(1), range(0, 360, 0, 1, 1), colour(150,150,150,200)
rslider bounds(155,320, 60, 70), channel("ph4"), text("Ph.4"), textBox(1), valueTextBox(1), range(0, 360, 0, 1, 1), colour(150,150,150,200)
rslider bounds(205,320, 60, 70), channel("ph5"), text("Ph.5"), textBox(1), valueTextBox(1), range(0, 360, 0, 1, 1), colour(150,150,150,200)
rslider bounds(255,320, 60, 70), channel("ph6"), text("Ph.6"), textBox(1), valueTextBox(1), range(0, 360, 0, 1, 1), colour(150,150,150,200)
rslider bounds(305,320, 60, 70), channel("ph7"), text("Ph.7"), textBox(1), valueTextBox(1), range(0, 360, 0, 1, 1), colour(150,150,150,200)
rslider bounds(355,320, 60, 70), channel("ph8"), text("Ph.8"), textBox(1), valueTextBox(1), range(0, 360, 0, 1, 1), colour(150,150,150,200)

rslider  bounds( 10,395, 50, 70), channel("base"), text("Base"), textBox(1), valueTextBox(1), range(0, 200, 1, 1,1), colour(110,110,110,200)
rslider  bounds( 60,395, 50, 70), channel("int"), text("Int."), textBox(1), valueTextBox(1), range(0, 200, 2, 1,1) , colour(110,110,110,200)
label    bounds(120,400, 95, 13), text("Table Size")
combobox bounds(120,415, 95, 20), channel("tabsize"), value(8), text("32","64","128","256","512","1024","2048","4096","8192")
label    bounds(195,400, 95, 13), text("Opcode")
combobox bounds(195,415, 95, 20), channel("opcode"), value(3), text("oscil","oscili","poscil")
button   bounds(285,395,120, 20), text("EXPORT TABLE"), channel("export"), value(0), fontColour("yellow")

rslider bounds(283,420, 45, 40), channel("spread"), text("Spread"), range(0, 1, 0.05,0.5,0.00001), colour("yellow")
rslider bounds(323,420, 45, 40), channel("voices"), text("Voices"), range(1, 20, 8,1,1), colour("yellow")
rslider bounds(363,420, 45, 40), channel("speed"), text("Speed"), range(0,100, 1.5,0.5,0.001), colour("yellow")

keyboard bounds(  0,475,420, 80)

</Cabbage>
                    
<CsoundSynthesizer>

<CsOptions>   
-dm0 -n -+rtmidi=NULL -M0
</CsOptions>

<CsInstruments>

; sr set by host
ksmps        =          32    ; NUMBER OF AUDIO SAMPLES IN EACH CONTROL CYCLE
nchnls       =          2     ; NUMBER OF CHANNELS (1=MONO)
0dbfs        =          1     ; MAXIMUM AMPLITUDE
             massign    0,3   ; send all midi notes to instr 3

gicos        ftgen    0,0,4096,11,1        ;COSINE WAVE (USED BY THE LFOS)
gieqffn      ftgen    0,0,4097,7,-1,4096,1
gieqlfn      ftgen    0,0,4097,7,-1,4096,1
gieqqfn      ftgen    0,0,4097,7,-1,4096,1

instr    1
    ; read in widgets
    gkpn1      chnget    "pn1"
    gkpn2      chnget    "pn2"
    gkpn3      chnget    "pn3"
    gkpn4      chnget    "pn4"
    gkpn5      chnget    "pn5"
    gkpn6      chnget    "pn6"
    gkpn7      chnget    "pn7"
    gkpn8      chnget    "pn8"

    gkstr1     chnget    "str1"
    gkstr2     chnget    "str2"
    gkstr3     chnget    "str3"
    gkstr4     chnget    "str4"
    gkstr5     chnget    "str5"
    gkstr6     chnget    "str6"
    gkstr7     chnget    "str7"
    gkstr8     chnget    "str8"

    gkmute1    chnget    "mute1"
    gkmute2    chnget    "mute2"
    gkmute3    chnget    "mute3"
    gkmute4    chnget    "mute4"
    gkmute5    chnget    "mute5"
    gkmute6    chnget    "mute6"
    gkmute7    chnget    "mute7"
    gkmute8    chnget    "mute8"

    gksolo1    chnget    "solo1"
    gksolo2    chnget    "solo2"
    gksolo3    chnget    "solo3"
    gksolo4    chnget    "solo4"
    gksolo5    chnget    "solo5"
    gksolo6    chnget    "solo6"
    gksolo7    chnget    "solo7"
    gksolo8    chnget    "solo8"

    kSoloSum   =         gksolo1+gksolo2+gksolo3+gksolo4+gksolo5+gksolo6+gksolo7+gksolo8

#define    SOLO_MUTE_STATUS(N)    
    #
    if gksolo$N==1 then
     kstatus$N    =    1
    elseif kSoloSum>0 then
     kstatus$N    =    0
    else
     kstatus$N    =    (1-gkmute$N)
    endif
    #
    $SOLO_MUTE_STATUS(1)
    $SOLO_MUTE_STATUS(2)
    $SOLO_MUTE_STATUS(3)
    $SOLO_MUTE_STATUS(4)
    $SOLO_MUTE_STATUS(5)
    $SOLO_MUTE_STATUS(6)
    $SOLO_MUTE_STATUS(7)
    $SOLO_MUTE_STATUS(8)

    gkph1     chnget    "ph1"
    gkph2     chnget    "ph2"
    gkph3     chnget    "ph3"
    gkph4     chnget    "ph4"
    gkph5     chnget    "ph5"
    gkph6     chnget    "ph6"
    gkph7     chnget    "ph7"
    gkph8     chnget    "ph8"

    gkbase    chnget    "base"
    gkint     chnget    "int"
    gkopcode  chnget    "opcode"
    gkopcode  init      3        ; init pass value for gkopcode
    
    ; if any of the variables in the input list are changed, a momentary '1' trigger is generated at the output. This trigger is used to update function tables.
    ktrig     changed   gkpn1,gkpn2,gkpn3,gkpn4,gkpn5,gkpn6,gkpn7,gkpn8,gkstr1,gkstr2,gkstr3,gkstr4,gkstr5,gkstr6,gkstr7,gkstr8,kstatus1,kstatus2,kstatus3,kstatus4,kstatus5,kstatus6,kstatus7,kstatus8,gkph1,gkph2,gkph3,gkph4,gkph5,gkph6,gkph7,gkph8,gkbase,gkint

    if ktrig==1 then
              reinit    UPDATE
    endif
    UPDATE:
    ; Update function tables.
    gi1    ftgen    1, 0,   32,9, i(gkbase)+i(gkpn1),i(gkstr1)*i(kstatus1),i(gkph1), i(gkbase)+i(gkpn2)+i(gkint),i(gkstr2)*i(kstatus2),i(gkph2), i(gkbase)+i(gkpn3)+(i(gkint)*2),i(gkstr3)*i(kstatus3),i(gkph3), i(gkbase)+i(gkpn4)+(i(gkint)*3),i(gkstr4)*i(kstatus4),i(gkph4), i(gkbase)+i(gkpn5)+(i(gkint)*4),i(gkstr5)*i(kstatus5),i(gkph5), i(gkbase)+i(gkpn6)+(i(gkint)*5),i(gkstr6)*i(kstatus6),i(gkph6), i(gkbase)+i(gkpn7)+(i(gkint)*6),i(gkstr7)*i(kstatus7),i(gkph7), i(gkbase)+i(gkpn8)+(i(gkint)*7),i(gkstr8)*i(kstatus8),i(gkph8)
    gi2    ftgen    2, 0,   64,9, i(gkbase)+i(gkpn1),i(gkstr1)*i(kstatus1),i(gkph1), i(gkbase)+i(gkpn2)+i(gkint),i(gkstr2)*i(kstatus2),i(gkph2), i(gkbase)+i(gkpn3)+(i(gkint)*2),i(gkstr3)*i(kstatus3),i(gkph3), i(gkbase)+i(gkpn4)+(i(gkint)*3),i(gkstr4)*i(kstatus4),i(gkph4), i(gkbase)+i(gkpn5)+(i(gkint)*4),i(gkstr5)*i(kstatus5),i(gkph5), i(gkbase)+i(gkpn6)+(i(gkint)*5),i(gkstr6)*i(kstatus6),i(gkph6), i(gkbase)+i(gkpn7)+(i(gkint)*6),i(gkstr7)*i(kstatus7),i(gkph7), i(gkbase)+i(gkpn8)+(i(gkint)*7),i(gkstr8)*i(kstatus8),i(gkph8)
    gi3    ftgen    3, 0,  128,9, i(gkbase)+i(gkpn1),i(gkstr1)*i(kstatus1),i(gkph1), i(gkbase)+i(gkpn2)+i(gkint),i(gkstr2)*i(kstatus2),i(gkph2), i(gkbase)+i(gkpn3)+(i(gkint)*2),i(gkstr3)*i(kstatus3),i(gkph3), i(gkbase)+i(gkpn4)+(i(gkint)*3),i(gkstr4)*i(kstatus4),i(gkph4), i(gkbase)+i(gkpn5)+(i(gkint)*4),i(gkstr5)*i(kstatus5),i(gkph5), i(gkbase)+i(gkpn6)+(i(gkint)*5),i(gkstr6)*i(kstatus6),i(gkph6), i(gkbase)+i(gkpn7)+(i(gkint)*6),i(gkstr7)*i(kstatus7),i(gkph7), i(gkbase)+i(gkpn8)+(i(gkint)*7),i(gkstr8)*i(kstatus8),i(gkph8)
    gi4    ftgen    4, 0,  256,9, i(gkbase)+i(gkpn1),i(gkstr1)*i(kstatus1),i(gkph1), i(gkbase)+i(gkpn2)+i(gkint),i(gkstr2)*i(kstatus2),i(gkph2), i(gkbase)+i(gkpn3)+(i(gkint)*2),i(gkstr3)*i(kstatus3),i(gkph3), i(gkbase)+i(gkpn4)+(i(gkint)*3),i(gkstr4)*i(kstatus4),i(gkph4), i(gkbase)+i(gkpn5)+(i(gkint)*4),i(gkstr5)*i(kstatus5),i(gkph5), i(gkbase)+i(gkpn6)+(i(gkint)*5),i(gkstr6)*i(kstatus6),i(gkph6), i(gkbase)+i(gkpn7)+(i(gkint)*6),i(gkstr7)*i(kstatus7),i(gkph7), i(gkbase)+i(gkpn8)+(i(gkint)*7),i(gkstr8)*i(kstatus8),i(gkph8)
    gi5    ftgen    5, 0,  512,9, i(gkbase)+i(gkpn1),i(gkstr1)*i(kstatus1),i(gkph1), i(gkbase)+i(gkpn2)+i(gkint),i(gkstr2)*i(kstatus2),i(gkph2), i(gkbase)+i(gkpn3)+(i(gkint)*2),i(gkstr3)*i(kstatus3),i(gkph3), i(gkbase)+i(gkpn4)+(i(gkint)*3),i(gkstr4)*i(kstatus4),i(gkph4), i(gkbase)+i(gkpn5)+(i(gkint)*4),i(gkstr5)*i(kstatus5),i(gkph5), i(gkbase)+i(gkpn6)+(i(gkint)*5),i(gkstr6)*i(kstatus6),i(gkph6), i(gkbase)+i(gkpn7)+(i(gkint)*6),i(gkstr7)*i(kstatus7),i(gkph7), i(gkbase)+i(gkpn8)+(i(gkint)*7),i(gkstr8)*i(kstatus8),i(gkph8)
    gi6    ftgen    6, 0, 1024,9, i(gkbase)+i(gkpn1),i(gkstr1)*i(kstatus1),i(gkph1), i(gkbase)+i(gkpn2)+i(gkint),i(gkstr2)*i(kstatus2),i(gkph2), i(gkbase)+i(gkpn3)+(i(gkint)*2),i(gkstr3)*i(kstatus3),i(gkph3), i(gkbase)+i(gkpn4)+(i(gkint)*3),i(gkstr4)*i(kstatus4),i(gkph4), i(gkbase)+i(gkpn5)+(i(gkint)*4),i(gkstr5)*i(kstatus5),i(gkph5), i(gkbase)+i(gkpn6)+(i(gkint)*5),i(gkstr6)*i(kstatus6),i(gkph6), i(gkbase)+i(gkpn7)+(i(gkint)*6),i(gkstr7)*i(kstatus7),i(gkph7), i(gkbase)+i(gkpn8)+(i(gkint)*7),i(gkstr8)*i(kstatus8),i(gkph8)
    gi7    ftgen    7, 0, 2048,9, i(gkbase)+i(gkpn1),i(gkstr1)*i(kstatus1),i(gkph1), i(gkbase)+i(gkpn2)+i(gkint),i(gkstr2)*i(kstatus2),i(gkph2), i(gkbase)+i(gkpn3)+(i(gkint)*2),i(gkstr3)*i(kstatus3),i(gkph3), i(gkbase)+i(gkpn4)+(i(gkint)*3),i(gkstr4)*i(kstatus4),i(gkph4), i(gkbase)+i(gkpn5)+(i(gkint)*4),i(gkstr5)*i(kstatus5),i(gkph5), i(gkbase)+i(gkpn6)+(i(gkint)*5),i(gkstr6)*i(kstatus6),i(gkph6), i(gkbase)+i(gkpn7)+(i(gkint)*6),i(gkstr7)*i(kstatus7),i(gkph7), i(gkbase)+i(gkpn8)+(i(gkint)*7),i(gkstr8)*i(kstatus8),i(gkph8)
    gi8    ftgen    8, 0, 4096,9, i(gkbase)+i(gkpn1),i(gkstr1)*i(kstatus1),i(gkph1), i(gkbase)+i(gkpn2)+i(gkint),i(gkstr2)*i(kstatus2),i(gkph2), i(gkbase)+i(gkpn3)+(i(gkint)*2),i(gkstr3)*i(kstatus3),i(gkph3), i(gkbase)+i(gkpn4)+(i(gkint)*3),i(gkstr4)*i(kstatus4),i(gkph4), i(gkbase)+i(gkpn5)+(i(gkint)*4),i(gkstr5)*i(kstatus5),i(gkph5), i(gkbase)+i(gkpn6)+(i(gkint)*5),i(gkstr6)*i(kstatus6),i(gkph6), i(gkbase)+i(gkpn7)+(i(gkint)*6),i(gkstr7)*i(kstatus7),i(gkph7), i(gkbase)+i(gkpn8)+(i(gkint)*7),i(gkstr8)*i(kstatus8),i(gkph8)
    gi9    ftgen    9, 0, 8192,9, i(gkbase)+i(gkpn1),i(gkstr1)*i(kstatus1),i(gkph1), i(gkbase)+i(gkpn2)+i(gkint),i(gkstr2)*i(kstatus2),i(gkph2), i(gkbase)+i(gkpn3)+(i(gkint)*2),i(gkstr3)*i(kstatus3),i(gkph3), i(gkbase)+i(gkpn4)+(i(gkint)*3),i(gkstr4)*i(kstatus4),i(gkph4), i(gkbase)+i(gkpn5)+(i(gkint)*4),i(gkstr5)*i(kstatus5),i(gkph5), i(gkbase)+i(gkpn6)+(i(gkint)*5),i(gkstr6)*i(kstatus6),i(gkph6), i(gkbase)+i(gkpn7)+(i(gkint)*6),i(gkstr7)*i(kstatus7),i(gkph7), i(gkbase)+i(gkpn8)+(i(gkint)*7),i(gkstr8)*i(kstatus8),i(gkph8)
    rireturn
    if ktrig==1 then
     chnset    "tableNumber(8)", "table"    ; update table display    
    endif
    ; update "P.N.1-8" (partial numbers) value display boxes.
    chnset    (gkbase)+(gkpn1),"pn1_out"
    chnset    (gkbase)+(gkpn2)+(gkint),"pn2_out"
    chnset    (gkbase)+(gkpn3)+((gkint)*2),"pn3_out"
    chnset    (gkbase)+(gkpn4)+((gkint)*3),"pn4_out"
    chnset    (gkbase)+(gkpn5)+((gkint)*4),"pn5_out"
    chnset    (gkbase)+(gkpn6)+((gkint)*5),"pn6_out"
    chnset    (gkbase)+(gkpn7)+((gkint)*6),"pn7_out"
    chnset    (gkbase)+(gkpn8)+((gkint)*7),"pn8_out"
    gkscal    min    gkbase+gkpn1, gkbase+gkpn2, gkbase+gkpn3, gkbase+gkpn4, gkbase+gkpn5, gkbase+gkpn6, gkbase+gkpn7, gkbase+gkpn8
    rireturn

    kexport    chnget    "export"
    ktrig    changed    kexport
        schedkwhen    ktrig,0,0,4,0,0.01

    gktabsize    init    9        ; init pass value for gktabsize
endin


instr    3    ; sound producing instrument

    gktabsize    chnget    "tabsize"
    gktabsize    init    9        ; init pass value for gktabsize

    icps    cpsmidi                ; CPS from midi note played
    iamp    ampmidi    0.3            ; amplitude from midi note velocity 
    ifn    init    i(gktabsize)        ; function table to be used by the oscillator. This will be dependent upon tables size selected.

    kvoices    chnget    "voices"
    ktrig    changed    kvoices
    if ktrig==1 then
     reinit update
    endif
    update:
    ivoices    init    i(kvoices)
    iscal    =    i(gkscal)
    aenv    linsegr    0,0.01,1,0.1,0        ; amplitude envelope to prevent clicks


    if ivoices==1 then    
     if gkopcode==1 then            ; if option 1 is selected by the "Opcode" combo box...
      asig    oscil    iamp,icps/gkscal,ifn    ; use oscil
     elseif gkopcode==2 then        ; if option 2 is selected by the "Opcode" combo box...
      asig    oscili    iamp,icps/gkscal,ifn    ; use oscili                                         
     elseif gkopcode==3 then        ; if option 3 is selected by the "Opcode" combo box...
      asig    poscil    iamp,icps/gkscal,ifn    ; use poscil                                         
     endif                    ; end of conditional
     asig    =    asig * aenv        ; apply envelope
          outs    asig,asig
        else
         kwave    init    ifn
         kspread    chnget    "spread"
         kspeed        chnget    "speed"
            ;OUTPUT    OPCODE  CPS  | AMD  |    FMD    |  PMD  | OVERLAPS   | SEED | L1MINF  | L1MAXF  | L2MINF  | L2MAXF  | LFOMODE | EQMINF  | EQMAXF | EQMINL | EQMAXL | EQMINQ | EQMAXQ  | EQMODE | KFN  | L1FN | L2FN | EQFFN  | EQLF   |  EQQFN |  TABL  | OUTFN
     aL    oscbnk    icps/(gkscal),   0,      0,      kspread,     ivoices,   rnd(1),      0,   kspeed,      0,        0,       238,      0,       8000,      1,       1,       1,       1,       -1,   kwave, gicos, gicos, gieqffn, gieqlfn, gieqqfn
     aR    oscbnk    icps/(gkscal),   0,      0,     -kspread,     ivoices,   rnd(1),      0,  -kspeed,      0,        0,       238,      0,       8000,      1,       1,       1,       1,       -1,   kwave, gicos, gicos, gieqffn, gieqlfn, gieqqfn
     aL    =    (aL * aenv * iamp) / (ivoices^0.5)         ; apply envelope
     aR    =    (aR * aenv * iamp) / (ivoices^0.5)        ; apply envelope
          outs    aL, aR
        endif
endin

instr    4
    ifn         chnget    "tabsize"
    isize       =         ftlen(ifn)
    SFile       =         "/GEN09Table.txt"
    SPath       chnget    "USER_HOME_DIRECTORY"
    SFilePath   strcat    SPath,SFile
    
    fprints    SFilePath, "giGEN09wave ftgen 0,0,%d,9,%d,%f,%f,%d,%f,%f,%d,%f,%f,%d,%f,%f,%d,%f,%f,%d,%f,%f,%d,%f,%f,%d,%f,%f",isize, i(gkbase)+i(gkpn1), i(gkstr1), i(gkph1), i(gkbase)+i(gkpn2)+i(gkint), i(gkstr2), i(gkph2), i(gkbase)+i(gkpn3)+(i(gkint)*2), i(gkstr3), i(gkph3), i(gkbase)+i(gkpn4)+(i(gkint)*3), i(gkstr4), i(gkph4), i(gkbase)+i(gkpn5)+(i(gkint)*4), i(gkstr5), i(gkph5), i(gkbase)+i(gkpn6)+(i(gkint)*5), i(gkstr6), i(gkph6), i(gkbase)+i(gkpn7)+(i(gkint)*6), i(gkstr7), i(gkph7), i(gkbase)+i(gkpn8)+(i(gkint)*7), i(gkstr8), i(gkph8)

endin

</CsInstruments>

<CsScore>
; create the function tables
f1 0    32 9  1 0 0
f2 0    64 9  1 0 0
f3 0   128 9  1 0 0
f4 0   256 9  1 0 0
f5 0   512 9  1 0 0
f6 0  1024 9  1 0 0
f7 0  2048 9  1 0 0
f8 0  4096 9  1 0 0
f9 0  8192 9  1 0 0
; play instrument 1 for 1 hour
i 1 0 3600
</CsScore>

</CsoundSynthesizer>
