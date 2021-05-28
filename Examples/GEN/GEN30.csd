
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; GEN30.csd
; Written by Iain McCurdy, 2014
; 
; GEN30 creates band limited versions of input harmonic waveforms. 
; Typically an input waveform is created using GEN07.
; The user can then define the limits, in terms of partial numbers, in the output waveform.
; If 'Interpolation' is on fractional settings for minimum and maximum partial numbers will fade the partials at the extremities of the spectrum in or out smoothly. 
; The input waveform is displayed in green, the output in yellow.

<Cabbage>
form caption("GEN30"), size(410, 290), pluginId("gn30"), colour(80, 30, 50)
gentable bounds(  5,  5, 400, 120), identChannel("table1"), tableNumber(1,2), tableColour("lime","yellow"), tableColour:0("lime"), tableColour:1("yellow"), fill(0), outlineThickness(2), tableGridColour(0,0,0,0)

label    bounds(348,130, 60, 10), text("SOURCE"), fontColour("lime"), align("right")
label    bounds(348,140, 60, 10), text("RESULT"), fontColour("yellow"), align("right")

label    bounds(  5, 136, 80, 12), text("Source")
combobox bounds(  5, 150, 80, 20), channel("src"), value(1), text("Sawtooth","Square","Triangle","Pulse")

rslider  bounds( 90,140, 80, 80), channel("minh"), text("Min.Harm."), textBox(1), valueTextBox(1), range(1.00, 100, 1), TrackerColour("yellow")
rslider  bounds(170,140, 80, 80), channel("maxh"), text("Max.Harm."), textBox(1), valueTextBox(1), range(1.00, 100, 20), TrackerColour("yellow")
rslider  bounds( 20,170, 50, 50), channel("pw"), text("P.W."), textBox(1), valueTextBox(1), range(1, 2048, 16,1,1), TrackerColour("yellow"), identChannel("pwID")
label    bounds(250,138, 80, 12), text("Listen to:")
button   bounds(250,152, 80, 18), text("SOURCE","RESULT"), channel("ListenTo"), value(1), fontColour:0("yellow"), fontColour:1("lime")
checkbox bounds(250,172,100, 14), channel("OnOff"),  value(0), text("Tone On/Off"), colour("yellow")
checkbox bounds(250,188,100, 14), channel("interp"),  value(1), text("Interpolate")
checkbox bounds(250,204,100, 14), channel("norm"),  value(1), text("Normalise"), colour("LightBlue")
hslider  bounds(  5,220,400, 30), channel("frq"), text("Freq."), textBox(1), valueTextBox(1), range(1, 5000, 200,0.5,1), TrackerColour("yellow")
hslider  bounds(  5,250,400, 30), channel("amp"), text("Ampl."), textBox(1), valueTextBox(1), range(0,    1, 0.1), TrackerColour("yellow")
</Cabbage>
                    
<CsoundSynthesizer>

<CsOptions>   
-dm0 -n -+rtmidi=NULL -M0
</CsOptions>

<CsInstruments>

; sr set by host
ksmps         =     32    ; NUMBER OF AUDIO SAMPLES IN EACH CONTROL CYCLE
nchnls         =     2    ; NUMBER OF CHANNELS (1=MONO)
0dbfs        =    1    ; MAXIMUM AMPLITUDE

gisaw    ftgen    101,0,4096,7,1,4096,-1
gisq    ftgen    102,0,4096,7,1,2048,1,0,-1,2048,-1
gitri    ftgen    103,0,4096,7,0,1024,1,2048,-1,1024,0
gipls    ftgen    104,0,4096,7,1,16,1,0,0,4096-16,0

gidisp    ftgen    2,0,4096,7,1,4096,-1

instr    1
 gkminh        chnget    "minh"
 gkmaxh        chnget    "maxh"
 gkinterp    chnget    "interp"
 gknorm        chnget    "norm"
 gksrc        chnget    "src"
 gkOnOff    chnget    "OnOff"
 gkfrq        chnget    "frq"
 gkamp        chnget    "amp"
 gkpw        chnget    "pw"
 gkListenTo    chnget    "ListenTo"
 gksrc    init    1
 kporttime    linseg    0,0.001,0.05
 gkfrq        portk    gkfrq, kporttime
 gkamp        portk    gkamp, kporttime
   
 if ( (gkfrq*gkmaxh) >= (sr/2) ) then
  chnset    gkmaxh-1,"maxh"
 endif 
    
 ; SHOW OR HIDE WIDGETS -------------------------------------
 kchange    changed    gksrc
 if(kchange==1) then
    if gksrc==4 then
     chnset "visible(1)", "pwID"
    else
     chnset "visible(0)", "pwID"
    endif
 endif
; -----------------------------------------------------------

 ktrig    changed    gkminh,gkmaxh,gksrc,gkinterp,gknorm, gkpw    ; If any of the input arguments are changed generate a trigger (momentary '1').
 if ktrig==1 then                    ; If a trigger has been generated...
  reinit    REBUILD_WAVEFORM            ; ...reinitialise from label
 endif
 REBUILD_WAVEFORM:
 inorm    =    (i(gknorm)==1?1:-1)
 gipls    ftgen    104, 0, 4096, 7, 1,i(gkpw),1,0,0,4096-i(gkpw),0
 ifn    ftgen    1, 0, 4096, 30*inorm, gisaw+i(gksrc)-1, i(gkminh), i(gkmaxh),sr,i(gkinterp)    ; generate a waveform based on chosen source waveform with user-set modifications
     tableicopy gidisp, gisaw+i(gksrc)-1
 rireturn

 if ktrig==1 then
  chnset    "tableNumber(2,1)", "table1"    ; update table display    
 endif

 if gkOnOff==1 then                    ; if 'Play Tone' is activated
  if gkListenTo==0 then
   asig    oscili    gkamp, gkfrq, 2                ; audio oscillator using GEN30 waveform
  else
   asig    oscili    gkamp, gkfrq, 1                ; audio oscillator using GEN30 waveform
  endif  
     outs    asig,asig
 endif
endin

</CsInstruments>

<CsScore>
i 1 0 [3600*24*7]
</CsScore>

</CsoundSynthesizer>
