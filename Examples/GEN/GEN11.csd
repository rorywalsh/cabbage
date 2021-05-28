
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; GEN11.csd
; Written by Iain McCurdy, 2018
; Demonstration of GEN11
; Create a wave and then play it using the keyboard

<Cabbage>
form caption("GEN11"), size(410, 340), pluginId("gn10"), colour(20,70,170, 50)

gentable bounds(  5,  5, 400, 120), identChannel("table1"), tableNumber(1), tableColour(100,100,250), zoom(-1)

rslider bounds( 40,135,110,110), channel("NHarms"), text("N.Harms"), valueTextBox(1), textBox(1), range(1, 40, 1, 1, 1), colour(100,100,250,200)
rslider bounds(150,135,110,110), channel("LHarm"), text("Lowest"), valueTextBox(1), textBox(1), range(1, 40, 1, 1, 1), colour(100,100,250,200)
rslider bounds(260,135,110,110), channel("Pow"), text("Power"), valueTextBox(1), textBox(1), range(0, 2.00, 1), colour(100,100,250,200)

keyboard bounds(  0,260,410, 80)
</Cabbage>
                    
<CsoundSynthesizer>

<CsOptions>   
-dm0 -n -+rtmidi=NULL -M0
</CsOptions>

<CsInstruments>

; sr set by host
ksmps     =     32        ; NUMBER OF AUDIO SAMPLES IN EACH CONTROL CYCLE
nchnls     =     2        ; NUMBER OF CHANNELS (1=MONO)
0dbfs    =    1        ; MAXIMUM AMPLITUDE
        massign    0,3    ; send all midi notes to instr 3 
            
giwave    ftgen    1,0, 4096,11, 1    ; GEN10 generated wave

instr    1
    ; read in widgets
    gkNHarms    chnget    "NHarms"
    gkLHarm     chnget    "LHarm"
    gkPow        chnget    "Pow"
    gkNHarms    init    1
    
    ; generate a trigger if any of the input variables changes
    ktrig    changed    gkNHarms,gkLHarm,gkPow
    if ktrig==1 then
     reinit UPDATE
    endif
    UPDATE:    
    giwave    ftgen    1, 0, 4096, 11, i(gkNHarms), i(gkLHarm), i(gkPow)
    rireturn
    if ktrig==1 then
     chnset    "tableNumber(1)", "table1"    ; update table display    
    endif
endin

instr    3
    icps    cpsmidi                ; CPS from midi note played
    iamp    ampmidi    0.5            ; amplitude from midi note velocity 
    a1    oscili    iamp,icps,giwave    ; audio oscillator read GEN10 wave created
    aenv    linsegr    0,0.01,1,0.1,0        ; amplitude envelope to prevent clicks
    a1    =    a1 * aenv        ; apply envelope
        outs    a1, a1            ; send audio to outputs
endin

</CsInstruments>

<CsScore>
i 1 0 [3600*24*7]
</CsScore>

</CsoundSynthesizer>