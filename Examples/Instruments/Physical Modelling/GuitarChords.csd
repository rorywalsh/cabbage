; GuitarChords.csd
; Written by Iain McCurdy, 2012.

<Cabbage>
form size(650, 265), caption("Guitar Chords"), pluginID("guit"), guirefresh(64)

label  bounds( 54, -1, 25, 10), text("Mute"), FontColour("white")
label  bounds( 84, -1, 25, 10), text("Open"), FontColour("white")

combobox bounds(  3, 10, 50, 15), channel("TuneHighE"), value(64), text("G9","F#9","F9","E9","D#9","D9","C#9","C9","B8","A#8","A8","G#8","G8","F#8","F8","E8","D#8","D8","C#8","C8","B7","A#7","A7","G#7","G7","F#7","F7","E7","D#7","D7","C#7","C7","B6","A#6","A6","G#6","G6","F#6","F6","E6","D#6","D6","C#6","C6","B5","A#5","A5","G#5","G5","F#5","F5","E5","D#5","D5","C#5","C5","B4","A#4","A4","G#4","G4","F#4","F4","E4","D#4","D4","C#4","C4","B3","A#3","A3","G#3","G3","F#3","F3","E3","D#3","D3","C#3","C3","B2","A#2","A2","G#2","G2","F#2","F2","E2","D#2","D2","C#2","C2","B1","A#1","A1","G#1","G1","F#1","F1","E1","D#1","D1","C#1","C1","B0","A#0","A0","G#0","G0","F#0","F0","E0","D#0","D0","C#0","C0","B-1","A#-1","A-1","G#-1","G-1","F#-1","F-1","E-1","D#-1","D-1","C#-1","C-1")
combobox bounds(  3, 30, 50, 15), channel("TuneB"),     value(69), text("G9","F#9","F9","E9","D#9","D9","C#9","C9","B8","A#8","A8","G#8","G8","F#8","F8","E8","D#8","D8","C#8","C8","B7","A#7","A7","G#7","G7","F#7","F7","E7","D#7","D7","C#7","C7","B6","A#6","A6","G#6","G6","F#6","F6","E6","D#6","D6","C#6","C6","B5","A#5","A5","G#5","G5","F#5","F5","E5","D#5","D5","C#5","C5","B4","A#4","A4","G#4","G4","F#4","F4","E4","D#4","D4","C#4","C4","B3","A#3","A3","G#3","G3","F#3","F3","E3","D#3","D3","C#3","C3","B2","A#2","A2","G#2","G2","F#2","F2","E2","D#2","D2","C#2","C2","B1","A#1","A1","G#1","G1","F#1","F1","E1","D#1","D1","C#1","C1","B0","A#0","A0","G#0","G0","F#0","F0","E0","D#0","D0","C#0","C0","B-1","A#-1","A-1","G#-1","G-1","F#-1","F-1","E-1","D#-1","D-1","C#-1","C-1")
combobox bounds(  3, 50, 50, 15), channel("TuneG"),     value(73), text("G9","F#9","F9","E9","D#9","D9","C#9","C9","B8","A#8","A8","G#8","G8","F#8","F8","E8","D#8","D8","C#8","C8","B7","A#7","A7","G#7","G7","F#7","F7","E7","D#7","D7","C#7","C7","B6","A#6","A6","G#6","G6","F#6","F6","E6","D#6","D6","C#6","C6","B5","A#5","A5","G#5","G5","F#5","F5","E5","D#5","D5","C#5","C5","B4","A#4","A4","G#4","G4","F#4","F4","E4","D#4","D4","C#4","C4","B3","A#3","A3","G#3","G3","F#3","F3","E3","D#3","D3","C#3","C3","B2","A#2","A2","G#2","G2","F#2","F2","E2","D#2","D2","C#2","C2","B1","A#1","A1","G#1","G1","F#1","F1","E1","D#1","D1","C#1","C1","B0","A#0","A0","G#0","G0","F#0","F0","E0","D#0","D0","C#0","C0","B-1","A#-1","A-1","G#-1","G-1","F#-1","F-1","E-1","D#-1","D-1","C#-1","C-1")
combobox bounds(  3, 70, 50, 15), channel("TuneD"),     value(78), text("G9","F#9","F9","E9","D#9","D9","C#9","C9","B8","A#8","A8","G#8","G8","F#8","F8","E8","D#8","D8","C#8","C8","B7","A#7","A7","G#7","G7","F#7","F7","E7","D#7","D7","C#7","C7","B6","A#6","A6","G#6","G6","F#6","F6","E6","D#6","D6","C#6","C6","B5","A#5","A5","G#5","G5","F#5","F5","E5","D#5","D5","C#5","C5","B4","A#4","A4","G#4","G4","F#4","F4","E4","D#4","D4","C#4","C4","B3","A#3","A3","G#3","G3","F#3","F3","E3","D#3","D3","C#3","C3","B2","A#2","A2","G#2","G2","F#2","F2","E2","D#2","D2","C#2","C2","B1","A#1","A1","G#1","G1","F#1","F1","E1","D#1","D1","C#1","C1","B0","A#0","A0","G#0","G0","F#0","F0","E0","D#0","D0","C#0","C0","B-1","A#-1","A-1","G#-1","G-1","F#-1","F-1","E-1","D#-1","D-1","C#-1","C-1")
combobox bounds(  3, 90, 50, 15), channel("TuneA"),     value(83), text("G9","F#9","F9","E9","D#9","D9","C#9","C9","B8","A#8","A8","G#8","G8","F#8","F8","E8","D#8","D8","C#8","C8","B7","A#7","A7","G#7","G7","F#7","F7","E7","D#7","D7","C#7","C7","B6","A#6","A6","G#6","G6","F#6","F6","E6","D#6","D6","C#6","C6","B5","A#5","A5","G#5","G5","F#5","F5","E5","D#5","D5","C#5","C5","B4","A#4","A4","G#4","G4","F#4","F4","E4","D#4","D4","C#4","C4","B3","A#3","A3","G#3","G3","F#3","F3","E3","D#3","D3","C#3","C3","B2","A#2","A2","G#2","G2","F#2","F2","E2","D#2","D2","C#2","C2","B1","A#1","A1","G#1","G1","F#1","F1","E1","D#1","D1","C#1","C1","B0","A#0","A0","G#0","G0","F#0","F0","E0","D#0","D0","C#0","C0","B-1","A#-1","A-1","G#-1","G-1","F#-1","F-1","E-1","D#-1","D-1","C#-1","C-1")
combobox bounds(  3,110, 50, 15), channel("TuneLowE"),  value(88), text("G9","F#9","F9","E9","D#9","D9","C#9","C9","B8","A#8","A8","G#8","G8","F#8","F8","E8","D#8","D8","C#8","C8","B7","A#7","A7","G#7","G7","F#7","F7","E7","D#7","D7","C#7","C7","B6","A#6","A6","G#6","G6","F#6","F6","E6","D#6","D6","C#6","C6","B5","A#5","A5","G#5","G5","F#5","F5","E5","D#5","D5","C#5","C5","B4","A#4","A4","G#4","G4","F#4","F4","E4","D#4","D4","C#4","C4","B3","A#3","A3","G#3","G3","F#3","F3","E3","D#3","D3","C#3","C3","B2","A#2","A2","G#2","G2","F#2","F2","E2","D#2","D2","C#2","C2","B1","A#1","A1","G#1","G1","F#1","F1","E1","D#1","D1","C#1","C1","B0","A#0","A0","G#0","G0","F#0","F0","E0","D#0","D0","C#0","C0","B-1","A#-1","A-1","G#-1","G-1","F#-1","F-1","E-1","D#-1","D-1","C#-1","C-1")

