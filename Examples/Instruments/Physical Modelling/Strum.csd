
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; Strum.csd
; Written by Iain McCurdy, 2014

; A moving xypad pointer plucks imaginary strings stretched across its surface.
; Movement in the y direction therefore controls pitch. 
; Movement in the x direction varies pluck position and panning location.

; Notes        -    number of notes/strings/subdivisions triggered along the y-axis of the xypad
; Spacing     -    spacing between adjacent notes in semitones or fractions thereof
; Offset    -    global note offset (note number)
; Shift        -    shift in semitones applied to notes when triggered by a descending widget
; Polyphony    -    polyphony  limit (to preserve CPU buffer). Oldest notes are removed if polyphony limit is exceeded.
; Level        -    output level

<Cabbage>
form caption("Strum") size(320,400), pluginId("strm"), guiRefresh(128)

image    bounds(  0,  0,320,300), colour(250,230,255),    shape("sharp"), outlineColour("black"), outlineThickness(1)
xypad    bounds(  0,  0,320,300), colour(220,200,250), channel("x", "y"), rangeX(0, 1.00, 0), rangeY(0, 1, 0), colour("DarkSlateGrey"), textColour("DarkSlateGrey")

image    bounds(  0,300,320,100), colour(220,200,250),    shape("sharp"), outlineColour("black"), outlineThickness(1)
rslider  bounds(  0,305, 70, 70), channel("notes"),     text("Notes"),    valueTextBox(1), textBox(1), range(1, 50, 5,1,1),         colour("SlateGrey"), trackerColour(20,40,70), textColour(20,40,70), fontColour("DarkSlateGrey")
rslider  bounds( 50,305, 70, 70), channel("spacing"),   text("Spacing"),  valueTextBox(1), textBox(1), range(0.25, 6.00, 1,1,0.25), colour("SlateGrey"), trackerColour(20,40,70), textColour(20,40,70), fontColour("DarkSlateGrey")
rslider  bounds(100,305, 70, 70), channel("offset"),    text("Offset"),   valueTextBox(1), textBox(1), range(24, 72, 48,1,1),       colour("SlateGrey"), trackerColour(20,40,70), textColour(20,40,70), fontColour("DarkSlateGrey")
rslider  bounds(150,305, 70, 70), channel("shift"),     text("Shift"),    valueTextBox(1), textBox(1), range(-24, 24, 12,1,0.25),   colour("SlateGrey"), trackerColour(20,40,70), textColour(20,40,70), fontColour("DarkSlateGrey")
rslider  bounds(200,305, 70, 70), channel("PolyLimit"), text("Polyphony"),valueTextBox(1), textBox(1), range(1, 24, 3,1,1),         colour("SlateGrey"), trackerColour(20,40,70), textColour(20,40,70), fontColour("DarkSlateGrey")
rslider  bounds(250,305, 70, 70), channel("lev"),       text("Level"),    valueTextBox(1), textBox(1), range(0, 1.00, 0.3),         colour("SlateGrey"), trackerColour(20,40,70), textColour(20,40,70), fontColour("DarkSlateGrey")

checkbox bounds( 25,380,110, 16), text("Include Edges"), colour("yellow"), channel("IncludeEdges"),  value(0), fontColour:0("DarkSlateGrey"), fontColour:1("DarkSlateGrey")

</Cabbage>

<CsoundSynthesizer>
<CsOptions>
-dm0 -n -+rtmidi=NULL -M0
</CsOptions>

<CsInstruments>

; sr set by host
ksmps     =     16
nchnls     =     2
0dbfs    =    1

gisine        ftgen    0,0,4096,10,1    ; sine wave
gaRvbSendL,gaRvbSendR    init    0    ; initialise reverb stereo send global audio signal
gkactive     init     0        ; number of active notes

