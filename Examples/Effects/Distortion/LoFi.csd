
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; LoFi.csd
; Written by Iain McCurdy, 2012.

<Cabbage>
form size(230, 120), caption("Lo Fi"), pluginId("lofi"), colour(57,70,75)
checkbox bounds(  5,5,80,15), channel("TestTone"), text("Test Tone") fontColour("White")
rslider bounds(  5, 31, 80, 80), text("Bits"),     channel("bits"),  range(1, 16, 16),        textColour("White"),    colour("orange"), trackerColour("brown"), outlineColour("grey")
rslider bounds( 75,  7, 80, 80), text("Foldover"), channel("fold"),  range(1, 1024, 0, 0.25), textColour("White"),    colour("orange"), trackerColour("brown"), outlineColour("grey")
rslider bounds(145, 31, 80, 80), text("Level"),    channel("level"), range(0, 1.00, 1),       textColour("White"),    colour("orange"), trackerColour("brown"), outlineColour("grey")
}
</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-d -n
</CsOptions>
<CsInstruments>

;sr is set by the host
ksmps = 32
nchnls = 2
0dbfs = 1

;Iain McCurdy, 2012
;http://iainmccurdy.org/csound.html


opcode  LoFi,a,akk
    ain,kbits,kfold xin                                 ; READ IN INPUT ARGUMENTS
    kvalues pow     2, kbits                            ; RAISES 2 TO THE POWER OF kbitdepth. THE OUTPUT VALUE REPRESENTS THE NUMBER OF POSSIBLE VALUES AT THAT PARTICULAR BIT DEPTH
    aout    =       (int((ain/0dbfs)*kvalues))/kvalues  ; BIT DEPTH REDUCE AUDIO SIGNAL
    aout    fold    aout, kfold                         ; APPLY SAMPLING RATE FOLDOVER
            xout    aout                                ; SEND AUDIO BACK TO CALLER INSTRUMENT
endop

instr 1 
kTestTone chnget  "TestTone"
kbits     chnget  "bits"
kfold     chnget  "fold"
klevel    chnget  "level"
if kTestTone==0 then
 a1,a2     ins
else
 a1       poscil  0.5,330
 a2       =       a1
endif
kporttime linseg  0, 0.001, 0.01
kfold     portk   kfold, kporttime
a1        LoFi    a1, kbits * 0.6, kfold
a2        LoFi    a2, kbits * 0.6, kfold
a1        =       a1 * klevel
a2        =       a2 * klevel
          outs    a1, a2
endin

</CsInstruments>

<CsScore>
i 1 0 [60*60*24*7]
</CsScore>

</CsoundSynthesizer>