checkbox bounds( 59, 10, 15, 15), channel("HighE_Mute"), colour(255, 20, 20), radiogroup(1)
checkbox bounds( 59, 30, 15, 15), channel("B_Mute"),     colour(255, 20, 20), radiogroup(2)
checkbox bounds( 59, 50, 15, 15), channel("G_Mute"),     colour(255, 20, 20), radiogroup(3)
checkbox bounds( 59, 70, 15, 15), channel("D_Mute"),     colour(255, 20, 20), radiogroup(4)
checkbox bounds( 59, 90, 15, 15), channel("A_Mute"),     colour(255, 20, 20), radiogroup(5)
checkbox bounds( 59,110, 15, 15), channel("LowE_Mute"),  colour(255, 20, 20), radiogroup(6), value(1)

checkbox bounds( 89, 10, 15, 15), channel("HighE0"), colour(  0,255,  0), radiogroup(1), value(1)
checkbox bounds( 89, 30, 15, 15), channel("B0"),     colour(  0,255,  0), radiogroup(2)
checkbox bounds( 89, 50, 15, 15), channel("G0"),     colour(  0,255,  0), radiogroup(3)
checkbox bounds( 89, 70, 15, 15), channel("D0"),     colour(  0,255,  0), radiogroup(4)
checkbox bounds( 89, 90, 15, 15), channel("A0"),     colour(  0,255,  0), radiogroup(5), value(1)
checkbox bounds( 89,110, 15, 15), channel("LowE0"),  colour(  0,255,  0), radiogroup(6)