instr        1
 kx        chnget    "x"
 ky        chnget    "y"
 knotes        chnget    "notes"
 gkoffset    chnget    "offset"        ; note number offset
 gkspacing    chnget    "spacing"        ; note spacing (in semitones or fraction thereof
 gkshift    chnget    "shift"            ; note shift applied to downwardly triggered notes
 gkPolyLimit    chnget    "PolyLimit"        ; polyphony limit
 gklev        chnget    "lev" 
 kIncludeEdges    chnget    "IncludeEdges"
 ktrig1    =    0                ; ktrig1 and ktrig2 can be added to ktrig to include triggerings at the boundaries
 ktrig2    =    0
 kOldy    init    0

 kVertDir    init    0            ; set initial (arbitrary) state for vertical direction (0=bottom to top, 1=top to bottom)
 ; UPPER AND LOWER BARRIERS 
 if kVertDir==0 then                ; if vertical direction is bottom to top...
  ktrig1    trigger    ky,kOldy,1        ; if current y position is less than previous y position - i.e. top edge barrier has been bounced against - generate a trigger
  if ktrig1==1 then                ; if bounce against top edge has been detected...
   kVertDir    =    1            ; change direction
  endif
 elseif kVertDir==1 then            ; if vertical direction is top to bottom...
  ktrig2    trigger    ky,kOldy,0        ; if current y position is greater than previous y position - i.e. bottom edge barrier has been bounced against - generate a trigger
  if ktrig2==1 then                ; if bounce against bottom edge has been detected...
   kVertDir    =    0            ; change direction
  endif
 endif 
 kOldy    =    ky                ; previous y position equals current y position (for the next k pass)

 ky    =    ky * (knotes + 1)
     
 ktrig    changed    int(ky)
 if kIncludeEdges==1 then
  ktrig    =    ktrig+ktrig1+ktrig2
 endif
     schedkwhen    ktrig,0,0,2,0,2,round(ky),kx,kVertDir
endin


; SCALE FOR REFLECTION DEPENDENT UPON MIDI NOTE NUMBER (LESS DAMPING FOR HIGHER NOTES)
giScal1    ftgen    0,0,128, -27,  0, 0.9, 24, 0.9, 36, 0.85, 48, 0.75, 60, 0.65, 72, 0.35, 84, 0.001, 96, 0.001, 127        ; longer sustaining notes / less damping
giScal2    ftgen    0,0,128, -27,  0, 0.983, 24, 0.983, 36, 0.971, 48, 0.939, 60, 0.855, 72, 0.747, 84, 0.364, 96, 0.001, 127    ; shorter sustaining notes / more damping

instr    2    ; longer harmonic pluck sound

 ; polyphony control
 gkactive init i(gkactive) + 1        ;INCREMENT NOTE COUNTER
 if gkactive>i(gkPolyLimit) then    ;IF POLYPHONY IS EXCEEDED (THROUGH THE ADDITION OF NEW NOTE)
  turnoff                ;REMOVE THIS NOTE
 endif
 krel release                ;IF NOTE HELD = 0, IF NOTE RELEASED = 1
 ktrig trigger krel,0.5,0        ;WHEN RELEASE FLAG CROSSES 0.5 UPWARDS, I.E. NOTE HAS BEEN RELEASED...    
 if ktrig==1 then        
  gkactive = gkactive - 1        ;...DECREMENT ACTIVE NOTES COUNTER
 endif

 aRelEnv    linsegr    1,0.2,0

 inum    limit    i(gkoffset) + (p4 * i(gkspacing)) + (p6 * i(gkshift)),0,127
 
 aEnv    linsegr    0, 0.005, 1, p3-0.105, 1, 0.1, 0        ; amplitude envelope
 iPlk    =    p5                        ; point at which to pluck the string
 iDtn    random    -0.05, 0.05                    ; random detune
 irefl    table    inum, giScal1                    ; read reflection value from giScal table according to note number  
 aSig    wgpluck2  0.58, gklev, cpsmidinn(inum+iDtn), iPlk, irefl    ; generate Karplus-Strong plucked string audio 
 kcf    expon    cpsoct(rnd(6)+6),p3,50                ; filter cutoff frequency envelope
 aSig    clfilt    aSig, kcf, 0, 2                    ; butterworth lowpass filter    
 aL,aR pan2      aSig * aEnv * aRelEnv, p5                ; random panning   
       outs      aL, aR                        ; send audio to outputs
 gaRvbSendL    =    gaRvbSendL + aL*0.1
 gaRvbSendR    =    gaRvbSendR + aR*0.1
endin

instr    201    ; reverb instrument
    aL,aR    reverbsc    gaRvbSendL,gaRvbSendR,0.85,7000
        outs        aL,aR
        clear        gaRvbSendL,gaRvbSendR
endin

</CsInstruments>

<CsScore>
i 1 0 [3600*24*7]    ; sense collisions with barriers
i 201 0 [3600*24*7]    ; reverb instrument
</CsScore>

</CsoundSynthesizer>
