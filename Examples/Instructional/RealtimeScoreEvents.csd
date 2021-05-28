
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; RealtimeScoreEvents.csd
; Sending traditional Csound score events typed in using Cabbage's textedit widget

; Written by Iain McCurdy, 2015. Borrowing from Rory Walsh's 'TextEditor.csd' example.

<Cabbage>
form caption("Realtime Score Events") size(800, 365), pluginId("RTSc"), guiRefresh(16)

label bounds(5, 5, 390, 15), text("Enter a score event for instrument 2 or 3..."), align("left"),  fontColour("white")
texteditor bounds(5, 25, 390, 20), channel("ScoreEvent"), text(""), color(20, 20, 20), fontColour(0, 0, 0, 255), 
button     bounds(5,50,80,20), text("Send Again","Send Again"), channel("SendAgain"), value(0)
csoundoutput bounds(5, 75, 390, 285)

textBox bounds(405, 5,390,350), file("RealtimeScoreEvents.txt"), colour("white"), fontColour("black")
</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-d -n
</CsOptions>

<CsInstruments>

; sr set by host
ksmps    =    32
nchnls    =    2
0dbfs    =    1

gisine    ftgen    0,0,4096,10,1

instr   1
 Sstr    chnget    "ScoreEvent"        ; read in a string from 'textedit' widget
 ktrigS    changed    Sstr            ; generate a trigger if some text has been sent (text changed or added and enter has been struck)
 kSendAgain    chnget    "SendAgain"
 ktrig    changed    kSendAgain
 scoreline    Sstr,ktrigS+ktrig    ; send the text as a score event. Hopefully a valid instrument call.
endin

instr    2
ifreq    =    p4

asig    poscil    0.1,ifreq,-1
     outs    asig,asig
endin

instr    3
ifreq    =    p4
indx    =    p5
iratio    =    p6

aenv    expon    0.1,p3,0.0001
kndx    expon    indx,p3,indx*(0.001)
asig    foscili    aenv,ifreq,1,iratio,kndx-0.001,gisine
     outs    asig,asig
endin
                
</CsInstruments>

<CsScore>
i 1 0 [3600*24*7]
</CsScore>

</CsoundSynthesizer>