; Neck (and Cabbage plant)
image bounds(110,  0,540,135), shape("sharp"), linethickness(0), plant("neck"), colour(white) {

checkbox bounds( 21, 10, 15, 15), channel("HighE1"),  colour:0(0,0,0,0), colour:1(250,250,  0), shape("ellipse"), radiogroup(1)
checkbox bounds( 73, 10, 15, 15), channel("HighE2"),  colour:0(0,0,0,0), colour:1(250,250,  0), shape("ellipse"), radiogroup(1)
checkbox bounds(122, 10, 15, 15), channel("HighE3"),  colour:0(0,0,0,0), colour:1(250,250,  0), shape("ellipse"), radiogroup(1)
checkbox bounds(169, 10, 15, 15), channel("HighE4"),  colour:0(0,0,0,0), colour:1(250,250,  0), shape("ellipse"), radiogroup(1)
checkbox bounds(215, 10, 15, 15), channel("HighE5"),  colour:0(0,0,0,0), colour:1(250,250,  0), shape("ellipse"), radiogroup(1)
checkbox bounds(259, 10, 15, 15), channel("HighE6"),  colour:0(0,0,0,0), colour:1(250,250,  0), shape("ellipse"), radiogroup(1)
checkbox bounds(301, 10, 15, 15), channel("HighE7"),  colour:0(0,0,0,0), colour:1(250,250,  0), shape("ellipse"), radiogroup(1)
checkbox bounds(340, 10, 15, 15), channel("HighE8"),  colour:0(0,0,0,0), colour:1(250,250,  0), shape("ellipse"), radiogroup(1)
checkbox bounds(380, 10, 15, 15), channel("HighE9"),  colour:0(0,0,0,0), colour:1(250,250,  0), shape("ellipse"), radiogroup(1)
checkbox bounds(415, 10, 15, 15), channel("HighE10"), colour:0(0,0,0,0), colour:1(250,250,  0), shape("ellipse"), radiogroup(1)
checkbox bounds(449, 10, 15, 15), channel("HighE11"), colour:0(0,0,0,0), colour:1(250,250,  0), shape("ellipse"), radiogroup(1)
checkbox bounds(481, 10, 15, 15), channel("HighE12"), colour:0(0,0,0,0), colour:1(250,250,  0), shape("ellipse"), radiogroup(1)
checkbox bounds(511, 10, 15, 15), channel("HighE13"), colour:0(0,0,0,0), colour:1(250,250,  0), shape("ellipse"), radiogroup(1)

checkbox bounds( 21, 30, 15, 15), channel("B1") , colour:0(0,0,0,0), colour:1(250,250,  0), shape("ellipse"), radiogroup(2)
checkbox bounds( 73, 30, 15, 15), channel("B2") , colour:0(0,0,0,0), colour:1(250,250,  0), shape("ellipse"), radiogroup(2), value(1)
checkbox bounds(122, 30, 15, 15), channel("B3") , colour:0(0,0,0,0), colour:1(250,250,  0), shape("ellipse"), radiogroup(2)
checkbox bounds(169, 30, 15, 15), channel("B4") , colour:0(0,0,0,0), colour:1(250,250,  0), shape("ellipse"), radiogroup(2)
checkbox bounds(215, 30, 15, 15), channel("B5") , colour:0(0,0,0,0), colour:1(250,250,  0), shape("ellipse"), radiogroup(2)
checkbox bounds(259, 30, 15, 15), channel("B6") , colour:0(0,0,0,0), colour:1(250,250,  0), shape("ellipse"), radiogroup(2)
checkbox bounds(301, 30, 15, 15), channel("B7") , colour:0(0,0,0,0), colour:1(250,250,  0), shape("ellipse"), radiogroup(2)
checkbox bounds(340, 30, 15, 15), channel("B8") , colour:0(0,0,0,0), colour:1(250,250,  0), shape("ellipse"), radiogroup(2)
checkbox bounds(380, 30, 15, 15), channel("B9") , colour:0(0,0,0,0), colour:1(250,250,  0), shape("ellipse"), radiogroup(2)
checkbox bounds(415, 30, 15, 15), channel("B10"), colour:0(0,0,0,0), colour:1(250,250,  0), shape("ellipse"), radiogroup(2)
checkbox bounds(449, 30, 15, 15), channel("B11"), colour:0(0,0,0,0), colour:1(250,250,  0), shape("ellipse"), radiogroup(2)
checkbox bounds(481, 30, 15, 15), channel("B12"), colour:0(0,0,0,0), colour:1(250,250,  0), shape("ellipse"), radiogroup(2)
checkbox bounds(511, 30, 15, 15), channel("B13"), colour:0(0,0,0,0), colour:1(250,250,  0), shape("ellipse"), radiogroup(2)

checkbox bounds( 21, 50, 15, 15), channel("G1"),  colour:0(0,0,0,0), colour:1(250,250,  0), shape("ellipse"), radiogroup(3)
checkbox bounds( 73, 50, 15, 15), channel("G2"),  colour:0(0,0,0,0), colour:1(250,250,  0), shape("ellipse"), radiogroup(3), value(1)    
checkbox bounds(122, 50, 15, 15), channel("G3"),  colour:0(0,0,0,0), colour:1(250,250,  0), shape("ellipse"), radiogroup(3)    
checkbox bounds(169, 50, 15, 15), channel("G4"),  colour:0(0,0,0,0), colour:1(250,250,  0), shape("ellipse"), radiogroup(3)    
checkbox bounds(215, 50, 15, 15), channel("G5"),  colour:0(0,0,0,0), colour:1(250,250,  0), shape("ellipse"), radiogroup(3)    
checkbox bounds(259, 50, 15, 15), channel("G6"),  colour:0(0,0,0,0), colour:1(250,250,  0), shape("ellipse"), radiogroup(3)
checkbox bounds(301, 50, 15, 15), channel("G7"),  colour:0(0,0,0,0), colour:1(250,250,  0), shape("ellipse"), radiogroup(3)
checkbox bounds(340, 50, 15, 15), channel("G8"),  colour:0(0,0,0,0), colour:1(250,250,  0), shape("ellipse"), radiogroup(3)
checkbox bounds(380, 50, 15, 15), channel("G9"),  colour:0(0,0,0,0), colour:1(250,250,  0), shape("ellipse"), radiogroup(3)
checkbox bounds(415, 50, 15, 15), channel("G10"), colour:0(0,0,0,0), colour:1(250,250,  0), shape("ellipse"), radiogroup(3)
checkbox bounds(449, 50, 15, 15), channel("G11"), colour:0(0,0,0,0), colour:1(250,250,  0), shape("ellipse"), radiogroup(3)
checkbox bounds(481, 50, 15, 15), channel("G12"), colour:0(0,0,0,0), colour:1(250,250,  0), shape("ellipse"), radiogroup(3)
checkbox bounds(511, 50, 15, 15), channel("G13"), colour:0(0,0,0,0), colour:1(250,250,  0), shape("ellipse"), radiogroup(3)

checkbox bounds( 21, 70, 15, 15), channel("D1"),  colour:0(0,0,0,0), colour:1(250,250,  0), shape("ellipse"), radiogroup(4)
checkbox bounds( 73, 70, 15, 15), channel("D2"),  colour:0(0,0,0,0), colour:1(250,250,  0), shape("ellipse"), radiogroup(4), value(1)    
checkbox bounds(122, 70, 15, 15), channel("D3"),  colour:0(0,0,0,0), colour:1(250,250,  0), shape("ellipse"), radiogroup(4)    
checkbox bounds(169, 70, 15, 15), channel("D4"),  colour:0(0,0,0,0), colour:1(250,250,  0), shape("ellipse"), radiogroup(4)    
checkbox bounds(215, 70, 15, 15), channel("D5"),  colour:0(0,0,0,0), colour:1(250,250,  0), shape("ellipse"), radiogroup(4)    
checkbox bounds(259, 70, 15, 15), channel("D6"),  colour:0(0,0,0,0), colour:1(250,250,  0), shape("ellipse"), radiogroup(4)
checkbox bounds(301, 70, 15, 15), channel("D7"),  colour:0(0,0,0,0), colour:1(250,250,  0), shape("ellipse"), radiogroup(4)
checkbox bounds(340, 70, 15, 15), channel("D8"),  colour:0(0,0,0,0), colour:1(250,250,  0), shape("ellipse"), radiogroup(4)
checkbox bounds(380, 70, 15, 15), channel("D9"),  colour:0(0,0,0,0), colour:1(250,250,  0), shape("ellipse"), radiogroup(4)
checkbox bounds(415, 70, 15, 15), channel("D10"), colour:0(0,0,0,0), colour:1(250,250,  0), shape("ellipse"), radiogroup(4)
checkbox bounds(449, 70, 15, 15), channel("D11"), colour:0(0,0,0,0), colour:1(250,250,  0), shape("ellipse"), radiogroup(4)
checkbox bounds(481, 70, 15, 15), channel("D12"), colour:0(0,0,0,0), colour:1(250,250,  0), shape("ellipse"), radiogroup(4)
checkbox bounds(511, 70, 15, 15), channel("D13"), colour:0(0,0,0,0), colour:1(250,250,  0), shape("ellipse"), radiogroup(4)

checkbox bounds( 21, 90, 15, 15), channel("A1"),  colour:0(0,0,0,0), colour:1(250,250,  0), shape("ellipse"), radiogroup(5)
checkbox bounds( 73, 90, 15, 15), channel("A2"),  colour:0(0,0,0,0), colour:1(250,250,  0), shape("ellipse"), radiogroup(5)    
checkbox bounds(122, 90, 15, 15), channel("A3"),  colour:0(0,0,0,0), colour:1(250,250,  0), shape("ellipse"), radiogroup(5)    
checkbox bounds(169, 90, 15, 15), channel("A4"),  colour:0(0,0,0,0), colour:1(250,250,  0), shape("ellipse"), radiogroup(5)    
checkbox bounds(215, 90, 15, 15), channel("A5"),  colour:0(0,0,0,0), colour:1(250,250,  0), shape("ellipse"), radiogroup(5)    
checkbox bounds(259, 90, 15, 15), channel("A6"),  colour:0(0,0,0,0), colour:1(250,250,  0), shape("ellipse"), radiogroup(5)
checkbox bounds(301, 90, 15, 15), channel("A7"),  colour:0(0,0,0,0), colour:1(250,250,  0), shape("ellipse"), radiogroup(5)
checkbox bounds(340, 90, 15, 15), channel("A8"),  colour:0(0,0,0,0), colour:1(250,250,  0), shape("ellipse"), radiogroup(5)
checkbox bounds(380, 90, 15, 15), channel("A9"),  colour:0(0,0,0,0), colour:1(250,250,  0), shape("ellipse"), radiogroup(5)
checkbox bounds(415, 90, 15, 15), channel("A10"), colour:0(0,0,0,0), colour:1(250,250,  0), shape("ellipse"), radiogroup(5)
checkbox bounds(449, 90, 15, 15), channel("A11"), colour:0(0,0,0,0), colour:1(250,250,  0), shape("ellipse"), radiogroup(5)
checkbox bounds(481, 90, 15, 15), channel("A12"), colour:0(0,0,0,0), colour:1(250,250,  0), shape("ellipse"), radiogroup(5)
checkbox bounds(511, 90, 15, 15), channel("A13"), colour:0(0,0,0,0), colour:1(250,250,  0), shape("ellipse"), radiogroup(5)

checkbox bounds( 21,110, 15, 15), channel("LowE1"),  colour:0(0,0,0,0), colour:1(250,250,  0), shape("ellipse"), radiogroup(6);52
checkbox bounds( 73,110, 15, 15), channel("LowE2"),  colour:0(0,0,0,0), colour:1(250,250,  0), shape("ellipse"), radiogroup(6);49
checkbox bounds(122,110, 15, 15), channel("LowE3"),  colour:0(0,0,0,0), colour:1(250,250,  0), shape("ellipse"), radiogroup(6);47
checkbox bounds(169,110, 15, 15), channel("LowE4"),  colour:0(0,0,0,0), colour:1(250,250,  0), shape("ellipse"), radiogroup(6);46
checkbox bounds(215,110, 15, 15), channel("LowE5"),  colour:0(0,0,0,0), colour:1(250,250,  0), shape("ellipse"), radiogroup(6);44
checkbox bounds(259,110, 15, 15), channel("LowE6"),  colour:0(0,0,0,0), colour:1(250,250,  0), shape("ellipse"), radiogroup(6);42
checkbox bounds(301,110, 15, 15), channel("LowE7"),  colour:0(0,0,0,0), colour:1(250,250,  0), shape("ellipse"), radiogroup(6);39
checkbox bounds(340,110, 15, 15), channel("LowE8"),  colour:0(0,0,0,0), colour:1(250,250,  0), shape("ellipse"), radiogroup(6);40
checkbox bounds(380,110, 15, 15), channel("LowE9"),  colour:0(0,0,0,0), colour:1(250,250,  0), shape("ellipse"), radiogroup(6);35
checkbox bounds(415,110, 15, 15), channel("LowE10"), colour:0(0,0,0,0), colour:1(250,250,  0), shape("ellipse"), radiogroup(6);34
checkbox bounds(449,110, 15, 15), channel("LowE11"), colour:0(0,0,0,0), colour:1(250,250,  0), shape("ellipse"), radiogroup(6);32
checkbox bounds(481,110, 15, 15), channel("LowE12"), colour:0(0,0,0,0), colour:1(250,250,  0), shape("ellipse"), radiogroup(6);30
checkbox bounds(511,110, 15, 15), channel("LowE13"), colour:0(0,0,0,0), colour:1(250,250,  0), shape("ellipse"), radiogroup(6);
                                                               
; dots
image bounds(122, 60, 15,15), colour(0,0,0), shape("ellipse")	; 3rd fret
image bounds(215, 60, 15,15), colour(0,0,0), shape("ellipse")	; 5th fret
image bounds(301, 60, 15,15), colour(0,0,0), shape("ellipse")	; 7th fret
image bounds(380, 60, 15,15), colour(0,0,0), shape("ellipse")	; 9th fret
image bounds(481, 30, 15,15), colour(0,0,0), shape("ellipse")	; 12th fret
image bounds(481, 90, 15,15), colour(0,0,0), shape("ellipse")	; 12th fret

; frets
line bounds( 54,  5,  2, 125), colour(50,50,50)	      	;	;
line bounds(105,  5,  2, 125), colour(50,50,50)		;3      ;
line bounds(152,  5,  2, 125), colour(50,50,50)		;       ;
line bounds(199,  5,  2, 125), colour(50,50,50)      	;5      ;
line bounds(244,  5,  2, 125), colour(50,50,50)       	;       ;
line bounds(287,  5,  2, 125), colour(50,50,50)       	;7      ;
line bounds(328,  5,  2, 125), colour(50,50,50)       	;       ;
line bounds(367,  5,  2, 125), colour(50,50,50)       	;9      ;
line bounds(404,  5,  2, 125), colour(50,50,50)       	;       ;
line bounds(439,  5,  2, 125), colour(50,50,50)       	;       ;
line bounds(472,  5,  2, 125), colour(50,50,50)       	;12     ;
line bounds(503,  5,  2, 125), colour(50,50,50)       	;       ;
line bounds(532,  5,  2, 125), colour(50,50,50)                 ;

}

