
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; GEN20.csd
; Written by Iain McCurdy, 2014

<Cabbage>
form caption("GEN20"), size(410, 230), pluginId("gn20"), colour("20,70,170,150") style("legacy")

gentable bounds(  5,  5, 400, 120), tableNumber(1), identChannel("table1"), zoom(-1), ampRange(0,1,1), tableColour("LightSlateGrey"), zoom(-1), tablebackgroundColour("white"), fill(0), outlineThickness(2) tableGridColour(220,220,220,20)

combobox bounds(130, 130, 175,20), channel("window"), value(1), text("Hamming","Hanning","Bartlett [Triangle]","Blackman [3-term]","Blackman-Harris [4-term]","Gaussian","Kaiser","Rectangle","Sync.")

hslider  bounds(  5,150,340, 30), text("Option"), channel("opt"), range(0, 10.00, 1, 0.5), valueTextBox(1), textBox(1), trackerColour("yellow"), fontColour("white"), textColour("White")
label    bounds(  3,172,110, 11), text("[Gaussian & Kaiser]"),  fontColour("white")
checkbox bounds(345,158, 55, 13), text("x 100") channel("x100"), colour("yellow"), fontColour("white"),  value(0)

image bounds(-5,-125,4,4), colour("red"), identChannel("scrubber"), shape("sharp")

nslider  bounds(  5,190, 50, 30), text("Index"), channel("ndx"), range(0, 4095,1024, 1,1),      fontColour("white")
nslider  bounds( 65,190, 50, 30), text("Value"), channel("val"), range(0,    1, 0, 1,0.0001), fontColour("white")

checkbox bounds(160,200,100, 13), text("Tone On/Off") channel("ToneOnOff"), colour("yellow"), fontColour("white"),  value(0)

</Cabbage>
                    
<CsoundSynthesizer>

<CsOptions>   
-dm0 -n -+rtmidi=null -M0
</CsOptions>

<CsInstruments>

;sr is set by the host
ksmps         =     32    ; NUMBER OF AUDIO SAMPLES IN EACH CONTROL CYCLE
nchnls         =     2    ; NUMBER OF CHANNELS (1=MONO)
0dbfs        =    1    ; MAXIMUM AMPLITUDE
            
giwindow    ftgen    1,0,4096,20,1,1,1

instr    1
    iTabSize    =    ftlen(giwindow)
    
    ; read in widgets
    gkwindow    chnget    "window"
    gkndx        chnget    "ndx"
    gkndx        init    1024
    gkwindow    init    1
    gkopt    chnget    "opt"
    gkopt    init    1
    gkx100    chnget    "x100"
    gkToneOnOff    chnget    "ToneOnOff"
    
    ktrig1    changed    gkwindow
    ktrig2    changed    gkopt,gkx100
    if ktrig1==1 || ( (ktrig2==1&&(gkwindow==6||gkwindow==7))) then
     reinit UPDATE
    endif
    UPDATE:
     giwindow    ftgen    1,0,ftlen(giwindow),20,i(gkwindow),1,i(gkopt)* ((i(gkx100)*99)+1)
    rireturn
    if ktrig1==1||ktrig2==1 then
     chnset    "tableNumber(1)", "table1"    ; update table display    
    endif
    
    ; Read index input and print value
    kval    table    gkndx,giwindow
    if changed(kval)==1||changed(gkndx)==1 then
     kval    table    gkndx,giwindow
         chnset    kval,"val"
     kxpos    =    5 + (400 * (gkndx/iTabSize))
     kypos    =    5 + (120 * (1-kval))     
     Smess    sprintfk    "bounds(%d,%d,2,%d)",kxpos-2,kypos+1,125-kypos-2
         chnset        Smess,"scrubber"
    endif
    
    ; CREATE A SOUND
    aenv    poscil    0.05*gkToneOnOff,1,giwindow
    asig    vco2    1,440,4,0.5
    asig    *=        aenv
            outs    asig,asig
endin

</CsInstruments>

<CsScore>
i 1 0 [3600*24*7]
e
</CsScore>

</CsoundSynthesizer>
