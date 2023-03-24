
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; GEN21.csd
; Written by Iain McCurdy

; demonstration of the GEN21 opcode, used to create various random distributions

; changing the value given for giTabSize (in instrument header area) and the value for 'drawmode' (part of the table widget) can change the appearance of the distribution.

; display is set between -1 and 1. Certain distributions may require reduction of the 'level' control in order for them to be fully displayed

<Cabbage>
form caption("GEN21"), size(410, 220), pluginId("gn21"), colour( 40,110, 80)

gentable bounds(  5,  5, 400, 120), tableNumber(1), tableColour("lime"), identChannel("table1"), ampRange(-1,1,1), fill(0)

combobox bounds( 10, 130, 200,20), channel("dist"), value(1), text("Uniform [pos.]","Linear [pos.]","Triangular [pos. and neg.]","Exponential [pos.]","Biexponential [pos. and neg.]","Gaussian [pos. and neg.]","Cauchy [pos. and neg.]","Cauchy [pos.]","Beta","Weibull","Poisson")
checkbox bounds( 10, 160, 100,14), channel("AudOnOff"), text("Audio On/Off"), fontColour:0("white"), fontColour:1("white")
rslider  bounds(210, 130, 80, 80), text("Level"), channel("level"), range(0, 1.00, 1,0.5,0.001), textBox(1), valueTextBox(1), colour(20, 90, 60), trackerColour("yellow"), fontColour("white"), textColour("White")
rslider  bounds(270, 130, 80, 80), text("Arg.1"), channel("arg1"),  range(0, 1.00, 1), textBox(1), valueTextBox(1), colour(20, 90, 60), trackerColour("yellow"), fontColour("white"), visible(0), identChannel("ident_arg1"), textColour("White")
rslider  bounds(330, 130, 80, 80), text("Arg.2"), channel("arg2"),  range(0, 1.00, 1), textBox(1), valueTextBox(1), colour(20, 90, 60), trackerColour("yellow"), fontColour("white"), visible(0), identChannel("ident_arg2"), textColour("White")

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

giTabSize    =    128    

gi_ ftgen    1,0,giTabSize,10,1

instr    1
    ; read in widgets
    gkdist    chnget    "dist"
    gklevel    chnget    "level"
    gkarg1    chnget    "arg1"
    gkarg2    chnget    "arg2"
    gkdist    init    1
    
    ktrig    changed    gkdist,gklevel,gkarg1,gkarg2
    if ktrig==1 then
     reinit UPDATE
    endif
    UPDATE:
    if i(gkdist)==9 then
     chnset "visible(1)", "ident_arg1"
     chnset "visible(1)", "ident_arg2"
    elseif i(gkdist)==10 then
     chnset "visible(1)", "ident_arg1"
     chnset "visible(0)", "ident_arg2"
    else
     chnset "visible(0)", "ident_arg1"
     chnset "visible(0)", "ident_arg2"
    endif
    idist    ftgen    1,0,giTabSize,-21,i(gkdist),i(gklevel),i(gkarg1),i(gkarg2)
    iaud    ftgen    2,0,2^18,-21,i(gkdist),i(gklevel),i(gkarg1),i(gkarg2)
    rireturn
    if ktrig==1 then
     chnset    "tableNumber(1)", "table1"    ; update table display    
    endif
    
    kAudOnOff    chnget    "AudOnOff"
    asig    poscil    0.1*kAudOnOff, sr/(2^18), iaud
            outs    asig,asig
endin

</CsInstruments>

<CsScore>
i 1 0 [3600*24*7]
e
</CsScore>

</CsoundSynthesizer>