button   bounds(  5,150,175, 50), text("strum"), channel("strum"), value(0), colour:0(50,50,50), colour:1(50,50,50), latched(0)
combobox bounds(185,175, 85, 25), channel("GuitarType"), value(1), text("6 String","12 String")
checkbox bounds(185,150,100, 20), channel("ReverseStrum"), text("Reverse") colour("lime")
hslider bounds(280,143,350, 40), text("Gap"),    colour("tan"), FontColour(195,126,  0), channel("gap"),   range(0,0.5,0.05,0.5,0.0001)
hslider bounds(280,168,350, 40), text("Damp"),   colour("tan"), FontColour(195,126,  0), channel("damp"),   range(0.01,0.99,0.17)
hslider bounds(280,193,350, 40), text("Pos."),   colour("tan"), FontColour(195,126,  0), channel("pos"),   range(0.0001, 0.5, 0.02, 0.5, 0.001)
filebutton bounds(5, 207, 98, 27), text("Store Chord", "Store Chord"), populate("*.snaps*", ""), colour:1(20, 20, 20), mode("snapshot"), channel("ChordSave")
combobox bounds(110, 207, 105, 26), populate("*.snaps", ""), channel("ChordChoose") 

image bounds(5, 242, 190, 19), colour(75, 85, 90, 50), plant("credit"){
label bounds(0.005, 0.2, 1, .7), text("Author: Iain McCurdy |2013|"), FontColour(195,126,  0)
}
</Cabbage>

<CsoundSynthesizer>
<CsOptions>
-n -dm0 -+rtmidi=NULL -M0 --midi-key-cps=4 --midi-velocity-amp=5
</CsOptions>
<CsInstruments>
sr = 44100
ksmps = 64
nchnls = 2
0dbfs=1

; A small amount of fixed random detuning improves the 12 string guitar sound
iDetuneRange	=	0.025
gidetune	ftgen	0, 0, 8, -21, 6, iDetuneRange	; table of detuning values for each string in a 12 string guitar configuration

instr	1

 ; macro for string set-up
 #define	STRING_SETUP(NAME)
 #
 k$NAME._Mute	chnget	"$NAME._Mute"
 kTune$NAME	chnget	"Tune$NAME"
 #

 $STRING_SETUP(HighE)
 $STRING_SETUP(B)
 $STRING_SETUP(G)
 $STRING_SETUP(D)
 $STRING_SETUP(A)
 $STRING_SETUP(LowE)




 ; macro that creates a value equivalent to the fretted note for that string
 #define SumString(NAME'N)
 #
 k$NAME.$N	chnget	"$NAME.$N"
 k$NAME._Sum	+=	k$NAME.$N * $N
 #



 ; macro that expands the previous macro for each fret and converts the result to cycles per second
 #define ExpandSummingMacro(NAME)
 #
 k$NAME._Sum	=	0 
 $SumString($NAME'0)
 $SumString($NAME'1)
 $SumString($NAME'2)
 $SumString($NAME'3)
 $SumString($NAME'4)
 $SumString($NAME'5)
 $SumString($NAME'6)
 $SumString($NAME'7)
 $SumString($NAME'8)
 $SumString($NAME'9)
 $SumString($NAME'10)
 $SumString($NAME'11)
 $SumString($NAME'12)
 $SumString($NAME'13) 
 kNoteNum$NAME	=	(128 - kTune$NAME) + k$NAME._Sum	; derive note number using open string tuning value
 kCps$NAME	=	cpsmidinn(kNoteNum$NAME)		; convert to cps 
 #
 $ExpandSummingMacro(HighE)
 $ExpandSummingMacro(B)
 $ExpandSummingMacro(G)
 $ExpandSummingMacro(D)
 $ExpandSummingMacro(A)
 $ExpandSummingMacro(LowE)



 kTimeElapsed	timeinsts					; elapsed time form the start of this note (this will be used to prevent premature triggering of the string) 
 
 ; macro for playing an individual string
 #define	PLAY_STRING(NAME'N)
 #
 if(k$NAME._Mute==1) kgoto SKIP$NAME				; if this is an open and muted string jump string playing code
 kRndStrt	random	0,kgap*0.15
 schedkwhen	ktrig,0,0,2.$N,kstart+kRndStrt,20,kCps$NAME,$N	; play string (instr 2)
 kstart	=	kstart + kgap					; increment start time
 SKIP$NAME:							; label
 if k$NAME._Mute==1 then
  turnoff2	2.$N,4,1						; turn off string  
 endif
 if changed(kCps$NAME)==1 && timeinstk()>1 then						; ...
  turnoff2	2.$N,4,1						; turn off string
  event	"i",2.$N,0,20,kCps$NAME,$N
 endif
 #
 
 kstrum	chnget	"strum"						; 'strum' button
 ktrig	trigger	kstrum,0.5,0					; if 'strum' button has been clicked...
 
 if ktrig==1 then						; ...
  turnoff2	2,0,1						; turn off all playing strings (in preparation for the new chord)
 endif
 
 kgap	chnget	"gap"
 kreverse	chnget	"ReverseStrum"
 if kreverse==0 then
  kstart	=	0					; start time (for first string plucked)
 else
  kstart	=	kgap*5
  kgap		=	-kgap
 endif

 $PLAY_STRING(LowE'1)
 $PLAY_STRING(A'2)
 $PLAY_STRING(D'3)
 $PLAY_STRING(G'4)
 $PLAY_STRING(B'5)
 $PLAY_STRING(HighE'6)


endin














instr	2	; string pluck
 kdamp	chnget	"damp"	; damping
 iplk	chnget	"pos"	; pluck position
 iamp	random	0.1,0.2
 
 iGuitarType	chnget	"GuitarType"
  if(iGuitarType==2) then					; if 12 string guitar...
  kmvt1	jspline		0.15,0.2,0.4				; a bit of random pick-up movement animates the tone of the guitar	
  kmvt2	jspline		0.15,0.2,0.4		
  idtn	table		p5,gidetune				; detuning value. used in 12 string guitar only.
  if p5<5 then							; strings 1 - 4 are in octaves in a 12-string guitar
   a1	wgpluck2	iplk, iamp/2, p4,          0.2+kmvt1, kdamp
   a2	wgpluck2	iplk, iamp/2, p4*(2+idtn), 0.2+kmvt2, kdamp
  else								; strings 5 & 6 are in unison in a 12-string guitar
   a1	wgpluck2	iplk, iamp/2, p4,          0.2         +kmvt1, kdamp
   a2	wgpluck2	iplk, iamp/2, p4*(1+idtn), 0.2+rnd(0.5)+kmvt2, kdamp
  endif
   a2	delay	a2,0.01+rnd(0.01)				; slight delay to second string
  asig	=	a1+a2
 else									; otherwise 6 string
  kmvt	jspline		0.15,0.2,0.4		
  asig	wgpluck2	iplk, iamp, p4,   0.2+kmvt,          kdamp
 endif
 
 aenv		linsegr	0, 0.0008, 1, p3-0.1008, 0, 0.05, 0	; amplitude envelope. Attack removes excessive 'click'. Decays to nothing across note duration (p3). Short release stage prevents click if a note is interrupted.
 
 /* FILTERING TO IMPROVE THE SOUND */
 af1	resonz	asig,200,200
 af2	resonz	asig,550,550
 af3	resonz	asig,2100,2100
 asig	sum	af1,af2*5,af3
 asig	=	asig/200
 /* ------------------------------ */
 
 asig	=	asig*aenv
 	outs	asig, asig
endin




</CsInstruments>  

<CsScore>
i 1 0 [3600*24*7]
</CsScore>

</CsoundSynthesizer>